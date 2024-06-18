#include "qpaintwidget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QtMath>
#include <QDebug>

QPaintWidget::QPaintWidget(QWidget * parent) : QWidget(parent)
{
    _uConv = 0;
    _uConvMax = 500;
    _widghtJitterU = 4;  // дрожание при недостатке масштаба
    _widghtJitterIa = 4;  // дрожание при недостатке масштаба
    _widghtJitterIb = 4;  // дрожание при недостатке масштаба
    _widghtJitterIc = 4;  // дрожание при недостатке масштаба

    _fontSize = 15;

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
    _maxRad = qMin(width()/2,height()/2) - (3 * _fontSize);       // чтобы помещались подписи векторов
    _center = QPoint(width()/2, height()/2);
    _zero = QPoint(0, 0);                       // начало координат в центр виджета

    drawScreen(&PaintScreen);   // экран осциллографа
    if(_showUph) drawUconv(&PaintScreen);    // трехфазное напряжение ПЧ
    if(_showI) drawIconv(&PaintScreen);      // трехфазный ток ПЧ
    if(_showUline) drawUline(&PaintScreen);  // линейное напряжение
    // qDebug() << "paintEvent. _widghtJitterX:" << _widghtJitterU << _widghtJitterIa << _widghtJitterIb << _widghtJitterIc;
}


void QPaintWidget::drawScreen(QPainter* PaintScreen){
    PaintScreen->save();

    int rad1 = _maxRad/4;
    int rad2 = rad1 * 2;
    int rad3 = rad1 * 3;
    int rad4 = rad1 * 4;

    PaintScreen->setPen(QPen(Qt::red, 1, Qt::DotLine)); // Внутренние круги красные пунктирные
    PaintScreen->drawEllipse(_center, rad1, rad1);
    PaintScreen->drawEllipse(_center, rad2, rad2);
    PaintScreen->drawEllipse(_center, rad3, rad3);
    PaintScreen->setPen(QPen(Qt::black, 1, Qt::SolidLine)); // Внешний круг сплошной черный
    PaintScreen->drawEllipse(_center, rad4, rad4);
    PaintScreen->translate(_center);
    PaintScreen->setPen(QPen(Qt::red, 1, Qt::DotLine)); // Оси красные пунктирные
    PaintScreen->drawLine(-rad4, 0, rad4, 0); // ось Х
    PaintScreen->drawLine(0, -rad4, 0, rad4); // ось У

    PaintScreen->restore();
}

QPoint lineVector(int value, int maxValue, int maxScreen, double angle){
    int lengthVector;  // масштабируемая к экрану длина вектора.

    if (maxValue <= 0) lengthVector = 0;
    else lengthVector = value * maxScreen / maxValue;  // длина вектора на экране

    int xVector = int(lengthVector * cos(qDegreesToRadians(angle)));
    int yVector = -int(lengthVector * sin(qDegreesToRadians(angle)));
    QPoint endLine(xVector, yVector);
    return endLine;
}

void QPaintWidget::drawUconv(QPainter* PaintUconv){
    PaintUconv->save();

    QFont font = PaintUconv->font();    // получаем шрифт
    font.setPixelSize(_fontSize);          // изменяем высоту шрифта
    PaintUconv->setFont(font);          // переустанавливаем шрифт

    PaintUconv->setPen(QPen(Qt::green, 2, Qt::SolidLine)); // Настройки рисования
    if(_uConv >= _uConvMax) {
        _uConv = _uConvMax;
        if(_widghtJitterU < 4){
            _widghtJitterU++;  // дрожание толщины вектора при выходе за пределы экрана
            PaintUconv->setPen(QPen(Qt::red, _widghtJitterU, Qt::SolidLine)); // Настройки рисования
        }
        else{
            _widghtJitterU = 2;
            PaintUconv->setPen(QPen(Qt::green, 2, Qt::SolidLine)); // Настройки рисования
        }
    }

    PaintUconv->translate(_center);
    QPoint vectorUa = lineVector(_uConv, _uConvMax, _maxRad, 0);
    PaintUconv->drawLine(_zero, vectorUa);
    PaintUconv->drawText(vectorUa, " Ua");

    QPoint vectorUb = lineVector(_uConv, _uConvMax, _maxRad, 120);
    PaintUconv->drawLine(_zero, vectorUb);
    PaintUconv->drawText(vectorUb," Ub");

    QPoint vectorUc = lineVector(_uConv, _uConvMax, _maxRad, -120);
    PaintUconv->drawLine(_zero, vectorUc);
    PaintUconv->drawText(vectorUc, " Uc");

    PaintUconv->restore();
}

