#include "chartdir.h"

void createChart(int chartIndex, const char *filename)
{
    // The value to display on the meter
    double value = 66.77;

    // The background, border and bar colors of the meters
    int bgColor[] = {0xcce4ff, 0xccffcc, 0xffdddd, 0xffffaa, 0xffccff, 0xeeeeee};
    int borderColor[] = {0x000088, 0x006600, 0x880000, 0xee6600, 0x6600aa, 0x666666};
    int barColor[] = {0x2299ff, 0x00ee00, 0xee4455, 0xff8800, 0x8833dd, 0x888888};

    // Create a LinearMeter object of size 70 x 260 pixels with a 3-pixel thick rounded frame
    LinearMeter *m = new LinearMeter(70, 260, bgColor[chartIndex], borderColor[chartIndex]);
    m->setRoundedFrame(Chart::Transparent);
    m->setThickFrame(3);

    // Set the scale region top-left corner at (28, 33), with size of 20 x 194 pixels. The scale
    // labels are located on the left (default - implies vertical meter).
    m->setMeter(28, 33, 20, 194);

    // Set meter scale from 0 - 100, with a tick every 10 units
    m->setScale(0, 100, 10);

    // Demostrate different types of color scales
    double smoothColorScale[] = {0, 0x0000ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xdddd00, 100,
        0xff0000};
    double stepColorScale[] = {0, 0x33ff33, 50, 0xffff00, 80, 0xff3333, 100};
    double highLowColorScale[] = {0, 0x0000ff, 40, Chart::Transparent, 60, Chart::Transparent, 100,
        0xff0000};
    double highColorScale[] = {70, Chart::Transparent, 100, 0xff0000};
    double lowColorScale[] = {0, 0x0000ff, 30, Chart::Transparent};

    if (chartIndex == 0) {
        // Add a 6-pixel thick smooth color scale at x = 53 (right of meter scale)
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 53, 6);
    } else if (chartIndex == 1) {
        // Add a high only color scale at x = 52 (right of meter scale) with thickness varying from
        // 0 to 8
        m->addColorScale(DoubleArray(highColorScale, (int)(sizeof(highColorScale) / sizeof(
            highColorScale[0]))), 52, 0, 52, 8);
        // Add a low only color scale at x = 52 (right of meter scale) with thickness varying from 8
        // to 0
        m->addColorScale(DoubleArray(lowColorScale, (int)(sizeof(lowColorScale) / sizeof(
            lowColorScale[0]))), 52, 8, 52, 0);
    } else if (chartIndex == 2) {
        // Add a high only color scale at x = 52 (right of meter scale) with thickness varying from
        // 0 to 8
        m->addColorScale(DoubleArray(highColorScale, (int)(sizeof(highColorScale) / sizeof(
            highColorScale[0]))), 52, 0, 52, 8);
    } else if (chartIndex == 3) {
        // Add a smooth color scale at x = 52 (right of meter scale) with thickness varying from 0
        // to 8
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 52, 0, 52, 8);
    } else if (chartIndex == 4) {
        // Add a 6-pixel thick high/low color scale at x = 53 (right of meter scale)
        m->addColorScale(DoubleArray(highLowColorScale, (int)(sizeof(highLowColorScale) / sizeof(
            highLowColorScale[0]))), 53, 6);
    } else {
        // Add a 6-pixel thick step color scale at x = 53 (right of meter scale)
        m->addColorScale(DoubleArray(stepColorScale, (int)(sizeof(stepColorScale) / sizeof(
            stepColorScale[0]))), 53, 6);
    }

    // Add a bar from 0 to value with glass effect and 4 pixel rounded corners
    m->addBar(0, value, barColor[chartIndex], Chart::glassEffect(Chart::NormalGlare, Chart::Left), 4
        );

    // Add a title using white (0xffffff) 8pt Arial Bold font with a border color background
    m->addTitle("Temp C", "arialbd.ttf", 8, 0xffffff)->setBackground(borderColor[chartIndex]);

    // Add a bottom title using white (0xffffff) 8pt Arial Bold font with a border color background
    // to display the value
    m->addTitle(Chart::Bottom, m->formatValue(value, "2"), "arialbd.ttf", 8, 0xffffff
        )->setBackground(borderColor[chartIndex]);

    // Output the chart
    m->makeChart(filename);

    //free up resources
    delete m;
}

int main(int argc, char *argv[])
{
    createChart(0, "colorvbarmeter0.png");
    createChart(1, "colorvbarmeter1.png");
    createChart(2, "colorvbarmeter2.png");
    createChart(3, "colorvbarmeter3.png");
    createChart(4, "colorvbarmeter4.png");
    createChart(5, "colorvbarmeter5.png");
    return 0;
}

