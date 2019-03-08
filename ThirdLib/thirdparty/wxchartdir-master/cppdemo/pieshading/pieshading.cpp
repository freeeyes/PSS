#include "chartdir.h"

void createChart(int chartIndex, const char *filename)
{
    // The data for the pie chart
    double data[] = {18, 30, 20, 15};

    // The colors to use for the sectors
    int colors[] = {0x66aaee, 0xeebb22, 0xbbbbbb, 0x8844ff};

    // Create a PieChart object of size 200 x 220 pixels. Use a vertical gradient color from blue
    // (0000cc) to deep blue (000044) as background. Use rounded corners of 16 pixels radius.
    PieChart *c = new PieChart(200, 220);
    c->setBackground(c->linearGradientColor(0, 0, 0, c->getHeight(), 0x0000cc, 0x000044));
    c->setRoundedFrame(0xffffff, 16);

    // Set the center of the pie at (100, 120) and the radius to 80 pixels
    c->setPieSize(100, 120, 80);

    // Set the pie data
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))));

    // Set the sector colors
    c->setColors(Chart::DataColor, IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));

    // Demonstrates various shading modes
    if (chartIndex == 0) {
        c->addTitle("Default Shading", "bold", 12, 0xffffff);
    } else if (chartIndex == 1) {
        c->addTitle("Local Gradient", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::LocalGradientShading);
    } else if (chartIndex == 2) {
        c->addTitle("Global Gradient", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::GlobalGradientShading);
    } else if (chartIndex == 3) {
        c->addTitle("Concave Shading", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::ConcaveShading);
    } else if (chartIndex == 4) {
        c->addTitle("Rounded Edge", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::RoundedEdgeShading);
    } else if (chartIndex == 5) {
        c->addTitle("Radial Gradient", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::RadialShading);
    }

    // Disable the sector labels by setting the color to Transparent
    c->setLabelStyle("", 8, Chart::Transparent);

    // Output the chart
    c->makeChart(filename);

    //free up resources
    delete c;
}

int main(int argc, char *argv[])
{
    createChart(0, "pieshading0.png");
    createChart(1, "pieshading1.png");
    createChart(2, "pieshading2.png");
    createChart(3, "pieshading3.png");
    createChart(4, "pieshading4.png");
    createChart(5, "pieshading5.png");
    return 0;
}

