#include "chartdir.h"

int main(int argc, char *argv[])
{
    // The data for the chart
    double data[] = {90, 60, 65, 75, 40};

    // The labels for the chart
    const char *labels[] = {"Speed", "Reliability", "Comfort", "Safety", "Efficiency"};

    // Create a PolarChart object of size 450 x 350 pixels
    PolarChart *c = new PolarChart(450, 350);

    // Set center of plot area at (225, 185) with radius 150 pixels
    c->setPlotArea(225, 185, 150);

    // Add an area layer to the polar chart
    c->addAreaLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), 0x9999ff);

    // Set the labels to the angular axis as spokes
    c->angularAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Output the chart
    c->makeChart("simpleradar.png");

    //free up resources
    delete c;
    return 0;
}

