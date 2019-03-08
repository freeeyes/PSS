#include "chartdir.h"

void createChart(int chartIndex, const char *filename)
{
    // The data for the pie chart
    double data0[] = {25, 18, 15};
    double data1[] = {14, 32, 24};
    double data2[] = {25, 23, 9};

    // The labels for the pie chart
    const char *labels[] = {"Software", "Hardware", "Services"};

    // Create a PieChart object of size 180 x 160 pixels
    PieChart *c = new PieChart(180, 160);

    // Set the center of the pie at (90, 80) and the radius to 60 pixels
    c->setPieSize(90, 80, 60);

    // Set the border color of the sectors to white (ffffff)
    c->setLineColor(0xffffff);

    // Set the background color of the sector label to pale yellow (ffffc0) with a black border
    // (000000)
    c->setLabelStyle()->setBackground(0xffffc0, 0x000000);

    // Set the label to be slightly inside the perimeter of the circle
    c->setLabelLayout(Chart::CircleLayout, -10);

    // Set the title, data and colors according to which pie to draw
    if (chartIndex == 0) {
        c->addTitle("Alpha Division", "arialbd.ttf", 8);
        c->setData(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), StringArray(labels,
            (int)(sizeof(labels) / sizeof(labels[0]))));
        int pattern1[] = {0xff3333, 0xff9999, 0xffcccc};
        c->setColors(Chart::DataColor, IntArray(pattern1, (int)(sizeof(pattern1) / sizeof(pattern1[0
            ]))));
    } else if (chartIndex == 1) {
        c->addTitle("Beta Division", "arialbd.ttf", 8);
        c->setData(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), StringArray(labels,
            (int)(sizeof(labels) / sizeof(labels[0]))));
        int pattern2[] = {0x33ff33, 0x99ff99, 0xccffcc};
        c->setColors(Chart::DataColor, IntArray(pattern2, (int)(sizeof(pattern2) / sizeof(pattern2[0
            ]))));
    } else {
        c->addTitle("Gamma Division", "arialbd.ttf", 8);
        c->setData(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), StringArray(labels,
            (int)(sizeof(labels) / sizeof(labels[0]))));
        int pattern3[] = {0x3333ff, 0x9999ff, 0xccccff};
        c->setColors(Chart::DataColor, IntArray(pattern3, (int)(sizeof(pattern3) / sizeof(pattern3[0
            ]))));
    }

    // Output the chart
    c->makeChart(filename);

    //free up resources
    delete c;
}

int main(int argc, char *argv[])
{
    createChart(0, "multipie0.png");
    createChart(1, "multipie1.png");
    createChart(2, "multipie2.png");
    return 0;
}

