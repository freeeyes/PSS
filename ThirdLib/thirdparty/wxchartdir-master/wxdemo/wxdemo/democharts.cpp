/*
** Name:        democharts.cpp
** Purpose:     wxWidgets Demo sample application
** Copyright:   (C) 2016, Advanced Software Engineering Limited (QT based implementation)
**              (C) 2018, Ulrich Telle (Adjustments for wxWidgets)
** License:     ChartDirector license
*/

#include "democharts.h"
#include "chartdir.h"
#include "FinanceChart.h"

////////////////////////////////////////////////////////////////////////////////
//
//	Implementation of Demo Charts
//
////////////////////////////////////////////////////////////////////////////////

BaseChart *simplepie(int /* chartIndex */, const char **imageMap)
{
    // The data for the pie chart
    double data[] = {25, 18, 15, 12, 8, 30, 35};

    // The labels for the pie chart
    const char *labels[] = {"Labor", "Licenses", "Taxes", "Legal", "Insurance", "Facilities",
        "Production"};

    // Create a PieChart object of size 360 x 300 pixels
    PieChart *c = new PieChart(360, 300);

    // Set the center of the pie at (180, 140) and the radius to 100 pixels
    c->setPieSize(180, 140, 100);

    // Set the pie data and the pie labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US${value}K ({percent}%)'");

    return c;
}


BaseChart *threedpie(int /* chartIndex */, const char **imageMap)
{
    // The data for the pie chart
    double data[] = {25, 18, 15, 12, 8, 30, 35};

    // The labels for the pie chart
    const char *labels[] = {"Labor", "Licenses", "Taxes", "Legal", "Insurance", "Facilities",
        "Production"};

    // Create a PieChart object of size 360 x 300 pixels
    PieChart *c = new PieChart(360, 300);

    // Set the center of the pie at (180, 140) and the radius to 100 pixels
    c->setPieSize(180, 140, 100);

    // Add a title to the pie chart
    c->addTitle("Project Cost Breakdown");

    // Draw the pie in 3D
    c->set3D();

    // Set the pie data and the pie labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Explode the 1st sector (index = 0)
    c->setExplode(0);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US${value}K ({percent}%)'");

    return c;
}


BaseChart *multidepthpie(int /* chartIndex */, const char **imageMap)
{
    // The data for the pie chart
    double data[] = {72, 18, 15, 12};

    // The labels for the pie chart
    const char *labels[] = {"Labor", "Machinery", "Facilities", "Computers"};

    // The depths for the sectors
    double depths[] = {30, 20, 10, 10};

    // Create a PieChart object of size 360 x 300 pixels, with a light blue (DDDDFF) background and
    // a 1 pixel 3D border
    PieChart *c = new PieChart(360, 300, 0xddddff, -1, 1);

    // Set the center of the pie at (180, 175) and the radius to 100 pixels
    c->setPieSize(180, 175, 100);

    // Add a title box using 15pt Times Bold Italic font and blue (AAAAFF) as background color
    c->addTitle("Project Cost Breakdown", "timesbi.ttf", 15)->setBackground(0xaaaaff);

    // Set the pie data and the pie labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Draw the pie in 3D with variable 3D depths
    c->set3D(DoubleArray(depths, (int)(sizeof(depths) / sizeof(depths[0]))));

    // Set the start angle to 225 degrees may improve layout when the depths of the sector are
    // sorted in descending order, because it ensures the tallest sector is at the back.
    c->setStartAngle(225);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US${value}K ({percent}%)'");

    return c;
}


BaseChart *sidelabelpie(int /* chartIndex */, const char **imageMap)
{
    // The data for the pie chart
    double data[] = {35, 30, 25, 7, 6, 5, 4, 3, 2, 1};

    // The labels for the pie chart
    const char *labels[] = {"Labor", "Production", "Facilities", "Taxes", "Misc", "Legal",
        "Insurance", "Licenses", "Transport", "Interest"};

    // Create a PieChart object of size 560 x 270 pixels, with a golden background and a 1 pixel 3D
    // border
    PieChart *c = new PieChart(560, 270, Chart::goldColor(), -1, 1);

    // Add a title box using 15pt Times Bold Italic font and metallic pink background color
    c->addTitle("Project Cost Breakdown", "timesbi.ttf", 15)->setBackground(Chart::metalColor(
        0xff9999));

    // Set the center of the pie at (280, 135) and the radius to 110 pixels
    c->setPieSize(280, 135, 110);

    // Draw the pie in 3D with 20 pixels 3D depth
    c->set3D(20);

    // Use the side label layout method
    c->setLabelLayout(Chart::SideLayout);

    // Set the label box background color the same as the sector color, with glass effect, and with
    // 5 pixels rounded corners
    TextBox *t = c->setLabelStyle();
    t->setBackground(Chart::SameAsMainColor, Chart::Transparent, Chart::glassEffect());
    t->setRoundedCorners(5);

    // Set the border color of the sector the same color as the fill color. Set the line color of
    // the join line to black (0x0)
    c->setLineColor(Chart::SameAsMainColor, 0x000000);

    // Set the start angle to 135 degrees may improve layout when there are many small sectors at
    // the end of the data array (that is, data sorted in descending order). It is because this
    // makes the small sectors position near the horizontal axis, where the text label has the least
    // tendency to overlap. For data sorted in ascending order, a start angle of 45 degrees can be
    // used instead.
    c->setStartAngle(135);

    // Set the pie data and the pie labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US${value}K ({percent}%)'");

    return c;
}


BaseChart *circlelabelpie(int chartIndex, const char **imageMap)
{
    // The data for the pie chart
    double data[] = {42, 18, 8};

    // The labels for the pie chart
    const char *labels[] = {"Agree", "Disagree", "Not Sure"};

    // The colors to use for the sectors
    int colors[] = {0x66ff66, 0xff6666, 0xffff00};

    // Create a PieChart object of size 300 x 300 pixels. Set the background to a gradient color
    // from blue (aaccff) to sky blue (ffffff), with a grey (888888) border. Use rounded corners and
    // soft drop shadow.
    PieChart *c = new PieChart(300, 300);
    c->setBackground(c->linearGradientColor(0, 0, 0, c->getHeight() / 2, 0xaaccff, 0xffffff),
        0x888888);
    c->setRoundedFrame();
    c->setDropShadow();

    if (chartIndex == 0) {
    //============================================================
    //    Draw a pie chart where the label is on top of the pie
    //============================================================

        // Set the center of the pie at (150, 150) and the radius to 120 pixels
        c->setPieSize(150, 150, 120);

        // Set the label position to -40 pixels from the perimeter of the pie (-ve means label is
        // inside the pie)
        c->setLabelPos(-40);

    } else {
    //============================================================
    //    Draw a pie chart where the label is outside the pie
    //============================================================

        // Set the center of the pie at (150, 150) and the radius to 80 pixels
        c->setPieSize(150, 150, 80);

        // Set the sector label position to be 20 pixels from the pie. Use a join line to connect
        // the labels to the sectors.
        c->setLabelPos(20, Chart::LineColor);

    }

    // Set the pie data and the pie labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Set the sector colors
    c->setColors(Chart::DataColor, IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));

    // Use local gradient shading, with a 1 pixel semi-transparent black (bb000000) border
    c->setSectorStyle(Chart::LocalGradientShading, 0xbb000000, 1);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{label}: {value} responses ({percent}%)'");

    return c;
}


BaseChart *legendpie(int /* chartIndex */, const char **imageMap)
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
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US${value}K ({percent}%)'");

    return c;
}


BaseChart *legendpie2(int /* chartIndex */, const char **imageMap)
{
    // The data for the pie chart
    double data[] = {21, 18, 15, 12, 8, 24};

    // The labels for the pie chart
    const char *labels[] = {"Labor", "Licenses", "Taxes", "Legal", "Facilities", "Production"};

    // The colors to use for the sectors
    int colors[] = {0x66aaee, 0xeebb22, 0xbbbbbb, 0x8844ff, 0xdd2222, 0x009900};

    // Create a PieChart object of size 600 x 320 pixels. Use a vertical gradient color from light
    // blue (99ccff) to white (ffffff) spanning the top 100 pixels as background. Set border to grey
    // (888888). Use rounded corners. Enable soft drop shadow.
    PieChart *c = new PieChart(600, 320);
    c->setBackground(c->linearGradientColor(0, 0, 0, 100, 0x99ccff, 0xffffff), 0x888888);
    c->setRoundedFrame();
    c->setDropShadow();

    // Add a title using 18pt Times New Roman Bold Italic font. Add 16 pixels top margin to the
    // title.
    c->addTitle("Pie Chart With Legend Demonstration", "timesbi.ttf", 18)->setMargin(0, 0, 16, 0);

    // Set the center of the pie at (160, 165) and the radius to 110 pixels
    c->setPieSize(160, 165, 110);

    // Draw the pie in 3D with a pie thickness of 25 pixels
    c->set3D(25);

    // Set the pie data and the pie labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Set the sector colors
    c->setColors(Chart::DataColor, IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));

    // Use local gradient shading for the sectors
    c->setSectorStyle(Chart::LocalGradientShading);

    // Use the side label layout method, with the labels positioned 16 pixels from the pie bounding
    // box
    c->setLabelLayout(Chart::SideLayout, 16);

    // Show only the sector number as the sector label
    c->setLabelFormat("{={sector}+1}");

    // Set the sector label style to Arial Bold 10pt, with a dark grey (444444) border
    c->setLabelStyle("arialbd.ttf", 10)->setBackground(Chart::Transparent, 0x444444);

    // Add a legend box, with the center of the left side anchored at (330, 175), and using 10pt
    // Arial Bold Italic font
    LegendBox *b = c->addLegend(330, 175, true, "arialbi.ttf", 10);
    b->setAlignment(Chart::Left);

    // Set the legend box border to dark grey (444444), and with rounded conerns
    b->setBackground(Chart::Transparent, 0x444444);
    b->setRoundedCorners();

    // Set the legend box margin to 16 pixels, and the extra line spacing between the legend entries
    // as 5 pixels
    b->setMargin(16);
    b->setKeySpacing(0, 5);

    // Set the legend box icon to have no border (border color same as fill color)
    b->setKeyBorder(Chart::SameAsMainColor);

    // Set the legend text to show the sector number, followed by a 120 pixels wide block showing
    // the sector label, and a 40 pixels wide block showing the percentage
    b->setText(
        "<*block,valign=top*>{={sector}+1}.<*advanceTo=22*><*block,width=120*>{label}<*/*>"
        "<*block,width=40,halign=right*>{percent}<*/*>%");

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US${value}K ({percent}%)'");

    return c;
}


BaseChart *explodedpie(int /* chartIndex */, const char **imageMap)
{
    // The data for the pie chart
    double data[] = {21, 18, 15, 12, 8, 24};

    // The labels for the pie chart
    const char *labels[] = {"Labor", "Licenses", "Taxes", "Legal", "Facilities", "Production"};

    // The colors to use for the sectors
    int colors[] = {0x66aaee, 0xeebb22, 0xbbbbbb, 0x8844ff, 0xdd2222, 0x009900};

    // Create a PieChart object of size 600 x 360 pixels.
    PieChart *c = new PieChart(600, 360);

    // Use the white on black palette, which means the default text and line colors are white
    c->setColors(Chart::whiteOnBlackPalette);

    // Use a vertical gradient color from deep blue (000066) to blue (0000cc) as background. Use
    // rounded corners of 20 pixels radius. Enable soft drop shadow.
    c->setBackground(c->linearGradientColor(0, 0, 0, c->getHeight(), 0x000066, 0x0000cc));
    c->setRoundedFrame(0xffffff, 20);
    c->setDropShadow();

    // Add a title using 18pt Times New Roman Bold Italic font. Add 16 pixels top margin to the
    // title.
    c->addTitle("Exploded Pie Chart Demonstration", "timesbi.ttf", 18)->setMargin(0, 0, 16, 0);

    // Set the center of the pie at (300, 195) and the radius to 110 pixels
    c->setPieSize(300, 195, 110);

    // Set the pie data and the pie labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Set the sector colors
    c->setColors(Chart::DataColor, IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));

    // Use local gradient shading for the sectors, with 5 pixels wide semi-transparent white
    // (bbffffff) borders
    c->setSectorStyle(Chart::LocalGradientShading, 0xbbffffff, 5);

    // Use the side label layout method
    c->setLabelLayout(Chart::SideLayout);

    // Use 10pt Arial Bold as the default label font. Set the label box background color the same as
    // the sector color. Use soft lighting effect with light direction from right. Use 8 pixels
    // rounded corners.
    TextBox *t = c->setLabelStyle("arialbd.ttf", 10, 0x000000);
    t->setBackground(Chart::SameAsMainColor, Chart::Transparent, Chart::softLighting(Chart::Right, 0
        ));
    t->setRoundedCorners(8);

    // Set the sector label format. The label is centered in a 110 pixels wide bounding box. It
    // consists of two lines. The first line is the sector name. The second line shows the data
    // value and percentage.
    c->setLabelFormat(
        "<*block,halign=center,width=110*>{label}\n<*font=arial.ttf,size=8*>US$ {value}M "
        "({percent}%)<*/*>");

    // Explode all sectors 10 pixels from the center
    c->setExplode(-1, 10);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US${value}M ({percent}%)'");

    return c;
}


BaseChart *iconpie(int /* chartIndex */, const char **imageMap)
{
    // The data for the pie chart
    double data[] = {72, 18, 15, 12};

    // The depths for the sectors
    double depths[] = {30, 20, 10, 10};

    // The labels for the pie chart
    const char *labels[] = {"Sunny", "Cloudy", "Rainy", "Snowy"};

    // The icons for the sectors
    const char *icons[] = {"sun.png", "cloud.png", "rain.png", "snowy.png"};

    // Create a PieChart object of size 400 x 310 pixels, with a blue (CCCCFF) vertical metal
    // gradient background, black border, 1 pixel 3D border effect and rounded corners
    PieChart *c = new PieChart(400, 310, Chart::metalColor(0xccccff, 0), 0x000000, 1);
    c->setRoundedFrame();

    // Set the center of the pie at (200, 180) and the radius to 100 pixels
    c->setPieSize(200, 180, 100);

    // Add a title box using 15pt Times Bold Italic font, on a blue (CCCCFF) background with glass
    // effect
    c->addTitle("Weather Profile in Wonderland", "timesbi.ttf", 15)->setBackground(0xccccff,
        0x000000, Chart::glassEffect());

    // Set the pie data and the pie labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Add icons to the chart as a custom field
    c->addExtraField(StringArray(icons, (int)(sizeof(icons) / sizeof(icons[0]))));

    // Configure the sector labels using CDML to include the icon images
    c->setLabelFormat(
        "<*block,valign=absmiddle*><*img={field0}*> <*block*>{label}\n{percent}%<*/*><*/*>");

    // Draw the pie in 3D with variable 3D depths
    c->set3D(DoubleArray(depths, (int)(sizeof(depths) / sizeof(depths[0]))));

    // Set the start angle to 225 degrees may improve layout when the depths of the sector are
    // sorted in descending order, because it ensures the tallest sector is at the back.
    c->setStartAngle(225);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: {value} days ({percent}%)'");

    return c;
}


BaseChart *iconpie2(int /* chartIndex */, const char **imageMap)
{
    // The data for the pie chart
    double data[] = {28, 45, 5, 1, 12};

    // The labels for the pie chart
    const char *labels[] = {"Excellent", "Good", "Bad", "Very Bad", "Neutral"};

    // The icons for the sectors
    const char *icons[] = {"laugh.png", "smile.png", "sad.png", "angry.png", "nocomment.png"};

    // Create a PieChart object of size 560 x 300 pixels, with a silver background, black border, 1
    // pxiel 3D border effect and rounded corners
    PieChart *c = new PieChart(560, 300, Chart::silverColor(), 0x000000, 1);
    c->setRoundedFrame();

    // Set the center of the pie at (280, 150) and the radius to 120 pixels
    c->setPieSize(280, 150, 120);

    // Add a title box with title written in CDML, on a sky blue (A0C8FF) background with glass
    // effect
    c->addTitle(
        "<*block,valign=absmiddle*><*img=doc.png*> Customer Survey: "
        "<*font=timesi.ttf,color=000000*>Do you like our <*font,color=dd0000*>Hyper<*super*>TM"
        "<*/font*> molecules?", "timesbi.ttf", 15, 0x000080)->setBackground(0xa0c8ff, 0x000000,
        Chart::glassEffect());

    // Add a logo to the chart written in CDML as the bottom title aligned to the bottom right
    c->addTitle(Chart::BottomRight,
        "<*block,valign=absmiddle*><*img=molecule.png*> <*block*><*color=FF*>"
        "<*font=timesbi.ttf,size=12*>Molecular Engineering\n<*font=arial.ttf,size=10*>Creating "
        "better molecules");

    // Set the pie data and the pie labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Set 3D style
    c->set3D();

    // Use the side label layout method
    c->setLabelLayout(Chart::SideLayout);

    // Set the label background color to transparent
    c->setLabelStyle()->setBackground(Chart::Transparent);

    // Add icons to the chart as a custom field
    c->addExtraField(StringArray(icons, (int)(sizeof(icons) / sizeof(icons[0]))));

    // Configure the sector labels using CDML to include the icon images
    c->setLabelFormat("<*block,valign=absmiddle*><*img={field0}*> {label} ({percent|0}%)");

    // Explode the 3rd and 4th sectors as a group (index = 2 and 3)
    c->setExplodeGroup(2, 3);

    // Set the start angle to 135 degrees may improve layout when there are many small sectors at
    // the end of the data array (that is, data sorted in descending order). It is because this
    // makes the small sectors position near the horizontal axis, where the text label has the least
    // tendency to overlap. For data sorted in ascending order, a start angle of 45 degrees can be
    // used instead.
    c->setStartAngle(135);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{label}: {value} responses ({percent}%)'");

    return c;
}


BaseChart *multipie(int chartIndex, const char **imageMap)
{
    // The data for the pie chart
    double data0[] = {25, 18, 15};
    double data1[] = {14, 32, 24};
    double data2[] = {25, 23, 9};

    // The labels for the pie chart
    const char *labels[] = {"Software", "Hardware", "Services"};

    // Create a PieChart object of size 180 x 160 pixels
    PieChart *c = new PieChart(180, 160);

    // Set the center of the pie at (90, 80) and the radius to 60 pixels
    c->setPieSize(90, 80, 60);

    // Set the border color of the sectors to white (ffffff)
    c->setLineColor(0xffffff);

    // Set the background color of the sector label to pale yellow (ffffc0) with a black border
    // (000000)
    c->setLabelStyle()->setBackground(0xffffc0, 0x000000);

    // Set the label to be slightly inside the perimeter of the circle
    c->setLabelLayout(Chart::CircleLayout, -10);

    // Set the title, data and colors according to which pie to draw
    if (chartIndex == 0) {
        c->addTitle("Alpha Division", "arialbd.ttf", 8);
        c->setData(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), StringArray(labels,
            (int)(sizeof(labels) / sizeof(labels[0]))));
        int pattern1[] = {0xff3333, 0xff9999, 0xffcccc};
        c->setColors(Chart::DataColor, IntArray(pattern1, (int)(sizeof(pattern1) / sizeof(pattern1[0
            ]))));
    } else if (chartIndex == 1) {
        c->addTitle("Beta Division", "arialbd.ttf", 8);
        c->setData(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), StringArray(labels,
            (int)(sizeof(labels) / sizeof(labels[0]))));
        int pattern2[] = {0x33ff33, 0x99ff99, 0xccffcc};
        c->setColors(Chart::DataColor, IntArray(pattern2, (int)(sizeof(pattern2) / sizeof(pattern2[0
            ]))));
    } else {
        c->addTitle("Gamma Division", "arialbd.ttf", 8);
        c->setData(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), StringArray(labels,
            (int)(sizeof(labels) / sizeof(labels[0]))));
        int pattern3[] = {0x3333ff, 0x9999ff, 0xccccff};
        c->setColors(Chart::DataColor, IntArray(pattern3, (int)(sizeof(pattern3) / sizeof(pattern3[0
            ]))));
    }

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US${value}M ({percent}%)'");

    return c;
}


BaseChart *donut(int /* chartIndex */, const char **imageMap)
{
    // The data for the pie chart
    double data[] = {25, 18, 15, 12, 8, 30, 35};

    // The labels for the pie chart
    const char *labels[] = {"Labor", "Licenses", "Taxes", "Legal", "Insurance", "Facilities",
        "Production"};

    // Create a PieChart object of size 600 x 320 pixels. Set background color to brushed silver,
    // with a 2 pixel 3D border. Use rounded corners of 20 pixels radius.
    PieChart *c = new PieChart(600, 320, Chart::brushedSilverColor(), Chart::Transparent, 2);
    c->setRoundedFrame(0xffffff, 20);

    // Add a title using 18pt Times New Roman Bold Italic font. #Set top/bottom margins to 8 pixels.
    TextBox *title = c->addTitle("Donut Chart Demonstration", "timesbi.ttf", 18);
    title->setMargin(0, 0, 8, 8);

    // Add a 2 pixels wide separator line just under the title
    c->addLine(10, title->getHeight(), c->getWidth() - 11, title->getHeight(), Chart::LineColor, 2);

    // Set donut center at (160, 175), and outer/inner radii as 110/55 pixels
    c->setDonutSize(160, 175, 110, 55);

    // Set the pie data and the pie labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Use ring shading effect for the sectors
    c->setSectorStyle(Chart::RingShading);

    // Use the side label layout method, with the labels positioned 16 pixels from the donut
    // bounding box
    c->setLabelLayout(Chart::SideLayout, 16);

    // Show only the sector number as the sector label
    c->setLabelFormat("{={sector}+1}");

    // Set the sector label style to Arial Bold 10pt, with a dark grey (444444) border
    c->setLabelStyle("arialbd.ttf", 10)->setBackground(Chart::Transparent, 0x444444);

    // Add a legend box, with the center of the left side anchored at (330, 175), and using 10pt
    // Arial Bold Italic font
    LegendBox *b = c->addLegend(330, 175, true, "arialbi.ttf", 10);
    b->setAlignment(Chart::Left);

    // Set the legend box border to dark grey (444444), and with rounded conerns
    b->setBackground(Chart::Transparent, 0x444444);
    b->setRoundedCorners();

    // Set the legend box margin to 16 pixels, and the extra line spacing between the legend entries
    // as 5 pixels
    b->setMargin(16);
    b->setKeySpacing(0, 5);

    // Set the legend text to show the sector number, followed by a 120 pixels wide block showing
    // the sector label, and a 40 pixels wide block showing the percentage
    b->setText(
        "<*block,valign=top*>{={sector}+1}.<*advanceTo=22*><*block,width=120*>{label}<*/*>"
        "<*block,width=40,halign=right*>{percent}<*/*>%");

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US${value}K ({percent}%)'");

    return c;
}


BaseChart *threeddonut(int /* chartIndex */, const char **imageMap)
{
    // The data for the pie chart
    double data[] = {20, 10, 15, 12};

    // The labels for the pie chart
    const char *labels[] = {"Labor", "Licenses", "Facilities", "Production"};

    // The colors to use for the sectors
    int colors[] = {0x66aaee, 0xeebb22, 0xcccccc, 0xcc88ff};

    // Create a PieChart object of size 600 x 320 pixels. Use a vertical gradient color from deep
    // blue (000066) to blue (0000cc) as background. Use rounded corners of 20 pixels radius.
    PieChart *c = new PieChart(600, 320);
    c->setBackground(c->linearGradientColor(0, 0, 0, c->getHeight(), 0x000066, 0x0000cc));
    c->setRoundedFrame(0xffffff, 20);

    // Add a title using 18pt Times New Roman Bold Italic font in white (ffffff) color. Set
    // top/bottom margins to 8 pixels.
    TextBox *title = c->addTitle("Donut Chart Demonstration", "timesbi.ttf", 18, 0xffffff);
    title->setMargin(0, 0, 8, 8);

    // Add a separator line in white color just under the title
    c->addLine(10, title->getHeight(), c->getWidth() - 11, title->getHeight(), 0xffffff);

    // Set donut center at (300, 170), and outer/inner radii as 120/60 pixels
    c->setDonutSize(300, 170, 120, 60);

    // Draw the pie in 3D with a pie thickness of 25 pixels
    c->set3D(25);

    // Set the pie data and the pie labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Set the sector colors
    c->setColors(Chart::DataColor, IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));

    // Use local gradient shading for the sectors
    c->setSectorStyle(Chart::LocalGradientShading);

    // Set sector border color to same as fill color. Set label join line color to white (ffffff)
    c->setLineColor(Chart::SameAsMainColor, 0xffffff);

    // Use the side label layout method, with the labels positioned 16 pixels from the donut
    // bounding box
    c->setLabelLayout(Chart::SideLayout, 16);

    // Set the sector label format. The label consists of two lines. The first line is the sector
    // name in Times Bold Italic font and is underlined. The second line shows the data value and
    // percentage.
    c->setLabelFormat(
        "<*block,halign=left*><*font=timesbi.ttf,size=12,underline=1*>{label}<*/font*><*br*>US$ "
        "{value}K ({percent}%)");

    // Set the label box background color the same as the sector color. Use soft lighting effect
    // with light direction from right. Use rounded corners.
    TextBox *t = c->setLabelStyle();
    t->setBackground(Chart::SameAsMainColor, Chart::Transparent, Chart::softLighting(Chart::Right));
    t->setRoundedCorners();

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US${value}K ({percent}%)'");

    return c;
}


BaseChart *icondonut(int /* chartIndex */, const char **imageMap)
{
    // The data for the pie chart
    double data[] = {72, 18, 15, 12};

    // The depths for the sectors
    double depths[] = {30, 20, 10, 10};

    // The labels for the pie chart
    const char *labels[] = {"Sunny", "Cloudy", "Rainy", "Snowy"};

    // The icons for the sectors
    const char *icons[] = {"sun.png", "cloud.png", "rain.png", "snowy.png"};

    // Create a PieChart object of size 400 x 300 pixels
    PieChart *c = new PieChart(400, 300);

    // Use the semi-transparent palette for this chart
    c->setColors(Chart::transparentPalette);

    // Set the background to metallic light blue (CCFFFF), with a black border and 1 pixel 3D border
    // effect,
    c->setBackground(Chart::metalColor(0xccccff), 0x000000, 1);

    // Set donut center at (200, 175), and outer/inner radii as 100/50 pixels
    c->setDonutSize(200, 175, 100, 50);

    // Add a title box using 15pt Times Bold Italic font and metallic blue (8888FF) background color
    c->addTitle("Weather Profile in Wonderland", "timesbi.ttf", 15)->setBackground(
        Chart::metalColor(0x8888ff));

    // Set the pie data and the pie labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Add icons to the chart as a custom field
    c->addExtraField(StringArray(icons, (int)(sizeof(icons) / sizeof(icons[0]))));

    // Configure the sector labels using CDML to include the icon images
    c->setLabelFormat(
        "<*block,valign=absmiddle*><*img={field0}*> <*block*>{label}\n{percent}%<*/*><*/*>");

    // Draw the pie in 3D with variable 3D depths
    c->set3D(DoubleArray(depths, (int)(sizeof(depths) / sizeof(depths[0]))));

    // Set the start angle to 225 degrees may improve layout when the depths of the sector are
    // sorted in descending order, because it ensures the tallest sector is at the back.
    c->setStartAngle(225);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: {value} days ({percent}%)'");

    return c;
}


BaseChart *texturedonut(int /* chartIndex */, const char **imageMap)
{
    // The data for the pie chart
    double data[] = {18, 45, 28};

    // The labels for the pie chart
    const char *labels[] = {"Marble", "Wood", "Granite"};

    // The icons for the sectors
    const char *texture[] = {"marble3.png", "wood.png", "rock.png"};

    // Create a PieChart object of size 400 x 330 pixels, with a metallic green (88EE88) background,
    // black border and 1 pixel 3D border effect
    PieChart *c = new PieChart(400, 330, Chart::metalColor(0x88ee88), 0x000000, 1);

    // Set donut center at (200, 160), and outer/inner radii as 120/60 pixels
    c->setDonutSize(200, 160, 120, 60);

    // Add a title box using 15pt Times Bold Italic font and metallic deep green (008000) background
    // color
    c->addTitle("Material Composition", "timesbi.ttf", 15)->setBackground(Chart::metalColor(0x008000
        ));

    // Set the pie data and the pie labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Set the colors of the sectors to the 3 texture patterns
    c->setColor(Chart::DataColor + 0, c->patternColor(texture[0]));
    c->setColor(Chart::DataColor + 1, c->patternColor(texture[1]));
    c->setColor(Chart::DataColor + 2, c->patternColor(texture[2]));

    // Draw the pie in 3D with a 3D depth of 30 pixels
    c->set3D(30);

    // Use 12pt Arial Bold Italic as the sector label font
    c->setLabelStyle("arialbi.ttf", 12);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: {value}kg ({percent}%)'");

    return c;
}


BaseChart *concentric(int /* chartIndex */, const char ** /* imageMap */)
{
    // Data for outer ring
    double data[] = {88, 124, 96};

    // Data for inner ring
    double data2[] = {77, 87, 45};

    // Labels for the sectors
    const char *labels[] = {"Hardware", "Software", "Services"};

    // Colors for the sectors
    int colors[] = {0xff9999, 0x9999ff, 0x66ff66};

    //
    // Create the main chart, which contains the chart title, the outer ring, and the legend box
    //

    // Create a PieChart object of size 450 x 360 pixels, with transparent background
    PieChart *c = new PieChart(450, 360);

    // Add a title to the chart with 18pt Times Bold Italic font
    c->addTitle("Concentric Donut Chart", "timesbi.ttf", 18);

    // Set donut center at (160, 200), and outer/inner radii as 150/100 pixels
    c->setDonutSize(160, 200, 150, 100);

    // Add a label at the bottom-right corner of the ring to label the outer ring Use 12pt Arial
    // Bold Italic font in white (ffffff) color, on a green (008800) background, with soft lighting
    // effect and 5 pixels rounded corners
    TextBox *t = c->addText(260, 300, " Year 2006 ", "arialbi.ttf", 12, 0xffffff);
    t->setBackground(0x008800, Chart::Transparent, Chart::softLighting());
    t->setRoundedCorners(5);

    // Set the legend box at (320, 50) with 12pt Arial Bold Italic font, with no border
    c->addLegend(320, 50, true, "arialbi.ttf", 13)->setBackground(Chart::Transparent,
        Chart::Transparent);

    // Set the pie data and the pie labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Set the pie colors
    c->setColors(Chart::DataColor, IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));

    // Set pie border color to white (ffffff)
    c->setLineColor(0xffffff);

    // Set pie label to value in $###M format, percentage in (##.#%) format, in two lines.
    c->setLabelFormat("${value}M<*br*>({percent|1}%)");

    // Use 10pt Airal Bold for the sector labels
    c->setLabelStyle("arialbd.ttf", 10);

    // Set the label position to -25 pixels from the sector (which would be internal to the sector)
    c->setLabelPos(-25);

    //
    // Create the inner ring.
    //

    // Create a PieChart object of size 280 x 320 pixels, with transparent background
    PieChart *c2 = new PieChart(280, 320, Chart::Transparent);

    // Set donut center at (110, 110), and outer/inner radii as 100/50 pixels
    c2->setDonutSize(110, 110, 100, 50);

    // Add a label at the center of the ring to label the inner ring. Use 12pt Arial Bold Italic
    // font in white (ffffff) color, on a deep blue (0000cc) background, with soft lighting effect
    // and 5 pixels rounded corners
    TextBox *t2 = c2->addText(110, 110, " Year 2005 ", "arialbi.ttf", 12, 0xffffff, Chart::Center);
    t2->setBackground(0x0000cc, Chart::Transparent, Chart::softLighting());
    t2->setRoundedCorners(5);

    // Set the pie data and the pie labels
    c2->setData(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), StringArray(labels,
        (int)(sizeof(labels) / sizeof(labels[0]))));

    // Set the pie colors
    c2->setColors(Chart::DataColor, IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));

    // Set pie border color to white (ffffff)
    c2->setLineColor(0xffffff);

    // Set pie label to value in $###M format, percentage in (##.#%) format, in two lines.
    c2->setLabelFormat("${value}M<*br*>({percent|1}%)");

    // Use 10pt Airal Bold for the sector labels
    c2->setLabelStyle("arialbd.ttf", 10);

    // Set the label position to -25 pixels from the sector (which would be internal to the sector)
    c2->setLabelPos(-25);

    // merge the inner ring into the outer ring at (50, 90)
    c->makeChart()->merge(c2->makeChart(), 50, 90, Chart::TopLeft, 0);

    // Output the chart
    c->makeChart();

    //free up resources
    delete c2;

    return c;
}


BaseChart *pieshading(int chartIndex, const char **imageMap)
{
    // The data for the pie chart
    double data[] = {18, 30, 20, 15};

    // The labels for the pie chart
    const char *labels[] = {"Labor", "Licenses", "Facilities", "Production"};

    // The colors to use for the sectors
    int colors[] = {0x66aaee, 0xeebb22, 0xbbbbbb, 0x8844ff};

    // Create a PieChart object of size 200 x 220 pixels. Use a vertical gradient color from blue
    // (0000cc) to deep blue (000044) as background. Use rounded corners of 16 pixels radius.
    PieChart *c = new PieChart(200, 220);
    c->setBackground(c->linearGradientColor(0, 0, 0, c->getHeight(), 0x0000cc, 0x000044));
    c->setRoundedFrame(0xffffff, 16);

    // Set the center of the pie at (100, 120) and the radius to 80 pixels
    c->setPieSize(100, 120, 80);

    // Set the pie data
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Set the sector colors
    c->setColors(Chart::DataColor, IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));

    // Demonstrates various shading modes
    if (chartIndex == 0) {
        c->addTitle("Default Shading", "bold", 12, 0xffffff);
    } else if (chartIndex == 1) {
        c->addTitle("Local Gradient", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::LocalGradientShading);
    } else if (chartIndex == 2) {
        c->addTitle("Global Gradient", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::GlobalGradientShading);
    } else if (chartIndex == 3) {
        c->addTitle("Concave Shading", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::ConcaveShading);
    } else if (chartIndex == 4) {
        c->addTitle("Rounded Edge", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::RoundedEdgeShading);
    } else if (chartIndex == 5) {
        c->addTitle("Radial Gradient", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::RadialShading);
    }

    // Disable the sector labels by setting the color to Transparent
    c->setLabelStyle("", 8, Chart::Transparent);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US${value}K ({percent}%)'");

    return c;
}


BaseChart *threedpieshading(int chartIndex, const char **imageMap)
{
    // The data for the pie chart
    double data[] = {18, 30, 20, 15};

    // The labels for the pie chart
    const char *labels[] = {"Labor", "Licenses", "Facilities", "Production"};

    // The colors to use for the sectors
    int colors[] = {0x66aaee, 0xeebb22, 0xbbbbbb, 0x8844ff};

    // Create a PieChart object of size 200 x 200 pixels. Use a vertical gradient color from blue
    // (0000cc) to deep blue (000044) as background. Use rounded corners of 16 pixels radius.
    PieChart *c = new PieChart(200, 200);
    c->setBackground(c->linearGradientColor(0, 0, 0, c->getHeight(), 0x0000cc, 0x000044));
    c->setRoundedFrame(0xffffff, 16);

    // Set the center of the pie at (100, 100) and the radius to 80 pixels
    c->setPieSize(100, 100, 80);

    // Set the pie data
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Set the sector colors
    c->setColors(Chart::DataColor, IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));

    // Draw the pie in 3D with a pie thickness of 20 pixels
    c->set3D(20);

    // Demonstrates various shading modes
    if (chartIndex == 0) {
        c->addTitle("Default Shading", "bold", 12, 0xffffff);
    } else if (chartIndex == 1) {
        c->addTitle("Flat Gradient", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::FlatShading);
    } else if (chartIndex == 2) {
        c->addTitle("Local Gradient", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::LocalGradientShading);
    } else if (chartIndex == 3) {
        c->addTitle("Global Gradient", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::GlobalGradientShading);
    } else if (chartIndex == 4) {
        c->addTitle("Concave Shading", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::ConcaveShading);
    } else if (chartIndex == 5) {
        c->addTitle("Rounded Edge", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::RoundedEdgeShading);
    } else if (chartIndex == 6) {
        c->addTitle("Radial Gradient", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::RadialShading);
    }

    // Disable the sector labels by setting the color to Transparent
    c->setLabelStyle("", 8, Chart::Transparent);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US${value}K ({percent}%)'");

    return c;
}


BaseChart *donutshading(int chartIndex, const char **imageMap)
{
    // The data for the pie chart
    double data[] = {18, 30, 20, 15};

    // The labels for the pie chart
    const char *labels[] = {"Labor", "Licenses", "Facilities", "Production"};

    // The colors to use for the sectors
    int colors[] = {0x66aaee, 0xeebb22, 0xbbbbbb, 0x8844ff};

    // Create a PieChart object of size 200 x 220 pixels. Use a vertical gradient color from blue
    // (0000cc) to deep blue (000044) as background. Use rounded corners of 16 pixels radius.
    PieChart *c = new PieChart(200, 220);
    c->setBackground(c->linearGradientColor(0, 0, 0, c->getHeight(), 0x0000cc, 0x000044));
    c->setRoundedFrame(0xffffff, 16);

    // Set donut center at (100, 120), and outer/inner radii as 80/40 pixels
    c->setDonutSize(100, 120, 80, 40);

    // Set the pie data
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Set the sector colors
    c->setColors(Chart::DataColor, IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));

    // Demonstrates various shading modes
    if (chartIndex == 0) {
        c->addTitle("Default Shading", "bold", 12, 0xffffff);
    } else if (chartIndex == 1) {
        c->addTitle("Local Gradient", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::LocalGradientShading);
    } else if (chartIndex == 2) {
        c->addTitle("Global Gradient", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::GlobalGradientShading);
    } else if (chartIndex == 3) {
        c->addTitle("Concave Shading", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::ConcaveShading);
    } else if (chartIndex == 4) {
        c->addTitle("Rounded Edge", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::RoundedEdgeShading);
    } else if (chartIndex == 5) {
        c->addTitle("Radial Gradient", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::RadialShading);
    } else if (chartIndex == 6) {
        c->addTitle("Ring Shading", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::RingShading);
    }

    // Disable the sector labels by setting the color to Transparent
    c->setLabelStyle("", 8, Chart::Transparent);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US${value}K ({percent}%)'");

    return c;
}


BaseChart *threeddonutshading(int chartIndex, const char **imageMap)
{
    // The data for the pie chart
    double data[] = {18, 30, 20, 15};

    // The labels for the pie chart
    const char *labels[] = {"Labor", "Licenses", "Facilities", "Production"};

    // The colors to use for the sectors
    int colors[] = {0x66aaee, 0xeebb22, 0xbbbbbb, 0x8844ff};

    // Create a PieChart object of size 200 x 200 pixels. Use a vertical gradient color from blue
    // (0000cc) to deep blue (000044) as background. Use rounded corners of 16 pixels radius.
    PieChart *c = new PieChart(200, 200);
    c->setBackground(c->linearGradientColor(0, 0, 0, c->getHeight(), 0x0000cc, 0x000044));
    c->setRoundedFrame(0xffffff, 16);

    // Set donut center at (100, 100), and outer/inner radii as 80/40 pixels
    c->setDonutSize(100, 100, 80, 40);

    // Set the pie data
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Set the sector colors
    c->setColors(Chart::DataColor, IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));

    // Draw the pie in 3D with a pie thickness of 20 pixels
    c->set3D(20);

    // Demonstrates various shading modes
    if (chartIndex == 0) {
        c->addTitle("Default Shading", "bold", 12, 0xffffff);
    } else if (chartIndex == 1) {
        c->addTitle("Flat Gradient", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::FlatShading);
    } else if (chartIndex == 2) {
        c->addTitle("Local Gradient", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::LocalGradientShading);
    } else if (chartIndex == 3) {
        c->addTitle("Global Gradient", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::GlobalGradientShading);
    } else if (chartIndex == 4) {
        c->addTitle("Concave Shading", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::ConcaveShading);
    } else if (chartIndex == 5) {
        c->addTitle("Rounded Edge", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::RoundedEdgeShading);
    } else if (chartIndex == 6) {
        c->addTitle("Radial Gradient", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::RadialShading);
    } else if (chartIndex == 7) {
        c->addTitle("Ring Shading", "bold", 12, 0xffffff);
        c->setSectorStyle(Chart::RingShading);
    }

    // Disable the sector labels by setting the color to Transparent
    c->setLabelStyle("", 8, Chart::Transparent);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US${value}K ({percent}%)'");

    return c;
}


BaseChart *fontpie(int /* chartIndex */, const char **imageMap)
{
    // The data for the pie chart
    double data[] = {25, 18, 15, 12, 8, 30, 35};

    // The labels for the pie chart
    const char *labels[] = {"Labor", "Licenses", "Taxes", "Legal", "Insurance", "Facilities",
        "Production"};

    // Create a PieChart object of size 480 x 300 pixels
    PieChart *c = new PieChart(480, 300);

    // Use a blue marble pattern as the background wallpaper, with a black border, and 1 pixel 3D
    // border effect
    c->setBackground(c->patternColor("marble.png"), 0x000000, 1);

    // Set the center of the pie at (150, 150) and the radius to 100 pixels
    c->setPieSize(150, 150, 100);

    // Add a title to the pie chart using Times Bold Italic/15 points/deep blue (0x000080) as font,
    // with a wood pattern as the title background
    c->addTitle("Project Cost Breakdown", "timesbi.ttf", 15, 0x000080)->setBackground(
        c->patternColor("wood.png"));

    // Draw the pie in 3D
    c->set3D();

    // Add a legend box using Arial Bold Italic/11 points font. Use a pink marble pattern as the
    // background wallpaper, with a 1 pixel 3D border. The legend box is top-right aligned relative
    // to the point (465, 70)
    LegendBox *b = c->addLegend(465, 70, true, "arialbi.ttf", 11);
    b->setBackground(c->patternColor("marble2.png"), Chart::Transparent, 1);
    b->setAlignment(Chart::TopRight);

    // Set the default font for all sector labels to Arial Bold/8pt/dark green (0x008000).
    c->setLabelStyle("arialbd.ttf", 8, 0x008000);

    // Set the pie data and the pie labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Explode the 3rd sector
    c->setExplode(2, 40);

    // Use Arial Bold/12pt/red as label font for the 3rd sector
    c->sector(2)->setLabelStyle("arialbd.ttf", 12, 0xff0000);

    // Use Arial/8pt/deep blue as label font for the 5th sector. Add a background box using the
    // sector fill color (SameAsMainColor), with a black (0x000000) edge and 2 pixel 3D border.
    c->sector(4)->setLabelStyle("arial.ttf", 8, 0x000080)->setBackground(Chart::SameAsMainColor,
        0x000000, 2);

    // Use Arial Italic/8pt/light red (0xff9999) as label font for the 6th sector. Add a dark blue
    // (0x000080) background box with a 2 pixel 3D border.
    c->sector(0)->setLabelStyle("ariali.ttf", 8, 0xff9999)->setBackground(0x000080,
        Chart::Transparent, 2);

    // Use Times Bold Italic/8pt/deep green (0x008000) as label font for 7th sector. Add a yellow
    // (0xFFFF00) background box with a black (0x000000) edge.
    c->sector(6)->setLabelStyle("timesbi.ttf", 8, 0x008000)->setBackground(0xffff00, 0x000000);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US${value}K ({percent}%)'");

    return c;
}


BaseChart *threedanglepie(int chartIndex, const char **imageMap)
{
    char buffer[256];

    // the tilt angle of the pie
    int angle = chartIndex * 15;

    // The data for the pie chart
    double data[] = {25, 18, 15, 12, 8, 30, 35};

    // The labels for the pie chart
    const char *labels[] = {"Labor", "Licenses", "Taxes", "Legal", "Insurance", "Facilities",
        "Production"};

    // Create a PieChart object of size 100 x 110 pixels
    PieChart *c = new PieChart(100, 110);

    // Set the center of the pie at (50, 55) and the radius to 38 pixels
    c->setPieSize(50, 55, 38);

    // Set the depth and tilt angle of the 3D pie (-1 means auto depth)
    c->set3D(-1, angle);

    // Add a title showing the tilt angle
    sprintf(buffer, "Tilt = %d deg", angle);
    c->addTitle(buffer, "arial.ttf", 8);

    // Set the pie data
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Disable the sector labels by setting the color to Transparent
    c->setLabelStyle("", 8, Chart::Transparent);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US${value}K ({percent}%)'");

    return c;
}


BaseChart *threeddepthpie(int chartIndex, const char **imageMap)
{
    char buffer[256];

    // the tilt angle of the pie
    int depth = chartIndex * 5 + 5;

    // The data for the pie chart
    double data[] = {25, 18, 15, 12, 8, 30, 35};

    // The labels for the pie chart
    const char *labels[] = {"Labor", "Licenses", "Taxes", "Legal", "Insurance", "Facilities",
        "Production"};

    // Create a PieChart object of size 100 x 110 pixels
    PieChart *c = new PieChart(100, 110);

    // Set the center of the pie at (50, 55) and the radius to 38 pixels
    c->setPieSize(50, 55, 38);

    // Set the depth of the 3D pie
    c->set3D(depth);

    // Add a title showing the depth
    sprintf(buffer, "Depth = %d pixels", depth);
    c->addTitle(buffer, "arial.ttf", 8);

    // Set the pie data
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Disable the sector labels by setting the color to Transparent
    c->setLabelStyle("", 8, Chart::Transparent);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US${value}K ({percent}%)'");

    return c;
}


BaseChart *shadowpie(int chartIndex, const char **imageMap)
{
    char buffer[256];

    // the tilt angle of the pie
    int angle = chartIndex * 90 + 45;

    // The data for the pie chart
    double data[] = {25, 18, 15, 12, 8, 30, 35};

    // The labels for the pie chart
    const char *labels[] = {"Labor", "Licenses", "Taxes", "Legal", "Insurance", "Facilities",
        "Production"};

    // Create a PieChart object of size 110 x 110 pixels
    PieChart *c = new PieChart(110, 110);

    // Set the center of the pie at (50, 55) and the radius to 36 pixels
    c->setPieSize(55, 55, 36);

    // Set the depth, tilt angle and 3D mode of the 3D pie (-1 means auto depth, "true" means the 3D
    // effect is in shadow mode)
    c->set3D(-1, angle, true);

    // Add a title showing the shadow angle
    sprintf(buffer, "Shadow @ %d deg", angle);
    c->addTitle(buffer, "arial.ttf", 8);

    // Set the pie data
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Disable the sector labels by setting the color to Transparent
    c->setLabelStyle("", 8, Chart::Transparent);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US${value}K ({percent}%)'");

    return c;
}


BaseChart *anglepie(int chartIndex, const char **imageMap)
{
    // The data for the pie chart
    double data[] = {25, 18, 15, 12, 8, 30, 35};

    // The labels for the pie chart
    const char *labels[] = {"Labor", "Licenses", "Taxes", "Legal", "Insurance", "Facilities",
        "Production"};

    // Create a PieChart object of size 280 x 240 pixels
    PieChart *c = new PieChart(280, 240);

    // Set the center of the pie at (140, 130) and the radius to 80 pixels
    c->setPieSize(140, 130, 80);

    // Add a title to the pie to show the start angle and direction
    if (chartIndex == 0) {
        c->addTitle("Start Angle = 0 degrees\nDirection = Clockwise");
    } else {
        c->addTitle("Start Angle = 90 degrees\nDirection = AntiClockwise");
        c->setStartAngle(90, false);
    }

    // Draw the pie in 3D
    c->set3D();

    // Set the pie data and the pie labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Explode the 1st sector (index = 0)
    c->setExplode(0);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US${value}K ({percent}%)'");

    return c;
}


BaseChart *donutwidth(int chartIndex, const char **imageMap)
{
    char buffer[256];

    // Determine the donut inner radius (as percentage of outer radius) based on input parameter
    int donutRadius = chartIndex * 25;

    // The data for the pie chart
    double data[] = {10, 10, 10, 10, 10};

    // The labels for the pie chart
    const char *labels[] = {"Marble", "Wood", "Granite", "Plastic", "Metal"};

    // Create a PieChart object of size 150 x 120 pixels, with a grey (EEEEEE) background, black
    // border and 1 pixel 3D border effect
    PieChart *c = new PieChart(150, 120, 0xeeeeee, 0x000000, 1);

    // Set donut center at (75, 65) and the outer radius to 50 pixels. Inner radius is computed
    // according donutWidth
    c->setDonutSize(75, 60, 50, 50 * donutRadius / 100);

    // Add a title to show the donut width
    sprintf(buffer, "Inner Radius = %d %%", donutRadius);
    c->addTitle(buffer, "arial.ttf", 10)->setBackground(0xcccccc, 0);

    // Draw the pie in 3D
    c->set3D(12);

    // Set the pie data and the pie labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Disable the sector labels by setting the color to Transparent
    c->setLabelStyle("", 8, Chart::Transparent);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: {value}kg ({percent}%)'");

    return c;
}


BaseChart *simplebar(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data[] = {85, 156, 179.5, 211, 123};

    // The labels for the bar chart
    const char *labels[] = {"Mon", "Tue", "Wed", "Thu", "Fri"};

    // Create a XYChart object of size 250 x 250 pixels
    XYChart *c = new XYChart(250, 250);

    // Set the plotarea at (30, 20) and of size 200 x 200 pixels
    c->setPlotArea(30, 20, 200, 200);

    // Add a bar chart layer using the given data
    c->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))));

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{xLabel}: {value} GBytes'");

    return c;
}


BaseChart *simplebar2(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data[] = {85, 156, 179, 211, 123, 189, 166};

    // The labels for the bar chart
    const char *labels[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

    // Create a XYChart object of size 600 x 400 pixels
    XYChart *c = new XYChart(600, 400);

    // Add a title box using grey (0x555555) 24pt Arial Bold font
    c->addTitle("    Bar Chart Demonstration", "arialbd.ttf", 24, 0x555555);

    // Set the plotarea at (70, 60) and of size 500 x 300 pixels, with transparent background and
    // border and light grey (0xcccccc) horizontal grid lines
    c->setPlotArea(70, 60, 500, 300, Chart::Transparent, -1, Chart::Transparent, 0xcccccc);

    // Set the x and y axis stems to transparent and the label font to 12pt Arial
    c->xAxis()->setColors(Chart::Transparent);
    c->yAxis()->setColors(Chart::Transparent);
    c->xAxis()->setLabelStyle("arial.ttf", 12);
    c->yAxis()->setLabelStyle("arial.ttf", 12);

    // Add a blue (0x6699bb) bar chart layer with transparent border using the given data
    c->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), 0x6699bb
        )->setBorderColor(Chart::Transparent);

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // For the automatic y-axis labels, set the minimum spacing to 40 pixels.
    c->yAxis()->setTickDensity(40);

    // Add a title to the y axis using dark grey (0x555555) 14pt Arial Bold font
    c->yAxis()->setTitle("Y-Axis Title Placeholder", "arialbd.ttf", 14, 0x555555);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{xLabel}: {value} kg'");

    return c;
}


BaseChart *barlabel(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data[] = {85, 156, 179, 211, 123, 189, 166};

    // The labels for the bar chart
    const char *labels[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

    // Create a XYChart object of size 600 x 360 pixels
    XYChart *c = new XYChart(600, 360);

    // Set the plotarea at (70, 20) and of size 500 x 300 pixels, with transparent background and
    // border and light grey (0xcccccc) horizontal grid lines
    c->setPlotArea(70, 20, 500, 300, Chart::Transparent, -1, Chart::Transparent, 0xcccccc);

    // Set the x and y axis stems to transparent and the label font to 12pt Arial
    c->xAxis()->setColors(Chart::Transparent);
    c->yAxis()->setColors(Chart::Transparent);
    c->xAxis()->setLabelStyle("arial.ttf", 12);
    c->yAxis()->setLabelStyle("arial.ttf", 12);

    // Add a blue (0x6699bb) bar chart layer using the given data
    BarLayer *layer = c->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))),
        0x6699bb);

    // Use bar gradient lighting with the light intensity from 0.8 to 1.3
    layer->setBorderColor(Chart::Transparent, Chart::barLighting(0.8, 1.3));

    // Set rounded corners for bars
    layer->setRoundedCorners();

    // Display labela on top of bars using 12pt Arial font
    layer->setAggregateLabelStyle("Arial", 12);

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // For the automatic y-axis labels, set the minimum spacing to 40 pixels.
    c->yAxis()->setTickDensity(40);

    // Add a title to the y axis using dark grey (0x555555) 14pt Arial Bold font
    c->yAxis()->setTitle("Y-Axis Title Placeholder", "arialbd.ttf", 14, 0x555555);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{xLabel}: ${value}M'");

    return c;
}


BaseChart *colorbar(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data[] = {85, 156, 179, 211, 123, 189, 166};

    // The labels for the bar chart
    const char *labels[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

    // The colors for the bars
    int colors[] = {0x5588bb, 0x66bbbb, 0xaa6644, 0x99bb55, 0xee9944, 0x444466, 0xbb5555};

    // Create a XYChart object of size 600 x 400 pixels
    XYChart *c = new XYChart(600, 400);

    // Add a title box using grey (0x555555) 24pt Arial font
    c->addTitle("Multi-Color Bar Chart", "arial.ttf", 24, 0x555555);

    // Set the plotarea at (70, 60) and of size 500 x 300 pixels, with transparent background and
    // border and light grey (0xcccccc) horizontal grid lines
    c->setPlotArea(70, 60, 500, 300, Chart::Transparent, -1, Chart::Transparent, 0xcccccc);

    // Set the x and y axis stems to transparent and the label font to 12pt Arial
    c->xAxis()->setColors(Chart::Transparent);
    c->yAxis()->setColors(Chart::Transparent);
    c->xAxis()->setLabelStyle("arial.ttf", 12);
    c->yAxis()->setLabelStyle("arial.ttf", 12);

    // Add a multi-color bar chart layer with transparent border using the given data
    c->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), IntArray(colors, (int)(
        sizeof(colors) / sizeof(colors[0]))))->setBorderColor(Chart::Transparent);

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // For the automatic y-axis labels, set the minimum spacing to 40 pixels.
    c->yAxis()->setTickDensity(40);

    // Add a title to the y axis using dark grey (0x555555) 14pt Arial font
    c->yAxis()->setTitle("Y-Axis Title Placeholder", "arial.ttf", 14, 0x555555);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{xLabel}: ${value}M'");

    return c;
}


BaseChart *colorbar2(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data[] = {85, 156, 179, 211, 123, 189, 166};

    // The labels for the bar chart
    const char *labels[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

    // The colors for the bars
    int colors[] = {0x5588bb, 0x66bbbb, 0xaa6644, 0x99bb55, 0xee9944, 0x444466, 0xbb5555};

    // Create a XYChart object of size 600 x 360 pixels
    XYChart *c = new XYChart(600, 360);

    // Set the plotarea at (70, 20) and of size 500 x 300 pixels, with transparent background and
    // border and light grey (0xcccccc) horizontal grid lines
    c->setPlotArea(70, 20, 500, 300, Chart::Transparent, -1, Chart::Transparent, 0xcccccc);

    // Set the x and y axis stems to transparent and the label font to 12pt Arial
    c->xAxis()->setColors(Chart::Transparent);
    c->yAxis()->setColors(Chart::Transparent);
    c->xAxis()->setLabelStyle("arial.ttf", 12);
    c->yAxis()->setLabelStyle("arial.ttf", 12);

    // Add a multi-color bar chart layer using the given data
    BarLayer *layer = c->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))),
        IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));

    // Use bar gradient lighting with the light intensity from 0.8 to 1.15
    layer->setBorderColor(Chart::Transparent, Chart::barLighting(0.8, 1.15));

    // Set rounded corners for bars
    layer->setRoundedCorners();

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // For the automatic y-axis labels, set the minimum spacing to 40 pixels.
    c->yAxis()->setTickDensity(40);

    // Add a title to the y axis using dark grey (0x555555) 14pt Arial font
    c->yAxis()->setTitle("Y-Axis Title Placeholder", "arial.ttf", 14, 0x555555);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{xLabel}: ${value}M'");

    return c;
}


BaseChart *softlightbar(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data[] = {450, 560, 630, 800, 1100, 1350, 1600, 1950, 2300, 2700};

    // The labels for the bar chart
    const char *labels[] = {"1996", "1997", "1998", "1999", "2000", "2001", "2002", "2003", "2004",
        "2005"};

    // Create a XYChart object of size 600 x 360 pixels
    XYChart *c = new XYChart(600, 360);

    // Add a title to the chart using 18pt Times Bold Italic font
    c->addTitle("Annual Revenue for Star Tech", "timesbi.ttf", 18);

    // Set the plotarea at (60, 40) and of size 500 x 280 pixels. Use a vertical gradient color from
    // light blue (eeeeff) to deep blue (0000cc) as background. Set border and grid lines to white
    // (ffffff).
    c->setPlotArea(60, 40, 500, 280, c->linearGradientColor(60, 40, 60, 280, 0xeeeeff, 0x0000cc),
        -1, 0xffffff, 0xffffff);

    // Add a multi-color bar chart layer using the supplied data. Use soft lighting effect with
    // light direction from left.
    c->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), IntArray(0, 0)
        )->setBorderColor(Chart::Transparent, Chart::softLighting(Chart::Left));

    // Set x axis labels using the given labels
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Draw the ticks between label positions (instead of at label positions)
    c->xAxis()->setTickOffset(0.5);

    // Add a title to the y axis with 10pt Arial Bold font
    c->yAxis()->setTitle("USD (millions)", "arialbd.ttf", 10);

    // Set axis label style to 8pt Arial Bold
    c->xAxis()->setLabelStyle("arialbd.ttf", 8);
    c->yAxis()->setLabelStyle("arialbd.ttf", 8);

    // Set axis line width to 2 pixels
    c->xAxis()->setWidth(2);
    c->yAxis()->setWidth(2);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='Year {xLabel}: US$ {value}M'");

    return c;
}


BaseChart *glasslightbar(int /* chartIndex */, const char **imageMap)
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
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='Year {xLabel}: US$ {value}M'");

    return c;
}


BaseChart *gradientbar(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data[] = {1350, 1600, 1950, 2300, 2700};

    // The labels for the bar chart
    const char *labels[] = {"2001", "2002", "2003", "2004", "2005"};

    // The colors for the bars
    int colors[] = {0xcc0000, 0x66aaee, 0xeebb22, 0xcccccc, 0xcc88ff};

    // Create a PieChart object of size 600 x 380 pixels.
    XYChart *c = new XYChart(600, 380);

    // Use the white on black palette, which means the default text and line colors are white
    c->setColors(Chart::whiteOnBlackPalette);

    // Use a vertical gradient color from blue (0000cc) to deep blue (000044) as background. Use
    // rounded corners of 30 pixels radius for the top-left and bottom-right corners.
    c->setBackground(c->linearGradientColor(0, 0, 0, c->getHeight(), 0x0000cc, 0x000044));
    c->setRoundedFrame(0xffffff, 30, 0, 30, 0);

    // Add a title using 18pt Times New Roman Bold Italic font. Add 6 pixels top and bottom margins
    // to the title.
    TextBox *title = c->addTitle("Annual Revenue for Star Tech", "timesbi.ttf", 18);
    title->setMargin(0, 0, 6, 6);

    // Add a separator line in white color just under the title
    c->addLine(20, title->getHeight(), c->getWidth() - 21, title->getHeight(), 0xffffff);

    // Tentatively set the plotarea at (70, 80) and of 480 x 240 pixels in size. Use transparent
    // border and white grid lines
    c->setPlotArea(70, 80, 480, 240, -1, -1, Chart::Transparent, 0xffffff);

    // Swap the axis so that the bars are drawn horizontally
    c->swapXY();

    // Add a multi-color bar chart layer using the supplied data. Use bar gradient lighting with the
    // light intensity from 0.75 to 2.0
    c->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), IntArray(colors, (int)(
        sizeof(colors) / sizeof(colors[0]))))->setBorderColor(Chart::Transparent,
        Chart::barLighting(0.75, 2.0));

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Show the same scale on the left and right y-axes
    c->syncYAxis();

    // Set the bottom y-axis title using 10pt Arial Bold font
    c->yAxis()->setTitle("USD (millions)", "arialbd.ttf", 10);

    // Set y-axes to transparent
    c->yAxis()->setColors(Chart::Transparent);
    c->yAxis2()->setColors(Chart::Transparent);

    // Disable ticks on the x-axis by setting the tick color to transparent
    c->xAxis()->setTickColor(Chart::Transparent);

    // Set the label styles of all axes to 8pt Arial Bold font
    c->xAxis()->setLabelStyle("arialbd.ttf", 8);
    c->yAxis()->setLabelStyle("arialbd.ttf", 8);
    c->yAxis2()->setLabelStyle("arialbd.ttf", 8);

    // Adjust the plot area size, such that the bounding box (inclusive of axes) is 30 pixels from
    // the left edge, 25 pixels below the title, 50 pixels from the right edge, and 25 pixels from
    // the bottom edge.
    c->packPlotArea(30, title->getHeight() + 25, c->getWidth() - 50, c->getHeight() - 25);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='Year {xLabel}: US$ {value}M'");

    return c;
}


BaseChart *cylinderlightbar(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data[] = {450, 560, 630, 800, 1100, 1350, 1600, 1950, 2300, 2700};

    // The labels for the bar chart
    const char *labels[] = {"1996", "1997", "1998", "1999", "2000", "2001", "2002", "2003", "2004",
        "2005"};

    // Create a XYChart object of size 600 x 380 pixels. Set background color to brushed silver,
    // with a 2 pixel 3D border. Use rounded corners of 20 pixels radius.
    XYChart *c = new XYChart(600, 380, Chart::brushedSilverColor(), Chart::Transparent, 2);

    // Add a title to the chart using 18pt Times Bold Italic font. Set top/bottom margins to 8
    // pixels.
    c->addTitle("Annual Revenue for Star Tech", "timesbi.ttf", 18)->setMargin(0, 0, 8, 8);

    // Set the plotarea at (70, 55) and of size 460 x 280 pixels. Use transparent border and black
    // grid lines. Use rounded frame with radius of 20 pixels.
    c->setPlotArea(70, 55, 460, 280, -1, -1, Chart::Transparent, 0x000000);
    c->setRoundedFrame(0xffffff, 20);

    // Add a multi-color bar chart layer using the supplied data. Set cylinder bar shape.
    c->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), IntArray(0, 0)
        )->setBarShape(Chart::CircleShape);

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Show the same scale on the left and right y-axes
    c->syncYAxis();

    // Set the left y-axis and right y-axis title using 10pt Arial Bold font
    c->yAxis()->setTitle("USD (millions)", "arialbd.ttf", 10);
    c->yAxis2()->setTitle("USD (millions)", "arialbd.ttf", 10);

    // Set y-axes to transparent
    c->yAxis()->setColors(Chart::Transparent);
    c->yAxis2()->setColors(Chart::Transparent);

    // Disable ticks on the x-axis by setting the tick color to transparent
    c->xAxis()->setTickColor(Chart::Transparent);

    // Set the label styles of all axes to 8pt Arial Bold font
    c->xAxis()->setLabelStyle("arialbd.ttf", 8);
    c->yAxis()->setLabelStyle("arialbd.ttf", 8);
    c->yAxis2()->setLabelStyle("arialbd.ttf", 8);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='Year {xLabel}: US$ {value}M'");

    return c;
}


BaseChart *threedbar(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data[] = {85, 156, 179.5, 211, 123};

    // The labels for the bar chart
    const char *labels[] = {"Mon", "Tue", "Wed", "Thu", "Fri"};

    // Create a XYChart object of size 300 x 280 pixels
    XYChart *c = new XYChart(300, 280);

    // Set the plotarea at (45, 30) and of size 200 x 200 pixels
    c->setPlotArea(45, 30, 200, 200);

    // Add a title to the chart
    c->addTitle("Weekly Server Load");

    // Add a title to the y axis
    c->yAxis()->setTitle("MBytes");

    // Add a title to the x axis
    c->xAxis()->setTitle("Work Week 25");

    // Add a bar chart layer with green (0x00ff00) bars using the given data
    c->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), 0x00ff00)->set3D();

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{xLabel}: {value} MBytes'");

    return c;
}


BaseChart *cylinderbar(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data[] = {85, 156, 179.5, 211, 123};

    // The labels for the bar chart
    const char *labels[] = {"Mon", "Tue", "Wed", "Thu", "Fri"};

    // Create a XYChart object of size 400 x 240 pixels.
    XYChart *c = new XYChart(400, 240);

    // Add a title to the chart using 14pt Times Bold Italic font
    c->addTitle("Weekly Server Load", "timesbi.ttf", 14);

    // Set the plotarea at (45, 40) and of 300 x 160 pixels in size. Use alternating light grey
    // (f8f8f8) / white (ffffff) background.
    c->setPlotArea(45, 40, 300, 160, 0xf8f8f8, 0xffffff);

    // Add a multi-color bar chart layer
    BarLayer *layer = c->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))),
        IntArray(0, 0));

    // Set layer to 3D with 10 pixels 3D depth
    layer->set3D(10);

    // Set bar shape to circular (cylinder)
    layer->setBarShape(Chart::CircleShape);

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Add a title to the y axis
    c->yAxis()->setTitle("MBytes");

    // Add a title to the x axis
    c->xAxis()->setTitle("Work Week 25");

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{xLabel}: {value} MBytes'");

    return c;
}


BaseChart *polygonbar(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data[] = {85, 156, 179.5, 211, 123, 176, 195};

    // The labels for the bar chart
    const char *labels[] = {"Square", "Star(8)", "Polygon(6)", "Cross", "Cross2", "Diamond",
        "Custom"};

    // Create a XYChart object of size 500 x 280 pixels.
    XYChart *c = new XYChart(500, 280);

    // Set the plotarea at (50, 40) with alternating light grey (f8f8f8) / white (ffffff) background
    c->setPlotArea(50, 40, 400, 200, 0xf8f8f8, 0xffffff);

    // Add a title to the chart using 14pt Arial Bold Italic font
    c->addTitle("    Bar Shape Demonstration", "arialbi.ttf", 14);

    // Add a multi-color bar chart layer
    BarLayer *layer = c->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))),
        IntArray(0, 0));

    // Set layer to 3D with 10 pixels 3D depth
    layer->set3D(10);

    // Set bar shape to circular (cylinder)
    layer->setBarShape(Chart::CircleShape);

    // Set the first bar (index = 0) to square shape
    layer->setBarShape(Chart::SquareShape, 0, 0);

    // Set the second bar to 8-pointed star
    layer->setBarShape(Chart::StarShape(8), 0, 1);

    // Set the third bar to 6-sided polygon
    layer->setBarShape(Chart::PolygonShape(6), 0, 2);

    // Set the next 3 bars to cross shape, X shape and diamond shape
    layer->setBarShape(Chart::CrossShape(), 0, 3);
    layer->setBarShape(Chart::Cross2Shape(), 0, 4);
    layer->setBarShape(Chart::DiamondShape, 0, 5);

    // Set the last bar to a custom shape, specified as an array of (x, y) points in normalized
    // coordinates
    int pattern1[] = {-500, 0, 0, 500, 500, 0, 500, 1000, 0, 500, -500, 1000};
    layer->setBarShape(IntArray(pattern1, (int)(sizeof(pattern1) / sizeof(pattern1[0]))), 0, 6);

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Add a title to the y axis
    c->yAxis()->setTitle("Frequency");

    // Add a title to the x axis
    c->xAxis()->setTitle("Shapes");

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{xLabel}: {value}'");

    return c;
}


BaseChart *stackedbar(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data0[] = {100, 115, 165, 107, 67};
    double data1[] = {85, 106, 129, 161, 123};
    double data2[] = {67, 87, 86, 167, 157};

    // The labels for the bar chart
    const char *labels[] = {"Mon", "Tue", "Wed", "Thu", "Fri"};

    // Create a XYChart object of size 600 x 360 pixels
    XYChart *c = new XYChart(600, 360);

    // Set the plotarea at (70, 20) and of size 400 x 300 pixels, with transparent background and
    // border and light grey (0xcccccc) horizontal grid lines
    c->setPlotArea(70, 20, 400, 300, Chart::Transparent, -1, Chart::Transparent, 0xcccccc);

    // Add a legend box at (480, 20) using vertical layout and 12pt Arial font. Set background and
    // border to transparent and key icon border to the same as the fill color.
    LegendBox *b = c->addLegend(480, 20, true, "arial.ttf", 12);
    b->setBackground(Chart::Transparent, Chart::Transparent);
    b->setKeyBorder(Chart::SameAsMainColor);

    // Set the x and y axis stems to transparent and the label font to 12pt Arial
    c->xAxis()->setColors(Chart::Transparent);
    c->yAxis()->setColors(Chart::Transparent);
    c->xAxis()->setLabelStyle("arial.ttf", 12);
    c->yAxis()->setLabelStyle("arial.ttf", 12);

    // Add a stacked bar layer
    BarLayer *layer = c->addBarLayer(Chart::Stack);

    // Add the three data sets to the bar layer
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0xaaccee,
        "Server # 1");
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0xbbdd88,
        "Server # 2");
    layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), 0xeeaa66,
        "Server # 3");

    // Set the bar border to transparent
    layer->setBorderColor(Chart::Transparent);

    // Enable labelling for the entire bar and use 12pt Arial font
    layer->setAggregateLabelStyle("arial.ttf", 12);

    // Enable labelling for the bar segments and use 12pt Arial font with center alignment
    layer->setDataLabelStyle("arial.ttf", 10)->setAlignment(Chart::Center);

    // For a vertical stacked bar with positive data, the first data set is at the bottom. For the
    // legend box, by default, the first entry at the top. We can reverse the legend order to make
    // the legend box consistent with the stacked bar.
    layer->setLegendOrder(Chart::ReverseLegend);

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // For the automatic y-axis labels, set the minimum spacing to 40 pixels.
    c->yAxis()->setTickDensity(40);

    // Add a title to the y axis using dark grey (0x555555) 14pt Arial Bold font
    c->yAxis()->setTitle("Y-Axis Title Placeholder", "arialbd.ttf", 14, 0x555555);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName} on {xLabel}: {value} MBytes/hour'");

    return c;
}


BaseChart *percentbar(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data0[] = {100, 125, 245, 147, 67};
    double data1[] = {85, 156, 179, 211, 123};
    double data2[] = {97, 87, 56, 267, 157};

    // The labels for the bar chart
    const char *labels[] = {"Mon", "Tue", "Wed", "Thu", "Fri"};

    // Create a XYChart object of size 500 x 320 pixels. Use a vertical gradient color from pale
    // blue (e8f0f8) to sky blue (aaccff) spanning half the chart height as background. Set border
    // to blue (88aaee). Use rounded corners. Enable soft drop shadow.
    XYChart *c = new XYChart(500, 320);
    c->setBackground(c->linearGradientColor(0, 0, 0, c->getHeight() / 2, 0xe8f0f8, 0xaaccff),
        0x88aaee);
    c->setRoundedFrame();
    c->setDropShadow();

    // Add a title to the chart using 15 points Arial Italic. Set top/bottom margins to 15 pixels.
    TextBox *title = c->addTitle("Weekly Product Sales", "ariali.ttf", 15);
    title->setMargin(0, 0, 15, 15);

    // Tentatively set the plotarea to 50 pixels from the left edge, and to just under the title.
    // Set the width to 60% of the chart width, and the height to 50 pixels from the bottom edge.
    // Use pale blue (e8f0f8) background, transparent border, and grey (aaaaaa) grid lines.
    c->setPlotArea(50, title->getHeight(), c->getWidth() * 6 / 10, c->getHeight() -
        title->getHeight() - 50, 0xe8f0f8, -1, Chart::Transparent, 0xaaaaaa);

    // Add a legend box where the top-right corner is anchored at 10 pixels from the right edge, and
    // just under the title. Use vertical layout and 8 points Arial font.
    LegendBox *legendBox = c->addLegend(c->getWidth() - 10, title->getHeight(), true, "arial.ttf", 8
        );
    legendBox->setAlignment(Chart::TopRight);

    // Set the legend box background and border to transparent
    legendBox->setBackground(Chart::Transparent, Chart::Transparent);

    // Set the legend box icon size to 16 x 32 pixels to match with custom icon size
    legendBox->setKeySize(16, 32);

    // Set axes to transparent
    c->xAxis()->setColors(Chart::Transparent);
    c->yAxis()->setColors(Chart::Transparent);

    // Set the labels on the x axis
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Add a percentage bar layer
    BarLayer *layer = c->addBarLayer(Chart::Percentage);

    // Add the three data sets to the bar layer, using icons images with labels as data set names
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0x66aaee,
        "<*block,valign=absmiddle*><*img=service.png*> Service<*/*>");
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0xeebb22,
        "<*block,valign=absmiddle*><*img=software.png*> Software<*/*>");
    layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), 0xcc88ff,
        "<*block,valign=absmiddle*><*img=computer.png*> Hardware<*/*>");

    // Use soft lighting effect with light direction from top
    layer->setBorderColor(Chart::Transparent, Chart::softLighting(Chart::Top));

    // Enable data label at the middle of the the bar
    layer->setDataLabelStyle()->setAlignment(Chart::Center);

    // For a vertical stacked chart with positive data only, the last data set is always on top.
    // However, in a vertical legend box, the last data set is at the bottom. This can be reversed
    // by using the setLegend method.
    layer->setLegend(Chart::ReverseLegend);

    // Adjust the plot area size, such that the bounding box (inclusive of axes) is 15 pixels from
    // the left edge, just below the title, 10 pixels to the right of the legend box, and 15 pixels
    // from the bottom edge.
    c->packPlotArea(15, title->getHeight(), c->layoutLegend()->getLeftX() - 10, c->getHeight() - 15)
        ;

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName} revenue on {xLabel}: US${value}K ({percent}%)'");

    return c;
}


BaseChart *multibar(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data0[] = {100, 125, 245, 147, 67};
    double data1[] = {85, 156, 179, 211, 123};
    double data2[] = {97, 87, 56, 267, 157};
    const char *labels[] = {"Mon", "Tue", "Wed", "Thu", "Fri"};

    // Create a XYChart object of size 400 x 240 pixels
    XYChart *c = new XYChart(400, 240);

    // Add a title to the chart using 10 pt Arial font
    c->addTitle("         Average Weekday Network Load", "", 10);

    // Set the plot area at (50, 25) and of size 320 x 180. Use two alternative background colors
    // (0xffffc0 and 0xffffe0)
    c->setPlotArea(50, 25, 320, 180, 0xffffc0, 0xffffe0);

    // Add a legend box at (55, 18) using horizontal layout. Use 8 pt Arial font, with transparent
    // background
    c->addLegend(55, 18, false, "", 8)->setBackground(Chart::Transparent);

    // Add a title to the y-axis
    c->yAxis()->setTitle("Throughput (MBytes Per Hour)");

    // Reserve 20 pixels at the top of the y-axis for the legend box
    c->yAxis()->setTopMargin(20);

    // Set the x axis labels
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Add a multi-bar layer with 3 data sets and 3 pixels 3D depth
    BarLayer *layer = c->addBarLayer(Chart::Side, 3);
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0xff8080,
        "Server #1");
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x80ff80,
        "Server #2");
    layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), 0x8080ff,
        "Server #3");

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName} on {xLabel}: {value} MBytes/hour'");

    return c;
}


BaseChart *softmultibar(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data0[] = {100, 125, 245, 147, 67};
    double data1[] = {85, 156, 179, 211, 123};
    double data2[] = {97, 87, 56, 267, 157};
    const char *labels[] = {"Mon", "Tue", "Wed", "Thur", "Fri"};

    // Create a XYChart object of size 540 x 375 pixels
    XYChart *c = new XYChart(540, 375);

    // Add a title to the chart using 18pt Times Bold Italic font
    c->addTitle("Average Weekly Network Load", "timesbi.ttf", 18);

    // Set the plotarea at (50, 55) and of 440 x 280 pixels in size. Use a vertical gradient color
    // from light red (ffdddd) to dark red (880000) as background. Set border and grid lines to
    // white (ffffff).
    c->setPlotArea(50, 55, 440, 280, c->linearGradientColor(0, 55, 0, 335, 0xffdddd, 0x880000), -1,
        0xffffff, 0xffffff);

    // Add a legend box at (50, 25) using horizontal layout. Use 10pt Arial Bold as font, with
    // transparent background.
    c->addLegend(50, 25, false, "arialbd.ttf", 10)->setBackground(Chart::Transparent);

    // Set the x axis labels
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Draw the ticks between label positions (instead of at label positions)
    c->xAxis()->setTickOffset(0.5);

    // Set axis label style to 8pt Arial Bold
    c->xAxis()->setLabelStyle("arialbd.ttf", 8);
    c->yAxis()->setLabelStyle("arialbd.ttf", 8);

    // Set axis line width to 2 pixels
    c->xAxis()->setWidth(2);
    c->yAxis()->setWidth(2);

    // Add axis title
    c->yAxis()->setTitle("Throughput (MBytes Per Hour)");

    // Add a multi-bar layer with 3 data sets and 4 pixels 3D depth
    BarLayer *layer = c->addBarLayer(Chart::Side, 4);
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0xffff00,
        "Server #1");
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x00ff00,
        "Server #2");
    layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), 0x9999ff,
        "Server #3");

    // Set bar border to transparent. Use soft lighting effect with light direction from top.
    layer->setBorderColor(Chart::Transparent, Chart::softLighting(Chart::Top));

    // Configure the bars within a group to touch each others (no gap)
    layer->setBarGap(0.2, Chart::TouchBar);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName} on {xLabel}: {value} MBytes/hour'");

    return c;
}


BaseChart *glassmultibar(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data0[] = {100, 125, 245, 147, 67};
    double data1[] = {85, 156, 179, 211, 123};
    double data2[] = {97, 87, 56, 267, 157};
    const char *labels[] = {"Mon", "Tue", "Wed", "Thur", "Fri"};

    // Create a XYChart object of size 540 x 375 pixels
    XYChart *c = new XYChart(540, 375);

    // Add a title to the chart using 18pt Times Bold Italic font
    c->addTitle("Average Weekly Network Load", "timesbi.ttf", 18);

    // Set the plotarea at (50, 55) and of 440 x 280 pixels in size. Use a vertical gradient color
    // from light blue (f9f9ff) to blue (6666ff) as background. Set border and grid lines to white
    // (ffffff).
    c->setPlotArea(50, 55, 440, 280, c->linearGradientColor(0, 55, 0, 335, 0xf9f9ff, 0x6666ff), -1,
        0xffffff, 0xffffff);

    // Add a legend box at (50, 28) using horizontal layout. Use 10pt Arial Bold as font, with
    // transparent background.
    c->addLegend(50, 28, false, "arialbd.ttf", 10)->setBackground(Chart::Transparent);

    // Set the x axis labels
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Draw the ticks between label positions (instead of at label positions)
    c->xAxis()->setTickOffset(0.5);

    // Set axis label style to 8pt Arial Bold
    c->xAxis()->setLabelStyle("arialbd.ttf", 8);
    c->yAxis()->setLabelStyle("arialbd.ttf", 8);

    // Set axis line width to 2 pixels
    c->xAxis()->setWidth(2);
    c->yAxis()->setWidth(2);

    // Add axis title
    c->yAxis()->setTitle("Throughput (MBytes Per Hour)");

    // Add a multi-bar layer with 3 data sets
    BarLayer *layer = c->addBarLayer(Chart::Side);
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0xff0000,
        "Server #1");
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x00ff00,
        "Server #2");
    layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), 0xff8800,
        "Server #3");

    // Set bar border to transparent. Use glass lighting effect with light direction from left.
    layer->setBorderColor(Chart::Transparent, Chart::glassEffect(Chart::NormalGlare, Chart::Left));

    // Configure the bars within a group to touch each others (no gap)
    layer->setBarGap(0.2, Chart::TouchBar);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName} on {xLabel}: {value} MBytes/hour'");

    return c;
}


BaseChart *gradientmultibar(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data0[] = {100, 125, 245, 147, 67};
    double data1[] = {85, 156, 179, 211, 123};
    double data2[] = {97, 87, 56, 267, 157};
    const char *labels[] = {"Mon", "Tue", "Wed", "Thur", "Fri"};

    // Create a XYChart object of size 540 x 375 pixels
    XYChart *c = new XYChart(540, 375);

    // Add a title to the chart using 18pt Times Bold Italic font
    c->addTitle("Average Weekly Network Load", "timesbi.ttf", 18);

    // Set the plotarea at (50, 55) and of 440 x 280 pixels in size. Use a vertical gradient color
    // from grey (888888) to black (000000) as background. Set border and grid lines to white
    // (ffffff).
    c->setPlotArea(50, 55, 440, 280, c->linearGradientColor(0, 55, 0, 335, 0x888888, 0x000000), -1,
        0xffffff, 0xffffff);

    // Add a legend box at (50, 25) using horizontal layout. Use 10pt Arial Bold as font, with
    // transparent background.
    c->addLegend(50, 25, false, "arialbd.ttf", 10)->setBackground(Chart::Transparent);

    // Set the x axis labels
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Draw the ticks between label positions (instead of at label positions)
    c->xAxis()->setTickOffset(0.5);

    // Set axis label style to 8pt Arial Bold
    c->xAxis()->setLabelStyle("arialbd.ttf", 8);
    c->yAxis()->setLabelStyle("arialbd.ttf", 8);

    // Set axis line width to 2 pixels
    c->xAxis()->setWidth(2);
    c->yAxis()->setWidth(2);

    // Add axis title
    c->yAxis()->setTitle("Throughput (MBytes Per Hour)");

    // Add a multi-bar layer with 3 data sets and 4 pixels 3D depth
    BarLayer *layer = c->addBarLayer(Chart::Side, 4);
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0x66aaee,
        "Server #1");
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0xeebb22,
        "Server #2");
    layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), 0xcc0000,
        "Server #3");

    // Set bar border to transparent. Use bar gradient lighting with light intensity from 0.75 to
    // 1.75.
    layer->setBorderColor(Chart::Transparent, Chart::barLighting(0.75, 1.75));

    // Configure the bars within a group to touch each others (no gap)
    layer->setBarGap(0.2, Chart::TouchBar);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName} on {xLabel}: {value} MBytes/hour'");

    return c;
}


BaseChart *multicylinder(int /* chartIndex */, const char **imageMap)
{
    // Data for the chart
    double data0[] = {100, 125, 245, 147, 67, 96, 160, 145, 97, 167, 220, 125};
    double data1[] = {85, 156, 179, 211, 123, 225, 127, 99, 111, 260, 175, 156};
    double data2[] = {97, 87, 56, 267, 157, 157, 67, 156, 77, 87, 197, 87};
    const char *labels[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct",
        "Nov", "Dec"};

    // Create a XYChart object of size 560 x 280 pixels.
    XYChart *c = new XYChart(560, 280);

    // Add a title to the chart using 14pt Arial Bold Italic font
    c->addTitle("     Average Weekly Network Load", "arialbi.ttf", 14);

    // Set the plotarea at (50, 50) and of 500 x 200 pixels in size. Use alternating light grey
    // (f8f8f8) / white (ffffff) background. Set border to transparent and use grey (CCCCCC) dotted
    // lines as horizontal and vertical grid lines
    c->setPlotArea(50, 50, 500, 200, 0xffffff, 0xf8f8f8, Chart::Transparent, c->dashLineColor(
        0xcccccc, Chart::DotLine), c->dashLineColor(0xcccccc, Chart::DotLine));

    // Add a legend box at (50, 22) using horizontal layout. Use 10 pt Arial Bold Italic font, with
    // transparent background
    c->addLegend(50, 22, false, "arialbi.ttf", 10)->setBackground(Chart::Transparent);

    // Set the x axis labels
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Draw the ticks between label positions (instead of at label positions)
    c->xAxis()->setTickOffset(0.5);

    // Add axis title
    c->yAxis()->setTitle("Throughput (MBytes Per Hour)");

    // Set axis line width to 2 pixels
    c->xAxis()->setWidth(2);
    c->yAxis()->setWidth(2);

    // Add a multi-bar layer with 3 data sets
    BarLayer *layer = c->addBarLayer(Chart::Side);
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0xff0000,
        "Server #1");
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x00ff00,
        "Server #2");
    layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), 0x0000ff,
        "Server #3");

    // Set bar shape to circular (cylinder)
    layer->setBarShape(Chart::CircleShape);

    // Configure the bars within a group to touch each others (no gap)
    layer->setBarGap(0.2, Chart::TouchBar);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName} on {xLabel}: {value} MBytes/hour'");

    return c;
}


BaseChart *multishapebar(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data0[] = {100, 125, 245, 147};
    double data1[] = {85, 156, 179, 211};
    double data2[] = {97, 87, 56, 267};
    const char *labels[] = {"1st Quarter", "2nd Quarter", "3rd Quarter", "4th Quarter"};

    // Create a XYChart object of size 600 x 350 pixels
    XYChart *c = new XYChart(600, 350);

    // Add a title to the chart using 14pt Arial Bold Italic font
    c->addTitle("Annual Product Revenue", "arialbi.ttf", 14);

    // Set the plot area at (50, 60) and of size 500 x 240. Use two alternative background colors
    // (f8f8f8 and ffffff)
    c->setPlotArea(50, 60, 500, 240, 0xf8f8f8, 0xffffff);

    // Add a legend box at (55, 22) using horizontal layout, with transparent background
    c->addLegend(55, 22, false)->setBackground(Chart::Transparent);

    // Set the x axis labels
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Draw the ticks between label positions (instead of at label positions)
    c->xAxis()->setTickOffset(0.5);

    // Add a multi-bar layer with 3 data sets and 9 pixels 3D depth
    BarLayer *layer = c->addBarLayer(Chart::Side, 9);
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), -1, "Product A");
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), -1, "Product B");
    layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), -1, "Product C");

    // Set data set 1 to use a bar shape of a 6-pointed star
    layer->setBarShape(Chart::StarShape(6), 0);

    // Set data set 2 to use a bar shapre of a 6-sided polygon
    layer->setBarShape(Chart::PolygonShape(6), 1);

    // Set data set 3 to use an X bar shape
    layer->setBarShape(Chart::Cross2Shape(), 2);

    // Add a title to the y-axis
    c->yAxis()->setTitle("Revenue (USD in millions)");

    // Add a title to the x axis
    c->xAxis()->setTitle("Year 2005");

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName}: {xLabel} Revenue = {value} millions'");

    return c;
}


BaseChart *overlapbar(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data0[] = {100, 125, 156, 147, 87, 124, 178, 109, 140, 106, 192, 122};
    double data1[] = {122, 156, 179, 211, 198, 177, 160, 220, 190, 188, 220, 270};
    double data2[] = {167, 190, 213, 267, 250, 320, 212, 199, 245, 267, 240, 310};
    const char *labels[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct",
        "Nov", "Dec"};

    // Create a XYChart object of size 580 x 280 pixels
    XYChart *c = new XYChart(580, 280);

    // Add a title to the chart using 14pt Arial Bold Italic font
    c->addTitle("Product Revenue For Last 3 Years", "arialbi.ttf", 14);

    // Set the plot area at (50, 50) and of size 500 x 200. Use two alternative background colors
    // (f8f8f8 and ffffff)
    c->setPlotArea(50, 50, 500, 200, 0xf8f8f8, 0xffffff);

    // Add a legend box at (50, 25) using horizontal layout. Use 8pt Arial as font, with transparent
    // background.
    c->addLegend(50, 25, false, "arial.ttf", 8)->setBackground(Chart::Transparent);

    // Set the x axis labels
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Draw the ticks between label positions (instead of at label positions)
    c->xAxis()->setTickOffset(0.5);

    // Add a multi-bar layer with 3 data sets
    BarLayer *layer = c->addBarLayer(Chart::Side);
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0xff8080,
        "Year 2003");
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x80ff80,
        "Year 2004");
    layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), 0x8080ff,
        "Year 2005");

    // Set 50% overlap between bars
    layer->setOverlapRatio(0.5);

    // Add a title to the y-axis
    c->yAxis()->setTitle("Revenue (USD in millions)");

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{xLabel} Revenue on {dataSetName}: {value} millions'");

    return c;
}


BaseChart *multistackbar(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data0[] = {44, 55, 100};
    double data1[] = {97, 87, 167};
    double data2[] = {156, 78, 147};
    double data3[] = {125, 118, 211};

    // The labels for the bar chart. The labels contains embedded images as icons.
    const char *labels[] = {"<*img=service.png*><*br*>Service",
        "<*img=software.png*><*br*>Software", "<*img=computer.png*><*br*>Hardware"};

    // Create a XYChart object of size 600 x 350 pixels, using 0xe0e0ff as the background color,
    // 0xccccff as the border color, with 1 pixel 3D border effect.
    XYChart *c = new XYChart(600, 350, 0xe0e0ff, 0xccccff, 1);

    // Add a title to the chart using 14 points Times Bold Itatic font and light blue (0x9999ff) as
    // the background color
    c->addTitle("Business Results 2001 vs 2002", "timesbi.ttf", 14)->setBackground(0x9999ff);

    // Set the plotarea at (60, 45) and of size 500 x 210 pixels, using white (0xffffff) as the
    // background
    c->setPlotArea(60, 45, 500, 210, 0xffffff);

    // Swap the x and y axes to create a horizontal bar chart
    c->swapXY();

    // Add a title to the y axis using 11 pt Times Bold Italic as font
    c->yAxis()->setTitle("Revenue (millions)", "timesbi.ttf", 11);

    // Set the labels on the x axis
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Disable x-axis ticks by setting the tick length to 0
    c->xAxis()->setTickLength(0);

    // Add a stacked bar layer to the chart
    BarLayer *layer = c->addBarLayer(Chart::Stack);

    // Add the first two data sets to the chart as a stacked bar group
    layer->addDataGroup("2001");
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0xaaaaff, "Local"
        );
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x6666ff,
        "International");

    // Add the remaining data sets to the chart as another stacked bar group
    layer->addDataGroup("2002");
    layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), 0xffaaaa, "Local"
        );
    layer->addDataSet(DoubleArray(data3, (int)(sizeof(data3) / sizeof(data3[0]))), 0xff6666,
        "International");

    // Set the sub-bar gap to 0, so there is no gap between stacked bars with a group
    layer->setBarGap(0.2, 0);

    // Set the bar border to transparent
    layer->setBorderColor(Chart::Transparent);

    // Set the aggregate label format
    layer->setAggregateLabelFormat("Year {dataGroupName}\n{value} millions");

    // Set the aggregate label font to 8 point Arial Bold Italic
    layer->setAggregateLabelStyle("arialbi.ttf", 8);

    // Reverse 20% space at the right during auto-scaling to allow space for the aggregate bar
    // labels
    c->yAxis()->setAutoScale(0.2);

    // Add a legend box at (310, 300) using TopCenter alignment, with 2 column grid layout, and use
    // 8pt Arial Bold Italic as font
    LegendBox *legendBox = c->addLegend2(310, 300, 2, "arialbi.ttf", 8);
    legendBox->setAlignment(Chart::TopCenter);

    // Set the format of the text displayed in the legend box
    legendBox->setText("Year {dataGroupName} {dataSetName} Revenue");

    // Set the background and border of the legend box to transparent
    legendBox->setBackground(Chart::Transparent, Chart::Transparent);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='Year {dataGroupName} {dataSetName} {xLabel} Revenue: {value} millions'");

    return c;
}


BaseChart *depthbar(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data0[] = {100, 125, 245, 147, 67};
    double data1[] = {85, 156, 179, 211, 123};
    double data2[] = {97, 87, 56, 267, 157};

    // The labels for the bar chart
    const char *labels[] = {"Mon", "Tue", "Wed", "Thu", "Fri"};

    // Create a XYChart object of size 500 x 320 pixels
    XYChart *c = new XYChart(500, 320);

    // Set the plotarea at (100, 40) and of size 280 x 240 pixels
    c->setPlotArea(100, 40, 280, 240);

    // Add a legend box at (405, 100)
    c->addLegend(405, 100);

    // Add a title to the chart
    c->addTitle("Weekday Network Load");

    // Add a title to the y axis. Draw the title upright (font angle = 0)
    c->yAxis()->setTitle("Average\nWorkload\n(MBytes\nPer Hour)")->setFontAngle(0);

    // Set the labels on the x axis
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Add three bar layers, each representing one data set. The bars are drawn in semi-transparent
    // colors.
    c->addBarLayer(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0x808080ff,
        "Server # 1", 5);
    c->addBarLayer(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x80ff0000,
        "Server # 2", 5);
    c->addBarLayer(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), 0x8000ff00,
        "Server # 3", 5);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName} on {xLabel}: {value} MBytes/hour'");

    return c;
}


BaseChart *posnegbar(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data[] = {-6.3, 2.3, 0.7, -3.4, 2.2, -2.9, -0.1, -0.1, 3.3, 6.2, 4.3, 1.6};

    // The labels for the bar chart
    const char *labels[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct",
        "Nov", "Dec"};

    // Create a XYChart object of size 500 x 320 pixels
    XYChart *c = new XYChart(500, 320);

    // Add a title to the chart using Arial Bold Italic font
    c->addTitle("Productivity Change - Year 2005", "arialbi.ttf");

    // Set the plotarea at (50, 30) and of size 400 x 250 pixels
    c->setPlotArea(50, 30, 400, 250);

    // Add a bar layer to the chart using the Overlay data combine method
    BarLayer *layer = c->addBarLayer(Chart::Overlay);

    // Select positive data and add it as data set with blue (6666ff) color
    layer->addDataSet(ArrayMath(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0])))).selectGEZ(
        DoubleArray(), Chart::NoValue), 0x6666ff);

    // Select negative data and add it as data set with orange (ff6600) color
    layer->addDataSet(ArrayMath(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0])))).selectLTZ(
        DoubleArray(), Chart::NoValue), 0xff6600);

    // Add labels to the top of the bar using 8 pt Arial Bold font. The font color is configured to
    // be red (0xcc3300) below zero, and blue (0x3333ff) above zero.
    layer->setAggregateLabelStyle("arialbd.ttf", 8, layer->yZoneColor(0, 0xcc3300, 0x3333ff));

    // Set the labels on the x axis and use Arial Bold as the label font
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0])))
        )->setFontStyle("arialbd.ttf");

    // Draw the y axis on the right of the plot area
    c->setYAxisOnRight(true);

    // Use Arial Bold as the y axis label font
    c->yAxis()->setLabelStyle("arialbd.ttf");

    // Add a title to the y axis
    c->yAxis()->setTitle("Percentage");

    // Add a light blue (0xccccff) zone for positive part of the plot area
    c->yAxis()->addZone(0, 9999, 0xccccff);

    // Add a pink (0xffffcc) zone for negative part of the plot area
    c->yAxis()->addZone(-9999, 0, 0xffcccc);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{xLabel}: {value}%'");

    return c;
}


BaseChart *hbar(int /* chartIndex */, const char **imageMap)
{
    // The data for the bar chart
    double data[] = {3.9, 8.1, 10.9, 14.2, 18.1, 19.0, 21.2, 23.2, 25.7, 36};

    // The labels for the bar chart
    const char *labels[] = {"Bastic Group", "Simpa", "YG Super", "CID", "Giga Tech", "Indo Digital",
        "Supreme", "Electech", "THP Thunder", "Flash Light"};

    // Create a XYChart object of size 600 x 250 pixels
    XYChart *c = new XYChart(600, 250);

    // Add a title to the chart using Arial Bold Italic font
    c->addTitle("Revenue Estimation - Year 2002", "arialbi.ttf");

    // Set the plotarea at (100, 30) and of size 400 x 200 pixels. Set the plotarea border,
    // background and grid lines to Transparent
    c->setPlotArea(100, 30, 400, 200, Chart::Transparent, Chart::Transparent, Chart::Transparent,
        Chart::Transparent, Chart::Transparent);

    // Add a bar chart layer using the given data. Use a gradient color for the bars, where the
    // gradient is from dark green (0x008000) to white (0xffffff)
    BarLayer *layer = c->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))),
        c->gradientColor(100, 0, 500, 0, 0x008000, 0xffffff));

    // Swap the axis so that the bars are drawn horizontally
    c->swapXY(true);

    // Set the bar gap to 10%
    layer->setBarGap(0.1);

    // Use the format "US$ xxx millions" as the bar label
    layer->setAggregateLabelFormat("US$ {value} millions");

    // Set the bar label font to 10pt Times Bold Italic/dark red (0x663300)
    layer->setAggregateLabelStyle("timesbi.ttf", 10, 0x663300);

    // Set the labels on the x axis
    TextBox *textbox = c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(
        labels[0]))));

    // Set the x axis label font to 10pt Arial Bold Italic
    textbox->setFontStyle("arialbi.ttf");
    textbox->setFontSize(10);

    // Set the x axis to Transparent, with labels in dark red (0x663300)
    c->xAxis()->setColors(Chart::Transparent, 0x663300);

    // Set the y axis and labels to Transparent
    c->yAxis()->setColors(Chart::Transparent, Chart::Transparent);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{xLabel}: US${value} millions'");

    return c;
}


BaseChart *dualhbar(int /* chartIndex */, const char **imageMap)
{
    // The age groups
    const char *labels[] = {"0 - 4", "5 - 9", "10 - 14", "15 - 19", "20 - 24", "24 - 29", "30 - 34",
        "35 - 39", "40 - 44", "44 - 49", "50 - 54", "55 - 59", "60 - 64", "65 - 69", "70 - 74",
        "75 - 79", "80+"};

    // The male population (in thousands)
    double male[] = {215, 238, 225, 236, 235, 260, 286, 340, 363, 305, 259, 164, 135, 127, 102, 68,
        66};

    // The female population (in thousands)
    double female[] = {194, 203, 201, 220, 228, 271, 339, 401, 384, 304, 236, 137, 116, 122, 112,
        85, 110};


    //=============================================================
    //    Draw the right bar chart
    //=============================================================

    // Create a XYChart object of size 320 x 300 pixels
    XYChart *c = new XYChart(320, 300);

    // Set the plotarea at (50, 0) and of size 250 x 255 pixels. Use pink (0xffdddd) as the
    // background.
    c->setPlotArea(50, 0, 250, 255, 0xffdddd);

    // Add a custom text label at the top right corner of the right bar chart
    c->addText(300, 0, "Female", "timesbi.ttf", 12, 0xa07070)->setAlignment(Chart::TopRight);

    // Add the pink (0xf0c0c0) bar chart layer using the female data
    BarLayer *femaleLayer = c->addBarLayer(DoubleArray(female, (int)(sizeof(female) / sizeof(female[
        0]))), 0xf0c0c0, "Female");

    // Swap the axis so that the bars are drawn horizontally
    c->swapXY(true);

    // Set the bar to touch each others
    femaleLayer->setBarGap(Chart::TouchBar);

    // Set the border style of the bars to 1 pixel 3D border
    femaleLayer->setBorderColor(-1, 1);

    // Add a Transparent line layer to the chart using the male data. As it is Transparent, only the
    // female bar chart can be seen. We need to put both male and female data in both left and right
    // charts, because we want auto-scaling to produce the same scale for both chart.
    c->addLineLayer(DoubleArray(male, (int)(sizeof(male) / sizeof(male[0]))), Chart::Transparent);

    // Set the y axis label font to Arial Bold
    c->yAxis()->setLabelStyle("arialbd.ttf");

    // Set the labels between the two bar charts, which can be considered as the x-axis labels for
    // the right chart
    TextBox *tb = c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0])
        )));

    // Use a fix width of 50 for the labels (height = automatic) with center alignment
    tb->setSize(50, 0);
    tb->setAlignment(Chart::Center);

    // Set the label font to Arial Bold
    tb->setFontStyle("arialbd.ttf");

    // Disable ticks on the x-axis by setting the tick length to 0
    c->xAxis()->setTickLength(0);

    //=============================================================
    //    Draw the left bar chart
    //=============================================================

    // Create a XYChart object of size 280 x 300 pixels with a transparent background.
    XYChart *c2 = new XYChart(280, 300, Chart::Transparent);

    // Set the plotarea at (20, 0) and of size 250 x 255 pixels. Use pale blue (0xddddff) as the
    // background.
    c2->setPlotArea(20, 0, 250, 255, 0xddddff);

    // Add a custom text label at the top left corner of the left bar chart
    c2->addText(20, 0, "Male", "timesbi.ttf", 12, 0x7070a0);

    // Add the pale blue (0xaaaaff) bar chart layer using the male data
    BarLayer *maleLayer = c2->addBarLayer(DoubleArray(male, (int)(sizeof(male) / sizeof(male[0]))),
        0xaaaaff, "Male");

    // Swap the axis so that the bars are drawn horizontally
    c2->swapXY(true);

    // Reverse the direction of the y-axis so it runs from right to left
    c2->yAxis()->setReverse();

    // Set the bar to touch each others
    maleLayer->setBarGap(Chart::TouchBar);

    // Set the border style of the bars to 1 pixel 3D border
    maleLayer->setBorderColor(-1, 1);

    // Add a Transparent line layer to the chart using the female data. As it is Transparent, only
    // the male bar chart can be seen. We need to put both male and female data in both left and
    // right charts, because we want auto-scaling to produce the same scale for both chart.
    c2->addLineLayer(DoubleArray(female, (int)(sizeof(female) / sizeof(female[0]))),
        Chart::Transparent);

    // Set the y axis label font to Arial Bold
    c2->yAxis()->setLabelStyle("arialbd.ttf");

    // Set the x-axis labels for tool tip purposes.
    c2->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Hide the x-axis labels by setting them to Transparent. We only need to display the x-axis
    // labels for the right chart.
    c2->xAxis()->setColors(0x000000, Chart::Transparent, -1, Chart::Transparent);

    //=============================================================
    //    Use a MultiChart to contain both bar charts
    //=============================================================

    // Create a MultiChart object of size 590 x 320 pixels.
    MultiChart *m = new MultiChart(590, 320);

    // Add a title to the chart using Arial Bold Italic font
    m->addTitle("Demographics Hong Kong Year 2002", "arialbi.ttf");

    // Add another title at the bottom using Arial Bold Italic font
    m->addTitle(Chart::Bottom, "Population (in thousands)", "arialbi.ttf", 10);

    // Put the right chart at (270, 25)
    m->addChart(270, 25, c);

    // Put the left chart at (0, 25)
    m->addChart(0, 25, c2);

    // Output the chart
    m->makeChart();

    //include tool tip for the chart
    *imageMap = m->getHTMLImageMap("clickable", "",
        "title='{dataSetName} (Age {xLabel}): Population {value}K'");

    //free up resources
    delete c;
    delete c2;

    return m;
}


BaseChart *markbar(int /* chartIndex */, const char **imageMap)
{
    // The data for the bars and the marks
    double barData[] = {100, 125, 245, 147, 67, 96, 160, 145, 97, 167, 220, 125};
    double markData[] = {85, 156, 220, 120, 80, 110, 140, 130, 111, 180, 175, 100};

    // The labels for the bar chart
    const char *labels[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct",
        "Nov", "Dec"};

    // Create a XYChart object of size 480 x 360 pixels. Use a vertical gradient color from pale
    // blue (e8f0f8) to sky blue (aaccff) spanning half the chart height as background. Set border
    // to blue (88aaee). Use rounded corners. Enable soft drop shadow.
    XYChart *c = new XYChart(480, 360);
    c->setBackground(c->linearGradientColor(0, 0, 0, c->getHeight() / 2, 0xe8f0f8, 0xaaccff),
        0x88aaee);
    c->setRoundedFrame();
    c->setDropShadow();

    // Add a title to the chart using 15 points Arial Italic font. Set top/bottom margins to 12
    // pixels.
    TextBox *title = c->addTitle("Bars with Marks Demonstration", "ariali.ttf", 15);
    title->setMargin(0, 0, 12, 12);

    // Tentatively set the plotarea to 50 pixels from the left edge to allow for the y-axis, and to
    // just under the title. Set the width to 65 pixels less than the chart width, and the height to
    // reserve 90 pixels at the bottom for the x-axis and the legend box. Use pale blue (e8f0f8)
    // background, transparent border, and grey (888888) dotted horizontal grid lines.
    c->setPlotArea(50, title->getHeight(), c->getWidth() - 65, c->getHeight() - title->getHeight() -
        90, 0xe8f0f8, -1, Chart::Transparent, c->dashLineColor(0x888888, Chart::DotLine));

    // Add a legend box where the bottom-center is anchored to the 15 pixels above the bottom-center
    // of the chart. Use horizontal layout and 8 points Arial font.
    LegendBox *legendBox = c->addLegend(c->getWidth() / 2, c->getHeight() - 15, false, "arial.ttf",
        8);
    legendBox->setAlignment(Chart::BottomCenter);

    // Set the legend box background and border to pale blue (e8f0f8) and bluish grey (445566)
    legendBox->setBackground(0xe8f0f8, 0x445566);

    // Use rounded corners of 5 pixel radius for the legend box
    legendBox->setRoundedCorners(5);

    // Use line style legend key
    legendBox->setLineStyleKey();

    // Set axes to transparent
    c->xAxis()->setColors(Chart::Transparent);
    c->yAxis()->setColors(Chart::Transparent);

    // Set the labels on the x axis
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Add a box-whisker layer with just the middle mark visible for the marks. Use red (ff0000)
    // color for the mark, with a line width of 2 pixels and 10% horizontal gap
    BoxWhiskerLayer *markLayer = c->addBoxWhiskerLayer(DoubleArray(), DoubleArray(), DoubleArray(),
        DoubleArray(), DoubleArray(markData, (int)(sizeof(markData) / sizeof(markData[0]))), -1,
        0xff0000);
    markLayer->setLineWidth(2);
    markLayer->setDataGap(0.1);

    // Add the legend key for the mark line
    legendBox->addKey("Target", 0xff0000, 2);

    // Tool tip for the mark layer
    markLayer->setHTMLImageMap("", "", "title='Target at {xLabel}: {med}'");

    // Add a blue (0066cc) bar layer using the given data.
    BarLayer *barLayer = c->addBarLayer(DoubleArray(barData, (int)(sizeof(barData) / sizeof(barData[
        0]))), 0x0066cc, "Actual");

    // Use soft lighting effect for the bars with light direction from left.
    barLayer->setBorderColor(Chart::Transparent, Chart::softLighting(Chart::Left));

    // Tool tip for the bar layer
    barLayer->setHTMLImageMap("", "", "title='{dataSetName} at {xLabel}: {value}'");

    // Adjust the plot area size, such that the bounding box (inclusive of axes) is 10 pixels from
    // the left edge, just below the title, 15 pixels from the right edge, and 10 pixels above the
    // legend box.
    c->packPlotArea(10, title->getHeight(), c->getWidth() - 15, c->layoutLegend()->getTopY() - 10);

    // Output the chart
    c->makeChart();

    // Include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable");

    return c;
}


BaseChart *histogram(int /* chartIndex */, const char **imageMap)
{
    char buffer[256];

    //
    // This example demonstrates creating a histogram with a bell curve from raw data. About half of
    // the code is to sort the raw data into slots and to generate the points on the bell curve. The
    // remaining half of the code is the actual charting code.
    //

    // Generate a random guassian distributed data series as the input data for this example.
    RanSeries *r = new RanSeries(66);
    DoubleArray samples = r->getGaussianSeries(200, 100, 10);

    //
    // Classify the numbers into slots. In this example, the slot width is 5 units.
    //
    int slotSize = 5;

    // Compute the min and max values, and extend them to the slot boundary.
    ArrayMath m = ArrayMath(samples);
    double minX = floor(m.minValue() / slotSize) * slotSize;
    double maxX = floor(m.maxValue() / slotSize) * slotSize + slotSize;

    // We can now determine the number of slots
    int slotCount = (int)((maxX - minX + 0.5) / slotSize);
    double *frequency = new double[slotCount];
    memset(frequency, 0, sizeof(*frequency) * slotCount);

    // Count the data points contained in each slot
    for(int i = 0; i < samples.len; ++i) {
        int slotIndex = (int)((samples[i] - minX) / slotSize);
        frequency[slotIndex] = frequency[slotIndex] + 1;
    }

    //
    // Compute Normal Distribution Curve
    //

    // The mean and standard deviation of the data
    double mean = m.avg();
    double stdDev = m.stdDev();

    // The normal distribution curve (bell curve) is a standard statistics curve. We need to
    // vertically scale it to make it proportion to the frequency count.
    double scaleFactor = slotSize * (samples.len) / stdDev / sqrt(6.2832);

    // In this example, we plot the bell curve up to 3 standard deviations.
    double stdDevWidth = 3.0;

    // We generate 4 points per standard deviation to be joined with a spline curve.
    int bellCurveResolution = (int)(stdDevWidth * 4 + 1);
    double *bellCurve = new double[bellCurveResolution];
    for(int i = 0; i < bellCurveResolution; ++i) {
        double z = 2 * i * stdDevWidth / (bellCurveResolution - 1) - stdDevWidth;
        bellCurve[i] = exp(-z * z / 2) * scaleFactor;
    }

    //
    // At this stage, we have obtained all data and can plot the chart.
    //

    // Create a XYChart object of size 600 x 360 pixels
    XYChart *c = new XYChart(600, 360);

    // Set the plotarea at (50, 30) and of size 500 x 300 pixels, with transparent background and
    // border and light grey (0xcccccc) horizontal grid lines
    c->setPlotArea(50, 30, 500, 300, Chart::Transparent, -1, Chart::Transparent, 0xcccccc);

    // Display the mean and standard deviation on the chart
    sprintf(buffer, "Mean = %.1f, Standard Deviation = %.1f", mean, stdDev);
    c->addTitle(buffer, "arial.ttf");

    // Set the x and y axis label font to 12pt Arial
    c->xAxis()->setLabelStyle("arial.ttf", 12);
    c->yAxis()->setLabelStyle("arial.ttf", 12);

    // Set the x and y axis stems to transparent, and the x-axis tick color to grey (0x888888)
    c->xAxis()->setColors(Chart::Transparent, Chart::TextColor, Chart::TextColor, 0x888888);
    c->yAxis()->setColors(Chart::Transparent);

    // Draw the bell curve as a spline layer in red (0xdd0000) with 2-pixel line width
    SplineLayer *bellLayer = c->addSplineLayer(DoubleArray(bellCurve, bellCurveResolution), 0xdd0000
        );
    bellLayer->setXData(mean - stdDevWidth * stdDev, mean + stdDevWidth * stdDev);
    bellLayer->setLineWidth(2);
    // No tooltip is needed for the spline layer
    bellLayer->setHTMLImageMap("{disable}");

    // Draw the histogram as bars in blue (0x6699bb) with dark blue (0x336688) border
    BarLayer *histogramLayer = c->addBarLayer(DoubleArray(frequency, slotCount), 0x6699bb);
    histogramLayer->setBorderColor(0x336688);
    // The center of the bars span from minX + half_bar_width to maxX - half_bar_width
    histogramLayer->setXData(minX + slotSize / 2.0, maxX - slotSize / 2.0);
    // Configure the bars to touch each other with no gap in between
    histogramLayer->setBarGap(Chart::TouchBar);
    // Use rounded corners for decoration
    histogramLayer->setRoundedCorners();
    // Tool tip for the histogram
    histogramLayer->setHTMLImageMap("", "", "title='{value}'");

    // ChartDirector by default will extend the x-axis scale by 0.5 unit to cater for the bar width.
    // It is because a bar plotted at x actually occupies (x +/- half_bar_width), and the bar width
    // is normally 1 for label based x-axis. However, this chart is using a linear x-axis instead of
    // label based. So we disable the automatic extension and add a dummy layer to extend the x-axis
    // scale to cover minX to maxX.
    c->xAxis()->setIndent(false);
    c->addLineLayer()->setXData(minX, maxX);

    // For the automatic y-axis labels, set the minimum spacing to 40 pixels.
    c->yAxis()->setTickDensity(40);

    // Output the chart
    c->makeChart();

    // Include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable");

    //free up resources
    delete r;
    delete frequency;
    delete bellCurve;

    return c;
}


BaseChart *pareto(int /* chartIndex */, const char **imageMap)
{
    // The data for the chart
    double data[] = {40, 15, 7, 5, 2};

    // The labels for the chart
    const char *labels[] = {"Hard Disk", "PCB", "Printer", "CDROM", "Keyboard"};

    // In the pareto chart, the line data are just the accumulation of the raw data, scaled to a
    // range of 0 - 100%
    ArrayMath lineData = ArrayMath(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))));
    lineData.acc();
    double scaleFactor = lineData.maxValue() / 100;
    if (scaleFactor == 0) {
        // Avoid division by zero error for zero data
        scaleFactor = 1;
    }
    lineData.div(scaleFactor);

    // Create a XYChart object of size 480 x 300 pixels. Set background color to brushed silver,
    // with a grey (bbbbbb) border and 2 pixel 3D raised effect. Use rounded corners. Enable soft
    // drop shadow.
    XYChart *c = new XYChart(400, 300, Chart::brushedSilverColor(), 0xbbbbbb, 2);
    c->setRoundedFrame();
    c->setDropShadow();

    // Add a title to the chart using 15 points Arial Italic. Set top/bottom margins to 12 pixels.
    TextBox *title = c->addTitle("Pareto Chart Demonstration", "ariali.ttf", 15);
    title->setMargin(0, 0, 12, 12);

    // Tentatively set the plotarea at (50, 40). Set the width to 100 pixels less than the chart
    // width, and the height to 80 pixels less than the chart height. Use pale grey (f4f4f4)
    // background, transparent border, and dark grey (444444) dotted grid lines.
    c->setPlotArea(50, 40, c->getWidth() - 100, c->getHeight() - 80, 0xf4f4f4, -1,
        Chart::Transparent, c->dashLineColor(0x444444, Chart::DotLine));

    // Add a line layer for the pareto line
    LineLayer *lineLayer = c->addLineLayer();

    // Add the pareto line using deep blue (0000ff) as the color, with circle symbols
    lineLayer->addDataSet(lineData.result(), 0x0000ff)->setDataSymbol(Chart::CircleShape, 9,
        0x0000ff, 0x0000ff);

    // Set the line width to 2 pixel
    lineLayer->setLineWidth(2);

    // Bind the line layer to the secondary (right) y-axis.
    lineLayer->setUseYAxis2();

    // Tool tip for the line layer
    lineLayer->setHTMLImageMap("", "", "title='Top {={x}+1} items: {value|2}%'");

    // Add a multi-color bar layer using the given data.
    BarLayer *barLayer = c->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))),
        IntArray(0, 0));

    // Set soft lighting for the bars with light direction from the right
    barLayer->setBorderColor(Chart::Transparent, Chart::softLighting(Chart::Right));

    // Tool tip for the bar layer
    barLayer->setHTMLImageMap("", "", "title='{xLabel}: {value} pieces'");

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Set the secondary (right) y-axis scale as 0 - 100 with a tick every 20 units
    c->yAxis2()->setLinearScale(0, 100, 20);

    // Set the format of the secondary (right) y-axis label to include a percentage sign
    c->yAxis2()->setLabelFormat("{value}%");

    // Set the relationship between the two y-axes, which only differ by a scaling factor
    c->yAxis()->syncAxis(c->yAxis2(), scaleFactor);

    // Set the format of the primary y-axis label foramt to show no decimal point
    c->yAxis()->setLabelFormat("{value|0}");

    // Add a title to the primary y-axis
    c->yAxis()->setTitle("Frequency");

    // Set all axes to transparent
    c->xAxis()->setColors(Chart::Transparent);
    c->yAxis()->setColors(Chart::Transparent);
    c->yAxis2()->setColors(Chart::Transparent);

    // Adjust the plot area size, such that the bounding box (inclusive of axes) is 10 pixels from
    // the left edge, just below the title, 10 pixels from the right edge, and 20 pixels from the
    // bottom edge.
    c->packPlotArea(10, title->getHeight(), c->getWidth() - 10, c->getHeight() - 20);

    // Output the chart
    c->makeChart();

    // Include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable");

    return c;
}


BaseChart *varwidthbar(int /* chartIndex */, const char **imageMap)
{
    char buffer[256];

    // The data for the chart
    double data[] = {800, 600, 1000, 1400};
    double widths[] = {250, 500, 960, 460};
    const char *labels[] = {"Wind", "Hydro", "Coal", "Natural Gas"};

    // The colors to use
    int colors[] = {0x00aa00, 0x66aaee, 0xee6622, 0xffbb00};

    // Create a XYChart object of size 500 x 350 pixels
    XYChart *c = new XYChart(500, 350);

    // Add a title to the chart using 15pt Arial Italic font
    c->addTitle("Energy Generation Breakdown", "ariali.ttf", 15);

    // Set the plotarea at (60, 60) and of (chart_width - 90) x (chart_height - 100) in size. Use a
    // vertical gradient color from light blue (f9f9ff) to sky blue (aaccff) as background. Set grid
    // lines to white (ffffff).
    int plotAreaBgColor = c->linearGradientColor(0, 60, 0, c->getHeight() - 40, 0xaaccff, 0xf9fcff);
    c->setPlotArea(60, 60, c->getWidth() - 90, c->getHeight() - 100, plotAreaBgColor, -1, -1,
        0xffffff);

    // Add a legend box at (50, 30) using horizontal layout and transparent background.
    c->addLegend(55, 30, false)->setBackground(Chart::Transparent);

    // Add titles to x/y axes with 10 points Arial Bold font
    c->xAxis()->setTitle("Mega Watts", "arialbd.ttf", 10);
    c->yAxis()->setTitle("Cost per MWh (dollars)", "arialbd.ttf", 10);

    // Set the x axis rounding to false, so that the x-axis will fit the data exactly
    c->xAxis()->setRounding(false, false);

    // In ChartDirector, there is no bar layer that can have variable bar widths, but you may create
    // a bar using an area layer. (A bar can be considered as the area under a rectangular outline.)
    // So by using a loop to create one bar per area layer, we can achieve a variable width bar
    // chart.

    // starting position of current bar
    double currentX = 0;

    for(int i = 0; i < (int)(sizeof(data) / sizeof(data[0])); ++i) {
        // ending position of current bar
        double nextX = currentX + widths[i];

        // outline of the bar
        double dataX[] = {currentX, currentX, nextX, nextX};
        double dataY[] = {0, data[i], data[i], 0};

        // create the area layer to fill the bar
        AreaLayer *layer = c->addAreaLayer(DoubleArray(dataY, (int)(sizeof(dataY) / sizeof(dataY[0])
            )), colors[i], labels[i]);
        layer->setXData(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0]))));

        // Tool tip for the layer
        sprintf(buffer, "title='%s: %g MW at $%g per MWh'", labels[i], widths[i], data[i]);
        layer->setHTMLImageMap("", "", buffer);

        // the ending position becomes the starting position of the next bar
        currentX = nextX;
    }

    // Output the chart
    c->makeChart();

    // Include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable");

    return c;
}


BaseChart *gapbar(int chartIndex, const char **imageMap)
{
    char buffer[256];

    double bargap = chartIndex * 0.25 - 0.25;

    // The data for the bar chart
    double data[] = {100, 125, 245, 147, 67};

    // The labels for the bar chart
    const char *labels[] = {"Mon", "Tue", "Wed", "Thu", "Fri"};

    // Create a XYChart object of size 150 x 150 pixels
    XYChart *c = new XYChart(150, 150);

    // Set the plotarea at (27, 20) and of size 120 x 100 pixels
    c->setPlotArea(27, 20, 120, 100);

    // Set the labels on the x axis
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    if (bargap >= 0) {
        // Add a title to display to bar gap using 8pt Arial font
        sprintf(buffer, "      Bar Gap = %g", bargap);
        c->addTitle(buffer, "arial.ttf", 8);
    } else {
        // Use negative value to mean TouchBar
        c->addTitle("      Bar Gap = TouchBar", "arial.ttf", 8);
        bargap = Chart::TouchBar;
    }

    // Add a bar chart layer using the given data and set the bar gap
    c->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))))->setBarGap(bargap);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='Production on {xLabel}: {value} kg'");

    return c;
}


BaseChart *simpleline(int /* chartIndex */, const char **imageMap)
{
    // The data for the line chart
    double data[] = {30, 28, 40, 55, 75, 68, 54, 60, 50, 62, 75, 65, 75, 91, 60, 55, 53, 35, 50, 66,
        56, 48, 52, 65, 62};

    // The labels for the line chart
    const char *labels[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12",
        "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24"};

    // Create a XYChart object of size 250 x 250 pixels
    XYChart *c = new XYChart(250, 250);

    // Set the plotarea at (30, 20) and of size 200 x 200 pixels
    c->setPlotArea(30, 20, 200, 200);

    // Add a line chart layer using the given data
    c->addLineLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))));

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Display 1 out of 3 labels on the x-axis.
    c->xAxis()->setLabelStep(3);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='Hour {xLabel}: Traffic {value} GBytes'")
        ;

    return c;
}


BaseChart *compactline(int /* chartIndex */, const char **imageMap)
{
    //
    //    We use a random number generator to simulate the data from 9:30am to 4:30pm with one data
    //    point every 4 minutes. The total number of points during that period is 106.  (7 hours x
    //    15 points/hour + 1)
    //
    int noOfPoints = 106;

    // Assume we have not reached the end of the day yet, and only 85 points are available. Create a
    // random table object of 1 col x 85 rows, using 9 as seed.
    RanTable *rantable = new RanTable(9, 1, 85);

    // Set the 1st column to start with 1800 and with random delta from -5 to 5.
    rantable->setCol(0, 1800, -5, 5);

    // Get the data as the 1st column of the random table
    DoubleArray data = rantable->getCol(0);

    // The x-axis labels for the chart
    const char *labels[] = {"-", "10am", "-", " ", "-", "12am", "-", " ", "-", "2pm", "-", " ", "-",
        "4pm", "-"};

    //
    //    Now we obtain the data into arrays, we can start to draw the chart using ChartDirector
    //

    // Create a XYChart object of size 180 x 180 pixels with a blue background (0x9c9cce)
    XYChart *c = new XYChart(180, 180, 0x9c9cce);

    // Add titles to the top and bottom of the chart using 7.5pt Arial font. The text is white
    // 0xffffff on a deep blue 0x31319C background.
    c->addTitle(Chart::Top, "STAR TECH INDEX  2003-01-28", "arial.ttf", 7.5, 0xffffff, 0x31319c);
    c->addTitle(Chart::Bottom, "LATEST  STI:1809.41 (+14.51)", "arial.ttf", 7.5, 0xffffff, 0x31319c)
        ;

    // Set the plotarea at (31, 21) and of size 145 x 124 pixels, with a pale yellow (0xffffc8)
    // background.
    c->setPlotArea(31, 21, 145, 124, 0xffffc8);

    // Add custom text at (176, 21) (top right corner of plotarea) using 11pt Times Bold Italic
    // font/red (0xc09090) color
    c->addText(176, 21, "Chart Demo", "timesbi.ttf", 11, 0xc09090)->setAlignment(Chart::TopRight);

    // Use 7.5pt Arial as the y axis label font
    c->yAxis()->setLabelStyle("", 7.5);

    // Set the labels on the x axis by spreading the labels evenly between the first point (index =
    // 0) and the last point (index = noOfPoints - 1)
    c->xAxis()->setLinearScale(0, noOfPoints - 1, StringArray(labels, (int)(sizeof(labels) / sizeof(
        labels[0]))));

    // Use 7.5pt Arial as the x axis label font
    c->xAxis()->setLabelStyle("", 7.5);

    // Add a deep blue (0x000080) line layer to the chart
    c->addLineLayer(data, 0x000080);

    // Output the chart
    c->makeChart();

    // Include tool tip for the chart. The chart starts at 9:30am and each point spans 240 seconds,
    // so we can compute the time as {x}*240+9.5*3600.
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{={x}*240+9.5*3600|h:nna}: {value|2}'");

    //free up resources
    delete rantable;

    return c;
}


BaseChart *threedline(int /* chartIndex */, const char **imageMap)
{
    // The data for the line chart
    double data[] = {30, 28, 40, 55, 75, 68, 54, 60, 50, 62, 75, 65, 75, 91, 60, 55, 53, 35, 50, 66,
        56, 48, 52, 65, 62};

    // The labels for the line chart
    const char *labels[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12",
        "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24"};

    // Create a XYChart object of size 300 x 280 pixels
    XYChart *c = new XYChart(300, 280);

    // Set the plotarea at (45, 30) and of size 200 x 200 pixels
    c->setPlotArea(45, 30, 200, 200);

    // Add a title to the chart using 12pt Arial Bold Italic font
    c->addTitle("Daily Server Utilization", "arialbi.ttf", 12);

    // Add a title to the y axis
    c->yAxis()->setTitle("MBytes");

    // Add a title to the x axis
    c->xAxis()->setTitle("June 12, 2001");

    // Add a blue (0x6666ff) 3D line chart layer using the give data
    c->addLineLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), 0x6666ff)->set3D();

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Display 1 out of 3 labels on the x-axis.
    c->xAxis()->setLabelStep(3);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='Hour {xLabel}: {value} MBytes'");

    return c;
}


BaseChart *multiline(int /* chartIndex */, const char **imageMap)
{
    // The data for the line chart
    double data0[] = {42, 49, 33, 38, 51, 46, 29, 41, 44, 57, 59, 52, 37, 34, 51, 56, 56, 60, 70,
        76, 63, 67, 75, 64, 51};
    double data1[] = {50, 55, 47, 34, 42, 49, 63, 62, 73, 59, 56, 50, 64, 60, 67, 67, 58, 59, 73,
        77, 84, 82, 80, 84, 98};
    double data2[] = {36, 28, 25, 33, 38, 20, 22, 30, 25, 33, 30, 24, 28, 15, 21, 26, 46, 42, 48,
        45, 43, 52, 64, 60, 70};

    // The labels for the line chart
    const char *labels[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12",
        "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24"};

    // Create an XYChart object of size 600 x 300 pixels, with a light blue (EEEEFF) background,
    // black border, 1 pxiel 3D border effect and rounded corners
    XYChart *c = new XYChart(600, 300, 0xeeeeff, 0x000000, 1);
    c->setRoundedFrame();

    // Set the plotarea at (55, 58) and of size 520 x 195 pixels, with white background. Turn on
    // both horizontal and vertical grid lines with light grey color (0xcccccc)
    c->setPlotArea(55, 58, 520, 195, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);

    // Add a legend box at (50, 30) (top of the chart) with horizontal layout. Use 9pt Arial Bold
    // font. Set the background and border color to Transparent.
    c->addLegend(50, 30, false, "arialbd.ttf", 9)->setBackground(Chart::Transparent);

    // Add a title box to the chart using 15pt Times Bold Italic font, on a light blue (CCCCFF)
    // background with glass effect. white (0xffffff) on a dark red (0x800000) background, with a 1
    // pixel 3D border.
    c->addTitle("Application Server Throughput", "timesbi.ttf", 15)->setBackground(0xccccff,
        0x000000, Chart::glassEffect());

    // Add a title to the y axis
    c->yAxis()->setTitle("MBytes per hour");

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Display 1 out of 3 labels on the x-axis.
    c->xAxis()->setLabelStep(3);

    // Add a title to the x axis
    c->xAxis()->setTitle("Jun 12, 2006");

    // Add a line layer to the chart
    LineLayer *layer = c->addLineLayer();

    // Set the default line width to 2 pixels
    layer->setLineWidth(2);

    // Add the three data sets to the line layer. For demo purpose, we use a dash line color for the
    // last line
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0xff0000,
        "Server #1");
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x008800,
        "Server #2");
    layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), c->dashLineColor(
        0x3333ff, Chart::DashLine), "Server #3");

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='[{dataSetName}] Hour {xLabel}: {value} MBytes'");

    return c;
}


BaseChart *multiline2(int /* chartIndex */, const char **imageMap)
{
    // In this example, we simply use random data for the 3 data series.
    RanSeries *r = new RanSeries(129);
    DoubleArray data0 = r->getSeries(100, 100, -15, 15);
    DoubleArray data1 = r->getSeries(100, 160, -15, 15);
    DoubleArray data2 = r->getSeries(100, 220, -15, 15);
    DoubleArray timeStamps = r->getDateSeries(100, Chart::chartTime(2014, 1, 1), 86400);

    // Create a XYChart object of size 600 x 400 pixels
    XYChart *c = new XYChart(600, 400);

    // Add a title box using grey (0x555555) 20pt Arial font
    c->addTitle("    Multi-Line Chart Demonstration", "arial.ttf", 20, 0x555555);

    // Set the plotarea at (70, 70) and of size 500 x 300 pixels, with transparent background and
    // border and light grey (0xcccccc) horizontal grid lines
    c->setPlotArea(70, 70, 500, 300, Chart::Transparent, -1, Chart::Transparent, 0xcccccc);

    // Add a legend box with horizontal layout above the plot area at (70, 35). Use 12pt Arial font,
    // transparent background and border, and line style legend icon.
    LegendBox *b = c->addLegend(70, 35, false, "arial.ttf", 12);
    b->setBackground(Chart::Transparent, Chart::Transparent);
    b->setLineStyleKey();

    // Set axis label font to 12pt Arial
    c->xAxis()->setLabelStyle("arial.ttf", 12);
    c->yAxis()->setLabelStyle("arial.ttf", 12);

    // Set the x and y axis stems to transparent, and the x-axis tick color to grey (0xaaaaaa)
    c->xAxis()->setColors(Chart::Transparent, Chart::TextColor, Chart::TextColor, 0xaaaaaa);
    c->yAxis()->setColors(Chart::Transparent);

    // Set the major/minor tick lengths for the x-axis to 10 and 0.
    c->xAxis()->setTickLength(10, 0);

    // For the automatic axis labels, set the minimum spacing to 80/40 pixels for the x/y axis.
    c->xAxis()->setTickDensity(80);
    c->yAxis()->setTickDensity(40);

    // Add a title to the y axis using dark grey (0x555555) 14pt Arial font
    c->yAxis()->setTitle("Y-Axis Title Placeholder", "arial.ttf", 14, 0x555555);

    // Add a line layer to the chart with 3-pixel line width
    LineLayer *layer = c->addLineLayer();
    layer->setLineWidth(3);

    // Add 3 data series to the line layer
    layer->addDataSet(data0, 0x5588cc, "Alpha");
    layer->addDataSet(data1, 0xee9944, "Beta");
    layer->addDataSet(data2, 0x99bb55, "Gamma");

    // The x-coordinates for the line layer
    layer->setXData(timeStamps);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='[{x|mm/dd/yyyy}] {dataSetName}: {value}'");

    //free up resources
    delete r;

    return c;
}


BaseChart *symbolline(int /* chartIndex */, const char **imageMap)
{
    // The data for the line chart
    double data0[] = {60.2, 51.7, 81.3, 48.6, 56.2, 68.9, 52.8};
    double data1[] = {30.0, 32.7, 33.9, 29.5, 32.2, 28.4, 29.8};
    const char *labels[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    // Create a XYChart object of size 300 x 180 pixels, with a pale yellow (0xffffc0) background, a
    // black border, and 1 pixel 3D border effect.
    XYChart *c = new XYChart(300, 180, 0xffffc0, 0x000000, 1);

    // Set the plotarea at (45, 35) and of size 240 x 120 pixels, with white background. Turn on
    // both horizontal and vertical grid lines with light grey color (0xc0c0c0)
    c->setPlotArea(45, 35, 240, 120, 0xffffff, -1, -1, 0xc0c0c0, -1);

    // Add a legend box at (45, 12) (top of the chart) using horizontal layout and 8pt Arial font
    // Set the background and border color to Transparent.
    c->addLegend(45, 12, false, "", 8)->setBackground(Chart::Transparent);

    // Add a title to the chart using 9pt Arial Bold/white font. Use a 1 x 2 bitmap pattern as the
    // background.
    int pattern1[] = {0x004000, 0x008000};
    c->addTitle("Server Load (Jun 01 - Jun 07)", "arialbd.ttf", 9, 0xffffff)->setBackground(
        c->patternColor(IntArray(pattern1, (int)(sizeof(pattern1) / sizeof(pattern1[0]))), 2));

    // Set the y axis label format to nn%
    c->yAxis()->setLabelFormat("{value}%");

    // Set the labels on the x axis
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Add a line layer to the chart
    LineLayer *layer = c->addLineLayer();

    // Add the first line. Plot the points with a 7 pixel square symbol
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0xcf4040, "Peak"
        )->setDataSymbol(Chart::SquareSymbol, 7);

    // Add the second line. Plot the points with a 9 pixel dismond symbol
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x40cf40,
        "Average")->setDataSymbol(Chart::DiamondSymbol, 9);

    // Enable data label on the data points. Set the label format to nn%.
    layer->setDataLabelFormat("{value|0}%");

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{xLabel}: {dataSetName} {value}%'");

    return c;
}


BaseChart *symbolline2(int /* chartIndex */, const char **imageMap)
{
    // The data for the line chart
    double data0[] = {42, 49, 33, 38, 64, 56, 29, 41, 44, 57, 59, 42};
    double data1[] = {65, 75, 47, 34, 42, 49, 73, 62, 90, 69, 66, 78};
    double data2[] = {36, 28, 25, 28, 38, 20, 22, 30, 25, 33, 30, 24};
    const char *labels[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct",
        "Nov", "Dec"};

    // Create a XYChart object of size 600 x 375 pixels
    XYChart *c = new XYChart(600, 375);

    // Add a title to the chart using 18pt Times Bold Italic font
    c->addTitle("Product Line Global Revenue", "timesbi.ttf", 18);

    // Set the plotarea at (50, 55) and of 500 x 280 pixels in size. Use a vertical gradient color
    // from light blue (f9f9ff) to sky blue (aaccff) as background. Set border to transparent and
    // grid lines to white (ffffff).
    c->setPlotArea(50, 55, 500, 280, c->linearGradientColor(0, 55, 0, 335, 0xf9fcff, 0xaaccff), -1,
        Chart::Transparent, 0xffffff);

    // Add a legend box at (50, 28) using horizontal layout. Use 10pt Arial Bold as font, with
    // transparent background.
    c->addLegend(50, 28, false, "arialbd.ttf", 10)->setBackground(Chart::Transparent);

    // Set the x axis labels
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Set y-axis tick density to 30 pixels. ChartDirector auto-scaling will use this as the
    // guideline when putting ticks on the y-axis.
    c->yAxis()->setTickDensity(30);

    // Set axis label style to 8pt Arial Bold
    c->xAxis()->setLabelStyle("arialbd.ttf", 8);
    c->yAxis()->setLabelStyle("arialbd.ttf", 8);

    // Set axis line width to 2 pixels
    c->xAxis()->setWidth(2);
    c->yAxis()->setWidth(2);

    // Add axis title using 10pt Arial Bold Italic font
    c->yAxis()->setTitle("Revenue in USD millions", "arialbi.ttf", 10);

    // Add a line layer to the chart
    LineLayer *layer = c->addLineLayer();

    // Set the line width to 3 pixels
    layer->setLineWidth(3);

    // Add the three data sets to the line layer, using circles, diamands and X shapes as symbols
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0xff0000,
        "Quantum Computer")->setDataSymbol(Chart::CircleSymbol, 9);
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x00ff00,
        "Atom Synthesizer")->setDataSymbol(Chart::DiamondSymbol, 11);
    layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), 0xff6600,
        "Proton Cannon")->setDataSymbol(Chart::Cross2Shape(), 11);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='Revenue of {dataSetName} in {xLabel}: US$ {value}M'");

    return c;
}


BaseChart *missingpoints(int /* chartIndex */, const char **imageMap)
{
    // The data for the chart
    double data0[] = {42, 49, Chart::NoValue, 38, 64, 56, 29, 41, 44, 57};
    double data1[] = {65, 75, 47, 34, 42, 49, 73, Chart::NoValue, 90, 69, 66, 78};
    double data2[] = {Chart::NoValue, Chart::NoValue, 25, 28, 38, 20, 22, Chart::NoValue, 25, 33,
        30, 24};
    const char *labels[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct",
        "Nov", "Dec"};

    // Create a XYChart object of size 600 x 360 pixels. Set background color to brushed silver,
    // with a 2 pixel 3D border. Use rounded corners.
    XYChart *c = new XYChart(600, 360, Chart::brushedSilverColor(), Chart::Transparent, 2);
    c->setRoundedFrame();

    // Add a title using 18pt Times New Roman Bold Italic font. #Set top/bottom margins to 6 pixels.
    TextBox *title = c->addTitle("Product Line Global Revenue", "timesbi.ttf", 18);
    title->setMargin(0, 0, 6, 6);

    // Add a separator line just under the title
    c->addLine(10, title->getHeight(), c->getWidth() - 11, title->getHeight(), Chart::LineColor);

    // Add a legend box where the top-center is anchored to the horizontal center of the chart, just
    // under the title. Use horizontal layout and 10 points Arial Bold font, and transparent
    // background and border.
    LegendBox *legendBox = c->addLegend(c->getWidth() / 2, title->getHeight(), false, "arialbd.ttf",
        10);
    legendBox->setAlignment(Chart::TopCenter);
    legendBox->setBackground(Chart::Transparent, Chart::Transparent);

    // Tentatively set the plotarea at (70, 75) and of 460 x 240 pixels in size. Use transparent
    // border and black (000000) grid lines
    c->setPlotArea(70, 75, 460, 240, -1, -1, Chart::Transparent, 0x000000, -1);

    // Set the x axis labels
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Show the same scale on the left and right y-axes
    c->syncYAxis();

    // Set y-axis tick density to 30 pixels. ChartDirector auto-scaling will use this as the
    // guideline when putting ticks on the y-axis.
    c->yAxis()->setTickDensity(30);

    // Set all axes to transparent
    c->xAxis()->setColors(Chart::Transparent);
    c->yAxis()->setColors(Chart::Transparent);
    c->yAxis2()->setColors(Chart::Transparent);

    // Set the x-axis margins to 15 pixels, so that the horizontal grid lines can extend beyond the
    // leftmost and rightmost vertical grid lines
    c->xAxis()->setMargin(15, 15);

    // Set axis label style to 8pt Arial Bold
    c->xAxis()->setLabelStyle("arialbd.ttf", 8);
    c->yAxis()->setLabelStyle("arialbd.ttf", 8);
    c->yAxis2()->setLabelStyle("arialbd.ttf", 8);

    // Add axis title using 10pt Arial Bold Italic font
    c->yAxis()->setTitle("Revenue in USD millions", "arialbi.ttf", 10);
    c->yAxis2()->setTitle("Revenue in USD millions", "arialbi.ttf", 10);

    // Add the first line. The missing data will be represented as gaps in the line (the default
    // behaviour)
    LineLayer *layer0 = c->addLineLayer();
    layer0->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0xff0000,
        "Quantum Computer")->setDataSymbol(Chart::GlassSphere2Shape, 11);
    layer0->setLineWidth(3);

    // Add the second line. The missing data will be represented by using dash lines to bridge the
    // gap
    LineLayer *layer1 = c->addLineLayer();
    layer1->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x00ff00,
        "Atom Synthesizer")->setDataSymbol(Chart::GlassSphere2Shape, 11);
    layer1->setLineWidth(3);
    layer1->setGapColor(c->dashLineColor(0x00ff00));

    // Add the third line. The missing data will be ignored - just join the gap with the original
    // line style.
    LineLayer *layer2 = c->addLineLayer();
    layer2->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), 0xff6600,
        "Proton Cannon")->setDataSymbol(Chart::GlassSphere2Shape, 11);
    layer2->setLineWidth(3);
    layer2->setGapColor(Chart::SameAsMainColor);

    // layout the legend so we can get the height of the legend box
    c->layoutLegend();

    // Adjust the plot area size, such that the bounding box (inclusive of axes) is 15 pixels from
    // the left edge, just under the legend box, 16 pixels from the right edge, and 25 pixels from
    // the bottom edge.
    c->packPlotArea(15, legendBox->getTopY() + legendBox->getHeight(), c->getWidth() - 16,
        c->getHeight() - 25);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='Revenue of {dataSetName} in {xLabel}: US$ {value}M'");

    return c;
}


BaseChart *unevenpoints(int /* chartIndex */, const char **imageMap)
{
    // Data points which more unevenly spaced in time
    double data0Y[] = {62, 69, 53, 58, 84, 76, 49, 61, 64, 77, 79};
    double data0X[] = {Chart::chartTime(2007, 1, 1), Chart::chartTime(2007, 1, 2), Chart::chartTime(
        2007, 1, 5), Chart::chartTime(2007, 1, 7), Chart::chartTime(2007, 1, 10), Chart::chartTime(
        2007, 1, 14), Chart::chartTime(2007, 1, 17), Chart::chartTime(2007, 1, 18),
        Chart::chartTime(2007, 1, 19), Chart::chartTime(2007, 1, 20), Chart::chartTime(2007, 1, 21)}
        ;

    // Data points which are evenly spaced in a certain time range
    double data1Y[] = {36, 25, 28, 38, 20, 30, 27, 35, 65, 60, 40, 73, 62, 90, 75, 72};
    double data1Start = Chart::chartTime(2007, 1, 1);
    double data1End = Chart::chartTime(2007, 1, 16);

    // Data points which are evenly spaced in another time range, in which the spacing is different
    // from the above series
    double data2Y[] = {25, 15, 30, 23, 32, 55, 45};
    double data2Start = Chart::chartTime(2007, 1, 9);
    double data2End = Chart::chartTime(2007, 1, 21);

    // Create a XYChart object of size 600 x 400 pixels. Use a vertical gradient color from light
    // blue (99ccff) to white (ffffff) spanning the top 100 pixels as background. Set border to grey
    // (888888). Use rounded corners. Enable soft drop shadow.
    XYChart *c = new XYChart(600, 400);
    c->setBackground(c->linearGradientColor(0, 0, 0, 100, 0x99ccff, 0xffffff), 0x888888);
    c->setRoundedFrame();
    c->setDropShadow();

    // Add a title using 18pt Times New Roman Bold Italic font. Set top margin to 16 pixels.
    c->addTitle("Product Line Order Backlog", "timesbi.ttf", 18)->setMargin(0, 0, 16, 0);

    // Set the plotarea at (60, 80) and of 510 x 275 pixels in size. Use transparent border and dark
    // grey (444444) dotted grid lines
    PlotArea *plotArea = c->setPlotArea(60, 80, 510, 275, -1, -1, Chart::Transparent,
        c->dashLineColor(0x444444, 0x0101), -1);

    // Add a legend box where the top-center is anchored to the horizontal center of the plot area
    // at y = 45. Use horizontal layout and 10 points Arial Bold font, and transparent background
    // and border.
    LegendBox *legendBox = c->addLegend(plotArea->getLeftX() + plotArea->getWidth() / 2, 45, false,
        "arialbd.ttf", 10);
    legendBox->setAlignment(Chart::TopCenter);
    legendBox->setBackground(Chart::Transparent, Chart::Transparent);

    // Set x-axis tick density to 75 pixels and y-axis tick density to 30 pixels. ChartDirector
    // auto-scaling will use this as the guidelines when putting ticks on the x-axis and y-axis.
    c->yAxis()->setTickDensity(30);
    c->xAxis()->setTickDensity(75);

    // Set all axes to transparent
    c->xAxis()->setColors(Chart::Transparent);
    c->yAxis()->setColors(Chart::Transparent);

    // Set the x-axis margins to 15 pixels, so that the horizontal grid lines can extend beyond the
    // leftmost and rightmost vertical grid lines
    c->xAxis()->setMargin(15, 15);

    // Set axis label style to 8pt Arial Bold
    c->xAxis()->setLabelStyle("arialbd.ttf", 8);
    c->yAxis()->setLabelStyle("arialbd.ttf", 8);
    c->yAxis2()->setLabelStyle("arialbd.ttf", 8);

    // Add axis title using 10pt Arial Bold Italic font
    c->yAxis()->setTitle("Backlog in USD millions", "arialbi.ttf", 10);

    // Add the first data series
    LineLayer *layer0 = c->addLineLayer();
    layer0->addDataSet(DoubleArray(data0Y, (int)(sizeof(data0Y) / sizeof(data0Y[0]))), 0xff0000,
        "Quantum Computer")->setDataSymbol(Chart::GlassSphere2Shape, 11);
    layer0->setXData(DoubleArray(data0X, (int)(sizeof(data0X) / sizeof(data0X[0]))));
    layer0->setLineWidth(3);

    // Add the second data series
    LineLayer *layer1 = c->addLineLayer();
    layer1->addDataSet(DoubleArray(data1Y, (int)(sizeof(data1Y) / sizeof(data1Y[0]))), 0x00ff00,
        "Atom Synthesizer")->setDataSymbol(Chart::GlassSphere2Shape, 11);
    layer1->setXData(data1Start, data1End);
    layer1->setLineWidth(3);

    // Add the third data series
    LineLayer *layer2 = c->addLineLayer();
    layer2->addDataSet(DoubleArray(data2Y, (int)(sizeof(data2Y) / sizeof(data2Y[0]))), 0xff6600,
        "Proton Cannon")->setDataSymbol(Chart::GlassSphere2Shape, 11);
    layer2->setXData(data2Start, data2End);
    layer2->setLineWidth(3);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='Backlog of {dataSetName} at {x|mm/dd/yyyy}: US$ {value}M'");

    return c;
}


BaseChart *splineline(int /* chartIndex */, const char **imageMap)
{
    // The data for the chart
    double data0[] = {32, 39, 23, 28, 41, 38, 26, 35, 29};
    double data1[] = {50, 55, 47, 34, 47, 53, 38, 40, 51};

    // The labels for the chart
    const char *labels[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8"};

    // Create a XYChart object of size 600 x 300 pixels, with a pale red (ffdddd) background, black
    // border, 1 pixel 3D border effect and rounded corners.
    XYChart *c = new XYChart(600, 300, 0xffdddd, 0x000000, 1);
    c->setRoundedFrame();

    // Set the plotarea at (55, 58) and of size 520 x 195 pixels, with white (ffffff) background.
    // Set horizontal and vertical grid lines to grey (cccccc).
    c->setPlotArea(55, 58, 520, 195, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);

    // Add a legend box at (55, 32) (top of the chart) with horizontal layout. Use 9pt Arial Bold
    // font. Set the background and border color to Transparent.
    c->addLegend(55, 32, false, "arialbd.ttf", 9)->setBackground(Chart::Transparent);

    // Add a title box to the chart using 15pt Times Bold Italic font. The title is in CDML and
    // includes embedded images for highlight. The text is white (ffffff) on a dark red (880000)
    // background, with soft lighting effect from the right side.
    c->addTitle(
        "<*block,valign=absmiddle*><*img=star.png*><*img=star.png*> Performance Enhancer "
        "<*img=star.png*><*img=star.png*><*/*>", "timesbi.ttf", 15, 0xffffff)->setBackground(
        0x880000, -1, Chart::softLighting(Chart::Right));

    // Add a title to the y axis
    c->yAxis()->setTitle("Energy Concentration (KJ per liter)");

    // Set the labels on the x axis
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Add a title to the x axis using CMDL
    c->xAxis()->setTitle("<*block,valign=absmiddle*><*img=clock.png*>  Elapsed Time (hour)<*/*>");

    // Set the axes width to 2 pixels
    c->xAxis()->setWidth(2);
    c->yAxis()->setWidth(2);

    // Add a spline layer to the chart
    SplineLayer *layer = c->addSplineLayer();

    // Set the default line width to 2 pixels
    layer->setLineWidth(2);

    // Add a data set to the spline layer, using blue (0000c0) as the line color, with yellow
    // (ffff00) circle symbols.
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x0000c0,
        "Target Group")->setDataSymbol(Chart::CircleSymbol, 9, 0xffff00);

    // Add a data set to the spline layer, using brown (982810) as the line color, with pink
    // (f040f0) diamond symbols.
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0x982810,
        "Control Group")->setDataSymbol(Chart::DiamondSymbol, 11, 0xf040f0);

    // Add a custom CDML text at the bottom right of the plot area as the logo
    c->addText(575, 250,
        "<*block,valign=absmiddle*><*img=small_molecule.png*> <*block*>"
        "<*font=timesbi.ttf,size=10,color=804040*>Molecular\nEngineering<*/*>")->setAlignment(
        Chart::BottomRight);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName} at t = {xLabel} hour: {value} KJ/liter'");

    return c;
}


BaseChart *stepline(int /* chartIndex */, const char **imageMap)
{
    // The data for the chart
    double dataY0[] = {4, 4.5, 5, 5.25, 5.75, 5.25, 5, 4.5, 4, 3, 2.5, 2.5};
    double dataX0[] = {Chart::chartTime(1997, 1, 1), Chart::chartTime(1998, 6, 25),
        Chart::chartTime(1999, 9, 6), Chart::chartTime(2000, 2, 6), Chart::chartTime(2000, 9, 21),
        Chart::chartTime(2001, 3, 4), Chart::chartTime(2001, 6, 8), Chart::chartTime(2002, 2, 4),
        Chart::chartTime(2002, 5, 19), Chart::chartTime(2002, 8, 16), Chart::chartTime(2002, 12, 1),
        Chart::chartTime(2003, 1, 1)};

    double dataY1[] = {7, 6.5, 6, 5, 6.5, 7, 6, 5.5, 5, 4, 3.5, 3.5};
    double dataX1[] = {Chart::chartTime(1997, 1, 1), Chart::chartTime(1997, 7, 1), Chart::chartTime(
        1997, 12, 1), Chart::chartTime(1999, 1, 15), Chart::chartTime(1999, 6, 9), Chart::chartTime(
        2000, 3, 3), Chart::chartTime(2000, 8, 13), Chart::chartTime(2001, 5, 5), Chart::chartTime(
        2001, 9, 16), Chart::chartTime(2002, 3, 16), Chart::chartTime(2002, 6, 1), Chart::chartTime(
        2003, 1, 1)};

    // Create a XYChart object of size 500 x 270 pixels, with a pale blue (e0e0ff) background, black
    // border, 1 pixel 3D border effect and rounded corners
    XYChart *c = new XYChart(600, 300, 0xe0e0ff, 0x000000, 1);
    c->setRoundedFrame();

    // Set the plotarea at (55, 60) and of size 520 x 200 pixels, with white (ffffff) background.
    // Set horizontal and vertical grid lines to grey (cccccc).
    c->setPlotArea(50, 60, 525, 200, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);

    // Add a legend box at (55, 32) (top of the chart) with horizontal layout. Use 9pt Arial Bold
    // font. Set the background and border color to Transparent.
    c->addLegend(55, 32, false, "arialbd.ttf", 9)->setBackground(Chart::Transparent);

    // Add a title box to the chart using 15pt Times Bold Italic font. The text is white (ffffff) on
    // a deep blue (000088) background, with soft lighting effect from the right side.
    c->addTitle("Long Term Interest Rates", "timesbi.ttf", 15, 0xffffff)->setBackground(0x000088,
        -1, Chart::softLighting(Chart::Right));

    // Set the y axis label format to display a percentage sign
    c->yAxis()->setLabelFormat("{value}%");

    // Add a red (ff0000) step line layer to the chart and set the line width to 2 pixels
    StepLineLayer *layer0 = c->addStepLineLayer(DoubleArray(dataY0, (int)(sizeof(dataY0) / sizeof(
        dataY0[0]))), 0xff0000, "Country AAA");
    layer0->setXData(DoubleArray(dataX0, (int)(sizeof(dataX0) / sizeof(dataX0[0]))));
    layer0->setLineWidth(2);

    // Add a blue (0000ff) step line layer to the chart and set the line width to 2 pixels
    StepLineLayer *layer1 = c->addStepLineLayer(DoubleArray(dataY1, (int)(sizeof(dataY1) / sizeof(
        dataY1[0]))), 0x0000ff, "Country BBB");
    layer1->setXData(DoubleArray(dataX1, (int)(sizeof(dataX1) / sizeof(dataX1[0]))));
    layer1->setLineWidth(2);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName} change to {value}% on {x|mmm dd, yyyy}'");

    return c;
}


BaseChart *linefill(int /* chartIndex */, const char **imageMap)
{
    // The data for the line chart
    double data0[] = {70, 73, 80, 90, 95, 93, 82, 77, 82, 101, 111, 115};
    double data1[] = {90, 96, 89, 77, 82, 96, 109, 109, 99, 108, 96, 91};
    double data2[] = {58, 34, 25, 49, 64, 10, 16, 40, 25, 49, 40, 22};

    // The labels for the line chart
    const char *labels[] = {"2008-01", "2008-02", "2008-03", "2008-04", "2008-05", "2008-06",
        "2008-07", "2008-08", "2008-09", "2008-10", "2008-11", "2008-12"};

    // Create a XYChart object of size 450 x 450 pixels
    XYChart *c = new XYChart(450, 450);

    // Add a title to the chart using 15pt Arial Italic font.
    TextBox *title = c->addTitle("Inter-line Coloring", "ariali.ttf", 15);

    // Add a legend box where the top-center is anchored to the horizontal center of the chart, just
    // under the title. Use horizontal layout and 10 points Arial Bold font, and transparent
    // background and border. Use line style legend key.
    LegendBox *legendBox = c->addLegend(c->getWidth() / 2, title->getHeight(), false, "arialbi.ttf",
        10);
    legendBox->setBackground(Chart::Transparent, Chart::Transparent);
    legendBox->setAlignment(Chart::TopCenter);
    legendBox->setLineStyleKey();

    // Tentatively set the plotarea at (70, 65) and of (chart_width - 100) x (chart_height - 110) in
    // size. Use light grey (c0c0c0) border and horizontal and vertical grid lines.
    PlotArea *plotArea = c->setPlotArea(70, 65, c->getWidth() - 100, c->getHeight() - 110, -1, -1,
        0xc0c0c0, 0xc0c0c0, -1);

    // Add a title to the y axis using 12pt Arial Bold Italic font
    c->yAxis()->setTitle("Axis Title Placeholder", "arialbi.ttf", 12);

    // Add a title to the x axis using 12pt Arial Bold Italic font
    c->xAxis()->setTitle("Axis Title Placeholder", "arialbi.ttf", 12);

    // Set the axes line width to 3 pixels
    c->xAxis()->setWidth(3);
    c->yAxis()->setWidth(3);

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Use 8 points Arial rotated by 90 degrees as the x-axis label font
    c->xAxis()->setLabelStyle("arial.ttf", 8, Chart::TextColor, 90);

    // Add a spline curve to the chart
    SplineLayer *layer0 = c->addSplineLayer(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]
        ))), 0xff0000, "Data Set 0");
    layer0->setLineWidth(2);

    // Add a normal line to the chart
    LineLayer *layer1 = c->addLineLayer(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))),
        0x008800, "Data Set 1");
    layer1->setLineWidth(2);

    // Color the region between the above spline curve and normal line. Use the semi-transparent red
    // (80ff000000) if the spline curve is higher than the normal line, otherwise use
    // semi-transparent green (80008800)
    c->addInterLineLayer(layer0->getLine(), layer1->getLine(), 0x80ff0000, 0x80008800);

    // Add another normal line to the chart
    LineLayer *layer2 = c->addLineLayer(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))),
        0x0000ff, "Data Set 2");
    layer2->setLineWidth(2);

    // Add a horizontal mark line to the chart at y = 40
    Mark *mark = c->yAxis()->addMark(40, -1, "Threshold");
    mark->setLineWidth(2);

    // Set the mark line to purple (880088) dash line. Use white (ffffff) for the mark label.
    mark->setMarkColor(c->dashLineColor(0x880088), 0xffffff);

    // Put the mark label at the left side of the mark, with a purple (880088) background.
    mark->setAlignment(Chart::Left);
    mark->setBackground(0x880088);

    // Color the region between the above normal line and mark line. Use the semi-transparent blue
    // (800000ff) if the normal line is higher than the mark line, otherwise use semi-transparent
    // purple (80880088)
    c->addInterLineLayer(layer2->getLine(), mark->getLine(), 0x800000ff, 0x80880088);

    // Layout the legend box, so we can get its height
    c->layoutLegend();

    // Adjust the plot area size, such that the bounding box (inclusive of axes) is 10 pixels from
    // the left edge, just under the legend box, 25 pixels from the right edge, and 10 pixels from
    // the bottom edge.
    c->packPlotArea(10, legendBox->getTopY() + legendBox->getHeight(), c->getWidth() - 25,
        c->getHeight() - 10);

    // After determining the exact plot area position, we may adjust the legend box and the title
    // positions so that they are centered relative to the plot area (instead of the chart)
    legendBox->setPos(plotArea->getLeftX() + (plotArea->getWidth() - legendBox->getWidth()) / 2,
        legendBox->getTopY());
    title->setPos(plotArea->getLeftX() + (plotArea->getWidth() - title->getWidth()) / 2,
        title->getTopY());

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{dataSetName} in {xLabel}: {value}'");

    return c;
}


BaseChart *linecompare(int /* chartIndex */, const char **imageMap)
{
    // The data for the upper and lower bounding lines
    double upperY[] = {60, 60, 100, 100, 60, 60};
    double lowerY[] = {40, 40, 80, 80, 40, 40};
    double zoneX[] = {0, 2.5, 3.5, 5.5, 6.5, 10};

    // The data for the spline curve
    double curveY[] = {50, 44, 54, 48, 58, 50, 90, 85, 104, 82, 96, 90, 74, 52, 35, 58, 46, 54, 48,
        52, 50};
    double curveX[] = {0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5, 9,
        9.5, 10};

    // Create a XYChart object of size 600 x 300 pixels, with a light grey (cccccc) background,
    // black border, and 1 pixel 3D border effect.
    XYChart *c = new XYChart(600, 300, 0xcccccc, 0x000000, 1);

    // Set the plotarea at (55, 58) and of size 520 x 195 pixels, with white background. Turn on
    // both horizontal and vertical grid lines with light grey color (cccccc)
    c->setPlotArea(55, 58, 520, 195, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);

    // Add a legend box at (55, 32) (top of the chart) with horizontal layout. Use 9pt Arial Bold
    // font. Set the background and border color to Transparent.
    c->addLegend(55, 32, false, "arialbd.ttf", 9)->setBackground(Chart::Transparent);

    // Add a title box to the chart using 15pt Times Bold Italic font. The title is in CDML and
    // includes embedded images for highlight. The text is white (ffffff) on a black background,
    // with a 1 pixel 3D border.
    c->addTitle(
        "<*block,valign=absmiddle*><*img=star.png*><*img=star.png*> Performance Enhancer "
        "<*img=star.png*><*img=star.png*><*/*>", "timesbi.ttf", 15, 0xffffff)->setBackground(
        0x000000, -1, 1);

    // Add a title to the y axis
    c->yAxis()->setTitle("Temperature");

    // Add a title to the x axis using CMDL
    c->xAxis()->setTitle("<*block,valign=absmiddle*><*img=clock.png*>  Elapsed Time (hour)<*/*>");

    // Set the axes width to 2 pixels
    c->xAxis()->setWidth(2);
    c->yAxis()->setWidth(2);

    // Add a purple (800080) spline layer to the chart with a line width of 2 pixels
    SplineLayer *splineLayer = c->addSplineLayer(DoubleArray(curveY, (int)(sizeof(curveY) / sizeof(
        curveY[0]))), 0x800080, "Molecular Temperature");
    splineLayer->setXData(DoubleArray(curveX, (int)(sizeof(curveX) / sizeof(curveX[0]))));
    splineLayer->setLineWidth(2);

    // Add a line layer to the chart with two dark green (338033) data sets, and a line width of 2
    // pixels
    LineLayer *lineLayer = c->addLineLayer();
    lineLayer->addDataSet(DoubleArray(upperY, (int)(sizeof(upperY) / sizeof(upperY[0]))), 0x338033,
        "Target Zone");
    lineLayer->addDataSet(DoubleArray(lowerY, (int)(sizeof(lowerY) / sizeof(lowerY[0]))), 0x338033);
    lineLayer->setXData(DoubleArray(zoneX, (int)(sizeof(zoneX) / sizeof(zoneX[0]))));
    lineLayer->setLineWidth(2);

    // Color the zone between the upper zone line and lower zone line as semi-transparent light
    // green (8099ff99)
    c->addInterLineLayer(lineLayer->getLine(0), lineLayer->getLine(1), 0x8099ff99, 0x8099ff99);

    // If the spline line gets above the upper zone line, color to area between the lines red
    // (ff0000)
    c->addInterLineLayer(splineLayer->getLine(0), lineLayer->getLine(0), 0xff0000,
        Chart::Transparent);

    // If the spline line gets below the lower zone line, color to area between the lines blue
    // (0000ff)
    c->addInterLineLayer(splineLayer->getLine(0), lineLayer->getLine(1), Chart::Transparent,
        0x0000ff);

    // Add a custom CDML text at the bottom right of the plot area as the logo
    c->addText(575, 250,
        "<*block,valign=absmiddle*><*img=small_molecule.png*> <*block*>"
        "<*font=timesbi.ttf,size=10,color=804040*>Molecular\nEngineering<*/*>")->setAlignment(
        Chart::BottomRight);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='Temperature at hour {x}: {value} C'");

    return c;
}


BaseChart *errline(int /* chartIndex */, const char **imageMap)
{
    // The data with error information
    double data[] = {42, 49, 33, 38, 51, 46, 29, 41, 44, 57, 59, 52, 37, 34, 51, 56, 56, 60, 70, 76,
        63, 67, 75, 64, 51};
    double errData[] = {5, 6, 5.1, 6.5, 6.6, 8, 5.4, 5.1, 4.6, 5.0, 5.2, 6.0, 4.9, 5.6, 4.8, 6.2,
        7.4, 7.1, 6.0, 6.6, 7.1, 5.3, 5.5, 7.9, 6.1};

    // The labels for the chart
    const char *labels[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12",
        "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24"};

    // Create a XYChart object of size 600 x 300 pixels, with a light grey (eeeeee) background,
    // black border, 1 pixel 3D border effect and rounded corners.
    XYChart *c = new XYChart(600, 300, 0xeeeeee, 0x000000, 1);
    c->setRoundedFrame();

    // Set the plotarea at (55, 55) and of size 520 x 195 pixels, with white (ffffff) background.
    // Set horizontal and vertical grid lines to grey (cccccc).
    c->setPlotArea(55, 55, 520, 195, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);

    // Add a title box to the chart using 15pt Times Bold Italic font. The title is in CDML and
    // includes embedded images for highlight. The text is on a light grey (dddddd) background, with
    // glass lighting effect.
    c->addTitle(
        "<*block,valign=absmiddle*><*img=star.png*><*img=star.png*> Molecular Temperature Control "
        "<*img=star.png*><*img=star.png*><*/*>", "timesbi.ttf", 15)->setBackground(0xdddddd, 0,
        Chart::glassEffect());

    // Add a title to the y axis
    c->yAxis()->setTitle("Temperature");

    // Add a title to the x axis using CMDL
    c->xAxis()->setTitle("<*block,valign=absmiddle*><*img=clock.png*>  Elapsed Time (hour)<*/*>");

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Display 1 out of 3 labels on the x-axis. Show minor ticks for remaining labels.
    c->xAxis()->setLabelStep(3, 1);

    // Set the axes width to 2 pixels
    c->xAxis()->setWidth(2);
    c->yAxis()->setWidth(2);

    // Add a line layer to the chart
    LineLayer *lineLayer = c->addLineLayer();

    // Add a blue (0xff) data set to the line layer, with yellow (0xffff80) diamond symbols
    lineLayer->addDataSet(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), 0x0000ff
        )->setDataSymbol(Chart::DiamondSymbol, 12, 0xffff80);

    // Set the line width to 2 pixels
    lineLayer->setLineWidth(2);

    // Add a box whisker layer to the chart. Use the upper and lower mark of the box whisker layer
    // to act as error zones. The upper and lower marks are computed using the ArrayMath object.
    BoxWhiskerLayer *errLayer = c->addBoxWhiskerLayer(DoubleArray(), DoubleArray(), ArrayMath(
        DoubleArray(data, (int)(sizeof(data) / sizeof(data[0])))).add(DoubleArray(errData, (int)(
        sizeof(errData) / sizeof(errData[0])))), ArrayMath(DoubleArray(data, (int)(sizeof(data) /
        sizeof(data[0])))).sub(DoubleArray(errData, (int)(sizeof(errData) / sizeof(errData[0])))),
        DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), Chart::Transparent, 0xbb6633);

    // Set the line width to 2 pixels
    errLayer->setLineWidth(2);

    // Set the error zone to occupy half the space between the symbols
    errLayer->setDataGap(0.5);

    // Add a custom CDML text at the bottom right of the plot area as the logo
    c->addText(575, 247,
        "<*block,valign=absmiddle*><*img=small_molecule.png*> <*block*>"
        "<*font=timesbi.ttf,size=10,color=804040*>Molecular\nEngineering<*/*>")->setAlignment(
        Chart::BottomRight);

    // Output the chart
    c->makeChart();

    // Include tool tip for the chart. We only need to show the tool tip for the box whisker layer.
    *imageMap = errLayer->getHTMLImageMap("clickable", "",
        "title='Temperature at hour {xLabel}: {med} +/- {={med}-{min}} C'");

    return c;
}


BaseChart *multisymbolline(int /* chartIndex */, const char **imageMap)
{
    // In this example, the data points are unevenly spaced on the x-axis
    double dataY[] = {4.7, 4.7, 6.6, 2.2, 4.7, 4.0, 4.0, 5.1, 4.5, 4.5, 6.8, 4.5, 4, 2.1, 3, 2.5,
        2.5, 3.1};
    double dataX[] = {Chart::chartTime(1999, 7, 1), Chart::chartTime(2000, 1, 1), Chart::chartTime(
        2000, 2, 1), Chart::chartTime(2000, 4, 1), Chart::chartTime(2000, 5, 8), Chart::chartTime(
        2000, 7, 5), Chart::chartTime(2001, 3, 5), Chart::chartTime(2001, 4, 7), Chart::chartTime(
        2001, 5, 9), Chart::chartTime(2002, 2, 4), Chart::chartTime(2002, 4, 4), Chart::chartTime(
        2002, 5, 8), Chart::chartTime(2002, 7, 7), Chart::chartTime(2002, 8, 30), Chart::chartTime(
        2003, 1, 2), Chart::chartTime(2003, 2, 16), Chart::chartTime(2003, 11, 6), Chart::chartTime(
        2004, 1, 4)};

    // Data points are assigned different symbols based on point type
    double pointType[] = {0, 1, 0, 1, 2, 1, 0, 0, 1, 1, 2, 2, 1, 0, 2, 1, 2, 0};

    // Create a XYChart object of size 480 x 320 pixels. Use a vertical gradient color from pale
    // blue (e8f0f8) to sky blue (aaccff) spanning half the chart height as background. Set border
    // to blue (88aaee). Use rounded corners. Enable soft drop shadow.
    XYChart *c = new XYChart(480, 320);
    c->setBackground(c->linearGradientColor(0, 0, 0, c->getHeight() / 2, 0xe8f0f8, 0xaaccff),
        0x88aaee);
    c->setRoundedFrame();
    c->setDropShadow();

    // Add a title to the chart using 15 points Arial Italic font. Set top/bottom margins to 12
    // pixels.
    TextBox *title = c->addTitle("Multi-Symbol Line Chart Demo", "ariali.ttf", 15);
    title->setMargin(0, 0, 12, 12);

    // Tentatively set the plotarea to 50 pixels from the left edge to allow for the y-axis, and to
    // just under the title. Set the width to 65 pixels less than the chart width, and the height to
    // reserve 90 pixels at the bottom for the x-axis and the legend box. Use pale blue (e8f0f8)
    // background, transparent border, and grey (888888) dotted horizontal and vertical grid lines.
    c->setPlotArea(50, title->getHeight(), c->getWidth() - 65, c->getHeight() - title->getHeight() -
        90, 0xe8f0f8, -1, Chart::Transparent, c->dashLineColor(0x888888, Chart::DotLine), -1);

    // Add a legend box where the bottom-center is anchored to the 12 pixels above the bottom-center
    // of the chart. Use horizontal layout and 8 points Arial font.
    LegendBox *legendBox = c->addLegend(c->getWidth() / 2, c->getHeight() - 12, false,
        "arialbd.ttf", 8);
    legendBox->setAlignment(Chart::BottomCenter);

    // Set the legend box background and border to pale blue (e8f0f8) and bluish grey (445566)
    legendBox->setBackground(0xe8f0f8, 0x445566);

    // Use rounded corners of 5 pixel radius for the legend box
    legendBox->setRoundedCorners(5);

    // Set the y axis label format to display a percentage sign
    c->yAxis()->setLabelFormat("{value}%");

    // Set y-axis title to use 10 points Arial Bold Italic font
    c->yAxis()->setTitle("Axis Title Placeholder", "arialbi.ttf", 10);

    // Set axis labels to use Arial Bold font
    c->yAxis()->setLabelStyle("arialbd.ttf");
    c->xAxis()->setLabelStyle("arialbd.ttf");

    // We add the different data symbols using scatter layers. The scatter layers are added before
    // the line layer to make sure the data symbols stay on top of the line layer.

    // We select the points with pointType = 0 (the non-selected points will be set to NoValue), and
    // use yellow (ffff00) 15 pixels high 5 pointed star shape symbols for the points. (This example
    // uses both x and y coordinates. For charts that have no x explicitly coordinates, use an empty
    // array as dataX.)
    c->addScatterLayer(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0]))), ArrayMath(
        DoubleArray(dataY, (int)(sizeof(dataY) / sizeof(dataY[0])))).selectEQZ(DoubleArray(
        pointType, (int)(sizeof(pointType) / sizeof(pointType[0]))), Chart::NoValue),
        "Point Type 0", Chart::StarShape(5), 15, 0xffff00);

    // Similar to above, we select the points with pointType - 1 = 0 and use green (ff00) 13 pixels
    // high six-sided polygon as symbols.
    c->addScatterLayer(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0]))), ArrayMath(
        DoubleArray(dataY, (int)(sizeof(dataY) / sizeof(dataY[0])))).selectEQZ(ArrayMath(
        DoubleArray(pointType, (int)(sizeof(pointType) / sizeof(pointType[0])))).sub(1),
        Chart::NoValue), "Point Type 1", Chart::PolygonShape(6), 13, 0x00ff00);

    // Similar to above, we select the points with pointType - 2 = 0 and use red (ff0000) 13 pixels
    // high X shape as symbols.
    c->addScatterLayer(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0]))), ArrayMath(
        DoubleArray(dataY, (int)(sizeof(dataY) / sizeof(dataY[0])))).selectEQZ(ArrayMath(
        DoubleArray(pointType, (int)(sizeof(pointType) / sizeof(pointType[0])))).sub(2),
        Chart::NoValue), "Point Type 2", Chart::Cross2Shape(), 13, 0xff0000);

    // Finally, add a blue (0000ff) line layer with line width of 2 pixels
    LineLayer *layer = c->addLineLayer(DoubleArray(dataY, (int)(sizeof(dataY) / sizeof(dataY[0]))),
        0x0000ff);
    layer->setXData(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0]))));
    layer->setLineWidth(2);

    // Adjust the plot area size, such that the bounding box (inclusive of axes) is 10 pixels from
    // the left edge, just below the title, 25 pixels from the right edge, and 8 pixels above the
    // legend box.
    c->packPlotArea(10, title->getHeight(), c->getWidth() - 25, c->layoutLegend()->getTopY() - 8);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{x|mmm dd, yyyy}: {value}%'");

    return c;
}


BaseChart *binaryseries(int /* chartIndex */, const char ** /* imageMap */)
{
    // The data for the chart
    double dataY[] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1};
    double dataX[] = {Chart::chartTime(2008, 7, 1, 0, 0, 0), Chart::chartTime(2008, 7, 1, 2, 17, 2),
        Chart::chartTime(2008, 7, 1, 8, 5, 30), Chart::chartTime(2008, 7, 1, 10, 54, 10),
        Chart::chartTime(2008, 7, 1, 15, 40, 0), Chart::chartTime(2008, 7, 1, 18, 22, 20),
        Chart::chartTime(2008, 7, 1, 22, 17, 14), Chart::chartTime(2008, 7, 2, 2, 55, 50),
        Chart::chartTime(2008, 7, 2, 8, 17, 14), Chart::chartTime(2008, 7, 2, 11, 55, 50),
        Chart::chartTime(2008, 7, 2, 13, 17, 14), Chart::chartTime(2008, 7, 2, 17, 55, 50),
        Chart::chartTime(2008, 7, 2, 20, 17, 14), Chart::chartTime(2008, 7, 3, 0, 0, 0)};

    // In this example, we only use position 1, 3, 5 for the data series. Positions 0, 2, 4, 6 are
    // empty and serve as gaps.
    const char *labels[] = {"", "ON Only Filling", "",
        "<*font,color=cc2200*>ON<*/font*> / <*font,color=00aa22*>OFF<*/font*> Filling", "",
        "Logic Line", ""};

    // Create a XYChart object of size 600 x 180 pixels
    XYChart *c = new XYChart(600, 180);

    // Add a title to the chart using 10 points Arial Bold font. Set top/bottom margins to 12
    // pixels.
    TextBox *title = c->addTitle("Binary Data Series Demonstration", "arialbd.ttf", 10);

    // Tentatively set the plotarea at (100, 30) and of size 470 x 120 pixels. Use transparent
    // border. Use grey (888888) solid line and light grey (ccccc) dotted line as major and minor
    // vertical grid lines.
    c->setPlotArea(100, 30, 470, 120, -1, -1, Chart::Transparent)->setGridColor(Chart::Transparent,
        0x888888, Chart::Transparent, c->dashLineColor(0xcccccc, Chart::DotLine));

    // Set axes to transparent
    c->xAxis()->setColors(Chart::Transparent);
    c->yAxis()->setColors(Chart::Transparent);

    // Set the y axis labels
    c->yAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Set y-axis label style to 8pt Arial Bold
    c->yAxis()->setLabelStyle("arialbd.ttf", 8);

    // Set x-axis major and minor tick density to 50 and 5 pixels. ChartDirector auto-scaling will
    // use this as the guideline when putting ticks on the x-axis.
    c->xAxis()->setTickDensity(50, 5);

    // Use "<*font=Arial Bold*>{value|mmm dd}" for the first label of an hour, and "{value|hh:nn}"
    // for all other labels.
    c->xAxis()->setMultiFormat(Chart::StartOfDayFilter(), "<*font=arialbd.ttf*>{value|mmm dd}",
        Chart::AllPassFilter(), "{value|hh:nn}");

    //
    // A Logic Line can be achieved using a StepLineLayer in ChartDirector
    //

    // Shift the data by 4.5, so instead of 0 - 1, it is now 4.5 to 5.5, or fluctuate around the y =
    // 5 (Logic Line label) position.
    ArrayMath shiftedLine0 = ArrayMath(DoubleArray(dataY, (int)(sizeof(dataY) / sizeof(dataY[0])))
        ).add(4.5);

    // Add step lines using the original and the reversed data
    StepLineLayer *layer0 = c->addStepLineLayer(shiftedLine0, 0x0000ff);
    layer0->setXData(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0]))));

    //
    // To perform ON/OFF filling, we draw the logic line, and its reverse, and fill the region in
    // between
    //

    // Shift the data by 2.5, so instead of 0 - 1, it is now 2.5 to 3.5, or fluctuate around the y =
    // 3 (ON/OFF Filing label) position.
    ArrayMath shiftedLine1 = ArrayMath(DoubleArray(dataY, (int)(sizeof(dataY) / sizeof(dataY[0])))
        ).add(2.5);
    // Reverse the data, so the 0 becomes 1 and 1 becomes 0, and shift it as well.
    ArrayMath reverseShiftedLine1 = ArrayMath(DoubleArray(dataY, (int)(sizeof(dataY) / sizeof(dataY[
        0])))).mul(-1).add(3.5);

    // Add step lines using the original and the reversed data
    StepLineLayer *layer1 = c->addStepLineLayer(shiftedLine1, Chart::Transparent);
    layer1->addDataSet(reverseShiftedLine1, Chart::Transparent);
    layer1->setXData(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0]))));

    // Fill the region between the two step lines with green (00aa22) or red (cc2200), depending on
    // whether the original or the reserve is higher.
    c->addInterLineLayer(layer1->getLine(0), layer1->getLine(1), 0x00aa22, 0xcc2200);

    //
    // The ON Only filling is the same as ON/OFF filling, except the OFF filling color is
    // transparent
    //

    // Shift the data by 0.5, so instead of 0 - 1, it is now 0.5 to 1.5, or fluctuate around the y =
    // 1 (ON Only Filing label) position.
    ArrayMath shiftedLine2 = ArrayMath(DoubleArray(dataY, (int)(sizeof(dataY) / sizeof(dataY[0])))
        ).add(0.5);
    // Reverse the data, so the 0 becomes 1 and 1 becomes 0, and shift it as well.
    ArrayMath reverseShiftedLine2 = ArrayMath(DoubleArray(dataY, (int)(sizeof(dataY) / sizeof(dataY[
        0])))).mul(-1).add(1.5);

    // Add step lines using the original and the reversed data
    StepLineLayer *layer2 = c->addStepLineLayer(shiftedLine2, Chart::Transparent);
    layer2->addDataSet(reverseShiftedLine2, Chart::Transparent);
    layer2->setXData(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0]))));

    // Fill the region between the two step lines with green (00aa22) or transparent, depending on
    // whether the original or the reserve is higher.
    c->addInterLineLayer(layer2->getLine(0), layer2->getLine(1), 0x00aa22, Chart::Transparent);

    // Adjust the plot area size, such that the bounding box (inclusive of axes) is 10 pixels from
    // the left edge, 10 pixels  below the title, 30 pixels from the right edge, and 10 pixels above
    // the bottom edge.
    c->packPlotArea(10, title->getHeight() + 10, c->getWidth() - 30, c->getHeight() - 10);

    // Output the chart
    c->makeChart();

    return c;
}


BaseChart *customsymbolline(int /* chartIndex */, const char **imageMap)
{
    // The data for the chart
    double data0[] = {600, 800, 1200, 1500, 1800, 1900, 2000, 1950};
    double data1[] = {300, 450, 500, 1000, 1500, 1600, 1650, 1600};

    // The labels for the chart
    const char *labels[] = {"1995", "1996", "1997", "1998", "1999", "2000", "2001", "2002"};

    // Create a XYChart object of size 450 x 250 pixels, with a pale yellow (0xffffc0) background, a
    // black border, and 1 pixel 3D border effect.
    XYChart *c = new XYChart(450, 250, 0xffffc0, 0, 1);

    // Set the plotarea at (60, 45) and of size 360 x 170 pixels, using white (0xffffff) as the plot
    // area background color. Turn on both horizontal and vertical grid lines with light grey color
    // (0xc0c0c0)
    c->setPlotArea(60, 45, 360, 170, 0xffffff, -1, -1, 0xc0c0c0, -1);

    // Add a legend box at (60, 20) (top of the chart) with horizontal layout. Use 8pt Arial Bold
    // font. Set the background and border color to Transparent.
    c->addLegend(60, 20, false, "arialbd.ttf", 8)->setBackground(Chart::Transparent);

    // Add a title to the chart using 12pt Arial Bold/white font. Use a 1 x 2 bitmap pattern as the
    // background.
    int pattern1[] = {0x000040, 0x000080};
    c->addTitle("Information Resource Usage", "arialbd.ttf", 12, 0xffffff)->setBackground(
        c->patternColor(IntArray(pattern1, (int)(sizeof(pattern1) / sizeof(pattern1[0]))), 2));

    // Set the labels on the x axis
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Reserve 8 pixels margins at both side of the x axis to avoid the first and last symbols
    // drawing outside of the plot area
    c->xAxis()->setMargin(8, 8);

    // Add a title to the y axis
    c->yAxis()->setTitle("Population");

    // Add a line layer to the chart
    LineLayer *layer = c->addLineLayer();

    // Add the first line using small_user.png as the symbol.
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0xcf4040, "Users"
        )->setDataSymbol("small_user.png");

    // Add the first line using small_computer.png as the symbol.
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x40cf40,
        "Computers")->setDataSymbol("small_computer.png");

    // Set the line width to 3 pixels
    layer->setLineWidth(3);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='Number of {dataSetName} at {xLabel}: {value}'");

    return c;
}


BaseChart *rotatedline(int /* chartIndex */, const char **imageMap)
{
    //
    //    We use a random table to simulate generating 12 months of data
    //

    // Create the random table object with 4 cols * 12 rows, using 3 as seed
    RanTable *rantable = new RanTable(3, 4, 12);

    // Set the 1st column to be the 12 months of year 2002
    rantable->setDateCol(0, Chart::chartTime(2002, 1, 1), 86400 * 30);

    // Set the 2nd, 3rd and 4th columns to be random numbers starting from 125, 75, and 100
    // respectively. The change between rows is set to -35 to + 35. The minimum value of any cell is
    // 0.
    rantable->setCol(1, 125, -35, 35, 0);
    rantable->setCol(2, 75, -35, 35, 0);
    rantable->setCol(3, 100, -35, 35, 0);

    // Get the 1st column (time) as the x data
    DoubleArray dataX = rantable->getCol(0);

    // Get the 2nd, 3rd and 4th columns as 3 data sets
    DoubleArray dataY0 = rantable->getCol(1);
    DoubleArray dataY1 = rantable->getCol(2);
    DoubleArray dataY2 = rantable->getCol(3);

    // Create a XYChart object of size 360 x 400 pixels
    XYChart *c = new XYChart(360, 400);

    // Add a title to the chart
    c->addTitle("<*underline=2*>Rotated Line Chart Demo", "timesbi.ttf", 14);

    // Set the plotarea at (60, 75) and of size 190 x 320 pixels. Turn on both horizontal and
    // vertical grid lines with light grey color (0xc0c0c0)
    c->setPlotArea(60, 75, 190, 320)->setGridColor(0xc0c0c0, 0xc0c0c0);

    // Add a legend box at (270, 75)
    c->addLegend(270, 75);

    // Swap the x and y axis to become a rotated chart
    c->swapXY();

    // Set the y axis on the top side (right + rotated = top)
    c->setYAxisOnRight();

    // Add a title to the y axis
    c->yAxis()->setTitle("Throughput (MBytes)");

    // Reverse the x axis so it is pointing downwards
    c->xAxis()->setReverse();

    // Add a line chart layer using the given data
    LineLayer *layer = c->addLineLayer();
    layer->setXData(dataX);
    layer->addDataSet(dataY0, 0xff0000, "Server A");
    layer->addDataSet(dataY1, 0x338033, "Server B");
    layer->addDataSet(dataY2, 0x0000ff, "Server C");

    // Set the line width to 2 pixels
    layer->setLineWidth(2);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='[{dataSetName}] {x|mm/yyyy}: {value|0} MByte'");

    //free up resources
    delete rantable;

    return c;
}


BaseChart *xyline(int /* chartIndex */, const char **imageMap)
{
    // The (x, y) data for the first line
    double dataX0[] = {20, 90, 40, 30, 12};
    double dataY0[] = {10, 40, 75, 54, 20};

    // The (x, y) data for the second line
    double dataX1[] = {10, 40, 75, 54, 60};
    double dataY1[] = {50, 90, 40, 30, 10};

    // Create a XYChart object of size 450 x 450 pixels
    XYChart *c = new XYChart(450, 450);

    // Set the plotarea at (55, 65) and of size 350 x 300 pixels, with white background and a light
    // grey border (0xc0c0c0). Turn on both horizontal and vertical grid lines with light grey color
    // (0xc0c0c0)
    c->setPlotArea(55, 65, 350, 300, 0xffffff, -1, 0xc0c0c0, 0xc0c0c0, -1);

    // Add a legend box at (50, 30) (top of the chart) with horizontal layout. Use 12pt Times Bold
    // Italic font. Set the background and border color to Transparent.
    c->addLegend(50, 30, false, "timesbi.ttf", 12)->setBackground(Chart::Transparent);

    // Add a title to the chart using 18pt Times Bold Itatic font
    c->addTitle("Reaction Path", "timesbi.ttf", 18);

    // Add a title to the y axis using 12pt Arial Bold Italic font
    c->yAxis()->setTitle("Temperature (Celcius)", "arialbi.ttf", 12);

    // Set the y axis line width to 3 pixels
    c->yAxis()->setWidth(3);

    // Add a title to the x axis using 12pt Arial Bold Italic font
    c->xAxis()->setTitle("Pressure (Pa)", "arialbi.ttf", 12);

    // Set the x axis line width to 3 pixels
    c->xAxis()->setWidth(3);

    // Add a red (0xff3333) line layer using dataX0 and dataY0
    LineLayer *layer1 = c->addLineLayer(DoubleArray(dataY0, (int)(sizeof(dataY0) / sizeof(dataY0[0])
        )), 0xff3333, "Compound AAA");
    layer1->setXData(DoubleArray(dataX0, (int)(sizeof(dataX0) / sizeof(dataX0[0]))));

    // Set the line width to 3 pixels
    layer1->setLineWidth(3);

    // Use 9 pixel square symbols for the data points
    layer1->getDataSet(0)->setDataSymbol(Chart::SquareSymbol, 9);

    // Add custom text labels to the first and last point on the scatter plot using Arial Bold font
    layer1->addCustomDataLabel(0, 0, "Start", "arialbd.ttf");
    layer1->addCustomDataLabel(0, 4, "End", "arialbd.ttf");

    // Add a green (0x33ff33) line layer using dataX1 and dataY1
    LineLayer *layer2 = c->addLineLayer(DoubleArray(dataY1, (int)(sizeof(dataY1) / sizeof(dataY1[0])
        )), 0x33ff33, "Compound BBB");
    layer2->setXData(DoubleArray(dataX1, (int)(sizeof(dataX1) / sizeof(dataX1[0]))));

    // Set the line width to 3 pixels
    layer2->setLineWidth(3);

    // Use 11 pixel diamond symbols for the data points
    layer2->getDataSet(0)->setDataSymbol(Chart::DiamondSymbol, 11);

    // Add custom text labels to the first and last point on the scatter plot using Arial Bold font
    layer2->addCustomDataLabel(0, 0, "Start", "arialbd.ttf");
    layer2->addCustomDataLabel(0, 4, "End", "arialbd.ttf");

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='[{dataSetName}] Pressure = {x} Pa, Temperature = {value} C'");

    return c;
}


BaseChart *trendline(int /* chartIndex */, const char **imageMap)
{
    // The data for the line chart
    double data[] = {50, 55, 47, 34, 42, 49, 63, 62, 73, 59, 56, 50, 64, 60, 67, 67, 58, 59, 73, 77,
        84, 82, 80, 91};

    // The labels for the line chart
    const char *labels[] = {"Jan 2000", "Feb 2000", "Mar 2000", "Apr 2000", "May 2000", "Jun 2000",
        "Jul 2000", "Aug 2000", "Sep 2000", "Oct 2000", "Nov 2000", "Dec 2000", "Jan 2001",
        "Feb 2001", "Mar 2001", "Apr 2001", "May 2001", "Jun 2001", "Jul 2001", "Aug 2001",
        "Sep 2001", "Oct 2001", "Nov 2001", "Dec 2001"};

    // Create a XYChart object of size 500 x 320 pixels, with a pale purpule (0xffccff) background,
    // a black border, and 1 pixel 3D border effect.
    XYChart *c = new XYChart(500, 320, 0xffccff, 0x000000, 1);

    // Set the plotarea at (55, 45) and of size 420 x 210 pixels, with white background. Turn on
    // both horizontal and vertical grid lines with light grey color (0xc0c0c0)
    c->setPlotArea(55, 45, 420, 210, 0xffffff, -1, -1, 0xc0c0c0, -1);

    // Add a legend box at (55, 25) (top of the chart) with horizontal layout. Use 8pt Arial font.
    // Set the background and border color to Transparent.
    c->addLegend(55, 22, false, "", 8)->setBackground(Chart::Transparent);

    // Add a title box to the chart using 13pt Times Bold Italic font. The text is white (0xffffff)
    // on a purple (0x800080) background, with a 1 pixel 3D border.
    c->addTitle("Long Term Server Load", "timesbi.ttf", 13, 0xffffff)->setBackground(0x800080, -1, 1
        );

    // Add a title to the y axis
    c->yAxis()->setTitle("MBytes");

    // Set the labels on the x axis. Rotate the font by 90 degrees.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0])))
        )->setFontAngle(90);

    // Add a line layer to the chart
    LineLayer *lineLayer = c->addLineLayer();

    // Add the data to the line layer using light brown color (0xcc9966) with a 7 pixel square
    // symbol
    lineLayer->addDataSet(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), 0xcc9966,
        "Server Utilization")->setDataSymbol(Chart::SquareSymbol, 7);

    // Set the line width to 2 pixels
    lineLayer->setLineWidth(2);

    // tool tip for the line layer
    lineLayer->setHTMLImageMap("", "", "title='{xLabel}: {value} MBytes'");

    // Add a trend line layer using the same data with a dark green (0x008000) color. Set the line
    // width to 2 pixels
    TrendLayer *trendLayer = c->addTrendLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0])
        )), 0x008000, "Trend Line");
    trendLayer->setLineWidth(2);

    // tool tip for the trend layer
    trendLayer->setHTMLImageMap("", "", "title='Change rate: {slope|2} MBytes/per month'");

    // Output the chart
    c->makeChart();

    // include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable");

    return c;
}


BaseChart *scattertrend(int /* chartIndex */, const char **imageMap)
{
    // The XY data of the first data series
    double dataX0[] = {50, 55, 37, 24, 42, 49, 63, 72, 83, 59};
    double dataY0[] = {3.6, 2.8, 2.5, 2.3, 3.8, 3.0, 3.8, 5.0, 6.0, 3.3};

    // The XY data of the second data series
    double dataX1[] = {50, 55, 37, 24, 42, 49, 63, 72, 83, 59};
    double dataY1[] = {1.6, 1.8, 0.8, 0.5, 1.3, 1.5, 2.3, 2.4, 2.9, 1.5};

    // Tool tip formats for data points and trend lines
    const char *scatterToolTip = "title='{dataSetName}: Response time at {x} TPS: {value} sec'";
    const char *trendToolTip = "title='Slope = {slope|4} sec/TPS; Intercept = {intercept|4} sec'";

    // Create a XYChart object of size 450 x 420 pixels
    XYChart *c = new XYChart(450, 420);

    // Set the plotarea at (55, 65) and of size 350 x 300 pixels, with white background and a light
    // grey border (0xc0c0c0). Turn on both horizontal and vertical grid lines with light grey color
    // (0xc0c0c0)
    c->setPlotArea(55, 65, 350, 300, 0xffffff, -1, 0xc0c0c0, 0xc0c0c0, -1);

    // Add a legend box at (50, 30) (top of the chart) with horizontal layout. Use 12pt Times Bold
    // Italic font. Set the background and border color to Transparent.
    c->addLegend(50, 30, false, "timesbi.ttf", 12)->setBackground(Chart::Transparent);

    // Add a title to the chart using 18 point Times Bold Itatic font.
    c->addTitle("Server Performance", "timesbi.ttf", 18);

    // Add titles to the axes using 12pt Arial Bold Italic font
    c->yAxis()->setTitle("Response Time (sec)", "arialbi.ttf", 12);
    c->xAxis()->setTitle("Server Load (TPS)", "arialbi.ttf", 12);

    // Set the axes line width to 3 pixels
    c->yAxis()->setWidth(3);
    c->xAxis()->setWidth(3);

    // Add a scatter layer using (dataX0, dataY0)
    ScatterLayer *scatter1 = c->addScatterLayer(DoubleArray(dataX0, (int)(sizeof(dataX0) / sizeof(
        dataX0[0]))), DoubleArray(dataY0, (int)(sizeof(dataY0) / sizeof(dataY0[0]))), "Server AAA",
        Chart::DiamondSymbol, 11, 0x008000);
    scatter1->setHTMLImageMap("", "", scatterToolTip);

    // Add a trend line layer for (dataX0, dataY0)
    TrendLayer *trend1 = c->addTrendLayer(DoubleArray(dataX0, (int)(sizeof(dataX0) / sizeof(dataX0[0
        ]))), DoubleArray(dataY0, (int)(sizeof(dataY0) / sizeof(dataY0[0]))), 0x008000);
    trend1->setLineWidth(3);
    trend1->setHTMLImageMap("", "", trendToolTip);

    // Add a scatter layer for (dataX1, dataY1)
    ScatterLayer *scatter2 = c->addScatterLayer(DoubleArray(dataX1, (int)(sizeof(dataX1) / sizeof(
        dataX1[0]))), DoubleArray(dataY1, (int)(sizeof(dataY1) / sizeof(dataY1[0]))), "Server BBB",
        Chart::TriangleSymbol, 9, 0x6666ff);
    scatter2->setHTMLImageMap("", "", scatterToolTip);

    // Add a trend line layer for (dataX1, dataY1)
    TrendLayer *trend2 = c->addTrendLayer(DoubleArray(dataX1, (int)(sizeof(dataX1) / sizeof(dataX1[0
        ]))), DoubleArray(dataY1, (int)(sizeof(dataY1) / sizeof(dataY1[0]))), 0x6666ff);
    trend2->setLineWidth(3);
    trend2->setHTMLImageMap("", "", trendToolTip);

    // Output the chart
    c->makeChart();

    // include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable");

    return c;
}


BaseChart *confidenceband(int /* chartIndex */, const char **imageMap)
{
    char buffer[256];

    // The XY data of the first data series
    double dataX[] = {50, 55, 37, 24, 42, 49, 63, 72, 83, 59};
    double dataY[] = {3.6, 2.8, 2.5, 2.3, 3.8, 3.0, 3.8, 5.0, 6.0, 3.3};

    // Create a XYChart object of size 450 x 420 pixels
    XYChart *c = new XYChart(450, 420);

    // Set the plotarea at (55, 65) and of size 350 x 300 pixels, with white background and a light
    // grey border (0xc0c0c0). Turn on both horizontal and vertical grid lines with light grey color
    // (0xc0c0c0)
    c->setPlotArea(55, 65, 350, 300, 0xffffff, -1, 0xc0c0c0, 0xc0c0c0, -1);

    // Add a title to the chart using 18 point Times Bold Itatic font.
    c->addTitle("Server Performance", "timesbi.ttf", 18);

    // Add titles to the axes using 12pt Arial Bold Italic font
    c->yAxis()->setTitle("Response Time (sec)", "arialbi.ttf", 12);
    c->xAxis()->setTitle("Server Load (TPS)", "arialbi.ttf", 12);

    // Set the axes line width to 3 pixels
    c->yAxis()->setWidth(3);
    c->xAxis()->setWidth(3);

    // Add a scatter layer using (dataX, dataY)
    ScatterLayer *scatterLayer = c->addScatterLayer(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(
        dataX[0]))), DoubleArray(dataY, (int)(sizeof(dataY) / sizeof(dataY[0]))), "",
        Chart::DiamondSymbol, 11, 0x008000);

    // tool tip for scatter layer
    scatterLayer->setHTMLImageMap("", "", "title='Response time at {x} TPS: {value} sec'");

    // Add a trend line layer for (dataX, dataY)
    TrendLayer *trendLayer = c->addTrendLayer(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[
        0]))), DoubleArray(dataY, (int)(sizeof(dataY) / sizeof(dataY[0]))), 0x008000);

    // Set the line width to 3 pixels
    trendLayer->setLineWidth(3);

    // Add a 95% confidence band for the line
    trendLayer->addConfidenceBand(0.95, 0x806666ff);

    // Add a 95% confidence band (prediction band) for the points
    trendLayer->addPredictionBand(0.95, 0x8066ff66);

    // tool tip for trend layer
    trendLayer->setHTMLImageMap("", "",
        "title='Slope = {slope|4} sec/TPS; Intercept = {intercept|4} sec'");

    // Add a legend box at (50, 30) (top of the chart) with horizontal layout. Use 10pt Arial Bold
    // Italic font. Set the background and border color to Transparent.
    LegendBox *legendBox = c->addLegend(50, 30, false, "arialbi.ttf", 10);
    legendBox->setBackground(Chart::Transparent);

    // Add entries to the legend box
    legendBox->addKey("95% Line Confidence", 0x806666ff);
    legendBox->addKey("95% Point Confidence", 0x8066ff66);

    // Display the trend line parameters as a text table formatted using CDML
    sprintf(buffer,
        "<*block*>Slope\nIntercept\nCorrelation\nStd Error<*/*>   <*block*>%.4f sec/tps\n%.4f sec\n"
        "%.4f\n%.4f sec<*/*>", trendLayer->getSlope(), trendLayer->getIntercept(),
        trendLayer->getCorrelation(), trendLayer->getStdError());
    TextBox *textbox = c->addText(56, 65, buffer, "arialbd.ttf", 8);

    // Set the background of the text box to light grey, with a black border, and 1 pixel 3D border
    textbox->setBackground(0xc0c0c0, 0, 1);

    // Output the chart
    c->makeChart();

    // include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable");

    return c;
}


BaseChart *paramcurve(int /* chartIndex */, const char **imageMap)
{
    // The XY data of the first data series
    double dataX0[] = {10, 35, 17, 4, 22, 29, 45, 52, 63, 39};
    double dataY0[] = {2.0, 3.2, 2.7, 1.2, 2.8, 2.9, 3.1, 3.0, 2.3, 3.3};

    // The XY data of the second data series
    double dataX1[] = {30, 35, 17, 4, 22, 59, 43, 52, 63, 39};
    double dataY1[] = {1.0, 1.3, 0.7, 0.6, 0.8, 3.0, 1.8, 2.3, 3.4, 1.5};

    // The XY data of the third data series
    double dataX2[] = {28, 35, 15, 10, 22, 60, 46, 64, 39};
    double dataY2[] = {2.0, 2.2, 1.2, 0.4, 1.8, 2.7, 2.4, 2.8, 2.4};

    // Create a XYChart object of size 540 x 480 pixels
    XYChart *c = new XYChart(540, 480);

    // Set the plotarea at (70, 65) and of size 400 x 350 pixels, with white background and a light
    // grey border (0xc0c0c0). Turn on both horizontal and vertical grid lines with light grey color
    // (0xc0c0c0)
    c->setPlotArea(70, 65, 400, 350, 0xffffff, -1, 0xc0c0c0, 0xc0c0c0, -1);

    // Add a legend box with the top center point anchored at (270, 30). Use horizontal layout. Use
    // 10pt Arial Bold Italic font. Set the background and border color to Transparent.
    LegendBox *legendBox = c->addLegend(270, 30, false, "arialbi.ttf", 10);
    legendBox->setAlignment(Chart::TopCenter);
    legendBox->setBackground(Chart::Transparent, Chart::Transparent);

    // Add a title to the chart using 18 point Times Bold Itatic font.
    c->addTitle("Parametric Curve Fitting", "timesbi.ttf", 18);

    // Add titles to the axes using 12pt Arial Bold Italic font
    c->yAxis()->setTitle("Axis Title Placeholder", "arialbi.ttf", 12);
    c->xAxis()->setTitle("Axis Title Placeholder", "arialbi.ttf", 12);

    // Set the axes line width to 3 pixels
    c->yAxis()->setWidth(3);
    c->xAxis()->setWidth(3);

    // Add a scatter layer using (dataX0, dataY0)
    c->addScatterLayer(DoubleArray(dataX0, (int)(sizeof(dataX0) / sizeof(dataX0[0]))), DoubleArray(
        dataY0, (int)(sizeof(dataY0) / sizeof(dataY0[0]))), "Polynomial", Chart::GlassSphere2Shape,
        11, 0xff0000);

    // Add a degree 2 polynomial trend line layer for (dataX0, dataY0)
    TrendLayer *trend0 = c->addTrendLayer(DoubleArray(dataX0, (int)(sizeof(dataX0) / sizeof(dataX0[0
        ]))), DoubleArray(dataY0, (int)(sizeof(dataY0) / sizeof(dataY0[0]))), 0xff0000);
    trend0->setLineWidth(3);
    trend0->setRegressionType(Chart::PolynomialRegression(2));
    trend0->setHTMLImageMap("{disable}");

    // Add a scatter layer for (dataX1, dataY1)
    c->addScatterLayer(DoubleArray(dataX1, (int)(sizeof(dataX1) / sizeof(dataX1[0]))), DoubleArray(
        dataY1, (int)(sizeof(dataY1) / sizeof(dataY1[0]))), "Exponential", Chart::GlassSphere2Shape,
        11, 0x00aa00);

    // Add an exponential trend line layer for (dataX1, dataY1)
    TrendLayer *trend1 = c->addTrendLayer(DoubleArray(dataX1, (int)(sizeof(dataX1) / sizeof(dataX1[0
        ]))), DoubleArray(dataY1, (int)(sizeof(dataY1) / sizeof(dataY1[0]))), 0x00aa00);
    trend1->setLineWidth(3);
    trend1->setRegressionType(Chart::ExponentialRegression);
    trend1->setHTMLImageMap("{disable}");

    // Add a scatter layer using (dataX2, dataY2)
    c->addScatterLayer(DoubleArray(dataX2, (int)(sizeof(dataX2) / sizeof(dataX2[0]))), DoubleArray(
        dataY2, (int)(sizeof(dataY2) / sizeof(dataY2[0]))), "Logarithmic", Chart::GlassSphere2Shape,
        11, 0x0000ff);

    // Add a logarithmic trend line layer for (dataX2, dataY2)
    TrendLayer *trend2 = c->addTrendLayer(DoubleArray(dataX2, (int)(sizeof(dataX2) / sizeof(dataX2[0
        ]))), DoubleArray(dataY2, (int)(sizeof(dataY2) / sizeof(dataY2[0]))), 0x0000ff);
    trend2->setLineWidth(3);
    trend2->setRegressionType(Chart::LogarithmicRegression);
    trend2->setHTMLImageMap("{disable}");

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='[{dataSetName}] ({x}, {value})'");

    return c;
}


BaseChart *curvefitting(int /* chartIndex */, const char **imageMap)
{
    // Use random table to generate a random series. The random table is set to 1 col x 51 rows,
    // with 9 as the seed
    RanTable *rantable = new RanTable(9, 1, 51);

    // Set the 1st column to start from 100, with changes between rows from -5 to +5
    rantable->setCol(0, 100, -5, 5);

    // Get the 1st column of the random table as the data set
    DoubleArray data = rantable->getCol(0);

    // Create a XYChart object of size 600 x 300 pixels
    XYChart *c = new XYChart(600, 300);

    // Set the plotarea at (50, 35) and of size 500 x 240 pixels. Enable both the horizontal and
    // vertical grids by setting their colors to grey (0xc0c0c0)
    c->setPlotArea(50, 35, 500, 240)->setGridColor(0xc0c0c0, 0xc0c0c0);

    // Add a title to the chart using 18 point Times Bold Itatic font.
    c->addTitle("LOWESS Generic Curve Fitting Algorithm", "timesbi.ttf", 18);

    // Set the y axis line width to 3 pixels
    c->yAxis()->setWidth(3);

    // Add a title to the x axis using 12pt Arial Bold Italic font
    c->xAxis()->setTitle("Server Load (TPS)", "arialbi.ttf", 12);

    // Set the x axis line width to 3 pixels
    c->xAxis()->setWidth(3);

    // Set the x axis scale from 0 - 50, with major tick every 5 units and minor tick every 1 unit
    c->xAxis()->setLinearScale(0, 50, 5, 1);

    // Add a blue layer to the chart
    LineLayer *layer = c->addLineLayer();

    // Add a red (0x80ff0000) data set to the chart with square symbols
    layer->addDataSet(data, 0x80ff0000)->setDataSymbol(Chart::SquareSymbol);

    // Set the line width to 2 pixels
    layer->setLineWidth(2);

    // Use lowess for curve fitting, and plot the fitted data using a spline layer with line width
    // set to 3 pixels
    c->addSplineLayer(ArrayMath(data).lowess(), 0x0000ff)->setLineWidth(3);

    // Set zero affinity to 0 to make sure the line is displayed in the most detail scale
    c->yAxis()->setAutoScale(0, 0, 0);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='({x}, {value|2})'");

    //free up resources
    delete rantable;

    return c;
}


BaseChart *scatter(int /* chartIndex */, const char **imageMap)
{
    // The XY points for the scatter chart
    double dataX0[] = {10, 15, 6, 12, 14, 8, 13, 13, 16, 12, 10.5};
    double dataY0[] = {130, 150, 80, 110, 110, 105, 130, 115, 170, 125, 125};

    double dataX1[] = {6, 12, 4, 3.5, 7, 8, 9, 10, 12, 11, 8};
    double dataY1[] = {65, 80, 40, 45, 70, 80, 80, 90, 100, 105, 60};

    // Create a XYChart object of size 450 x 420 pixels
    XYChart *c = new XYChart(450, 420);

    // Set the plotarea at (55, 65) and of size 350 x 300 pixels, with a light grey border
    // (0xc0c0c0). Turn on both horizontal and vertical grid lines with light grey color (0xc0c0c0)
    c->setPlotArea(55, 65, 350, 300, -1, -1, 0xc0c0c0, 0xc0c0c0, -1);

    // Add a legend box at (50, 30) (top of the chart) with horizontal layout. Use 12pt Times Bold
    // Italic font. Set the background and border color to Transparent.
    c->addLegend(50, 30, false, "timesbi.ttf", 12)->setBackground(Chart::Transparent);

    // Add a title to the chart using 18pt Times Bold Itatic font.
    c->addTitle("Genetically Modified Predator", "timesbi.ttf", 18);

    // Add a title to the y axis using 12pt Arial Bold Italic font
    c->yAxis()->setTitle("Length (cm)", "arialbi.ttf", 12);

    // Add a title to the x axis using 12pt Arial Bold Italic font
    c->xAxis()->setTitle("Weight (kg)", "arialbi.ttf", 12);

    // Set the axes line width to 3 pixels
    c->xAxis()->setWidth(3);
    c->yAxis()->setWidth(3);

    // Add an orange (0xff9933) scatter chart layer, using 13 pixel diamonds as symbols
    c->addScatterLayer(DoubleArray(dataX0, (int)(sizeof(dataX0) / sizeof(dataX0[0]))), DoubleArray(
        dataY0, (int)(sizeof(dataY0) / sizeof(dataY0[0]))), "Genetically Engineered",
        Chart::DiamondSymbol, 13, 0xff9933);

    // Add a green (0x33ff33) scatter chart layer, using 11 pixel triangles as symbols
    c->addScatterLayer(DoubleArray(dataX1, (int)(sizeof(dataX1) / sizeof(dataX1[0]))), DoubleArray(
        dataY1, (int)(sizeof(dataY1) / sizeof(dataY1[0]))), "Natural", Chart::TriangleSymbol, 11,
        0x33ff33);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='[{dataSetName}] Weight = {x} kg, Length = {value} cm'");

    return c;
}


BaseChart *builtinsymbols(int /* chartIndex */, const char **imageMap)
{
    // Some ChartDirector built-in symbols
    int symbols[] = {Chart::CircleShape, Chart::GlassSphereShape, Chart::GlassSphere2Shape,
        Chart::SolidSphereShape, Chart::SquareShape, Chart::DiamondShape, Chart::TriangleShape,
        Chart::RightTriangleShape, Chart::LeftTriangleShape, Chart::InvertedTriangleShape,
        Chart::StarShape(3), Chart::StarShape(4), Chart::StarShape(5), Chart::StarShape(6),
        Chart::StarShape(7), Chart::StarShape(8), Chart::StarShape(9), Chart::StarShape(10),
        Chart::PolygonShape(5), Chart::Polygon2Shape(5), Chart::PolygonShape(6),
        Chart::Polygon2Shape(6), Chart::Polygon2Shape(7), Chart::CrossShape(0.1), Chart::CrossShape(
        0.2), Chart::CrossShape(0.3), Chart::CrossShape(0.4), Chart::CrossShape(0.5),
        Chart::CrossShape(0.6), Chart::CrossShape(0.7), Chart::Cross2Shape(0.1), Chart::Cross2Shape(
        0.2), Chart::Cross2Shape(0.3), Chart::Cross2Shape(0.4), Chart::Cross2Shape(0.5),
        Chart::Cross2Shape(0.6), Chart::Cross2Shape(0.7), Chart::ArrowShape(), Chart::ArrowShape(45
        ), Chart::ArrowShape(90, 0.5), Chart::ArrowShape(135, 0.5, 0.2), Chart::ArrowShape(180, 0.3,
        0.2, 0.3), Chart::ArrowShape(225, 1, 0.5, 0.7), Chart::ArrowShape(270, 1, 0.5, 0.25),
        Chart::ArrowShape(315, 0.5, 0.5, 0), Chart::ArrowShape(30, 0.5, 0.1, 0.6),
        Chart::ArrowShape(210, 0.5, 0.1, 0.6), Chart::ArrowShape(330, 0.7, 0.1), Chart::ArrowShape(
        150, 0.7, 0.1)};

    // Create a XYChart object of size 500 x 450 pixels
    XYChart *c = new XYChart(500, 450);

    // Set the plotarea at (55, 40) and of size 400 x 350 pixels, with a light grey border
    // (0xc0c0c0). Turn on both horizontal and vertical grid lines with light grey color (0xc0c0c0)
    c->setPlotArea(55, 40, 400, 350, -1, -1, 0xc0c0c0, 0xc0c0c0, -1);

    // Add a title to the chart using 18pt Times Bold Itatic font.
    c->addTitle("Built-in Symbols", "timesbi.ttf", 18);

    // Set the axes line width to 3 pixels
    c->xAxis()->setWidth(3);
    c->yAxis()->setWidth(3);

    // Ensure the ticks are at least 1 unit part (integer ticks)
    c->xAxis()->setMinTickInc(1);
    c->yAxis()->setMinTickInc(1);

    // Add each symbol as a separate scatter layer.
    for(int i = 0; i < (int)(sizeof(symbols) / sizeof(symbols[0])); ++i) {
        double coor1[] = {i % 7 + 1.0};
        double coor2[] = {i / 7 + 1.0};
        c->addScatterLayer(DoubleArray(coor1, (int)(sizeof(coor1) / sizeof(coor1[0]))), DoubleArray(
            coor2, (int)(sizeof(coor2) / sizeof(coor2[0]))), "", symbols[i], 17);
    }

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='(x, y) = ({x}, {value})'");

    return c;
}


BaseChart *scattersymbols(int /* chartIndex */, const char **imageMap)
{
    // The XY points for the scatter chart
    double dataX[] = {200, 400, 300, 250, 500};
    double dataY[] = {40, 100, 50, 150, 250};

    // The custom symbols for the points
    const char *symbols[] = {"robot1.png", "robot2.png", "robot3.png", "robot4.png", "robot5.png"};

    // Create a XYChart object of size 450 x 400 pixels
    XYChart *c = new XYChart(450, 400);

    // Set the plotarea at (55, 40) and of size 350 x 300 pixels, with a light grey border
    // (0xc0c0c0). Turn on both horizontal and vertical grid lines with light grey color (0xc0c0c0)
    c->setPlotArea(55, 40, 350, 300, -1, -1, 0xc0c0c0, 0xc0c0c0, -1);

    // Add a title to the chart using 18pt Times Bold Itatic font.
    c->addTitle("Battle Robots", "timesbi.ttf", 18);

    // Add a title to the y axis using 12pt Arial Bold Italic font
    c->yAxis()->setTitle("Speed (km/s)", "arialbi.ttf", 12);

    // Add a title to the y axis using 12pt Arial Bold Italic font
    c->xAxis()->setTitle("Range (km)", "arialbi.ttf", 12);

    // Set the axes line width to 3 pixels
    c->xAxis()->setWidth(3);
    c->yAxis()->setWidth(3);

    // Add each point of the data as a separate scatter layer, so that they can have a different
    // symbol
    for(int i = 0; i < (int)(sizeof(dataX) / sizeof(dataX[0])); ++i) {
        double coor1[] = {dataX[i]};
        double coor2[] = {dataY[i]};
        c->addScatterLayer(DoubleArray(coor1, (int)(sizeof(coor1) / sizeof(coor1[0]))), DoubleArray(
            coor2, (int)(sizeof(coor2) / sizeof(coor2[0]))))->getDataSet(0)->setDataSymbol(symbols[i
            ]);
    }

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='Range = {x} km, Speed = {value} km/s'");

    return c;
}


BaseChart *scatterlabels(int /* chartIndex */, const char **imageMap)
{
    // The XY points for the scatter chart
    double dataX[] = {150, 400, 300, 1500, 800};
    double dataY[] = {0.6, 8, 5.4, 2, 4};

    // The labels for the points
    const char *labels[] = {"Nano\n100", "SpeedTron\n200 Lite", "SpeedTron\n200", "Marathon\nExtra",
        "Marathon\n2000"};

    // Create a XYChart object of size 450 x 400 pixels
    XYChart *c = new XYChart(450, 400);

    // Set the plotarea at (55, 40) and of size 350 x 300 pixels, with a light grey border
    // (0xc0c0c0). Turn on both horizontal and vertical grid lines with light grey color (0xc0c0c0)
    c->setPlotArea(55, 40, 350, 300, 0xffffff, -1, 0xc0c0c0, 0xc0c0c0, -1);

    // Add a title to the chart using 18pt Times Bold Itatic font.
    c->addTitle("Product Comparison Chart", "timesbi.ttf", 18);

    // Add a title to the y axis using 12pt Arial Bold Italic font
    c->yAxis()->setTitle("Capacity (tons)", "arialbi.ttf", 12);

    // Add a title to the x axis using 12pt Arial Bold Italic font
    c->xAxis()->setTitle("Range (miles)", "arialbi.ttf", 12);

    // Set the axes line width to 3 pixels
    c->xAxis()->setWidth(3);
    c->yAxis()->setWidth(3);

    // Add the data as a scatter chart layer, using a 15 pixel circle as the symbol
    ScatterLayer *layer = c->addScatterLayer(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0
        ]))), DoubleArray(dataY, (int)(sizeof(dataY) / sizeof(dataY[0]))), "",
        Chart::GlassSphereShape, 15, 0xff3333, 0xff3333);

    // Add labels to the chart as an extra field
    layer->addExtraField(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Set the data label format to display the extra field
    layer->setDataLabelFormat("{field0}");

    // Use 8pt Arial Bold to display the labels
    TextBox *textbox = layer->setDataLabelStyle("arialbd.ttf", 8);

    // Set the background to purple with a 1 pixel 3D border
    textbox->setBackground(0xcc99ff, Chart::Transparent, 1);

    // Put the text box 4 pixels to the right of the data point
    textbox->setAlignment(Chart::Left);
    textbox->setPos(4, 0);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='Range = {x} miles, Capacity = {value} tons'");

    return c;
}


BaseChart *bubble(int /* chartIndex */, const char **imageMap)
{
    // The XYZ points for the bubble chart
    double dataX0[] = {150, 300, 1000, 1700};
    double dataY0[] = {12, 60, 25, 65};
    double dataZ0[] = {20, 50, 50, 85};

    double dataX1[] = {500, 1000, 1300};
    double dataY1[] = {35, 50, 75};
    double dataZ1[] = {30, 55, 95};

    // Create a XYChart object of size 450 x 420 pixels
    XYChart *c = new XYChart(450, 420);

    // Set the plotarea at (55, 65) and of size 350 x 300 pixels, with a light grey border
    // (0xc0c0c0). Turn on both horizontal and vertical grid lines with light grey color (0xc0c0c0)
    c->setPlotArea(55, 65, 350, 300, -1, -1, 0xc0c0c0, 0xc0c0c0, -1);

    // Add a legend box at (50, 30) (top of the chart) with horizontal layout. Use 12pt Times Bold
    // Italic font. Set the background and border color to Transparent.
    c->addLegend(50, 30, false, "timesbi.ttf", 12)->setBackground(Chart::Transparent);

    // Add a title to the chart using 18pt Times Bold Itatic font.
    c->addTitle("Product Comparison Chart", "timesbi.ttf", 18);

    // Add a title to the y axis using 12pt Arial Bold Italic font
    c->yAxis()->setTitle("Capacity (tons)", "arialbi.ttf", 12);

    // Add a title to the x axis using 12pt Arial Bold Italic font
    c->xAxis()->setTitle("Range (miles)", "arialbi.ttf", 12);

    // Set the axes line width to 3 pixels
    c->xAxis()->setWidth(3);
    c->yAxis()->setWidth(3);

    // Add (dataX0, dataY0) as a scatter layer with semi-transparent red (0x80ff3333) circle
    // symbols, where the circle size is modulated by dataZ0. This creates a bubble effect.
    c->addScatterLayer(DoubleArray(dataX0, (int)(sizeof(dataX0) / sizeof(dataX0[0]))), DoubleArray(
        dataY0, (int)(sizeof(dataY0) / sizeof(dataY0[0]))), "Technology AAA", Chart::CircleSymbol,
        9, 0x80ff3333, 0x80ff3333)->setSymbolScale(DoubleArray(dataZ0, (int)(sizeof(dataZ0) /
        sizeof(dataZ0[0]))));

    // Add (dataX1, dataY1) as a scatter layer with semi-transparent green (0x803333ff) circle
    // symbols, where the circle size is modulated by dataZ1. This creates a bubble effect.
    c->addScatterLayer(DoubleArray(dataX1, (int)(sizeof(dataX1) / sizeof(dataX1[0]))), DoubleArray(
        dataY1, (int)(sizeof(dataY1) / sizeof(dataY1[0]))), "Technology BBB", Chart::CircleSymbol,
        9, 0x803333ff, 0x803333ff)->setSymbolScale(DoubleArray(dataZ1, (int)(sizeof(dataZ1) /
        sizeof(dataZ1[0]))));

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='[{dataSetName}] Range = {x} miles, Capacity = {value} tons, Length = {z} meters'");

    return c;
}


BaseChart *threedbubble(int /* chartIndex */, const char **imageMap)
{
    // The XYZ points for the bubble chart
    double dataX0[] = {170, 300, 1000, 1700};
    double dataY0[] = {16, 69, 16, 75};
    double dataZ0[] = {52, 105, 88, 140};

    double dataX1[] = {500, 1000, 1300};
    double dataY1[] = {40, 58, 85};
    double dataZ1[] = {140, 202, 84};

    // Create a XYChart object of size 540 x 480 pixels
    XYChart *c = new XYChart(540, 480);

    // Set the plotarea at (70, 65) and of size 400 x 350 pixels. Turn on both horizontal and
    // vertical grid lines with light grey color (0xc0c0c0)
    c->setPlotArea(70, 65, 400, 350, -1, -1, Chart::Transparent, 0xc0c0c0, -1);

    // Add a legend box at (70, 30) (top of the chart) with horizontal layout. Use 12pt Times Bold
    // Italic font. Set the background and border color to Transparent.
    c->addLegend(70, 30, false, "timesbi.ttf", 12)->setBackground(Chart::Transparent);

    // Add a title to the chart using 18pt Times Bold Itatic font.
    c->addTitle("Product Comparison Chart", "timesbi.ttf", 18);

    // Add titles to the axes using 12pt Arial Bold Italic font
    c->yAxis()->setTitle("Capacity (tons)", "arialbi.ttf", 12);
    c->xAxis()->setTitle("Range (miles)", "arialbi.ttf", 12);

    // Set the axes line width to 3 pixels
    c->xAxis()->setWidth(3);
    c->yAxis()->setWidth(3);

    // Add (dataX0, dataY0) as a scatter layer with red (ff3333) glass spheres, where the sphere
    // size is modulated by dataZ0. This creates a bubble effect.
    c->addScatterLayer(DoubleArray(dataX0, (int)(sizeof(dataX0) / sizeof(dataX0[0]))), DoubleArray(
        dataY0, (int)(sizeof(dataY0) / sizeof(dataY0[0]))), "Technology AAA",
        Chart::GlassSphereShape, 15, 0xff3333)->setSymbolScale(DoubleArray(dataZ0, (int)(sizeof(
        dataZ0) / sizeof(dataZ0[0]))));

    // Add (dataX1, dataY1) as a scatter layer with blue (0000ff) glass spheres, where the sphere
    // size is modulated by dataZ1. This creates a bubble effect.
    c->addScatterLayer(DoubleArray(dataX1, (int)(sizeof(dataX1) / sizeof(dataX1[0]))), DoubleArray(
        dataY1, (int)(sizeof(dataY1) / sizeof(dataY1[0]))), "Technology BBB",
        Chart::GlassSphereShape, 15, 0x0000ff)->setSymbolScale(DoubleArray(dataZ1, (int)(sizeof(
        dataZ1) / sizeof(dataZ1[0]))));

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='[{dataSetName}] Range = {x} miles, Capacity = {value} tons, Length = {z} meters'");

    return c;
}


BaseChart *threedbubble2(int /* chartIndex */, const char **imageMap)
{
    // The XYZ points for the bubble chart
    double dataX0[] = {170, 300, 1000, 1700};
    double dataY0[] = {16, 69, 16, 75};
    double dataZ0[] = {52, 105, 88, 140};

    double dataX1[] = {500, 1000, 1300};
    double dataY1[] = {40, 58, 85};
    double dataZ1[] = {140, 202, 84};

    // Create a XYChart object of size 540 x 480 pixels
    XYChart *c = new XYChart(540, 480);

    // Set the plotarea at (70, 65) and of size 400 x 350 pixels. Turn on both horizontal and
    // vertical grid lines with light grey color (0xc0c0c0)
    c->setPlotArea(70, 65, 400, 350, -1, -1, Chart::Transparent, 0xc0c0c0, -1);

    // Add a legend box at (70, 30) (top of the chart) with horizontal layout. Use 12pt Times Bold
    // Italic font. Set the background and border color to Transparent.
    c->addLegend(70, 30, false, "timesbi.ttf", 12)->setBackground(Chart::Transparent);

    // Add a title to the chart using 18pt Times Bold Itatic font.
    c->addTitle("Product Comparison Chart", "timesbi.ttf", 18);

    // Add titles to the axes using 12pt Arial Bold Italic font
    c->yAxis()->setTitle("Capacity (tons)", "arialbi.ttf", 12);
    c->xAxis()->setTitle("Range (miles)", "arialbi.ttf", 12);

    // Set the axes line width to 3 pixels
    c->xAxis()->setWidth(3);
    c->yAxis()->setWidth(3);

    // Add (dataX0, dataY0) as a scatter layer with red (ff3333) glass spheres, where the sphere
    // size is modulated by dataZ0. This creates a bubble effect.
    c->addScatterLayer(DoubleArray(dataX0, (int)(sizeof(dataX0) / sizeof(dataX0[0]))), DoubleArray(
        dataY0, (int)(sizeof(dataY0) / sizeof(dataY0[0]))), "Technology AAA",
        Chart::GlassSphere2Shape, 15, 0xff3333)->setSymbolScale(DoubleArray(dataZ0, (int)(sizeof(
        dataZ0) / sizeof(dataZ0[0]))));

    // Add (dataX1, dataY1) as a scatter layer with blue (0000ff) glass spheres, where the sphere
    // size is modulated by dataZ1. This creates a bubble effect.
    c->addScatterLayer(DoubleArray(dataX1, (int)(sizeof(dataX1) / sizeof(dataX1[0]))), DoubleArray(
        dataY1, (int)(sizeof(dataY1) / sizeof(dataY1[0]))), "Technology BBB",
        Chart::GlassSphere2Shape, 15, 0x0000ff)->setSymbolScale(DoubleArray(dataZ1, (int)(sizeof(
        dataZ1) / sizeof(dataZ1[0]))));

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='[{dataSetName}] Range = {x} miles, Capacity = {value} tons, Length = {z} meters'");

    return c;
}


BaseChart *threedbubble3(int /* chartIndex */, const char **imageMap)
{
    // The XYZ points for the bubble chart
    double dataX0[] = {170, 300, 1000, 1700};
    double dataY0[] = {16, 69, 16, 75};
    double dataZ0[] = {52, 105, 88, 140};

    double dataX1[] = {500, 1000, 1300};
    double dataY1[] = {40, 58, 85};
    double dataZ1[] = {140, 202, 84};

    // Create a XYChart object of size 540 x 480 pixels
    XYChart *c = new XYChart(540, 480);

    // Set the plotarea at (70, 65) and of size 400 x 350 pixels. Turn on both horizontal and
    // vertical grid lines with light grey color (0xc0c0c0)
    c->setPlotArea(70, 65, 400, 350, -1, -1, Chart::Transparent, 0xc0c0c0, -1);

    // Add a legend box at (70, 30) (top of the chart) with horizontal layout. Use 12pt Times Bold
    // Italic font. Set the background and border color to Transparent.
    c->addLegend(70, 30, false, "timesbi.ttf", 12)->setBackground(Chart::Transparent);

    // Add a title to the chart using 18pt Times Bold Itatic font.
    c->addTitle("Product Comparison Chart", "timesbi.ttf", 18);

    // Add titles to the axes using 12pt Arial Bold Italic font
    c->yAxis()->setTitle("Capacity (tons)", "arialbi.ttf", 12);
    c->xAxis()->setTitle("Range (miles)", "arialbi.ttf", 12);

    // Set the axes line width to 3 pixels
    c->xAxis()->setWidth(3);
    c->yAxis()->setWidth(3);

    // Add (dataX0, dataY0) as a scatter layer with red (ff3333) spheres, where the sphere size is
    // modulated by dataZ0. This creates a bubble effect.
    c->addScatterLayer(DoubleArray(dataX0, (int)(sizeof(dataX0) / sizeof(dataX0[0]))), DoubleArray(
        dataY0, (int)(sizeof(dataY0) / sizeof(dataY0[0]))), "Technology AAA",
        Chart::SolidSphereShape, 15, 0xff3333)->setSymbolScale(DoubleArray(dataZ0, (int)(sizeof(
        dataZ0) / sizeof(dataZ0[0]))));

    // Add (dataX1, dataY1) as a scatter layer with blue (0000ff) spheres, where the sphere size is
    // modulated by dataZ1. This creates a bubble effect.
    c->addScatterLayer(DoubleArray(dataX1, (int)(sizeof(dataX1) / sizeof(dataX1[0]))), DoubleArray(
        dataY1, (int)(sizeof(dataY1) / sizeof(dataY1[0]))), "Technology BBB",
        Chart::SolidSphereShape, 15, 0x0000ff)->setSymbolScale(DoubleArray(dataZ1, (int)(sizeof(
        dataZ1) / sizeof(dataZ1[0]))));

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='[{dataSetName}] Range = {x} miles, Capacity = {value} tons, Length = {z} meters'");

    return c;
}


BaseChart *bubblescale(int /* chartIndex */, const char **imageMap)
{
    // The XY points for the bubble chart. The bubble chart has independent bubble size on the X and
    // Y direction.
    double dataX0[] = {1000, 1500, 1700};
    double dataY0[] = {25, 20, 65};
    double dataZX0[] = {500, 200, 600};
    double dataZY0[] = {15, 30, 20};

    double dataX1[] = {500, 1000, 1300};
    double dataY1[] = {35, 50, 75};
    double dataZX1[] = {800, 300, 500};
    double dataZY1[] = {8, 27, 25};

    double dataX2[] = {150, 300};
    double dataY2[] = {20, 60};
    double dataZX2[] = {160, 400};
    double dataZY2[] = {30, 20};

    // Create a XYChart object of size 450 x 420 pixels
    XYChart *c = new XYChart(450, 420);

    // Set the plotarea at (55, 65) and of size 350 x 300 pixels, with a light grey border
    // (0xc0c0c0). Turn on both horizontal and vertical grid lines with light grey color (0xc0c0c0)
    c->setPlotArea(55, 65, 350, 300, -1, -1, 0xc0c0c0, 0xc0c0c0, -1);

    // Add a legend box at (50, 30) (top of the chart) with horizontal layout. Use 12pt Times Bold
    // Italic font. Set the background and border color to Transparent.
    c->addLegend(50, 30, false, "timesbi.ttf", 12)->setBackground(Chart::Transparent);

    // Add a title to the chart using 18pt Times Bold Itatic font.
    c->addTitle("Plasma Battery Comparison", "timesbi.ttf", 18);

    // Add titles to the axes using 12pt Arial Bold Italic font
    c->yAxis()->setTitle("Operating Current", "arialbi.ttf", 12);
    c->xAxis()->setTitle("Operating Voltage", "arialbi.ttf", 12);

    // Set the axes line width to 3 pixels
    c->xAxis()->setWidth(3);
    c->yAxis()->setWidth(3);

    // Add (dataX0, dataY0) as a standard scatter layer, and also as a "bubble" scatter layer, using
    // circles as symbols. The "bubble" scatter layer has symbol size modulated by (dataZX0,
    // dataZY0) using the scale on the x and y axes.
    c->addScatterLayer(DoubleArray(dataX0, (int)(sizeof(dataX0) / sizeof(dataX0[0]))), DoubleArray(
        dataY0, (int)(sizeof(dataY0) / sizeof(dataY0[0]))), "Vendor A", Chart::CircleSymbol, 9,
        0xff3333, 0xff3333);
    c->addScatterLayer(DoubleArray(dataX0, (int)(sizeof(dataX0) / sizeof(dataX0[0]))), DoubleArray(
        dataY0, (int)(sizeof(dataY0) / sizeof(dataY0[0]))), "", Chart::CircleSymbol, 9, 0x80ff3333,
        0x80ff3333)->setSymbolScale(DoubleArray(dataZX0, (int)(sizeof(dataZX0) / sizeof(dataZX0[0]))
        ), Chart::XAxisScale, DoubleArray(dataZY0, (int)(sizeof(dataZY0) / sizeof(dataZY0[0]))),
        Chart::YAxisScale);

    // Add (dataX1, dataY1) as a standard scatter layer, and also as a "bubble" scatter layer, using
    // squares as symbols. The "bubble" scatter layer has symbol size modulated by (dataZX1,
    // dataZY1) using the scale on the x and y axes.
    c->addScatterLayer(DoubleArray(dataX1, (int)(sizeof(dataX1) / sizeof(dataX1[0]))), DoubleArray(
        dataY1, (int)(sizeof(dataY1) / sizeof(dataY1[0]))), "Vendor B", Chart::SquareSymbol, 7,
        0x3333ff, 0x3333ff);
    c->addScatterLayer(DoubleArray(dataX1, (int)(sizeof(dataX1) / sizeof(dataX1[0]))), DoubleArray(
        dataY1, (int)(sizeof(dataY1) / sizeof(dataY1[0]))), "", Chart::SquareSymbol, 9, 0x803333ff,
        0x803333ff)->setSymbolScale(DoubleArray(dataZX1, (int)(sizeof(dataZX1) / sizeof(dataZX1[0]))
        ), Chart::XAxisScale, DoubleArray(dataZY1, (int)(sizeof(dataZY1) / sizeof(dataZY1[0]))),
        Chart::YAxisScale);

    // Add (dataX2, dataY2) as a standard scatter layer, and also as a "bubble" scatter layer, using
    // diamonds as symbols. The "bubble" scatter layer has symbol size modulated by (dataZX2,
    // dataZY2) using the scale on the x and y axes.
    c->addScatterLayer(DoubleArray(dataX2, (int)(sizeof(dataX2) / sizeof(dataX2[0]))), DoubleArray(
        dataY2, (int)(sizeof(dataY2) / sizeof(dataY2[0]))), "Vendor C", Chart::DiamondSymbol, 9,
        0x00ff00, 0x00ff00);
    c->addScatterLayer(DoubleArray(dataX2, (int)(sizeof(dataX2) / sizeof(dataX2[0]))), DoubleArray(
        dataY2, (int)(sizeof(dataY2) / sizeof(dataY2[0]))), "", Chart::DiamondSymbol, 9, 0x8033ff33,
        0x8033ff33)->setSymbolScale(DoubleArray(dataZX2, (int)(sizeof(dataZX2) / sizeof(dataZX2[0]))
        ), Chart::XAxisScale, DoubleArray(dataZY2, (int)(sizeof(dataZY2) / sizeof(dataZY2[0]))),
        Chart::YAxisScale);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='Voltage = {x} +/- {={zx}/2} V, Current = {value} +/- {={zy}/2} A'");

    return c;
}


BaseChart *vector(int /* chartIndex */, const char **imageMap)
{
    // The (x, y) coordinates of the starting point of the vectors
    double dataX[] = {20, 40, 60, 80, 100, 20, 40, 60, 80, 100, 20, 40, 58, 62, 80, 100, 20, 40, 60,
        80, 100, 20, 40, 60, 80, 100};
    double dataY[] = {20, 20, 20, 20, 20, 40, 40, 40, 40, 40, 60, 60, 60, 60, 60, 60, 80, 80, 80,
        80, 80, 100, 100, 100, 100, 100};

    // The lengths (radii) and directions (angles) of the vectors
    double dataR[] = {6, 6, 9, 6, 6, 6, 9, 12, 9, 6, 9, 12, 12, 12, 12, 9, 6, 9, 12, 9, 6, 6, 6, 9,
        6, 6};
    double dataA[] = {-45, -30, 0, 30, 45, -60, -45, 0, 45, 60, -90, -90, -90, 90, 90, 90, -120,
        -135, 180, 135, 120, -135, -150, 180, 150, 135};

    // Create a XYChart object of size 450 x 390 pixels
    XYChart *c = new XYChart(450, 390);

    // Set the plotarea at (55, 40) and of size 350 x 300 pixels, with a light grey border
    // (0xc0c0c0). Turn on both horizontal and vertical grid lines with light grey color (0xc0c0c0)
    c->setPlotArea(55, 40, 350, 300, -1, -1, 0xc0c0c0, 0xc0c0c0, -1);

    // Add a title to the chart using 18pt Times Bold Itatic font.
    c->addTitle("Vector Chart Demonstration", "timesbi.ttf", 18);

    // Add a title to the y axis using 12pt Arial Bold Italic font
    c->yAxis()->setTitle("Y Axis Title", "arialbi.ttf", 12);

    // Add a title to the x axis using 12pt Arial Bold Italic font
    c->xAxis()->setTitle("X Axis Title", "arialbi.ttf", 12);

    // Set the axes line width to 3 pixels
    c->xAxis()->setWidth(3);
    c->yAxis()->setWidth(3);

    // Add a vector layer to the chart using blue (0000CC) color, with vector arrow size set to 11
    // pixels
    c->addVectorLayer(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0]))), DoubleArray(
        dataY, (int)(sizeof(dataY) / sizeof(dataY[0]))), DoubleArray(dataR, (int)(sizeof(dataR) /
        sizeof(dataR[0]))), DoubleArray(dataA, (int)(sizeof(dataA) / sizeof(dataA[0]))),
        Chart::YAxisScale, 0x0000cc)->setArrowHead(11);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='Vector at ({x}, {value}): Length = {len}, Angle = {dir} deg'");

    return c;
}


BaseChart *simplearea(int /* chartIndex */, const char **imageMap)
{
    // The data for the area chart
    double data[] = {30, 28, 40, 55, 75, 68, 54, 60, 50, 62, 75, 65, 75, 89, 60, 55, 53, 35, 50, 66,
        56, 48, 52, 65, 62};

    // The labels for the area chart
    const char *labels[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12",
        "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24"};

    // Create a XYChart object of size 250 x 250 pixels
    XYChart *c = new XYChart(250, 250);

    // Set the plotarea at (30, 20) and of size 200 x 200 pixels
    c->setPlotArea(30, 20, 200, 200);

    // Add an area chart layer using the given data
    c->addAreaLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))));

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Display 1 out of 3 labels on the x-axis.
    c->xAxis()->setLabelStep(3);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='Hour {xLabel}: Traffic {value} GBytes'")
        ;

    return c;
}


BaseChart *enhancedarea(int /* chartIndex */, const char **imageMap)
{
    // The data for the area chart
    double data[] = {30, 28, 40, 55, 75, 68, 54, 60, 50, 62, 75, 65, 75, 89, 60, 55, 53, 35, 50, 66,
        56, 48, 52, 65, 62};

    // The labels for the area chart
    const char *labels[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12",
        "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24"};

    // Create a XYChart object of size 600 x 360 pixels, with a brushed silver background, 1 pixel
    // 3D border effect, rounded corners and soft drop shadow.
    XYChart *c = new XYChart(600, 360, Chart::brushedSilverColor(), Chart::Transparent, 1);
    c->setRoundedFrame();
    c->setDropShadow();

    // Add a title box to the chart using 18pt Times Bold Italic font.
    TextBox *title = c->addTitle(
        "<*block,valign=absmiddle*><*img=star.png*><*img=star.png*> Performance Enhancer "
        "<*img=star.png*><*img=star.png*><*/*>", "timesbi.ttf", 18);

    //
    // Use a text box with a depressed 3D border to create the inner depressed region
    //

    // The width of the frame border
    int frameWidth = 5;

    // Set the depressed region position
    TextBox *contentBox = c->addText(frameWidth, title->getHeight(), "");
    contentBox->setSize(c->getDrawArea()->getWidth() - 1 - frameWidth * 2, c->getDrawArea(
        )->getHeight() - title->getHeight() - frameWidth - 1);

    // Use -1 as the rasied effect to create a depressed region
    contentBox->setBackground(Chart::Transparent, Chart::Transparent, -1);

    // Set rounded corners, and put the text box at the back of the chart
    contentBox->setRoundedCorners(10);
    contentBox->setZOrder(Chart::ChartBackZ);

    // Tentatively set the plotarea to 50 pixels from the left depressed edge, and 25 pixels under
    // the top depressed edge. Set the width to 75 pixels less than the depressed region width, and
    // the height to 75 pixels less than the depressed region height. Use white (ffffff) background,
    // transparent border, and grey (cccccc) horizontal and vertical grid lines.
    PlotArea *plotArea = c->setPlotArea(50 + contentBox->getLeftX(), contentBox->getTopY() + 25,
        contentBox->getWidth() - 75, contentBox->getHeight() - 75, 0xffffff, -1, -1, 0xcccccc, -1);

    // Add a title to the y axis
    c->yAxis()->setTitle("Energy Concentration (KJ per liter)");

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Display 1 out of 3 labels on the x-axis.
    c->xAxis()->setLabelStep(3);

    // Add a title to the x axis using CDML
    c->xAxis()->setTitle("<*block,valign=absmiddle*><*img=clock.png*>  Elapsed Time (hour)<*/*>");

    // Set the axes width to 2 pixels
    c->xAxis()->setWidth(2);
    c->yAxis()->setWidth(2);

    // Add an area layer to the chart using a gradient color that changes vertically from
    // semi-transparent red (60ff0000) to semi-transparent white (60ffffff)
    c->addAreaLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))),
        c->linearGradientColor(0, contentBox->getTopY() + 20, 0, contentBox->getTopY() +
        contentBox->getHeight() - 50, 0x60ff0000, 0x60ffffff));

    // Adjust the plot area size, such that the bounding box (inclusive of axes) is 15 pixels from
    // the left depressed edge, 25 pixels below the top depressed edge, 25 pixels from the right
    // depressed edge, and 15 pixels above the bottom depressed edge.
    c->packPlotArea(contentBox->getLeftX() + 15, contentBox->getTopY() + 25, contentBox->getLeftX()
         + contentBox->getWidth() - 25, contentBox->getTopY() + contentBox->getHeight() - 15);

    // Add a custom CDML text with the bottom right corner is anchored to the bootom right corner of
    // the plot area, with 5 pixels margin.
    c->addText(plotArea->getLeftX() + plotArea->getWidth() - 5, plotArea->getTopY() +
        plotArea->getHeight() - 5,
        "<*block,valign=absmiddle*><*img=small_molecule.png*> <*block*>"
        "<*font=timesbi.ttf,size=10,color=804040*>Molecular\nEngineering<*/*>")->setAlignment(
        Chart::BottomRight);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='Hour {xLabel}: {value} KJ/liter'");

    return c;
}


BaseChart *arealine(int /* chartIndex */, const char **imageMap)
{
    // In this example, we simply use random data for the 2 data series.
    RanSeries *r = new RanSeries(127);
    DoubleArray data0 = r->getSeries(180, 70, -5, 5);
    DoubleArray data1 = r->getSeries(180, 150, -15, 15);
    DoubleArray timeStamps = r->getDateSeries(180, Chart::chartTime(2014, 3, 1), 86400);

    // Create a XYChart object of size 640 x 420 pixels
    XYChart *c = new XYChart(640, 420);

    // Add a title box using grey (0x555555) 20pt Arial Bold font
    c->addTitle("   Plasma Stabilizer Energy Usage", "arialbd.ttf", 20, 0x555555);

    // Set the plotarea at (70, 70) and of size 540 x 320 pixels, with transparent background and
    // border and light grey (0xcccccc) horizontal grid lines
    c->setPlotArea(70, 70, 540, 320, -1, -1, Chart::Transparent, 0xcccccc);

    // Add a legend box with horizontal layout above the plot area at (70, 32). Use 12pt Arial Bold
    // dark grey (0x555555) font, transparent background and border, and line style legend icon.
    LegendBox *b = c->addLegend(70, 32, false, "arialbd.ttf", 12);
    b->setFontColor(0x555555);
    b->setBackground(Chart::Transparent, Chart::Transparent);
    b->setLineStyleKey();

    // Set axis label font to 12pt Arial
    c->xAxis()->setLabelStyle("arial.ttf", 12);
    c->yAxis()->setLabelStyle("arial.ttf", 12);

    // Set the x and y axis stems to transparent, and the x-axis tick color to grey (0xaaaaaa)
    c->xAxis()->setColors(Chart::Transparent, Chart::TextColor, Chart::TextColor, 0xaaaaaa);
    c->yAxis()->setColors(Chart::Transparent);

    // Set the major/minor tick lengths for the x-axis to 10 and 0.
    c->xAxis()->setTickLength(10, 0);

    // For the automatic axis labels, set the minimum spacing to 80/40 pixels for the x/y axis.
    c->xAxis()->setTickDensity(80);
    c->yAxis()->setTickDensity(40);

    // Use "mm/yyyy" as the x-axis label format for the first plotted month of a year, and "mm" for
    // other months
    c->xAxis()->setMultiFormat(Chart::StartOfYearFilter(), "{value|mm/yyyy} ",
        Chart::StartOfMonthFilter(), "{value|mm}");

    // Add a title to the y axis using dark grey (0x555555) 12pt Arial Bold font
    c->yAxis()->setTitle("Energy (kWh)", "arialbd.ttf", 14, 0x555555);

    // Add a line layer with 2-pixel line width
    LineLayer *layer0 = c->addLineLayer(data0, 0xcc0000, "Power Usage");
    layer0->setXData(timeStamps);
    layer0->setLineWidth(2);

    // Add an area layer using semi-transparent blue (0x7f0044cc) as the fill color
    AreaLayer *layer1 = c->addAreaLayer(data1, 0x7f0044cc, "Effective Load");
    layer1->setXData(timeStamps);
    layer1->setBorderColor(Chart::SameAsMainColor);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='[{x|mm dd, yyyy}] {value} kWh'");

    //free up resources
    delete r;

    return c;
}


BaseChart *threedarea(int /* chartIndex */, const char **imageMap)
{
    // The data for the area chart
    double data[] = {30, 28, 40, 55, 75, 68, 54, 60, 50, 62, 75, 65, 75, 89, 60, 55, 53, 35, 50, 66,
        56, 48, 52, 65, 62};

    // The labels for the area chart
    const char *labels[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12",
        "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24"};

    // Create a XYChart object of size 300 x 300 pixels
    XYChart *c = new XYChart(300, 300);

    // Set the plotarea at (45, 30) and of size 200 x 200 pixels
    c->setPlotArea(45, 30, 200, 200);

    // Add a title to the chart using 12pt Arial Bold Italic font
    c->addTitle("Daily Server Utilization", "arialbi.ttf", 12);

    // Add a title to the y axis
    c->yAxis()->setTitle("MBytes");

    // Add a title to the x axis
    c->xAxis()->setTitle("June 12, 2001");

    // Add a green (0x00ff00) 3D area chart layer using the give data
    c->addAreaLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), 0x00ff00)->set3D();

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Display 1 out of 3 labels on the x-axis.
    c->xAxis()->setLabelStep(3);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='Hour {xLabel}: Traffic {value} MBytes'")
        ;

    return c;
}


BaseChart *patternarea(int /* chartIndex */, const char **imageMap)
{
    // The data for the area chart
    double data[] = {3.0, 2.8, 4.0, 5.5, 7.5, 6.8, 5.4, 6.0, 5.0, 6.2, 7.5, 6.5, 7.5, 8.1, 6.0, 5.5,
        5.3, 3.5, 5.0, 6.6, 5.6, 4.8, 5.2, 6.5, 6.2};

    // The labels for the area chart
    const char *labels[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12",
        "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24"};

    // Create a XYChart object of size 300 x 180 pixels. Set the background to pale yellow
    // (0xffffa0) with a black border (0x0)
    XYChart *c = new XYChart(300, 180, 0xffffa0, 0x000000);

    // Set the plotarea at (45, 35) and of size 240 x 120 pixels. Set the background to white
    // (0xffffff). Set both horizontal and vertical grid lines to black (&H0&) dotted lines (pattern
    // code 0x0103)
    c->setPlotArea(45, 35, 240, 120, 0xffffff, -1, -1, c->dashLineColor(0x000000, 0x000103),
        c->dashLineColor(0x000000, 0x000103));

    // Add a title to the chart using 10pt Arial Bold font. Use a 1 x 2 bitmap pattern as the
    // background. Set the border to black (0x0).
    int pattern1[] = {0xb0b0f0, 0xe0e0ff};
    c->addTitle("Snow Percipitation (Dec 12)", "arialbd.ttf", 10)->setBackground(c->patternColor(
        IntArray(pattern1, (int)(sizeof(pattern1) / sizeof(pattern1[0]))), 2), 0x000000);

    // Add a title to the y axis
    c->yAxis()->setTitle("mm per hour");

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Display 1 out of 3 labels on the x-axis.
    c->xAxis()->setLabelStep(3);

    // Add an area layer to the chart
    AreaLayer *layer = c->addAreaLayer();

    // Load a snow pattern from an external file "snow.png".
    int snowPattern = c->patternColor("snow.png");

    // Add a data set to the area layer using the snow pattern as the fill color. Use deep blue
    // (0x0000ff) as the area border line color (&H0000ff&)
    layer->addDataSet(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))))->setDataColor(
        snowPattern, 0x0000ff);

    // Set the line width to 2 pixels to highlight the line
    layer->setLineWidth(2);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{xLabel}:00 - {value} mm/hour'");

    return c;
}


BaseChart *stackedarea(int /* chartIndex */, const char **imageMap)
{
    // The data for the area chart
    double data0[] = {42, 49, 33, 38, 51, 46, 29, 41, 44, 57, 59, 52, 37, 34, 51, 56, 56, 60, 70,
        76, 63, 67, 75, 64, 51};
    double data1[] = {50, 45, 47, 34, 42, 49, 63, 62, 73, 59, 56, 50, 64, 60, 67, 67, 58, 59, 73,
        77, 84, 82, 80, 84, 89};
    double data2[] = {61, 79, 85, 66, 53, 39, 24, 21, 37, 56, 37, 22, 21, 33, 13, 17, 4, 23, 16, 25,
        9, 10, 5, 7, 16};
    const char *labels[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12",
        "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24"};

    // Create a XYChart object of size 300 x 210 pixels. Set the background to pale yellow
    // (0xffffc0) with a black border (0x0)
    XYChart *c = new XYChart(300, 210, 0xffffc0, 0x000000);

    // Set the plotarea at (50, 30) and of size 240 x 140 pixels. Use white (0xffffff) background.
    c->setPlotArea(50, 30, 240, 140)->setBackground(0xffffff);

    // Add a legend box at (50, 185) (below of plot area) using horizontal layout. Use 8pt Arial
    // font with Transparent background.
    c->addLegend(50, 185, false, "", 8)->setBackground(Chart::Transparent);

    // Add a title box to the chart using 8pt Arial Bold font, with yellow (0xffff40) background and
    // a black border (0x0)
    c->addTitle("Sales Volume", "arialbd.ttf", 8)->setBackground(0xffff40, 0);

    // Set the y axis label format to US$nnnn
    c->yAxis()->setLabelFormat("US${value}");

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Display 1 out of 2 labels on the x-axis. Show minor ticks for remaining labels.
    c->xAxis()->setLabelStep(2, 1);

    // Add an stack area layer with three data sets
    AreaLayer *layer = c->addAreaLayer(Chart::Stack);
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0x4040ff,
        "Store #1");
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0xff4040,
        "Store #2");
    layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), 0x40ff40,
        "Store #3");

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName} sales at hour {xLabel}: US${value}K'");

    return c;
}


BaseChart *threedstackarea(int /* chartIndex */, const char **imageMap)
{
    // The data for the area chart
    double data0[] = {42, 49, 33, 38, 51, 46, 29, 41, 44, 57, 59, 52, 37, 34, 51, 56, 56, 60, 70,
        76, 63, 67, 75, 64, 51};
    double data1[] = {50, 45, 47, 34, 42, 49, 63, 62, 73, 59, 56, 50, 64, 60, 67, 67, 58, 59, 73,
        77, 84, 82, 80, 84, 89};
    double data2[] = {61, 79, 85, 66, 53, 39, 24, 21, 37, 56, 37, 22, 21, 33, 13, 17, 4, 23, 16, 25,
        9, 10, 5, 7, 16};
    const char *labels[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12",
        "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24"};

    // Create a XYChart object of size 500 x 300 pixels
    XYChart *c = new XYChart(500, 300);

    // Set the plotarea at (90, 30) and of size 300 x 240 pixels.
    c->setPlotArea(90, 30, 300, 240);

    // Add a legend box at (405, 100)
    c->addLegend(405, 100);

    // Add a title to the chart
    c->addTitle("Daily System Load");

    // Add a title to the y axis. Draw the title upright (font angle = 0)
    c->yAxis()->setTitle("Database\nQueries\n(per sec)")->setFontAngle(0);

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Display 1 out of 2 labels on the x-axis. Show minor ticks for remaining labels.
    c->xAxis()->setLabelStep(2, 1);

    // Add an area layer
    AreaLayer *layer = c->addAreaLayer();

    // Draw the area layer in 3D
    layer->set3D();

    // Add the three data sets to the area layer
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), -1, "Server # 1")
        ;
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), -1, "Server # 2")
        ;
    layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), -1, "Server # 3")
        ;

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName} load at hour {xLabel}: {value} queries/sec'");

    return c;
}


BaseChart *percentarea(int /* chartIndex */, const char **imageMap)
{
    // The data for the area chart
    double data0[] = {42, 49, 33, 38, 51, 46, 29, 41, 44, 57, 59, 52, 37, 34, 51, 56, 56, 60, 70,
        76, 63, 67, 75, 64, 51};
    double data1[] = {50, 55, 47, 34, 42, 49, 63, 62, 73, 59, 56, 50, 64, 60, 67, 67, 58, 59, 73,
        77, 84, 82, 80, 84, 98};
    double data2[] = {87, 89, 85, 66, 53, 39, 24, 21, 37, 56, 37, 23, 21, 33, 13, 17, 14, 23, 16,
        25, 29, 30, 45, 47, 46};

    // The timestamps on the x-axis
    double labels[] = {Chart::chartTime(1996, 1, 1), Chart::chartTime(1996, 4, 1), Chart::chartTime(
        1996, 7, 1), Chart::chartTime(1996, 10, 1), Chart::chartTime(1997, 1, 1), Chart::chartTime(
        1997, 4, 1), Chart::chartTime(1997, 7, 1), Chart::chartTime(1997, 10, 1), Chart::chartTime(
        1998, 1, 1), Chart::chartTime(1998, 4, 1), Chart::chartTime(1998, 7, 1), Chart::chartTime(
        1998, 10, 1), Chart::chartTime(1999, 1, 1), Chart::chartTime(1999, 4, 1), Chart::chartTime(
        1999, 7, 1), Chart::chartTime(1999, 10, 1), Chart::chartTime(2000, 1, 1), Chart::chartTime(
        2000, 4, 1), Chart::chartTime(2000, 7, 1), Chart::chartTime(2000, 10, 1), Chart::chartTime(
        2001, 1, 1), Chart::chartTime(2001, 4, 1), Chart::chartTime(2001, 7, 1), Chart::chartTime(
        2001, 10, 1), Chart::chartTime(2002, 1, 1)};

    // Create a XYChart object of size 500 x 280 pixels, using 0xffffcc as background color, with a
    // black border, and 1 pixel 3D border effect
    XYChart *c = new XYChart(500, 280, 0xffffcc, 0, 1);

    // Set the plotarea at (50, 45) and of size 320 x 200 pixels with white background. Enable
    // horizontal and vertical grid lines using the grey (0xc0c0c0) color.
    c->setPlotArea(50, 45, 320, 200, 0xffffff)->setGridColor(0xc0c0c0, 0xc0c0c0);

    // Add a legend box at (370, 45) using vertical layout and 8 points Arial Bold font.
    LegendBox *legendBox = c->addLegend(370, 45, true, "arialbd.ttf", 8);

    // Set the legend box background and border to transparent
    legendBox->setBackground(Chart::Transparent, Chart::Transparent);

    // Set the legend box icon size to 16 x 32 pixels to match with custom icon size
    legendBox->setKeySize(16, 32);

    // Add a title to the chart using 14 points Times Bold Itatic font and white font color, and
    // 0x804020 as the background color
    c->addTitle("Quarterly Product Sales", "timesbi.ttf", 14, 0xffffff)->setBackground(0x804020);

    // Set the labels on the x axis.
    c->xAxis()->setLabels(DoubleArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Set multi-style axis label formatting. Start of year labels are displayed as yyyy. For other
    // labels, just show minor tick.
    c->xAxis()->setMultiFormat(Chart::StartOfYearFilter(), "{value|yyyy}", Chart::AllPassFilter(),
        "-");

    // Add a percentage area layer to the chart
    AreaLayer *layer = c->addAreaLayer(Chart::Percentage);

    // Add the three data sets to the area layer, using icons images with labels as data set names
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0x40ddaa77,
        "<*block,valign=absmiddle*><*img=service.png*> Service<*/*>");
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x40aadd77,
        "<*block,valign=absmiddle*><*img=software.png*> Software<*/*>");
    layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), 0x40aa77dd,
        "<*block,valign=absmiddle*><*img=computer.png*> Hardware<*/*>");

    // For a vertical stacked chart with positive data only, the last data set is always on top.
    // However, in a vertical legend box, the last data set is at the bottom. This can be reversed
    // by using the setLegend method.
    layer->setLegend(Chart::ReverseLegend);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName} sales at {xLabel|yyyy} Q{=({xLabel|m}+2)/3|0}: US${value}K "
        "({percent}%)'");

    return c;
}


BaseChart *deptharea(int /* chartIndex */, const char **imageMap)
{
    // The data for the area chart
    double data0[] = {42, 49, 33, 38, 51, 46, 29, 41, 44, 57, 59, 52, 37, 34, 51, 56, 56, 60, 70,
        76, 63, 67, 75, 64, 51};
    double data1[] = {50, 55, 47, 34, 42, 49, 63, 62, 73, 59, 56, 50, 64, 60, 67, 67, 58, 59, 73,
        77, 84, 82, 80, 84, 89};
    double data2[] = {87, 89, 85, 66, 53, 39, 24, 21, 37, 56, 37, 22, 21, 33, 13, 17, 4, 23, 16, 25,
        9, 10, 5, 7, 6};
    const char *labels[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12",
        "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24"};

    // Create a XYChart object of size 350 x 230 pixels
    XYChart *c = new XYChart(350, 230);

    // Set the plotarea at (50, 30) and of size 250 x 150 pixels.
    c->setPlotArea(50, 30, 250, 150);

    // Add a legend box at (55, 0) (top of the chart) using 8pt Arial Font. Set background and
    // border to Transparent.
    c->addLegend(55, 0, false, "", 8)->setBackground(Chart::Transparent);

    // Add a title to the x axis
    c->xAxis()->setTitle("Network Load for Jun 12");

    // Add a title to the y axis
    c->yAxis()->setTitle("MBytes");

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Display 1 out of 2 labels on the x-axis. Show minor ticks for remaining labels.
    c->xAxis()->setLabelStep(2, 1);

    // Add three area layers, each representing one data set. The areas are drawn in
    // semi-transparent colors.
    c->addAreaLayer(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), 0x808080ff,
        "Server #1", 3);
    c->addAreaLayer(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0x80ff0000,
        "Server #2", 3);
    c->addAreaLayer(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x8000ff00,
        "Server #3", 3);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName} load at hour {xLabel}: {value} MBytes'");

    return c;
}


BaseChart *rotatedarea(int /* chartIndex */, const char **imageMap)
{
    // The data for the area chart
    double data[] = {30, 28, 40, 55, 75, 68, 54, 60, 50, 62, 75, 65, 75, 89, 60, 55, 53, 35, 50, 66,
        56, 48, 52, 65, 62};

    // The labels for the area chart
    double labels[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
        22, 23, 24};

    // Create a XYChart object of size 320 x 320 pixels
    XYChart *c = new XYChart(320, 320);

    // Swap the x and y axis to become a rotated chart
    c->swapXY();

    // Set the y axis on the top side (right + rotated = top)
    c->setYAxisOnRight();

    // Reverse the x axis so it is pointing downwards
    c->xAxis()->setReverse();

    // Set the plotarea at (50, 50) and of size 200 x 200 pixels. Enable horizontal and vertical
    // grids by setting their colors to grey (0xc0c0c0).
    c->setPlotArea(50, 50, 250, 250)->setGridColor(0xc0c0c0, 0xc0c0c0);

    // Add a line chart layer using the given data
    c->addAreaLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), c->gradientColor(50,
        0, 300, 0, 0xffffff, 0x0000ff));

    // Set the labels on the x axis. Append "m" after the value to show the unit.
    c->xAxis()->setLabels(DoubleArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))),
        "{value} m");

    // Display 1 out of 3 labels.
    c->xAxis()->setLabelStep(3);

    // Add a title to the x axis
    c->xAxis()->setTitle("Depth");

    // Add a title to the y axis
    c->yAxis()->setTitle("Carbon Dioxide Concentration (ppm)");

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='Carbon dioxide concentration at {xLabel}: {value} ppm'");

    return c;
}


BaseChart *boxwhisker(int /* chartIndex */, const char **imageMap)
{
    // Sample data for the Box-Whisker chart. Represents the minimum, 1st quartile, medium, 3rd
    // quartile and maximum values of some quantities
    double Q0Data[] = {40, 45, 40, 30, 20, 50, 25, 44};
    double Q1Data[] = {55, 60, 50, 40, 38, 60, 51, 60};
    double Q2Data[] = {62, 70, 60, 50, 48, 70, 62, 70};
    double Q3Data[] = {70, 80, 65, 60, 53, 78, 69, 76};
    double Q4Data[] = {80, 90, 75, 70, 60, 85, 80, 84};

    // The labels for the chart
    const char *labels[] = {"Group A", "Group B", "Group C", "Group D", "Group E", "Group F",
        "Group G", "Group H"};

    // Create a XYChart object of size 550 x 250 pixels
    XYChart *c = new XYChart(550, 250);

    // Set the plotarea at (50, 25) and of size 450 x 200 pixels. Enable both horizontal and
    // vertical grids by setting their colors to grey (0xc0c0c0)
    c->setPlotArea(50, 25, 450, 200)->setGridColor(0xc0c0c0, 0xc0c0c0);

    // Add a title to the chart
    c->addTitle("Computer Vision Test Scores");

    // Set the labels on the x axis and the font to Arial Bold
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0])))
        )->setFontStyle("arialbd.ttf");

    // Set the font for the y axis labels to Arial Bold
    c->yAxis()->setLabelStyle("arialbd.ttf");

    // Add a Box Whisker layer using light blue 0x9999ff as the fill color and blue (0xcc) as the
    // line color. Set the line width to 2 pixels
    c->addBoxWhiskerLayer(DoubleArray(Q3Data, (int)(sizeof(Q3Data) / sizeof(Q3Data[0]))),
        DoubleArray(Q1Data, (int)(sizeof(Q1Data) / sizeof(Q1Data[0]))), DoubleArray(Q4Data, (int)(
        sizeof(Q4Data) / sizeof(Q4Data[0]))), DoubleArray(Q0Data, (int)(sizeof(Q0Data) / sizeof(
        Q0Data[0]))), DoubleArray(Q2Data, (int)(sizeof(Q2Data) / sizeof(Q2Data[0]))), 0x9999ff,
        0x0000cc)->setLineWidth(2);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{xLabel}: min/med/max = {min}/{med}/{max}\nInter-quartile range: {bottom} to {top}'"
        );

    return c;
}


BaseChart *boxwhisker2(int /* chartIndex */, const char **imageMap)
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
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='[{xLabel}] min/med/max = {min}/{med}/{max}\nInter-quartile range: {bottom} to "
        "{top}'");

    return c;
}


BaseChart *hboxwhisker(int /* chartIndex */, const char **imageMap)
{
    // Sample data for the Box-Whisker chart. Represents the minimum, 1st quartile, medium, 3rd
    // quartile and maximum values of some quantities
    double Q0Data[] = {40, 45, 35};
    double Q1Data[] = {55, 60, 50};
    double Q2Data[] = {62, 70, 60};
    double Q3Data[] = {70, 80, 65};
    double Q4Data[] = {80, 90, 75};

    // The labels for the chart
    const char *labels[] = {"<*img=robot1.png*><*br*>Bipedal Type",
        "<*img=robot2.png*><*br*>Wolf Type", "<*img=robot5.png*><*br*>Bird Type"};

    // Create a XYChart object of size 540 x 320 pixels
    XYChart *c = new XYChart(540, 320);

    // swap the x and y axes to create a horizontal box-whisker chart
    c->swapXY();

    // Set the plotarea at (75, 25) and of size 440 x 270 pixels. Enable both horizontal and
    // vertical grids by setting their colors to grey (0xc0c0c0)
    c->setPlotArea(75, 25, 440, 270)->setGridColor(0xc0c0c0, 0xc0c0c0);

    // Add a title to the chart
    c->addTitle("           Robot Shooting Accuracy Scores");

    // Set the labels on the x axis and the font to Arial Bold
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0])))
        )->setFontStyle("arialbd.ttf");

    // Disable x axis ticks by setting the length to 0
    c->xAxis()->setTickLength(0);

    // Set the font for the y axis labels to Arial Bold
    c->yAxis()->setLabelStyle("arialbd.ttf");

    // Add a Box Whisker layer using light blue 0x9999ff as the fill color and blue (0xcc) as the
    // line color. Set the line width to 2 pixels
    c->addBoxWhiskerLayer2(DoubleArray(Q3Data, (int)(sizeof(Q3Data) / sizeof(Q3Data[0]))),
        DoubleArray(Q1Data, (int)(sizeof(Q1Data) / sizeof(Q1Data[0]))), DoubleArray(Q4Data, (int)(
        sizeof(Q4Data) / sizeof(Q4Data[0]))), DoubleArray(Q0Data, (int)(sizeof(Q0Data) / sizeof(
        Q0Data[0]))), DoubleArray(Q2Data, (int)(sizeof(Q2Data) / sizeof(Q2Data[0]))))->setLineWidth(
        2);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{xLabel}: min/med/max = {min}/{med}/{max}\n Inter-quartile range: {bottom} to "
        "{top}'");

    return c;
}


BaseChart *floatingbox(int /* chartIndex */, const char **imageMap)
{
    // Sample data for the Box-Whisker chart. Represents the minimum, 1st quartile, medium, 3rd
    // quartile and maximum values of some quantities
    double Q0Data[] = {40, 45, 40, 30, 20, 50, 25, 44};
    double Q1Data[] = {55, 60, 50, 40, 38, 60, 51, 60};
    double Q2Data[] = {62, 70, 60, 50, 48, 70, 62, 70};
    double Q3Data[] = {70, 80, 65, 60, 53, 78, 69, 76};
    double Q4Data[] = {80, 90, 75, 70, 60, 85, 80, 84};

    // The labels for the chart
    const char *labels[] = {"Group A", "Group B", "Group C", "Group D", "Group E", "Group F",
        "Group G", "Group H"};

    // Create a XYChart object of size 550 x 250 pixels
    XYChart *c = new XYChart(550, 275);

    // Set the plotarea at (50, 25) and of size 450 x 200 pixels. Enable both horizontal and
    // vertical grids by setting their colors to grey (0xc0c0c0)
    c->setPlotArea(50, 50, 450, 200)->setGridColor(0xc0c0c0, 0xc0c0c0);

    // Add a title to the chart
    c->addTitle("Computer Vision Test Scores");

    // Set the labels on the x axis and the font to Arial Bold
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0])))
        )->setFontStyle("arialbd.ttf");

    // Set the font for the y axis labels to Arial Bold
    c->yAxis()->setLabelStyle("arialbd.ttf");

    // Add a Box Whisker layer using light blue 0x9999ff as the fill color and blue (0xcc) as the
    // line color. Set the line width to 2 pixels
    c->addBoxLayer(DoubleArray(Q4Data, (int)(sizeof(Q4Data) / sizeof(Q4Data[0]))), DoubleArray(
        Q3Data, (int)(sizeof(Q3Data) / sizeof(Q3Data[0]))), 0x00ff00, "Top 25%");
    c->addBoxLayer(DoubleArray(Q3Data, (int)(sizeof(Q3Data) / sizeof(Q3Data[0]))), DoubleArray(
        Q2Data, (int)(sizeof(Q2Data) / sizeof(Q2Data[0]))), 0x9999ff, "25% - 50%");
    c->addBoxLayer(DoubleArray(Q2Data, (int)(sizeof(Q2Data) / sizeof(Q2Data[0]))), DoubleArray(
        Q1Data, (int)(sizeof(Q1Data) / sizeof(Q1Data[0]))), 0xffff00, "50% - 75%");
    c->addBoxLayer(DoubleArray(Q1Data, (int)(sizeof(Q1Data) / sizeof(Q1Data[0]))), DoubleArray(
        Q0Data, (int)(sizeof(Q0Data) / sizeof(Q0Data[0]))), 0xff0000, "Bottom 25%");

    // Add legend box at top center above the plot area using 10pt Arial Bold Font
    LegendBox *b = c->addLegend(50 + 225, 22, false, "arialbd.ttf", 10);
    b->setAlignment(Chart::TopCenter);
    b->setBackground(Chart::Transparent);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{xLabel} ({dataSetName}): {bottom} to {top} points'");

    return c;
}


BaseChart *waterfall(int /* chartIndex */, const char **imageMap)
{
    // 4 data points to represent the cash flow for the Q1 - Q4
    double data[] = {230, 140, 220, 330, 150};

    // We want to plot a waterfall chart showing the 4 quarters as well as the total
    const char *labels[] = {"Product 1", "Product 2", "Product 3", "Product 4", "Product 5", "Total"
        };

    // The top side of the bars in a waterfall chart is the accumulated data. We use the
    // ChartDirector ArrayMath utility to accumulate the data. The "total" is handled by inserting a
    // zero point at the end before accumulation (after accumulation it will become the total).
    ArrayMath boxTop = ArrayMath(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0])))).insert(0,
        1).acc();

    // The botom side of the bars is just the top side of the previous bar. So we shifted the top
    // side data to obtain the bottom side data.
    ArrayMath boxBottom = ArrayMath(boxTop).shift(1, 0);

    // The last point (total) is different. Its bottom side is always 0.
    boxBottom.trim(0, (int)(sizeof(data) / sizeof(data[0]))).insert(0, 1);

    // Create a XYChart object of size 500 x 280 pixels. Set background color to light blue
    // (ccccff), with 1 pixel 3D border effect.
    XYChart *c = new XYChart(500, 290, 0xccccff, 0x000000, 1);

    // Add a title to the chart using 13 points Arial Bold Itatic font, with white (ffffff) text on
    // a deep blue (0x80) background
    c->addTitle("Product Revenue - Year 2004", "arialbi.ttf", 13, 0xffffff)->setBackground(0x000080)
        ;

    // Set the plotarea at (55, 50) and of size 430 x 215 pixels. Use alternative white/grey
    // background.
    c->setPlotArea(55, 45, 430, 215, 0xffffff, 0xeeeeee);

    // Set the labels on the x axis using Arial Bold font
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0])))
        )->setFontStyle("arialbd.ttf");

    // Set the x-axis ticks and grid lines to be between the bars
    c->xAxis()->setTickOffset(0.5);

    // Use Arial Bold as the y axis label font
    c->yAxis()->setLabelStyle("arialbd.ttf");

    // Add a title to the y axis
    c->yAxis()->setTitle("USD (in millions)");

    // Add a multi-color box-whisker layer to represent the waterfall bars
    BoxWhiskerLayer *layer = c->addBoxWhiskerLayer2(boxTop, boxBottom);

    // Put data labels on the bars to show the cash flow using Arial Bold font
    layer->setDataLabelFormat("{={top}-{bottom}}M");
    layer->setDataLabelStyle("arialbd.ttf")->setAlignment(Chart::Center);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{xLabel}: {={top}-{bottom}} millions'");

    return c;
}


BaseChart *posnegwaterfall(int /* chartIndex */, const char **imageMap)
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
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{xLabel}: {={top}-{bottom}} millions'");

    return c;
}


BaseChart *gantt(int /* chartIndex */, const char **imageMap)
{
    // data for the gantt chart, representing the start date, end date and names for various
    // activities
    double startDate[] = {Chart::chartTime(2004, 8, 16), Chart::chartTime(2004, 8, 30),
        Chart::chartTime(2004, 9, 13), Chart::chartTime(2004, 9, 20), Chart::chartTime(2004, 9, 27),
        Chart::chartTime(2004, 10, 4), Chart::chartTime(2004, 10, 25), Chart::chartTime(2004, 11, 1
        ), Chart::chartTime(2004, 11, 8)};
    double endDate[] = {Chart::chartTime(2004, 8, 30), Chart::chartTime(2004, 9, 13),
        Chart::chartTime(2004, 9, 27), Chart::chartTime(2004, 10, 4), Chart::chartTime(2004, 10, 11
        ), Chart::chartTime(2004, 11, 8), Chart::chartTime(2004, 11, 8), Chart::chartTime(2004, 11,
        22), Chart::chartTime(2004, 11, 22)};
    const char *labels[] = {"Market Research", "Define Specifications", "Overall Archiecture",
        "Project Planning", "Detail Design", "Software Development", "Test Plan", "Testing and QA",
        "User Documentation"};

    // Create a XYChart object of size 620 x 280 pixels. Set background color to light blue
    // (ccccff), with 1 pixel 3D border effect.
    XYChart *c = new XYChart(620, 280, 0xccccff, 0x000000, 1);

    // Add a title to the chart using 15 points Times Bold Itatic font, with white (ffffff) text on
    // a deep blue (000080) background
    c->addTitle("Simple Gantt Chart Demo", "timesbi.ttf", 15, 0xffffff)->setBackground(0x000080);

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

    // Add a green (33ff33) box-whisker layer showing the box only.
    c->addBoxWhiskerLayer(DoubleArray(startDate, (int)(sizeof(startDate) / sizeof(startDate[0]))),
        DoubleArray(endDate, (int)(sizeof(endDate) / sizeof(endDate[0]))), DoubleArray(),
        DoubleArray(), DoubleArray(), 0x00cc00, Chart::SameAsMainColor, Chart::SameAsMainColor);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{xLabel}: {top|mmm dd, yyyy} to {bottom|mmm dd, yyyy}'");

    return c;
}


BaseChart *colorgantt(int /* chartIndex */, const char **imageMap)
{
    // The tasks for the gantt chart
    const char *labels[] = {"Market Research", "Define Specifications", "Overall Archiecture",
        "Project Planning", "Detail Design", "Software Development", "Test Plan", "Testing and QA",
        "User Documentation"};

    // The task index, start date, end date and color for each bar
    double taskNo[] = {0, 0, 1, 2, 3, 4, 5, 6, 6, 7, 8, 8};
    double startDate[] = {Chart::chartTime(2004, 8, 16), Chart::chartTime(2004, 10, 4),
        Chart::chartTime(2004, 8, 30), Chart::chartTime(2004, 9, 13), Chart::chartTime(2004, 9, 20),
        Chart::chartTime(2004, 9, 27), Chart::chartTime(2004, 10, 4), Chart::chartTime(2004, 10, 4),
        Chart::chartTime(2004, 10, 25), Chart::chartTime(2004, 11, 1), Chart::chartTime(2004, 10, 18
        ), Chart::chartTime(2004, 11, 8)};
    double endDate[] = {Chart::chartTime(2004, 8, 30), Chart::chartTime(2004, 10, 18),
        Chart::chartTime(2004, 9, 13), Chart::chartTime(2004, 9, 27), Chart::chartTime(2004, 10, 4),
        Chart::chartTime(2004, 10, 11), Chart::chartTime(2004, 11, 8), Chart::chartTime(2004, 10, 18
        ), Chart::chartTime(2004, 11, 8), Chart::chartTime(2004, 11, 22), Chart::chartTime(2004, 11,
        1), Chart::chartTime(2004, 11, 22)};
    int colors[] = {0x00cc00, 0x00cc00, 0x00cc00, 0x0000cc, 0x0000cc, 0xcc0000, 0xcc0000, 0x0000cc,
        0xcc0000, 0xcc0000, 0x00cc00, 0xcc0000};

    // Create a XYChart object of size 620 x 325 pixels. Set background color to light red
    // (0xffcccc), with 1 pixel 3D border effect.
    XYChart *c = new XYChart(620, 325, 0xffcccc, 0x000000, 1);

    // Add a title to the chart using 15 points Times Bold Itatic font, with white (ffffff) text on
    // a dark red (800000) background
    c->addTitle("Mutli-Color Gantt Chart Demo", "timesbi.ttf", 15, 0xffffff)->setBackground(0x800000
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

    // Add some symbols to the chart to represent milestones. The symbols are added using scatter
    // layers. We need to specify the task index, date, name, symbol shape, size and color.
    double coor1[] = {1};
    double date1[] = {Chart::chartTime(2004, 9, 13)};
    c->addScatterLayer(DoubleArray(coor1, (int)(sizeof(coor1) / sizeof(coor1[0]))), DoubleArray(
        date1, (int)(sizeof(date1) / sizeof(date1[0]))), "Milestone 1", Chart::Cross2Shape(), 13,
        0xffff00)->setHTMLImageMap("{disable}");
    double coor2[] = {3};
    double date2[] = {Chart::chartTime(2004, 10, 4)};
    c->addScatterLayer(DoubleArray(coor2, (int)(sizeof(coor2) / sizeof(coor2[0]))), DoubleArray(
        date2, (int)(sizeof(date2) / sizeof(date2[0]))), "Milestone 2", Chart::StarShape(5), 15,
        0xff00ff)->setHTMLImageMap("{disable}");
    double coor3[] = {5};
    double date3[] = {Chart::chartTime(2004, 11, 8)};
    c->addScatterLayer(DoubleArray(coor3, (int)(sizeof(coor3) / sizeof(coor3[0]))), DoubleArray(
        date3, (int)(sizeof(date3) / sizeof(date3[0]))), "Milestone 3", Chart::TriangleSymbol, 13,
        0xff9933)->setHTMLImageMap("{disable}");

    // Add a multi-color box-whisker layer to represent the gantt bars
    BoxWhiskerLayer *layer = c->addBoxWhiskerLayer2(DoubleArray(startDate, (int)(sizeof(startDate) /
        sizeof(startDate[0]))), DoubleArray(endDate, (int)(sizeof(endDate) / sizeof(endDate[0]))),
        DoubleArray(), DoubleArray(), DoubleArray(), IntArray(colors, (int)(sizeof(colors) / sizeof(
        colors[0]))));
    layer->setXData(DoubleArray(taskNo, (int)(sizeof(taskNo) / sizeof(taskNo[0]))));
    layer->setBorderColor(Chart::SameAsMainColor);

    // Divide the plot area height ( = 200 in this chart) by the number of tasks to get the height
    // of each slot. Use 80% of that as the bar height.
    layer->setDataWidth(200 * 4 / 5 / (int)(sizeof(labels) / sizeof(labels[0])));

    // Add a legend box at (140, 265) - bottom of the plot area. Use 8pt Arial Bold as the font with
    // auto-grid layout. Set the width to the same width as the plot area. Set the backgorund to
    // grey (dddddd).
    LegendBox *legendBox = c->addLegend2(140, 265, Chart::AutoGrid, "arialbd.ttf", 8);
    legendBox->setWidth(461);
    legendBox->setBackground(0xdddddd);

    // The keys for the scatter layers (milestone symbols) will automatically be added to the legend
    // box. We just need to add keys to show the meanings of the bar colors.
    legendBox->addKey("Market Team", 0x00cc00);
    legendBox->addKey("Planning Team", 0x0000cc);
    legendBox->addKey("Development Team", 0xcc0000);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{xLabel}: {top|mmm dd, yyyy} to {bottom|mmm dd, yyyy}'");

    return c;
}


BaseChart *layergantt(int /* chartIndex */, const char **imageMap)
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
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{xLabel} ({dataSetName}): {top|mmm dd, yyyy} to {bottom|mmm dd, yyyy}'");

    return c;
}


BaseChart *contour(int /* chartIndex */, const char ** /* imageMap */)
{
    // The x and y coordinates of the grid
    double dataX[] = {-10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    double dataY[] = {-10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // The values at the grid points. In this example, we will compute the values using the formula
    // z = x * sin(y) + y * sin(x).
    double dataZ[(int)(sizeof(dataX) / sizeof(dataX[0])) * (int)(sizeof(dataY) / sizeof(dataY[0]))];
    for(int yIndex = 0; yIndex < (int)(sizeof(dataY) / sizeof(dataY[0])); ++yIndex) {
        double y = dataY[yIndex];
        for(int xIndex = 0; xIndex < (int)(sizeof(dataX) / sizeof(dataX[0])); ++xIndex) {
            double x = dataX[xIndex];
            dataZ[yIndex * (int)(sizeof(dataX) / sizeof(dataX[0])) + xIndex] = x * sin(y) + y * sin(
                x);
        }
    }

    // Create a XYChart object of size 600 x 500 pixels
    XYChart *c = new XYChart(600, 500);

    // Add a title to the chart using 15 points Arial Bold Italic font
    c->addTitle("z = x * sin(y) + y * sin(x)      ", "arialbi.ttf", 15);

    // Set the plotarea at (75, 40) and of size 400 x 400 pixels. Use semi-transparent black
    // (80000000) dotted lines for both horizontal and vertical grid lines
    c->setPlotArea(75, 40, 400, 400, -1, -1, -1, c->dashLineColor(0x80000000, Chart::DotLine), -1);

    // Set x-axis and y-axis title using 12 points Arial Bold Italic font
    c->xAxis()->setTitle("X-Axis Title Place Holder", "arialbi.ttf", 12);
    c->yAxis()->setTitle("Y-Axis Title Place Holder", "arialbi.ttf", 12);

    // Set x-axis and y-axis labels to use Arial Bold font
    c->xAxis()->setLabelStyle("arialbd.ttf");
    c->yAxis()->setLabelStyle("arialbd.ttf");

    // When auto-scaling, use tick spacing of 40 pixels as a guideline
    c->yAxis()->setTickDensity(40);
    c->xAxis()->setTickDensity(40);

    // Add a contour layer using the given data
    ContourLayer *layer = c->addContourLayer(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0
        ]))), DoubleArray(dataY, (int)(sizeof(dataY) / sizeof(dataY[0]))), DoubleArray(dataZ, (int)(
        sizeof(dataZ) / sizeof(dataZ[0]))));

    // Move the grid lines in front of the contour layer
    c->getPlotArea()->moveGridBefore(layer);

    // Add a color axis (the legend) in which the top left corner is anchored at (505, 40). Set the
    // length to 400 pixels and the labels on the right side.
    ColorAxis *cAxis = layer->setColorAxis(505, 40, Chart::TopLeft, 400, Chart::Right);

    // Add a title to the color axis using 12 points Arial Bold Italic font
    cAxis->setTitle("Color Legend Title Place Holder", "arialbi.ttf", 12);

    // Set color axis labels to use Arial Bold font
    cAxis->setLabelStyle("arialbd.ttf");

    // Output the chart
    c->makeChart();

    return c;
}


BaseChart *scattercontour(int /* chartIndex */, const char ** /* imageMap */)
{
    // The (x, y, z) coordinates of the scattered data
    double dataX[] = {0.5, 1.9, 4.9, 1.0, 8.9, 9.8, 5.9, 2.9, 6.8, 9.0, 0.0, 8.9, 1.9, 4.8, 2.4,
        3.4, 7.9, 7.5, 4.8, 7.5, 9.5, 0.4, 8.9, 0.9, 5.4, 9.4, 2.9, 8.9, 0.9, 8.9, 10.0, 1.0, 6.8,
        3.8, 9.0, 5.3, 6.4, 4.9, 4.5, 2.0, 5.4, 0.0, 10.0, 3.9, 5.4, 5.9, 5.8, 0.3, 4.4, 8.3};
    double dataY[] = {3.3, 3.0, 0.7, 1.0, 9.3, 4.5, 8.4, 0.1, 0.8, 0.1, 9.3, 1.8, 4.3, 1.3, 2.3,
        5.4, 6.9, 9.0, 9.8, 7.5, 1.8, 1.4, 4.5, 7.8, 3.8, 4.0, 2.9, 2.4, 3.9, 2.9, 2.3, 9.3, 2.0,
        3.4, 4.8, 2.3, 3.4, 2.3, 1.5, 7.8, 4.5, 0.9, 6.3, 2.4, 6.9, 2.8, 1.3, 2.9, 6.4, 6.3};
    double dataZ[] = {6.6, 12.5, 7.4, 6.2, 9.6, 13.6, 19.9, 2.2, 6.9, 3.4, 8.7, 8.4, 7.8, 8.0, 9.4,
        11.9, 9.6, 15.7, 12.0, 13.3, 9.6, 6.4, 9.0, 6.9, 4.6, 9.7, 10.6, 9.2, 7.0, 6.9, 9.7, 8.6,
        8.0, 13.6, 13.2, 5.9, 9.0, 3.2, 8.3, 9.7, 8.2, 6.1, 8.7, 5.6, 14.9, 9.8, 9.3, 5.1, 10.8, 9.8
        };

    // Create a XYChart object of size 450 x 540 pixels
    XYChart *c = new XYChart(450, 540);

    // Add a title to the chart using 15 points Arial Italic font
    c->addTitle("      Contour Chart with Scattered Data", "ariali.ttf", 15);

    // Set the plotarea at (65, 40) and of size 360 x 360 pixels. Use semi-transparent black
    // (c0000000) for both horizontal and vertical grid lines
    c->setPlotArea(65, 40, 360, 360, -1, -1, -1, 0xc0000000, -1);

    // Set x-axis and y-axis title using 12 points Arial Bold Italic font
    c->xAxis()->setTitle("X-Axis Title Place Holder", "arialbi.ttf", 10);
    c->yAxis()->setTitle("Y-Axis Title Place Holder", "arialbi.ttf", 10);

    // Set x-axis and y-axis labels to use Arial Bold font
    c->xAxis()->setLabelStyle("arialbd.ttf");
    c->yAxis()->setLabelStyle("arialbd.ttf");

    // When x-axis and y-axis color to transparent
    c->xAxis()->setColors(Chart::Transparent);
    c->yAxis()->setColors(Chart::Transparent);

    // Add a scatter layer to the chart to show the position of the data points
    c->addScatterLayer(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0]))), DoubleArray(
        dataY, (int)(sizeof(dataY) / sizeof(dataY[0]))), "", Chart::Cross2Shape(0.2), 7, 0x000000);

    // Add a contour layer using the given data
    ContourLayer *layer = c->addContourLayer(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0
        ]))), DoubleArray(dataY, (int)(sizeof(dataY) / sizeof(dataY[0]))), DoubleArray(dataZ, (int)(
        sizeof(dataZ) / sizeof(dataZ[0]))));

    // Move the grid lines in front of the contour layer
    c->getPlotArea()->moveGridBefore(layer);

    // Add a color axis (the legend) in which the top center is anchored at (245, 455). Set the
    // length to 330 pixels and the labels on the top side.
    ColorAxis *cAxis = layer->setColorAxis(245, 455, Chart::TopCenter, 330, Chart::Top);

    // Add a bounding box to the color axis using the default line color as border.
    cAxis->setBoundingBox(Chart::Transparent, Chart::LineColor);

    // Add a title to the color axis using 12 points Arial Bold Italic font
    cAxis->setTitle("Color Legend Title Place Holder", "arialbi.ttf", 10);

    // Set color axis labels to use Arial Bold font
    cAxis->setLabelStyle("arialbd.ttf");

    // Set the color axis range as 0 to 20, with a step every 2 units
    cAxis->setLinearScale(0, 20, 2);

    // Output the chart
    c->makeChart();

    return c;
}


BaseChart *contourcolor(int chartIndex, const char ** /* imageMap */)
{
    // The x and y coordinates of the grid
    double dataX[] = {-4, -3, -2, -1, 0, 1, 2, 3, 4};
    double dataY[] = {-4, -3, -2, -1, 0, 1, 2, 3, 4};

    // Use random numbers for the z values on the XY grid
    RanSeries *r = new RanSeries(99);
    DoubleArray dataZ = r->get2DSeries((int)(sizeof(dataX) / sizeof(dataX[0])), (int)(sizeof(dataY)
         / sizeof(dataY[0])), -0.9, 0.9);

    // Create a XYChart object of size 420 x 360 pixels
    XYChart *c = new XYChart(420, 360);

    // Set the plotarea at (30, 25) and of size 300 x 300 pixels. Use semi-transparent grey
    // (0xdd000000) horizontal and vertical grid lines
    c->setPlotArea(30, 25, 300, 300, -1, -1, -1, 0xdd000000, -1);

    // Set the x-axis and y-axis scale
    c->xAxis()->setLinearScale(-4, 4, 1);
    c->yAxis()->setLinearScale(-4, 4, 1);

    // Add a contour layer using the given data
    ContourLayer *layer = c->addContourLayer(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0
        ]))), DoubleArray(dataY, (int)(sizeof(dataY) / sizeof(dataY[0]))), dataZ);

    // Move the grid lines in front of the contour layer
    c->getPlotArea()->moveGridBefore(layer);

    // Add a color axis (the legend) in which the top left corner is anchored at (350, 25). Set the
    // length to 400 300 and the labels on the right side.
    ColorAxis *cAxis = layer->setColorAxis(350, 25, Chart::TopLeft, 300, Chart::Right);

    if (chartIndex == 1) {
        // Speicify a color gradient as a list of colors, and use it in the color axis.
        int colorGradient[] = {0x0044cc, 0xffffff, 0x00aa00};
        cAxis->setColorGradient(false, IntArray(colorGradient, (int)(sizeof(colorGradient) / sizeof(
            colorGradient[0]))));
    } else if (chartIndex == 2) {
        // Specify the color scale to use in the color axis
        double colorScale[] = {-1.0, 0x1a9850, -0.75, 0x66bd63, -0.5, 0xa6d96a, -0.25, 0xd9ef8b, 0,
            0xfee08b, 0.25, 0xfdae61, 0.5, 0xf46d43, 0.75, 0xd73027, 1};
        cAxis->setColorScale(DoubleArray(colorScale, (int)(sizeof(colorScale) / sizeof(colorScale[0]
            ))));
    } else if (chartIndex == 3) {
        // Specify the color scale to use in the color axis. Also specify an underflow color
        // 0x66ccff (blue) for regions that fall below the lower axis limit.
        double colorScale[] = {0, 0xffff99, 0.2, 0x80cdc1, 0.4, 0x35978f, 0.6, 0x01665e, 0.8,
            0x003c30, 1};
        cAxis->setColorScale(DoubleArray(colorScale, (int)(sizeof(colorScale) / sizeof(colorScale[0]
            ))), 0x66ccff);
    }

    // Output the chart
    c->makeChart();

    //free up resources
    delete r;

    return c;
}


BaseChart *contourlegend(int /* chartIndex */, const char ** /* imageMap */)
{
    // The x and y coordinates of the grid
    double dataX[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    double dataY[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Use random numbers for the z values on the XY grid
    RanSeries *r = new RanSeries(999);
    DoubleArray dataZ = r->get2DSeries((int)(sizeof(dataX) / sizeof(dataX[0])), (int)(sizeof(dataY)
         / sizeof(dataY[0])), -0.9, 1.15);

    // Create a XYChart object of size 640 x 460 pixels
    XYChart *c = new XYChart(640, 460);

    // Set the plotarea at (30, 25) and of size 400 x 400 pixels. Use semi-transparent grey
    // (0xdd000000) horizontal and vertical grid lines
    c->setPlotArea(50, 25, 400, 400, -1, -1, Chart::Transparent, 0xdd000000, -1);

    // Set the x and y axis stems to transparent and the label font to 12pt Arial
    c->xAxis()->setColors(Chart::Transparent);
    c->yAxis()->setColors(Chart::Transparent);
    c->xAxis()->setLabelStyle("arial.ttf", 12);
    c->yAxis()->setLabelStyle("arial.ttf", 12);

    // Set the x-axis and y-axis scale
    c->xAxis()->setLinearScale(0, 10, 1);
    c->yAxis()->setLinearScale(0, 10, 1);

    // Add a contour layer using the given data
    ContourLayer *layer = c->addContourLayer(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0
        ]))), DoubleArray(dataY, (int)(sizeof(dataY) / sizeof(dataY[0]))), dataZ);

    // Move the grid lines in front of the contour layer
    c->getPlotArea()->moveGridBefore(layer);

    // Define the color scale
    double colorScale[] = {-0.8, 0x0066ff, -0.5, 0x66ccff, -0.3, 0x66ffff, 0, 0x88ff88, 0.4,
        0x00ff00, 0.7, 0xffff00, 0.9, 0xff6600, 1.0, 0xcc0000, 1.1};
    // Apply the color scale, and specify the underflow and overflow colors for regions exceeding
    // the color scale
    layer->colorAxis()->setColorScale(DoubleArray(colorScale, (int)(sizeof(colorScale) / sizeof(
        colorScale[0]))), 0x0000cc, 0x000000);

    //
    // Instead of displaying the color axis, we use a legend box to display the colors. This is
    // useful for colors that are unevenly spaced on the color axis.
    //

    // Add a legend box at (460, 25) with vertical layout, with 12pt Arial font, transparent
    // background and border, icon size of 15 x 15 pixels, and line spacing of 8 pixels.
    LegendBox *b = c->addLegend(460, 25, true, "arial.ttf", 12);
    b->setBackground(Chart::Transparent, Chart::Transparent);
    b->setKeySize(15, 15);
    b->setKeySpacing(0, 8);

    // Add the legend box entries
    b->addKey("> 1.1 (Critical)", 0x000000);
    b->addKey("1.0 to 1.1 (Alert)", 0xcc0000);
    b->addKey("0.9 to 1.0", 0xff6600);
    b->addKey("0.7 to 0.9", 0xffff00);
    b->addKey("0.4 to 0.7", 0x00ff00);
    b->addKey("0.0 to 0.4", 0x88ff88);
    b->addKey("-0.3 to 0.0", 0x66ffff);
    b->addKey("-0.5 to -0.3", 0x66ccff);
    b->addKey("-0.8 to -0.5", 0x0066ff);
    b->addKey("< -0.8", 0x0000cc);

    // Output the chart
    c->makeChart();

    //free up resources
    delete r;

    return c;
}


BaseChart *smoothcontour(int /* chartIndex */, const char ** /* imageMap */)
{
    // The x and y coordinates of the grid
    double dataX[] = {-10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    double dataY[] = {-10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // The values at the grid points. In this example, we will compute the values using the formula
    // z = Sin(x / 2) * Sin(y / 2).
    double dataZ[(int)(sizeof(dataX) / sizeof(dataX[0])) * (int)(sizeof(dataY) / sizeof(dataY[0]))];
    for(int yIndex = 0; yIndex < (int)(sizeof(dataY) / sizeof(dataY[0])); ++yIndex) {
        double y = dataY[yIndex];
        for(int xIndex = 0; xIndex < (int)(sizeof(dataX) / sizeof(dataX[0])); ++xIndex) {
            double x = dataX[xIndex];
            dataZ[yIndex * (int)(sizeof(dataX) / sizeof(dataX[0])) + xIndex] = sin(x / 2.0) * sin(y
                 / 2.0);
        }
    }

    // Create a XYChart object of size 600 x 500 pixels
    XYChart *c = new XYChart(600, 500);

    // Add a title to the chart using 18 points Times New Roman Bold Italic font
    c->addTitle("Nano Lattice Twister Field Intensity        ", "timesbi.ttf", 18);

    // Set the plotarea at (75, 40) and of size 400 x 400 pixels. Use semi-transparent black
    // (80000000) dotted lines for both horizontal and vertical grid lines
    c->setPlotArea(75, 40, 400, 400, -1, -1, -1, c->dashLineColor(0x80000000, Chart::DotLine), -1);

    // Set x-axis and y-axis title using 12 points Arial Bold Italic font
    c->xAxis()->setTitle("Lattice X Direction (nm)", "arialbi.ttf", 12);
    c->yAxis()->setTitle("Lattice Y Direction (nm)", "arialbi.ttf", 12);

    // Set x-axis and y-axis labels to use Arial Bold font
    c->xAxis()->setLabelStyle("arialbd.ttf");
    c->yAxis()->setLabelStyle("arialbd.ttf");

    // When auto-scaling, use tick spacing of 40 pixels as a guideline
    c->yAxis()->setTickDensity(40);
    c->xAxis()->setTickDensity(40);

    // Add a contour layer using the given data
    ContourLayer *layer = c->addContourLayer(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0
        ]))), DoubleArray(dataY, (int)(sizeof(dataY) / sizeof(dataY[0]))), DoubleArray(dataZ, (int)(
        sizeof(dataZ) / sizeof(dataZ[0]))));

    // Set the contour color to transparent
    layer->setContourColor(Chart::Transparent);

    // Move the grid lines in front of the contour layer
    c->getPlotArea()->moveGridBefore(layer);

    // Add a color axis (the legend) in which the left center point is anchored at (495, 240). Set
    // the length to 370 pixels and the labels on the right side.
    ColorAxis *cAxis = layer->setColorAxis(495, 240, Chart::Left, 370, Chart::Right);

    // Add a bounding box to the color axis using light grey (eeeeee) as the background and dark
    // grey (444444) as the border.
    cAxis->setBoundingBox(0xeeeeee, 0x444444);

    // Add a title to the color axis using 12 points Arial Bold Italic font
    cAxis->setTitle("Twist Pressure (Twist-Volt)", "arialbi.ttf", 12);

    // Set color axis labels to use Arial Bold font
    cAxis->setLabelStyle("arialbd.ttf");

    // Use smooth gradient coloring
    cAxis->setColorGradient(true);

    // Output the chart
    c->makeChart();

    return c;
}


BaseChart *contourinterpolate(int chartIndex, const char ** /* imageMap */)
{
    // The x and y coordinates of the grid
    double dataX[] = {-4, -3, -2, -1, 0, 1, 2, 3, 4};
    double dataY[] = {-4, -3, -2, -1, 0, 1, 2, 3, 4};

    // The values at the grid points. In this example, we will compute the values using the formula
    // z = Sin(x * pi / 3) * Sin(y * pi / 3).
    double dataZ[(int)(sizeof(dataX) / sizeof(dataX[0])) * (int)(sizeof(dataY) / sizeof(dataY[0]))];
    for(int yIndex = 0; yIndex < (int)(sizeof(dataY) / sizeof(dataY[0])); ++yIndex) {
        double y = dataY[yIndex];
        for(int xIndex = 0; xIndex < (int)(sizeof(dataX) / sizeof(dataX[0])); ++xIndex) {
            double x = dataX[xIndex];
            dataZ[yIndex * (int)(sizeof(dataX) / sizeof(dataX[0])) + xIndex] = sin(x * 3.1416 / 3) *
                sin(y * 3.1416 / 3);
        }
    }

    // Create a XYChart object of size 360 x 360 pixels
    XYChart *c = new XYChart(360, 360);

    // Set the plotarea at (30, 25) and of size 300 x 300 pixels. Use semi-transparent black
    // (c0000000) for both horizontal and vertical grid lines
    c->setPlotArea(30, 25, 300, 300, -1, -1, -1, 0xc0000000, -1);

    // Add a contour layer using the given data
    ContourLayer *layer = c->addContourLayer(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0
        ]))), DoubleArray(dataY, (int)(sizeof(dataY) / sizeof(dataY[0]))), DoubleArray(dataZ, (int)(
        sizeof(dataZ) / sizeof(dataZ[0]))));

    // Set the x-axis and y-axis scale
    c->xAxis()->setLinearScale(-4, 4, 1);
    c->yAxis()->setLinearScale(-4, 4, 1);

    if (chartIndex == 0) {
        // Discrete coloring, spline surface interpolation
        c->addTitle("Spline Surface - Discrete Coloring", "arialbi.ttf", 12);
    } else if (chartIndex == 1) {
        // Discrete coloring, linear surface interpolation
        c->addTitle("Linear Surface - Discrete Coloring", "arialbi.ttf", 12);
        layer->setSmoothInterpolation(false);
    } else if (chartIndex == 2) {
        // Smooth coloring, spline surface interpolation
        c->addTitle("Spline Surface - Continuous Coloring", "arialbi.ttf", 12);
        layer->setContourColor(Chart::Transparent);
        layer->colorAxis()->setColorGradient(true);
    } else {
        // Discrete coloring, linear surface interpolation
        c->addTitle("Linear Surface - Continuous Coloring", "arialbi.ttf", 12);
        layer->setSmoothInterpolation(false);
        layer->setContourColor(Chart::Transparent);
        layer->colorAxis()->setColorGradient(true);
    }

    // Output the chart
    c->makeChart();

    return c;
}


BaseChart *hloc(int /* chartIndex */, const char **imageMap)
{
    //
    // Sample data for the HLOC chart.
    //
    double highData[] = {2043, 2039, 2076, 2064, 2048, 2058, 2070, 2033, 2027, 2029, 2071, 2085,
        2034, 2031, 2056, 2128, 2180, 2183, 2192, 2213, 2230, 2281, 2272};

    double lowData[] = {1931, 1921, 1985, 2028, 1986, 1994, 1999, 1958, 1943, 1944, 1962, 2011,
        1975, 1962, 1928, 2059, 2112, 2103, 2151, 2127, 2123, 2152, 2212};

    double openData[] = {2000, 1957, 1993, 2037, 2018, 2021, 2045, 2009, 1959, 1985, 2008, 2048,
        2006, 2010, 1971, 2080, 2116, 2137, 2170, 2172, 2171, 2191, 2240};

    double closeData[] = {1950, 1991, 2026, 2029, 2004, 2053, 2011, 1962, 1987, 2019, 2040, 2016,
        1996, 1985, 2006, 2113, 2142, 2167, 2158, 2201, 2188, 2231, 2242};

    // The labels for the HLOC chart
    const char *labels[] = {"Mon 1", "Tue 2", "Wed 3", "Thu 4", "Fri 5", "Mon 8", "Tue 9", "Wed 10",
        "Thu 11", "Fri 12", "Mon 15", "Tue 16", "Wed 17", "Thu 18", "Fri 19", "Mon 22", "Tue 23",
        "Wed 24", "Thu 25", "Fri 26", "Mon 29", "Tue 30", "Wed 31"};

    // Create a XYChart object of size 600 x 350 pixels
    XYChart *c = new XYChart(600, 350);

    // Set the plotarea at (50, 25) and of size 500 x 250 pixels. Enable both the horizontal and
    // vertical grids by setting their colors to grey (0xc0c0c0)
    c->setPlotArea(50, 25, 500, 250)->setGridColor(0xc0c0c0, 0xc0c0c0);

    // Add a title to the chart
    c->addTitle("Universal Stock Index on Jan 2001");

    // Add a custom text at (50, 25) (the upper left corner of the plotarea). Use 12pt Arial
    // Bold/blue (4040c0) as the font.
    c->addText(50, 25, "(c) Global XYZ ABC Company", "arialbd.ttf", 12, 0x4040c0);

    // Add a title to the x axis
    c->xAxis()->setTitle("Jan 2001");

    // Set the labels on the x axis. Rotate the labels by 45 degrees.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0])))
        )->setFontAngle(45);

    // Add a title to the y axis
    c->yAxis()->setTitle("Universal Stock Index");

    // Draw the y axis on the right hand side of the plot area
    c->setYAxisOnRight(true);

    // Add a HLOC layer to the chart using green (008000) for up days and red (ff0000) for down days
    HLOCLayer *layer = c->addHLOCLayer(DoubleArray(highData, (int)(sizeof(highData) / sizeof(
        highData[0]))), DoubleArray(lowData, (int)(sizeof(lowData) / sizeof(lowData[0]))),
        DoubleArray(openData, (int)(sizeof(openData) / sizeof(openData[0]))), DoubleArray(closeData,
        (int)(sizeof(closeData) / sizeof(closeData[0]))), 0x008000, 0xff0000);

    // Set the line width to 2 pixels
    layer->setLineWidth(2);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{xLabel} Jan 2001\nHigh:{high}\nOpen:{open}\nClose:{close}\nLow:{low}'");

    return c;
}


BaseChart *candlestick(int /* chartIndex */, const char **imageMap)
{
    //
    // Sample data for the CandleStick chart.
    //
    double highData[] = {2043, 2039, 2076, 2064, 2048, 2058, 2070, 2033, 2027, 2029, 2071, 2085,
        2034, 2031, 2056, 2128, 2180, 2183, 2192, 2213, 2230, 2281, 2272};

    double lowData[] = {1931, 1921, 1985, 2028, 1986, 1994, 1999, 1958, 1943, 1944, 1962, 2011,
        1975, 1962, 1928, 2059, 2112, 2103, 2151, 2127, 2123, 2152, 2212};

    double openData[] = {2000, 1957, 1993, 2037, 2018, 2021, 2045, 2009, 1959, 1985, 2008, 2048,
        2006, 2010, 1971, 2080, 2116, 2137, 2170, 2172, 2171, 2191, 2240};

    double closeData[] = {1950, 1991, 2026, 2029, 2004, 2053, 2011, 1962, 1987, 2019, 2040, 2016,
        1996, 1985, 2006, 2113, 2142, 2167, 2158, 2201, 2188, 2231, 2242};

    // The labels for the CandleStick chart
    const char *labels[] = {"Mon 1", "Tue 2", "Wed 3", "Thu 4", "Fri 5", "Mon 8", "Tue 9", "Wed 10",
        "Thu 11", "Fri 12", "Mon 15", "Tue 16", "Wed 17", "Thu 18", "Fri 19", "Mon 22", "Tue 23",
        "Wed 24", "Thu 25", "Fri 26", "Mon 29", "Tue 30", "Wed 31"};

    // Create a XYChart object of size 600 x 350 pixels
    XYChart *c = new XYChart(600, 350);

    // Set the plotarea at (50, 25) and of size 500 x 250 pixels. Enable both the horizontal and
    // vertical grids by setting their colors to grey (0xc0c0c0)
    c->setPlotArea(50, 25, 500, 250)->setGridColor(0xc0c0c0, 0xc0c0c0);

    // Add a title to the chart
    c->addTitle("Universal Stock Index on Jan 2001");

    // Add a custom text at (50, 25) (the upper left corner of the plotarea). Use 12pt Arial
    // Bold/blue (4040c0) as the font.
    c->addText(50, 25, "(c) Global XYZ ABC Company", "arialbd.ttf", 12, 0x4040c0);

    // Add a title to the x axis
    c->xAxis()->setTitle("Jan 2001");

    // Set the labels on the x axis. Rotate the labels by 45 degrees.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0])))
        )->setFontAngle(45);

    // Add a title to the y axis
    c->yAxis()->setTitle("Universal Stock Index");

    // Draw the y axis on the right hand side of the plot area
    c->setYAxisOnRight(true);

    // Add a CandleStick layer to the chart using green (00ff00) for up candles and red (ff0000) for
    // down candles
    CandleStickLayer *layer = c->addCandleStickLayer(DoubleArray(highData, (int)(sizeof(highData) /
        sizeof(highData[0]))), DoubleArray(lowData, (int)(sizeof(lowData) / sizeof(lowData[0]))),
        DoubleArray(openData, (int)(sizeof(openData) / sizeof(openData[0]))), DoubleArray(closeData,
        (int)(sizeof(closeData) / sizeof(closeData[0]))), 0x00ff00, 0xff0000);

    // Set the line width to 2 pixels
    layer->setLineWidth(2);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{xLabel} Jan 2001\nHigh:{high}\nOpen:{open}\nClose:{close}\nLow:{low}'");

    return c;
}


BaseChart *finance(int /* chartIndex */, const char ** /* imageMap */)
{
    // Create a finance chart demo containing 100 days of data
    int noOfDays = 100;

    // To compute moving averages starting from the first day, we need to get extra data points
    // before the first day
    int extraDays = 30;

    // In this exammple, we use a random number generator utility to simulate the data. We set up
    // the random table to create 6 cols x (noOfDays + extraDays) rows, using 9 as the seed.
    RanTable *rantable = new RanTable(9, 6, noOfDays + extraDays);

    // Set the 1st col to be the timeStamp, starting from Sep 4, 2002, with each row representing
    // one day, and counting week days only (jump over Sat and Sun)
    rantable->setDateCol(0, Chart::chartTime(2002, 9, 4), 86400, true);

    // Set the 2nd, 3rd, 4th and 5th columns to be high, low, open and close data. The open value
    // starts from 100, and the daily change is random from -5 to 5.
    rantable->setHLOCCols(1, 100, -5, 5);

    // Set the 6th column as the vol data from 5 to 25 million
    rantable->setCol(5, 50000000, 250000000);

    // Now we read the data from the table into arrays
    DoubleArray timeStamps = rantable->getCol(0);
    DoubleArray highData = rantable->getCol(1);
    DoubleArray lowData = rantable->getCol(2);
    DoubleArray openData = rantable->getCol(3);
    DoubleArray closeData = rantable->getCol(4);
    DoubleArray volData = rantable->getCol(5);

    // Create a FinanceChart object of width 640 pixels
    FinanceChart *c = new FinanceChart(640);

    // Add a title to the chart
    c->addTitle("Finance Chart Demonstration");

    // Set the data into the finance chart object
    c->setData(timeStamps, highData, lowData, openData, closeData, volData, extraDays);

    // Add the main chart with 240 pixels in height
    c->addMainChart(240);

    // Add a 5 period simple moving average to the main chart, using brown color
    c->addSimpleMovingAvg(5, 0x663300);

    // Add a 20 period simple moving average to the main chart, using purple color
    c->addSimpleMovingAvg(20, 0x9900ff);

    // Add HLOC symbols to the main chart, using green/red for up/down days
    c->addHLOC(0x008000, 0xcc0000);

    // Add 20 days bollinger band to the main chart, using light blue (9999ff) as the border and
    // semi-transparent blue (c06666ff) as the fill color
    c->addBollingerBand(20, 2, 0x9999ff, 0xc06666ff);

    // Add a 75 pixels volume bars sub-chart to the bottom of the main chart, using green/red/grey
    // for up/down/flat days
    c->addVolBars(75, 0x99ff99, 0xff9999, 0x808080);

    // Append a 14-days RSI indicator chart (75 pixels high) after the main chart. The main RSI line
    // is purple (800080). Set threshold region to +/- 20 (that is, RSI = 50 +/- 25). The
    // upper/lower threshold regions will be filled with red (ff0000)/blue (0000ff).
    c->addRSI(75, 14, 0x800080, 20, 0xff0000, 0x0000ff);

    // Append a 12-days momentum indicator chart (75 pixels high) using blue (0000ff) color.
    c->addMomentum(75, 12, 0x0000ff);

    // Output the chart
    c->makeChart();

    //free up resources
    delete rantable;

    return c;
}


BaseChart *finance2(int /* chartIndex */, const char ** /* imageMap */)
{
    // Create a finance chart demo containing 100 days of data
    int noOfDays = 100;

    // To compute moving averages starting from the first day, we need to get extra data points
    // before the first day
    int extraDays = 30;

    // In this exammple, we use a random number generator utility to simulate the data. We set up
    // the random table to create 6 cols x (noOfDays + extraDays) rows, using 9 as the seed.
    RanTable *rantable = new RanTable(9, 6, noOfDays + extraDays);

    // Set the 1st col to be the timeStamp, starting from Sep 4, 2002, with each row representing
    // one day, and counting week days only (jump over Sat and Sun)
    rantable->setDateCol(0, Chart::chartTime(2002, 9, 4), 86400, true);

    // Set the 2nd, 3rd, 4th and 5th columns to be high, low, open and close data. The open value
    // starts from 100, and the daily change is random from -5 to 5.
    rantable->setHLOCCols(1, 100, -5, 5);

    // Set the 6th column as the vol data from 5 to 25 million
    rantable->setCol(5, 50000000, 250000000);

    // Now we read the data from the table into arrays
    DoubleArray timeStamps = rantable->getCol(0);
    DoubleArray highData = rantable->getCol(1);
    DoubleArray lowData = rantable->getCol(2);
    DoubleArray openData = rantable->getCol(3);
    DoubleArray closeData = rantable->getCol(4);
    DoubleArray volData = rantable->getCol(5);

    // Create a FinanceChart object of width 640 pixels
    FinanceChart *c = new FinanceChart(640);

    // Add a title to the chart
    c->addTitle("Finance Chart Demonstration");

    // Set the data into the finance chart object
    c->setData(timeStamps, highData, lowData, openData, closeData, volData, extraDays);

    // Add a slow stochastic chart (75 pixels high) with %K = 14 and %D = 3
    c->addSlowStochastic(75, 14, 3, 0x006060, 0x606000);

    // Add the main chart with 240 pixels in height
    c->addMainChart(240);

    // Add a 10 period simple moving average to the main chart, using brown color
    c->addSimpleMovingAvg(10, 0x663300);

    // Add a 20 period simple moving average to the main chart, using purple color
    c->addSimpleMovingAvg(20, 0x9900ff);

    // Add candlestick symbols to the main chart, using green/red for up/down days
    c->addCandleStick(0x00ff00, 0xff0000);

    // Add 20 days donchian channel to the main chart, using light blue (9999ff) as the border and
    // semi-transparent blue (c06666ff) as the fill color
    c->addDonchianChannel(20, 0x9999ff, 0xc06666ff);

    // Add a 75 pixels volume bars sub-chart to the bottom of the main chart, using green/red/grey
    // for up/down/flat days
    c->addVolBars(75, 0x99ff99, 0xff9999, 0x808080);

    // Append a MACD(26, 12) indicator chart (75 pixels high) after the main chart, using 9 days for
    // computing divergence.
    c->addMACD(75, 26, 12, 9, 0x0000ff, 0xff00ff, 0x008000);

    // Output the chart
    c->makeChart();

    //free up resources
    delete rantable;

    return c;
}


BaseChart *financesymbols(int /* chartIndex */, const char ** /* imageMap */)
{
    // Create a finance chart demo containing 100 days of data
    int noOfDays = 100;

    // To compute moving averages starting from the first day, we need to get extra data points
    // before the first day
    int extraDays = 30;

    // In this exammple, we use a random number generator utility to simulate the data. We set up
    // the random table to create 6 cols x (noOfDays + extraDays) rows, using 9 as the seed.
    RanTable *rantable = new RanTable(9, 6, noOfDays + extraDays);

    // Set the 1st col to be the timeStamp, starting from Sep 4, 2014, with each row representing
    // one day, and counting week days only (jump over Sat and Sun)
    rantable->setDateCol(0, Chart::chartTime(2014, 9, 4), 86400, true);

    // Set the 2nd, 3rd, 4th and 5th columns to be high, low, open and close data. The open value
    // starts from 100, and the daily change is random from -5 to 5.
    rantable->setHLOCCols(1, 100, -5, 5);

    // Set the 6th column as the vol data from 5 to 25 million
    rantable->setCol(5, 50000000, 250000000);

    // Now we read the data from the table into arrays
    DoubleArray timeStamps = rantable->getCol(0);
    DoubleArray highData = rantable->getCol(1);
    DoubleArray lowData = rantable->getCol(2);
    DoubleArray openData = rantable->getCol(3);
    DoubleArray closeData = rantable->getCol(4);
    DoubleArray volData = rantable->getCol(5);

    // Custom data series should be of the same length as the OHLC data series
    double *buySignal = new double[closeData.len];
    double *sellSignal = new double[closeData.len];

    //
    // The following is just an arbitrary algorithm to create some meaningless buySignal and
    // sellSignal. They are just for demonstrating the charting engine. Please do not use them for
    // actual trading.
    //
    ArrayMath sma5Calculator(closeData);
    DoubleArray sma5 = sma5Calculator.movAvg(5).result();
    ArrayMath sma20Calculator(closeData);
    DoubleArray sma20 = sma20Calculator.movAvg(20).result();
    for(int i = 0; i < sma5.len; ++i) {
        buySignal[i] = Chart::NoValue;
        sellSignal[i] = Chart::NoValue;
        if (i > 0) {
            if ((sma5[i - 1] <= sma20[i - 1]) && (sma5[i] > sma20[i])) {
                buySignal[i] = lowData[i];
            }
            if ((sma5[i - 1] >= sma20[i - 1]) && (sma5[i] < sma20[i])) {
                sellSignal[i] = highData[i];
            }
        }
    }

    // Create a FinanceChart object of width 640 pixels
    FinanceChart *c = new FinanceChart(640);

    // Add a title to the chart
    c->addTitle("Finance Chart with Custom Symbols");

    // Set the data into the finance chart object
    c->setData(timeStamps, highData, lowData, openData, closeData, volData, extraDays);

    // Add the main chart with 240 pixels in height
    XYChart *mainChart = c->addMainChart(240);

    // Add buy signal symbols to the main chart, using cyan (0x00ffff) upward pointing arrows as
    // symbols
    ScatterLayer *buyLayer = mainChart->addScatterLayer(DoubleArray(), DoubleArray(buySignal,
        closeData.len), "Buy", Chart::ArrowShape(0, 1, 0.4, 0.4), 11, 0x00ffff);
    // Shift the symbol lower by 20 pixels
    buyLayer->getDataSet(0)->setSymbolOffset(0, 20);

    // Add sell signal symbols to the main chart, using purple (0x9900cc) downward pointing arrows
    // as symbols
    ScatterLayer *sellLayer = mainChart->addScatterLayer(DoubleArray(), DoubleArray(sellSignal,
        closeData.len), "Sell", Chart::ArrowShape(180, 1, 0.4, 0.4), 11, 0x9900cc);
    // Shift the symbol higher by 20 pixels
    sellLayer->getDataSet(0)->setSymbolOffset(0, -20);

    // Add a 5 period simple moving average to the main chart, using brown color
    c->addSimpleMovingAvg(5, 0x663300);

    // Add a 20 period simple moving average to the main chart, using purple color
    c->addSimpleMovingAvg(20, 0x9900ff);

    // Add candlestick symbols to the main chart, using green/red for up/down days
    c->addCandleStick(0x66ff66, 0xff6666);

    // Add a volume indicator chart (75 pixels high) after the main chart, using green/red/grey for
    // up/down/flat days
    c->addVolIndicator(75, 0x99ff99, 0xff9999, 0x808080);

    // Append a 14-days RSI indicator chart (75 pixels high) after the main chart. The main RSI line
    // is purple (800080). Set threshold region to +/- 20 (that is, RSI = 50 +/- 25). The
    // upper/lower threshold regions will be filled with red (ff0000)/blue (0000ff).
    c->addRSI(75, 14, 0x800080, 20, 0xff0000, 0x0000ff);

    // Output the chart
    c->makeChart();

    //free up resources
    delete rantable;
    delete buySignal;
    delete sellSignal;

    return c;
}


BaseChart *markzone(int /* chartIndex */, const char **imageMap)
{
    // The data for the chart
    double data[] = {40, 45, 37, 24, 32, 39, 53, 52, 63, 49, 46, 40, 54, 50, 57, 57, 48, 49, 63, 67,
        74, 72, 70, 89, 74};
    const char *labels[] = {"0\nJun 4", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11",
        "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "0\nJun 5"};

    // Create a XYChart object of size 400 x 270 pixels
    XYChart *c = new XYChart(400, 270);

    // Set the plotarea at (80, 60) and of size 300 x 200 pixels. Turn off the grid lines by setting
    // their colors to Transparent.
    c->setPlotArea(80, 28, 300, 200)->setGridColor(Chart::Transparent);

    // Add a title to the y axis
    TextBox *textbox = c->yAxis()->setTitle("Temperature");

    // Set the y axis title upright (font angle = 0)
    textbox->setFontAngle(0);

    // Put the y axis title on top of the axis
    textbox->setAlignment(Chart::TopLeft2);

    // Add green (0x99ff99), yellow (0xffff99) and red (0xff9999) zones to the y axis to represent
    // the ranges 0 - 50, 50 - 80, and 80 - max.
    c->yAxis()->addZone(0, 50, 0x99ff99);
    c->yAxis()->addZone(50, 80, 0xffff99);
    c->yAxis()->addZone(80, 9999, 0xff9999);

    // Add a purple (0x800080) mark at y = 70 using a line width of 2.
    c->yAxis()->addMark(70, 0x800080, "Alert = 70")->setLineWidth(2);

    // Add a green (0x008000) mark at y = 40 using a line width of 2.
    c->yAxis()->addMark(40, 0x008000, "Watch = 40")->setLineWidth(2);

    // Add a legend box at (165, 0) (top right of the chart) using 8pt Arial font. and horizontal
    // layout.
    LegendBox *legend = c->addLegend(165, 0, false, "arialbd.ttf", 8);

    // Disable the legend box boundary by setting the colors to Transparent
    legend->setBackground(Chart::Transparent, Chart::Transparent);

    // Add 3 custom entries to the legend box to represent the 3 zones
    legend->addKey("Normal", 0x80ff80);
    legend->addKey("Warning", 0xffff80);
    legend->addKey("Critical", 0xff8080);

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Display 1 out of 3 labels on the x-axis. Show minor ticks for remaining labels.
    c->xAxis()->setLabelStep(3, 1);

    // Add a 3D bar layer with the given data
    BarLayer *layer = c->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))),
        0xbbbbff);

    // Set the bar gap to 0 so that the bars are packed tightly
    layer->setBarGap(0);

    // Set the border color of the bars same as the fill color, with 1 pixel 3D border effect.
    layer->setBorderColor(Chart::SameAsMainColor, 1);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='Temperature at {x}:00 = {value} C'");

    return c;
}


BaseChart *markzone2(int /* chartIndex */, const char **imageMap)
{
    // The data for the chart
    double data[] = {50, 55, 47, 34, 42, 49, 63, 62, 73, 59, 56, 50, 64, 60, 67, 67, 58, 59, 73, 77,
        84, 82, 80, 84, 89};

    // The labels for the chart. Note the "-" means a minor tick.
    const char *labels[] = {"0\nJun 4", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11",
        "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "0\nJun 5"};

    // Create a XYChart object of size 400 x 270 pixels
    XYChart *c = new XYChart(400, 270);

    // Set the plotarea at (80, 25) and of size 300 x 200 pixels. Use alternate color background
    // (0xeeeeee) and (0xffffff). Set border and grid colors to grey (0xc0c0c0).
    c->setPlotArea(50, 25, 300, 200, 0xeeeeee, 0xffffff, 0xc0c0c0, 0xc0c0c0, 0xc0c0c0);

    // Add a title to the chart using 14pt Times Bold Italic font
    c->addTitle("Server Monitor", "timesbi.ttf", 14);

    // Add a title to the y axis
    c->yAxis()->setTitle("Server Load (MBytes)");

    // Set the y axis width to 2 pixels
    c->yAxis()->setWidth(2);

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Display 1 out of 3 labels on the x-axis. Show minor ticks for remaining labels.
    c->xAxis()->setLabelStep(3, 1);

    // Set the x axis width to 2 pixels
    c->xAxis()->setWidth(2);

    // Add a horizontal red (0x800080) mark line at y = 80
    Mark *yMark = c->yAxis()->addMark(80, 0xff0000, "Critical Threshold Set Point");

    // Set the mark line width to 2 pixels
    yMark->setLineWidth(2);

    // Put the mark label at the top center of the mark line
    yMark->setAlignment(Chart::TopCenter);

    // Add an orange (0xffcc66) zone from x = 18 to x = 20
    c->xAxis()->addZone(18, 20, 0xffcc66);

    // Add a vertical brown (0x995500) mark line at x = 18
    Mark *xMark1 = c->xAxis()->addMark(18, 0x995500, "Backup Start");

    // Set the mark line width to 2 pixels
    xMark1->setLineWidth(2);

    // Put the mark label at the left of the mark line
    xMark1->setAlignment(Chart::Left);

    // Rotate the mark label by 90 degrees so it draws vertically
    xMark1->setFontAngle(90);

    // Add a vertical brown (0x995500) mark line at x = 20
    Mark *xMark2 = c->xAxis()->addMark(20, 0x995500, "Backup End");

    // Set the mark line width to 2 pixels
    xMark2->setLineWidth(2);

    // Put the mark label at the right of the mark line
    xMark2->setAlignment(Chart::Right);

    // Rotate the mark label by 90 degrees so it draws vertically
    xMark2->setFontAngle(90);

    // Add a green (0x00cc00) line layer with line width of 2 pixels
    c->addLineLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), 0x00cc00
        )->setLineWidth(2);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='Load at {x}:00 = {value} MBytes'");

    return c;
}


BaseChart *yzonecolor(int /* chartIndex */, const char **imageMap)
{
    // The data for the chart
    double data[] = {30, 28, 40, 55, 75, 68, 54, 60, 50, 62, 75, 65, 75, 89, 60, 55, 53, 35, 50, 66,
        56, 48, 52, 65, 62};

    // The labels for the chart
    const char *labels[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12",
        "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24"};

    // Create a XYChart object of size 500 x 300 pixels, with a pale yellow (0xffffc0) background, a
    // black border, and 1 pixel 3D border effect
    XYChart *c = new XYChart(500, 300, 0xffffc0, 0x000000, 1);

    // Set the plotarea at (55, 50) and of size 420 x 205 pixels, with white background. Turn on
    // both horizontal and vertical grid lines with light grey color (0xc0c0c0)
    c->setPlotArea(55, 50, 420, 205, 0xffffff)->setGridColor(0xc0c0c0, 0xc0c0c0);

    // Add a legend box at (55, 25) (top of the chart) with horizontal layout. Use 8pt Arial font.
    // Set the background and border color to Transparent.
    LegendBox *legendBox = c->addLegend(55, 25, false, "", 8);
    legendBox->setBackground(Chart::Transparent);

    // Add keys to the legend box to explain the color zones
    legendBox->addKey("Normal Zone", 0x8033ff33);
    legendBox->addKey("Alert Zone", 0x80ff3333);

    // Add a title box to the chart using 13pt Arial Bold Italic font. The title is in CDML and
    // includes embedded images for highlight. The text is white (0xffffff) on a black background,
    // with a 1 pixel 3D border.
    c->addTitle(
        "<*block,valign=absmiddle*><*img=star.png*><*img=star.png*> Y Zone Color Demo "
        "<*img=star.png*><*img=star.png*><*/*>", "arialbi.ttf", 13, 0xffffff)->setBackground(
        0x000000, -1, 1);

    // Add a title to the y axis
    c->yAxis()->setTitle("Energy Concentration (KJ per liter)");

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Display 1 out of 3 labels on the x-axis.
    c->xAxis()->setLabelStep(3);

    // Add a title to the x axis using CDML
    c->xAxis()->setTitle("<*block,valign=absmiddle*><*img=clock.png*>  Elapsed Time (hour)<*/*>");

    // Set the axes width to 2 pixels
    c->xAxis()->setWidth(2);
    c->yAxis()->setWidth(2);

    // Add an area layer to the chart. The area is using a y zone color, where the color is
    // semi-transparent green below 60, and semi-transparent red above 60.
    c->addAreaLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), c->yZoneColor(60,
        0x8033ff33, 0x80ff3333));

    // Add a custom CDML text at the bottom right of the plot area as the logo
    c->addText(475, 255,
        "<*block,valign=absmiddle*><*img=small_molecule.png*> <*block*>"
        "<*font=timesbi.ttf,size=10,color=804040*>Molecular\nEngineering<*/*>")->setAlignment(
        Chart::BottomRight);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='Hour {xLabel}: {value} KJ/liter'");

    return c;
}


BaseChart *xzonecolor(int /* chartIndex */, const char **imageMap)
{
    // The data for the chart
    double data[] = {50, 55, 47, 34, 42, 49, 63, 62, 73, 59, 56, 50, 64, 60, 67, 67, 58, 59, 73, 77,
        84, 82, 80, 84, 89};

    // The error data representing the error band around the data points
    double errData[] = {5, 6, 5.1, 6.5, 6.6, 8, 5.4, 5.1, 4.6, 5.0, 5.2, 6.0, 4.9, 5.6, 4.8, 6.2,
        7.4, 7.1, 6.5, 9.6, 12.1, 15.3, 18.5, 20.9, 24.1};

    // The timestamps for the data
    double labels[] = {Chart::chartTime(2001, 1, 1), Chart::chartTime(2001, 2, 1), Chart::chartTime(
        2001, 3, 1), Chart::chartTime(2001, 4, 1), Chart::chartTime(2001, 5, 1), Chart::chartTime(
        2001, 6, 1), Chart::chartTime(2001, 7, 1), Chart::chartTime(2001, 8, 1), Chart::chartTime(
        2001, 9, 1), Chart::chartTime(2001, 10, 1), Chart::chartTime(2001, 11, 1), Chart::chartTime(
        2001, 12, 1), Chart::chartTime(2002, 1, 1), Chart::chartTime(2002, 2, 1), Chart::chartTime(
        2002, 3, 1), Chart::chartTime(2002, 4, 1), Chart::chartTime(2002, 5, 1), Chart::chartTime(
        2002, 6, 1), Chart::chartTime(2002, 7, 1), Chart::chartTime(2002, 8, 1), Chart::chartTime(
        2002, 9, 1), Chart::chartTime(2002, 10, 1), Chart::chartTime(2002, 11, 1), Chart::chartTime(
        2002, 12, 1), Chart::chartTime(2003, 1, 1)};

    // Create a XYChart object of size 550 x 220 pixels
    XYChart *c = new XYChart(550, 220);

    // Set the plot area at (50, 10) and of size 480 x 180 pixels. Enabled both vertical and
    // horizontal grids by setting their colors to light grey (cccccc)
    c->setPlotArea(50, 10, 480, 180)->setGridColor(0xcccccc, 0xcccccc);

    // Add a legend box (50, 10) (top of plot area) using horizontal layout. Use 8pt Arial font.
    // Disable bounding box (set border to transparent).
    LegendBox *legendBox = c->addLegend(50, 10, false, "", 8);
    legendBox->setBackground(Chart::Transparent);

    // Add keys to the legend box to explain the color zones
    legendBox->addKey("Historical", 0x9999ff);
    legendBox->addKey("Forecast", 0xff9966);

    // Add a title to the y axis.
    c->yAxis()->setTitle("Energy Consumption");

    // Set the labels on the x axis
    c->xAxis()->setLabels(DoubleArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Set multi-style axis label formatting. Use Arial Bold font for yearly labels and display them
    // as "yyyy". Use default font for monthly labels and display them as "mmm". Replace some labels
    // with minor ticks to ensure the labels are at least 3 units apart.
    c->xAxis()->setMultiFormat(Chart::StartOfYearFilter(), "<*font=arialbd.ttf*>{value|yyyy}",
        Chart::StartOfMonthFilter(), "{value|mmm}", 3);

    // Add a line layer to the chart
    LineLayer *layer = c->addLineLayer();

    // Create the color to draw the data line. The line is blue (0x333399) to the left of x = 18,
    // and become a red (0xd04040) dash line to the right of x = 18.
    int lineColor = layer->xZoneColor(18, 0x333399, c->dashLineColor(0xd04040, Chart::DashLine));

    // Add the data line
    layer->addDataSet(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), lineColor, "Average"
        );

    // We are not showing the data set name in the legend box. The name is for showing in tool tips
    // only.
    layer->setLegend(Chart::NoLegend);

    // Create the color to draw the err zone. The color is semi-transparent blue (0x809999ff) to the
    // left of x = 18, and become semi-transparent red (0x80ff9966) to the right of x = 18.
    int errColor = layer->xZoneColor(18, 0x809999ff, 0x80ff9966);

    // Add the upper border of the err zone
    layer->addDataSet(ArrayMath(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0])))).add(
        DoubleArray(errData, (int)(sizeof(errData) / sizeof(errData[0])))), errColor, "Upper bound")
        ;

    // Add the lower border of the err zone
    layer->addDataSet(ArrayMath(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0])))).sub(
        DoubleArray(errData, (int)(sizeof(errData) / sizeof(errData[0])))), errColor, "Lower bound")
        ;

    // Set the default line width to 2 pixels
    layer->setLineWidth(2);

    // In this example, we are not showing the data set name in the legend box
    layer->setLegend(Chart::NoLegend);

    // Color the region between the err zone lines
    c->addInterLineLayer(layer->getLine(1), layer->getLine(2), errColor);

    // Output the chart
    c->makeChart();

    // Include tool tip for the chart.
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName} on {xLabel|mmm yyyy}: {value} MJoule'");

    return c;
}


BaseChart *dualyaxis(int /* chartIndex */, const char **imageMap)
{
    // The data for the chart
    double data0[] = {0.05, 0.06, 0.48, 0.1, 0.01, 0.05};
    double data1[] = {100, 125, 265, 147, 67, 105};
    const char *labels[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun"};

    // Create a XYChart object of size 300 x 180 pixels
    XYChart *c = new XYChart(300, 180);

    // Set the plot area at (50, 20) and of size 200 x 130 pixels
    c->setPlotArea(50, 20, 200, 130);

    // Add a title to the chart using 8pt Arial Bold font
    c->addTitle("Independent Y-Axis Demo", "arialbd.ttf", 8);

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Add a title to the primary (left) y axis
    c->yAxis()->setTitle("Packet Drop Rate (pps)");

    // Set the axis, label and title colors for the primary y axis to red (0xc00000) to match the
    // first data set
    c->yAxis()->setColors(0xc00000, 0xc00000, 0xc00000);

    // Add a title to the secondary (right) y axis
    c->yAxis2()->setTitle("Throughtput (MBytes)");

    // set the axis, label and title colors for the primary y axis to green (0x008000) to match the
    // second data set
    c->yAxis2()->setColors(0x008000, 0x008000, 0x008000);

    // Add a line layer to for the first data set using red (0xc00000) color with a line width to 3
    // pixels
    LineLayer *lineLayer = c->addLineLayer(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0])
        )), 0xc00000);
    lineLayer->setLineWidth(3);

    // tool tip for the line layer
    lineLayer->setHTMLImageMap("", "", "title='Packet Drop Rate on {xLabel}: {value} pps'");

    // Add a bar layer to for the second data set using green (0x00C000) color. Bind the second data
    // set to the secondary (right) y axis
    BarLayer *barLayer = c->addBarLayer(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))),
        0x00c000);
    barLayer->setUseYAxis2();

    // tool tip for the bar layer
    barLayer->setHTMLImageMap("", "", "title='Throughput on {xLabel}: {value} MBytes'");

    // Output the chart
    c->makeChart();

    // include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable");

    return c;
}


BaseChart *dualxaxis(int /* chartIndex */, const char **imageMap)
{
    // The data for the chart
    double data0[] = {42, 49, 33, 38, 51, 46, 29, 41, 44, 57, 59, 52, 37, 34, 51, 56, 56, 60, 70,
        76, 63, 67, 75, 64, 51};
    double data1[] = {50, 55, 47, 34, 42, 49, 63, 62, 73, 59, 56, 50, 64, 60, 67, 67, 58, 59, 73,
        77, 84, 82, 80, 84, 98};

    // The labels for the bottom x axis. Note the "-" means a minor tick.
    const char *labels0[] = {"0\nJun 4", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11",
        "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "0\nJun 5"};

    // The labels for the top x axis. Note that "-" means a minor tick.
    const char *labels1[] = {"Jun 3\n12", "13", "14", "15", "16", "17", "18", "19", "20", "21",
        "22", "23", "Jun 4\n0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"};

    // Create a XYChart object of size 310 x 310 pixels
    XYChart *c = new XYChart(310, 310);

    // Set the plotarea at (50, 50) and of size 200 x 200 pixels
    c->setPlotArea(50, 50, 200, 200);

    // Add a title to the primary (left) y axis
    c->yAxis()->setTitle("US Dollars");

    // Set the tick length to -4 pixels (-ve means ticks inside the plot area)
    c->yAxis()->setTickLength(-4);

    // Add a title to the secondary (right) y axis
    c->yAxis2()->setTitle("HK Dollars (1 USD = 7.8 HKD)");

    // Set the tick length to -4 pixels (-ve means ticks inside the plot area)
    c->yAxis2()->setTickLength(-4);

    // Synchronize the y-axis such that y2 = 7.8 x y1
    c->syncYAxis(7.8);

    // Add a title to the bottom x axis
    c->xAxis()->setTitle("Hong Kong Time");

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels0, (int)(sizeof(labels0) / sizeof(labels0[0]))));

    // Display 1 out of 3 labels on the x-axis. Show minor ticks for remaining labels.
    c->xAxis()->setLabelStep(3, 1);

    // Set the major tick length to -4 pixels and minor tick length to -2 pixels (-ve means ticks
    // inside the plot area)
    c->xAxis()->setTickLength(-4, -2);

    // Set the distance between the axis labels and the axis to 6 pixels
    c->xAxis()->setLabelGap(6);

    // Add a title to the top x-axis
    c->xAxis2()->setTitle("New York Time");

    // Set the labels on the x axis.
    c->xAxis2()->setLabels(StringArray(labels1, (int)(sizeof(labels1) / sizeof(labels1[0]))));

    // Display 1 out of 3 labels on the x-axis. Show minor ticks for remaining labels.
    c->xAxis2()->setLabelStep(3, 1);

    // Set the major tick length to -4 pixels and minor tick length to -2 pixels (-ve means ticks
    // inside the plot area)
    c->xAxis2()->setTickLength(-4, -2);

    // Set the distance between the axis labels and the axis to 6 pixels
    c->xAxis2()->setLabelGap(6);

    // Add a line layer to the chart with a line width of 2 pixels
    c->addLineLayer(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), -1,
        "Red Transactions")->setLineWidth(2);

    // Add an area layer to the chart with no area boundary line
    c->addAreaLayer(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), -1,
        "Green Transactions")->setLineWidth(0);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName}\nHKT Jun {=3.5+{x}/24|0} {={x}%24}:00 (NYT Jun {=3+{x}/24|0} "
        "{=({x}+12)%24}:00)\nHKD {={value}*7.8} (USD {value})'");

    return c;
}


BaseChart *multiaxes(int /* chartIndex */, const char **imageMap)
{
    // Data for the chart
    double data0[] = {1700, 3900, 2900, 3800, 4100, 4600, 2900, 4100, 4400, 5700, 5900, 5200, 3700,
        3400, 5100, 5600, 5600, 6000, 7000, 7600, 6300, 6700, 7500, 6400, 8800};
    double data1[] = {500, 550, 670, 990, 820, 730, 800, 720, 730, 790, 860, 800, 840, 680, 740,
        890, 680, 790, 730, 770, 840, 820, 800, 840, 670};
    double data2[] = {46, 68, 35, 33, 38, 20, 12, 18, 15, 23, 30, 24, 28, 15, 21, 26, 46, 42, 38,
        25, 23, 32, 24, 20, 25};
    double data3[] = {0.84, 0.82, 0.82, 0.38, 0.25, 0.52, 0.54, 0.52, 0.38, 0.51, 0.46, 0.29, 0.5,
        0.55, 0.47, 0.34, 0.52, 0.33, 0.21, 0.3, 0.25, 0.15, 0.18, 0.22, 0.14};

    // Labels for the chart
    const char *labels[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12",
        "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24"};

    // Create a XYChart object of size 600 x 360 pixels. Use a vertical gradient color from sky blue
    // (aaccff) to white (ffffff) as background. Set border to grey (888888). Use rounded corners.
    // Enable soft drop shadow.
    XYChart *c = new XYChart(600, 360);
    c->setBackground(c->linearGradientColor(0, 0, 0, c->getHeight(), 0xaaccff, 0xffffff), 0x888888);
    c->setRoundedFrame();
    c->setDropShadow();

    // Add a title box to the chart using 15pt Arial Bold Italic font. Set top margin to 16 pixels.
    TextBox *title = c->addTitle("Multiple Axes Demonstration", "arialbi.ttf", 15);
    title->setMargin(0, 0, 16, 0);

    // Set the plotarea at (100, 80) and of size 400 x 230 pixels, with white (ffffff) background.
    // Use grey #(aaaaa) dotted lines for both horizontal and vertical grid lines.
    c->setPlotArea(100, 80, 400, 230, 0xffffff, -1, -1, c->dashLineColor(0xaaaaaa, Chart::DotLine),
        -1);

    // Add a legend box with the bottom center anchored at (300, 80) (top center of the plot area).
    // Use horizontal layout, and 8 points Arial Bold font. Set background and border to
    // transparent.
    LegendBox *legendBox = c->addLegend(300, 80, false, "arialbd.ttf", 8);
    legendBox->setAlignment(Chart::BottomCenter);
    legendBox->setBackground(Chart::Transparent, Chart::Transparent);

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Display 1 out of 3 labels on the x-axis.
    c->xAxis()->setLabelStep(3);

    // Add a title to the x-axis
    c->xAxis()->setTitle("Hour of Day");

    // Add a title on top of the primary (left) y axis.
    c->yAxis()->setTitle("Power\n(Watt)")->setAlignment(Chart::TopLeft2);
    // Set the axis, label and title colors for the primary y axis to red (c00000) to match the
    // first data set
    c->yAxis()->setColors(0xcc0000, 0xcc0000, 0xcc0000);

    // Add a title on top of the secondary (right) y axis.
    c->yAxis2()->setTitle("Load\n(Mbps)")->setAlignment(Chart::TopRight2);
    // Set the axis, label and title colors for the secondary y axis to green (00800000) to match
    // the second data set
    c->yAxis2()->setColors(0x008000, 0x008000, 0x008000);

    // Add the third y-axis at 50 pixels to the left of the plot area
    Axis *leftAxis = c->addAxis(Chart::Left, 50);
    // Add a title on top of the third y axis.
    leftAxis->setTitle("Temp\n(C)")->setAlignment(Chart::TopLeft2);
    // Set the axis, label and title colors for the third y axis to blue (0000cc) to match the third
    // data set
    leftAxis->setColors(0x0000cc, 0x0000cc, 0x0000cc);

    // Add the fouth y-axis at 50 pixels to the right of the plot area
    Axis *rightAxis = c->addAxis(Chart::Right, 50);
    // Add a title on top of the fourth y axis.
    rightAxis->setTitle("Error\n(%)")->setAlignment(Chart::TopRight2);
    // Set the axis, label and title colors for the fourth y axis to purple (880088) to match the
    // fourth data set
    rightAxis->setColors(0x880088, 0x880088, 0x880088);

    // Add a line layer to for the first data set using red (c00000) color, with a line width of 2
    // pixels
    LineLayer *layer0 = c->addLineLayer(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))),
        0xcc0000, "Power");
    layer0->setLineWidth(2);

    // Add a line layer to for the second data set using green (00c0000) color, with a line width of
    // 2 pixels. Bind the layer to the secondary y-axis.
    LineLayer *layer1 = c->addLineLayer(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))),
        0x008000, "Load");
    layer1->setLineWidth(2);
    layer1->setUseYAxis2();

    // Add a line layer to for the third data set using blue (0000cc) color, with a line width of 2
    // pixels. Bind the layer to the third y-axis.
    LineLayer *layer2 = c->addLineLayer(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))),
        0x0000cc, "Temperature");
    layer2->setLineWidth(2);
    layer2->setUseYAxis(leftAxis);

    // Add a line layer to for the fourth data set using purple (880088) color, with a line width of
    // 2 pixels. Bind the layer to the fourth y-axis.
    LineLayer *layer3 = c->addLineLayer(DoubleArray(data3, (int)(sizeof(data3) / sizeof(data3[0]))),
        0x880088, "Error Rate");
    layer3->setLineWidth(2);
    layer3->setUseYAxis(rightAxis);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName} at hour {xLabel} = {value}'");

    return c;
}


BaseChart *fourq(int /* chartIndex */, const char **imageMap)
{
    // XY points for the scatter chart
    double dataX0[] = {10, 15, 6, -12, 14, -8, 13, -13, 16, 12, 10.5};
    double dataY0[] = {130, 150, 80, 110, -110, -105, -130, 115, -170, 125, 125};

    double dataX1[] = {6, 7, -4, 3.5, 7, 8, -9, -10, -12, 11, 8};
    double dataY1[] = {65, -40, -40, 45, -70, -80, 80, 10, -100, 105, 60};

    double dataX2[] = {-10, -12, 11, 8, 6, 12, -4, 3.5, 7, 8, -9};
    double dataY2[] = {65, -80, -40, 45, -70, -80, 80, 90, -100, 105, 60};

    // Create a XYChart object of size 600 x 300 pixels, with a light blue (ccccff) background, a
    // black border, and 1 pixel 3D border effect
    XYChart *c = new XYChart(600, 480, 0xccccff, 0x000000, 1);

    // Add a title box to the chart using 16pt Arial Bold Italic font, with white text on deep blue
    // background
    c->addTitle("Four Quadrant Chart Demonstration", "arialbi.ttf", 16, 0xffffff)->setBackground(
        0x000080);

    // Set the plotarea at (20, 60) and of size 560 x 360 pixels, with grey (808080) border, and
    // light grey (c0c0c0) horizontal and vertical grid lines. Set 4 quadrant coloring, where the
    // colors of the quadrants alternate between lighter and deeper grey (dddddd/eeeeee)
    c->setPlotArea(20, 60, 560, 360, -1, -1, 0x808080, 0xc0c0c0, 0xc0c0c0)->set4QBgColor(0xdddddd,
        0xeeeeee, 0xdddddd, 0xeeeeee);

    // Set 4 quadrant mode, with both x and y axes symetrical around the origin
    c->setAxisAtOrigin(Chart::XYAxisAtOrigin, Chart::XAxisSymmetric + Chart::YAxisSymmetric);

    // Add a legend box at (300, 460) (bottom center of the chart) with horizontal layout. Use 8pt
    // Arial Bold font.
    LegendBox *legendBox = c->addLegend(300, 460, false, "arialbd.ttf", 8);
    legendBox->setAlignment(Chart::BottomCenter);

    // Set legend box background to light grey (dddddd) with a black border
    legendBox->setBackground(0xdddddd, 0);

    // Set left/right margin to 20 pixels and top/bottom margin to 5 pixels
    legendBox->setMargin(20, 20, 5, 5);

    // Add a titles to axes
    c->xAxis()->setTitle("Alpha Index");
    c->yAxis()->setTitle("Beta Index");

    // Set axes width to 2 pixels
    c->xAxis()->setWidth(2);
    c->yAxis()->setWidth(2);

    // Add scatter layer, using 15 pixels red (ff33333) X shape symbols
    c->addScatterLayer(DoubleArray(dataX0, (int)(sizeof(dataX0) / sizeof(dataX0[0]))), DoubleArray(
        dataY0, (int)(sizeof(dataY0) / sizeof(dataY0[0]))), "Group A", Chart::Cross2Shape(), 15,
        0xff3333);

    // Add scatter layer, using 15 pixels green (33ff33) 6-sided polygon symbols
    c->addScatterLayer(DoubleArray(dataX1, (int)(sizeof(dataX1) / sizeof(dataX1[0]))), DoubleArray(
        dataY1, (int)(sizeof(dataY1) / sizeof(dataY1[0]))), "Group B", Chart::PolygonShape(6), 15,
        0x33ff33);

    // Add scatter layer, using 15 pixels blue (3333ff) triangle symbols
    c->addScatterLayer(DoubleArray(dataX2, (int)(sizeof(dataX2) / sizeof(dataX2[0]))), DoubleArray(
        dataY2, (int)(sizeof(dataY2) / sizeof(dataY2[0]))), "Group C", Chart::TriangleSymbol, 15,
        0x3333ff);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='[{dataSetName}] Alpha = {x}, Beta = {value}'");

    return c;
}


BaseChart *datatable(int /* chartIndex */, const char **imageMap)
{
    char buffer[256];

    // The data for the line chart
    double data0[] = {42, 49, 33, 38, 64, 56, 29, 41, 44, 57, 59, 42};
    double data1[] = {65, 75, 47, 34, 42, 49, 73, 62, 90, 69, 66, 78};
    double data2[] = {36, 28, 25, 28, 38, 20, 22, 30, 25, 33, 30, 24};
    const char *labels[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct",
        "Nov", "Dec"};

    // Create a XYChart object of size 600 x 400 pixels
    XYChart *c = new XYChart(600, 400);

    // Add a title to the chart using 18pt Times Bold Italic font
    TextBox *title = c->addTitle("Product Line Global Revenue", "timesbi.ttf", 18);

    // Tentatively set the plotarea at (50, 55) and of (chart_width - 100) x (chart_height - 120)
    // pixels in size. Use a vertical gradient color from sky blue (aaccff) t0 light blue (f9f9ff)
    // as background. Set both horizontal and vertical grid lines to dotted semi-transprent black
    // (aa000000).
    PlotArea *plotArea = c->setPlotArea(50, 55, c->getWidth() - 100, c->getHeight() - 120,
        c->linearGradientColor(0, 55, 0, 55 + c->getHeight() - 120, 0xaaccff, 0xf9fcff), -1, -1,
        c->dashLineColor(0xaa000000, Chart::DotLine), -1);

    // Add a legend box and anchored the top center at the horizontal center of the chart, just
    // under the title. Use 10pt Arial Bold as font, with transparent background and border.
    LegendBox *legendBox = c->addLegend(c->getWidth() / 2, title->getHeight(), false, "arialbd.ttf",
        10);
    legendBox->setAlignment(Chart::TopCenter);
    legendBox->setBackground(Chart::Transparent, Chart::Transparent);

    // Set y-axis title using 10 points Arial Bold Italic font, label style to 8 points Arial Bold,
    // and axis color to transparent
    c->yAxis()->setTitle("Revenue (USD millions)", "arialbi.ttf", 10);
    c->yAxis()->setLabelStyle("arialbd.ttf", 8);
    c->yAxis()->setColors(Chart::Transparent);

    // Set y-axis tick density to 30 pixels. ChartDirector auto-scaling will use this as the
    // guideline when putting ticks on the y-axis.
    c->yAxis()->setTickDensity(30);

    // Add a line layer to the chart
    LineLayer *layer = c->addLineLayer();

    // Set the line width to 3 pixels
    layer->setLineWidth(3);

    // Add the three data sets to the line layer, using circles, diamands and X shapes as symbols
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0xff0000,
        "Quantum Computer")->setDataSymbol(Chart::CircleSymbol, 9);
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x00ff00,
        "Atom Synthesizer")->setDataSymbol(Chart::DiamondSymbol, 11);
    layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), 0xff6600,
        "Proton Cannon")->setDataSymbol(Chart::Cross2Shape(), 11);

    // Set the x axis labels
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Convert the labels on the x-axis to a CDMLTable
    CDMLTable *table = c->xAxis()->makeLabelTable();

    // Set the default top/bottom margins of the cells to 3 pixels
    table->getStyle()->setMargin(0, 0, 3, 3);

    // Use Arial Bold as the font for the first row
    table->getRowStyle(0)->setFontStyle("arialbd.ttf");

    //
    // We can add more information to the table. In this sample code, we add the data series and the
    // legend icons to the table.
    //

    // Add 3 more rows to the table. Set the background of the 1st and 3rd rows to light grey
    // (eeeeee).
    table->appendRow()->setBackground(0xeeeeee, Chart::LineColor);
    table->appendRow();
    table->appendRow()->setBackground(0xeeeeee, Chart::LineColor);

    // Put the values of the 3 data series to the cells in the 3 rows
    for(int i = 0; i < (int)(sizeof(data0) / sizeof(data0[0])); ++i) {
        sprintf(buffer, "%g", data0[i]);
        table->setText(i, 1, buffer);
        sprintf(buffer, "%g", data1[i]);
        table->setText(i, 2, buffer);
        sprintf(buffer, "%g", data2[i]);
        table->setText(i, 3, buffer);
    }

    // Insert a column on the left for the legend icons. Use 5 pixels left/right margins and 3
    // pixels top/bottom margins for the cells in this column.
    table->insertCol(0)->setMargin(5, 5, 3, 3);

    // The top cell is set to transparent, so it is invisible
    table->getCell(0, 0)->setBackground(Chart::Transparent, Chart::Transparent);

    // The other 3 cells are set to the legend icons of the 3 data series
    table->setText(0, 1, layer->getLegendIcon(0));
    table->setText(0, 2, layer->getLegendIcon(1));
    table->setText(0, 3, layer->getLegendIcon(2));

    // Layout legend box first, so we can get its size
    c->layoutLegend();

    // Adjust the plot area size, such that the bounding box (inclusive of axes) is 2 pixels from
    // the left, right and bottom edge, and is just under the legend box.
    c->packPlotArea(2, legendBox->getTopY() + legendBox->getHeight(), c->getWidth() - 3,
        c->getHeight() - 3);

    // After determining the exact plot area position, we may adjust the legend box and the title
    // positions so that they are centered relative to the plot area (instead of the chart)
    legendBox->setPos(plotArea->getLeftX() + (plotArea->getWidth() - legendBox->getWidth()) / 2,
        legendBox->getTopY());
    title->setPos(plotArea->getLeftX() + (plotArea->getWidth() - title->getWidth()) / 2,
        title->getTopY());

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='Revenue of {dataSetName} in {xLabel}: US$ {value}M'");

    return c;
}


BaseChart *datatable2(int /* chartIndex */, const char **imageMap)
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
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='Revenue in {xLabel} {dataSetName}: US$ {value}M'");

    return c;
}


BaseChart *fontxy(int /* chartIndex */, const char **imageMap)
{
    // The data for the chart
    double data0[] = {100, 125, 245, 147, 67};
    double data1[] = {85, 156, 179, 211, 123};
    double data2[] = {97, 87, 56, 267, 157};
    const char *labels[] = {"Mon Jun 4", "Tue Jun 5", "Wed Jun 6", "Thu Jun 7", "Fri Jun 8"};

    // Create a XYChart object of size 540 x 350 pixels
    XYChart *c = new XYChart(540, 350);

    // Set the plot area to start at (120, 40) and of size 280 x 240 pixels
    c->setPlotArea(120, 40, 280, 240);

    // Add a title to the chart using 20pt Times Bold Italic (timesbi.ttf) font and using a deep
    // blue color (0x000080)
    c->addTitle("Weekly Server Load", "timesbi.ttf", 20, 0x000080);

    // Add a legend box at (420, 100) (right of plot area) using 12pt Times Bold font. Sets the
    // background of the legend box to light grey 0xd0d0d0 with a 1 pixel 3D border.
    c->addLegend(420, 100, true, "timesbd.ttf", 12)->setBackground(0xd0d0d0, 0xd0d0d0, 1);

    // Add a title to the y-axis using 12pt Arial Bold/deep blue (0x000080) font. Set the background
    // to yellow (0xffff00) with a 2 pixel 3D border.
    c->yAxis()->setTitle("Throughput (per hour)", "arialbd.ttf", 12, 0x000080)->setBackground(
        0xffff00, 0xffff00, 2);

    // Use 10pt Arial Bold/orange (0xcc6600) font for the y axis labels
    c->yAxis()->setLabelStyle("arialbd.ttf", 10, 0xcc6600);

    // Set the axis label format to "nnn MBytes"
    c->yAxis()->setLabelFormat("{value} MBytes");

    // Use 10pt Arial Bold/green (0x008000) font for the x axis labels. Set the label angle to 45
    // degrees.
    c->xAxis()->setLabelStyle("arialbd.ttf", 10, 0x008000)->setFontAngle(45);

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Add a 3D stack bar layer with a 3D depth of 5 pixels
    BarLayer *layer = c->addBarLayer(Chart::Stack, 5);

    // Use Arial Italic as the default data label font in the bars
    layer->setDataLabelStyle("ariali.ttf");

    // Use 10pt Times Bold Italic (timesbi.ttf) as the aggregate label font. Set the background to
    // flesh (0xffcc66) color with a 1 pixel 3D border.
    layer->setAggregateLabelStyle("timesbi.ttf", 10)->setBackground(0xffcc66, Chart::Transparent, 1)
        ;

    // Add the first data set to the stacked bar layer
    layer->addDataSet(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), -1, "Server #1");

    // Add the second data set to the stacked bar layer
    layer->addDataSet(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), -1, "Server #2");

    // Add the third data set to the stacked bar layer, and set its data label font to Arial Bold
    // Italic.
    TextBox *textbox = layer->addDataSet(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))
        ), -1, "Server #3")->setDataLabelStyle("arialbi.ttf");

    // Set the data label font color for the third data set to yellow (0xffff00)
    textbox->setFontColor(0xffff00);

    // Set the data label background color to the same color as the bar segment, with a 1 pixel 3D
    // border.
    textbox->setBackground(Chart::SameAsMainColor, Chart::Transparent, 1);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName} on {xLabel}: {value} MBytes/hour'");

    return c;
}


BaseChart *background(int chartIndex, const char **imageMap)
{
    // The data for the chart
    double data[] = {85, 156, 179.5, 211, 123};
    const char *labels[] = {"Mon", "Tue", "Wed", "Thu", "Fri"};

    // Create a XYChart object of size 270 x 270 pixels
    XYChart *c = new XYChart(270, 270);

    // Set the plot area at (40, 32) and of size 200 x 200 pixels
    PlotArea *plotarea = c->setPlotArea(40, 32, 200, 200);

    // Set the background style based on the input parameter
    if (chartIndex == 0) {
        // Has wallpaper image
        c->setWallpaper("tile.gif");
    } else if (chartIndex == 1) {
        // Use a background image as the plot area background
        plotarea->setBackground("bg.png");
    } else if (chartIndex == 2) {
        // Use white (0xffffff) and grey (0xe0e0e0) as two alternate plotarea background colors
        plotarea->setBackground(0xffffff, 0xe0e0e0);
    } else {
        // Use a dark background palette
        c->setColors(Chart::whiteOnBlackPalette);
    }

    // Set the labels on the x axis
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Add a color bar layer using the given data. Use a 1 pixel 3D border for the bars.
    c->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), IntArray(0, 0)
        )->setBorderColor(-1, 1);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='Revenue for {xLabel}: US${value}K'");

    return c;
}


BaseChart *logaxis(int chartIndex, const char **imageMap)
{
    // The data for the chart
    double data[] = {100, 125, 260, 147, 67};
    const char *labels[] = {"Mon", "Tue", "Wed", "Thu", "Fri"};

    // Create a XYChart object of size 200 x 180 pixels
    XYChart *c = new XYChart(200, 180);

    // Set the plot area at (30, 10) and of size 140 x 130 pixels
    c->setPlotArea(30, 10, 140, 130);

    // Ise log scale axis if required
    if (chartIndex == 1) {
        c->yAxis()->setLogScale();
    }

    // Set the labels on the x axis
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Add a color bar layer using the given data. Use a 1 pixel 3D border for the bars.
    c->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), IntArray(0, 0)
        )->setBorderColor(-1, 1);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='Mileage on {xLabel}: {value} miles'");

    return c;
}


BaseChart *axisscale(int chartIndex, const char **imageMap)
{
    // The data for the chart
    double data[] = {5.5, 3.5, -3.7, 1.7, -1.4, 3.3};
    const char *labels[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun"};

    // Create a XYChart object of size 200 x 190 pixels
    XYChart *c = new XYChart(200, 190);

    // Set the plot area at (30, 20) and of size 140 x 140 pixels
    c->setPlotArea(30, 20, 140, 140);

    // Configure the axis as according to the input parameter
    if (chartIndex == 0) {
        c->addTitle("No Axis Extension", "arial.ttf", 8);
    } else if (chartIndex == 1) {
        c->addTitle("Top/Bottom Extensions = 0/0", "arial.ttf", 8);
        // Reserve 20% margin at top of plot area when auto-scaling
        c->yAxis()->setAutoScale(0, 0);
    } else if (chartIndex == 2) {
        c->addTitle("Top/Bottom Extensions = 0.2/0.2", "arial.ttf", 8);
        // Reserve 20% margin at top and bottom of plot area when auto-scaling
        c->yAxis()->setAutoScale(0.2, 0.2);
    } else if (chartIndex == 3) {
        c->addTitle("Axis Top Margin = 15", "arial.ttf", 8);
        // Reserve 15 pixels at top of plot area
        c->yAxis()->setMargin(15);
    } else {
        c->addTitle("Manual Scale -5 to 10", "arial.ttf", 8);
        // Set the y axis to scale from -5 to 10, with ticks every 5 units
        c->yAxis()->setLinearScale(-5, 10, 5);
    }

    // Set the labels on the x axis
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Add a color bar layer using the given data. Use a 1 pixel 3D border for the bars.
    c->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), IntArray(0, 0)
        )->setBorderColor(-1, 1);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='ROI for {xLabel}: {value}%'");

    return c;
}


BaseChart *ticks(int chartIndex, const char **imageMap)
{
    // The data for the chart
    double data[] = {100, 125, 265, 147, 67, 105};
    const char *labels[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun"};

    // Create a XYChart object of size 250 x 250 pixels
    XYChart *c = new XYChart(250, 250);

    // Set the plot area at (27, 25) and of size 200 x 200 pixels
    c->setPlotArea(27, 25, 200, 200);

    if (chartIndex == 1) {
        // High tick density, uses 10 pixels as tick spacing
        c->addTitle("Tick Density = 10 pixels");
        c->yAxis()->setTickDensity(10);
    } else {
        // Normal tick density, just use the default setting
        c->addTitle("Default Tick Density");
    }

    // Set the labels on the x axis
    c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Add a color bar layer using the given data. Use a 1 pixel 3D border for the bars.
    c->addBarLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), IntArray(0, 0)
        )->setBorderColor(-1, 1);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='Revenue for {xLabel}: US${value}M'");

    return c;
}


BaseChart *surface(int /* chartIndex */, const char ** /* imageMap */)
{
    // The x and y coordinates of the grid
    double dataX[] = {-10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    double dataY[] = {-10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // The values at the grid points. In this example, we will compute the values using the formula
    // z = x * sin(y) + y * sin(x).
    double dataZ[(int)(sizeof(dataX) / sizeof(dataX[0])) * (int)(sizeof(dataY) / sizeof(dataY[0]))];
    for(int yIndex = 0; yIndex < (int)(sizeof(dataY) / sizeof(dataY[0])); ++yIndex) {
        double y = dataY[yIndex];
        for(int xIndex = 0; xIndex < (int)(sizeof(dataX) / sizeof(dataX[0])); ++xIndex) {
            double x = dataX[xIndex];
            dataZ[yIndex * (int)(sizeof(dataX) / sizeof(dataX[0])) + xIndex] = x * sin(y) + y * sin(
                x);
        }
    }

    // Create a SurfaceChart object of size 720 x 600 pixels
    SurfaceChart *c = new SurfaceChart(720, 600);

    // Add a title to the chart using 20 points Times New Roman Italic font
    c->addTitle("Surface Energy Density   ", "timesi.ttf", 20);

    // Set the center of the plot region at (350, 280), and set width x depth x height to 360 x 360
    // x 270 pixels
    c->setPlotRegion(350, 280, 360, 360, 270);

    // Set the data to use to plot the chart
    c->setData(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0]))), DoubleArray(dataY,
        (int)(sizeof(dataY) / sizeof(dataY[0]))), DoubleArray(dataZ, (int)(sizeof(dataZ) / sizeof(
        dataZ[0]))));

    // Spline interpolate data to a 80 x 80 grid for a smooth surface
    c->setInterpolation(80, 80);

    // Add a color axis (the legend) in which the left center is anchored at (645, 270). Set the
    // length to 200 pixels and the labels on the right side.
    c->setColorAxis(645, 270, Chart::Left, 200, Chart::Right);

    // Set the x, y and z axis titles using 10 points Arial Bold font
    c->xAxis()->setTitle("X (nm)", "arialbd.ttf", 10);
    c->yAxis()->setTitle("Y (nm)", "arialbd.ttf", 10);
    c->zAxis()->setTitle("Energy Density (J/m<*font,super*>2<*/font*>)", "arialbd.ttf", 10);

    // Output the chart
    c->makeChart();

    return c;
}


BaseChart *surface2(int /* chartIndex */, const char ** /* imageMap */)
{
    // The x and y coordinates of the grid
    double dataX[] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    double dataY[] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};

    // The values at the grid points. In this example, we will compute the values using the formula
    // z = sin((x - 0.5) * 2 * pi) * sin((y - 0.5) * 2 * pi)
    double dataZ[(int)(sizeof(dataX) / sizeof(dataX[0])) * (int)(sizeof(dataY) / sizeof(dataY[0]))];
    for(int yIndex = 0; yIndex < (int)(sizeof(dataY) / sizeof(dataY[0])); ++yIndex) {
        double y = (dataY[yIndex] - 0.5) * 2 * 3.1416;
        for(int xIndex = 0; xIndex < (int)(sizeof(dataX) / sizeof(dataX[0])); ++xIndex) {
            double x = (dataX[xIndex] - 0.5) * 2 * 3.1416;
            dataZ[yIndex * (int)(sizeof(dataX) / sizeof(dataX[0])) + xIndex] = sin(x) * sin(y);
        }
    }

    // Create a SurfaceChart object of size 720 x 540 pixels
    SurfaceChart *c = new SurfaceChart(720, 540);

    // Add a title to the chart using 20 points Times New Roman Italic font
    c->addTitle("Quantum Wave Function", "timesi.ttf", 20);

    // Set the center of the plot region at (360, 245), and set width x depth x height to 360 x 360
    // x 270 pixels
    c->setPlotRegion(360, 245, 360, 360, 270);

    // Set the elevation and rotation angles to 20 and 30 degrees
    c->setViewAngle(20, 30);

    // Set the data to use to plot the chart
    c->setData(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0]))), DoubleArray(dataY,
        (int)(sizeof(dataY) / sizeof(dataY[0]))), DoubleArray(dataZ, (int)(sizeof(dataZ) / sizeof(
        dataZ[0]))));

    // Spline interpolate data to a 80 x 80 grid for a smooth surface
    c->setInterpolation(80, 80);

    // Set surface grid lines to semi-transparent black (dd000000)
    c->setSurfaceAxisGrid(0xdd000000);

    // Set contour lines to semi-transparent white (80ffffff)
    c->setContourColor(0x80ffffff);

    // Add a color axis (the legend) in which the left center is anchored at (645, 270). Set the
    // length to 200 pixels and the labels on the right side. Use smooth gradient coloring.
    c->setColorAxis(645, 270, Chart::Left, 200, Chart::Right)->setColorGradient();

    // Set the x, y and z axis titles using 10 points Arial Bold font
    c->xAxis()->setTitle("x/L(x)", "arialbd.ttf", 10);
    c->yAxis()->setTitle("y/L(y)", "arialbd.ttf", 10);
    c->zAxis()->setTitle("Wave Function Amplitude", "arialbd.ttf", 10);

    // Output the chart
    c->makeChart();

    return c;
}


BaseChart *surface3(int /* chartIndex */, const char ** /* imageMap */)
{
    // The x and y coordinates of the grid
    double dataX[] = {-10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    double dataY[] = {-10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // The values at the grid points. In this example, we will compute the values using the formula
    // z = Sin(x * x / 128 - y * y / 256 + 3) * Cos(x / 4 + 1 - Exp(y / 8))
    double dataZ[(int)(sizeof(dataX) / sizeof(dataX[0])) * (int)(sizeof(dataY) / sizeof(dataY[0]))];
    for(int yIndex = 0; yIndex < (int)(sizeof(dataY) / sizeof(dataY[0])); ++yIndex) {
        double y = dataY[yIndex];
        for(int xIndex = 0; xIndex < (int)(sizeof(dataX) / sizeof(dataX[0])); ++xIndex) {
            double x = dataX[xIndex];
            dataZ[yIndex * (int)(sizeof(dataX) / sizeof(dataX[0])) + xIndex] = sin(x * x / 128.0 - y
                 * y / 256.0 + 3) * cos(x / 4.0 + 1 - exp(y / 8.0));
        }
    }

    // Create a SurfaceChart object of size 750 x 600 pixels
    SurfaceChart *c = new SurfaceChart(750, 600);

    // Add a title to the chart using 20 points Times New Roman Italic font
    c->addTitle("Surface Energy Density       ", "timesi.ttf", 20);

    // Set the center of the plot region at (380, 260), and set width x depth x height to 360 x 360
    // x 270 pixels
    c->setPlotRegion(380, 260, 360, 360, 270);

    // Set the elevation and rotation angles to 30 and 210 degrees
    c->setViewAngle(30, 210);

    // Set the perspective level to 60
    c->setPerspective(60);

    // Set the data to use to plot the chart
    c->setData(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0]))), DoubleArray(dataY,
        (int)(sizeof(dataY) / sizeof(dataY[0]))), DoubleArray(dataZ, (int)(sizeof(dataZ) / sizeof(
        dataZ[0]))));

    // Spline interpolate data to a 80 x 80 grid for a smooth surface
    c->setInterpolation(80, 80);

    // Use semi-transparent black (c0000000) for x and y major surface grid lines. Use dotted style
    // for x and y minor surface grid lines.
    int majorGridColor = 0xc0000000;
    int minorGridColor = c->dashLineColor(majorGridColor, Chart::DotLine);
    c->setSurfaceAxisGrid(majorGridColor, majorGridColor, minorGridColor, minorGridColor);

    // Set contour lines to semi-transparent white (80ffffff)
    c->setContourColor(0x80ffffff);

    // Add a color axis (the legend) in which the left center is anchored at (665, 280). Set the
    // length to 200 pixels and the labels on the right side.
    c->setColorAxis(665, 280, Chart::Left, 200, Chart::Right);

    // Set the x, y and z axis titles using 12 points Arial Bold font
    c->xAxis()->setTitle("X Title\nPlaceholder", "arialbd.ttf", 12);
    c->yAxis()->setTitle("Y Title\nPlaceholder", "arialbd.ttf", 12);
    c->zAxis()->setTitle("Z Title Placeholder", "arialbd.ttf", 12);

    // Output the chart
    c->makeChart();

    return c;
}


BaseChart *scattersurface(int /* chartIndex */, const char ** /* imageMap */)
{
    // The (x, y, z) coordinates of the scattered data
    double dataX[] = {0.5, 1.9, 4.9, 1.0, 8.9, 9.8, 5.9, 2.9, 6.8, 9.0, 0.0, 8.9, 1.9, 4.8, 2.4,
        3.4, 7.9, 7.5, 4.8, 7.5, 9.5, 0.4, 8.9, 0.9, 5.4, 9.4, 2.9, 8.9, 0.9, 8.9, 10.0, 1.0, 6.8,
        3.8, 9.0, 5.3, 6.4, 4.9, 4.5, 2.0, 5.4, 0.0, 10.0, 3.9, 5.4, 5.9, 5.8, 0.3, 4.4, 8.3};
    double dataY[] = {3.3, 3.0, 0.7, 1.0, 9.3, 4.5, 8.4, 0.1, 0.8, 0.1, 9.3, 1.8, 4.3, 1.3, 2.3,
        5.4, 6.9, 9.0, 9.8, 7.5, 1.8, 1.4, 4.5, 7.8, 3.8, 4.0, 2.9, 2.4, 3.9, 2.9, 2.3, 9.3, 2.0,
        3.4, 4.8, 2.3, 3.4, 2.3, 1.5, 7.8, 4.5, 0.9, 6.3, 2.4, 6.9, 2.8, 1.3, 2.9, 6.4, 6.3};
    double dataZ[] = {6.6, 12.5, 7.4, 6.2, 9.6, 13.6, 19.9, 2.2, 6.9, 3.4, 8.7, 8.4, 7.8, 8.0, 9.4,
        11.9, 9.6, 15.7, 12.0, 13.3, 9.6, 6.4, 9.0, 6.9, 4.6, 9.7, 10.6, 9.2, 7.0, 6.9, 9.7, 8.6,
        8.0, 13.6, 13.2, 5.9, 9.0, 3.2, 8.3, 9.7, 8.2, 6.1, 8.7, 5.6, 14.9, 9.8, 9.3, 5.1, 10.8, 9.8
        };

    // Create a SurfaceChart object of size 680 x 550 pixels. Set background to brushed silver and
    // border to grey (888888). Set the top-left and bottom-right corners to rounded corners with 20
    // pixels radius.
    SurfaceChart *c = new SurfaceChart(680, 550, Chart::brushedSilverColor(), 0x888888);
    c->setRoundedFrame(0xffffff, 20, 0, 20, 0);

    // Add a title to the chart using 20 points Times New Roman Italic font. Set top/bottom margin
    // to 8 pixels.
    TextBox *title = c->addTitle("Surface Created Using Scattered Data Points", "timesi.ttf", 20);
    title->setMargin(0, 0, 8, 8);

    // Add a 2 pixel wide black (000000) separator line under the title
    c->addLine(10, title->getHeight(), c->getWidth() - 10, title->getHeight(), 0x000000, 2);

    // Set the center of the plot region at (290, 235), and set width x depth x height to 360 x 360
    // x 180 pixels
    c->setPlotRegion(290, 235, 360, 360, 180);

    // Set the elevation and rotation angles to 45 and -45 degrees
    c->setViewAngle(45, -45);

    // Set the perspective level to 30
    c->setPerspective(30);

    // Set the data to use to plot the chart
    c->setData(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0]))), DoubleArray(dataY,
        (int)(sizeof(dataY) / sizeof(dataY[0]))), DoubleArray(dataZ, (int)(sizeof(dataZ) / sizeof(
        dataZ[0]))));

    // Add a color axis (the legend) in which the top right corner is anchored at (660, 80). Set the
    // length to 200 pixels and the labels on the right side.
    ColorAxis *cAxis = c->setColorAxis(660, 80, Chart::TopRight, 200, Chart::Right);

    // Set the color axis title with 12 points Arial Bold font
    cAxis->setTitle("Z Title Placeholder", "arialbd.ttf", 12);

    // Add a bounding box with light grey (eeeeee) background and grey (888888) border. Set the
    // top-left and bottom-right corners to rounded corners of  10 pixels radius.
    cAxis->setBoundingBox(0xeeeeee, 0x888888);
    cAxis->setRoundedCorners(10, 0, 10, 0);

    // Set surface grid lines to semi-transparent black (cc000000)
    c->setSurfaceAxisGrid(0xcc000000);

    // Set contour lines to semi-transparent white (80ffffff)
    c->setContourColor(0x80ffffff);

    // Set the walls to black in color
    c->setWallColor(0x000000);

    // Set the xyz major wall grid lines to white (ffffff), and minor wall grid lines to grey
    // (888888)
    c->setWallGrid(0xffffff, 0xffffff, 0xffffff, 0x888888, 0x888888, 0x888888);

    // Set the wall thickness to 0
    c->setWallThickness(0, 0, 0);

    // Show only the xy wall, and hide the yz and zx walls.
    c->setWallVisibility(true, false, false);

    // Set the x, y and z axis titles using 12 points Arial Bold font
    c->xAxis()->setTitle("X Title\nPlaceholder", "arialbd.ttf", 12);
    c->yAxis()->setTitle("Y Title\nPlaceholder", "arialbd.ttf", 12);

    // Output the chart
    c->makeChart();

    return c;
}


BaseChart *surfaceaxis(int /* chartIndex */, const char ** /* imageMap */)
{
    // The x and y coordinates of the grid
    double dataX[] = {Chart::chartTime(2008, 9, 1), Chart::chartTime(2008, 9, 2), Chart::chartTime(
        2008, 9, 3), Chart::chartTime(2008, 9, 4), Chart::chartTime(2008, 9, 5), Chart::chartTime(
        2008, 9, 6)};
    const char *dataY[] = {"Low", "Medium", "High"};

    // The data series
    double lowData[] = {24, 38, 33, 25, 28, 36};
    double mediumData[] = {49, 42, 34, 47, 53, 50};
    double highData[] = {44, 51, 38, 33, 47, 42};

    // Create a SurfaceChart object of size 760 x 500 pixels
    SurfaceChart *c = new SurfaceChart(760, 500);

    // Add a title to the chart using 18 points Arial font
    c->addTitle("Surface Chart Axis Types", "arial.ttf", 18);

    // Set the center of the plot region at (385, 240), and set width x depth x height to 480 x 240
    // x 240 pixels
    c->setPlotRegion(385, 240, 480, 240, 240);

    // Set the elevation and rotation angles to 30 and -10 degrees
    c->setViewAngle(30, -10);

    // Set the data to use to plot the chart. As the y-data are text strings (enumerated), we will
    // use an empty array for the y-coordinates. For the z data series, they are just the
    // concatenation of the individual data series.
    c->setData(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0]))), DoubleArray(),
        ArrayMath(DoubleArray(lowData, (int)(sizeof(lowData) / sizeof(lowData[0])))).insert(
        DoubleArray(mediumData, (int)(sizeof(mediumData) / sizeof(mediumData[0])))).insert(
        DoubleArray(highData, (int)(sizeof(highData) / sizeof(highData[0])))));

    // Set the y-axis labels
    c->yAxis()->setLabels(StringArray(dataY, (int)(sizeof(dataY) / sizeof(dataY[0]))));

    // Set x-axis tick density to 75 pixels. ChartDirector auto-scaling will use this as the
    // guideline when putting ticks on the x-axis.
    c->xAxis()->setTickDensity(75);

    // Spline interpolate data to a 80 x 40 grid for a smooth surface
    c->setInterpolation(80, 40);

    // Set surface grid lines to semi-transparent black (cc000000).
    c->setSurfaceAxisGrid(0xcc000000);

    // Set contour lines to the same color as the fill color at the contour level
    c->setContourColor(Chart::SameAsMainColor);

    // Add a color axis (the legend) in which the top right corner is anchored at (95, 100). Set the
    // length to 160 pixels and the labels on the left side.
    ColorAxis *cAxis = c->setColorAxis(95, 100, Chart::TopRight, 160, Chart::Left);

    // Add a bounding box with light grey (eeeeee) background and grey (888888) border.
    cAxis->setBoundingBox(0xeeeeee, 0x888888);

    // Set label style to Arial bold for all axes
    c->xAxis()->setLabelStyle("arialbd.ttf");
    c->yAxis()->setLabelStyle("arialbd.ttf");
    c->zAxis()->setLabelStyle("arialbd.ttf");
    c->colorAxis()->setLabelStyle("arialbd.ttf");

    // Set the x, y and z axis titles using deep blue (000088) 15 points Arial font
    c->xAxis()->setTitle("Date/Time Axis", "ariali.ttf", 15, 0x000088);
    c->yAxis()->setTitle("Label\nBased\nAxis", "ariali.ttf", 15, 0x000088);
    c->zAxis()->setTitle("Numeric Axis", "ariali.ttf", 15, 0x000088);

    // Output the chart
    c->makeChart();

    return c;
}


BaseChart *surfacelighting(int chartIndex, const char ** /* imageMap */)
{
    // The x and y coordinates of the grid
    double dataX[] = {-10, -8, -6, -4, -2, 0, 2, 4, 6, 8, 10};
    double dataY[] = {-10, -8, -6, -4, -2, 0, 2, 4, 6, 8, 10};

    // The values at the grid points. In this example, we will compute the values using the formula
    // z = x * sin(y) + y * sin(x).
    double dataZ[(int)(sizeof(dataX) / sizeof(dataX[0])) * (int)(sizeof(dataY) / sizeof(dataY[0]))];
    for(int yIndex = 0; yIndex < (int)(sizeof(dataY) / sizeof(dataY[0])); ++yIndex) {
        double y = dataY[yIndex];
        for(int xIndex = 0; xIndex < (int)(sizeof(dataX) / sizeof(dataX[0])); ++xIndex) {
            double x = dataX[xIndex];
            dataZ[yIndex * (int)(sizeof(dataX) / sizeof(dataX[0])) + xIndex] = x * sin(y) + y * sin(
                x);
        }
    }

    // Create a SurfaceChart object of size 380 x 400 pixels, with white (ffffff) background and
    // grey (888888) border.
    SurfaceChart *c = new SurfaceChart(380, 400, 0xffffff, 0x888888);

    // Demonstrate various lighting parameters
    if (chartIndex == 0) {
        c->addTitle(
            "Default Lighting<*br*><*size=10*>Ambient = 0.5, Diffuse = 0.5, Specular = 1, "
            "Shininess = 8");
    } else if (chartIndex == 1) {
        c->addTitle(
            "Matte (Non-Glossy) Lighting<*br*><*size=10*>Ambient = 0.5, Diffuse = 0.5, Specular = "
            "0, Shininess = 0");
        c->setLighting(0.5, 0.5, 0, 0);
    } else if (chartIndex == 2) {
        c->addTitle(
            "Flat Lighting<*br*><*size=10*>Ambient = 1, Diffuse = 0, Specular = 0, Shininess = 0");
        c->setLighting(1, 0, 0, 0);
    } else {
        c->addTitle(
            "Strong Glossy Lighting<*br*><*size=10*>Ambient = 0.5, Diffuse = 0.5, Specular = 4, "
            "Shininess = 32");
        c->setLighting(0.5, 0.5, 4, 32);
    }

    // Set the center of the plot region at (175, 200), and set width x depth x height to 200 x 200
    // x 160 pixels
    c->setPlotRegion(175, 200, 200, 200, 160);

    // Set the plot region wall thichness to 5 pixels
    c->setWallThickness(5);

    // Set the elevation and rotation angles to 45 and 60 degrees
    c->setViewAngle(45, 60);

    // Set the perspective level to 35
    c->setPerspective(35);

    // Set the data to use to plot the chart
    c->setData(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0]))), DoubleArray(dataY,
        (int)(sizeof(dataY) / sizeof(dataY[0]))), DoubleArray(dataZ, (int)(sizeof(dataZ) / sizeof(
        dataZ[0]))));

    // Spline interpolate data to a 50 x 50 grid for a smooth surface
    c->setInterpolation(50, 50);

    // Set contour lines to semi-transparent black (c0000000)
    c->setContourColor(0xc0000000);

    // Output the chart
    c->makeChart();

    return c;
}


BaseChart *surfaceshading(int chartIndex, const char ** /* imageMap */)
{
    // The x and y coordinates of the grid
    double dataX[] = {-10, -8, -6, -4, -2, 0, 2, 4, 6, 8, 10};
    double dataY[] = {-10, -8, -6, -4, -2, 0, 2, 4, 6, 8, 10};

    // The values at the grid points. In this example, we will compute the values using the formula
    // z = x * sin(y) + y * sin(x).
    double dataZ[(int)(sizeof(dataX) / sizeof(dataX[0])) * (int)(sizeof(dataY) / sizeof(dataY[0]))];
    for(int yIndex = 0; yIndex < (int)(sizeof(dataY) / sizeof(dataY[0])); ++yIndex) {
        double y = dataY[yIndex];
        for(int xIndex = 0; xIndex < (int)(sizeof(dataX) / sizeof(dataX[0])); ++xIndex) {
            double x = dataX[xIndex];
            dataZ[yIndex * (int)(sizeof(dataX) / sizeof(dataX[0])) + xIndex] = x * sin(y) + y * sin(
                x);
        }
    }

    // Create a SurfaceChart object of size 380 x 400 pixels, with white (ffffff) background and
    // grey (888888) border.
    SurfaceChart *c = new SurfaceChart(380, 400, 0xffffff, 0x888888);

    // Demonstrate various shading methods
    if (chartIndex == 0) {
        c->addTitle("11 x 11 Data Points\nSmooth Shading");
    } else if (chartIndex == 1) {
        c->addTitle("11 x 11 Points - Spline Fitted to 50 x 50\nSmooth Shading");
        c->setInterpolation(50, 50);
    } else if (chartIndex == 2) {
        c->addTitle("11 x 11 Data Points\nRectangular Shading");
        c->setShadingMode(Chart::RectangularShading);
    } else {
        c->addTitle("11 x 11 Data Points\nTriangular Shading");
        c->setShadingMode(Chart::TriangularShading);
    }

    // Set the center of the plot region at (175, 200), and set width x depth x height to 200 x 200
    // x 160 pixels
    c->setPlotRegion(175, 200, 200, 200, 160);

    // Set the plot region wall thichness to 5 pixels
    c->setWallThickness(5);

    // Set the elevation and rotation angles to 45 and 60 degrees
    c->setViewAngle(45, 60);

    // Set the perspective level to 35
    c->setPerspective(35);

    // Set the data to use to plot the chart
    c->setData(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0]))), DoubleArray(dataY,
        (int)(sizeof(dataY) / sizeof(dataY[0]))), DoubleArray(dataZ, (int)(sizeof(dataZ) / sizeof(
        dataZ[0]))));

    // Set contour lines to semi-transparent black (c0000000)
    c->setContourColor(0xc0000000);

    // Output the chart
    c->makeChart();

    return c;
}


BaseChart *surfacewireframe(int chartIndex, const char ** /* imageMap */)
{
    // The x and y coordinates of the grid
    double dataX[] = {-2, -1, 0, 1, 2};
    double dataY[] = {-2, -1, 0, 1, 2};

    // The values at the grid points. In this example, we will compute the values using the formula
    // z = square_root(15 - x * x - y * y).
    double dataZ[(int)(sizeof(dataX) / sizeof(dataX[0])) * (int)(sizeof(dataY) / sizeof(dataY[0]))];
    for(int yIndex = 0; yIndex < (int)(sizeof(dataY) / sizeof(dataY[0])); ++yIndex) {
        double y = dataY[yIndex];
        for(int xIndex = 0; xIndex < (int)(sizeof(dataX) / sizeof(dataX[0])); ++xIndex) {
            double x = dataX[xIndex];
            dataZ[yIndex * (int)(sizeof(dataX) / sizeof(dataX[0])) + xIndex] = sqrt(15 - x * x - y *
                y);
        }
    }

    // Create a SurfaceChart object of size 380 x 340 pixels, with white (ffffff) background and
    // grey (888888) border.
    SurfaceChart *c = new SurfaceChart(380, 340, 0xffffff, 0x888888);

    // Demonstrate various wireframes with and without interpolation
    if (chartIndex == 0) {
        // Original data without interpolation
        c->addTitle("5 x 5 Data Points\nStandard Shading", "arialbd.ttf", 12);
        c->setContourColor(0x80ffffff);
    } else if (chartIndex == 1) {
        // Original data, spline interpolated to 40 x 40 for smoothness
        c->addTitle("5 x 5 Points - Spline Fitted to 40 x 40\nStandard Shading", "arialbd.ttf", 12);
        c->setContourColor(0x80ffffff);
        c->setInterpolation(40, 40);
    } else if (chartIndex == 2) {
        // Rectangular wireframe of original data
        c->addTitle("5 x 5 Data Points\nRectangular Wireframe");
        c->setShadingMode(Chart::RectangularFrame);
    } else if (chartIndex == 3) {
        // Rectangular wireframe of original data spline interpolated to 40 x 40
        c->addTitle("5 x 5 Points - Spline Fitted to 40 x 40\nRectangular Wireframe");
        c->setShadingMode(Chart::RectangularFrame);
        c->setInterpolation(40, 40);
    } else if (chartIndex == 4) {
        // Triangular wireframe of original data
        c->addTitle("5 x 5 Data Points\nTriangular Wireframe");
        c->setShadingMode(Chart::TriangularFrame);
    } else {
        // Triangular wireframe of original data spline interpolated to 40 x 40
        c->addTitle("5 x 5 Points - Spline Fitted to 40 x 40\nTriangular Wireframe");
        c->setShadingMode(Chart::TriangularFrame);
        c->setInterpolation(40, 40);
    }

    // Set the center of the plot region at (200, 170), and set width x depth x height to 200 x 200
    // x 150 pixels
    c->setPlotRegion(200, 170, 200, 200, 150);

    // Set the plot region wall thichness to 5 pixels
    c->setWallThickness(5);

    // Set the elevation and rotation angles to 20 and 30 degrees
    c->setViewAngle(20, 30);

    // Set the data to use to plot the chart
    c->setData(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0]))), DoubleArray(dataY,
        (int)(sizeof(dataY) / sizeof(dataY[0]))), DoubleArray(dataZ, (int)(sizeof(dataZ) / sizeof(
        dataZ[0]))));

    // Output the chart
    c->makeChart();

    return c;
}


BaseChart *surfaceperspective(int chartIndex, const char ** /* imageMap */)
{
    char buffer[256];

    // The x and y coordinates of the grid
    double dataX[] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    double dataY[] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};

    // The values at the grid points. In this example, we will compute the values using the formula
    // z = sin((x - 0.5) * 2 * pi) * sin((y - 0.5) * 2 * pi)
    double dataZ[(int)(sizeof(dataX) / sizeof(dataX[0])) * (int)(sizeof(dataY) / sizeof(dataY[0]))];
    for(int yIndex = 0; yIndex < (int)(sizeof(dataY) / sizeof(dataY[0])); ++yIndex) {
        double y = (dataY[yIndex] - 0.5) * 2 * 3.1416;
        for(int xIndex = 0; xIndex < (int)(sizeof(dataX) / sizeof(dataX[0])); ++xIndex) {
            double x = (dataX[xIndex] - 0.5) * 2 * 3.1416;
            dataZ[yIndex * (int)(sizeof(dataX) / sizeof(dataX[0])) + xIndex] = sin(x) * sin(y);
        }
    }

    // the perspective level
    int perspective = chartIndex * 12;

    // Create a SurfaceChart object of size 360 x 360 pixels, with white (ffffff) background and
    // grey (888888) border.
    SurfaceChart *c = new SurfaceChart(360, 360, 0xffffff, 0x888888);

    // Set the perspective level
    c->setPerspective(perspective);
    sprintf(buffer, "Perspective = %d", perspective);
    c->addTitle(buffer);

    // Set the center of the plot region at (195, 165), and set width x depth x height to 200 x 200
    // x 150 pixels
    c->setPlotRegion(195, 165, 200, 200, 150);

    // Set the plot region wall thichness to 5 pixels
    c->setWallThickness(5);

    // Set the elevation and rotation angles to 30 and 30 degrees
    c->setViewAngle(30, 30);

    // Set the data to use to plot the chart
    c->setData(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(dataX[0]))), DoubleArray(dataY,
        (int)(sizeof(dataY) / sizeof(dataY[0]))), DoubleArray(dataZ, (int)(sizeof(dataZ) / sizeof(
        dataZ[0]))));

    // Spline interpolate data to a 40 x 40 grid for a smooth surface
    c->setInterpolation(40, 40);

    // Use smooth gradient coloring.
    c->colorAxis()->setColorGradient();

    // Output the chart
    c->makeChart();

    return c;
}


BaseChart *threedscatter(int /* chartIndex */, const char **imageMap)
{
    // The XYZ data for the 3D scatter chart as 3 random data series
    RanSeries *r = new RanSeries(0);
    DoubleArray xData = r->getSeries(100, 100, -10, 10);
    DoubleArray yData = r->getSeries(100, 0, 0, 20);
    DoubleArray zData = r->getSeries(100, 100, -10, 10);

    // Create a ThreeDScatterChart object of size 720 x 600 pixels
    ThreeDScatterChart *c = new ThreeDScatterChart(720, 600);

    // Add a title to the chart using 20 points Times New Roman Italic font
    c->addTitle("3D Scatter Chart (1)  ", "timesi.ttf", 20);

    // Set the center of the plot region at (350, 280), and set width x depth x height to 360 x 360
    // x 270 pixels
    c->setPlotRegion(350, 280, 360, 360, 270);

    // Add a scatter group to the chart using 11 pixels glass sphere symbols, in which the color
    // depends on the z value of the symbol
    c->addScatterGroup(xData, yData, zData, "", Chart::GlassSphere2Shape, 11, Chart::SameAsMainColor
        );

    // Add a color axis (the legend) in which the left center is anchored at (645, 270). Set the
    // length to 200 pixels and the labels on the right side.
    c->setColorAxis(645, 270, Chart::Left, 200, Chart::Right);

    // Set the x, y and z axis titles using 10 points Arial Bold font
    c->xAxis()->setTitle("X-Axis Place Holder", "arialbd.ttf", 10);
    c->yAxis()->setTitle("Y-Axis Place Holder", "arialbd.ttf", 10);
    c->zAxis()->setTitle("Z-Axis Place Holder", "arialbd.ttf", 10);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='(x={x|p}, y={y|p}, z={z|p}'");

    //free up resources
    delete r;

    return c;
}


BaseChart *threedscatter2(int /* chartIndex */, const char **imageMap)
{
    // The XYZ data for the 3D scatter chart as 3 random data series
    RanSeries *r = new RanSeries(3);
    DoubleArray xData = r->getSeries(20, 100, -10, 10);
    DoubleArray yData = r->getSeries(20, 100, -10, 10);
    DoubleArray zData = r->getSeries(20, 100, -10, 10);

    // Create a ThreeDScatterChart object of size 720 x 520 pixels
    ThreeDScatterChart *c = new ThreeDScatterChart(720, 520);

    // Add a title to the chart using 20 points Times New Roman Italic font
    c->addTitle("3D Scatter Chart (2)  ", "timesi.ttf", 20);

    // Set the center of the plot region at (350, 240), and set width x depth x height to 360 x 360
    // x 270 pixels
    c->setPlotRegion(350, 240, 360, 360, 270);

    // Set the elevation and rotation angles to 15 and 30 degrees
    c->setViewAngle(15, 30);

    // Add a scatter group to the chart using 13 pixels glass sphere symbols, in which the color
    // depends on the z value of the symbol
    ThreeDScatterGroup *g = c->addScatterGroup(xData, yData, zData, "", Chart::GlassSphere2Shape,
        13, Chart::SameAsMainColor);

    // Add grey (888888) drop lines to the symbols
    g->setDropLine(0x888888);

    // Add a color axis (the legend) in which the left center is anchored at (645, 220). Set the
    // length to 200 pixels and the labels on the right side. Use smooth gradient coloring.
    c->setColorAxis(645, 220, Chart::Left, 200, Chart::Right)->setColorGradient();

    // Set the x, y and z axis titles using 10 points Arial Bold font
    c->xAxis()->setTitle("X-Axis Place Holder", "arialbd.ttf", 10);
    c->yAxis()->setTitle("Y-Axis Place Holder", "arialbd.ttf", 10);
    c->zAxis()->setTitle("Z-Axis Place Holder", "arialbd.ttf", 10);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='(x={x|p}, y={y|p}, z={z|p}'");

    //free up resources
    delete r;

    return c;
}


BaseChart *threedscattergroups(int /* chartIndex */, const char ** /* imageMap */)
{
    // The random XYZ data for the first 3D scatter group
    RanSeries *r0 = new RanSeries(7);
    DoubleArray xData0 = r0->getSeries(100, 100, -10, 10);
    DoubleArray yData0 = r0->getSeries(100, 0, 0, 20);
    DoubleArray zData0 = r0->getSeries(100, 100, -10, 10);

    // The random XYZ data for the second 3D scatter group
    RanSeries *r1 = new RanSeries(4);
    DoubleArray xData1 = r1->getSeries(100, 100, -10, 10);
    DoubleArray yData1 = r1->getSeries(100, 0, 0, 20);
    DoubleArray zData1 = r1->getSeries(100, 100, -10, 10);

    // The random XYZ data for the third 3D scatter group
    RanSeries *r2 = new RanSeries(8);
    DoubleArray xData2 = r2->getSeries(100, 100, -10, 10);
    DoubleArray yData2 = r2->getSeries(100, 0, 0, 20);
    DoubleArray zData2 = r2->getSeries(100, 100, -10, 10);

    // Create a ThreeDScatterChart object of size 800 x 520 pixels
    ThreeDScatterChart *c = new ThreeDScatterChart(800, 520);

    // Add a title to the chart using 20 points Times New Roman Italic font
    c->addTitle("3D Scatter Groups                    ", "timesi.ttf", 20);

    // Set the center of the plot region at (350, 240), and set width x depth x height to 360 x 360
    // x 270 pixels
    c->setPlotRegion(350, 240, 360, 360, 270);

    // Set the elevation and rotation angles to 15 and 30 degrees
    c->setViewAngle(15, 30);

    // Add a legend box at (640, 180)
    c->addLegend(640, 180);

    // Add 3 scatter groups to the chart with 9 pixels glass sphere symbols of red (ff0000), green
    // (00ff00) and blue (0000ff) colors
    c->addScatterGroup(xData0, yData0, zData0, "Alpha", Chart::GlassSphere2Shape, 9, 0xff0000);
    c->addScatterGroup(xData1, yData1, zData1, "Beta", Chart::GlassSphere2Shape, 9, 0x00ff00);
    c->addScatterGroup(xData2, yData2, zData2, "Gamma", Chart::GlassSphere2Shape, 9, 0x0000ff);

    // Set the x, y and z axis titles
    c->xAxis()->setTitle("X-Axis Place Holder");
    c->yAxis()->setTitle("Y-Axis Place Holder");
    c->zAxis()->setTitle("Z-Axis Place Holder");

    // Output the chart
    c->makeChart();

    //free up resources
    delete r0;
    delete r1;
    delete r2;

    return c;
}


BaseChart *threedscatteraxis(int /* chartIndex */, const char ** /* imageMap */)
{

    // The x coordinates for the 2 scatter groups
    double dataX[] = {Chart::chartTime(2011, 9, 1), Chart::chartTime(2011, 9, 2), Chart::chartTime(
        2011, 9, 3), Chart::chartTime(2011, 9, 4), Chart::chartTime(2011, 9, 5), Chart::chartTime(
        2011, 9, 6), Chart::chartTime(2011, 9, 7), Chart::chartTime(2011, 9, 8), Chart::chartTime(
        2011, 9, 9), Chart::chartTime(2011, 9, 10), Chart::chartTime(2011, 9, 11)};

    // The y and z coordinates for the first scatter group
    double dataY0[] = {0.4, 0.2, 0.5, 0.4, 0.7, 1.3, 1.1, 1.0, 0.6, 0.4, 0.5};
    double dataZ0[] = {43, 38, 33, 23.4, 28, 36, 34, 47, 53, 45, 40};

    // The y and z coordinates for the second scatter group
    double dataY1[] = {1.4, 1.0, 1.8, 1.9, 1.5, 1.0, 0.6, 0.7, 1.2, 1.7, 1.5};
    double dataZ1[] = {46, 41, 33, 37, 28, 29, 34, 37, 41, 52, 50};

    // Instead of displaying numeric values, labels are used for the y-axis
    const char *labelsY[] = {"Low", "Medium", "High"};

    // Create a ThreeDScatterChart object of size 760 x 520 pixels
    ThreeDScatterChart *c = new ThreeDScatterChart(760, 520);

    // Add a title to the chart using 18 points Arial font
    c->addTitle("3D Scatter Chart Axis Types", "arial.ttf", 18);

    // Set the center of the plot region at (385, 270), and set width x depth x height to 480 x 240
    // x 240 pixels
    c->setPlotRegion(385, 270, 480, 240, 240);

    // Set the elevation and rotation angles to 30 and -10 degrees
    c->setViewAngle(30, -10);

    // Add a legend box at (380, 40) with horizontal layout. Use 9pt Arial Bold font.
    LegendBox *b = c->addLegend(380, 40, false, "arialbd.ttf", 9);
    b->setAlignment(Chart::TopCenter);
    b->setRoundedCorners();

    // Add a scatter group to the chart using 13 pixels red (ff0000) glass sphere symbols, with
    // dotted drop lines
    ThreeDScatterGroup *g0 = c->addScatterGroup(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(
        dataX[0]))), DoubleArray(dataY0, (int)(sizeof(dataY0) / sizeof(dataY0[0]))), DoubleArray(
        dataZ0, (int)(sizeof(dataZ0) / sizeof(dataZ0[0]))), "Alpha Series",
        Chart::GlassSphere2Shape, 13, 0xff0000);
    g0->setDropLine(c->dashLineColor(Chart::SameAsMainColor, Chart::DotLine));

    // Add a scatter group to the chart using 13 pixels blue (00cc00) cross symbols, with dotted
    // drop lines
    ThreeDScatterGroup *g1 = c->addScatterGroup(DoubleArray(dataX, (int)(sizeof(dataX) / sizeof(
        dataX[0]))), DoubleArray(dataY1, (int)(sizeof(dataY1) / sizeof(dataY1[0]))), DoubleArray(
        dataZ1, (int)(sizeof(dataZ1) / sizeof(dataZ1[0]))), "Beta Series", Chart::Cross2Shape(), 13,
        0x00cc00);
    g1->setDropLine(c->dashLineColor(Chart::SameAsMainColor, Chart::DotLine));

    // Set x-axis tick density to 50 pixels. ChartDirector auto-scaling will use this as the
    // guideline when putting ticks on the x-axis.
    c->xAxis()->setTickDensity(50);

    // Set the y-axis labels
    c->yAxis()->setLabels(StringArray(labelsY, (int)(sizeof(labelsY) / sizeof(labelsY[0]))));

    // Set label style to Arial bold for all axes
    c->xAxis()->setLabelStyle("arialbd.ttf");
    c->yAxis()->setLabelStyle("arialbd.ttf");
    c->zAxis()->setLabelStyle("arialbd.ttf");

    // Set the x, y and z axis titles using deep blue (000088) 15 points Arial font
    c->xAxis()->setTitle("Date/Time Axis", "ariali.ttf", 15, 0x000088);
    c->yAxis()->setTitle("Label\nBased\nAxis", "ariali.ttf", 15, 0x000088);
    c->zAxis()->setTitle("Numeric Axis", "ariali.ttf", 15, 0x000088);

    // Output the chart
    c->makeChart();

    return c;
}


BaseChart *simpleradar(int /* chartIndex */, const char **imageMap)
{
    // The data for the chart
    double data[] = {90, 60, 65, 75, 40};

    // The labels for the chart
    const char *labels[] = {"Speed", "Reliability", "Comfort", "Safety", "Efficiency"};

    // Create a PolarChart object of size 450 x 350 pixels
    PolarChart *c = new PolarChart(450, 350);

    // Set center of plot area at (225, 185) with radius 150 pixels
    c->setPlotArea(225, 185, 150);

    // Add an area layer to the polar chart
    c->addAreaLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), 0x9999ff);

    // Set the labels to the angular axis as spokes
    c->angularAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: score = {value}'");

    return c;
}


BaseChart *multiradar(int /* chartIndex */, const char **imageMap)
{
    // The data for the chart
    double data0[] = {90, 60, 85, 75, 55};
    double data1[] = {60, 80, 70, 80, 85};

    // The labels for the chart
    const char *labels[] = {"Speed", "Reliability", "Comfort", "Safety", "Efficiency"};

    // Create a PolarChart object of size 480 x 380 pixels. Set background color to gold, with 1
    // pixel 3D border effect
    PolarChart *c = new PolarChart(480, 380, Chart::goldColor(), 0x000000, 1);

    // Add a title to the chart using 15pt Times Bold Italic font. The title text is white (ffffff)
    // on a deep blue (000080) background
    c->addTitle("Space Travel Vehicles Compared", "timesbi.ttf", 15, 0xffffff)->setBackground(
        0x000080);

    // Set plot area center at (240, 210), with 150 pixels radius, and a white (ffffff) background.
    c->setPlotArea(240, 210, 150, 0xffffff);

    // Add a legend box at top right corner (470, 35) using 10pt Arial Bold font. Set the background
    // to silver, with 1 pixel 3D border effect.
    LegendBox *b = c->addLegend(470, 35, true, "arialbd.ttf", 10);
    b->setAlignment(Chart::TopRight);
    b->setBackground(Chart::silverColor(), Chart::Transparent, 1);

    // Add an area layer to the chart using semi-transparent blue (0x806666cc). Add a blue
    // (0x6666cc) line layer using the same data with 3 pixel line width to highlight the border of
    // the area.
    c->addAreaLayer(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0x806666cc,
        "Model Saturn");
    c->addLineLayer(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0x6666cc
        )->setLineWidth(3);

    // Add an area layer to the chart using semi-transparent red (0x80cc6666). Add a red (0xcc6666)
    // line layer using the same data with 3 pixel line width to highlight the border of the area.
    c->addAreaLayer(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0x80cc6666,
        "Model Jupiter");
    c->addLineLayer(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0xcc6666
        )->setLineWidth(3);

    // Set the labels to the angular axis as spokes.
    c->angularAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='[{dataSetName}] {label}: score = {value}'");

    return c;
}


BaseChart *stackradar(int /* chartIndex */, const char **imageMap)
{
    // The data for the chart
    double data0[] = {100, 100, 100, 100, 100};
    double data1[] = {90, 85, 85, 80, 70};
    double data2[] = {80, 65, 65, 75, 45};

    // The labels for the chart
    const char *labels[] = {"Population<*br*><*font=arial.ttf*>6 millions",
        "GDP<*br*><*font=arial.ttf*>120 billions", "Export<*br*><*font=arial.ttf*>25 billions",
        "Import<*br*><*font=arial.ttf*>24 billions",
        "Investments<*br*><*font=arial.ttf*>20 billions"};

    // Create a PolarChart object of size 480 x 460 pixels. Set background color to silver, with 1
    // pixel 3D border effect
    PolarChart *c = new PolarChart(480, 460, Chart::silverColor(), 0x000000, 1);

    // Add a title to the chart using 15pt Times Bold Italic font. The title text is white (ffffff)
    // on a deep green (008000) background
    c->addTitle("Economic Growth", "timesbi.ttf", 15, 0xffffff)->setBackground(0x008000);

    // Set plot area center at (240, 270), with 150 pixels radius
    c->setPlotArea(240, 270, 150);

    // Use 1 pixel width semi-transparent black (c0000000) lines as grid lines
    c->setGridColor(0xc0000000, 1, 0xc0000000, 1);

    // Add a legend box at top-center of plot area (240, 35) using horizontal layout. Use 10pt Arial
    // Bold font, with silver background and 1 pixel 3D border effect.
    LegendBox *b = c->addLegend(240, 35, false, "arialbd.ttf", 10);
    b->setAlignment(Chart::TopCenter);
    b->setBackground(Chart::silverColor(), Chart::Transparent, 1);

    // Add area layers of different colors to represent the data
    c->addAreaLayer(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0xcc8880,
        "Year 2004");
    c->addAreaLayer(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), 0xffd080,
        "Year 1994");
    c->addAreaLayer(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), 0xa0bce0,
        "Year 1984");

    // Set the labels to the angular axis as spokes.
    c->angularAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Set radial axis from 0 - 100 with a tick every 20 units
    c->radialAxis()->setLinearScale(0, 100, 20);

    // Just show the radial axis as a grid line. Hide the axis labels by setting the label color to
    // Transparent
    c->radialAxis()->setColors(0xc0000000, Chart::Transparent);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='Current {label}: {value}% in {dataSetName}'");

    return c;
}


BaseChart *polarline(int /* chartIndex */, const char **imageMap)
{
    // The data for the chart
    double data0[] = {90, 25, 40, 55, 68, 44, 79, 85, 50};
    double angles0[] = {15, 60, 110, 180, 230, 260, 260, 310, 340};

    double data1[] = {80, 91, 66, 80, 92, 87};
    double angles1[] = {40, 65, 88, 110, 150, 200};

    // Create a PolarChart object of size 460 x 500 pixels, with a grey (e0e0e0) background and 1
    // pixel 3D border
    PolarChart *c = new PolarChart(460, 500, 0xe0e0e0, 0x000000, 1);

    // Add a title to the chart at the top left corner using 15pt Arial Bold Italic font. Use a wood
    // pattern as the title background.
    c->addTitle("Polar Line Chart Demo", "arialbi.ttf", 15)->setBackground(c->patternColor(
        "wood.png"));

    // Set center of plot area at (230, 280) with radius 180 pixels, and white (ffffff) background.
    c->setPlotArea(230, 280, 180, 0xffffff);

    // Set the grid style to circular grid, with grids below the chart layers
    c->setGridStyle(false, false);

    // Add a legend box at top-center of plot area (230, 35) using horizontal layout. Use 10pt Arial
    // Bold font, with 1 pixel 3D border effect.
    LegendBox *b = c->addLegend(230, 35, false, "arialbd.ttf", 9);
    b->setAlignment(Chart::TopCenter);
    b->setBackground(Chart::Transparent, Chart::Transparent, 1);

    // Set angular axis as 0 - 360, with a spoke every 30 units
    c->angularAxis()->setLinearScale(0, 360, 30);

    // Add a blue (0xff) line layer to the chart using (data0, angle0)
    PolarLineLayer *layer0 = c->addLineLayer(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0
        ]))), 0x0000ff, "Close Loop Line");
    layer0->setAngles(DoubleArray(angles0, (int)(sizeof(angles0) / sizeof(angles0[0]))));

    // Set the line width to 2 pixels
    layer0->setLineWidth(2);

    // Use 11 pixel triangle symbols for the data points
    layer0->setDataSymbol(Chart::TriangleSymbol, 11);

    // Enable data label and set its format
    layer0->setDataLabelFormat("({value},{angle})");

    // Set the data label text box with light blue (0x9999ff) backgruond color and 1 pixel 3D border
    // effect
    layer0->setDataLabelStyle()->setBackground(0x9999ff, Chart::Transparent, 1);

    // Add a red (0xff0000) line layer to the chart using (data1, angle1)
    PolarLineLayer *layer1 = c->addLineLayer(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0
        ]))), 0xff0000, "Open Loop Line");
    layer1->setAngles(DoubleArray(angles1, (int)(sizeof(angles1) / sizeof(angles1[0]))));

    // Set the line width to 2 pixels
    layer1->setLineWidth(2);

    // Use 11 pixel diamond symbols for the data points
    layer1->setDataSymbol(Chart::DiamondSymbol, 11);

    // Set the line to open loop
    layer1->setCloseLoop(false);

    // Enable data label and set its format
    layer1->setDataLabelFormat("({value},{angle})");

    // Set the data label text box with light red (0xff9999) backgruond color and 1 pixel 3D border
    // effect
    layer1->setDataLabelStyle()->setBackground(0xff9999, Chart::Transparent, 1);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='[{dataSetName}] ({radius}, {angle})'");

    return c;
}


BaseChart *polararea(int /* chartIndex */, const char **imageMap)
{
    // Data for the chart
    double data0[] = {5, 3, 10, 4, 3, 5, 2, 5};
    double data1[] = {12, 6, 17, 6, 7, 9, 4, 7};
    double data2[] = {17, 7, 22, 7, 18, 13, 5, 11};

    const char *labels[] = {"North", "North<*br*>East", "East", "South<*br*>East", "South",
        "South<*br*>West", "West", "North<*br*>West"};

    // Create a PolarChart object of size 460 x 500 pixels, with a grey (e0e0e0) background and 1
    // pixel 3D border
    PolarChart *c = new PolarChart(460, 500, 0xe0e0e0, 0x000000, 1);

    // Add a title to the chart at the top left corner using 15pt Arial Bold Italic font. Use a wood
    // pattern as the title background.
    c->addTitle("Polar Area Chart Demo", "arialbi.ttf", 15)->setBackground(c->patternColor(
        "wood.png"));

    // Set center of plot area at (230, 280) with radius 180 pixels, and white (ffffff) background.
    c->setPlotArea(230, 280, 180, 0xffffff);

    // Set the grid style to circular grid
    c->setGridStyle(false);

    // Add a legend box at top-center of plot area (230, 35) using horizontal layout. Use 10pt Arial
    // Bold font, with 1 pixel 3D border effect.
    LegendBox *b = c->addLegend(230, 35, false, "arialbd.ttf", 9);
    b->setAlignment(Chart::TopCenter);
    b->setBackground(Chart::Transparent, Chart::Transparent, 1);

    // Set angular axis using the given labels
    c->angularAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Specify the label format for the radial axis
    c->radialAxis()->setLabelFormat("{value}%");

    // Set radial axis label background to semi-transparent grey (40cccccc)
    c->radialAxis()->setLabelStyle()->setBackground(0x40cccccc, 0);

    // Add the data as area layers
    c->addAreaLayer(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), -1,
        "5 m/s or above");
    c->addAreaLayer(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))), -1, "1 - 5 m/s");
    c->addAreaLayer(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), -1,
        "less than 1 m/s");

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='[{label}] {dataSetName}: {value}%'");

    return c;
}


BaseChart *polarspline(int /* chartIndex */, const char **imageMap)
{
    // The data for the chart
    double data0[] = {5.1, 2.6, 1.5, 2.2, 5.1, 4.3, 4.0, 9.0, 1.7, 8.8, 9.9, 9.5, 9.4, 1.8, 2.1,
        2.3, 3.5, 7.7, 8.8, 6.1, 5.0, 3.1, 6.0, 4.3};
    double angles0[] = {0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240,
        255, 270, 285, 300, 315, 330, 345};

    double data1[] = {8.1, 2.5, 5, 5.2, 6.5, 8.5, 9, 7.6, 8.7, 6.4, 5.5, 5.4, 3.0, 8.7, 7.1, 8.8,
        7.9, 2.2, 5.0, 4.0, 1.5, 7.5, 8.3, 9.0};
    double angles1[] = {0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240,
        255, 270, 285, 300, 315, 330, 345};

    // Create a PolarChart object of size 460 x 460 pixels
    PolarChart *c = new PolarChart(460, 460);

    // Add a title to the chart at the top left corner using 15pt Arial Bold Italic font
    c->addTitle(Chart::TopLeft, "<*underline=2*>EM Field Strength", "arialbi.ttf", 15);

    // Set center of plot area at (230, 240) with radius 180 pixels
    c->setPlotArea(230, 240, 180);

    // Set the grid style to circular grid
    c->setGridStyle(false);

    // Add a legend box at the top right corner of the chart using 9pt Arial Bold font
    c->addLegend(459, 0, true, "arialbd.ttf", 9)->setAlignment(Chart::TopRight);

    // Set angular axis as 0 - 360, with a spoke every 30 units
    c->angularAxis()->setLinearScale(0, 360, 30);

    // Add a red (0xff9999) spline area layer to the chart using (data0, angles0)
    c->addSplineAreaLayer(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))), 0xff9999,
        "Above 100MHz")->setAngles(DoubleArray(angles0, (int)(sizeof(angles0) / sizeof(angles0[0])))
        );

    // Add a blue (0xff) spline line layer to the chart using (data1, angle1)
    PolarSplineLineLayer *layer1 = c->addSplineLineLayer(DoubleArray(data1, (int)(sizeof(data1) /
        sizeof(data1[0]))), 0x0000ff, "Below 100MHz");
    layer1->setAngles(DoubleArray(angles1, (int)(sizeof(angles1) / sizeof(angles1[0]))));

    // Set the line width to 3 pixels
    layer1->setLineWidth(3);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName} EM field at {angle} deg: {value} Watt'");

    return c;
}


BaseChart *polarscatter(int /* chartIndex */, const char **imageMap)
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
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName} at ({value} km, {angle} deg)'");

    return c;
}


BaseChart *polarbubble(int /* chartIndex */, const char **imageMap)
{
    // The data for the chart
    double data0[] = {6, 12.5, 18.2, 15};
    double angles0[] = {45, 96, 169, 258};
    double size0[] = {41, 105, 12, 20};

    double data1[] = {18, 16, 11, 14};
    double angles1[] = {30, 210, 240, 310};
    double size1[] = {30, 45, 12, 90};

    // Create a PolarChart object of size 460 x 460 pixels
    PolarChart *c = new PolarChart(460, 460);

    // Add a title to the chart at the top left corner using 15pt Arial Bold Italic font
    c->addTitle(Chart::TopLeft, "<*underline=2*>EM Field Strength", "arialbi.ttf", 15);

    // Set center of plot area at (230, 240) with radius 180 pixels
    c->setPlotArea(230, 240, 180);

    // Use alternative light grey/dark grey circular background color
    c->setPlotAreaBg(0xdddddd, 0xeeeeee);

    // Set the grid style to circular grid
    c->setGridStyle(false);

    // Add a legend box at the top right corner of the chart using 9pt Arial Bold font
    c->addLegend(459, 0, true, "arialbd.ttf", 9)->setAlignment(Chart::TopRight);

    // Set angular axis as 0 - 360, with a spoke every 30 units
    c->angularAxis()->setLinearScale(0, 360, 30);

    // Set the radial axis label format
    c->radialAxis()->setLabelFormat("{value} km");

    // Add a blue (0x9999ff) line layer to the chart using (data0, angle0)
    PolarLineLayer *layer0 = c->addLineLayer(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0
        ]))), 0x9999ff, "Cold Spot");
    layer0->setAngles(DoubleArray(angles0, (int)(sizeof(angles0) / sizeof(angles0[0]))));

    // Disable the line by setting its width to 0, so only the symbols are visible
    layer0->setLineWidth(0);

    // Use a circular data point symbol
    layer0->setDataSymbol(Chart::CircleSymbol, 11);

    // Modulate the symbol size by size0 to produce a bubble chart effect
    layer0->setSymbolScale(DoubleArray(size0, (int)(sizeof(size0) / sizeof(size0[0]))));

    // Add a red (0xff9999) line layer to the chart using (data1, angle1)
    PolarLineLayer *layer1 = c->addLineLayer(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0
        ]))), 0xff9999, "Hot Spot");
    layer1->setAngles(DoubleArray(angles1, (int)(sizeof(angles1) / sizeof(angles1[0]))));

    // Disable the line by setting its width to 0, so only the symbols are visible
    layer1->setLineWidth(0);

    // Use a circular data point symbol
    layer1->setDataSymbol(Chart::CircleSymbol, 11);

    // Modulate the symbol size by size1 to produce a bubble chart effect
    layer1->setSymbolScale(DoubleArray(size1, (int)(sizeof(size1) / sizeof(size1[0]))));

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='{dataSetName} at ({value} km, {angle} deg)\nStrength = {z} Watt'");

    return c;
}


BaseChart *polarvector(int /* chartIndex */, const char **imageMap)
{
    // Coordinates of the starting points of the vectors
    double radius[] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 20, 20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25};
    double angle[] = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 0, 30, 60, 90, 120,
        150, 180, 210, 240, 270, 300, 330, 0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 0,
        30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 0, 30, 60, 90, 120, 150, 180, 210, 240,
        270, 300, 330};

    // Magnitude and direction of the vectors
    double magnitude[] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1};
    double direction[] = {60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 0, 30, 60, 90, 120, 150,
        180, 210, 240, 270, 300, 330, 0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 0, 30,
        60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 0, 30, 60, 90, 120, 150, 180, 210, 240, 270,
        300, 330, 0, 30};

    // Create a PolarChart object of size 460 x 460 pixels
    PolarChart *c = new PolarChart(460, 460);

    // Add a title to the chart at the top left corner using 15pt Arial Bold Italic font
    c->addTitle("Polar Vector Chart Demonstration", "arialbi.ttf", 15);

    // Set center of plot area at (230, 240) with radius 180 pixels
    c->setPlotArea(230, 240, 180);

    // Set the grid style to circular grid
    c->setGridStyle(false);

    // Set angular axis as 0 - 360, with a spoke every 30 units
    c->angularAxis()->setLinearScale(0, 360, 30);

    // Add a polar vector layer to the chart with blue (0000ff) vectors
    c->addVectorLayer(DoubleArray(radius, (int)(sizeof(radius) / sizeof(radius[0]))), DoubleArray(
        angle, (int)(sizeof(angle) / sizeof(angle[0]))), DoubleArray(magnitude, (int)(sizeof(
        magnitude) / sizeof(magnitude[0]))), DoubleArray(direction, (int)(sizeof(direction) /
        sizeof(direction[0]))), Chart::RadialAxisScale, 0x0000ff);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='Vector at ({value}, {angle} deg): Length = {len}, Angle = {dir} deg'");

    return c;
}


BaseChart *rose(int /* chartIndex */, const char ** /* imageMap */)
{
    // Data for the chart
    double data[] = {9.4, 1.8, 2.1, 2.3, 3.5, 7.7, 8.8, 6.1, 5.0, 3.1, 6.0, 4.3, 5.1, 2.6, 1.5, 2.2,
        5.1, 4.3, 4.0, 9.0, 1.7, 8.8, 9.9, 9.5};
    double angles[] = {0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240,
        255, 270, 285, 300, 315, 330, 345};

    // Create a PolarChart object of size 460 x 460 pixels, with a silver background and a 1 pixel
    // 3D border
    PolarChart *c = new PolarChart(460, 460, Chart::silverColor(), 0x000000, 1);

    // Add a title to the chart at the top left corner using 15pt Arial Bold Italic font. Use white
    // text on deep blue background.
    c->addTitle("Polar Vector Chart Demonstration", "arialbi.ttf", 15, 0xffffff)->setBackground(
        0x000080);

    // Set plot area center at (230, 240) with radius 180 pixels and white background
    c->setPlotArea(230, 240, 180, 0xffffff);

    // Set the grid style to circular grid
    c->setGridStyle(false);

    // Set angular axis as 0 - 360, with a spoke every 30 units
    c->angularAxis()->setLinearScale(0, 360, 30);

    // Add sectors to the chart as sector zones
    for(int i = 0; i < (int)(sizeof(data) / sizeof(data[0])); ++i) {
        c->angularAxis()->addZone(angles[i], angles[i] + 15, 0, data[i], 0x33ff33, 0x008000);
    }

    // Add an Transparent invisible layer to ensure the axis is auto-scaled using the data
    c->addLineLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), Chart::Transparent);

    // Output the chart
    c->makeChart();

    return c;
}


BaseChart *stackrose(int /* chartIndex */, const char ** /* imageMap */)
{
    // Data for the chart
    double data0[] = {5, 3, 10, 4, 3, 5, 2, 5};
    double data1[] = {12, 6, 17, 6, 7, 9, 4, 7};
    double data2[] = {17, 7, 22, 7, 18, 13, 5, 11};

    double angles[] = {0, 45, 90, 135, 180, 225, 270, 315};
    const char *labels[] = {"North", "North\nEast", "East", "South\nEast", "South", "South\nWest",
        "West", "North\nWest"};

    // Create a PolarChart object of size 460 x 500 pixels, with a grey (e0e0e0) background and a 1
    // pixel 3D border
    PolarChart *c = new PolarChart(460, 500, 0xe0e0e0, 0x000000, 1);

    // Add a title to the chart at the top left corner using 15pt Arial Bold Italic font. Use white
    // text on deep blue background.
    c->addTitle("Wind Direction", "arialbi.ttf", 15, 0xffffff)->setBackground(0x000080);

    LegendBox *legendBox = c->addLegend(230, 35, false, "arialbd.ttf", 9);
    legendBox->setAlignment(Chart::TopCenter);
    legendBox->setBackground(Chart::Transparent, Chart::Transparent, 1);

    legendBox->addKey("5 m/s or above", 0xff3333);
    legendBox->addKey("1 - 5 m/s", 0x33ff33);
    legendBox->addKey("less than 1 m/s", 0x3333ff);

    // Set plot area center at (230, 280) with radius 180 pixels and white background
    c->setPlotArea(230, 280, 180, 0xffffff);

    // Set the grid style to circular grid
    c->setGridStyle(false);

    // Set angular axis as 0 - 360, with a spoke every 30 units
    c->angularAxis()->setLinearScale(0, 360, StringArray(labels, (int)(sizeof(labels) / sizeof(
        labels[0]))));

    for(int i = 0; i < (int)(sizeof(angles) / sizeof(angles[0])); ++i) {
        c->angularAxis()->addZone(angles[i] - 10, angles[i] + 10, 0, data0[i], 0x3333ff, 0);
        c->angularAxis()->addZone(angles[i] - 10, angles[i] + 10, data0[i], data1[i], 0x33ff33, 0);
        c->angularAxis()->addZone(angles[i] - 10, angles[i] + 10, data1[i], data2[i], 0xff3333, 0);
    }

    // Add an Transparent invisible layer to ensure the axis is auto-scaled using the data
    c->addLineLayer(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))), Chart::Transparent)
        ;

    // Output the chart
    c->makeChart();

    return c;
}


BaseChart *polarzones(int /* chartIndex */, const char **imageMap)
{
    // The data for the chart
    double data[] = {51, 15, 51, 40, 17, 87, 94, 21, 35, 88, 50, 60};

    // The labels for the chart
    const char *labels[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct",
        "Nov", "Dec"};

    // Create a PolarChart object of size 400 x 420 pixels
    PolarChart *c = new PolarChart(400, 420);

    // Set background color to a 2 pixel pattern color, with a black border and 1 pixel 3D border
    // effect
    int pattern1[] = {0xffffff, 0xe0e0e0};
    c->setBackground(c->patternColor(IntArray(pattern1, (int)(sizeof(pattern1) / sizeof(pattern1[0])
        )), 2), 0, 1);

    // Add a title to the chart using 16pt Arial Bold Italic font. The title text is white
    // (0xffffff) on 2 pixel pattern background
    int pattern2[] = {0x000000, 0x000080};
    c->addTitle("Chemical Concentration", "arialbi.ttf", 16, 0xffffff)->setBackground(
        c->patternColor(IntArray(pattern2, (int)(sizeof(pattern2) / sizeof(pattern2[0]))), 2));

    // Set center of plot area at (200, 240) with radius 145 pixels. Set background color to blue
    // (9999ff)
    c->setPlotArea(200, 240, 145, 0x9999ff);

    // Color the region between radius = 40 to 80 as green (99ff99)
    c->radialAxis()->addZone(40, 80, 0x99ff99);

    // Color the region with radius > 80 as red (ff9999)
    c->radialAxis()->addZone(80, 999, 0xff9999);

    // Set the grid style to circular grid
    c->setGridStyle(false);

    // Set the radial axis label format
    c->radialAxis()->setLabelFormat("{value} ppm");

    // Use semi-transparent (40ffffff) label background so as not to block the data
    c->radialAxis()->setLabelStyle()->setBackground(0x40ffffff, 0x40000000);

    // Add a legend box at (200, 30) top center aligned, using 9pt Arial Bold font. with a black
    // border.
    LegendBox *legendBox = c->addLegend(200, 30, false, "arialbd.ttf", 9);
    legendBox->setAlignment(Chart::TopCenter);

    // Add legend keys to represent the red/green/blue zones
    legendBox->addKey("Under-Absorp", 0x9999ff);
    legendBox->addKey("Normal", 0x99ff99);
    legendBox->addKey("Over-Absorp", 0xff9999);

    // Add a blue (000080) spline line layer with line width set to 3 pixels and using yellow
    // (ffff00) circles to represent the data points
    PolarSplineLineLayer *layer = c->addSplineLineLayer(DoubleArray(data, (int)(sizeof(data) /
        sizeof(data[0]))), 0x000080);
    layer->setLineWidth(3);
    layer->setDataSymbol(Chart::CircleShape, 11, 0xffff00);

    // Set the labels to the angular axis as spokes.
    c->angularAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Output the chart
    c->makeChart();

    // Include tool tip for the chart.
    *imageMap = layer->getHTMLImageMap("clickable", "",
        "title='Concentration on {label}: {value} ppm'");

    return c;
}


BaseChart *polarzones2(int /* chartIndex */, const char **imageMap)
{
    // The data for the chart
    double data[] = {5.1, 1.5, 5.1, 4.0, 1.7, 8.7, 9.4, 2.1, 3.5, 8.8, 5.0, 6.0};

    // The labels for the chart
    const char *labels[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct",
        "Nov", "Dec"};

    // Create a PolarChart object of size 400 x 420 pixels. with a metallic blue (9999ff) background
    // color and 1 pixel 3D border
    PolarChart *c = new PolarChart(400, 420, Chart::metalColor(0x9999ff), 0x000000, 1);

    // Add a title to the chart using 16pt Arial Bold Italic font. The title text is white
    // (0xffffff) on deep blue (000080) background
    c->addTitle("Chemical Concentration", "arialbi.ttf", 16, 0xffffff)->setBackground(0x000080);

    // Set center of plot area at (200, 240) with radius 145 pixels. Set background color to green
    // (0x33ff33)
    c->setPlotArea(200, 240, 145, 0x33ff33);

    // Set the labels to the angular axis
    c->angularAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

    // Color the sector between label index = 5.5 to 7.5 as red (ff3333) zone
    c->angularAxis()->addZone(5.5, 7.5, 0xff3333);

    // Color the sector between label index = 4.5 to 5.5, and also between 7.5 to 8.5, as yellow
    // (ff3333) zones
    c->angularAxis()->addZone(4.5, 5.5, 0xffff00);
    c->angularAxis()->addZone(7.5, 8.5, 0xffff00);

    // Set the grid style to circular grid
    c->setGridStyle(false);

    // Use semi-transparent (40ffffff) label background so as not to block the data
    c->radialAxis()->setLabelStyle()->setBackground(0x40ffffff, 0x40000000);

    // Add a legend box at (200, 30) top center aligned, using 9pt Arial Bold font. with a black
    // border.
    LegendBox *legendBox = c->addLegend(200, 30, false, "arialbd.ttf", 9);
    legendBox->setAlignment(Chart::TopCenter);

    // Add legend keys to represent the red/yellow/green zones
    legendBox->addKey("Very Dry", 0xff3333);
    legendBox->addKey("Critical", 0xffff00);
    legendBox->addKey("Moderate", 0x33ff33);

    // Add a blue (0x80) line layer with line width set to 3 pixels and use purple (ff00ff) cross
    // symbols for the data points
    PolarLineLayer *layer = c->addLineLayer(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))
        ), 0x000080);
    layer->setLineWidth(3);
    layer->setDataSymbol(Chart::Cross2Shape(), 15, 0xff00ff);

    // Output the chart
    c->makeChart();

    // Include tool tip for the chart.
    *imageMap = layer->getHTMLImageMap("clickable", "",
        "title='Concentration on {label}: {value} ppm'");

    return c;
}


BaseChart *simplepyramid(int /* chartIndex */, const char **imageMap)
{
    // The data for the pyramid chart
    double data[] = {156, 123, 211, 179};

    // The labels for the pyramid chart
    const char *labels[] = {"Funds", "Bonds", "Stocks", "Cash"};

    // Create a PyramidChart object of size 360 x 360 pixels
    PyramidChart *c = new PyramidChart(360, 360);

    // Set the pyramid center at (180, 180), and width x height to 150 x 180 pixels
    c->setPyramidSize(180, 180, 150, 300);

    // Set the pyramid data and labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Add labels at the center of the pyramid layers using Arial Bold font. The labels will have
    // two lines showing the layer name and percentage.
    c->setCenterLabel("{label}\n{percent}%", "arialbd.ttf");

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US$ {value}M ({percent}%)'");

    return c;
}


BaseChart *threedpyramid(int /* chartIndex */, const char **imageMap)
{
    // The data for the pyramid chart
    double data[] = {156, 123, 211, 179};

    // The labels for the pyramid chart
    const char *labels[] = {"Corporate Tax", "Working Capital", "Re-investment", "Dividend"};

    // The colors for the pyramid layers
    int colors[] = {0x66aaee, 0xeebb22, 0xcccccc, 0xcc88ff};

    // Create a PyramidChart object of size 500 x 400 pixels
    PyramidChart *c = new PyramidChart(500, 400);

    // Set the pyramid center at (200, 180), and width x height to 150 x 300 pixels
    c->setPyramidSize(200, 180, 150, 300);

    // Set the elevation to 15 degrees
    c->setViewAngle(15);

    // Set the pyramid data and labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Set the layer colors to the given colors
    c->setColors(Chart::DataColor, IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));

    // Leave 1% gaps between layers
    c->setLayerGap(0.01);

    // Add labels at the center of the pyramid layers using Arial Bold font. The labels will show
    // the percentage of the layers.
    c->setCenterLabel("{percent}%", "arialbd.ttf");

    // Add labels at the right side of the pyramid layers using Arial Bold font. The labels will
    // have two lines showing the layer name and value.
    c->setRightLabel("{label}\nUS$ {value}M", "arialbd.ttf");

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US$ {value}M ({percent}%)'");

    return c;
}


BaseChart *rotatedpyramid(int /* chartIndex */, const char **imageMap)
{
    // The data for the pyramid chart
    double data[] = {156, 123, 211, 179};

    // The labels for the pyramid chart
    const char *labels[] = {"Funds", "Bonds", "Stocks", "Cash"};

    // The semi-transparent colors for the pyramid layers
    int colors[] = {0x400000cc, 0x4066aaee, 0x40ffbb00, 0x40ee6622};

    // Create a PyramidChart object of size 450 x 400 pixels
    PyramidChart *c = new PyramidChart(450, 400);

    // Set the pyramid center at (220, 180), and width x height to 150 x 300 pixels
    c->setPyramidSize(220, 180, 150, 300);

    // Set the elevation to 15 degrees and rotation to 75 degrees
    c->setViewAngle(15, 75);

    // Set the pyramid data and labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Set the layer colors to the given colors
    c->setColors(Chart::DataColor, IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));

    // Leave 1% gaps between layers
    c->setLayerGap(0.01);

    // Add a legend box at (320, 60), with light grey (eeeeee) background and grey (888888) border.
    // Set the top-left and bottom-right corners to rounded corners of 10 pixels radius.
    LegendBox *legendBox = c->addLegend(320, 60);
    legendBox->setBackground(0xeeeeee, 0x888888);
    legendBox->setRoundedCorners(10, 0, 10, 0);

    // Add labels at the center of the pyramid layers using Arial Bold font. The labels will show
    // the percentage of the layers.
    c->setCenterLabel("{percent}%", "arialbd.ttf");

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US$ {value}M ({percent}%)'");

    return c;
}


BaseChart *cone(int /* chartIndex */, const char **imageMap)
{
    // The data for the pyramid chart
    double data[] = {156, 123, 211, 179};

    // The labels for the pyramid chart
    const char *labels[] = {"Funds", "Bonds", "Stocks", "Cash"};

    // The semi-transparent colors for the pyramid layers
    int colors[] = {0x60000088, 0x6066aaee, 0x60ffbb00, 0x60ee6622};

    // Create a PyramidChart object of size 480 x 400 pixels
    PyramidChart *c = new PyramidChart(480, 400);

    // Set the cone center at (280, 180), and width x height to 150 x 300 pixels
    c->setConeSize(280, 180, 150, 300);

    // Set the elevation to 15 degrees
    c->setViewAngle(15);

    // Set the pyramid data and labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Set the layer colors to the given colors
    c->setColors(Chart::DataColor, IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));

    // Leave 1% gaps between layers
    c->setLayerGap(0.01);

    // Add labels at the left side of the pyramid layers using Arial Bold font. The labels will have
    // 3 lines showing the layer name, value and percentage.
    c->setLeftLabel("{label}\nUS ${value}K\n({percent}%)", "arialbd.ttf");

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US$ {value}K ({percent}%)'");

    return c;
}


BaseChart *funnel(int /* chartIndex */, const char **imageMap)
{
    // The data for the pyramid chart
    double data[] = {156, 123, 211, 179};

    // The labels for the pyramid chart
    const char *labels[] = {"Corporate Tax", "Working Capital", "Re-investment", "Dividend"};

    // The colors for the pyramid layers
    int colors[] = {0x66aaee, 0xeebb22, 0xcccccc, 0xcc88ff};

    // Create a PyramidChart object of size 500 x 400 pixels
    PyramidChart *c = new PyramidChart(500, 400);

    // Set the funnel center at (200, 210), and width x height to 150 x 300 pixels
    c->setFunnelSize(200, 210, 150, 300);

    // Set the elevation to 5 degrees
    c->setViewAngle(5);

    // Set the pyramid data and labels
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
        sizeof(labels) / sizeof(labels[0]))));

    // Set the layer colors to the given colors
    c->setColors(Chart::DataColor, IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));

    // Leave 1% gaps between layers
    c->setLayerGap(0.01);

    // Add labels at the right side of the pyramid layers using Arial Bold font. The labels will
    // have 3 lines showing the layer name, value and percentage.
    c->setRightLabel("{label}\nUS ${value}K\n({percent}%)", "arialbd.ttf");

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "", "title='{label}: US$ {value}M ({percent}%)'");

    return c;
}


BaseChart *pyramidelevation(int chartIndex, const char ** /* imageMap */)
{
    char buffer[256];

    // The data for the pyramid chart
    double data[] = {156, 123, 211, 179};

    // The colors for the pyramid layers
    int colors[] = {0x66aaee, 0xeebb22, 0xcccccc, 0xcc88ff};

    // The elevation angle
    int angle = chartIndex * 15;

    // Create a PyramidChart object of size 200 x 200 pixels, with white (ffffff) background and
    // grey (888888) border
    PyramidChart *c = new PyramidChart(200, 200, 0xffffff, 0x888888);

    // Set the pyramid center at (100, 100), and width x height to 60 x 120 pixels
    c->setPyramidSize(100, 100, 60, 120);

    // Set the elevation angle
    sprintf(buffer, "Elevation = %d", angle);
    c->addTitle(buffer, "ariali.ttf", 15);
    c->setViewAngle(angle);

    // Set the pyramid data
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))));

    // Set the layer colors to the given colors
    c->setColors(Chart::DataColor, IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));

    // Leave 1% gaps between layers
    c->setLayerGap(0.01);

    // Output the chart
    c->makeChart();

    return c;
}


BaseChart *pyramidrotation(int chartIndex, const char ** /* imageMap */)
{
    char buffer[256];

    // The data for the pyramid chart
    double data[] = {156, 123, 211, 179};

    // The semi-transparent colors for the pyramid layers
    int colors[] = {0x400000cc, 0x4066aaee, 0x40ffbb00, 0x40ee6622};

    // The rotation angle
    int angle = chartIndex * 15;

    // Create a PyramidChart object of size 200 x 200 pixels, with white (ffffff) background and
    // grey (888888) border
    PyramidChart *c = new PyramidChart(200, 200, 0xffffff, 0x888888);

    // Set the pyramid center at (100, 100), and width x height to 60 x 120 pixels
    c->setPyramidSize(100, 100, 60, 120);

    // Set the elevation to 15 degrees and use the given rotation angle
    sprintf(buffer, "Rotation = %d", angle);
    c->addTitle(buffer, "ariali.ttf", 15);
    c->setViewAngle(15, angle);

    // Set the pyramid data
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))));

    // Set the layer colors to the given colors
    c->setColors(Chart::DataColor, IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));

    // Leave 1% gaps between layers
    c->setLayerGap(0.01);

    // Output the chart
    c->makeChart();

    return c;
}


BaseChart *pyramidgap(int chartIndex, const char ** /* imageMap */)
{
    char buffer[256];

    // The data for the pyramid chart
    double data[] = {156, 123, 211, 179};

    // The colors for the pyramid layers
    int colors[] = {0x66aaee, 0xeebb22, 0xcccccc, 0xcc88ff};

    // The layer gap
    double gap = chartIndex * 0.01;

    // Create a PyramidChart object of size 200 x 200 pixels, with white (ffffff) background and
    // grey (888888) border
    PyramidChart *c = new PyramidChart(200, 200, 0xffffff, 0x888888);

    // Set the pyramid center at (100, 100), and width x height to 60 x 120 pixels
    c->setPyramidSize(100, 100, 60, 120);

    // Set the layer gap
    sprintf(buffer, "Gap = %g", gap);
    c->addTitle(buffer, "ariali.ttf", 15);
    c->setLayerGap(gap);

    // Set the elevation to 15 degrees
    c->setViewAngle(15);

    // Set the pyramid data
    c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))));

    // Set the layer colors to the given colors
    c->setColors(Chart::DataColor, IntArray(colors, (int)(sizeof(colors) / sizeof(colors[0]))));

    // Output the chart
    c->makeChart();

    return c;
}


BaseChart *semicirclemeter(int /* chartIndex */, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 72.55;

    // Create an AngularMeter object of size 300 x 180 pixels with transparent background
    AngularMeter *m = new AngularMeter(300, 180, Chart::Transparent);

    // Center at (150, 150), scale radius = 128 pixels, scale angle -90 to +90 degrees
    m->setMeter(150, 150, 128, -90, 90);

    // Add a pale grey (0xeeeeee) scale background of 148 pixels radius, with a 10 pixel thick light
    // grey (0xcccccc) border
    m->addScaleBackground(148, 0xeeeeee, 10, 0xcccccc);

    // Meter scale is 0 - 100, with major tick every 20 units, minor tick every 10 units, and micro
    // tick every 5 units
    m->setScale(0, 100, 20, 10, 5);

    // Set the scale label style to 15pt Arial Italic. Set the major/minor/micro tick lengths to
    // 16/16/10 pixels pointing inwards, and their widths to 2/1/1 pixels.
    m->setLabelStyle("ariali.ttf", 16);
    m->setTickLength(-16, -16, -10);
    m->setLineWidth(0, 2, 1, 1);

    // Add a smooth color scale to the meter
    double smoothColorScale[] = {0, 0x3333ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xdddd00, 100,
        0xff0000};
    m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
        smoothColorScale[0]))));

    // Add a text label centered at (150, 125) with 15pt Arial Italic font
    m->addText(150, 125, "CPU", "ariali.ttf", 15, Chart::TextColor, Chart::BottomCenter);

    // Add a red (0xff0000) pointer at the specified value
    m->addPointer2(value, 0xff0000);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *colorsemicirclemeter(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 72.55;

    // The background and border colors of the meters
    int bgColor[] = {0x88ccff, 0xffdddd, 0xffddaa, 0xffccff, 0xdddddd, 0xccffcc};
    int borderColor[] = {0x000077, 0x880000, 0xee6600, 0x440088, 0x000000, 0x006000};

    // Create an AngularMeter object of size 300 x 180 pixels with transparent background
    AngularMeter *m = new AngularMeter(300, 180, Chart::Transparent);

    // Center at (150, 150), scale radius = 124 pixels, scale angle -90 to +90 degrees
    m->setMeter(150, 150, 124, -90, 90);

    // Background gradient color with brighter color at the center
    double bgGradient[] = {0, (double)(m->adjustBrightness(bgColor[chartIndex], 3)), 0.75, (double)(
        bgColor[chartIndex])};

    // Add a scale background of 148 pixels radius using the background gradient, with a 13 pixel
    // thick border
    m->addScaleBackground(148, m->relativeRadialGradient(DoubleArray(bgGradient, (int)(sizeof(
        bgGradient) / sizeof(bgGradient[0])))), 13, borderColor[chartIndex]);

    // Meter scale is 0 - 100, with major tick every 20 units, minor tick every 10 units, and micro
    // tick every 5 units
    m->setScale(0, 100, 20, 10, 5);

    // Set the scale label style to 15pt Arial Italic. Set the major/minor/micro tick lengths to
    // 16/16/10 pixels pointing inwards, and their widths to 2/1/1 pixels.
    m->setLabelStyle("ariali.ttf", 16);
    m->setTickLength(-16, -16, -10);
    m->setLineWidth(0, 2, 1, 1);

    // Demostrate different types of color scales and putting them at different positions
    double smoothColorScale[] = {0, 0x3333ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xdddd00, 100,
        0xff0000};
    double stepColorScale[] = {0, 0x00cc00, 60, 0xffdd00, 80, 0xee0000, 100};
    double highLowColorScale[] = {0, 0x00ff00, 70, Chart::Transparent, 100, 0xff0000};

    if (chartIndex == 0) {
        // Add the smooth color scale at the default position
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))));
    } else if (chartIndex == 1) {
        // Add the smooth color scale starting at radius 124 with zero width and ending at radius
        // 124 with 16 pixels inner width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 124, 0, 124, -16);
    } else if (chartIndex == 2) {
        // Add the smooth color scale starting at radius 65 with zero width and ending at radius 55
        // with 20 pixels outer width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 65, 0, 55, 20);
    } else if (chartIndex == 3) {
        // Add the high/low color scale at the default position
        m->addColorScale(DoubleArray(highLowColorScale, (int)(sizeof(highLowColorScale) / sizeof(
            highLowColorScale[0]))));
    } else if (chartIndex == 4) {
        // Add the step color scale at the default position
        m->addColorScale(DoubleArray(stepColorScale, (int)(sizeof(stepColorScale) / sizeof(
            stepColorScale[0]))));
    } else {
        // Add the smooth color scale at radius 55 with 20 pixels outer width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 55, 20);
    }

    // Add a text label centered at (150, 125) with 15pt Arial Italic font
    m->addText(150, 125, "CPU", "ariali.ttf", 15, Chart::TextColor, Chart::BottomCenter);

    // Demonstrate two different types of pointers - thin triangular pointer (the default) and line
    // pointer
    if (chartIndex % 2 == 0) {
        m->addPointer2(value, 0xff0000);
    } else {
        m->addPointer2(value, 0xff0000, -1, Chart::LinePointer2);
    }

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *blacksemicirclemeter(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 72.55;

    // Create an AngularMeter object of size 300 x 180 pixels with transparent background
    AngularMeter *m = new AngularMeter(300, 180, Chart::Transparent);

    // Set the default text and line colors to white (0xffffff)
    m->setColor(Chart::TextColor, 0xffffff);
    m->setColor(Chart::LineColor, 0xffffff);

    // Center at (150, 150), scale radius = 128 pixels, scale angle -90 to +90 degrees
    m->setMeter(150, 150, 128, -90, 90);

    // Gradient color for the border to make it silver-like
    double ringGradient[] = {1, 0x909090, 0.5, 0xd6d6d6, 0, 0xeeeeee, -0.5, 0xd6d6d6, -1, 0x909090};

    // Add a black (0x000000) scale background of 148 pixels radius with a 10 pixel thick silver
    // border
    m->addScaleBackground(148, 0, 10, m->relativeLinearGradient(DoubleArray(ringGradient, (int)(
        sizeof(ringGradient) / sizeof(ringGradient[0]))), 45, 148));

    // Meter scale is 0 - 100, with major tick every 20 units, minor tick every 10 units, and micro
    // tick every 5 units
    m->setScale(0, 100, 20, 10, 5);

    // Set the scale label style to 15pt Arial Italic. Set the major/minor/micro tick lengths to
    // 16/16/10 pixels pointing inwards, and their widths to 2/1/1 pixels.
    m->setLabelStyle("ariali.ttf", 16);
    m->setTickLength(-16, -16, -10);
    m->setLineWidth(0, 2, 1, 1);

    // Demostrate different types of color scales and putting them at different positions
    double smoothColorScale[] = {0, 0x0000ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xdddd00, 100,
        0xff0000};
    double stepColorScale[] = {0, 0x00aa00, 60, 0xddaa00, 80, 0xcc0000, 100};
    double highLowColorScale[] = {0, 0x00ff00, 70, Chart::Transparent, 100, 0xff0000};

    if (chartIndex == 0) {
        // Add the smooth color scale at the default position
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))));
    } else if (chartIndex == 1) {
        // Add the smooth color scale starting at radius 128 with zero width and ending at radius
        // 128 with 16 pixels inner width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 128, 0, 128, -16);
    } else if (chartIndex == 2) {
        // Add the smooth color scale starting at radius 70 with zero width and ending at radius 60
        // with 20 pixels outer width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 70, 0, 60, 20);
    } else if (chartIndex == 3) {
        // Add the high/low color scale at the default position
        m->addColorScale(DoubleArray(highLowColorScale, (int)(sizeof(highLowColorScale) / sizeof(
            highLowColorScale[0]))));
    } else if (chartIndex == 4) {
        // Add the step color scale at the default position
        m->addColorScale(DoubleArray(stepColorScale, (int)(sizeof(stepColorScale) / sizeof(
            stepColorScale[0]))));
    } else {
        // Add the smooth color scale at radius 60 with 15 pixels outer width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 60, 15);
    }

    // Add a text label centered at (150, 125) with 15pt Arial Italic font
    m->addText(150, 125, "CPU", "ariali.ttf", 15, Chart::TextColor, Chart::BottomCenter);

    // Add a red (0xff0000) pointer at the specified value
    m->addPointer2(value, 0xff0000);

    // Add glare up to radius 138 (= region inside border)
    if (chartIndex % 2 == 0) {
        m->addGlare(138);
    }

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *whitesemicirclemeter(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 72.55;

    // Create an AngularMeter object of size 300 x 180 pixels with transparent background
    AngularMeter *m = new AngularMeter(300, 180, Chart::Transparent);

    // Set the default text and line colors to very dark grey (0x222222)
    m->setColor(Chart::TextColor, 0x222222);
    m->setColor(Chart::LineColor, 0x222222);

    // Center at (150, 150), scale radius = 128 pixels, scale angle -90 to +90 degrees
    m->setMeter(150, 150, 128, -90, 90);

    // Gradient color for the border to make it silver-like
    double ringGradient[] = {1, 0x999999, 0.5, 0xdddddd, 0, 0xf8f8f8, -0.5, 0xdddddd, -1, 0x999999};

    // Background gradient color from white (0xffffff) at the center to light grey (0xdddddd) at the
    // border
    double bgGradient[] = {0, 0xffffff, 0.75, 0xeeeeee, 1, 0xdddddd};

    // Add a scale background of 148 pixels radius using the gradient background, with a 10 pixel
    // thick silver border
    m->addScaleBackground(148, m->relativeRadialGradient(DoubleArray(bgGradient, (int)(sizeof(
        bgGradient) / sizeof(bgGradient[0]))), 148), 10, m->relativeLinearGradient(DoubleArray(
        ringGradient, (int)(sizeof(ringGradient) / sizeof(ringGradient[0]))), 45, 148));

    // Add a 1 pixel light grey (0xbbbbbb) line at the inner edge of the thick silver border (radius
    // = 138) to enhance its contrast with the background gradient
    m->addScaleBackground(138, Chart::Transparent, 1, 0xbbbbbb);

    // Meter scale is 0 - 100, with major tick every 20 units, minor tick every 10 units, and micro
    // tick every 5 units
    m->setScale(0, 100, 20, 10, 5);

    // Set the scale label style to 15pt Arial Italic. Set the major/minor/micro tick lengths to
    // 16/16/10 pixels pointing inwards, and their widths to 2/1/1 pixels.
    m->setLabelStyle("ariali.ttf", 16);
    m->setTickLength(-16, -16, -10);
    m->setLineWidth(0, 2, 1, 1);

    // Demostrate different types of color scales and putting them at different positions
    double smoothColorScale[] = {0, 0x3333ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xdddd00, 100,
        0xff0000};
    double stepColorScale[] = {0, 0x00cc00, 60, 0xffdd00, 80, 0xee0000, 100};
    double highLowColorScale[] = {0, 0x00ff00, 70, Chart::Transparent, 100, 0xff0000};

    if (chartIndex == 0) {
        // Add the smooth color scale at the default position
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))));
    } else if (chartIndex == 1) {
        // Add the smooth color scale starting at radius 128 with zero width and ending at radius
        // 128 with 16 pixels inner width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 128, 0, 128, -16);
    } else if (chartIndex == 2) {
        // Add the smooth color scale starting at radius 70 with zero width and ending at radius 60
        // with 20 pixels outer width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 70, 0, 60, 20);
    } else if (chartIndex == 3) {
        // Add the high/low color scale at the default position
        m->addColorScale(DoubleArray(highLowColorScale, (int)(sizeof(highLowColorScale) / sizeof(
            highLowColorScale[0]))));
    } else if (chartIndex == 4) {
        // Add the step color scale at the default position
        m->addColorScale(DoubleArray(stepColorScale, (int)(sizeof(stepColorScale) / sizeof(
            stepColorScale[0]))));
    } else {
        // Add the smooth color scale at radius 60 with 15 pixels outer width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 60, 15);
    }

    // Add a text label centered at (150, 125) with 15pt Arial Italic font
    m->addText(150, 125, "CPU", "ariali.ttf", 15, Chart::TextColor, Chart::BottomCenter);

    // Add a red (0xff0000) pointer at the specified value
    m->addPointer2(value, 0xff0000);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *semicirclemeterreadout(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 66;

    // The background and border colors of the meters
    int bgColor[] = {0x88ccff, 0xffdddd};
    int borderColor[] = {0x000077, 0x880000};

    // Create an AngularMeter object of size 300 x 200 pixels with transparent background
    AngularMeter *m = new AngularMeter(300, 200, Chart::Transparent);

    // Center at (150, 150), scale radius = 124 pixels, scale angle -90 to +90 degrees
    m->setMeter(150, 150, 124, -90, 90);

    // Background gradient color with brighter color at the center
    double bgGradient[] = {0, (double)(m->adjustBrightness(bgColor[chartIndex], 3)), 0.75, (double)(
        bgColor[chartIndex])};

    // Add a scale background of 148 pixels radius using the background gradient, with a 13 pixel
    // thick border
    m->addScaleBackground(148, m->relativeRadialGradient(DoubleArray(bgGradient, (int)(sizeof(
        bgGradient) / sizeof(bgGradient[0])))), 13, borderColor[chartIndex]);

    // Meter scale is 0 - 100, with major tick every 20 units, minor tick every 10 units, and micro
    // tick every 5 units
    m->setScale(0, 100, 20, 10, 5);

    // Set the scale label style to 15pt Arial Italic. Set the major/minor/micro tick lengths to
    // 16/16/10 pixels pointing inwards, and their widths to 2/1/1 pixels.
    m->setLabelStyle("ariali.ttf", 16);
    m->setTickLength(-16, -16, -10);
    m->setLineWidth(0, 2, 1, 1);

    // Demostrate different types of color scales and putting them at different positions
    double smoothColorScale[] = {0, 0x3333ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xdddd00, 100,
        0xff0000};

    if (chartIndex == 0) {
        // Add the smooth color scale at the default position
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))));
        // Add a red (0xff0000) triangular pointer starting from 38% and ending at 60% of scale
        // radius, with a width 6 times the default
        m->addPointer2(value, 0xff0000, -1, Chart::TriangularPointer2, 0.38, 0.6, 6);
    } else {
        // Add the smooth color scale starting at radius 124 with zero width and ending at radius
        // 124 with 16 pixels inner width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 124, 0, 124, -16);
        // Add a red (0xff0000) pointer
        m->addPointer2(value, 0xff0000);
    }

    // Configure a large "pointer cap" to be used as the readout circle at the center. The cap
    // radius and border width is set to 33% and 4% of the meter scale radius. The cap color is dark
    // blue (0x000044). The border color is light blue (0x66bbff) with a 60% brightness gradient
    // effect.
    m->setCap2(Chart::Transparent, 0x000044, 0x66bbff, 0.6, 0, 0.33, 0.04);

    // Add value label at the center with light blue (0x66ddff) 28pt Arial Italic font
    m->addText(150, 150, m->formatValue(value, "{value|0}"), "ariali.ttf", 28, 0x66ddff,
        Chart::Center)->setMargin(0);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *roundmeter(int /* chartIndex */, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 72.3;

    // Create an AngularMeter object of size 250 x 250 pixels with transparent background
    AngularMeter *m = new AngularMeter(250, 250, Chart::Transparent);

    // Center at (125, 125), scale radius = 111 pixels, scale angle -145 to +145 degrees
    m->setMeter(125, 125, 111, -145, 145);

    // Add a very light grey (0xeeeeee) circle with radius 123 pixels as background
    m->addRing(0, 123, 0xeeeeee);
    // Add a grey (0xcccccc) ring between radii 116 and 123 pixels as border
    m->addRing(116, 123, 0xcccccc);

    // Meter scale is 0 - 100, with major/minor/micro ticks every 10/5/1 units
    m->setScale(0, 100, 10, 5, 1);

    // Set the scale label style to 15pt Arial Italic. Set the major/minor/micro tick lengths to
    // 12/9/6 pixels pointing inwards, and their widths to 2/1/1 pixels.
    m->setLabelStyle("ariali.ttf", 15);
    m->setTickLength(-12, -9, -6);
    m->setLineWidth(0, 2, 1, 1);

    // Add a smooth color scale to the meter
    double smoothColorScale[] = {0, 0x3333ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xdddd00, 100,
        0xff0000};
    m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
        smoothColorScale[0]))));

    // Add a text label centered at (125, 175) with 15pt Arial Italic font
    m->addText(125, 175, "CPU", "ariali.ttf", 15, Chart::TextColor, Chart::Center);

    // Add a red (0xff0000) pointer at the specified value
    m->addPointer2(value, 0xff0000);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *colorroundmeter(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 72.3;

    // The background and border colors of the meters
    int bgColor[] = {0x88ccff, 0xffdddd, 0xffddaa, 0xffccff, 0xdddddd, 0xccffcc};
    int borderColor[] = {0x000077, 0x880000, 0xee6600, 0x440088, 0x000000, 0x006000};

    // Create an AngularMeter object of size 250 x 250 pixels with transparent background
    AngularMeter *m = new AngularMeter(250, 250, Chart::Transparent);

    // Demonstration two different meter scale angles
    if (chartIndex % 2 == 0) {
        // Center at (125, 125), scale radius = 111 pixels, scale angle -140 to +140 degrees
        m->setMeter(125, 125, 111, -140, 140);
    } else {
        // Center at (125, 125), scale radius = 111 pixels, scale angle -180 to +90 degrees
         m->setMeter(125, 125, 111, -180, 90);
    }

    // Background gradient color with brighter color at the center
    double bgGradient[] = {0, (double)(m->adjustBrightness(bgColor[chartIndex], 3)), 0.75, (double)(
        bgColor[chartIndex])};
    // Add circle with radius 123 pixels as background using the background gradient
    m->addRing(0, 123, m->relativeRadialGradient(DoubleArray(bgGradient, (int)(sizeof(bgGradient) /
        sizeof(bgGradient[0])))));
    // Add a ring between radii 116 and 123 pixels as border
    m->addRing(116, 123, borderColor[chartIndex]);

    // Meter scale is 0 - 100, with major/minor/micro ticks every 10/5/1 units
    m->setScale(0, 100, 10, 5, 1);

    // Set the scale label style to 15pt Arial Italic. Set the major/minor/micro tick lengths to
    // 12/9/6 pixels pointing inwards, and their widths to 2/1/1 pixels.
    m->setLabelStyle("ariali.ttf", 15);
    m->setTickLength(-12, -9, -6);
    m->setLineWidth(0, 2, 1, 1);

    // Demostrate different types of color scales and putting them at different positions
    double smoothColorScale[] = {0, 0x3333ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xdddd00, 100,
        0xff0000};
    double stepColorScale[] = {0, 0x00cc00, 60, 0xffdd00, 80, 0xee0000, 100};
    double highLowColorScale[] = {0, 0x00ff00, 70, Chart::Transparent, 100, 0xff0000};

    if (chartIndex == 0) {
        // Add the smooth color scale at the default position
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))));
    } else if (chartIndex == 1) {
        // Add the smooth color scale starting at radius 62 with zero width and ending at radius 40
        // with 22 pixels outer width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 62, 0, 40, 22);
    } else if (chartIndex == 2) {
        // Add green, yellow and red zones between radii 44 and 60
        m->addZone(0, 60, 44, 60, 0x00dd00);
        m->addZone(60, 80, 44, 60, 0xffff00);
        m->addZone(80, 100, 44, 60, 0xff0000);
    } else if (chartIndex == 3) {
        // Add the high/low color scale at the default position
        m->addColorScale(DoubleArray(highLowColorScale, (int)(sizeof(highLowColorScale) / sizeof(
            highLowColorScale[0]))));
    } else if (chartIndex == 4) {
        // Add the smooth color scale at radius 44 with 16 pixels outer width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 44, 16);
    } else {
        // Add the step color scale at the default position
        m->addColorScale(DoubleArray(stepColorScale, (int)(sizeof(stepColorScale) / sizeof(
            stepColorScale[0]))));
    }

    // Add a text label centered at (125, 175) with 15pt Arial Italic font
    m->addText(125, 175, "CPU", "ariali.ttf", 15, Chart::TextColor, Chart::Center);

    // Add a readout to some of the charts as demonstration
    if (chartIndex == 0 || chartIndex == 2) {
        // Put the value label center aligned at (125, 232), using white (0xffffff) 14pt Arial font
        // on a black (0x000000) background. Set box width to 50 pixels with 5 pixels rounded
        // corners.
        TextBox *t = m->addText(125, 232, m->formatValue(value,
            "<*block,width=50,halign=center*>{value|1}"), "arial.ttf", 14, 0xffffff,
            Chart::BottomCenter);
        t->setBackground(0x000000);
        t->setRoundedCorners(5);
    }

    // Add a red (0xff0000) pointer at the specified value
    m->addPointer2(value, 0xff0000);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *blackroundmeter(int chartIndex, const char ** /* imageMap */)
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
    m->makeChart();

    return m;
}


BaseChart *whiteroundmeter(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 72.3;

    // Create an AngularMeter object of size 250 x 250 pixels with transparent background
    AngularMeter *m = new AngularMeter(250, 250, Chart::Transparent);

    // Set the default text and line colors to dark grey (0x333333)
    m->setColor(Chart::TextColor, 0x333333);
    m->setColor(Chart::LineColor, 0x333333);

    // Demonstration two different meter scale angles
    if (chartIndex % 2 == 0) {
        // Center at (125, 125), scale radius = 111 pixels, scale angle -140 to +140 degrees
        m->setMeter(125, 125, 109, -140, 140);
    } else {
        // Center at (125, 125), scale radius = 111 pixels, scale angle -180 to +90 degrees
         m->setMeter(125, 125, 109, -180, 90);
    }

    // Add a black (0x000000) circle with radius 123 pixels as background
    m->addRing(0, 123, 0x000000);

    // Background gradient color from white (0xffffff) at the center to light grey (0xdddddd) at the
    // border
    double bgGradient[] = {0, 0xffffff, 0.75, 0xeeeeee, 1, 0xdddddd};
    // Add circle with radius 123 pixels as background using the background gradient
    m->addRing(0, 123, m->relativeRadialGradient(DoubleArray(bgGradient, (int)(sizeof(bgGradient) /
        sizeof(bgGradient[0]))), 123));

    // Gradient color for the border to make it silver-like
    double ringGradient[] = {1, 0x999999, 0.5, 0xdddddd, 0, 0xffffff, -0.5, 0xdddddd, -1, 0x999999};
    // Add a ring between radii 114 and 123 pixels using the silver gradient with a light grey
    // (0xbbbbbb) edge as the meter border
    m->addRing(114, 123, m->relativeLinearGradient(DoubleArray(ringGradient, (int)(sizeof(
        ringGradient) / sizeof(ringGradient[0]))), 45, 123), 0xbbbbbb);

    // Meter scale is 0 - 100, with major/minor/micro ticks every 10/5/1 units
    m->setScale(0, 100, 10, 5, 1);

    // Set the scale label style to 15pt Arial Italic. Set the major/minor/micro tick lengths to
    // 12/9/6 pixels pointing inwards, and their widths to 2/1/1 pixels.
    m->setLabelStyle("ariali.ttf", 15);
    m->setTickLength(-12, -9, -6);
    m->setLineWidth(0, 2, 1, 1);

    // Demostrate different types of color scales and putting them at different positions
    double smoothColorScale[] = {0, 0x3333ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xdddd00, 100,
        0xff0000};
    double stepColorScale[] = {0, 0x00cc00, 60, 0xffdd00, 80, 0xee0000, 100};
    double highLowColorScale[] = {0, 0x00ff00, 70, Chart::Transparent, 100, 0xff0000};

    if (chartIndex == 0) {
        // Add the smooth color scale at the default position
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))));
    } else if (chartIndex == 1) {
        // Add the smooth color scale starting at radius 62 with zero width and ending at radius 40
        // with 22 pixels outer width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 62, 0, 40, 22);
    } else if (chartIndex == 2) {
        // Add the smooth color scale starting at radius 109 with zero width and ending at radius
        // 109 with 12 pixels inner width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 109, 0, 109, -12);
    } else if (chartIndex == 3) {
        // Add the high/low color scale at the default position
        m->addColorScale(DoubleArray(highLowColorScale, (int)(sizeof(highLowColorScale) / sizeof(
            highLowColorScale[0]))));
    } else if (chartIndex == 4) {
        // Add the smooth color scale at radius 44 with 16 pixels outer width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 44, 16);
    } else {
        // Add the step color scale at the default position
        m->addColorScale(DoubleArray(stepColorScale, (int)(sizeof(stepColorScale) / sizeof(
            stepColorScale[0]))));
    }

    // Add a text label centered at (125, 175) with 15pt Arial Italic font
    m->addText(125, 175, "CPU", "ariali.ttf", 15, Chart::TextColor, Chart::Center);

    // Add a readout to some of the charts as demonstration
    if (chartIndex == 0 || chartIndex == 2) {
        // Put the value label center aligned at (125, 232), using white (0xffffff) 14pt Arial font
        // on a dark grey (0x222222) background. Set box width to 50 pixels with 5 pixels rounded
        // corners.
        TextBox *t = m->addText(125, 232, m->formatValue(value,
            "<*block,width=50,halign=center*>{value|1}"), "arial.ttf", 14, 0xffffff,
            Chart::BottomCenter);
        t->setBackground(0x222222);
        t->setRoundedCorners(5);
    }

    // Add a red (0xff0000) pointer at the specified value
    m->addPointer2(value, 0xff0000);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *neonroundmeter(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 50;

    // The main color of the four meters in this example. The other colors and gradients are derived
    // from the main color.
    int colorList[] = {0x007700, 0x770077, 0x0033dd, 0x880000};
    int mainColor = colorList[chartIndex];

    //
    // In this example, we demonstrate how to parameterized by size, so that the chart size can be
    // changed by changing just one variable.
    //
    int size = 300;

    // The radius of the entire meter, which is size / 2, minus 2 pixels for margin
    int outerRadius = size / 2 - 2;

    // The radius of the meter scale
    int scaleRadius = outerRadius * 92 / 100;

    // The radius of the inner decorative circle
    int innerRadius = scaleRadius * 40 / 100;

    // The width of the color scale
    int colorScaleWidth = scaleRadius * 10 / 100;

    // Major tick length
    int tickLength = scaleRadius * 10 / 100;

    // Major tick width
    int tickWidth = scaleRadius * 1 / 100 + 1;

    // Label font size
    int fontSize = scaleRadius * 13 / 100;

    //
    // Create an angular meter based on the above parameters
    //

    // Create an AngularMeter object of the specified size. In this demo, we use black (0x000000) as
    // the background color. You can also use transparent or other colors.
    AngularMeter *m = new AngularMeter(size, size, 0x000000);

    // Set the default text and line colors to white (0xffffff)
    m->setColor(Chart::TextColor, 0xffffff);
    m->setColor(Chart::LineColor, 0xffffff);

    // Set meter center and scale radius, and set the scale angle from -180 to +90 degrees
    m->setMeter(size / 2, size / 2, scaleRadius, -180, 90);

    // Background gradient with the mainColor at the center and become darker near the border
    double bgGradient[] = {0, (double)(mainColor), 0.5, (double)(m->adjustBrightness(mainColor, 0.75
        )), 1, (double)(m->adjustBrightness(mainColor, 0.15))};

    // Fill the meter background with the background gradient
    m->addRing(0, outerRadius, m->relativeRadialGradient(DoubleArray(bgGradient, (int)(sizeof(
        bgGradient) / sizeof(bgGradient[0]))), outerRadius * 0.66));

    // Fill the inner circle with the same background gradient for decoration
    m->addRing(0, innerRadius, m->relativeRadialGradient(DoubleArray(bgGradient, (int)(sizeof(
        bgGradient) / sizeof(bgGradient[0]))), innerRadius * 0.8));

    // Gradient for the neon backlight, with the main color at the scale radius fading to
    // transparent
    double neonGradient[] = {0.89, Chart::Transparent, 1, (double)(mainColor), 1.07,
        Chart::Transparent};
    m->addRing(scaleRadius * 85 / 100, outerRadius, m->relativeRadialGradient(DoubleArray(
        neonGradient, (int)(sizeof(neonGradient) / sizeof(neonGradient[0])))));

    // The neon ring at the scale radius with width equal to 1/80 of the scale radius, creating
    // using a brighter version of the main color
    m->addRing(scaleRadius, scaleRadius + scaleRadius / 80, m->adjustBrightness(mainColor, 2));

    // Meter scale is 0 - 100, with major/minor/micro ticks every 10/5/1 units
    m->setScale(0, 100, 10, 5, 1);

    // Set the scale label style, tick length and tick width. The minor and micro tick lengths are
    // 80% and 60% of the major tick length, and their widths are around half of the major tick
    // width.
    m->setLabelStyle("ariali.ttf", fontSize);
    m->setTickLength(-tickLength, -tickLength * 80 / 100, -tickLength * 60 / 100);
    m->setLineWidth(0, tickWidth, (tickWidth + 1) / 2, (tickWidth + 1) / 2);

    // Demostrate different types of color scales and glare effects and putting them at different
    // positions.
    double smoothColorScale[] = {0, 0x0000ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xdddd00, 100,
        0xff0000};
    double stepColorScale[] = {0, 0x00dd00, 60, 0xddaa00, 80, 0xdd0000, 100};
    double highColorScale[] = {70, Chart::Transparent, 100, 0xff0000};

    if (chartIndex == 1) {
        // Add the smooth color scale just outside the inner circle
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), innerRadius + 1, colorScaleWidth);
        // Add glare up to the scale radius, concave and spanning 190 degrees
        m->addGlare(scaleRadius, -190);
    } else if (chartIndex == 2) {
        // Add the high color scale at the default position
        m->addColorScale(DoubleArray(highColorScale, (int)(sizeof(highColorScale) / sizeof(
            highColorScale[0]))));
        // Add glare up to the scale radius
        m->addGlare(scaleRadius);
    } else {
        // Add the step color scale just outside the inner circle
        m->addColorScale(DoubleArray(stepColorScale, (int)(sizeof(stepColorScale) / sizeof(
            stepColorScale[0]))), innerRadius + 1, colorScaleWidth);
        // Add glare up to the scale radius, concave and spanning 190 degrees and rotated by -45
        // degrees
        m->addGlare(scaleRadius, -190, -45);
    }

    // Add a red (0xff0000) pointer at the specified value
    m->addPointer2(value, 0xff0000);

    // Set the cap background to a brighter version of the mainColor, and using black (0x000000) for
    // the cap and grey (0x999999) for the cap border
    m->setCap2(m->adjustBrightness(mainColor, 1.1), 0x000000, 0x999999);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *roundmeterreadout(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 54;

    // The main color of the four meters in this example. The other colors and gradients are derived
    // from the main color.
    int colorList[] = {0x0033dd, 0xaaaa00};
    int mainColor = colorList[chartIndex];

    //
    // In this example, we demonstrate how to parameterized by size, so that the chart size can be
    // changed by changing just one variable.
    //
    int size = 300;

    // The radius of the entire meter, which is size / 2, minus 2 pixels for margin
    int outerRadius = size / 2 - 2;

    // The radius of the meter scale
    int scaleRadius = outerRadius * 92 / 100;

    // The radius of the color scale
    int colorScaleRadius = scaleRadius * 43 / 100;

    // The width of the color scale
    int colorScaleWidth = scaleRadius * 10 / 100;

    // Major tick length
    int tickLength = scaleRadius * 10 / 100;

    // Major tick width
    int tickWidth = scaleRadius * 1 / 100 + 1;

    // Label font size
    int fontSize = scaleRadius * 13 / 100;

    // Radius of readout circle as a ratio to the scale radius
    double readOutRadiusRatio = 0.333333333333;

    // Readout font size
    int readOutFontSize = scaleRadius * 24 / 100;

    //
    // Create an angular meter based on the above parameters
    //

    // Create an AngularMeter object of the specified size. In this demo, we use black (0x000000) as
    // the background color. You can also use transparent or other colors.
    AngularMeter *m = new AngularMeter(size, size, 0x000000);

    // Set the default text and line colors to white (0xffffff)
    m->setColor(Chart::TextColor, 0xffffff);
    m->setColor(Chart::LineColor, 0xffffff);

    // Set meter center and scale radius, and set the scale angle from -180 to +90 degrees
    m->setMeter(size / 2, size / 2, scaleRadius, -180, 90);

    // Background gradient with the mainColor at the center and become darker near the border
    double bgGradient[] = {0, (double)(mainColor), 0.5, (double)(m->adjustBrightness(mainColor, 0.75
        )), 1, (double)(m->adjustBrightness(mainColor, 0.15))};

    // Fill the meter background with the background gradient
    m->addRing(0, outerRadius, m->relativeRadialGradient(DoubleArray(bgGradient, (int)(sizeof(
        bgGradient) / sizeof(bgGradient[0]))), outerRadius * 0.66));

    // Gradient for the neon backlight, with the main color at the scale radius fading to
    // transparent
    double neonGradient[] = {0.89, Chart::Transparent, 1, (double)(mainColor), 1.07,
        Chart::Transparent};
    m->addRing(scaleRadius * 85 / 100, outerRadius, m->relativeRadialGradient(DoubleArray(
        neonGradient, (int)(sizeof(neonGradient) / sizeof(neonGradient[0])))));

    // The neon ring at the scale radius with width equal to 1/80 of the scale radius, creating
    // using a brighter version of the main color
    m->addRing(scaleRadius, scaleRadius + scaleRadius / 80, m->adjustBrightness(mainColor, 2));

    // Meter scale is 0 - 100, with major/minor/micro ticks every 10/5/1 units
    m->setScale(0, 100, 10, 5, 1);

    // Set the scale label style, tick length and tick width. The minor and micro tick lengths are
    // 80% and 60% of the major tick length, and their widths are around half of the major tick
    // width.
    m->setLabelStyle("ariali.ttf", fontSize);
    m->setTickLength(-tickLength, -tickLength * 80 / 100, -tickLength * 60 / 100);
    m->setLineWidth(0, tickWidth, (tickWidth + 1) / 2, (tickWidth + 1) / 2);

    // Demostrate different types of color scales and putting them at different positions.
    double smoothColorScale[] = {0, 0x0000ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xdddd00, 100,
        0xff0000};
    double highColorScale[] = {70, Chart::Transparent, 100, 0xff0000};

    if (chartIndex == 0) {
        // Add the smooth color scale
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), colorScaleRadius, colorScaleWidth);
        // Add a red (0xff0000) pointer
        m->addPointer2(value, 0xff0000);
    } else {
        // Add the high color scale at the default position
        m->addColorScale(DoubleArray(highColorScale, (int)(sizeof(highColorScale) / sizeof(
            highColorScale[0]))));
        // Add a red (0xff0000) triangular pointer starting from 40% and ending at 60% of scale
        // radius, with a width 6 times the default
        m->addPointer2(value, 0xff0000, -1, Chart::TriangularPointer2, 0.4, 0.6, 6);
    }

    // Configure a large "pointer cap" to be used as the readout circle at the center. The cap
    // border width is set to 1.5% of the scale radius. The cap color is darker version of the main
    // color. The border color is a brighter version of the main color with 75% brightness gradient
    // effect.
    m->setCap2(Chart::Transparent, m->adjustBrightness(mainColor, 0.3), m->adjustBrightness(
        mainColor, 1.5), 0.75, 0, readOutRadiusRatio, 0.015);

    // Add value label at the center using a brighter version of the main color and Arial Italic
    // font
    m->addText(size / 2, size / 2, m->formatValue(value, "{value|0}"), "ariali.ttf",
        readOutFontSize, m->adjustBrightness(mainColor, 2.5), Chart::Center)->setMargin(0);

    // Add glare up to the scale radius
    m->addGlare(scaleRadius);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *rectangularmeter(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 72.55;

    // Create an AngularMeter object of size 240 x 170 pixels with very light grey (0xeeeeee)
    // background, and a rounded 4-pixel thick light grey (0xcccccc) border
    AngularMeter *m = new AngularMeter(240, 170, 0xeeeeee, 0xcccccc);
    m->setRoundedFrame(Chart::Transparent);
    m->setThickFrame(4);

    // Set the default text and line colors to dark grey (0x222222)
    m->setColor(Chart::TextColor, 0x222222);
    m->setColor(Chart::LineColor, 0x222222);

    // Center at (120, 145), scale radius = 128 pixels, scale angle -60 to +60 degrees
    m->setMeter(120, 145, 128, -60, 60);

    // Meter scale is 0 - 100, with major/minor/micro ticks every 20/10/5 units
    m->setScale(0, 100, 20, 10, 5);

    // Set the scale label style to 14pt Arial Italic. Set the major/minor/micro tick lengths to
    // 16/16/10 pixels pointing inwards, and their widths to 2/1/1 pixels.
    m->setLabelStyle("ariali.ttf", 14);
    m->setTickLength(-16, -16, -10);
    m->setLineWidth(0, 2, 1, 1);

    // Demostrate different types of color scales and putting them at different positions
    double smoothColorScale[] = {0, 0x3333ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xdddd00, 100,
        0xff0000};
    double stepColorScale[] = {0, 0x00cc00, 60, 0xffdd00, 80, 0xee0000, 100};
    double highLowColorScale[] = {0, 0x00ff00, 70, Chart::Transparent, 100, 0xff0000};

    if (chartIndex == 0) {
        // Add the smooth color scale at the default position
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))));
    } else if (chartIndex == 1) {
        // Add the smooth color scale starting at radius 128 with zero width and ending at radius
        // 128 with 16 pixels inner width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 128, 0, 128, -16);
    } else if (chartIndex == 2) {
        // Add the smooth color scale starting at radius 70 with zero width and ending at radius 60
        // with 20 pixels outer width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 70, 0, 60, 20);
    } else if (chartIndex == 3) {
        // Add the high/low color scale at the default position
        m->addColorScale(DoubleArray(highLowColorScale, (int)(sizeof(highLowColorScale) / sizeof(
            highLowColorScale[0]))));
    } else if (chartIndex == 4) {
        // Add the step color scale at the default position
        m->addColorScale(DoubleArray(stepColorScale, (int)(sizeof(stepColorScale) / sizeof(
            stepColorScale[0]))));
    } else {
        // Add the smooth color scale at radius 60 with 15 pixels outer width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 60, 15);
    }

    // Add a text label centered at (120, 120) with 15pt Arial Italic font
    m->addText(120, 120, "CPU", "ariali.ttf", 15, Chart::TextColor, Chart::BottomCenter);

    // Add a red (0xff0000) pointer at the specified value
    m->addPointer2(value, 0xff0000);

    // Add a semi-transparent light grey (0x3fcccccc) rectangle at (0, 120) and of size 240 x 60
    // pixels to cover the bottom part of the meter for decoration
    TextBox *cover = m->addText(0, 120, "");
    cover->setSize(240, 60);
    cover->setBackground(0x3fcccccc);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *squareameter(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 4.75;

    // Create an AugularMeter object of size 110 x 110 pixels, using silver background color with a
    // black 2 pixel 3D depressed border.
    AngularMeter *m = new AngularMeter(110, 110, Chart::silverColor(), 0x000000, -2);

    // Set meter appearance according to a parameter
    if (chartIndex == 0) {
        // Set the meter center at bottom left corner (15, 95), with radius 85 pixels. Meter spans
        // from 90 - 0 degrees.
        m->setMeter(15, 95, 85, 90, 0);
        // Add a label to the meter centered at (35, 75)
        m->addText(35, 75, "VDC", "arialbd.ttf", 12, Chart::TextColor, Chart::Center);
        // Add a text box to show the value at top right corner (103, 7)
        m->addText(103, 7, m->formatValue(value, "2"), "arial.ttf", 8, 0xffffff, Chart::TopRight
            )->setBackground(0, 0, -1);
    } else if (chartIndex == 1) {
        // Set the meter center at top left corner (15, 15), with radius 85 pixels. Meter spans from
        // 90 - 180 degrees.
        m->setMeter(15, 15, 85, 90, 180);
        // Add a label to the meter centered at (35, 35)
        m->addText(35, 35, "AMP", "arialbd.ttf", 12, Chart::TextColor, Chart::Center);
        // Add a text box to show the value at bottom right corner (103, 103)
        m->addText(103, 103, m->formatValue(value, "2"), "arial.ttf", 8, 0xffffff,
            Chart::BottomRight)->setBackground(0, 0, -1);
    } else if (chartIndex == 2) {
        // Set the meter center at top right corner (15, 95), with radius 85 pixels. Meter spans
        // from 270 - 180 degrees.
        m->setMeter(95, 15, 85, 270, 180);
        // Add a label to the meter centered at (75, 35)
        m->addText(75, 35, "KW", "arialbd.ttf", 12, Chart::TextColor, Chart::Center);
        // Add a text box to show the value at bottom left corner (7, 103)
        m->addText(7, 103, m->formatValue(value, "2"), "arial.ttf", 8, 0xffffff, Chart::BottomLeft
            )->setBackground(0, 0, -1);
    } else {
        // Set the meter center at bottom right corner (95, 95), with radius 85 pixels. Meter spans
        // from 270 - 360 degrees.
        m->setMeter(95, 95, 85, 270, 360);
        // Add a label to the meter centered at (75, 75)
        m->addText(75, 75, "RPM", "arialbd.ttf", 12, Chart::TextColor, Chart::Center);
        // Add a text box to show the value at top left corner (7, 7)
        m->addText(7, 7, m->formatValue(value, "2"), "arial.ttf", 8, 0xffffff, Chart::TopLeft
            )->setBackground(0, 0, -1);
    }

    // Meter scale is 0 - 10, with a major tick every 2 units, and minor tick every 1 unit
    m->setScale(0, 10, 2, 1);

    // Set 0 - 6 as green (99ff99) zone, 6 - 8 as yellow (ffff00) zone, and 8 - 10 as red (ff3333)
    // zone
    m->addZone(0, 6, 0x99ff99, 0x808080);
    m->addZone(6, 8, 0xffff00, 0x808080);
    m->addZone(8, 10, 0xff3333, 0x808080);

    // Add a semi-transparent black (80000000) pointer at the specified value
    m->addPointer(value, 0x80000000);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *angularpointer(int /* chartIndex */, const char ** /* imageMap */)
{
    // Create an AngularMeter object of size 300 x 300 pixels with transparent background
    AngularMeter *m = new AngularMeter(300, 300, Chart::Transparent);

    // Set the default text and line colors to white (0xffffff)
    m->setColor(Chart::TextColor, 0xffffff);
    m->setColor(Chart::LineColor, 0xffffff);

    // Center at (150, 150), scale radius = 128 pixels, scale angle 0 to 360 degrees
    m->setMeter(150, 150, 128, 0, 360);

    // Add a black (0x000000) circle with radius 148 pixels as background
    m->addRing(0, 148, 0x000000);

    // Add a ring between radii 139 and 147 pixels using the silver color with a light grey
    // (0xcccccc) edge as border
    m->addRing(139, 147, Chart::silverColor(), 0xcccccc);

    // Meter scale is 0 - 100, with major/minor/micro ticks every 10/5/1 units
    m->setScale(0, 100, 10, 5, 1);

    // Set the scale label style to 16pt Arial Italic. Set the major/minor/micro tick lengths to
    // 13/10/7 pixels pointing inwards, and their widths to 2/1/1 pixels.
    m->setLabelStyle("ariali.ttf", 16);
    m->setTickLength(-13, -10, -7);
    m->setLineWidth(0, 2, 1, 1);

    // Add a semi-transparent blue (0x7f6666ff) pointer using the default shape
    m->addPointer(25, 0x7f6666ff, 0x6666ff);

    // Add a semi-transparent red (0x7fff6666) pointer using the arrow shape
    m->addPointer(9, 0x7fff6666, 0xff6666)->setShape(Chart::ArrowPointer2);

    // Add a semi-transparent yellow (0x7fffff66) pointer using another arrow shape
    m->addPointer(51, 0x7fffff66, 0xffff66)->setShape(Chart::ArrowPointer);

    // Add a semi-transparent green (0x7f66ff66) pointer using the line shape
    m->addPointer(72, 0x7f66ff66, 0x66ff66)->setShape(Chart::LinePointer);

    // Add a semi-transparent grey (0x7fcccccc) pointer using the pencil shape
    m->addPointer(85, 0x7fcccccc, 0xcccccc)->setShape(Chart::PencilPointer);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *angularpointer2(int /* chartIndex */, const char ** /* imageMap */)
{
    // Create an AngularMeter object of size 300 x 300 pixels with transparent background
    AngularMeter *m = new AngularMeter(300, 300, Chart::Transparent);

    // Set the default text and line colors to white (0xffffff)
    m->setColor(Chart::TextColor, 0xffffff);
    m->setColor(Chart::LineColor, 0xffffff);

    // Center at (150, 150), scale radius = 125 pixels, scale angle 0 to 360 degrees
    m->setMeter(150, 150, 125, 0, 360);

    // Add a black (0x000000) circle with radius 148 pixels as background
    m->addRing(0, 148, 0x000000);

    // Add a ring between radii 139 and 147 pixels using the silver color with a light grey
    // (0xcccccc) edge as border
    m->addRing(139, 147, Chart::silverColor(), 0xcccccc);

    // Meter scale is 0 - 100, with major/minor/micro ticks every 10/5/1 units
    m->setScale(0, 100, 10, 5, 1);

    // Set the scale label style to 16pt Arial Italic. Set the major/minor/micro tick lengths to
    // 13/10/7 pixels pointing inwards, and their widths to 2/1/1 pixels.
    m->setLabelStyle("ariali.ttf", 16);
    m->setTickLength(-13, -10, -7);
    m->setLineWidth(0, 2, 1, 1);

    // Add a default red (0xff0000) pointer
    m->addPointer2(25, 0xff0000);

    // Add a semi-transparent green (0x3f00ff00) line style pointer
    m->addPointer2(9, 0x3f00ff00, -1, Chart::LinePointer2);

    // Add a semi-transparent blue (0x7f66aaff) triangular pointer floating between 60% and 85% of
    // the scale radius with the pointer width 5 times the default
    m->addPointer2(52, 0x7f66aaff, 0x66aaff, Chart::TriangularPointer2, 0.6, 0.85, 5);

    // Add a semi-transparent yellow (0x7fffff66) triangular pointer floating between 80% and 90% of
    // the scale radius with the pointer width 5 times the default
    m->addPointer2(65, 0x7fffff66, 0xffff66, Chart::TriangularPointer2, 0.8, 0.9, 5);

    // Add two red (0xff0000) triangular pointer at 72 and 94. The pointers float between 110% and
    // 100% of the scale radius with widths 3 times the default
    m->addPointer2(72, 0xff0000, -1, Chart::TriangularPointer2, 1.1, 1.0, 3);
    m->addPointer2(94, 0xff0000, -1, Chart::TriangularPointer2, 1.1, 1.0, 3);

    // Add a red (0xcc0000) zone spanning from 72 to 94, and between the radii 112 and 125
    m->addZone(72, 94, 125, 112, 0xcc0000);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *iconameter(int /* chartIndex */, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 85;

    // Create an AugularMeter object of size 70 x 90 pixels, using black background with a 2 pixel
    // 3D depressed border.
    AngularMeter *m = new AngularMeter(70, 90, 0, 0, -2);

    // Use white on black color palette for default text and line colors
    m->setColors(Chart::whiteOnBlackPalette);

    // Set the meter center at (10, 45), with radius 50 pixels, and span from 135 to 45 degrees
    m->setMeter(10, 45, 50, 135, 45);

    // Set meter scale from 0 - 100, with the specified labels
    const char * pattern1[] = {"E", " ", " ", " ", "F"};
    m->setScale(0, 100, StringArray(pattern1, (int)(sizeof(pattern1) / sizeof(pattern1[0]))));

    // Set the angular arc and major tick width to 2 pixels
    m->setLineWidth(2, 2);

    // Add a red zone at 0 - 15
    m->addZone(0, 15, 0xff3333);

    // Add an icon at (25, 35)
    m->addText(25, 35, "<*img=gas.gif*>");

    // Add a yellow (ffff00) pointer at the specified value
    m->addPointer(value, 0xffff00);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *hlinearmeter(int /* chartIndex */, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 74.25;

    // Create an LinearMeter object of size 250 x 65 pixels with a very light grey (0xeeeeee)
    // background, and a rounded 3-pixel thick light grey (0xcccccc) border
    LinearMeter *m = new LinearMeter(250, 65, 0xeeeeee, 0xcccccc);
    m->setRoundedFrame(Chart::Transparent);
    m->setThickFrame(3);

    // Set the scale region top-left corner at (14, 23), with size of 218 x 20 pixels. The scale
    // labels are located on the top (implies horizontal meter)
    m->setMeter(14, 23, 218, 20, Chart::Top);

    // Set meter scale from 0 - 100, with a tick every 10 units
    m->setScale(0, 100, 10);

    // Add a smooth color scale to the meter
    double smoothColorScale[] = {0, 0x6666ff, 25, 0x00bbbb, 50, 0x00ff00, 75, 0xffff00, 100,
        0xff0000};
    m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
        smoothColorScale[0]))));

    // Add a blue (0x0000cc) pointer at the specified value
    m->addPointer(value, 0x0000cc);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *colorhlinearmeter(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 74.25;

    // The background and border colors of the meters
    int bgColor[] = {0xbbddff, 0xccffcc, 0xffccff, 0xffffaa, 0xffdddd, 0xeeeeee};
    int borderColor[] = {0x000088, 0x006600, 0x6600aa, 0xee6600, 0x880000, 0x666666};

    // Create a LinearMeter object of size 250 x 75 pixels with a 3-pixel thick rounded frame
    LinearMeter *m = new LinearMeter(250, 75, bgColor[chartIndex], borderColor[chartIndex]);
    m->setRoundedFrame(Chart::Transparent);
    m->setThickFrame(3);

    // Set the scale region top-left corner at (14, 23), with size of 218 x 20 pixels. The scale
    // labels are located on the top (implies horizontal meter)
    m->setMeter(14, 23, 218, 20, Chart::Top);

    // Set meter scale from 0 - 100, with a tick every 10 units
    m->setScale(0, 100, 10);

    // Demostrate different types of color scales and putting them at different positions
    double smoothColorScale[] = {0, 0x6666ff, 25, 0x00bbbb, 50, 0x00ff00, 75, 0xffff00, 100,
        0xff0000};
    double stepColorScale[] = {0, 0x33ff33, 50, 0xffff33, 80, 0xff3333, 100};
    double highLowColorScale[] = {0, 0x6666ff, 70, Chart::Transparent, 100, 0xff0000};

    if (chartIndex == 0) {
        // Add the smooth color scale at the default position
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))));
    } else if (chartIndex == 1) {
        // Add the step color scale at the default position
        m->addColorScale(DoubleArray(stepColorScale, (int)(sizeof(stepColorScale) / sizeof(
            stepColorScale[0]))));
    } else if (chartIndex == 2) {
        // Add the high low scale at the default position
        m->addColorScale(DoubleArray(highLowColorScale, (int)(sizeof(highLowColorScale) / sizeof(
            highLowColorScale[0]))));
    } else if (chartIndex == 3) {
        // Add the smooth color scale starting at y = 23 (top of scale) with zero width and ending
        // at y = 23 with 20 pixels width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 23, 0, 23, 20);
    } else if (chartIndex == 4) {
        // Add the smooth color scale starting at y = 33 (center of scale) with zero width and
        // ending at y = 23 with 20 pixels width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 33, 0, 23, 20);
    } else {
        // Add the smooth color scale starting at y = 43 (bottom of scale) with zero width and
        // ending at y = 23 with 20 pixels width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 43, 0, 23, 20);
    }

    // Add a blue (0x0000cc) pointer at the specified value
    m->addPointer(value, 0x0000cc);

    // Add a label left aligned to (10, 61) using 8pt Arial Bold font
    m->addText(10, 61, "Temperature C", "arialbd.ttf", 8, Chart::TextColor, Chart::Left);

    // Add a text box right aligned to (235, 61). Display the value using white (0xffffff) 8pt Arial
    // Bold font on a black (0x000000) background with depressed rounded border.
    TextBox *t = m->addText(235, 61, m->formatValue(value, "2"), "arialbd.ttf", 8, 0xffffff,
        Chart::Right);
    t->setBackground(0x000000, 0x000000, -1);
    t->setRoundedCorners(3);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *blackhlinearmeter(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 75.35;

    // Create a LinearMeter object of size 250 x 75 pixels with black background and rounded corners
    LinearMeter *m = new LinearMeter(250, 75, 0x000000);
    m->setRoundedFrame(Chart::Transparent);

    // Set the default text and line colors to white (0xffffff)
    m->setColor(Chart::TextColor, 0xffffff);
    m->setColor(Chart::LineColor, 0xffffff);

    // Set the scale region top-left corner at (14, 23), with size of 218 x 20 pixels. The scale
    // labels are located on the top (implies horizontal meter)
    m->setMeter(14, 23, 218, 20, Chart::Top);

    // Set meter scale from 0 - 100, with a tick every 10 units
    m->setScale(0, 100, 10);

    // The tick line width to 1 pixel
    m->setLineWidth(0, 1);

    // Demostrate different types of color scales and putting them at different positions
    double smoothColorScale[] = {0, 0x0000ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xdddd00, 100,
        0xff0000};
    double stepColorScale[] = {0, 0x00cc00, 50, 0xeecc00, 80, 0xdd0000, 100};
    double highLowColorScale[] = {0, 0x0000ff, 70, Chart::Transparent, 100, 0xff0000};

    if (chartIndex == 0) {
        // Add the smooth color scale at the default position
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))));
    } else if (chartIndex == 1) {
        // Add the smooth color scale starting at y = 23 (top of scale) with zero width and ending
        // at y = 23 with 20 pixels width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 23, 0, 23, 20);
    } else if (chartIndex == 2) {
        // Add the high low scale at the default position
        m->addColorScale(DoubleArray(highLowColorScale, (int)(sizeof(highLowColorScale) / sizeof(
            highLowColorScale[0]))));
    } else if (chartIndex == 3) {
        // Add the smooth color scale starting at y = 33 (center of scale) with zero width and
        // ending at y = 23 with 20 pixels width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 33, 0, 23, 20);
    } else if (chartIndex == 4) {
        // Add the step color scale at the default position
        m->addColorScale(DoubleArray(stepColorScale, (int)(sizeof(stepColorScale) / sizeof(
            stepColorScale[0]))));
    } else {
        // Add the smooth color scale starting at y = 43 (bottom of scale) with zero width and
        // ending at y = 23 with 20 pixels width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 43, 0, 23, 20);
    }

    // Add a blue (0x0000cc) pointer with white (0xffffff) border at the specified value
    m->addPointer(value, 0x0000cc, 0xffffff);

    // Add a label left aligned to (10, 61) using 8pt Arial Bold font
    m->addText(10, 61, "Temperature C", "arialbd.ttf", 8, Chart::TextColor, Chart::Left);

    // Add a text box right aligned to (235, 61). Display the value using white (0xffffff) 8pt Arial
    // Bold font on a black (0x000000) background with depressed grey (0x444444) rounded border.
    TextBox *t = m->addText(235, 61, m->formatValue(value, "2"), "arialbd.ttf", 8, 0xffffff,
        Chart::Right);
    t->setBackground(0x000000, 0x444444, -1);
    t->setRoundedCorners(3);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *whitehlinearmeter(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 74.25;

    // Create a LinearMeter object of size 250 x 75 pixels with very light grey (0xeeeeee)
    // backgruond and a light grey (0xccccccc) 3-pixel thick rounded frame
    LinearMeter *m = new LinearMeter(250, 75, 0xeeeeee, 0xcccccc);
    m->setRoundedFrame(Chart::Transparent);
    m->setThickFrame(3);

    // Set the scale region top-left corner at (14, 23), with size of 218 x 20 pixels. The scale
    // labels are located on the top (implies horizontal meter)
    m->setMeter(14, 23, 218, 20, Chart::Top);

    // Set meter scale from 0 - 100, with a tick every 10 units
    m->setScale(0, 100, 10);

    // Demostrate different types of color scales and putting them at different positions
    double smoothColorScale[] = {0, 0x6666ff, 25, 0x00bbbb, 50, 0x00ff00, 75, 0xffff00, 100,
        0xff0000};
    double stepColorScale[] = {0, 0x33ff33, 50, 0xffff33, 80, 0xff3333, 100};
    double highLowColorScale[] = {0, 0x6666ff, 70, Chart::Transparent, 100, 0xff0000};

    if (chartIndex == 0) {
        // Add the smooth color scale at the default position
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))));
    } else if (chartIndex == 1) {
        // Add the high low scale at the default position
        m->addColorScale(DoubleArray(highLowColorScale, (int)(sizeof(highLowColorScale) / sizeof(
            highLowColorScale[0]))));
    } else if (chartIndex == 2) {
        // Add the smooth color scale starting at y = 43 (bottom of scale) with zero width and
        // ending at y = 23 with 20 pixels width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 43, 0, 23, 20);
    } else {
        // Add the step color scale at the default position
        m->addColorScale(DoubleArray(stepColorScale, (int)(sizeof(stepColorScale) / sizeof(
            stepColorScale[0]))));
    }

    // Add a blue (0x0000cc) pointer at the specified value
    m->addPointer(value, 0x0000cc);

    // Add a label left aligned to (10, 61) using 8pt Arial Bold font
    m->addText(10, 61, "Temperature C", "arialbd.ttf", 8, Chart::TextColor, Chart::Left);

    // Add a text box right aligned to (235, 61). Display the value using white (0xffffff) 8pt Arial
    // Bold font on a black (0x000000) background with depressed rounded border.
    TextBox *t = m->addText(235, 61, m->formatValue(value, "2"), "arialbd.ttf", 8, 0xffffff,
        Chart::Right);
    t->setBackground(0x000000, 0x000000, -1);
    t->setRoundedCorners(3);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *hlinearmeterorientation(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 74.25;

    // Create a LinearMeter object of size 250 x 75 pixels with very light grey (0xeeeeee)
    // backgruond and a light grey (0xccccccc) 3-pixel thick rounded frame
    LinearMeter *m = new LinearMeter(250, 75, 0xeeeeee, 0xcccccc);
    m->setRoundedFrame(Chart::Transparent);
    m->setThickFrame(3);

    // This example demonstrates putting the text labels at the top or bottom. This is by setting
    // the label alignment, scale position and label position.
    int alignment[] = {Chart::Top, Chart::Top, Chart::Bottom, Chart::Bottom};
    int meterYPos[] = {23, 23, 34, 34};
    int labelYPos[] = {61, 61, 15, 15};

    // Set the scale region
    m->setMeter(14, meterYPos[chartIndex], 218, 20, alignment[chartIndex]);

    // Set meter scale from 0 - 100, with a tick every 10 units
    m->setScale(0, 100, 10);

    // Add a smooth color scale at the default position
    double smoothColorScale[] = {0, 0x6666ff, 25, 0x00bbbb, 50, 0x00ff00, 75, 0xffff00, 100,
        0xff0000};
    m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
        smoothColorScale[0]))));

    // Add a blue (0x0000cc) pointer at the specified value
    m->addPointer(value, 0x0000cc);

    //
    // In this example, some charts have the "Temperauture" label on the left side and the value
    // readout on the right side, and some charts have the reverse
    //

    if (chartIndex % 2 == 0) {
        // Add a label on the left side using 8pt Arial Bold font
        m->addText(10, labelYPos[chartIndex], "Temperature C", "arialbd.ttf", 8, Chart::TextColor,
            Chart::Left);

        // Add a text box on the right side. Display the value using white (0xffffff) 8pt Arial Bold
        // font on a black (0x000000) background with depressed rounded border.
        TextBox *t = m->addText(235, labelYPos[chartIndex], m->formatValue(value, "2"),
            "arialbd.ttf", 8, 0xffffff, Chart::Right);
        t->setBackground(0x000000, 0x000000, -1);
        t->setRoundedCorners(3);
    } else {
        // Add a label on the right side using 8pt Arial Bold font
        m->addText(237, labelYPos[chartIndex], "Temperature C", "arialbd.ttf", 8, Chart::TextColor,
            Chart::Right);

        // Add a text box on the left side. Display the value using white (0xffffff) 8pt Arial Bold
        // font on a black (0x000000) background with depressed rounded border.
        TextBox *t = m->addText(11, labelYPos[chartIndex], m->formatValue(value, "2"),
            "arialbd.ttf", 8, 0xffffff, Chart::Left);
        t->setBackground(0x000000, 0x000000, -1);
        t->setRoundedCorners(3);
    }

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *vlinearmeter(int /* chartIndex */, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 74.35;

    // Create an LinearMeter object of size 70 x 240 pixels with a very light grey (0xeeeeee)
    // background, and a rounded 3-pixel thick light grey (0xcccccc) border
    LinearMeter *m = new LinearMeter(70, 240, 0xeeeeee, 0xcccccc);
    m->setRoundedFrame(Chart::Transparent);
    m->setThickFrame(3);

    // Set the scale region top-left corner at (28, 18), with size of 20 x 205 pixels. The scale
    // labels are located on the left (default - implies vertical meter).
    m->setMeter(28, 18, 20, 205);

    // Set meter scale from 0 - 100, with a tick every 10 units
    m->setScale(0, 100, 10);

    // Add a smooth color scale to the meter
    double smoothColorScale[] = {0, 0x6666ff, 25, 0x00bbbb, 50, 0x00ff00, 75, 0xffff00, 100,
        0xff0000};
    m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
        smoothColorScale[0]))));

    // Add a blue (0x0000cc) pointer at the specified value
    m->addPointer(value, 0x0000cc);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *colorvlinearmeter(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 74.25;

    // The background and border colors of the meters
    int bgColor[] = {0xbbddff, 0xccffcc, 0xffccff, 0xffffaa, 0xffdddd, 0xeeeeee};
    int borderColor[] = {0x000088, 0x006600, 0x6600aa, 0xee6600, 0x880000, 0x666666};

    // Create a LinearMeter object of size 70 x 260 pixels with a 3-pixel thick rounded frame
    LinearMeter *m = new LinearMeter(70, 260, bgColor[chartIndex], borderColor[chartIndex]);
    m->setRoundedFrame(Chart::Transparent);
    m->setThickFrame(3);

    // Set the scale region top-left corner at (28, 30), with size of 20 x 200 pixels. The scale
    // labels are located on the left (default - implies vertical meter)
    m->setMeter(28, 30, 20, 200);

    // Set meter scale from 0 - 100, with a tick every 10 units
    m->setScale(0, 100, 10);

    // Demostrate different types of color scales and putting them at different positions
    double smoothColorScale[] = {0, 0x6666ff, 25, 0x00bbbb, 50, 0x00ff00, 75, 0xffff00, 100,
        0xff0000};
    double stepColorScale[] = {0, 0x33ff33, 50, 0xffff33, 80, 0xff3333, 100};
    double highLowColorScale[] = {0, 0x6666ff, 70, Chart::Transparent, 100, 0xff0000};

    if (chartIndex == 0) {
        // Add the smooth color scale at the default position
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))));
    } else if (chartIndex == 1) {
        // Add the step color scale at the default position
        m->addColorScale(DoubleArray(stepColorScale, (int)(sizeof(stepColorScale) / sizeof(
            stepColorScale[0]))));
    } else if (chartIndex == 2) {
        // Add the high low scale at the default position
        m->addColorScale(DoubleArray(highLowColorScale, (int)(sizeof(highLowColorScale) / sizeof(
            highLowColorScale[0]))));
    } else if (chartIndex == 3) {
        // Add the smooth color scale starting at x = 28 (left of scale) with zero width and ending
        // at x = 28 with 20 pixels width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 28, 0, 28, 20);
    } else if (chartIndex == 4) {
        // Add the smooth color scale starting at x = 38 (center of scale) with zero width and
        // ending at x = 28 with 20 pixels width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 38, 0, 28, 20);
    } else {
        // Add the smooth color scale starting at x = 48 (right of scale) with zero width and ending
        // at x = 28 with 20 pixels width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 48, 0, 28, 20);
    }

    // Add a blue (0x0000cc) pointer at the specified value
    m->addPointer(value, 0x0000cc);

    // Add a title using white (0xffffff) 8pt Arial Bold font with a border color background
    m->addTitle("Temp C", "arialbd.ttf", 8, 0xffffff)->setBackground(borderColor[chartIndex]);

    // Add a bottom title using white (0xffffff) 8pt Arial Bold font with a border color background
    // to display the value
    m->addTitle(Chart::Bottom, m->formatValue(value, "2"), "arialbd.ttf", 8, 0xffffff
        )->setBackground(borderColor[chartIndex]);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *blackvlinearmeter(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 74.25;

    // Create a LinearMeter object of size 70 x 260 pixels with black background and rounded corners
    LinearMeter *m = new LinearMeter(70, 260, 0x000000);
    m->setRoundedFrame(Chart::Transparent);

    // Set the default text and line colors to white (0xffffff)
    m->setColor(Chart::TextColor, 0xffffff);
    m->setColor(Chart::LineColor, 0xffffff);

    // Set the scale region top-left corner at (28, 30), with size of 20 x 196 pixels. The scale
    // labels are located on the left (default - implies vertical meter)
    m->setMeter(28, 30, 20, 196);

    // Set meter scale from 0 - 100, with a tick every 10 units
    m->setScale(0, 100, 10);

    // The tick line width to 1 pixel
    m->setLineWidth(0, 1);

    // Demostrate different types of color scales and putting them at different positions
    double smoothColorScale[] = {0, 0x0000ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xdddd00, 100,
        0xff0000};
    double stepColorScale[] = {0, 0x00cc00, 50, 0xeecc00, 80, 0xdd0000, 100};
    double highLowColorScale[] = {0, 0x0000ff, 70, Chart::Transparent, 100, 0xff0000};

    if (chartIndex == 0) {
        // Add the smooth color scale at the default position
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))));
    } else if (chartIndex == 1) {
        // Add the step color scale at the default position
        m->addColorScale(DoubleArray(stepColorScale, (int)(sizeof(stepColorScale) / sizeof(
            stepColorScale[0]))));
    } else if (chartIndex == 2) {
        // Add the high low scale at the default position
        m->addColorScale(DoubleArray(highLowColorScale, (int)(sizeof(highLowColorScale) / sizeof(
            highLowColorScale[0]))));
    } else if (chartIndex == 3) {
        // Add the smooth color scale starting at x = 28 (left of scale) with zero width and ending
        // at x = 28 with 20 pixels width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 28, 0, 28, 20);
    } else if (chartIndex == 4) {
        // Add the smooth color scale starting at x = 38 (center of scale) with zero width and
        // ending at x = 28 with 20 pixels width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 38, 0, 28, 20);
    } else {
        // Add the smooth color scale starting at x = 48 (right of scale) with zero width and ending
        // at x = 28 with 20 pixels width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 48, 0, 28, 20);
    }

    // Add a blue (0x0000cc) pointer with white (0xffffff) border at the specified value
    m->addPointer(value, 0x0000cc, 0xffffff);

    // Add a label at the top-center using 8pt Arial Bold font
    m->addText(m->getWidth() / 2, 5, "Temp C", "arialbd.ttf", 8, Chart::TextColor, Chart::Top);

    // Add a text box at the bottom-center. Display the value using white (0xffffff) 8pt Arial Bold
    // font on a black (0x000000) background with depressed grey (0x444444) rounded border.
    TextBox *t = m->addText(m->getWidth() / 2, m->getHeight() - 7, m->formatValue(value, "2"),
        "arialbd.ttf", 8, 0xffffff, Chart::Bottom);
    t->setBackground(0x000000, 0x444444, -1);
    t->setRoundedCorners(3);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *whitevlinearmeter(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 75.35;

    // Create a LinearMeter object of size 250 x 75 pixels with very light grey (0xeeeeee)
    // backgruond and a light grey (0xccccccc) 3-pixel thick rounded frame
    LinearMeter *m = new LinearMeter(70, 260, 0xeeeeee, 0xcccccc);
    m->setRoundedFrame(Chart::Transparent);
    m->setThickFrame(3);

    // Set the scale region top-left corner at (28, 30), with size of 20 x 196 pixels. The scale
    // labels are located on the left (default - implies vertical meter)
    m->setMeter(28, 30, 20, 196);

    // Set meter scale from 0 - 100, with a tick every 10 units
    m->setScale(0, 100, 10);

    // Demostrate different types of color scales and putting them at different positions
    double smoothColorScale[] = {0, 0x6666ff, 25, 0x00bbbb, 50, 0x00ff00, 75, 0xffff00, 100,
        0xff0000};
    double stepColorScale[] = {0, 0x33ff33, 50, 0xffff33, 80, 0xff3333, 100};
    double highLowColorScale[] = {0, 0x6666ff, 70, Chart::Transparent, 100, 0xff0000};

    if (chartIndex == 0) {
        // Add the smooth color scale at the default position
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))));
    } else if (chartIndex == 1) {
        // Add the step color scale at the default position
        m->addColorScale(DoubleArray(stepColorScale, (int)(sizeof(stepColorScale) / sizeof(
            stepColorScale[0]))));
    } else if (chartIndex == 2) {
        // Add the high low scale at the default position
        m->addColorScale(DoubleArray(highLowColorScale, (int)(sizeof(highLowColorScale) / sizeof(
            highLowColorScale[0]))));
    } else if (chartIndex == 3) {
        // Add the smooth color scale starting at x = 28 (left of scale) with zero width and ending
        // at x = 28 with 20 pixels width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 28, 0, 28, 20);
    } else if (chartIndex == 4) {
        // Add the smooth color scale starting at x = 38 (center of scale) with zero width and
        // ending at x = 28 with 20 pixels width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 38, 0, 28, 20);
    } else {
        // Add the smooth color scale starting at x = 48 (right of scale) with zero width and ending
        // at x = 28 with 20 pixels width
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 48, 0, 28, 20);
    }

    // In this demo, we demostrate pointers of different shapes
    if (chartIndex < 3) {
        // Add a blue (0x0000cc) pointer of default shape at the specified value
        m->addPointer(value, 0x0000cc);
    } else {
        // Add a blue (0x0000cc) pointer of triangular shape the specified value
        m->addPointer(value, 0x0000cc)->setShape(Chart::TriangularPointer, 0.7, 0.5);
    }

    // Add a title using 8pt Arial Bold font with a border color background
    m->addTitle("Temp C", "arialbd.ttf", 8, Chart::TextColor)->setBackground(0xcccccc);

    // Add a text box at the bottom-center. Display the value using white (0xffffff) 8pt Arial Bold
    // font on a black (0x000000) background with rounded border.
    TextBox *t = m->addText(m->getWidth() / 2, m->getHeight() - 8, m->formatValue(value, "2"),
        "arialbd.ttf", 8, 0xffffff, Chart::Bottom);
    t->setBackground(0x000000);
    t->setRoundedCorners(3);
    t->setMargin(5, 5, 2, 1);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *vlinearmeterorientation(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 75.35;

    // Create a LinearMeter object of size 70 x 240 pixels with very light grey (0xeeeeee)
    // backgruond and a light grey (0xccccccc) 3-pixel thick rounded frame
    LinearMeter *m = new LinearMeter(70, 240, 0xeeeeee, 0xcccccc);
    m->setRoundedFrame(Chart::Transparent);
    m->setThickFrame(3);

    // This example demonstrates putting the text labels at the left or right side by setting the
    // label alignment and scale position.
    if (chartIndex == 0) {
        m->setMeter(28, 18, 20, 205, Chart::Left);
    } else {
        m->setMeter(20, 18, 20, 205, Chart::Right);
    }

    // Set meter scale from 0 - 100, with a tick every 10 units
    m->setScale(0, 100, 10);

    // Add a smooth color scale to the meter
    double smoothColorScale[] = {0, 0x6666ff, 25, 0x00bbbb, 50, 0x00ff00, 75, 0xffff00, 100,
        0xff0000};
    m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
        smoothColorScale[0]))));

    // Add a blue (0x0000cc) pointer at the specified value
    m->addPointer(value, 0x0000cc);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *multihmeter(int /* chartIndex */, const char ** /* imageMap */)
{
    // The values to display on the meter
    double value0 = 30.99;
    double value1 = 45.35;
    double value2 = 77.64;

    // Create an LinearMeter object of size 250 x 75 pixels, using silver background with a 2 pixel
    // black 3D depressed border.
    LinearMeter *m = new LinearMeter(250, 75, Chart::silverColor(), 0, -2);

    // Set the scale region top-left corner at (15, 25), with size of 220 x 20 pixels. The scale
    // labels are located on the top (implies horizontal meter)
    m->setMeter(15, 25, 220, 20, Chart::Top);

    // Set meter scale from 0 - 100, with a tick every 10 units
    m->setScale(0, 100, 10);

    // Set 0 - 50 as green (99ff99) zone, 50 - 80 as yellow (ffff66) zone, and 80 - 100 as red
    // (ffcccc) zone
    m->addZone(0, 50, 0x99ff99);
    m->addZone(50, 80, 0xffff66);
    m->addZone(80, 100, 0xffcccc);

    // Add deep red (000080), deep green (008000) and deep blue (800000) pointers to reflect the
    // values
    m->addPointer(value0, 0x000080);
    m->addPointer(value1, 0x008000);
    m->addPointer(value2, 0x800000);

    // Add a label at bottom-left (10, 68) using Arial Bold/8pt/red (c00000)
    m->addText(10, 68, "Temp C", "arialbd.ttf", 8, 0xc00000, Chart::BottomLeft);

    // Add three text boxes to show the values in this meter
    m->addText(148, 70, m->formatValue(value0, "2"), "arial.ttf", 8, 0x6666ff, Chart::BottomRight
        )->setBackground(0, 0, -1);
    m->addText(193, 70, m->formatValue(value1, "2"), "arial.ttf", 8, 0x33ff33, Chart::BottomRight
        )->setBackground(0, 0, -1);
    m->addText(238, 70, m->formatValue(value2, "2"), "arial.ttf", 8, 0xff3333, Chart::BottomRight
        )->setBackground(0, 0, -1);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *multivmeter(int /* chartIndex */, const char ** /* imageMap */)
{
    // The values to display on the meter
    double value0 = 30.99;
    double value1 = 45.35;
    double value2 = 77.64;

    // Create an LinearMeter object of size 60 x 245 pixels, using silver background with a 2 pixel
    // black 3D depressed border.
    LinearMeter *m = new LinearMeter(60, 245, Chart::silverColor(), 0, -2);

    // Set the scale region top-left corner at (25, 30), with size of 20 x 200 pixels. The scale
    // labels are located on the left (default - implies vertical meter)
    m->setMeter(25, 30, 20, 200);

    // Set meter scale from 0 - 100, with a tick every 10 units
    m->setScale(0, 100, 10);

    // Set 0 - 50 as green (99ff99) zone, 50 - 80 as yellow (ffff66) zone, and 80 - 100 as red
    // (ffcccc) zone
    m->addZone(0, 50, 0x99ff99);
    m->addZone(50, 80, 0xffff66);
    m->addZone(80, 100, 0xffcccc);

    // Add deep red (000080), deep green (008000) and deep blue (800000) pointers to reflect the
    // values
    m->addPointer(value0, 0x000080);
    m->addPointer(value1, 0x008000);
    m->addPointer(value2, 0x800000);

    // Add a text box label at top-center (30, 5) using Arial Bold/8pt/deep blue (000088), with a
    // light blue (9999ff) background
    m->addText(30, 5, "Temp C", "arialbd.ttf", 8, 0x000088, Chart::TopCenter)->setBackground(
        0x9999ff);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *linearzonemeter(int /* chartIndex */, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 85;

    // Create an LinearMeter object of size 210 x 45 pixels, using silver background with a 2 pixel
    // black 3D depressed border.
    LinearMeter *m = new LinearMeter(210, 45, Chart::silverColor(), 0, -2);

    // Set the scale region top-left corner at (5, 5), with size of 200 x 20 pixels. The scale
    // labels are located on the bottom (implies horizontal meter)
    m->setMeter(5, 5, 200, 20, Chart::Bottom);

    // Set meter scale from 0 - 100
    m->setScale(0, 100);

    // Add a title at the bottom of the meter with a 1 pixel raised 3D border
    m->addTitle(Chart::Bottom, "Battery Level", "arialbd.ttf", 8)->setBackground(Chart::Transparent,
        -1, 1);

    // Set 3 zones of different colors to represent Good/Weak/Bad data ranges
    m->addZone(50, 100, 0x99ff99, "Good");
    m->addZone(20, 50, 0xffff66, "Weak");
    m->addZone(0, 20, 0xffcccc, "Bad");

    // Add empty labels (just need the ticks) at 0/20/50/80 as separators for zones
    m->addLabel(0, " ");
    m->addLabel(20, " ");
    m->addLabel(50, " ");
    m->addLabel(100, " ");

    // Add a semi-transparent blue (800000ff) pointer at the specified value, using triangular
    // pointer shape
    m->addPointer(value, 0x800000ff)->setShape(Chart::TriangularPointer);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *hbarmeter(int /* chartIndex */, const char ** /* imageMap */)
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
    m->makeChart();

    return m;
}


BaseChart *colorhbarmeter(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 75.35;

    // The background, border and bar colors of the meters
    int bgColor[] = {0xbbddff, 0xccffcc, 0xffddff, 0xffffaa, 0xffdddd, 0xeeeeee};
    int borderColor[] = {0x000088, 0x006600, 0x880088, 0xee6600, 0x880000, 0x666666};
    int barColor[] = {0x0088ff, 0x00cc00, 0x8833dd, 0xff8800, 0xee3333, 0x888888};

    // Create a LinearMeter object of size 260 x 80 pixels with a 3-pixel thick rounded frame
    LinearMeter *m = new LinearMeter(260, 80, bgColor[chartIndex], borderColor[chartIndex]);
    m->setRoundedFrame(Chart::Transparent);
    m->setThickFrame(3);

    // Set the scale region top-left corner at (18, 24), with size of 222 x 20 pixels. The scale
    // labels are located on the top (implies horizontal meter)
    m->setMeter(18, 24, 222, 20, Chart::Top);

    // Set meter scale from 0 - 100, with a tick every 10 units
    m->setScale(0, 100, 10);

    if (chartIndex % 4 == 0) {
        // Add a 5-pixel thick smooth color scale at y = 48 (below the meter scale)
        double smoothColorScale[] = {0, 0x0000ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xdddd00, 100,
            0xff0000};
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 48, 5);
    } else if (chartIndex % 4 == 1) {
        // Add a 5-pixel thick step color scale at y = 48 (below the meter scale)
        double stepColorScale[] = {0, 0x00cc00, 50, 0xffdd00, 80, 0xff3333, 100};
        m->addColorScale(DoubleArray(stepColorScale, (int)(sizeof(stepColorScale) / sizeof(
            stepColorScale[0]))), 48, 5);
    } else if (chartIndex % 4 == 2) {
        // Add a 5-pixel thick high/low color scale at y = 48 (below the meter scale)
        double highLowColorScale[] = {0, 0x0000ff, 40, Chart::Transparent, 60, Chart::Transparent,
            100, 0xff0000};
        m->addColorScale(DoubleArray(highLowColorScale, (int)(sizeof(highLowColorScale) / sizeof(
            highLowColorScale[0]))), 48, 5);
    } else {
        // Add a 5-pixel thick high only color scale at y = 48 (below the meter scale)
        double highColorScale[] = {70, Chart::Transparent, 100, 0xff0000};
        m->addColorScale(DoubleArray(highColorScale, (int)(sizeof(highColorScale) / sizeof(
            highColorScale[0]))), 48, 0, 48, 8);
    }

    // Add a bar from 0 to value with glass effect and 4 pixel rounded corners
    m->addBar(0, value, barColor[chartIndex], Chart::glassEffect(Chart::NormalGlare, Chart::Top), 4)
        ;

    // Add a label right aligned to (243, 65) using 8pt Arial Bold font
    m->addText(243, 65, "Temperature C", "arialbd.ttf", 8, Chart::TextColor, Chart::Right);

    // Add a text box left aligned to (18, 65). Display the value using white (0xffffff) 8pt Arial
    // Bold font on a black (0x000000) background with depressed rounded border.
    TextBox *t = m->addText(18, 65, m->formatValue(value, "2"), "arial.ttf", 8, 0xffffff,
        Chart::Left);
    t->setBackground(0x000000, 0x000000, -1);
    t->setRoundedCorners(3);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *blackhbarmeter(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 75.35;

    // Create a LinearMeter object of size 260 x 80 pixels with black background and rounded corners
    LinearMeter *m = new LinearMeter(260, 80, 0x000000);
    m->setRoundedFrame(Chart::Transparent);

    // Set the default text and line colors to white (0xffffff)
    m->setColor(Chart::TextColor, 0xffffff);
    m->setColor(Chart::LineColor, 0xffffff);

    // Set the scale region top-left corner at (15, 24), with size of 228 x 20 pixels. The scale
    // labels are located on the top (implies horizontal meter)
    m->setMeter(15, 24, 228, 20, Chart::Top);

    // Set meter scale from 0 - 100, with a tick every 10 units
    m->setScale(0, 100, 10);

    // Demostrate different types of color scales
    double smoothColorScale[] = {0, 0x0000ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xdddd00, 100,
        0xff0000};
    double stepColorScale[] = {0, 0x00dd00, 50, 0xffff00, 80, 0xff3333, 100};
    double highLowColorScale[] = {0, 0x0000ff, 40, Chart::Transparent, 60, Chart::Transparent, 100,
        0xff0000};
    double highColorScale[] = {70, Chart::Transparent, 100, 0xff0000};

    if (chartIndex == 0) {
        // Add a blue (0x0088ff) bar from 0 to value with glass effect and 4 pixel rounded corners
        m->addBar(0, value, 0x0088ff, Chart::glassEffect(Chart::NormalGlare, Chart::Top), 4);
        // Add a 5-pixel thick smooth color scale at y = 48 (below the meter scale)
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 48, 5);
    } else if (chartIndex == 1) {
        // Add a purple (0xaa66ff) bar from 0 to value
        m->addBar(0, value, 0xaa66ff);
        // Add a 4 pixel thick purple (0x880088) frame
        m->setThickFrame(4, 0x880088);
        // Add a 5-pixel thick high/low color scale at y = 48 (below the meter scale)
        m->addColorScale(DoubleArray(highLowColorScale, (int)(sizeof(highLowColorScale) / sizeof(
            highLowColorScale[0]))), 48, 5);
    } else if (chartIndex == 2) {
         // Add a green (0x00ee00) bar from 0 to value with right side soft lighting effect and 4
         // pixel rounded corners
        m->addBar(0, value, 0x00ee00, Chart::softLighting(Chart::Right), 4);
        // Add a 5-pixel thick step color scale at y = 48 (below the meter scale)
        m->addColorScale(DoubleArray(stepColorScale, (int)(sizeof(stepColorScale) / sizeof(
            stepColorScale[0]))), 48, 5);
    } else if (chartIndex == 3) {
          // Add an orange (0xff8800) bar from 0 to value with cylinder lighting effect
        m->addBar(0, value, 0xff8800, Chart::cylinderEffect());
        // Add a 4 pixel thick brown (0xbb5500) frame
        m->setThickFrame(4, 0xbb5500);
        // Add a 5-pixel thick high only color scale at y = 48 (below the meter scale)
        m->addColorScale(DoubleArray(highColorScale, (int)(sizeof(highColorScale) / sizeof(
            highColorScale[0]))), 48, 5);
    } else if (chartIndex == 4) {
         // Add a magneta (0xdd00dd) bar from 0 to value with top side soft lighting effect and 4
         // pixel rounded corners
        m->addBar(0, value, 0xdd00dd, Chart::softLighting(Chart::Top), 4);
        // Add a 5-pixel thick smooth color scale at y = 48 (below the meter scale)
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 48, 5);
    } else {
         // Add a red (0xff0000) bar from 0 to value with bar lighting effect
        m->addBar(0, value, 0xff0000, Chart::barLighting());
        // Add a 4 pixel thick red (0xaa0000) frame
        m->setThickFrame(4, 0xaa0000);
           // Add a 5-pixel thick high/low color scale at y = 48 (below the meter scale)
        m->addColorScale(DoubleArray(highLowColorScale, (int)(sizeof(highLowColorScale) / sizeof(
            highLowColorScale[0]))), 48, 5);
    }

    // Add a label left aligned to (12, 65) using 8pt Arial Bold font
    m->addText(12, 65, "Temperature C", "arialbd.ttf", 8, Chart::TextColor, Chart::Left);

    // Add a text box right aligned to (243, 65). Display the value using white (0xffffff) 8pt Arial
    // Bold font on a black (0x000000) background with depressed dark grey (0x333333) rounded
    // border.
    TextBox *t = m->addText(243, 65, m->formatValue(value, "2"), "arial.ttf", 8, 0xffffff,
        Chart::Right);
    t->setBackground(0x000000, 0x333333, -1);
    t->setRoundedCorners(3);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *whitehbarmeter(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 75.35;

    // Create a LinearMeter object of size 260 x 80 pixels with very light grey (0xeeeeee)
    // backgruond and a light grey (0xccccccc) 3-pixel thick rounded frame
    LinearMeter *m = new LinearMeter(260, 80, 0xeeeeee, 0xaaaaaa);
    m->setRoundedFrame(Chart::Transparent);
    m->setThickFrame(3);

    // Set the scale region top-left corner at (18, 24), with size of 222 x 20 pixels. The scale
    // labels are located on the top (implies horizontal meter)
    m->setMeter(18, 24, 222, 20, Chart::Top);

    // Set meter scale from 0 - 100, with a tick every 10 units
    m->setScale(0, 100, 10);

    // Demostrate different types of color scales
    double smoothColorScale[] = {0, 0x0000ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xdddd00, 100,
        0xff0000};
    double stepColorScale[] = {0, 0x00dd00, 50, 0xffff00, 80, 0xff0000, 100};
    double highLowColorScale[] = {0, 0x0000ff, 40, Chart::Transparent, 60, Chart::Transparent, 100,
        0xff0000};
    double highColorScale[] = {70, Chart::Transparent, 100, 0xff0000};

    if (chartIndex == 0) {
        // Add a blue (0x0088ff) bar from 0 to value with glass effect and 4 pixel rounded corners
        m->addBar(0, value, 0x0088ff, Chart::glassEffect(Chart::NormalGlare, Chart::Top), 4);
        // Add a 5-pixel thick smooth color scale at y = 48 (below the meter scale)
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 48, 5);
    } else if (chartIndex == 1) {
         // Add a green (0x00cc00) bar from 0 to value with bar lighting effect
        m->addBar(0, value, 0x00cc00, Chart::barLighting());
        // Add a 5-pixel thick step color scale at y = 48 (below the meter scale)
        m->addColorScale(DoubleArray(stepColorScale, (int)(sizeof(stepColorScale) / sizeof(
            stepColorScale[0]))), 48, 5);
    } else if (chartIndex == 2) {
        // Add a purple (0x8833dd) bar from 0 to value with glass effect and 4 pixel rounded corners
        m->addBar(0, value, 0x8833dd, Chart::glassEffect(Chart::NormalGlare, Chart::Top), 4);
        // Add a 5-pixel thick high/low color scale at y = 48 (below the meter scale)
        m->addColorScale(DoubleArray(highLowColorScale, (int)(sizeof(highLowColorScale) / sizeof(
            highLowColorScale[0]))), 48, 5);
    } else if (chartIndex == 3) {
          // Add an orange (0xff8800) bar from 0 to value with cylinder lighting effect
        m->addBar(0, value, 0xff8800, Chart::cylinderEffect());
        // Add a high only color scale at y = 48 (below the meter scale) with thickness varying from
        // 0 to 8
        m->addColorScale(DoubleArray(highColorScale, (int)(sizeof(highColorScale) / sizeof(
            highColorScale[0]))), 48, 0, 48, 8);
    } else if (chartIndex == 4) {
        // Add a red (0xee3333) bar from 0 to value with glass effect and 4 pixel rounded corners
        m->addBar(0, value, 0xee3333, Chart::glassEffect(Chart::NormalGlare, Chart::Top), 4);
        // Add a 5-pixel thick smooth color scale at y = 48 (below the meter scale)
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 48, 5);
    } else {
        // Add a grey (0xaaaaaa) bar from 0 to value
        m->addBar(0, value, 0xaaaaaa);
        // Add a 5-pixel thick step color scale at y = 48 (below the meter scale)
        m->addColorScale(DoubleArray(stepColorScale, (int)(sizeof(stepColorScale) / sizeof(
            stepColorScale[0]))), 48, 5);
    }

    // Add a label right aligned to (243, 65) using 8pt Arial Bold font
    m->addText(243, 65, "Temperature C", "arialbd.ttf", 8, Chart::TextColor, Chart::Right);

    // Add a text box left aligned to (18, 65). Display the value using white (0xffffff) 8pt Arial
    // Bold font on a black (0x000000) background with depressed rounded border.
    TextBox *t = m->addText(18, 65, m->formatValue(value, "2"), "arial.ttf", 8, 0xffffff,
        Chart::Left);
    t->setBackground(0x000000, 0x000000, -1);
    t->setRoundedCorners(3);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *hbarmeterorientation(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 75.35;

    // The bar colors for the meters
    int barColor[] = {0x0088ff, 0x00cc00, 0x8833dd, 0xff8800};

    // Create a LinearMeter object of size 260 x 80 pixels with very light grey (0xeeeeee)
    // backgruond and a light grey (0xccccccc) 3-pixel thick rounded frame
    LinearMeter *m = new LinearMeter(260, 80, 0xeeeeee, 0xcccccc);
    m->setRoundedFrame(Chart::Transparent);
    m->setThickFrame(3);

    // This example demonstrates putting the text labels at the top or bottom. This is by setting
    // the label alignment, as well as the scale, color scale and label positions.
    int alignment[] = {Chart::Top, Chart::Top, Chart::Bottom, Chart::Bottom};
    int meterYPos[] = {24, 24, 36, 36};
    int labelYPos[] = {65, 65, 16, 16};
    int colorScalePos[] = {48, 48, 27, 27};

    // Set the scale region
    m->setMeter(18, meterYPos[chartIndex], 222, 20, alignment[chartIndex]);

    // Set meter scale from 0 - 100, with a tick every 10 units
    m->setScale(0, 100, 10);

    // Add a 5-pixel thick smooth color scale
    double smoothColorScale[] = {0, 0x0000ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xdddd00, 100,
        0xff0000};
    m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
        smoothColorScale[0]))), colorScalePos[chartIndex], 5);

    // Add a bar from 0 to value with glass effect and 4 pixel rounded corners
    m->addBar(0, value, barColor[chartIndex], Chart::glassEffect(Chart::NormalGlare, Chart::Top), 4)
        ;

    //
    // In this example, some charts have the "Temperauture" label on the left side and the value
    // readout on the right side, and some charts have the reverse
    //

    if (chartIndex % 2 == 0) {
        // Add a label on the left side using 8pt Arial Bold font
        m->addText(15, labelYPos[chartIndex], "Temperature C", "arialbd.ttf", 8, Chart::TextColor,
            Chart::Left);

        // Add a text box on the right side. Display the value using white (0xffffff) 8pt Arial Bold
        // font on a black (0x000000) background with depressed rounded border.
        TextBox *t = m->addText(240, labelYPos[chartIndex], m->formatValue(value, "2"), "arial.ttf",
            8, 0xffffff, Chart::Right);
        t->setBackground(0x000000, 0x000000, -1);
        t->setRoundedCorners(3);
    } else {
        // Add a label on the right side using 8pt Arial Bold font
        m->addText(243, labelYPos[chartIndex], "Temperature C", "arialbd.ttf", 8, Chart::TextColor,
            Chart::Right);

        // Add a text box on the left side. Display the value using white (0xffffff) 8pt Arial Bold
        // font on a black (0x000000) background with depressed rounded border.
        TextBox *t = m->addText(18, labelYPos[chartIndex], m->formatValue(value, "2"), "arial.ttf",
            8, 0xffffff, Chart::Left);
        t->setBackground(0x000000, 0x000000, -1);
        t->setRoundedCorners(3);
    }

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *vbarmeter(int /* chartIndex */, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 66.77;

    // Create an LinearMeter object of size 70 x 240 pixels with a very light grey (0xeeeeee)
    // background, and a rounded 3-pixel thick light grey (0xbbbbbb) border
    LinearMeter *m = new LinearMeter(70, 240, 0xeeeeee, 0xbbbbbb);
    m->setRoundedFrame(Chart::Transparent);
    m->setThickFrame(3);

    // Set the scale region top-left corner at (28, 18), with size of 20 x 205 pixels. The scale
    // labels are located on the left (default - implies vertical meter).
    m->setMeter(28, 18, 20, 205);

    // Set meter scale from 0 - 100, with a tick every 10 units
    m->setScale(0, 100, 10);

    // Add a 5-pixel thick smooth color scale to the meter at x = 54 (right of meter scale)
    double smoothColorScale[] = {0, 0x0000ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xffff00, 100,
        0xff0000};
    m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
        smoothColorScale[0]))), 54, 5);

    // Add a light blue (0x0088ff) bar from 0 to the data value with glass effect and 4 pixel
    // rounded corners
    m->addBar(0, value, 0x0088ff, Chart::glassEffect(Chart::NormalGlare, Chart::Left), 4);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *colorvbarmeter(int chartIndex, const char ** /* imageMap */)
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
    m->makeChart();

    return m;
}


BaseChart *blackvbarmeter(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 66.77;

    // Create a LinearMeter object of size 70 x 260 pixels with black background and rounded corners
    LinearMeter *m = new LinearMeter(70, 260, 0x000000);
    m->setRoundedFrame(Chart::Transparent);

    // Set the default text and line colors to white (0xffffff)
    m->setColor(Chart::TextColor, 0xffffff);
    m->setColor(Chart::LineColor, 0xffffff);

    // Set the scale region top-left corner at (36, 30), with size of 20 x 196 pixels. The scale
    // labels are located on the left (default - implies vertical meter).
    m->setMeter(36, 30, 20, 196);

    // Set meter scale from 0 - 100, with a tick every 10 units
    m->setScale(0, 100, 10);

    // Add the chart title at the top center
    TextBox *title = m->addText(m->getWidth() / 2, 5, "Temp C", "arialbd.ttf", 8, Chart::TextColor,
        Chart::TopCenter);

    // Move the scale labels 8 pixels from the meter scale to make room for the color scale
    m->setLabelPos(false, 8);

    // Demostrate different types of color scales
    double smoothColorScale[] = {0, 0x0000ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xdddd00, 100,
        0xff0000};
    double stepColorScale[] = {0, 0x00dd00, 50, 0xffff00, 80, 0xff0000, 100};
    double highLowColorScale[] = {0, 0x0000ff, 40, Chart::Transparent, 60, Chart::Transparent, 100,
        0xff0000};
    double highColorScale[] = {70, Chart::Transparent, 100, 0xff0000};

    if (chartIndex == 0) {
        // Add a blue (0x0088ff) bar from 0 to value with glass effect and 4 pixel rounded corners
        m->addBar(0, value, 0x0088ff, Chart::glassEffect(Chart::NormalGlare, Chart::Left), 4);
        // Add a 4-pixel thick smooth color scale at x = 29 (left of meter scale)
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 29, 4);
    } else if (chartIndex == 1) {
         // Add a yellow (0xCC9922) bar from 0 to value with bar lighting effect
        m->addBar(0, value, 0xcc9922, Chart::barLighting());
        // Add a 4-pixel thick smooth color scale at x = 29 (left of meter scale)
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 29, 4);

        // Set the title style to black text on a yellow (0xcc9922) background with rounded corners
        title->setFontColor(0x000000);
        title->setBackground(0xcc9922);
        title->setRoundedCorners(2);
    } else if (chartIndex == 2) {
         // Add a grey (0xaaaaaa) bar from 0 to value with bar lighting effect
        m->addBar(0, value, 0xaaaaaa, Chart::barLighting());
        // Add a 4-pixel high/low color scale at x = 29 (left of meter scale)
        m->addColorScale(DoubleArray(highLowColorScale, (int)(sizeof(highLowColorScale) / sizeof(
            highLowColorScale[0]))), 29, 4);
    } else if (chartIndex == 3) {
         // Add a brown (0xbb6622) bar from 0 to value with cylinder lighting effect
        m->addBar(0, value, 0xbb6622, Chart::cylinderEffect());
        // Add a 4 pixel thick brown (0xbb6622) frame
        m->setThickFrame(4, 0xbb6622);
        // Add a high only color scale at x = 29 (left of meter scale)
        m->addColorScale(DoubleArray(highColorScale, (int)(sizeof(highColorScale) / sizeof(
            highColorScale[0]))), 29, 4);
    } else if (chartIndex == 4) {
        // Add a purple (0x7755ee) bar from 0 to value
        m->addBar(0, value, 0x7755ee);
        // Add a 4 pixel thick purple (0x880088) frame
        m->setThickFrame(4, 0x880088);
        // Add a 4-pixel high/low color scale at x = 29 (left of meter scale)
        m->addColorScale(DoubleArray(highLowColorScale, (int)(sizeof(highLowColorScale) / sizeof(
            highLowColorScale[0]))), 29, 4);
    } else {
         // Add a green (0x00bb00) bar from 0 to value with soft lighting effect and 4 pixel rounded
         // corners
        m->addBar(0, value, 0x00bb00, Chart::softLighting(), 4);
        // Add a 4-pixel step color scale at x = 29 (left of meter scale)
        m->addColorScale(DoubleArray(stepColorScale, (int)(sizeof(stepColorScale) / sizeof(
            stepColorScale[0]))), 29, 4);
    }

    // Add a text box at the bottom-center. Display the value using white (0xffffff) 8pt Arial Bold
    // font on a black (0x000000) background with depressed grey (0x333333) rounded border.
    TextBox *t = m->addText(m->getWidth() / 2, m->getHeight() - 7, m->formatValue(value, "2"),
        "arialbd.ttf", 8, 0xffffff, Chart::BottomCenter);
    t->setBackground(0x000000, 0x333333, -1);
    t->setRoundedCorners(3);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *whitevbarmeter(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 74.25;

    // Create a LinearMeter object of size 70 x 260 pixels with very light grey (0xeeeeee)
    // backgruond and a grey (0xbbbbbb) 3-pixel thick rounded frame
    LinearMeter *m = new LinearMeter(70, 260, 0xeeeeee, 0xbbbbbb);
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
    double stepColorScale[] = {0, 0x00dd00, 50, 0xffff00, 80, 0xff0000, 100};
    double highColorScale[] = {70, Chart::Transparent, 100, 0xff0000};
    double lowColorScale[] = {0, 0x0000ff, 30, Chart::Transparent};

    if (chartIndex == 0) {
        // Add a blue (0x0088ff) bar from 0 to value with glass effect and 4 pixel rounded corners
        m->addBar(0, value, 0x0088ff, Chart::glassEffect(Chart::NormalGlare, Chart::Left), 4);
        // Add a 6-pixel thick smooth color scale at x = 53 (right of meter scale)
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 53, 6);
    } else if (chartIndex == 1) {
         // Add a green (0x00cc00) bar from 0 to value with bar lighting effect and 4 pixel rounded
         // corners
        m->addBar(0, value, 0x00cc00, Chart::barLighting(), 4);
        // Add a high only color scale at x = 52 (right of meter scale) with thickness varying from
        // 0 to 8
        m->addColorScale(DoubleArray(highColorScale, (int)(sizeof(highColorScale) / sizeof(
            highColorScale[0]))), 52, 0, 52, 8);
        // Add a low only color scale at x = 52 (right of meter scale) with thickness varying from 8
        // to 0
        m->addColorScale(DoubleArray(lowColorScale, (int)(sizeof(lowColorScale) / sizeof(
            lowColorScale[0]))), 52, 8, 52, 0);
    } else if (chartIndex == 2) {
        // Add a purple (0x0088ff) bar from 0 to value with glass effect and 4 pixel rounded corners
        m->addBar(0, value, 0x8833dd, Chart::glassEffect(Chart::NormalGlare, Chart::Left), 4);
        // Add a high only color scale at x = 52 (right of meter scale) with thickness varying from
        // 0 to 8
        m->addColorScale(DoubleArray(highColorScale, (int)(sizeof(highColorScale) / sizeof(
            highColorScale[0]))), 52, 0, 52, 8);
    } else if (chartIndex == 3) {
         // Add a orange (0xff8800) bar from 0 to value with cylinder lighting effect
        m->addBar(0, value, 0xff8800, Chart::cylinderEffect());
        // Add a high only color scale at x = 53 (right of meter scale)
        m->addColorScale(DoubleArray(highColorScale, (int)(sizeof(highColorScale) / sizeof(
            highColorScale[0]))), 53, 6);
    } else if (chartIndex == 4) {
        // Add a red (0xee3333) bar from 0 to value with glass effect and 4 pixel rounded corners
        m->addBar(0, value, 0xee3333, Chart::glassEffect(Chart::NormalGlare, Chart::Left), 4);
        // Add a step color scale at x = 53 (right of meter scale)
        m->addColorScale(DoubleArray(stepColorScale, (int)(sizeof(stepColorScale) / sizeof(
            stepColorScale[0]))), 53, 6);
    } else {
         // Add a grey (0xaaaaaa) bar from 0 to value
        m->addBar(0, value, 0xaaaaaa);
        // Add a smooth color scale at x = 52 (right of meter scale) with thickness varying from 0
        // to 8
        m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
            smoothColorScale[0]))), 52, 0, 52, 8);
    }

    // Add a title using 8pt Arial Bold font with grey (0xbbbbbb) background
    m->addTitle("Temp C", "arialbd.ttf", 8, Chart::TextColor)->setBackground(0xbbbbbb);

    // Add a text box at the bottom-center. Display the value using white (0xffffff) 8pt Arial Bold
    // font on a black (0x000000) background with rounded border.
    TextBox *t = m->addText(m->getWidth() / 2, m->getHeight() - 8, m->formatValue(value, "2"),
        "arialbd.ttf", 8, 0xffffff, Chart::Bottom);
    t->setBackground(0x000000);
    t->setRoundedCorners(3);
    t->setMargin(5, 5, 2, 1);

    // Output the chart
    m->makeChart();

    return m;
}


BaseChart *vbarmeterorientation(int chartIndex, const char ** /* imageMap */)
{
    // The value to display on the meter
    double value = 74.25;

    // Bar colors of the meters
    int barColor[] = {0x2299ff, 0x00ee00, 0xaa66ee, 0xff7711};

    // Create a LinearMeter object of size 70 x 240 pixels with very light grey (0xeeeeee)
    // backgruond and a grey (0xaaaaaa) 3-pixel thick rounded frame
    LinearMeter *m = new LinearMeter(70, 240, 0xeeeeee, 0xaaaaaa);
    m->setRoundedFrame(Chart::Transparent);
    m->setThickFrame(3);

    // This example demonstrates putting the text labels at the left or right side of the meter
    // scale, and putting the color scale on the same side as the labels or on opposite side.
    int alignment[] = {Chart::Left, Chart::Left, Chart::Right, Chart::Right};
    int meterXPos[] = {28, 38, 12, 21};
    int labelGap[] = {2, 12, 10, 2};
    int colorScalePos[] = {53, 28, 36, 10};

    // Configure the position of the meter scale and which side to put the text labels
    m->setMeter(meterXPos[chartIndex], 18, 20, 205, alignment[chartIndex]);

    // Set meter scale from 0 - 100, with a tick every 10 units
    m->setScale(0, 100, 10);

    // To put the color scale on the same side as the text labels, we need to increase the gap
    // between the labels and the meter scale to make room for the color scale
    m->setLabelPos(false, labelGap[chartIndex]);

    // Add a smooth color scale to the meter
    double smoothColorScale[] = {0, 0x0000ff, 25, 0x0088ff, 50, 0x00ff00, 75, 0xdddd00, 100,
        0xff0000};
    m->addColorScale(DoubleArray(smoothColorScale, (int)(sizeof(smoothColorScale) / sizeof(
        smoothColorScale[0]))), colorScalePos[chartIndex], 6);

    // Add a bar from 0 to value with glass effect and 4 pixel rounded corners
    m->addBar(0, value, barColor[chartIndex], Chart::glassEffect(Chart::NormalGlare, Chart::Left), 4
        );

    // Output the chart
    m->makeChart();

    return m;
}


////////////////////////////////////////////////////////////////////////////////
//
//	An array storing all demo chart functions. An empty entry signals the end 
//	of the array. 
//
////////////////////////////////////////////////////////////////////////////////
DemoChart demoCharts[] = 
{

    {"Pie Charts", 0, 0},

        {"Simple Pie Chart", 1, simplepie},
        {"3D Pie Chart", 1, threedpie},
        {"Multi-Depth Pie Chart", 1, multidepthpie},
        {"Side Label Layout", 1, sidelabelpie},
        {"Circular Label Layout", 2, circlelabelpie},
        {"Pie Chart with Legend (1)", 1, legendpie},
        {"Pie Chart with Legend (2)", 1, legendpie2},
        {"Exploded Pie Chart", 1, explodedpie},
        {"Icon Pie Chart (1)", 1, iconpie},
        {"Icon Pie Chart (2)", 1, iconpie2},
        {"Multi-Pie Chart", 3, multipie},
        {"Donut Chart", 1, donut},
        {"3D Donut Chart", 1, threeddonut},
        {"Icon Donut Chart", 1, icondonut},
        {"Texture Donut Chart", 1, texturedonut},
        {"Concentric Donut Chart", 1, concentric},
        {"2D Pie Shading", 6, pieshading},
        {"3D Pie Shading", 7, threedpieshading},
        {"2D Donut Shading", 7, donutshading},
        {"3D Donut Shading", 8, threeddonutshading},
        {"Text Style and Colors", 1, fontpie},
        {"3D Angle", 7, threedanglepie},
        {"3D Depth", 5, threeddepthpie},
        {"3D Shadow Mode", 4, shadowpie},
        {"Start Angle and Direction", 2, anglepie},
        {"Donut Width", 5, donutwidth},

    {"Bar Charts", 0, 0},

        {"Simple Bar Chart (1)", 1, simplebar},
        {"Simple Bar Chart (2)", 1, simplebar2},
        {"Bar Labels", 1, barlabel},
        {"Multi-Color Bar Chart (1)", 1, colorbar},
        {"Multi-Color Bar Chart (2)", 1, colorbar2},
        {"Soft Bar Shading", 1, softlightbar},
        {"Glass Bar Shading", 1, glasslightbar},
        {"Gradient Bar Shading", 1, gradientbar},
        {"Cylinder Bar Shading", 1, cylinderlightbar},
        {"3D Bar Chart", 1, threedbar},
        {"Cylinder Bar Shape", 1, cylinderbar},
        {"Polygon Bar Shapes", 1, polygonbar},
        {"Stacked Bar Chart", 1, stackedbar},
        {"Percentage Bar Chart", 1, percentbar},
        {"Multi-Bar Chart", 1, multibar},
        {"Soft Multi-Bar Chart", 1, softmultibar},
        {"Glass Multi-Bar Chart", 1, glassmultibar},
        {"Gradient Multi-Bar Chart", 1, gradientmultibar},
        {"Multi-Cylinder Chart", 1, multicylinder},
        {"Multi-Shape Bar Chart", 1, multishapebar},
        {"Overlapping Bar Chart", 1, overlapbar},
        {"Multi-Stacked Bar Chart", 1, multistackbar},
        {"Depth Bar Chart", 1, depthbar},
        {"Positive Negative Bars", 1, posnegbar},
        {"Borderless Bar Chart", 1, hbar},
        {"Dual Horizontal Bar Charts", 1, dualhbar},
        {"Bars with Marks", 1, markbar},
        {"Histogram with Bell Curve", 1, histogram},
        {"Pareto Chart", 1, pareto},
        {"Variable Width Bar Chart", 1, varwidthbar},
        {"Bar Gap", 6, gapbar},

    {"Line Charts", 0, 0},

        {"Simple Line Chart", 1, simpleline},
        {"Compact Line Chart", 1, compactline},
        {"3D Line Chart", 1, threedline},
        {"Multi-Line Chart (1)", 1, multiline},
        {"Multi-Line Chart (2)", 1, multiline2},
        {"Symbol Line Chart", 1, symbolline},
        {"Symbol Line Chart (2)", 1, symbolline2},
        {"Missing Data Points", 1, missingpoints},
        {"Uneven Data Points ", 1, unevenpoints},
        {"Spline Line Chart", 1, splineline},
        {"Step Line Chart", 1, stepline},
        {"Inter-Line Coloring", 1, linefill},
        {"Line with Target Zone", 1, linecompare},
        {"Line with Error Symbols", 1, errline},
        {"Multi-Symbol Line Chart", 1, multisymbolline},
        {"Binary Data Series", 1, binaryseries},
        {"Custom Symbols", 1, customsymbolline},
        {"Rotated Line Chart", 1, rotatedline},
        {"Arbitrary XY Line Chart", 1, xyline},

    {"Trending and Curve Fitting", 0, 0},

        {"Trend Line Chart", 1, trendline},
        {"Scatter Trend Chart", 1, scattertrend},
        {"Confidence Band", 1, confidenceband},
        {"Parametric Curve Fitting", 1, paramcurve},
        {"General Curve Fitting", 1, curvefitting},

    {"Scatter/Bubble/Vector Charts", 0, 0},

        {"Scatter Chart", 1, scatter},
        {"Built-in Symbols", 1, builtinsymbols},
        {"Custom Scatter Symbols", 1, scattersymbols},
        {"Custom Scatter Labels", 1, scatterlabels},
        {"Bubble Chart", 1, bubble},
        {"3D Bubble Chart (1)", 1, threedbubble},
        {"3D Bubble Chart (2)", 1, threedbubble2},
        {"3D Bubble Chart (3)", 1, threedbubble3},
        {"Bubble XY Scaling", 1, bubblescale},
        {"Vector Chart", 1, vector},

    {"Area Charts", 0, 0},

        {"Simple Area Chart", 1, simplearea},
        {"Enhanced Area Chart", 1, enhancedarea},
        {"Area Line Chart", 1, arealine},
        {"3D Area Chart", 1, threedarea},
        {"Pattern Area Chart", 1, patternarea},
        {"Stacked Area Chart", 1, stackedarea},
        {"3D Stacked Area Chart", 1, threedstackarea},
        {"Percentage Area Chart", 1, percentarea},
        {"Depth Area Chart", 1, deptharea},
        {"Rotated Area Chart", 1, rotatedarea},

    {"Floating Box/Waterfall Charts", 0, 0},

        {"Box-Whisker Chart (1)", 1, boxwhisker},
        {"Box-Whisker Chart (2)", 1, boxwhisker2},
        {"Horizontal Box-Whisker Chart", 1, hboxwhisker},
        {"Floating Box Chart", 1, floatingbox},
        {"Waterfall Chart", 1, waterfall},
        {"Pos/Neg Waterfall Chart", 1, posnegwaterfall},

    {"Gantt Charts", 0, 0},

        {"Simple Gantt Chart", 1, gantt},
        {"Multi-Color Gantt Chart", 1, colorgantt},
        {"Multi-Layer Gantt Chart", 1, layergantt},

    {"Contour Charts/Heat Maps", 0, 0},

        {"Contour Chart", 1, contour},
        {"Scattered Data Contour Chart", 1, scattercontour},
        {"Contour Color Scale", 4, contourcolor},
        {"Contour Color Legend", 1, contourlegend},
        {"Continuous Contour Coloring", 1, smoothcontour},
        {"Contour Interpolation", 4, contourinterpolate},

    {"Finance Charts", 0, 0},

        {"Simple HLOC Chart", 1, hloc},
        {"Simple Candlestick Chart", 1, candlestick},
        {"Finance Chart (1)", 1, finance},
        {"Finance Chart (2)", 1, finance2},
        {"Finance Chart Custom Symbols", 1, financesymbols},

    {"Other XY Chart Features", 0, 0},

        {"Marks and Zones (1)", 1, markzone},
        {"Marks and Zones (2)", 1, markzone2},
        {"Y Zone Coloring", 1, yzonecolor},
        {"X Zone Coloring", 1, xzonecolor},
        {"Dual Y-Axis", 1, dualyaxis},
        {"Dual X-Axis", 1, dualxaxis},
        {"Multiple Axes", 1, multiaxes},
        {"4 Quadrant Chart", 1, fourq},
        {"Data Table (1)", 1, datatable},
        {"Data Table (2)", 1, datatable2},
        {"Text Style and Colors", 1, fontxy},
        {"Background and Wallpaper", 4, background},
        {"Log Scale Axis", 2, logaxis},
        {"Y-Axis Scaling", 5, axisscale},
        {"Tick Density", 2, ticks},

    {"Surface Charts", 0, 0},

        {"Surface Chart (1)", 1, surface},
        {"Surface Chart (2)", 1, surface2},
        {"Surface Chart (3)", 1, surface3},
        {"Scattered Data Surface Chart", 1, scattersurface},
        {"Surface Chart Axis Types", 1, surfaceaxis},
        {"Surface Lighting", 4, surfacelighting},
        {"Surface Shading", 4, surfaceshading},
        {"Surface Wireframe", 6, surfacewireframe},
        {"Surface Perspective", 6, surfaceperspective},

    {"3D Scatter Charts", 0, 0},

        {"3D Scatter Chart (1)", 1, threedscatter},
        {"3D Scatter Chart (2)", 1, threedscatter2},
        {"3D Scatter Groups", 1, threedscattergroups},
        {"3D Scatter Axis Types", 1, threedscatteraxis},

    {"Polar/Radar Charts", 0, 0},

        {"Simple Radar Chart", 1, simpleradar},
        {"Multi Radar Chart", 1, multiradar},
        {"Stacked Radar Chart", 1, stackradar},
        {"Polar Line Chart", 1, polarline},
        {"Polar Area Chart", 1, polararea},
        {"Polar Spline Chart", 1, polarspline},
        {"Polar Scatter Chart", 1, polarscatter},
        {"Polar Bubble Chart", 1, polarbubble},
        {"Polar Vector Chart", 1, polarvector},
        {"Simple Rose Chart", 1, rose},
        {"Stacked Rose Chart", 1, stackrose},
        {"Circular Zones", 1, polarzones},
        {"Sector Zones", 1, polarzones2},

    {"Pyramids/Cones/Funnels", 0, 0},

        {"Simple Pyramid Chart", 1, simplepyramid},
        {"3D Pyramid Chart", 1, threedpyramid},
        {"Rotated Pyramid Chart", 1, rotatedpyramid},
        {"Cone Chart", 1, cone},
        {"Funnel Chart", 1, funnel},
        {"Pyramid Elevation", 7, pyramidelevation},
        {"Pyramid Rotation", 7, pyramidrotation},
        {"Pyramid Gap", 6, pyramidgap},

    {"Angular Meters/Guages", 0, 0},

        {"Semicircle Meter", 1, semicirclemeter},
        {"Color Semicircle Meters", 6, colorsemicirclemeter},
        {"Black Semicircle Meters", 6, blacksemicirclemeter},
        {"White Semicircle Meters", 6, whitesemicirclemeter},
        {"Semicircle Meter with Readout", 2, semicirclemeterreadout},
        {"Round Meter", 1, roundmeter},
        {"Color Round Meters", 6, colorroundmeter},
        {"Black Round Meters", 6, blackroundmeter},
        {"White Round Meters", 6, whiteroundmeter},
        {"Neon Round Meters", 4, neonroundmeter},
        {"Round Meters with Readout", 2, roundmeterreadout},
        {"Rectangular Angular Meters", 6, rectangularmeter},
        {"Square Angular Meters", 4, squareameter},
        {"Angular Meter Pointers (1)", 1, angularpointer},
        {"Angular Meter Pointers (2)", 1, angularpointer2},
        {"Icon Angular Meter", 1, iconameter},

    {"Linear Meters/Guages", 0, 0},

        {"Horizontal Linear Meter", 1, hlinearmeter},
        {"Color Horizontal Linear Meters", 6, colorhlinearmeter},
        {"Black Horizontal Linear Meters", 6, blackhlinearmeter},
        {"White Horizontal Linear Meters", 4, whitehlinearmeter},
        {"H-Linear Meter Orientation", 4, hlinearmeterorientation},
        {"Vertical Linear Meter", 1, vlinearmeter},
        {"Color Vertical Linear Meters", 6, colorvlinearmeter},
        {"Black Vertical Linear Meters", 6, blackvlinearmeter},
        {"White Vertical Linear Meters", 6, whitevlinearmeter},
        {"V-Linear Meter Orientation", 2, vlinearmeterorientation},
        {"Multi-Pointer Horizontal Meter", 1, multihmeter},
        {"Multi-Pointer Vertical Meter", 1, multivmeter},
        {"Linear Zone Meter", 1, linearzonemeter},

    {"Bar Meters/Guages", 0, 0},

        {"Horizontal Bar Meter", 1, hbarmeter},
        {"Color Horizontal Bar Meters", 6, colorhbarmeter},
        {"Black Horizontal Bar Meters", 6, blackhbarmeter},
        {"White Horizontal Bar Meters", 6, whitehbarmeter},
        {"H-Bar Meter Orientation", 4, hbarmeterorientation},
        {"Vertical Bar Meter", 1, vbarmeter},
        {"Color Vertical Bar Meters", 6, colorvbarmeter},
        {"Black Vertical Bar Meters", 6, blackvbarmeter},
        {"White Vertical Bar Meters", 6, whitevbarmeter},
        {"V-Bar Meter Orientation", 4, vbarmeterorientation},
    {0, 0, 0}
};
