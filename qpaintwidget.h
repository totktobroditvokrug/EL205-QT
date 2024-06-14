#ifndef QPAINTWIDGET_H
#define QPAINTWIDGET_H

#include <QWidget>

class QPaintEvent;

class QPaintWidget : public QWidget
{
    Q_OBJECT
public:
    QPaintWidget(QWidget * parent = nullptr);


    int uconv(){ return _uConv; }

    void drawScreen(QPainter*);
    void drawUconv(QPainter*);
    void drawIconv(QPainter*);
private:
    int _uConv;  // вектор напряжения
    int _uConvMax;  // максимальное напряжение

    int _iA;
    int _iB;
    int _iC;
    int _iMax;

    int _phA;
    int _phB;
    int _phC;

public slots:
    void setUconv(int uConv, int uConvMax);
    void setIconv(int iA, int iB, int iC, int iMax, int phA, int phB, int phC);

protected:
    void paintEvent(QPaintEvent *);

};

#endif // QPAINTWIDGET_H
