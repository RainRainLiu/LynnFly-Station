#include "updatefirmware.h"
#include "ui_updatefirmware.h"
#include "QFileDialog"
#include <QMessageBox>

UpdateFirmware::UpdateFirmware(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UpdateFirmware)
{
    ui->setupUi(this);
    update = false;
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
            ui->updateButton->setEnabled(true);
            boot->getInfo();
        }
        else
        {
            text = "Device Disconnected!";
            ui->updateButton->setEnabled(false);
            ui->runButton->setEnabled(false);
            ui->eraseButton->setEnabled(false);
        }
        break;
    case bootloadProcess::EARSE_RESULT:
        if (*(bool*)(arg))
        {
            text = "Earse Sucess!";
            if (update == false)
            {
                boot->getInfo();
            }
        }
        else
        {
            text = "Earse Fail!";
            QMessageBox::warning(this,"Error","Earse Fail!",QMessageBox::Ok);
        }
        break;
    case bootloadProcess::WRITE_ERROR:
        break;
    case bootloadProcess::DLOWNLOAD_PROGRESS:

        text = "Download is in progress";

        ui->progressBar->setValue(*(int*)arg);
        break;
    case bootloadProcess::DLOWNLOAD_RESULT:
        update = false;
        if (*(bool*)(arg))
        {
            text = "Dolwnload Sucess!";
            boot->getInfo();
        }
        else
        {
            QMessageBox::warning(this,"Error","Download firmware fail!",QMessageBox::Ok);
            text = "Dolwnload Fail!";
        }
        break;

    case bootloadProcess::RUN_FIRMWARE_RESULT:
        if (*(bool*)(arg))
        {
            text = "Run Sucess!";
            boot->getInfo();
        }
        else
        {
            QMessageBox::warning(this,"Error","Run firmware fail!",QMessageBox::Ok);
            text = "Run Fail!";
        }
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
        ui->eraseButton->setEnabled(true);
    }
    else
    {
        ui->firmwareVersionLabel->setText("Invalid");
        ui->runButton->setEnabled(false);
        ui->eraseButton->setEnabled(false);
    }

}
/******************************************
 * @函数说明：升级设备固件
 * @输入参数：QByteArray binByteArray 固件
 * @返回参数：无
 * @修订日期：
******************************************/
void UpdateFirmware::on_updateButton_clicked()
{
    if (ui->pathEdit->text() == "")
    {
        QMessageBox::warning(this,"Error","Please select a file",QMessageBox::Ok);
        return;
    }

    QFile *file = new QFile(ui->pathEdit->text());

    if(!file->open(QIODevice::ReadOnly))    //文件存在可以打开
    {
        QMessageBox::warning(this,"Error","Cann't open file",QMessageBox::Ok);
        return;
    }

    QDataStream binFile(file);

    //char buff[file->size()];
    char *buff = (char *)malloc(file->size());

    binFile.readRawData(buff, (qint64)file->size());    //读取文件

    QByteArray binArray(buff, file->size()); //转换为QByteArray

    ui->progressBar->setEnabled(true);
    ui->progressBar->setRange(0,binArray.length());
    ui->progressBar->setValue(0);

    update = true;
    bool ok;
    boot->updateFirmware(binArray, ui->versionEdit->text(),(uint32_t)ui->addressEdit->text().toInt(&ok, 0));

    free(buff);
}

void UpdateFirmware::on_eraseButton_clicked()
{
    bool ok;
    boot->erase(0, (uint32_t)ui->addressEdit->text().toInt(&ok, 0));
}

void UpdateFirmware::closeEvent(QCloseEvent *event)
{
    event = event;
}

void UpdateFirmware::on_runButton_clicked()
{
    boot->runFirmware();
}
