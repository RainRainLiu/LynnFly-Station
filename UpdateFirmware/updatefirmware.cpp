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


void UpdateFirmware::readData(QByteArray array)
{

}

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