void QPaintWidget::drawUline(QPainter* PaintUconv){
    if(_uConv >= _uConvMax) {
        return;
    }

    PaintUconv->save();

    QFont font = PaintUconv->font();    // получаем шрифт
    font.setPixelSize(_fontSize);          // изменяем высоту шрифта
    PaintUconv->setFont(font);          // переустанавливаем шрифт

    PaintUconv->setPen(QPen(Qt::green, 1, Qt::DotLine)); // Настройки рисования

    PaintUconv->translate(_center);
    QPoint vectorUa = lineVector(_uConv, _uConvMax, _maxRad, 0);

    PaintUconv->drawText(vectorUa, " Ua");

    QPoint vectorUb = lineVector(_uConv, _uConvMax, _maxRad, 120);

    PaintUconv->drawText(vectorUb," Ub");

    QPoint vectorUc = lineVector(_uConv, _uConvMax, _maxRad, -120);

    PaintUconv->drawText(vectorUc, " Uc");

    PaintUconv->drawLine(vectorUa, vectorUb);
    PaintUconv->drawLine(vectorUb, vectorUc);
    PaintUconv->drawLine(vectorUc, vectorUa);

    PaintUconv->restore();
}

void QPaintWidget::drawIconv(QPainter* PaintIconv){
    PaintIconv->save();
    QFont font = PaintIconv->font();    // получаем шрифт
    font.setPixelSize(_fontSize);          // изменяем высоту шрифта
    PaintIconv->setFont(font);          // переустанавливаем шрифт

    PaintIconv->setPen(QPen(Qt::blue, 2, Qt::SolidLine)); // Настройки рисования

    PaintIconv->translate(_center);

    if(_iA >= _iMax) {
        _iA = _iMax;
        if(_widghtJitterIa < 4){
            _widghtJitterIa++;  // дрожание толщины вектора при выходе за пределы экрана
            PaintIconv->setPen(QPen(Qt::red, _widghtJitterIa, Qt::SolidLine)); // Настройки рисования
        }
        else{
            _widghtJitterIa = 2;
            PaintIconv->setPen(QPen(Qt::blue, 2, Qt::SolidLine)); // Настройки рисования
        }
        // qDebug() << "выход за масштаб Ia:" << _widghtJitterIa;
    }
    else  PaintIconv->setPen(QPen(Qt::blue, 2, Qt::SolidLine)); // Настройки рисования

    QPoint vectorIa = lineVector(_iA, _iMax, _maxRad, _phA);
    PaintIconv->drawLine(_zero, vectorIa);
    PaintIconv->drawText(vectorIa, " Ia");

    PaintIconv->setPen(QPen(Qt::blue, 2, Qt::SolidLine)); // Настройки рисования

    if(_iB >= _iMax) {
        _iB = _iMax;
        if(_widghtJitterIb < 4){
            _widghtJitterIb++;  // дрожание толщины вектора при выходе за пределы экрана
            PaintIconv->setPen(QPen(Qt::red, _widghtJitterIb, Qt::SolidLine)); // Настройки рисования
        }
        else{
            _widghtJitterIb = 2;
            PaintIconv->setPen(QPen(Qt::blue, 2, Qt::SolidLine)); // Настройки рисования
        }
    }
    else  PaintIconv->setPen(QPen(Qt::blue, 2, Qt::SolidLine)); // Настройки рисовани

    QPoint vectorIb = lineVector(_iB, _iMax, _maxRad, _phB);
    PaintIconv->drawLine(_zero, vectorIb);
    PaintIconv->drawText(vectorIb, " Ib");

    PaintIconv->setPen(QPen(Qt::blue, 2, Qt::SolidLine)); // Настройки рисования

    if(_iC >= _iMax) {
        _iC = _iMax;
        if(_widghtJitterIc < 4){
            _widghtJitterIc++;  // дрожание толщины вектора при выходе за пределы экрана
            PaintIconv->setPen(QPen(Qt::red, _widghtJitterIc, Qt::SolidLine)); // Настройки рисования
        }
        else{
            _widghtJitterIc = 2;
            PaintIconv->setPen(QPen(Qt::blue, 2, Qt::SolidLine)); // Настройки рисования
        }
    }
    else  PaintIconv->setPen(QPen(Qt::blue, 2, Qt::SolidLine)); // Настройки рисования

    QPoint vectorIc = lineVector(_iC, _iMax, _maxRad, _phC);
    PaintIconv->drawLine(_zero, vectorIc);
    PaintIconv->drawText(vectorIc, " Ic");

    PaintIconv->restore();
}

void QPaintWidget::setUconv(int uConv, int uConvMax, bool showUph, bool showUline){
    _showUph = showUph;
    _showUline = showUline;
    _uConv = uConv;
    _uConvMax = uConvMax;
}

void QPaintWidget::setIconv(int iA, int iB, int iC, int iMax, int phA, int phB, int phC, bool showI){
    _showI = showI;
    _iA = iA;
    _iB = iB;
    _iC = iC;
    _iMax = iMax;
    _phA = phA;
    _phB = phB;
    _phC = phC;
}

