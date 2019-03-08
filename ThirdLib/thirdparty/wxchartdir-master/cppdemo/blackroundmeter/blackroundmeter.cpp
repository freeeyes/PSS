#include "chartdir.h"

void createChart(int chartIndex, const char *filename)
{
    // The value to display on the meter
    double value = 72.3;

    // Create an AngularMeter object of size 250 x 250 pixels with transparent background
    AngularMeter *m = new AngularMeter(250, 250, Chart::Transparent);

    // Set the default text and line colors to white (0xffffff)
    m->setColor(Chart::TextColor, 0xffffff);
    m->setColor(Chart::LineColor, 0xffffff);

    // Demonstration two different meter scale angles
    if (chartIndex % 2 == 0) {
        // Center at (125, 125), scale radius = 111 pixels, scale angle -140 to +140 degrees
        m->setMeter(125, 125, 111, -140, 140);
    } else {
        // Center at (125, 125), scale radius = 111 pixels, scale angle -180 to +90 degrees
         m->setMeter(125, 125, 111, -180, 90);
    }

    // Add a black (0x000000) circle with radius 123 pixels as background
    m->addRing(0, 123, 0x000000);

    // Gradient color for the border to make it silver-like
    double ringGradient[] = {1, 0x7f7f7f, 0.5, 0xd6d6d6, 0, 0xffffff, -0.5, 0xd6d6d6, -1, 0x7f7f7f};
    // Add a ring between radii 116 and 122 pixels using the silver gradient as border
    m->addRing(116, 122, m->relativeLinearGradient(DoubleArray(ringGradient, (int)(sizeof(
        ringGradient) / sizeof(ringGradient[0]))), 45, 122));

    // Meter scale is 0 - 100, with major/minor/micro ticks every 10/5/1 units
    m->setScale(0, 100, 10, 5, 1);

    // Set the scale label style to 15pt Arial Italic. Set the major/minor/micro tick lengths to
    // 12/9/6 pixels pointing inwards, and their widths to 2/1/1 pixels.
    m->setLabelStyle("ariali.ttf", 15);
    m->setTickLength(-12, -9, -6);
    m->setLineWidth(0, 2, 1, 1);

    // Demostrate different types of color scales and glare effects and putting them at different
    // positions.
    double smoothColorScale[] = {0, 0x0000ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xdddd00, 100,
        0xff0000};
    double stepColorScale[] = {0, 0x00aa00, 60, 0xddaa00, 80, 0xcc0000, 100};
    double highLowColorScale[] = {0, 0x00ff00, 70, Chart::Transparent, 100, 0xff0000};

    if (chartIndex == 0) {
        // Add the smooth color scale at the default position
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))));
        // Add glare up to radius 116 (= region inside border)
        m->addGlare(116);
    } else if (chartIndex == 1) {
        // Add the smooth color scale starting at radius 62 with zero width and ending at radius 40
        // with 22 pixels outer width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 62, 0, 40, 22);
        // Add glare up to radius 116 (= region inside border), concave and spanning 190 degrees
        m->addGlare(116, -190);
    } else if (chartIndex == 2) {
        // Add the smooth color scale starting at radius 111 with zero width and ending at radius
        // 111 with 12 pixels inner width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 111, 0, 111, -12);
        // Add glare up to radius 116 (= region inside border), concave and spanning 190 degrees and
        // rotated by 45 degrees
        m->addGlare(116, -190, 45);
    } else if (chartIndex == 3) {
        // Add the high/low color scale at the default position
        m->addColorScale(DoubleArray(highLowColorScale, (int)(sizeof(highLowColorScale) / sizeof(
            highLowColorScale[0]))));
    } else if (chartIndex == 4) {
        // Add the smooth color scale at radius 44 with 16 pixels outer width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 44, 16);
        // Add glare up to radius 116 (= region inside border), concave and spanning 190 degrees and
        // rotated by -45 degrees
        m->addGlare(116, -190, -45);
    } else {
        // Add the step color scale at the default position
        m->addColorScale(DoubleArray(stepColorScale, (int)(sizeof(stepColorScale) / sizeof(
            stepColorScale[0]))));
    }

    // Add a text label centered at (125, 175) with 15pt Arial Italic font
    m->addText(125, 175, "CPU", "ariali.ttf", 15, Chart::TextColor, Chart::Center);

    // Add a readout to some of the charts as demonstration
    if (chartIndex == 0 || chartIndex == 2) {
        // Put the value label center aligned at (125, 232), using black (0x000000) 14pt Arial font
        // on a light blue (0x99ccff) background. Set box width to 50 pixels with 5 pixels rounded
        // corners.
        TextBox *t = m->addText(125, 232, m->formatValue(value,
            "<*block,width=50,halign=center*>{value|1}"), "arial.ttf", 14, 0x000000,
            Chart::BottomCenter);
        t->setBackground(0x99ccff);
        t->setRoundedCorners(5);
    }

    // Add a red (0xff0000) pointer at the specified value
    m->addPointer2(value, 0xff0000);

    // Output the chart
    m->makeChart(filename);

    //free up resources
    delete m;
}

int main(int argc, char *argv[])
{
    createChart(0, "blackroundmeter0.png");
    createChart(1, "blackroundmeter1.png");
    createChart(2, "blackroundmeter2.png");
    createChart(3, "blackroundmeter3.png");
    createChart(4, "blackroundmeter4.png");
    createChart(5, "blackroundmeter5.png");
    return 0;
}

