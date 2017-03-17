#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "QtSerialPort/QSerialPortInfo"
#include "QFileDialog"
#include "QDebug"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serialPort = new mySerialPort();



    CreatMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CreatMenu()
{
    QMenu *menu = ui->menuBar->addMenu(tr("SerialPort"));

    QAction *action = new QAction(QIcon(":/ico/ico/find.png"), "Scan Port");
    menu->addAction(action);

    foreach (const QSerialPortInfo &Info, QSerialPortInfo::availablePorts())
    {
        QAction *action = new QAction(QIcon(":/ico/ico/Port.png"), Info.portName());
        menu->addAction(action);
    }
    connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(OpenCom(QAction*)));
}


/******************************************
 * @函数说明：选择端口的信号槽
 * @输入参数：QAction * action 读取action->text
 *          作为端口号的名称
 * @返回参数：无
 * @修订日期：
******************************************/
void MainWindow::OpenCom(QAction * action)
{
    if (action->text() == "Scan Port")
    {
        ui->menuBar->clear();
        CreatMenu();
        return;
    }
    serialPort->closePort();
    if (serialPort->openPortForDef(action->text()) == false)
    {
        ui->updateFirmwareButton->setEnabled(false);


        QMessageBox message(QMessageBox::Information, "Serial Port Error", "Unable to open port!", QMessageBox::Yes, NULL);
        message.setIconPixmap(QPixmap(":/ico/ico/close.png"));
        message.exec();
    }
    else
    {
        ui->updateFirmwareButton->setEnabled(true);
    }
}

/******************************************
 * @函数说明：更新固件的动作的槽
 * @输入参数：
 * @返回参数：无
 * @修订日期：
******************************************/
void MainWindow::runFirmware(bool b)
{
    b = b;
}

/******************************************
 * @函数说明：更新固件的动作的槽
 * @输入参数：
 * @返回参数：无
 * @修订日期：
******************************************/
void MainWindow::updateFirmware(bool b)
{
    b = b;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                      "",
                                                      tr("Firmware (*.bin)"));
    if (fileName == "")
    {
        return;
    }

    QFile *file = new QFile(fileName);

    if(!file->open(QIODevice::ReadOnly))    //文件存在可以打开
    {
        QMessageBox::warning(this,"Error","Cann't open file",QMessageBox::Ok);
        return;
    }

    QDataStream binFile(file);

    char buff[file->size()];

    binFile.readRawData(buff, (qint64)file->size());    //读取文件

    QByteArray binArray(buff, file->size()); //转换为QByteArray
/*
    if (bootload->updateFirmware(binArray) == true)
    {
        progDlg = new QProgressDialog();
        progDlg->setWindowTitle("Downlaod Firmware...");
        progDlg->setFixedWidth(500);
        progDlg->setRange(0, file->size());
        progDlg->show();
        progDlg->setAutoClose(true);
        progDlg->setAutoReset(false);
        progDlg->setLabelText("Downlaod Firmware");

        connect(progDlg, SIGNAL(canceled()), this, SLOT(progDlogCandeled));
        connect(bootload,SIGNAL(updateFirmwareProgress(uint32_t)),
                this, SLOT(progDlogUpdate(uint32_t)));
    }
    */
}
/******************************************
 * @函数说明：更新固件的动作的槽
 * @输入参数：
 * @返回参数：无
 * @修订日期：
******************************************/
void MainWindow::progDlogUpdate(uint32_t progress)
{/*
    progDlg->setValue(progress);
    progDlg->show();

    if (progDlg->value() >= progDlg->maximum())
    {
        progDlg->cancel();
        QMessageBox message(QMessageBox::Information, "Update Firmware Sucess", "Do you want to run firmware?", QMessageBox::Yes | QMessageBox::No, NULL);
        message.setIconPixmap(QPixmap(":/ico/ico/jump.png"));
        if(message.exec() == QMessageBox::Yes)
        {
            bootload->runFirmware();
        }
    }*/
}







void MainWindow::on_updateFirmwareButton_clicked()
{
    updateWindow = new UpdateFirmware();
    connect(serialPort, SIGNAL(receiceData(QByteArray)), updateWindow, SLOT(readData(QByteArray)));
    connect(updateWindow,SIGNAL(writeData(QByteArray)), serialPort, SLOT(writeData(QByteArray)));
    updateWindow->show();
}


void MainWindow::on_serialPortComboBox_activated(const QString &arg1)
{
    serialPort->closePort();
    if (serialPort->openPortForDef(arg1) == false)
    {
        QMessageBox message(QMessageBox::Information, "Serial Port Error", "Unable to open port!", QMessageBox::Yes, NULL);
        message.setIconPixmap(QPixmap(":/ico/ico/close.png"));
        message.exec();
    }
}
