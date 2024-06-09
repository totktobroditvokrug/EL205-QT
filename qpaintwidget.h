#ifndef QPAINTWIDGET_H
#define QPAINTWIDGET_H

#include <QWidget>

class QPaintEvent;

class QPaintWidget : public QWidget
{
    Q_OBJECT
public:
    QPaintWidget(QWidget * parent = nullptr);
protected:
    void paintEvent(QPaintEvent *);
};

#endif // QPAINTWIDGET_H
