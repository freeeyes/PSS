#include "chartdir.h"

int main(int argc, char *argv[])
{
    // The data for the pie chart
    double data[] = {25, 18, 15, 12, 8, 30, 35};

    // The labels for the pie chart
    const char *labels[] = {"Labor", "Licenses", "Taxes", "Legal", "Insurance", "Facilities",
        "Production"};

    // Create a PieChart object of size 450 x 270 pixels
    PieChart *c = new PieChart(450, 270);

    // Set the center of the pie at (150, 100) and the radius to 80 pixels
    c->setPieSize(150, 135, 100);

    // add a legend box where the top left corner is at (330, 50)
    c->addLegend(330, 60);

    // modify the sector label format to show percentages only
    c->setLabelFormat("{percent}%");

    // Set the pie data and the pie labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Use rounded edge shading, with a 1 pixel white (FFFFFF) border
    c->setSectorStyle(Chart::RoundedEdgeShading, 0xffffff, 1);

    // Output the chart
    c->makeChart("legendpie.png");

    //free up resources
    delete c;
    return 0;
}

