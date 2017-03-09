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
    CreatMenu();
    statueLable = new QLabel("未选择端口");
    ui->statusBar->addWidget(statueLable);
    bootload = new bootloadProcess();
    serialPort = new mySerialPort();
    connect(bootload, SIGNAL(writeData(QByteArray)), serialPort, SLOT(writeData(QByteArray)));
    connect(serialPort, SIGNAL(receiceData(QByteArray)), bootload, SLOT(receiveProcess(QByteArray)));
    connect(bootload, SIGNAL(deviceConnect()), this, SLOT(deviceConnect()));
    connect(bootload, SIGNAL(deviceDisconnect()), this, SLOT(deviceDisconnect()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/******************************************
 * @函数说明：
 * @输入参数：
 * @返回参数：
 * @修订日期：
******************************************/
void MainWindow::CreatMenu()
{
    QMenu *menu = ui->menuBar->addMenu(tr("SerialPort"));

    foreach (const QSerialPortInfo &Info, QSerialPortInfo::availablePorts())
    {
        QAction *action = new QAction(QIcon(":/ico/ico/Port.png"), Info.portName());
        menu->addAction(action);
    }
    connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(OpenCom(QAction*)));

    QMenu *menuUpdate = ui->menuBar->addMenu(tr("Tools"));
    QAction *action = new QAction(QIcon(":/ico/ico/Update.png"), "Update firmware");
    menuUpdate->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(updateFirmware(bool)));

    action = new QAction(QIcon(":/ico/ico/jump.png"), "Run firmware");
    menuUpdate->addAction(action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(runFirmware(bool)));

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
    serialPort->closePort();
    if (serialPort->openPortForDef(action->text()) == true)
    {
        statueLable->setText("正在监听： " + action->text());
        bootload->startHearbeat();
    }
    else
    {
        statueLable->setText("端口打开失败： " + action->text());
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
    bootload->runFirmware();
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
}
/******************************************
 * @函数说明：更新固件的动作的槽
 * @输入参数：
 * @返回参数：无
 * @修订日期：
******************************************/
void MainWindow::progDlogUpdate(uint32_t progress)
{
    progDlg->setValue(progress);

    qDebug()<<("%d", progress);
    if (progDlg->value() >= progDlg->maximum() && progDlg->wasCanceled() == false)
    {
        progDlg->cancel();
        QMessageBox message(QMessageBox::Information, "Update Firmware Sucess", "Do you want to run the firmware?", QMessageBox::Yes | QMessageBox::No, NULL);
        message.setIconPixmap(QPixmap(":/ico/ico/jump.png"));
        if(message.exec() == QMessageBox::Yes)
        {
            bootload->runFirmware();
        }
    }
}

/******************************************
 * @函数说明：更新固件的动作的槽
 * @输入参数：
 * @返回参数：无
 * @修订日期：
******************************************/
void MainWindow::progDlogCandeled()
{


}

/******************************************
 * @函数说明：更新固件的动作的槽
 * @输入参数：
 * @返回参数：无
 * @修订日期：
******************************************/
void MainWindow::deviceConnect()
{
    statueLable->setText("设备已连接···");
}
/******************************************
 * @函数说明：更新固件的动作的槽
 * @输入参数：
 * @返回参数：无
 * @修订日期：
******************************************/
void MainWindow::deviceDisconnect()
{
    statueLable->setText("设备已断开···");
}
