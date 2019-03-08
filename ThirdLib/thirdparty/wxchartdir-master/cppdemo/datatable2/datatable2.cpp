#include "chartdir.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    char buffer[256];

    // The data for the line chart
    double data0[] = {410, 420, 500, 590};
    double data1[] = {500, 370, 680, 850};
    const char *labels[] = {"Q1", "Q2", "Q3", "Q4"};

    // Create a XYChart object of size 600 x 400 pixels
    XYChart *c = new XYChart(600, 400);

    // Add a title to the chart using 18pt Times Bold Italic font
    TextBox *title = c->addTitle("Product Line Global Revenue", "timesbi.ttf", 18);

    // Tentatively set the plotarea at (50, 55) and of (chart_width - 100) x (chart_height - 150)
    // pixels in size. Use a vertical gradient color from sky blue (aaccff) t0 light blue (f9f9ff)
    // as background. Set both horizontal and vertical grid lines to dotted semi-transprent black
    // (aa000000).
    PlotArea *plotArea = c->setPlotArea(50, 55, c->getWidth() - 100, c->getHeight() - 150,
        c->linearGradientColor(0, 55, 0, 55 + c->getHeight() - 150, 0xaaccff, 0xf9fcff), -1, -1,
        c->dashLineColor(0xaa000000, Chart::DotLine), -1);

    // Set y-axis title using 12 points Arial Bold Italic font, and set its position 10 pixels from
    // the axis.
    c->yAxis()->setTitle("Revenue (USD millions)", "arialbi.ttf", 12);
    c->yAxis()->setTitlePos(Chart::Left, 10);

    // Set y-axis label style to 10 points Arial Bold and axis color to transparent
    c->yAxis()->setLabelStyle("arialbd.ttf", 10);
    c->yAxis()->setColors(Chart::Transparent);

    // Set y-axis tick density to 30 pixels. ChartDirector auto-scaling will use this as the
    // guideline when putting ticks on the y-axis.
    c->yAxis()->setTickDensity(30);

    // Add a bar layer to the chart with side layout
    BarLayer *layer = c->addBarLayer(Chart::Side);

    // Add two data sets to the bar layer
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0xff6600,
        "FY 2007");
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x0088ff,
        "FY 2008");

    // Use soft lighting effect with light direction from the left
    layer->setBorderColor(Chart::Transparent, Chart::softLighting(Chart::Left));

    // Set the x axis labels
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Convert the labels on the x-axis to a CDMLTable
    CDMLTable *table = c->xAxis()->makeLabelTable();

    // Set the default left/right margins to 5 pixels and top/bottom margins to 3 pixels. Set the
    // default font size to 10 points
    TextBox *cellStyle = table->getStyle();
    cellStyle->setMargin(5, 5, 4, 3);
    cellStyle->setFontSize(10);

    // Set the first row to use Arial Bold font, with a light grey (eeeeee) background.
    TextBox *firstRowStyle = table->getRowStyle(0);
    firstRowStyle->setFontStyle("arialbd.ttf");
    firstRowStyle->setBackground(0xeeeeee, Chart::LineColor);

    //
    // We can add more information to the table. In this sample code, we add the data series and the
    // legend icons to the table.
    //

    // Add 3 more rows to the table. Set the background of the 2nd row to light grey (eeeeee).
    table->appendRow();
    table->appendRow()->setBackground(0xeeeeee, Chart::LineColor);
    table->appendRow();

    // Put the values of the 2 data series in the first 2 rows. Put the percentage differences in
    // the 3rd row.
    for(int i = 0; i < (int)(sizeof(data0) / sizeof(data0[0])); ++i) {
        sprintf(buffer, "%g", data0[i]);
        table->setText(i, 1, buffer);
        sprintf(buffer, "%g", data1[i]);
        table->setText(i, 2, buffer);

        double percentageDiff = 100.0 * (data1[i] - data0[i]) / data0[i];

        // Use red or green color depending on whether the difference is positive or negative
        const char *formatString = "<*color=008800*>+{value|1}%";
        if (percentageDiff < 0) {
            formatString = "<*color=cc0000*>{value|1}%";
        }
        table->setText(i, 3, c->formatValue(percentageDiff, formatString));
    }

    // Insert a column on the left for the legend icons using Arial Bold font.
    table->insertCol(0)->setFontStyle("arialbd.ttf");

    // The top cell is set to transparent, so it is invisible
    table->getCell(0, 0)->setBackground(Chart::Transparent, Chart::Transparent);

    // The next 2 cells are set to the legend icons and names of the 2 data series
    sprintf(buffer, "%s  FY 2007", layer->getLegendIcon(0));
    table->setText(0, 1, buffer);
    sprintf(buffer, "%s  FY 2008", layer->getLegendIcon(1));
    table->setText(0, 2, buffer);

    // The last cell is set to "Change"
    table->setText(0, 3, "Change");

    // Append a column on the right for the total values.
    table->appendCol();

    // Put "Total" in the top cell as the heading of this column
    table->setText(table->getColCount() - 1, 0, "Total");

    // The next two cells are the total of the data series
    double total0 = ArrayMath(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0])))).sum();
    double total1 = ArrayMath(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0])))).sum();
    sprintf(buffer, "%g", total0);
    table->setText(table->getColCount() - 1, 1, buffer);
    sprintf(buffer, "%g", total1);
    table->setText(table->getColCount() - 1, 2, buffer);

    // The last cell is the percentage differences of the total
    double totalPercentageDiff = (total1 - total0) / total0 * 100;

    // Use red or green color depending on whether the difference is positive or negative
    const char *totalFormatString = "<*color=008800*>+{value|1}%";
    if (totalPercentageDiff < 0) {
        totalFormatString = "<*color=cc0000*>{value|1}%";
    }
    table->setText(table->getColCount() - 1, 3, c->formatValue(totalPercentageDiff,
        totalFormatString));

    //
    // We now demonstrate how to adjust the plot area positions, to allow space for the newly
    // inserted left and right columns in the table.
    //

    // We layout the axis first in order to get the axis metrics (including table metrics)
    c->layoutAxes();

    // If the first column is wider than the left y-axis, we need to reserve for some left margin to
    // ensure the first column stays within the chart.
    int leftMargin = 0;
    if (table->getColWidth(0) > c->yAxis()->getThickness()) {
        leftMargin = table->getColWidth(0) - c->yAxis()->getThickness();
    }

    // Similarly, we need to reserve some right margin for the last column
    int rightMargin = table->getColWidth(table->getColCount() - 1);

    // Adjust the plot area size, such that the bounding box (inclusive of axes) using the given
    // left and right margin, plus 2 more pixels. Put the plot area 10 pixels below the title and
    // use 2 pixels as the bottom margin. from the left, right and bottom edge, and is just under
    // the legend box.
    c->packPlotArea(leftMargin + 2, title->getHeight() + 10, c->getWidth() - 3 - rightMargin,
        c->getHeight() - 3);

    // After determining the exact plot area position, we may adjust title position so that it is
    // centered relative to the plot area (instead of the chart)
    title->setPos(plotArea->getLeftX() + (plotArea->getWidth() - title->getWidth()) / 2,
        title->getTopY());

    // Output the chart
    c->makeChart("datatable2.png");

    //free up resources
    delete c;
    return 0;
}

