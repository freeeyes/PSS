#include "chartdir.h"

int main(int argc, char *argv[])
{
    // The data for the bar chart
    double data0[] = {100, 115, 165, 107, 67};
    double data1[] = {85, 106, 129, 161, 123};
    double data2[] = {67, 87, 86, 167, 157};

    // The labels for the bar chart
    const char *labels[] = {"Mon", "Tue", "Wed", "Thu", "Fri"};

    // Create a XYChart object of size 600 x 360 pixels
    XYChart *c = new XYChart(600, 360);

    // Set the plotarea at (70, 20) and of size 400 x 300 pixels, with transparent background and
    // border and light grey (0xcccccc) horizontal grid lines
    c->setPlotArea(70, 20, 400, 300, Chart::Transparent, -1, Chart::Transparent, 0xcccccc);

    // Add a legend box at (480, 20) using vertical layout and 12pt Arial font. Set background and
    // border to transparent and key icon border to the same as the fill color.
    LegendBox *b = c->addLegend(480, 20, true, "arial.ttf", 12);
    b->setBackground(Chart::Transparent, Chart::Transparent);
    b->setKeyBorder(Chart::SameAsMainColor);

    // Set the x and y axis stems to transparent and the label font to 12pt Arial
    c->xAxis()->setColors(Chart::Transparent);
    c->yAxis()->setColors(Chart::Transparent);
    c->xAxis()->setLabelStyle("arial.ttf", 12);
    c->yAxis()->setLabelStyle("arial.ttf", 12);

    // Add a stacked bar layer
    BarLayer *layer = c->addBarLayer(Chart::Stack);

    // Add the three data sets to the bar layer
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0xaaccee,
        "Server # 1");
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0xbbdd88,
        "Server # 2");
    layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), 0xeeaa66,
        "Server # 3");

    // Set the bar border to transparent
    layer->setBorderColor(Chart::Transparent);

    // Enable labelling for the entire bar and use 12pt Arial font
    layer->setAggregateLabelStyle("arial.ttf", 12);

    // Enable labelling for the bar segments and use 12pt Arial font with center alignment
    layer->setDataLabelStyle("arial.ttf", 10)->setAlignment(Chart::Center);

    // For a vertical stacked bar with positive data, the first data set is at the bottom. For the
    // legend box, by default, the first entry at the top. We can reverse the legend order to make
    // the legend box consistent with the stacked bar.
    layer->setLegendOrder(Chart::ReverseLegend);

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // For the automatic y-axis labels, set the minimum spacing to 40 pixels.
    c->yAxis()->setTickDensity(40);

    // Add a title to the y axis using dark grey (0x555555) 14pt Arial Bold font
    c->yAxis()->setTitle("Y-Axis Title Placeholder", "arialbd.ttf", 14, 0x555555);

    // Output the chart
    c->makeChart("stackedbar.png");

    //free up resources
    delete c;
    return 0;
}

