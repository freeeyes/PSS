#include "chartdir.h"

int main(int argc, char *argv[])
{
    // The XYZ points for the bubble chart
    double dataX0[] = {150, 300, 1000, 1700};
    double dataY0[] = {12, 60, 25, 65};
    double dataZ0[] = {20, 50, 50, 85};

    double dataX1[] = {500, 1000, 1300};
    double dataY1[] = {35, 50, 75};
    double dataZ1[] = {30, 55, 95};

    // Create a XYChart object of size 450 x 420 pixels
    XYChart *c = new XYChart(450, 420);

    // Set the plotarea at (55, 65) and of size 350 x 300 pixels, with a light grey border
    // (0xc0c0c0). Turn on both horizontal and vertical grid lines with light grey color (0xc0c0c0)
    c->setPlotArea(55, 65, 350, 300, -1, -1, 0xc0c0c0, 0xc0c0c0, -1);

    // Add a legend box at (50, 30) (top of the chart) with horizontal layout. Use 12pt Times Bold
    // Italic font. Set the background and border color to Transparent.
    c->addLegend(50, 30, false, "timesbi.ttf", 12)->setBackground(Chart::Transparent);

    // Add a title to the chart using 18pt Times Bold Itatic font.
    c->addTitle("Product Comparison Chart", "timesbi.ttf", 18);

    // Add a title to the y axis using 12pt Arial Bold Italic font
    c->yAxis()->setTitle("Capacity (tons)", "arialbi.ttf", 12);

    // Add a title to the x axis using 12pt Arial Bold Italic font
    c->xAxis()->setTitle("Range (miles)", "arialbi.ttf", 12);

    // Set the axes line width to 3 pixels
    c->xAxis()->setWidth(3);
    c->yAxis()->setWidth(3);

    // Add (dataX0, dataY0) as a scatter layer with semi-transparent red (0x80ff3333) circle
    // symbols, where the circle size is modulated by dataZ0. This creates a bubble effect.
    c->addScatterLayer(DoubleArray(dataX0, (int)(sizeof(dataX0) / sizeof(dataX0[0]))), DoubleArray(
        dataY0, (int)(sizeof(dataY0) / sizeof(dataY0[0]))), "Technology AAA", Chart::CircleSymbol,
        9, 0x80ff3333, 0x80ff3333)->setSymbolScale(DoubleArray(dataZ0, (int)(sizeof(dataZ0) /
        sizeof(dataZ0[0]))));

    // Add (dataX1, dataY1) as a scatter layer with semi-transparent green (0x803333ff) circle
    // symbols, where the circle size is modulated by dataZ1. This creates a bubble effect.
    c->addScatterLayer(DoubleArray(dataX1, (int)(sizeof(dataX1) / sizeof(dataX1[0]))), DoubleArray(
        dataY1, (int)(sizeof(dataY1) / sizeof(dataY1[0]))), "Technology BBB", Chart::CircleSymbol,
        9, 0x803333ff, 0x803333ff)->setSymbolScale(DoubleArray(dataZ1, (int)(sizeof(dataZ1) /
        sizeof(dataZ1[0]))));

    // Output the chart
    c->makeChart("bubble.png");

    //free up resources
    delete c;
    return 0;
}

