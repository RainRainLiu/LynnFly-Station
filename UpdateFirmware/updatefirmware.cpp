#include "updatefirmware.h"
#include "ui_updatefirmware.h"
#include "QFileDialog"
#include <QMessageBox>

UpdateFirmware::UpdateFirmware(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UpdateFirmware)
{
    ui->setupUi(this);
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
