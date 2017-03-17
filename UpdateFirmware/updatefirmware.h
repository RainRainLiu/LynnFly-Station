#ifndef UPDATEFIRMWARE_H
#define UPDATEFIRMWARE_H

#include <QMainWindow>
#include "bootloadProcess.h"
namespace Ui {
class UpdateFirmware;
}

class UpdateFirmware : public QMainWindow
{
    Q_OBJECT

public:
    explicit UpdateFirmware(QWidget *parent = 0);
    ~UpdateFirmware();

private:
    Ui::UpdateFirmware *ui;
    bootloadProcess *boot;


public slots:
    void readData(QByteArray array);

signals:
    int writeData(QByteArray array);


private slots:
    void on_filePathButton_clicked();
    int bootloadWrite(QByteArray array);
    void bootloadEvent(bootloadProcess::BOOTLOAD_EVENT_T event, void *arg);
};

#endif // UPDATEFIRMWARE_H
