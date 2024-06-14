#include "qpaintwidget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QtMath>
#include <QDebug>

QPaintWidget::QPaintWidget(QWidget * parent) : QWidget(parent)
{
    _uConv = 0;
    _uConvMax = 500;
    _iA = 0;
    _iB = 0;
    _iC = 0;
    _iMax = 500;
    _phA = 0;
    _phB = 120;
    _phC = 240;
}

void QPaintWidget::paintEvent(QPaintEvent *) {
    QPainter PaintScreen(this); // объект рисовальщика экрана
    drawScreen(&PaintScreen);   // экран осциллографа
    drawUconv(&PaintScreen);    // трехфазное напряжение ПЧ
    drawIconv(&PaintScreen);    // трехфазный ток ПЧ
}


void QPaintWidget::drawScreen(QPainter* PaintScreen){
    PaintScreen->save();
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

//   QPainter PaintScreen(this); // объект рисовальщика экрана

   PaintScreen->setPen(QPen(Qt::red, 1, Qt::DotLine)); // Настройки рисования
   PaintScreen->drawEllipse(center, rad1, rad1);
   PaintScreen->drawEllipse(center, rad2, rad2);
   PaintScreen->drawEllipse(center, rad3, rad3);
   PaintScreen->setPen(QPen(Qt::black, 1, Qt::SolidLine)); // Настройки рисования
   PaintScreen->drawEllipse(center, rad4, rad4);
   PaintScreen->translate(center);
   PaintScreen->setPen(QPen(Qt::red, 1, Qt::DotLine)); // Настройки рисования
   PaintScreen->drawLine(-rad4, 0, rad4, 0); // ось Х
   PaintScreen->drawLine(0, -rad4, 0, rad4); // ось У

   PaintScreen->restore();
}

QPoint lineVector(int value, int maxValue, int maxScreen, double angle){
    int lengthVector;
    if (maxValue <= 0) lengthVector = 0;
    else lengthVector = value * maxScreen / maxValue;  // длина вектора на экране
    int xVector = int(lengthVector * cos(qDegreesToRadians(angle)));
    int yVector = -int(lengthVector * sin(qDegreesToRadians(angle)));
    QPoint endLine(xVector, yVector);
    return endLine;
}

void QPaintWidget::drawUconv(QPainter* PaintUconv){

//    QPainter PaintUconv(this); // объект рисовальщика экрана
    PaintUconv->save();
    QPoint center(width()/2, height()/2); // начало координат в центр виджета
    QPoint zero(0, 0);
    int maxScreen = qMin(width()*2/5,height()*2/5);

    PaintUconv->setPen(QPen(Qt::green, 2, Qt::SolidLine)); // Настройки рисования
    PaintUconv->translate(center);
 //   PaintUconv.drawLine(zero, endLine);
    PaintUconv->drawLine(zero, lineVector(_uConv, _uConvMax, maxScreen, 0));
    PaintUconv->drawLine(zero, lineVector(_uConv, _uConvMax, maxScreen, 120));
    PaintUconv->drawLine(zero, lineVector(_uConv, _uConvMax, maxScreen, -120));
 //   qDebug() << "функция рисования вектора. U ="  << _uConv;
    PaintUconv->restore();
}
void QPaintWidget::drawIconv(QPainter* PaintIconv){
    PaintIconv->save();
    QPoint center(width()/2, height()/2); // начало координат в центр виджета
    QPoint zero(0, 0);
    int maxScreen = qMin(width()*2/5,height()*2/5);

    PaintIconv->setPen(QPen(Qt::blue, 2, Qt::SolidLine)); // Настройки рисования
    PaintIconv->translate(center);
 //   PaintUconv.drawLine(zero, endLine);
    PaintIconv->drawLine(zero, lineVector(_iA, _iMax, maxScreen, _phA));
    PaintIconv->drawLine(zero, lineVector(_iB, _iMax, maxScreen, _phB));
    PaintIconv->drawLine(zero, lineVector(_iC, _iMax, maxScreen, -_phC));

//    PaintIconv->drawLine(zero, lineVector(200, _iMax, maxScreen, _phA));
//    PaintIconv->drawLine(zero, lineVector(300, _iMax, maxScreen, _phB));
//    PaintIconv->drawLine(zero, lineVector(400, _iMax, maxScreen, -_phC));

   // qDebug() << "функция рисования вектора. U ="  << _uConv;
    PaintIconv->restore();

}

void QPaintWidget::setUconv(int uConv, int uConvMax){
    _uConv = uConv;
    _uConvMax = uConvMax;
}

void QPaintWidget::setIconv(int iA, int iB, int iC, int iMax, int phA, int phB, int phC){
    _iA = iA;
    _iB = iB;
    _iC = iC;
    _iMax = iMax;
    _phA = phA;
    _phB = phB;
    _phC = phC;
}

