#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QLabel"
#include "Communication/deceivemontor.h"

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

private:
    Ui::MainWindow *ui;
    void CreatMenu();

    QLabel *statueLable;
    deceiveMontor *devMintor;
};

#endif // MAINWINDOW_H
