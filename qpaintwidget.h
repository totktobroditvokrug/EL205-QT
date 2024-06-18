#ifndef QPAINTWIDGET_H
#define QPAINTWIDGET_H

#include <QWidget>

class QPaintEvent;

class QPaintWidget : public QWidget
{
    Q_OBJECT
public:
    QPaintWidget(QWidget * parent = nullptr);

    // сеттеры взаимодействия с главной формой
    void setUconv(int uConv, int uConvMax, bool showUph, bool showUline);
    void setIconv(int iA, int iB, int iC, int iMax, int phA, int phB, int phC, bool showI);

    void drawScreen(QPainter*);
    void drawUconv(QPainter*);
    void drawUline(QPainter*);
    void drawIconv(QPainter*);

private:
    int _maxRad;    // максимальный радиус окна векторной диаграммы

    QPoint _center; // центр виджета
    QPoint _zero;

    // колебания толщины вектора при выходе за масштаб
    int _widghtJitterU;
    int _widghtJitterIa;
    int _widghtJitterIb;
    int _widghtJitterIc;

    bool _showUph;
    bool _showUline;
    bool _showI;

    int _fontSize;

    int _uConv;     // вектор напряжения
    int _uConvMax;  // максимальное напряжение

    int _iA;
    int _iB;
    int _iC;
    int _iMax;

    int _phA;
    int _phB;
    int _phC;

public slots:
 //   void setUconv(int uConv, int uConvMax);
 //   void setIconv(int iA, int iB, int iC, int iMax, int phA, int phB, int phC);

protected:
    void paintEvent(QPaintEvent *);

};

#endif // QPAINTWIDGET_H
