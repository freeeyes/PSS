#include "chartdir.h"

int main(int argc, char *argv[])
{
    // The (x, y) coordinates of the starting point of the vectors
    double dataX[] = {20, 40, 60, 80, 100, 20, 40, 60, 80, 100, 20, 40, 58, 62, 80, 100, 20, 40, 60,
        80, 100, 20, 40, 60, 80, 100};
    double dataY[] = {20, 20, 20, 20, 20, 40, 40, 40, 40, 40, 60, 60, 60, 60, 60, 60, 80, 80, 80,
        80, 80, 100, 100, 100, 100, 100};

    // The lengths (radii) and directions (angles) of the vectors
    double dataR[] = {6, 6, 9, 6, 6, 6, 9, 12, 9, 6, 9, 12, 12, 12, 12, 9, 6, 9, 12, 9, 6, 6, 6, 9,
        6, 6};
    double dataA[] = {-45, -30, 0, 30, 45, -60, -45, 0, 45, 60, -90, -90, -90, 90, 90, 90, -120,
        -135, 180, 135, 120, -135, -150, 180, 150, 135};

    // Create a XYChart object of size 450 x 390 pixels
    XYChart *c = new XYChart(450, 390);

    // Set the plotarea at (55, 40) and of size 350 x 300 pixels, with a light grey border
    // (0xc0c0c0). Turn on both horizontal and vertical grid lines with light grey color (0xc0c0c0)
    c->setPlotArea(55, 40, 350, 300, -1, -1, 0xc0c0c0, 0xc0c0c0, -1);

    // Add a title to the chart using 18pt Times Bold Itatic font.
    c->addTitle("Vector Chart Demonstration", "timesbi.ttf", 18);

    // Add a title to the y axis using 12pt Arial Bold Italic font
    c->yAxis()->setTitle("Y Axis Title", "arialbi.ttf", 12);

    // Add a title to the x axis using 12pt Arial Bold Italic font
    c->xAxis()->setTitle("X Axis Title", "arialbi.ttf", 12);

    // Set the axes line width to 3 pixels
    c->xAxis()->setWidth(3);
    c->yAxis()->setWidth(3);

    // Add a vector layer to the chart using blue (0000CC) color, with vector arrow size set to 11
    // pixels
    c->addVectorLayer(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0]))), DoubleArray(
        dataY, (int)(sizeof(dataY) / sizeof(dataY[0]))), DoubleArray(dataR, (int)(sizeof(dataR) /
        sizeof(dataR[0]))), DoubleArray(dataA, (int)(sizeof(dataA) / sizeof(dataA[0]))),
        Chart::YAxisScale, 0x0000cc)->setArrowHead(11);

    // Output the chart
    c->makeChart("vector.png");

    //free up resources
    delete c;
    return 0;
}

