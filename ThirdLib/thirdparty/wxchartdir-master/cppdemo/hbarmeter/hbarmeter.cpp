#include "chartdir.h"

int main(int argc, char *argv[])
{
    // The value to display on the meter
    double value = 75.35;

    // Create an LinearMeter object of size 260 x 66 pixels with a very light grey (0xeeeeee)
    // background, and a rounded 3-pixel thick light grey (0xaaaaaa) border
    LinearMeter *m = new LinearMeter(260, 66, 0xeeeeee, 0xaaaaaa);
    m->setRoundedFrame(Chart::Transparent);
    m->setThickFrame(3);

    // Set the scale region top-left corner at (18, 24), with size of 222 x 20 pixels. The scale
    // labels are located on the top (implies horizontal meter)
    m->setMeter(18, 24, 222, 20, Chart::Top);

    // Set meter scale from 0 - 100, with a tick every 10 units
    m->setScale(0, 100, 10);

    // Add a 5-pixel thick smooth color scale to the meter at y = 48 (below the meter scale)
    double smoothColorScale[] = {0, 0x0000ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xffff00, 100,
        0xff0000};
    m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
        smoothColorScale[0]))), 48, 5);

    // Add a light blue (0x0088ff) bar from 0 to the data value with glass effect and 4 pixel
    // rounded corners
    m->addBar(0, value, 0x0088ff, Chart::glassEffect(Chart::NormalGlare, Chart::Top), 4);

    // Output the chart
    m->makeChart("hbarmeter.png");

    //free up resources
    delete m;
    return 0;
}

