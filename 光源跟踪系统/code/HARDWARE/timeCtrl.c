/*
 * @Author: 
 * @Date: 2021-04-10 20:40:34
 * @LastEditTime: 2021-05-18 10:45:53
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \ZET6\HARDWARE\timeCtrl.c
 */
#include "timeCtrl.h"
#include "gps.h"
#include "oled.h"
#include "timeCtrl.h"

// 以下变量都是数值型的整型变量
unsigned char monthGPS = 0;  // 用来记录月份
unsigned char dayGPS = 0;  // 用来记录日子
unsigned char hourGPS = 0;  // 用来记录小时
unsigned char minGPS = 0;  // 用来记录分钟
int jingdu = 0;  // 用来记录经度
float allDay = 0;  // 用来记录所有的天数
float weidu = 0.0;  // 用来记录太阳直射点的纬度值,可以为负值,负值代表的是南纬
float dangqianweidu = 0.0;  // 用来记录当前纬度值
float qiYuWeiDuZhengWuTaiYangGaoDu = 0;  // 用来记录同一经线上其余纬度的正午太阳高度角

void getZhiSheWeidu()
{
    int INT_weidu = 0;
    int new_all_day = 0;
    allDay = (monthGPS)*30 + dayGPS;  // 这里的monthGps需要减去1,因为没有0月份,最小的月份是1月份
    if((allDay > CHUN_FEN) && (allDay <= XIA_ZHI))  // 从春分日到夏至日
    {
        weidu = 0 + ((float)23/(float)CHUNFEN_TO_XIAZHI) * (allDay - CHUN_FEN);  // 北纬,纬度值为正值,这里必须要用强制类型转换
        INT_weidu = weidu;
    }
    if((allDay > XIA_ZHI) && (allDay <= QIUFEN))  // 从夏至到秋分
    {
        weidu = 23 - ((float)23/(float)XIAZHI_TO_QIUFEN) * (allDay - XIA_ZHI);  // 北纬,纬度值为正值
        INT_weidu = weidu;
    }
    if((allDay > QIUFEN) && (allDay <= DONG_ZHI))  // 从秋分到冬至
    {
        weidu = 0 - ((float)23/(float)QIUFEN_TO_DONGZHI) * (allDay - QIUFEN);  // 南纬,纬度值为负值
        INT_weidu = -weidu;  // 由于得到的是负值,所以需要
    }
    if((allDay > DONG_ZHI) && (allDay <= 390))  // 从冬至到12月30,由于要跨越一个年份,所以需要注意一点
    {
        weidu = -23 + ((float)23/(float)QIUFEN_TO_DONGZHI) * (allDay - DONG_ZHI);  // 南纬,纬度值为负值
        INT_weidu = -weidu;  // 由于得到的是负值,所以需要
    }
    if((allDay > 0) && (allDay <= CHUN_FEN))  // 进入第二年,注意,需要加上去年的值
    {
        allDay = (monthGPS - 1)*30 + dayGPS + 8;
        weidu = ((float)23 / (float)DONGZHI_TO_CHUNFEN) * (allDay) - 23;
        INT_weidu = -weidu;  // 因为纬度值求出来后一定是负值,所以需要到以下
    }
    OLED_ShowNum(X(0),Y(6),INT_weidu,2,16);   
    OLED_ShowChar(X(3),Y(6),'N',16);
    OLED_Refresh();
}

void getSunlightGaodu(void)
{
    /*
        该函数是为了根据正午的太阳直射纬度,来确定同一经线上的其它纬度的正午太阳高度角
    */
    int gaoDuJiao = 0;
    float _weidu_ = 0.0;
    // 计算太阳高度角计算公式为:太阳高度角 = |当地纬度-太阳直射点纬度|



    // 需要转换一下，因为weidu的值可能为负值，所以需要判断一下，如果为负值，就需要转换，因为在上面的公式中的变量都是按照照整数来计算的。
    if(weidu < 0)
    {
        _weidu_ = -weidu;
    }
    else
    {
        _weidu_ = weidu;
    }
    // 这个也需要转换一下，其实这个高度角有可能是两种情况，一种是向前仰，一种是向后仰


    gaoDuJiao =90 - ((int)dangqianweidu - (int)_weidu_);  // 这个公式需要加上绝对值吗？？
    
    
    if(gaoDuJiao < 0)
    {
        gaoDuJiao = -gaoDuJiao;
    }
    qiYuWeiDuZhengWuTaiYangGaoDu = gaoDuJiao;  // 将当前地理位置的正午太阳直射纬度记录下来,该变量为全局的变量,可以被所有的文件和函数访问到
    OLED_ShowNum(X(6),Y(6),(int)qiYuWeiDuZhengWuTaiYangGaoDu,2,16);  // 显示一下
    OLED_Refresh();  // 刷新OLED显示屏
}


/*
     在以上内容中,正午太阳直射角度的确定和一天内的时间没有关系,不会因为时差的问题导致正午太阳直射角度发生变化
    因为正午太阳直射角度是是日子(天)的函数,而不是小时的函数,或者说一天内的时间对太阳高度角的变化影响可以忽略不计
*/


void lastGetJiaodu(void)
{
    // 计算当前时间
    int __hourGPS__ = 0;  // 用来计算小时差值
    int __minGPS__ = 0;  // 用来计算分钟差值
    __minGPS__ = jingdu*4;  // 经度每差一度，时间上差4分钟
    __hourGPS__ = __minGPS__ / 60;  // 转化为小时，这是个小时的差值
    __minGPS__ = __minGPS__ % 60;  // 计算减去正小时数后，还剩下多少分钟
    

    // 以下一段代码用来计算小时差值和分钟差值
    if((minGPS + __minGPS__) >= 60)  // 判断是否超过一个小时
    {
        __hourGPS__ = __hourGPS__ + hourGPS + 1;  // 如果超过了一个小时，则需要加上一个小时
        __minGPS__ = (minGPS + __minGPS__ - 60);  // 转化剩下的分钟数
    }
    else  // 如果不超过一个小时
    {
        __hourGPS__ = __hourGPS__ + hourGPS;
        __minGPS__ = minGPS + __minGPS__;
    }

    // 一下一段代码用来计算是否满一天
    if(__hourGPS__ >= 23)  // 判断转化过来的UTC是否满一天？
    {
        __hourGPS__ = __hourGPS__ - 23;  // 计算转化剩下的小时数
    }

    OLED_ShowNum(X(11),Y(4),__hourGPS__,2,16);
    OLED_ShowChar(X(14),Y(4),':',16);
    OLED_ShowNum(X(16),Y(4),__minGPS__,2,16);
}



