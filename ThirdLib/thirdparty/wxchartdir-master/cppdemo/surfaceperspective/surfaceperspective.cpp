#include "chartdir.h"
#include <stdio.h>
#include <math.h>

void createChart(int chartIndex, const char *filename)
{
    char buffer[256];

    // The x and y coordinates of the grid
    double dataX[] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    double dataY[] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};

    // The values at the grid points. In this example, we will compute the values using the formula
    // z = sin((x - 0.5) * 2 * pi) * sin((y - 0.5) * 2 * pi)
    double dataZ[(int)(sizeof(dataX) / sizeof(dataX[0])) * (int)(sizeof(dataY) / sizeof(dataY[0]))];
    for(int yIndex = 0; yIndex < (int)(sizeof(dataY) / sizeof(dataY[0])); ++yIndex) {
        double y = (dataY[yIndex] - 0.5) * 2 * 3.1416;
        for(int xIndex = 0; xIndex < (int)(sizeof(dataX) / sizeof(dataX[0])); ++xIndex) {
            double x = (dataX[xIndex] - 0.5) * 2 * 3.1416;
            dataZ[yIndex * (int)(sizeof(dataX) / sizeof(dataX[0])) + xIndex] = sin(x) * sin(y);
        }
    }

    // the perspective level
    int perspective = chartIndex * 12;

    // Create a SurfaceChart object of size 360 x 360 pixels, with white (ffffff) background and
    // grey (888888) border.
    SurfaceChart *c = new SurfaceChart(360, 360, 0xffffff, 0x888888);

    // Set the perspective level
    c->setPerspective(perspective);
    sprintf(buffer, "Perspective = %d", perspective);
    c->addTitle(buffer);

    // Set the center of the plot region at (195, 165), and set width x depth x height to 200 x 200
    // x 150 pixels
    c->setPlotRegion(195, 165, 200, 200, 150);

    // Set the plot region wall thichness to 5 pixels
    c->setWallThickness(5);

    // Set the elevation and rotation angles to 30 and 30 degrees
    c->setViewAngle(30, 30);

    // Set the data to use to plot the chart
    c->setData(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0]))), DoubleArray(dataY,
        (int)(sizeof(dataY) / sizeof(dataY[0]))), DoubleArray(dataZ, (int)(sizeof(dataZ) / sizeof(
        dataZ[0]))));

    // Spline interpolate data to a 40 x 40 grid for a smooth surface
    c->setInterpolation(40, 40);

    // Use smooth gradient coloring.
    c->colorAxis()->setColorGradient();

    // Output the chart
    c->makeChart(filename);

    //free up resources
    delete c;
}

int main(int argc, char *argv[])
{
    createChart(0, "surfaceperspective0.jpg");
    createChart(1, "surfaceperspective1.jpg");
    createChart(2, "surfaceperspective2.jpg");
    createChart(3, "surfaceperspective3.jpg");
    createChart(4, "surfaceperspective4.jpg");
    createChart(5, "surfaceperspective5.jpg");
    return 0;
}

