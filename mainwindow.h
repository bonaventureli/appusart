#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qextserialport.h"
#include <QTimer>
#include <QTime>
#include <QtDebug>
#include <QMessageBox>
#include "qextserialenumerator.h"
#include <QFile>
namespace Ui {
class MainWindow;
}
class  QextSerialPort;
class  QextSerialEnumerator;
struct QextPortInfo;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void SendCmd(QString sCmd);
private slots:
    void on_openduoji_clicked();
    void SelectPort();
    void on_portNameComboBox_currentIndexChanged(int index);
    void readMyCom();

    void on_closeduoji_clicked();

    void on_openprojector_clicked();

    void on_closeprojector_clicked();

    void on_lineEditCommand_returnPressed();

    void on_foreward_clicked();

    void on_overturn_clicked();

    void on_stop_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QextSerialPort *myCom;
    QList<QextPortInfo> *pPorts;
    QextSerialEnumerator *pEnumerator;
    QString portName;
    QString friendName;
    QString m_sSerialString;
};

#endif // MAINWINDOW_H
