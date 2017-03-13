#include "updatefirmware.h"
#include "ui_updatefirmware.h"

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
