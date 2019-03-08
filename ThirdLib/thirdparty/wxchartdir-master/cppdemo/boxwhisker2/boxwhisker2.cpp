#include "chartdir.h"

int main(int argc, char *argv[])
{
    // Sample data for the Box-Whisker chart. Represents the minimum, 1st quartile, medium, 3rd
    // quartile and maximum values of some quantities
    double Q0Data[] = {40, 45, 40, 30, 20, 50, 25, 44};
    double Q1Data[] = {55, 60, 50, 40, 38, 60, 51, 60};
    double Q2Data[] = {62, 70, 60, 50, 48, 70, 62, 70};
    double Q3Data[] = {70, 80, 65, 60, 53, 78, 69, 76};
    double Q4Data[] = {80, 90, 75, 70, 60, 85, 80, 84};

    // The labels for the chart
    const char *labels[] = {"A", "B", "C", "D", "E", "F", "G", "H"};

    // Create a XYChart object of size 450 x 400 pixels
    XYChart *c = new XYChart(450, 400);

    // Set the plotarea at (50, 30) and of size 380 x 340 pixels, with transparent background and
    // border and light grey (0xcccccc) horizontal grid lines
    c->setPlotArea(50, 30, 380, 340, Chart::Transparent, -1, Chart::Transparent, 0xcccccc);

    // Add a title box using grey (0x555555) 18pt Arial font
    TextBox *title = c->addTitle("     Pattern Recognition Accuracy", "arial.ttf", 18, 0x555555);

    // Set the x and y axis stems to transparent and the label font to 12pt Arial
    c->xAxis()->setColors(Chart::Transparent);
    c->yAxis()->setColors(Chart::Transparent);
    c->xAxis()->setLabelStyle("arial.ttf", 12);
    c->yAxis()->setLabelStyle("arial.ttf", 12);

    // Set the labels on the x axis
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // For the automatic y-axis labels, set the minimum spacing to 30 pixels.
    c->yAxis()->setTickDensity(30);

    // Add a box whisker layer using light blue (0x99ccee) for the fill color and blue (0x6688aa)
    // for the whisker color. Set line width to 2 pixels. Use rounded corners and bar lighting
    // effect.
    BoxWhiskerLayer *b = c->addBoxWhiskerLayer(DoubleArray(Q3Data, (int)(sizeof(Q3Data) / sizeof(
        Q3Data[0]))), DoubleArray(Q1Data, (int)(sizeof(Q1Data) / sizeof(Q1Data[0]))), DoubleArray(
        Q4Data, (int)(sizeof(Q4Data) / sizeof(Q4Data[0]))), DoubleArray(Q0Data, (int)(sizeof(Q0Data)
         / sizeof(Q0Data[0]))), DoubleArray(Q2Data, (int)(sizeof(Q2Data) / sizeof(Q2Data[0]))),
        0x99ccee, 0x6688aa);
    b->setLineWidth(2);
    b->setRoundedCorners();
    b->setBorderColor(Chart::Transparent, Chart::barLighting());

    // Adjust the plot area to fit under the title with 10-pixel margin on the other three sides.
    c->packPlotArea(10, title->getHeight(), c->getWidth() - 10, c->getHeight() - 10);

    // Output the chart
    c->makeChart("boxwhisker2.png");

    //free up resources
    delete c;
    return 0;
}

