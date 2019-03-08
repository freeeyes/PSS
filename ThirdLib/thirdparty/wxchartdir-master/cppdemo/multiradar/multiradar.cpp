#include "chartdir.h"

int main(int argc, char *argv[])
{
    // The data for the chart
    double data0[] = {90, 60, 85, 75, 55};
    double data1[] = {60, 80, 70, 80, 85};

    // The labels for the chart
    const char *labels[] = {"Speed", "Reliability", "Comfort", "Safety", "Efficiency"};

    // Create a PolarChart object of size 480 x 380 pixels. Set background color to gold, with 1
    // pixel 3D border effect
    PolarChart *c = new PolarChart(480, 380, Chart::goldColor(), 0x000000, 1);

    // Add a title to the chart using 15pt Times Bold Italic font. The title text is white (ffffff)
    // on a deep blue (000080) background
    c->addTitle("Space Travel Vehicles Compared", "timesbi.ttf", 15, 0xffffff)->setBackground(
        0x000080);

    // Set plot area center at (240, 210), with 150 pixels radius, and a white (ffffff) background.
    c->setPlotArea(240, 210, 150, 0xffffff);

    // Add a legend box at top right corner (470, 35) using 10pt Arial Bold font. Set the background
    // to silver, with 1 pixel 3D border effect.
    LegendBox *b = c->addLegend(470, 35, true, "arialbd.ttf", 10);
    b->setAlignment(Chart::TopRight);
    b->setBackground(Chart::silverColor(), Chart::Transparent, 1);

    // Add an area layer to the chart using semi-transparent blue (0x806666cc). Add a blue
    // (0x6666cc) line layer using the same data with 3 pixel line width to highlight the border of
    // the area.
    c->addAreaLayer(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0x806666cc,
        "Model Saturn");
    c->addLineLayer(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0x6666cc
        )->setLineWidth(3);

    // Add an area layer to the chart using semi-transparent red (0x80cc6666). Add a red (0xcc6666)
    // line layer using the same data with 3 pixel line width to highlight the border of the area.
    c->addAreaLayer(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x80cc6666,
        "Model Jupiter");
    c->addLineLayer(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0xcc6666
        )->setLineWidth(3);

    // Set the labels to the angular axis as spokes.
    c->angularAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Output the chart
    c->makeChart("multiradar.png");

    //free up resources
    delete c;
    return 0;
}

