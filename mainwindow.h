#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QLabel"
#include "Communication/deceivemontor.h"
#include "QProgressDialog"
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
    void progDlogCandeled();
    void progDlogUpdate(QString title, uint32_t progress, bool end);

private:
    Ui::MainWindow *ui;
    void CreatMenu();
    QProgressDialog *progDlg;
    QLabel *statueLable;
    deceiveMontor *devMintor;
};

#endif // MAINWINDOW_H
