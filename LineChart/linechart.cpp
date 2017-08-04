#include "linechart.h"
#include "ui_linechart.h"

LineChart::LineChart(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LineChart)
{
    ui->setupUi(this);

    gestureProcess = new gesture();
    connect(gestureProcess, SIGNAL(OutData(QByteArray)), this, SLOT(OutData_slots(QByteArray)));
    connect(gestureProcess, SIGNAL(GestureUpdate(GESTURE_TYPE_T,uint32_t)), this, SLOT(seriesUpdate(gesture::GESTURE_TYPE_T,uint32_t)));

    seriesGX = new QLineSeries();
    seriesGY = new QLineSeries();
    seriesGZ = new QLineSeries();
    seriesAX = new QLineSeries();
    seriesAY = new QLineSeries();
    seriesAZ = new QLineSeries();
    seriesANX = new QLineSeries();
    seriesANY = new QLineSeries();
    seriesANZ = new QLineSeries();

    seriesGX->setName("gyroX");
    seriesGY->setName("gyroY");
    seriesGZ->setName("gyroZ");
    seriesAX->setName("accX");
    seriesAY->setName("accY");
    seriesAZ->setName("accZ");
    seriesANX->setName("angleX");
    seriesANY->setName("angleY");
    seriesANZ->setName("angleZ");


    chart = new QChart();

    chart->addSeries(seriesGX);
    chart->addSeries(seriesGY);
    chart->addSeries(seriesGZ);
    chart->addSeries(seriesAX);
    chart->addSeries(seriesAY);
    chart->addSeries(seriesAZ);
    chart->addSeries(seriesANX);
    chart->addSeries(seriesANY);
    chart->addSeries(seriesANZ);


    //chart->setTitle("Rain");


    axisX = new QValueAxis;
    axisX->setLabelFormat("%d");
    axisX->setTickCount(LINE_CHART_X_LENG);
    axisX->setRange(0, LINE_CHART_X_LENG);
    chart->addAxis(axisX, Qt::AlignBottom);

    axisY = new QValueAxis;
    axisY->setLabelFormat("%d");
    axisY->setTickCount(10);
    axisY->setRange(0,10);


    chart->addAxis(axisY, Qt::AlignLeft);
    chart->setAnimationOptions(QChart::SeriesAnimations);//设置曲线呈动画显示

    chart->setTheme(QChart::ChartThemeDark);
    ui->widget->setChart(chart);
}

LineChart::~LineChart()
{
    delete ui;
}



void LineChart::on_horizontalSlider_X_valueChanged(int value)
{
    chart->axisX()->setRange(0, value);
    axisX->setTickCount(value);
}

void LineChart::on_checkBox_gyroX_toggled(bool checked)
{
    if (checked)
    {
        chart->addSeries(seriesGX);
    }
    else
    {
        chart->removeSeries(seriesGX);
    }
}

void LineChart::on_checkBox_gyroY_toggled(bool checked)
{
    if (checked)
    {
        chart->addSeries(seriesGY);
    }
    else
    {
        chart->removeSeries(seriesGY);
    }
}

void LineChart::on_checkBox_gyroZ_toggled(bool checked)
{
    if (checked)
    {
        chart->addSeries(seriesGZ);
    }
    else
    {
        chart->removeSeries(seriesGZ);
    }
}

void LineChart::on_checkBox_accX_toggled(bool checked)
{
    if (checked)
    {
        chart->addSeries(seriesAX);
    }
    else
    {
        chart->removeSeries(seriesAX);
    }
}

void LineChart::on_checkBox_accY_toggled(bool checked)
{
    if (checked)
    {
        chart->addSeries(seriesAY);
    }
    else
    {
        chart->removeSeries(seriesAY);
    }
}

void LineChart::on_checkBox_accZ_toggled(bool checked)
{
    if (checked)
    {
        chart->addSeries(seriesAZ);
    }
    else
    {
        chart->removeSeries(seriesAZ);
    }
}

void LineChart::on_checkBox_angleX_toggled(bool checked)
{
    if (checked)
    {
        chart->addSeries(seriesANX);
    }
    else
    {
        chart->removeSeries(seriesANX);
    }
}

void LineChart::on_checkBox_anbleZ_toggled(bool checked)
{
    if (checked)
    {
        chart->addSeries(seriesANZ);
    }
    else
    {
        chart->removeSeries(seriesANZ);
    }
}

void LineChart::on_checkBox_angleY_toggled(bool checked)
{
    if (checked)
    {
        chart->addSeries(seriesANY);
    }
    else
    {
        chart->removeSeries(seriesANY);
    }
}


void LineChart::seriesUpdate(gesture::GESTURE_TYPE_T type, uint32_t data)
{
    QLineSeries *series = NULL;
    switch (type) {
        case gesture::GESTURE_ACC_X:
            series = seriesAX;
            break;
        case gesture::GESTURE_ACC_Y:
            series = seriesAY;
            break;
        case gesture::GESTURE_ACC_Z:
            series = seriesAZ;
            break;
        case gesture::GESTURE_GYRO_X:
            series = seriesGX;
            break;
        case gesture::GESTURE_GYRO_Y:
            series = seriesGY;
            break;
        case gesture::GESTURE_GYRO_Z:
            series = seriesGZ;
            break;
        case gesture::GESTURE_ANGLE_X:
           series =  seriesANX;
            break;
        case gesture::GESTURE_ANGLE_Y:
            series = seriesANY;
            break;
        case gesture::GESTURE_ANGLE_Z:
            series = seriesANZ;
            break;
        default:
            break;
    }

    if (series != NULL)
    {
        series->append(series->count() + 1, data);

        if (data > axisY->max())
        {
            axisY->setMax(data);
        }

        if (series->count() > axisX->max())
        {
            axisX->setRange(series->count() - LINE_CHART_X_LENG, series->count());
        }

        if (series->count() > LINE_CHART_X_LENG)
        {
            series->removePoints(0, 1); //删除最前面的
        }

    }

}
