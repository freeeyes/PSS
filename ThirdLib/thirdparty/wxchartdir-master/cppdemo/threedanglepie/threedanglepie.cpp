#include "chartdir.h"
#include <stdio.h>

void createChart(int chartIndex, const char *filename)
{
    char buffer[256];

    // the tilt angle of the pie
    int angle = chartIndex * 15;

    // The data for the pie chart
    double data[] = {25, 18, 15, 12, 8, 30, 35};

    // Create a PieChart object of size 100 x 110 pixels
    PieChart *c = new PieChart(100, 110);

    // Set the center of the pie at (50, 55) and the radius to 38 pixels
    c->setPieSize(50, 55, 38);

    // Set the depth and tilt angle of the 3D pie (-1 means auto depth)
    c->set3D(-1, angle);

    // Add a title showing the tilt angle
    sprintf(buffer, "Tilt = %d deg", angle);
    c->addTitle(buffer, "arial.ttf", 8);

    // Set the pie data
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))));

    // Disable the sector labels by setting the color to Transparent
    c->setLabelStyle("", 8, Chart::Transparent);

    // Output the chart
    c->makeChart(filename);

    //free up resources
    delete c;
}

int main(int argc, char *argv[])
{
    createChart(0, "threedanglepie0.png");
    createChart(1, "threedanglepie1.png");
    createChart(2, "threedanglepie2.png");
    createChart(3, "threedanglepie3.png");
    createChart(4, "threedanglepie4.png");
    createChart(5, "threedanglepie5.png");
    createChart(6, "threedanglepie6.png");
    return 0;
}

