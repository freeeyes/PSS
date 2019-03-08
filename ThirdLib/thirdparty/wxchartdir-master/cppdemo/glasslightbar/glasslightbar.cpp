#include "chartdir.h"

int main(int argc, char *argv[])
{
    // The data for the bar chart
    double data[] = {450, 560, 630, 800, 1100, 1350, 1600, 1950, 2300, 2700};

    // The labels for the bar chart
    const char *labels[] = {"1996", "1997", "1998", "1999", "2000", "2001", "2002", "2003", "2004",
        "2005"};

    // Create a XYChart object of size 600 x 360 pixels
    XYChart *c = new XYChart(600, 360);

    // Set the plotarea at (60, 40) and of size 480 x 280 pixels. Use a vertical gradient color from
    // light blue (eeeeff) to deep blue (0000cc) as background. Set border and grid lines to white
    // (ffffff).
    c->setPlotArea(60, 40, 480, 280, c->linearGradientColor(60, 40, 60, 280, 0xeeeeff, 0x0000cc),
        -1, 0xffffff, 0xffffff);

    // Add a title to the chart using 18pt Times Bold Italic font
    c->addTitle("Annual Revenue for Star Tech", "timesbi.ttf", 18);

    // Add a multi-color bar chart layer using the supplied data. Use glass lighting effect with
    // light direction from the left.
    c->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), IntArray(0, 0)
        )->setBorderColor(Chart::Transparent, Chart::glassEffect(Chart::NormalGlare, Chart::Left));

    // Set the x axis labels
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Show the same scale on the left and right y-axes
    c->syncYAxis();

    // Set the left y-axis and right y-axis title using 10pt Arial Bold font
    c->yAxis()->setTitle("USD (millions)", "arialbd.ttf", 10);
    c->yAxis2()->setTitle("USD (millions)", "arialbd.ttf", 10);

    // Set all axes to transparent
    c->xAxis()->setColors(Chart::Transparent);
    c->yAxis()->setColors(Chart::Transparent);
    c->yAxis2()->setColors(Chart::Transparent);

    // Set the label styles of all axes to 8pt Arial Bold font
    c->xAxis()->setLabelStyle("arialbd.ttf", 8);
    c->yAxis()->setLabelStyle("arialbd.ttf", 8);
    c->yAxis2()->setLabelStyle("arialbd.ttf", 8);

    // Output the chart
    c->makeChart("glasslightbar.png");

    //free up resources
    delete c;
    return 0;
}

