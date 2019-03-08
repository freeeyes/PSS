#include "chartdir.h"

int main(int argc, char *argv[])
{
    // 4 data points to represent the cash flow for the Q1 - Q4
    double data[] = {230, -140, 220, 330};

    // We want to plot a waterfall chart showing the 4 quarters as well as the total
    const char *labels[] = {"1st Quarter", "2nd Quarter", "3rd Quarter", "4th Quarter", "Total"};

    // The top side of the bars in a waterfall chart is the accumulated data. We use the
    // ChartDirector ArrayMath utility to accumulate the data. The "total" is handled by inserting a
    // zero point at the end before accumulation (after accumulation it will become the total).
    ArrayMath boxTop = ArrayMath(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0])))).insert(0,
        1, (int)(sizeof(data) / sizeof(data[0]))).acc();

    // The botom side of the bars is just the top side of the previous bar. So we shifted the top
    // side data to obtain the bottom side data.
    ArrayMath boxBottom = ArrayMath(boxTop).shift(1, 0);

    // The last point (total) is different. Its bottom side is always 0.
    boxBottom.trim(0, (int)(sizeof(data) / sizeof(data[0]))).insert(0, 1);

    // In this example, we want to use different colors depending on the data is positive or
    // negative.
    int posColor = 0x00ff00;
    int negColor = 0xff0000;

    // Create a XYChart object of size 500 x 280 pixels. Set background color to light blue
    // (ccccff), with 1 pixel 3D border effect.
    XYChart *c = new XYChart(500, 300, 0xccccff, 0x000000, 1);

    // Add a title to the chart using 13 points Arial Bold Itatic font, with white (ffffff) text on
    // a deep blue (0x80) background
    c->addTitle("Corporate Cash Flow - Year 2004", "arialbi.ttf", 13, 0xffffff)->setBackground(
        0x000080);

    // Set the plotarea at (55, 50) and of size 430 x 215 pixels. Use alternative white/grey
    // background.
    c->setPlotArea(55, 50, 430, 215, 0xffffff, 0xeeeeee);

    // Add a legend box at (55, 25) using 8pt Arial Bold font with horizontal layout, with
    // transparent background and border color.
    LegendBox *b = c->addLegend(55, 25, false, "arialbd.ttf", 8);
    b->setBackground(Chart::Transparent, Chart::Transparent);

    // Add keys to show the colors for positive and negative cash flows
    b->addKey("Positive Cash Flow", posColor);
    b->addKey("Negative Cash Flow", negColor);

    // Set the labels on the x axis using Arial Bold font
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0])))
        )->setFontStyle("arialbd.ttf");

    // Set the x-axis ticks and grid lines to be between the bars
    c->xAxis()->setTickOffset(0.5);

    // Use Arial Bold as the y axis label font
    c->yAxis()->setLabelStyle("arialbd.ttf");

    // Add a title to the y axis
    c->yAxis()->setTitle("USD (in millions)");

    // Add a box-whisker layer to represent the waterfall bars
    BoxWhiskerLayer *layer = c->addBoxWhiskerLayer(boxTop, boxBottom);

    DoubleArray boxTopData = boxTop.result();
    DoubleArray boxBottomData = boxBottom.result();
    for (int i = 0; i < boxTopData.len; ++i)
    {
        if (boxTopData[i] >= boxBottomData[i])
            layer->setBoxColor(i, posColor);
        else
            layer->setBoxColor(i, negColor);
    }

    // Put data labels on the bars to show the cash flow using Arial Bold font
    layer->setDataLabelFormat("{={top}-{bottom}}M");
    layer->setDataLabelStyle("arialbd.ttf")->setAlignment(Chart::Center);

    // Output the chart
    c->makeChart("posnegwaterfall.png");

    //free up resources
    delete c;
    return 0;
}

