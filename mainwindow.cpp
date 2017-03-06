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
    devMintor = new deceiveMontor();
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
    if (devMintor->montorSerialPort(action->text()) == true)
    {
        statueLable->setText("正在监听： " + action->text());
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

    if (devMintor->updateFirmware(binArray) == true)
    {
        progDlg = new QProgressDialog();
        progDlg->setWindowTitle("Please wait...");
        progDlg->setFixedWidth(500);
        progDlg->setRange(0, file->size());
        progDlg->show();
        progDlg->setValue(100);

        connect(progDlg, SIGNAL(canceled()), this, SLOT(progDlogCandeled));
        connect(devMintor,SIGNAL(updateFirmwareProgress(QString,uint32_t,bool)),
                this, SLOT(progDlogUpdate(QString,uint32_t,bool)));
    }
}
/******************************************
 * @函数说明：更新固件的动作的槽
 * @输入参数：
 * @返回参数：无
 * @修订日期：
******************************************/
void MainWindow::progDlogUpdate(QString title, uint32_t progress, bool end)
{
    end = end;
    progDlg->setWindowTitle(title);
    progDlg->setValue(progress);
    progDlg->show();
}

/******************************************
 * @函数说明：更新固件的动作的槽
 * @输入参数：
 * @返回参数：无
 * @修订日期：
******************************************/
void MainWindow::progDlogCandeled()
{
    disconnect(devMintor,SIGNAL(updateFirmwareProgress(QString,uint32_t,bool)),
               this, SLOT(progDlogUpdate(QString,uint32_t,bool)));
}
