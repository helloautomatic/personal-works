/*
 * @Author: your name
 * @Date: 2021-04-10 20:40:41
 * @LastEditTime: 2021-05-18 10:06:43
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \ZET6\HARDWARE\timeCtrl.h
 */
#ifndef __TIMECTRL_H
#define __TIMECTRL_H
#define CHUN_FEN 111  // 秋分日
#define XIA_ZHI 202  // 夏至日
#define QIUFEN 293 // 秋分日
#define DONG_ZHI 382  //TODO 冬至日,这里尤其要注意,由于没有0月份,所以这里不用再减去30天了,但是在换年的时候,算法该什么设计呢??
#define CHUNFEN_TO_XIAZHI 90  // 172-102
#define XIAZHI_TO_QIUFEN 91  // 263-172
#define QIUFEN_TO_DONGZHI 89  // 352-263
#define DONGZHI_TO_CHUNFEN 90  // 
#define YUE_12_30 20  // TODO 经过计算得到,12月30号的时候,太阳直射南纬20度,这个是经过测试出来的.这在换年的时候会使用到


// 以下变量都是数值型的整型变量
extern unsigned char monthGPS;  // 用来记录月份
extern unsigned char dayGPS;  // 用来记录日子
extern unsigned char hourGPS;  // 用来记录小时
extern unsigned char minGPS;  // 用来记录分钟
extern float weidu;  // 用来记录纬度
extern int jingdu;  // 用来记录经度
extern float allDay;  // 用来记录所有的天数  
extern float dangqianweidu;  // 用来记录当前纬度值
extern float qiYuWeiDuZhengWuTaiYangGaoDu;  // 用来记录同一经线上其余纬度的正午太阳高度角


void getZhiSheWeidu();
void getSunlightGaodu(void);
void lastGetJiaodu(void);
#endif


