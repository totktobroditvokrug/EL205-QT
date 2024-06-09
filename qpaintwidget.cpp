#include "qpaintwidget.h"

#include <QPainter>
#include <QPaintEvent>

QPaintWidget::QPaintWidget(QWidget * parent) : QWidget(parent)
{
}

void QPaintWidget::paintEvent(QPaintEvent *) {
    QPainter PaintVectrorBM(this); // объект рисовальщика векторной диаграммы ВЭД
    PaintVectrorBM.setPen(QPen(Qt::red, 2, Qt::DotLine)); // Настройки рисования
    QPoint center(width()/2,height()/2);
    int rad = qMin(width()/4,height()/4);
    PaintVectrorBM.drawEllipse(center,rad,rad);

    QPoint endLine(int(rad*1.2), int(rad*1.2));
    PaintVectrorBM.setPen(QPen(Qt::blue, 2, Qt::SolidLine)); // Настройки рисования
    PaintVectrorBM.drawLine(center, endLine);
  //  PaintVectrorBM.drawEllipse(0,0,width(),height()); // Рисование линии
}
