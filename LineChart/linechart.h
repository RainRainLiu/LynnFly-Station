#ifndef LINECHART_H
#define LINECHART_H

#include <QMainWindow>
#include <QtCharts>
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

private:
    Ui::LineChart *ui;
};

#endif // LINECHART_H
