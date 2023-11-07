# 数据集的准备
该项目中主要使用CPLID（中国绝缘子数据集）数据集和IDID（Insulator Defect Image Dataset）数据集。

[CPLID数据集的链接](https://aistudio.baidu.com/datasetdetail/90042)

[IDID数据集的链接](https://ieee-dataport.s3.amazonaws.com/competition/78246/Train_IDID_V1.2.zip?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAJOHYI4KJCE6Q7MIQ%2F20231107%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20231107T141511Z&X-Amz-SignedHeaders=Host&X-Amz-Expires=86400&X-Amz-Signature=a5d1182245619ec21211d24cd0009fc58d42c822bf02b1d54664acc2e88da860)

## 数据集格式的转换
为了统一数据集格式，该项目中所有的数据集都采用COCO数据集的标注格式。
如果一个数据集标注好了，但是他的标注不是COCO格式的，那么需要将其转换为COCO格式的。
只要一个数据集被标注好了，不管他是什么格式的，理论上就一定可以通过python脚本将其转换为
COCO格式的数据集。

## CPLID数据集的格式转化为COCO格式
这里的CPLID数据集的格式采用的是VOC标注格式。因此只需要将VOC转换为COCO就可以了。
这里可以参考一些网上的VOC转COCO的教程。或者自己写转换代码都可以。

## IDID数据集格式转化为COCO
IDID转换为COCO只能自己写代码转换，或者根据IDID数据集格式的特点继承并重写Pytorch库中的Dataset类。具体的重写方法可以参考Pytorch的[官网教程](https://pytorch.org/tutorials/beginner/basics/data_tutorial.html)。
比如，主要改写下面的类
```
#  导入运行以下代码所需要的库

class YourImageDataset(Dataset):  # 继承Pytorch中的Dataset

    # 根据自己的数据集特点重新 __init__ 方法
    def __init__(self, annotations_file, img_dir, transform=None, target_transform=None):  
        ''' 此处插入代码 '''

    def __len__(self):  # 根据自己的数据集特点重写 __len__ 方法
        ''' 此处插入代码 '''
        return len(self.img_labels)

    def __getitem__(self, idx):  # 根据自己的数据集特点重写 __getitem__ 方法
        ''' 此处插入代码 '''
        return image, label
```

采用重写Dataset的方法以适应别人的模型，可能会遇到很多麻烦。比如图像的预处理会和别人的不太一样、每个数据集都要重写一次Dataset类、都要重写调试和测试代码等等。因此，这里将数据集统一化为COCO数据集。

## IDID数据集的标注格式
绝缘子（shell）标注如下
![170540](https://github.com/helloautomatic/personal-works/assets/88640443/21de0dc0-e3d0-4980-a2ad-9f7b49a0f7ae)

![STR 75-Line 295000](https://github.com/helloautomatic/personal-works/assets/88640443/544ea0c4-36f6-4043-9ae2-791286c2d375)







