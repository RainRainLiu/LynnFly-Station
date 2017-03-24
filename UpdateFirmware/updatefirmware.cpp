#include "updatefirmware.h"
#include "ui_updatefirmware.h"
#include "QFileDialog"
#include <QMessageBox>

UpdateFirmware::UpdateFirmware(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UpdateFirmware)
{
    ui->setupUi(this);
    boot = new bootloadProcess();
    boot->startHearbeat();
    connect(boot, SIGNAL(writeData(QByteArray)),this, SLOT(bootloadWrite(QByteArray)));
    connect(boot, SIGNAL(bootloadEvent(bootloadProcess::BOOTLOAD_EVENT_T,void*)),
            this ,SLOT(bootloadEvent(bootloadProcess::BOOTLOAD_EVENT_T,void*)));
    connect(boot, SIGNAL(bootloadInfo(QString,QString,bool)), this, SLOT(bootloadInfo(QString,QString,bool)));
}

UpdateFirmware::~UpdateFirmware()
{
    delete ui;

}

/******************************************
 * @函数说明：升级设备固件
 * @输入参数：QByteArray binByteArray 固件
 * @返回参数：无
 * @修订日期：
******************************************/
void UpdateFirmware::readData(QByteArray array)
{
    boot->receiveProcess(array);

}
/******************************************
 * @函数说明：升级设备固件
 * @输入参数：QByteArray binByteArray 固件
 * @返回参数：无
 * @修订日期：
******************************************/
int UpdateFirmware::bootloadWrite(QByteArray array)
{
    return emit writeData(array);

}

/******************************************
 * @函数说明：升级设备固件
 * @输入参数：QByteArray binByteArray 固件
 * @返回参数：无
 * @修订日期：
******************************************/
void UpdateFirmware::bootloadEvent(bootloadProcess::BOOTLOAD_EVENT_T event, void *arg)
{
    QString text;
    switch (event) {
    case bootloadProcess::CONNECT_STATE:
        if (*(bool*)(arg))
        {
            text = "Device Connected!";
            ui->eraseButton->setEnabled(true);
            ui->updateButton->setEnabled(true);
            ui->runButton->setEnabled(true);
        }
        else
        {
            text = "Device Disconnected!";
        }
        break;
    case bootloadProcess::EARSE_RESULT:
        break;
    case bootloadProcess::WRITE_ERROR:
        break;
    case bootloadProcess::DLOWNLOAD_PROGRESS:
        break;
    default:
        break;
    }
    ui->statusbar->showMessage(text, 0xffffffff);
}

/******************************************
 * @函数说明：升级设备固件
 * @输入参数：QByteArray binByteArray 固件
 * @返回参数：无
 * @修订日期：
******************************************/
void UpdateFirmware::on_filePathButton_clicked()
{
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
    ui->pathEdit->setText(fileName);

    QDataStream binFile(file);

    char buff[file->size()];

    binFile.readRawData(buff, (qint64)file->size());    //读取文件

    QByteArray binArray(buff, file->size()); //转换为QByteArray
}

/******************************************
 * @函数说明：升级设备固件
 * @输入参数：QByteArray binByteArray 固件
 * @返回参数：无
 * @修订日期：
******************************************/
void UpdateFirmware::bootloadInfo(QString bootVsersion, QString firemwareVersion, bool state)
{
    ui->bootloaderVersionLabel->setText(bootVsersion);

    if (state)
    {
        ui->firmwareVersionLabel->setText(firemwareVersion);
        ui->runButton->setEnabled(true);
    }
    else
    {
        ui->firmwareVersionLabel->setText("Invalid");
        ui->runButton->setEnabled(false);
    }

}
