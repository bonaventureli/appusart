#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pPorts = new QList<QextPortInfo>;
    pEnumerator = new QextSerialEnumerator();
    ui->openduoji->setEnabled(false);
    ui->openprojector->setEnabled(false);
    ui->closeduoji->setEnabled(false);
    ui->closeprojector->setEnabled(false);
    ui->foreward->setEnabled(false);
    ui->overturn->setEnabled(false);
    ui->stop->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);

    SelectPort();
    //QTimer::singleShot(10000, this, SLOT(SelectPort())); // Check in 10 seconds
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openduoji_clicked()
{
    SendCmd("a");
}
void MainWindow::SendCmd(QString sCmd)
{
if(myCom)myCom->write(sCmd.toLatin1()+'\n');
qDebug() << "SendCmd->" << sCmd;
}

void MainWindow::SelectPort()
{
    //startWatchDogTimer();
     *pPorts = pEnumerator->getPorts();
    if(pPorts->size()>0){
        qDebug() << "Scanning For Serial Port Devices (" << pPorts->size() << "found )";
       for (int i = 0; i < pPorts->size(); i++) {
           qDebug() << "  description " << pPorts->at(i).friendName;
           qDebug() << "  vendorID    " << pPorts->at(i).vendorID;
           qDebug() << "  productID   " << pPorts->at(i).productID;
           qDebug() << "  productID   " << pPorts->at(i).portName;
           ui->portNameComboBox->addItem(pPorts->at(i).portName);
       }
    }
}


void MainWindow::on_portNameComboBox_currentIndexChanged(int index)
{
    ui->portNameComboBox->setCurrentIndex(index);
    portName = pPorts->at(index).portName;
    friendName = pPorts->at(index).friendName;
    if(portName == "/dev/ttyACM0")
    {
        myCom = new QextSerialPort(portName,QextSerialPort::EventDriven);
        myCom ->open(QIODevice::ReadWrite); //打开串口
        myCom->setBaudRate(BAUD115200);
        myCom->setDataBits(DATA_8);
        myCom->setParity(PAR_NONE);
        myCom->setStopBits(STOP_1);
        myCom->setFlowControl(FLOW_OFF); //设置数据流控制，我们使用无数据流控制的默认设置
        myCom->setTimeout(500); //设置延时
        connect(myCom,SIGNAL(readyRead()),this,SLOT(readMyCom()));
        ui->statusBar->showMessage(friendName);
        ui->openduoji->setEnabled(true);
        ui->openprojector->setEnabled(true);
        ui->closeduoji->setEnabled(true);
        ui->closeprojector->setEnabled(true);
        ui->foreward->setEnabled(true);
        ui->overturn->setEnabled(true);
        ui->stop->setEnabled(true);
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(true);

    }
}
void MainWindow::readMyCom()
{
    QByteArray ba = myCom->readAll();


    int iCurPos = 0;
    int iInput = -1;
    char c;
        while(iCurPos<ba.size()){
                c = ba.at(iCurPos);
                iCurPos++;
                if(c!='\r') m_sSerialString+=QString(c);
                if(c=='\n'){
                    // Line Read Complete, process data

                   if(m_sSerialString.left(1) != "P"&& m_sSerialString.length()>0){
                        // We only emit this data for display & log purposes
                        if(m_sSerialString.left(1) == "C"){
                            qDebug() << m_sSerialString.right(m_sSerialString.length()-1) << "\n";
                            ui->textBrowser->insertPlainText( m_sSerialString.right(m_sSerialString.length()-1) );
                        }
                        else{
                            //emit BC_RawData(m_sSerialString);
                            qDebug() << m_sSerialString << "\n";
                            //ui->textBrowser->insertPlainText( m_sSerialString.right(m_sSerialString.length()-1) );
                        }
                    }

                   QFile file("D:\\log.txt");//cyp
                   file.open(QIODevice::WriteOnly | QIODevice::Append);
                   QTextStream text_stream(&file);
                   text_stream << "RECV:\t" << m_sSerialString << "\r\n";
                   file.flush();
                   file.close();

                    int iCmdID = m_sSerialString.left(1).toUpper().toLatin1().at(0);
                    switch (iCmdID){
                    case 'U':
                        iInput = m_sSerialString.right(m_sSerialString.length()-1).toInt();
                        if(iInput == 1){
                        qDebug() << "receive cmd U1 " << "\n";
                        }
                        else if(iInput == 2){
                        qDebug() << "receive cmd U2 " << "\n";
                        }
                        else if(iInput == 3){
                        qDebug() << "receive cmd U3 " << "\n";
                        }
                        else
                        qDebug() << "receive cmd U " << "\n";
                        break;
                    case 'Q':
                        break;
                    case 'P':
                        iInput = m_sSerialString.right(m_sSerialString.length()-1).toInt();
                        break;
                    case 'L':
                        break;

                    case 'X':
                        break;

                    case 'R':
                        break;

                    default:

                        break;
                    }
                    m_sSerialString=""; // Line processed, clear it for next line
                }
            }


}

void MainWindow::on_closeduoji_clicked()
{
    SendCmd("b");
}

void MainWindow::on_openprojector_clicked()
{
     SendCmd("d");
}

void MainWindow::on_closeprojector_clicked()
{
     SendCmd("e");
}

void MainWindow::on_lineEditCommand_returnPressed()
{
    SendCmd(ui->lineEditCommand->text());
    ui->lineEditCommand->clear();
}

void MainWindow::on_foreward_clicked()
{
    SendCmd("f1");
}

void MainWindow::on_overturn_clicked()
{
    SendCmd("f2");
}

void MainWindow::on_stop_clicked()
{
    SendCmd("f3");
}

void MainWindow::on_pushButton_clicked()
{
     SendCmd("L1");
}

void MainWindow::on_pushButton_2_clicked()
{
     SendCmd("L2");
}
