#ifndef LINECHART_H
#define LINECHART_H

#include <QMainWindow>
#include <QtCharts>
#include <LineChart/gesture.h>

#define LINE_CHART_X_LENG   20

using namespace QtCharts;

namespace Ui {
class LineChart;
}

class LineChart : public QMainWindow
{
    Q_OBJECT

public:
    explicit LineChart(QWidget *parent = 0);
    ~LineChart();

private slots:

    void on_horizontalSlider_X_valueChanged(int value);

    void on_checkBox_gyroX_toggled(bool checked);

    void on_checkBox_gyroY_toggled(bool checked);

    void on_checkBox_gyroZ_toggled(bool checked);

    void on_checkBox_accX_toggled(bool checked);

    void on_checkBox_accY_toggled(bool checked);

    void on_checkBox_accZ_toggled(bool checked);

    void on_checkBox_angleX_toggled(bool checked);

    void on_checkBox_anbleZ_toggled(bool checked);

    void on_checkBox_angleY_toggled(bool checked);

    void OutData_slots(QByteArray byteData)
    {
        emit OutData(byteData);
    }

    void seriesUpdate(gesture::GESTURE_TYPE_T type, uint32_t data);

private:
    Ui::LineChart *ui;
    QChart *chart;
    QValueAxis *axisX;
    QValueAxis *axisY;

    QLineSeries *seriesGX;
    QLineSeries *seriesGY;
    QLineSeries *seriesGZ;

    QLineSeries *seriesAX;
    QLineSeries *seriesAY;
    QLineSeries *seriesAZ;

    QLineSeries *seriesANX;
    QLineSeries *seriesANY;
    QLineSeries *seriesANZ;
    gesture *gestureProcess;

signals:
    void OutData(QByteArray byteData);

public slots:
    void InData(QByteArray byteData)
    {
        gestureProcess->InData(byteData);
    }

};

#endif // LINECHART_H
