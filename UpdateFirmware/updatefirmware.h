#ifndef UPDATEFIRMWARE_H
#define UPDATEFIRMWARE_H

#include <QMainWindow>

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


public slots:
    void readData(QByteArray array);

signals:
    int writeData(QByteArray array);


private slots:
    void on_filePathButton_clicked();
};

#endif // UPDATEFIRMWARE_H
