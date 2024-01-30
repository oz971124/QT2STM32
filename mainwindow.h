#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>   // ADD
#include <qtextstream.h>    // ADD

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void add_point(double x, double y);     // ADD
    void clear_data();      // ADD
    void plot();            // ADD

public slots:   // callback func
    void readyRead();

private slots:
    void on_pushButtonSend_clicked();

    void on_checkBoxLED1_stateChanged(int arg1);

    void on_checkBoxLED2_stateChanged(int arg1);

    void on_checkBoxLED3_stateChanged(int arg1);

    void on_dial_servo_valueChanged(int value);

    void on_dial_led_valueChanged(int value);

    void on_pushButton_clear_clicked();

    void on_horizontalSlider_DHT11_interval_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QUdpSocket *socket = nullptr;   // ADD
    QVector <double> qv_x, qv_y;    // ADD
    QString temp;                   // ADD
};

#endif // MAINWINDOW_H
