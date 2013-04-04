#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class QTimer;
class QextSerialPort;
class QextSerialEnumerator;
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void onStartPause();
    void onReadyRead();

private:
    Ui::Widget *ui;
    void MatchPoint(unsigned char angle, char dev_id);
//    QPoint point_lh_fix, point_rh_fix, point_ll_fix, point_rl_fix, point_left_hand, point_right_hand, point_left_leg, point_right_leg;
   int lh_x,lh_y,rh_x,rh_y,ll_x,ll_y,rl_x,rl_y;
    QextSerialPort *port;
    QTimer *timer;
};

#endif // WIDGET_H
