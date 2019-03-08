#include "chartdir.h"

int main(int argc, char *argv[])
{
    // The data for the chart
    double data0[] = {43, 89, 76, 64, 48, 18, 92, 68, 44, 79, 71, 85};
    double angles0[] = {45, 96, 169, 258, 15, 30, 330, 260, 60, 75, 110, 140};

    double data1[] = {50, 91, 26, 29, 80, 53, 62, 87, 19, 40};
    double angles1[] = {230, 210, 240, 310, 179, 250, 244, 199, 89, 160};

    double data2[] = {88, 65, 76, 49, 80, 53};
    double angles2[] = {340, 310, 340, 210, 30, 300};

    // The labels on the angular axis (spokes)
    const char *labels[] = {"North", "North\nEast", "East", "South\nEast", "South", "South\nWest",
        "West", "North\nWest"};

    // Create a PolarChart object of size 460 x 460 pixels
    PolarChart *c = new PolarChart(460, 460);

    // Add a title to the chart at the top left corner using 15pt Arial Bold Italic font
    c->addTitle(Chart::TopLeft, "<*underline=2*>Plants in Wonderland", "arialbi.ttf", 15);

    // Set center of plot area at (230, 240) with radius 180 pixels
    c->setPlotArea(230, 240, 180);

    // Use alternative light grey/dark grey sector background color
    c->setPlotAreaBg(0xdddddd, 0xeeeeee, false);

    // Set the grid style to circular grid
    c->setGridStyle(false);

    // Add a legend box at the top right corner of the chart using 9pt Arial Bold font
    c->addLegend(459, 0, true, "arialbd.ttf", 9)->setAlignment(Chart::TopRight);

    // Set angular axis as 0 - 360, either 8 spokes
    c->angularAxis()->setLinearScale(0, 360, StringArray(labels, (int)(sizeof(labels) / sizeof(
        labels[0]))));

    // Set the radial axis label format
    c->radialAxis()->setLabelFormat("{value} km");

    // Add a blue (0xff) polar line layer to the chart using (data0, angle0)
    PolarLineLayer *layer0 = c->addLineLayer(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0
        ]))), 0x0000ff, "Immortal Weed");
    layer0->setAngles(DoubleArray(angles0, (int)(sizeof(angles0) / sizeof(angles0[0]))));

    layer0->setLineWidth(0);
    layer0->setDataSymbol(Chart::TriangleSymbol, 11);

    // Add a red (0xff0000) polar line layer to the chart using (data1, angles1)
    PolarLineLayer *layer1 = c->addLineLayer(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0
        ]))), 0xff0000, "Precious Flower");
    layer1->setAngles(DoubleArray(angles1, (int)(sizeof(angles1) / sizeof(angles1[0]))));

    // Disable the line by setting its width to 0, so only the symbols are visible
    layer1->setLineWidth(0);

    // Use a 11 pixel diamond data point symbol
    layer1->setDataSymbol(Chart::DiamondSymbol, 11);

    // Add a green (0x00ff00) polar line layer to the chart using (data2, angles2)
    PolarLineLayer *layer2 = c->addLineLayer(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0
        ]))), 0x00ff00, "Magical Tree");
    layer2->setAngles(DoubleArray(angles2, (int)(sizeof(angles2) / sizeof(angles2[0]))));

    // Disable the line by setting its width to 0, so only the symbols are visible
    layer2->setLineWidth(0);

    // Use a 9 pixel square data point symbol
    layer2->setDataSymbol(Chart::SquareSymbol, 9);

    // Output the chart
    c->makeChart("polarscatter.png");

    //free up resources
    delete c;
    return 0;
}

