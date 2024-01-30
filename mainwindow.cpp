#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"    // ADD
#include <QDebug>   // ADD stdio.h 같은 거
#include <QString>  // ADD string

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // code start
    // 1. create socket
    socket = new QUdpSocket(this);

    // 2. IP, Port bind
    bool result = socket->bind(QHostAddress::AnyIPv4, 9999);

    qDebug() << result;     // ADD == cout
    if (result)
    {
        qDebug() << "PASS";
    }
    else
    {
        qDebug() << "FAIL";
    }
    // SIGNAL : event 발생
    // SLOT : INT service routine (함수)
    //3. connect : mapping
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));  // 소켓으로부터 읽을 데이터가 존재하면 나의(this) readyRead()를 호출하라
    ui->plot->setInteraction(QCP::iRangeDrag, true);
    ui->plot->setInteraction(QCP::iRangeZoom, true);
    ui->plot->addGraph();
    //ui->plot->xAxis->setLabel("time(s)");
    ui->plot->yAxis->setLabel("temp");
    ui->plot->yAxis->setRange(0.0, 40.0);
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);     //점찍는 스타일 결정.
    ui->plot->graph(0)->setLineStyle(QCPGraph::lsLine);                           //라인 스타일 결정.
    connect(ui->plot, SIGNAL(mouseDoubleClickEvent(QMouseEvent*)), SLOT(QMouseEvent*));
}

void MainWindow::readyRead()
{
    QByteArray Buffer;
    double temp_digit;  // temperature
    double wet_digit;

    static int time = 0;

    QHostAddress sender;    // 송신자의 IP
    quint16 senderPort;     // 송신자의 Port
    Buffer.resize(socket->pendingDatagramSize());

    // 3. socket으로부터 데이터 read
    socket->readDatagram(Buffer.data(), Buffer.size(), &sender, &senderPort);
    Buffer.chop(1);     // \n 제거 (마지막 바이트 제거)
    ui->textEditRxData->append(Buffer);
    ui->textEditRxData->show();

    // LCD
    temp_digit = Buffer.right(2).toDouble();    // 오른쪽 2개 잘라내기
    Buffer.chop(9);
    qDebug() << Buffer;
    wet_digit = Buffer.right(2).toDouble();
    ui->lcdNumberTemp->display(temp_digit);
    ui->lcdNumberWet->display(wet_digit);

//    qDebug() <<"Message From :: " << sender.toString();
//    qDebug() <<"Port From :: "<< senderPort;
//    qDebug() <<"Buffer :: " << Buffer;

    add_point(time, temp_digit);
    time+=3;
    ui->plot->xAxis->setRange(0, time+3);
    plot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 생성경로 : send -> 마우스 우클릭 -> go to slot -> click
void MainWindow::on_pushButtonSend_clicked()
{
    QByteArray Data;

    Data = ui->lineEditSendData->text().toUtf8();
    socket->writeDatagram(Data, QHostAddress("10.10.15.87"), 9999);

}

// 생성경로 : LED1 -> 마우스 우클릭 -> go to slot -> statechange
void MainWindow::on_checkBoxLED1_stateChanged(int arg1)
{
    QString buffer;
    QByteArray SendData;
    buffer.sprintf("LED001");
    SendData = buffer.toUtf8();     // ascii는 utf8과 값이 동일 / 변환이 굳이 필요 없음
    socket->writeDatagram(SendData, QHostAddress("10.10.15.87"), 9999);
}

void MainWindow::on_checkBoxLED2_stateChanged(int arg1)
{
    QString buffer;
    QByteArray SendData;
    buffer.sprintf("LED002");
    SendData = buffer.toUtf8();     // ascii는 utf8과 값이 동일 / 변환이 굳이 필요 없음
    socket->writeDatagram(SendData, QHostAddress("10.10.15.87"), 9999);
}

void MainWindow::on_checkBoxLED3_stateChanged(int arg1)
{
    QString buffer;
    QByteArray SendData;
    buffer.sprintf("LED003");
    SendData = buffer.toUtf8();     // ascii는 utf8과 값이 동일 / 변환이 굳이 필요 없음￩￩￩
    socket->writeDatagram(SendData, QHostAddress("10.10.15.87"), 9999);
}

void MainWindow::add_point(double x, double y){
    qv_x.append(x);
    qv_y.append(y);
}

void MainWindow::clear_data(){
    qv_x.clear();
    qv_y.clear();
}

void MainWindow::plot(){
    ui->plot->graph(0)->setData(qv_x, qv_y);
    ui->plot->replot();
    ui->plot->update();
}

// SERVO:999\n
void MainWindow::on_dial_servo_valueChanged(int value)
{
    QByteArray servo_data = "SERVO:";

    servo_data.append(QString::number(ui->dial_servo->value()));
    ui->lcdNumber_Servo->display(ui->dial_servo->value());
    socket->writeDatagram(servo_data, QHostAddress("10.10.15.87"), 9999);

    qDebug() << "servo data : " << servo_data << endl;
}

//LED:999\n
void MainWindow::on_dial_led_valueChanged(int value)
{
    QByteArray led_data = "LED:";

    led_data.append(QString::number(ui->dial_led->value()));
    ui->lcdNumber_led->display(ui->dial_led->value());
    socket->writeDatagram(led_data, QHostAddress("10.10.15.87"), 9999);

    qDebug() << "led data : " << led_data << endl;
}

void MainWindow::on_pushButton_clear_clicked()
{
    clear_data();
}

// DHT:999\n
void MainWindow::on_horizontalSlider_DHT11_interval_valueChanged(int value)
{
    QByteArray dht_data = "DHT:";

    dht_data.append(QString::number(ui->horizontalSlider_DHT11_interval->value()));

    socket->writeDatagram(dht_data, QHostAddress("10.10.15.87"), 9999);

    qDebug() << "DHT data : " << dht_data << endl;
}
