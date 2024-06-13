#include "qpaintwidget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QtMath>

QPaintWidget::QPaintWidget(QWidget * parent) : QWidget(parent)
{

}

void QPaintWidget::paintEvent(QPaintEvent *) {
    drawScreen();   // экран осциллографа
    drawUconv();    // трехфазное напряжение ПЧ
    drawIconv();    // трехфазный ток ПЧ
}


void QPaintWidget::drawScreen(){
   QPoint center(width()/2, height()/2); // начало координат в центр виджета
   int maxRad = qMin(width()/2,height()/2);

   int rad1 = maxRad/5;
   int rad2 = rad1 * 2;
   int rad3 = rad1 * 3;
   int rad4 = rad1 * 4;

   QRectF radius1(-rad1, -rad1, rad1, rad1);
   QRectF radius2(-rad2, -rad2, rad2, rad2);
   QRectF radius3(-rad3, -rad3, rad3, rad3);
   QRectF radius4(-rad4, -rad4, rad4, rad4);

   QPainter PaintScreen(this); // объект рисовальщика экрана

   PaintScreen.setPen(QPen(Qt::red, 1, Qt::DotLine)); // Настройки рисования
   PaintScreen.drawEllipse(center, rad1, rad1);
   PaintScreen.drawEllipse(center, rad2, rad2);
   PaintScreen.drawEllipse(center, rad3, rad3);
   PaintScreen.setPen(QPen(Qt::black, 1, Qt::SolidLine)); // Настройки рисования
   PaintScreen.drawEllipse(center, rad4, rad4);
   PaintScreen.translate(center);
   PaintScreen.setPen(QPen(Qt::red, 1, Qt::DotLine)); // Настройки рисования
   PaintScreen.drawLine(-rad4, 0, rad4, 0); // ось Х
   PaintScreen.drawLine(0, -rad4, 0, rad4); // ось У
}

QPoint lineVector(int value, int maxValue, int maxScreen, double angle){
    int lengthVector = value * maxScreen / maxValue;  // длина вектора на экране
    int xVector = int(lengthVector * cos(qDegreesToRadians(angle)));
    int yVector = -int(lengthVector * sin(qDegreesToRadians(angle)));
    QPoint endLine(xVector, yVector);
    return endLine;
}

void QPaintWidget::drawUconv(){
    QPoint center(width()/2, height()/2); // начало координат в центр виджета
    QPoint zero(0, 0);
    int maxScreen = qMin(width()*2/5,height()*2/5);
    QPoint endLine(60, 0);
    QPainter PaintUconv(this); // объект рисовальщика экрана
    PaintUconv.setPen(QPen(Qt::green, 2, Qt::SolidLine)); // Настройки рисования
    PaintUconv.translate(center);
 //   PaintUconv.drawLine(zero, endLine);
    PaintUconv.drawLine(zero, lineVector(30, 100, maxScreen, 0));
    PaintUconv.drawLine(zero, lineVector(60, 100, maxScreen, 120));
    PaintUconv.drawLine(zero, lineVector(90, 100, maxScreen, 240));

}
void QPaintWidget::drawIconv(){
    QPoint center(width()/2, height()/2); // начало координат в центр виджета
    int maxRad = qMin(width()/2,height()/2);

}

