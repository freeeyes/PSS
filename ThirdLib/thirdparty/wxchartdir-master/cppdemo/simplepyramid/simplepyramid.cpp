#include "chartdir.h"

int main(int argc, char *argv[])
{
    // The data for the pyramid chart
    double data[] = {156, 123, 211, 179};

    // The labels for the pyramid chart
    const char *labels[] = {"Funds", "Bonds", "Stocks", "Cash"};

    // Create a PyramidChart object of size 360 x 360 pixels
    PyramidChart *c = new PyramidChart(360, 360);

    // Set the pyramid center at (180, 180), and width x height to 150 x 180 pixels
    c->setPyramidSize(180, 180, 150, 300);

    // Set the pyramid data and labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Add labels at the center of the pyramid layers using Arial Bold font. The labels will have
    // two lines showing the layer name and percentage.
    c->setCenterLabel("{label}\n{percent}%", "arialbd.ttf");

    // Output the chart
    c->makeChart("simplepyramid.png");

    //free up resources
    return 0;
}

