#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "QtSerialPort/QSerialPortInfo"


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
    QMenu *menu = ui->menuBar->addMenu(tr("COM"));

    foreach (const QSerialPortInfo &Info, QSerialPortInfo::availablePorts())
    {
        QAction *action = new QAction(Info.portName());
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
    if (devMintor->montorSerialPort(action->text()) == true)
    {
        statueLable->setText("正在监听： " + action->text());
    }
    else
    {
        statueLable->setText("端口打开失败： " + action->text());
    }
}
