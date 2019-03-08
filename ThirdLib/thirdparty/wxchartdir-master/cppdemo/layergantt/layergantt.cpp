#include "chartdir.h"

int main(int argc, char *argv[])
{
    // the names of the tasks
    const char *labels[] = {"Market Research", "Define Specifications", "Overall Archiecture",
        "Project Planning", "Detail Design", "Software Development", "Test Plan", "Testing and QA",
        "User Documentation"};

    // the planned start dates and end dates for the tasks
    double startDate[] = {Chart::chartTime(2004, 8, 16), Chart::chartTime(2004, 8, 30),
        Chart::chartTime(2004, 9, 13), Chart::chartTime(2004, 9, 20), Chart::chartTime(2004, 9, 27),
        Chart::chartTime(2004, 10, 4), Chart::chartTime(2004, 10, 25), Chart::chartTime(2004, 11, 1
        ), Chart::chartTime(2004, 11, 8)};
    double endDate[] = {Chart::chartTime(2004, 8, 30), Chart::chartTime(2004, 9, 13),
        Chart::chartTime(2004, 9, 27), Chart::chartTime(2004, 10, 4), Chart::chartTime(2004, 10, 11
        ), Chart::chartTime(2004, 11, 8), Chart::chartTime(2004, 11, 8), Chart::chartTime(2004, 11,
        22), Chart::chartTime(2004, 11, 22)};

    // the actual start dates and end dates for the tasks up to now
    double actualStartDate[] = {Chart::chartTime(2004, 8, 16), Chart::chartTime(2004, 8, 27),
        Chart::chartTime(2004, 9, 9), Chart::chartTime(2004, 9, 18), Chart::chartTime(2004, 9, 22)};
    double actualEndDate[] = {Chart::chartTime(2004, 8, 27), Chart::chartTime(2004, 9, 9),
        Chart::chartTime(2004, 9, 27), Chart::chartTime(2004, 10, 2), Chart::chartTime(2004, 10, 8)}
        ;

    // Create a XYChart object of size 620 x 280 pixels. Set background color to light green
    // (ccffcc) with 1 pixel 3D border effect.
    XYChart *c = new XYChart(620, 280, 0xccffcc, 0x000000, 1);

    // Add a title to the chart using 15 points Times Bold Itatic font, with white (ffffff) text on
    // a dark green (0x6000) background
    c->addTitle("Multi-Layer Gantt Chart Demo", "timesbi.ttf", 15, 0xffffff)->setBackground(0x006000
        );

    // Set the plotarea at (140, 55) and of size 460 x 200 pixels. Use alternative white/grey
    // background. Enable both horizontal and vertical grids by setting their colors to grey
    // (c0c0c0). Set vertical major grid (represents month boundaries) 2 pixels in width
    c->setPlotArea(140, 55, 460, 200, 0xffffff, 0xeeeeee, Chart::LineColor, 0xc0c0c0, 0xc0c0c0
        )->setGridWidth(2, 1, 1, 1);

    // swap the x and y axes to create a horziontal box-whisker chart
    c->swapXY();

    // Set the y-axis scale to be date scale from Aug 16, 2004 to Nov 22, 2004, with ticks every 7
    // days (1 week)
    c->yAxis()->setDateScale(Chart::chartTime(2004, 8, 16), Chart::chartTime(2004, 11, 22), 86400 *
        7);

    // Add a red (ff0000) dash line to represent the current day
    c->yAxis()->addMark(Chart::chartTime(2004, 10, 8), c->dashLineColor(0xff0000, Chart::DashLine));

    // Set multi-style axis label formatting. Month labels are in Arial Bold font in "mmm d" format.
    // Weekly labels just show the day of month and use minor tick (by using '-' as first character
    // of format string).
    c->yAxis()->setMultiFormat(Chart::StartOfMonthFilter(), "<*font=arialbd.ttf*>{value|mmm d}",
        Chart::StartOfDayFilter(), "-{value|d}");

    // Set the y-axis to shown on the top (right + swapXY = top)
    c->setYAxisOnRight();

    // Set the labels on the x axis
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Reverse the x-axis scale so that it points downwards.
    c->xAxis()->setReverse();

    // Set the horizontal ticks and grid lines to be between the bars
    c->xAxis()->setTickOffset(0.5);

    // Use blue (0000aa) as the color for the planned schedule
    int plannedColor = 0x0000aa;

    // Use a red hash pattern as the color for the actual dates. The pattern is created as a 4 x 4
    // bitmap defined in memory as an array of colors.
    int pattern1[] = {0xffffff, 0xffffff, 0xffffff, 0xff0000, 0xffffff, 0xffffff, 0xff0000, 0xffffff
        , 0xffffff, 0xff0000, 0xffffff, 0xffffff, 0xff0000, 0xffffff, 0xffffff, 0xffffff};
    int actualColor = c->patternColor(IntArray(pattern1, (int)(sizeof(pattern1) / sizeof(pattern1[0]
        ))), 4);

    // Add a box whisker layer to represent the actual dates. We add the actual dates layer first,
    // so it will be the top layer.
    BoxWhiskerLayer *actualLayer = c->addBoxLayer(DoubleArray(actualStartDate, (int)(sizeof(
        actualStartDate) / sizeof(actualStartDate[0]))), DoubleArray(actualEndDate, (int)(sizeof(
        actualEndDate) / sizeof(actualEndDate[0]))), actualColor, "Actual");

    // Set the bar height to 8 pixels so they will not block the bottom bar
    actualLayer->setDataWidth(8);

    // Add a box-whisker layer to represent the planned schedule date
    c->addBoxLayer(DoubleArray(startDate, (int)(sizeof(startDate) / sizeof(startDate[0]))),
        DoubleArray(endDate, (int)(sizeof(endDate) / sizeof(endDate[0]))), plannedColor, "Planned"
        )->setBorderColor(Chart::SameAsMainColor);

    // Add a legend box on the top right corner (595, 60) of the plot area with 8 pt Arial Bold
    // font. Use a semi-transparent grey (80808080) background.
    LegendBox *b = c->addLegend(595, 60, false, "arialbd.ttf", 8);
    b->setAlignment(Chart::TopRight);
    b->setBackground(0x80808080, -1, 2);

    // Output the chart
    c->makeChart("layergantt.png");

    //free up resources
    delete c;
    return 0;
}

