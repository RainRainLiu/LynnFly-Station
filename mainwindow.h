#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QLabel"
#include "QProgressDialog"
#include "Componemts/myserialport.h"
#include "UpdateFirmware/updatefirmware.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void OpenCom(QAction * action);
    void updateFirmware(bool b);
    void progDlogUpdate(uint32_t progress);

    void runFirmware(bool b);

    void on_updateFirmwareButton_clicked();

    void on_serialPortComboBox_activated(const QString &arg1);

private:
    Ui::MainWindow *ui;

    UpdateFirmware *updateWindow;
    void CreatMenu();
    QLabel *statueLable;
    mySerialPort *serialPort;

};

#endif // MAINWINDOW_H
