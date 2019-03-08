#include "chartdir.h"

int main(int argc, char *argv[])
{
    // The XY data of the first data series
    double dataX0[] = {10, 35, 17, 4, 22, 29, 45, 52, 63, 39};
    double dataY0[] = {2.0, 3.2, 2.7, 1.2, 2.8, 2.9, 3.1, 3.0, 2.3, 3.3};

    // The XY data of the second data series
    double dataX1[] = {30, 35, 17, 4, 22, 59, 43, 52, 63, 39};
    double dataY1[] = {1.0, 1.3, 0.7, 0.6, 0.8, 3.0, 1.8, 2.3, 3.4, 1.5};

    // The XY data of the third data series
    double dataX2[] = {28, 35, 15, 10, 22, 60, 46, 64, 39};
    double dataY2[] = {2.0, 2.2, 1.2, 0.4, 1.8, 2.7, 2.4, 2.8, 2.4};

    // Create a XYChart object of size 540 x 480 pixels
    XYChart *c = new XYChart(540, 480);

    // Set the plotarea at (70, 65) and of size 400 x 350 pixels, with white background and a light
    // grey border (0xc0c0c0). Turn on both horizontal and vertical grid lines with light grey color
    // (0xc0c0c0)
    c->setPlotArea(70, 65, 400, 350, 0xffffff, -1, 0xc0c0c0, 0xc0c0c0, -1);

    // Add a legend box with the top center point anchored at (270, 30). Use horizontal layout. Use
    // 10pt Arial Bold Italic font. Set the background and border color to Transparent.
    LegendBox *legendBox = c->addLegend(270, 30, false, "arialbi.ttf", 10);
    legendBox->setAlignment(Chart::TopCenter);
    legendBox->setBackground(Chart::Transparent, Chart::Transparent);

    // Add a title to the chart using 18 point Times Bold Itatic font.
    c->addTitle("Parametric Curve Fitting", "timesbi.ttf", 18);

    // Add titles to the axes using 12pt Arial Bold Italic font
    c->yAxis()->setTitle("Axis Title Placeholder", "arialbi.ttf", 12);
    c->xAxis()->setTitle("Axis Title Placeholder", "arialbi.ttf", 12);

    // Set the axes line width to 3 pixels
    c->yAxis()->setWidth(3);
    c->xAxis()->setWidth(3);

    // Add a scatter layer using (dataX0, dataY0)
    c->addScatterLayer(DoubleArray(dataX0, (int)(sizeof(dataX0) / sizeof(dataX0[0]))), DoubleArray(
        dataY0, (int)(sizeof(dataY0) / sizeof(dataY0[0]))), "Polynomial", Chart::GlassSphere2Shape,
        11, 0xff0000);

    // Add a degree 2 polynomial trend line layer for (dataX0, dataY0)
    TrendLayer *trend0 = c->addTrendLayer(DoubleArray(dataX0, (int)(sizeof(dataX0) / sizeof(dataX0[0
        ]))), DoubleArray(dataY0, (int)(sizeof(dataY0) / sizeof(dataY0[0]))), 0xff0000);
    trend0->setLineWidth(3);
    trend0->setRegressionType(Chart::PolynomialRegression(2));

    // Add a scatter layer for (dataX1, dataY1)
    c->addScatterLayer(DoubleArray(dataX1, (int)(sizeof(dataX1) / sizeof(dataX1[0]))), DoubleArray(
        dataY1, (int)(sizeof(dataY1) / sizeof(dataY1[0]))), "Exponential", Chart::GlassSphere2Shape,
        11, 0x00aa00);

    // Add an exponential trend line layer for (dataX1, dataY1)
    TrendLayer *trend1 = c->addTrendLayer(DoubleArray(dataX1, (int)(sizeof(dataX1) / sizeof(dataX1[0
        ]))), DoubleArray(dataY1, (int)(sizeof(dataY1) / sizeof(dataY1[0]))), 0x00aa00);
    trend1->setLineWidth(3);
    trend1->setRegressionType(Chart::ExponentialRegression);

    // Add a scatter layer using (dataX2, dataY2)
    c->addScatterLayer(DoubleArray(dataX2, (int)(sizeof(dataX2) / sizeof(dataX2[0]))), DoubleArray(
        dataY2, (int)(sizeof(dataY2) / sizeof(dataY2[0]))), "Logarithmic", Chart::GlassSphere2Shape,
        11, 0x0000ff);

    // Add a logarithmic trend line layer for (dataX2, dataY2)
    TrendLayer *trend2 = c->addTrendLayer(DoubleArray(dataX2, (int)(sizeof(dataX2) / sizeof(dataX2[0
        ]))), DoubleArray(dataY2, (int)(sizeof(dataY2) / sizeof(dataY2[0]))), 0x0000ff);
    trend2->setLineWidth(3);
    trend2->setRegressionType(Chart::LogarithmicRegression);

    // Output the chart
    c->makeChart("paramcurve.png");

    //free up resources
    delete c;
    return 0;
}

