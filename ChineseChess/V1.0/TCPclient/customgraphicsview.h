#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H
#include <QMouseEvent>
#include <QPointF>
#include <QGraphicsView>

class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void clicked(const QPointF &pos);
};


#endif // CUSTOMGRAPHICSVIEW_H
