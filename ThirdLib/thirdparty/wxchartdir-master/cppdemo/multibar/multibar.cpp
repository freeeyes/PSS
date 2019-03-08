#include "chartdir.h"

int main(int argc, char *argv[])
{
    // The data for the bar chart
    double data0[] = {100, 125, 245, 147, 67};
    double data1[] = {85, 156, 179, 211, 123};
    double data2[] = {97, 87, 56, 267, 157};
    const char *labels[] = {"Mon", "Tue", "Wed", "Thu", "Fri"};

    // Create a XYChart object of size 400 x 240 pixels
    XYChart *c = new XYChart(400, 240);

    // Add a title to the chart using 10 pt Arial font
    c->addTitle("         Average Weekday Network Load", "", 10);

    // Set the plot area at (50, 25) and of size 320 x 180. Use two alternative background colors
    // (0xffffc0 and 0xffffe0)
    c->setPlotArea(50, 25, 320, 180, 0xffffc0, 0xffffe0);

    // Add a legend box at (55, 18) using horizontal layout. Use 8 pt Arial font, with transparent
    // background
    c->addLegend(55, 18, false, "", 8)->setBackground(Chart::Transparent);

    // Add a title to the y-axis
    c->yAxis()->setTitle("Throughput (MBytes Per Hour)");

    // Reserve 20 pixels at the top of the y-axis for the legend box
    c->yAxis()->setTopMargin(20);

    // Set the x axis labels
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Add a multi-bar layer with 3 data sets and 3 pixels 3D depth
    BarLayer *layer = c->addBarLayer(Chart::Side, 3);
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0xff8080,
        "Server #1");
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x80ff80,
        "Server #2");
    layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), 0x8080ff,
        "Server #3");

    // Output the chart
    c->makeChart("multibar.png");

    //free up resources
    delete c;
    return 0;
}

