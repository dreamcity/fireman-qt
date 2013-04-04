#include "widget.h"
#include "ui_widget.h"
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include "qextserialport.h"
#include "qextserialenumerator.h"
#include <math.h>
#include <QTimer>

#define DEVICE_LH	0x01
#define DEVICE_RH	0x02
#define DEVICE_LL	0x03
#define DEVICE_RL	0x04

#define STAT_0_DEG	0x00
#define STAT_30_DEG	0x01
#define STAT_60_DEG	0x02
#define STAT_90_DEG	0x03
#define STAT_120_DEG	0x04
#define STAT_150_DEG	0x05

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    setWindowTitle("fireman v1.0");
    ui->setupUi(this);
    QPixmap pm("head_body.png");
    ui->label->setPixmap(pm);
    ui->label->setScaledContents(true);
    ui->label->show();
    PortSettings settings = {BAUD115200, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
    port = new QextSerialPort("ttyUSB0", settings, QextSerialPort::Polling);
    timer = new QTimer();
    timer->setInterval(50);
    lh_x=rh_x=ll_x=rl_x=0;
    lh_y=rh_y=ll_y=rl_y=-100;
//    QPoint point_lh_fix, point_rh_fix, point_ll_fix, point_rl_fix, point_left_hand, point_right_hand, point_left_leg, point_right_leg;
//    point_lh_fix.setX(320);
//    point_lh_fix.setY(100);
//    point_left_hand.setX(320);
//    point_left_hand.setY(200);
    connect(timer, SIGNAL(timeout()), SLOT(onReadyRead()));
    connect(ui->startButton, SIGNAL(clicked()), SLOT(onStartPause()));
    connect(port, SIGNAL(readyRead()), SLOT(onReadyRead()));
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
	painter.setPen(QPen(Qt::black, 20, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(360,100,360-lh_x,100-lh_y);
    painter.drawLine(440,100,440+rh_x,100-rh_y);
    painter.drawLine(360,280,360-ll_x,280-ll_y);
    painter.drawLine(440,280,440+rl_x,280-rl_y);
}
void Widget::onStartPause()
{
    if (!port->isOpen())
    {
        port->open(QIODevice::ReadWrite);
    }
    else
    {
        port->close();
    }

    //If using polling mode, we need a QTimer
    if (port->isOpen() && port->queryMode() == QextSerialPort::Polling)
    {
        timer->start();
    }
    else
    {
        timer->stop();
    }
}

void Widget::onReadyRead()
{
    QByteArray dataRec;
    char *ch;
    if (port->bytesAvailable() >= 3){
        dataRec = port->readAll();
        ch = dataRec.data();
        if(ch[0] == 0){
            MatchPoint((unsigned char)ch[2], ch[1]);
        }
    }
}

void Widget::MatchPoint(unsigned char angle, char dev_id)
{
    int temp;
    if(angle < 30 && angle > 0){
        temp = STAT_150_DEG;
    } else if(angle < 60 && angle >=30){
        temp = STAT_120_DEG;
    } else if(angle < 90 && angle >= 60){
        temp = STAT_90_DEG;
    } else if(angle < 120 && angle >= 90){
        temp = STAT_60_DEG;
    } else if(angle < 150 && angle >= 120){
        temp = STAT_30_DEG;
    } else if(angle <= 180 && angle >= 150){
        temp = STAT_0_DEG;
    }
    if(dev_id == DEVICE_LH){
    lh_x = 100 * sin(1.0 / 6.0 * 3.1416 * temp);
    lh_y = 100 * cos(1.0 / 6.0 * 3.1416 * temp);
    } else if(dev_id == DEVICE_RH){
    rh_x = 100 * sin(1.0 / 6.0 * 3.1416 * temp);
    rh_x = 100 * cos(1.0 / 6.0 * 3.1416 * temp);
    } else if(dev_id == DEVICE_LL){
    ll_x = 100 * sin(1.0 / 6.0 * 3.1416 * temp);
    ll_x = 100 * cos(1.0 / 6.0 * 3.1416 * temp);
    } else if(dev_id == DEVICE_LH){
    rl_x = 100 * sin(1.0 / 6.0 * 3.1416 * temp);
    rl_x = 100 * cos(1.0 / 6.0 * 3.1416 * temp);
    }
    update();
}

Widget::~Widget()
{
    delete ui;
}

