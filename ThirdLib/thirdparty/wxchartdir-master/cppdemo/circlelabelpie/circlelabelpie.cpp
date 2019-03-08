#include "chartdir.h"

void createChart(int chartIndex, const char *filename)
{
    // The data for the pie chart
    double data[] = {42, 18, 8};

    // The labels for the pie chart
    const char *labels[] = {"Agree", "Disagree", "Not Sure"};

    // The colors to use for the sectors
    int colors[] = {0x66ff66, 0xff6666, 0xffff00};

    // Create a PieChart object of size 300 x 300 pixels. Set the background to a gradient color
    // from blue (aaccff) to sky blue (ffffff), with a grey (888888) border. Use rounded corners and
    // soft drop shadow.
    PieChart *c = new PieChart(300, 300);
    c->setBackground(c->linearGradientColor(0, 0, 0, c->getHeight() / 2, 0xaaccff, 0xffffff),
        0x888888);
    c->setRoundedFrame();
    c->setDropShadow();

    if (chartIndex == 0) {
    //============================================================
    //    Draw a pie chart where the label is on top of the pie
    //============================================================

        // Set the center of the pie at (150, 150) and the radius to 120 pixels
        c->setPieSize(150, 150, 120);

        // Set the label position to -40 pixels from the perimeter of the pie (-ve means label is
        // inside the pie)
        c->setLabelPos(-40);

    } else {
    //============================================================
    //    Draw a pie chart where the label is outside the pie
    //============================================================

        // Set the center of the pie at (150, 150) and the radius to 80 pixels
        c->setPieSize(150, 150, 80);

        // Set the sector label position to be 20 pixels from the pie. Use a join line to connect
        // the labels to the sectors.
        c->setLabelPos(20, Chart::LineColor);

    }

    // Set the pie data and the pie labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Set the sector colors
    c->setColors(Chart::DataColor, IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));

    // Use local gradient shading, with a 1 pixel semi-transparent black (bb000000) border
    c->setSectorStyle(Chart::LocalGradientShading, 0xbb000000, 1);

    // Output the chart
    c->makeChart(filename);

    //free up resources
    delete c;
}

int main(int argc, char *argv[])
{
    createChart(0, "circlelabelpie0.png");
    createChart(1, "circlelabelpie1.png");
    return 0;
}

