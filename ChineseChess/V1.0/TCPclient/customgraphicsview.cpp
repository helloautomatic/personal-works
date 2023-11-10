/*
由于Qt中自带的GraphicView的鼠标事件相关的成员变量和方法都是protected类型,不能直接访问
所以需要继承该Qt的GraphicView类,并对其中的方法重写
*/

#include "customgraphicsview.h"

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QPointF pos = mapToScene(event->pos());
        emit clicked(pos);
    }
    QGraphicsView::mousePressEvent(event);
}
