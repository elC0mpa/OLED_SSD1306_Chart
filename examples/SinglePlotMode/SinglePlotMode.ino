/*
  Name:    SinglePlotMode.ino
  Created:  04/01/2020 11:59:52 AM
  Author: José Gabriel Companioni Benítez (https://github.com/elC0mpa)
  Description: Example to demonstrate how to draw cartesian charts in Adafruit SSD1306 oleds in single plot mode 
*/

#include <Arduino.h>
#include <OLED_SSD1306_Chart.h>
#include <Adafruit_I2CDevice.h> //Include this to avoid compile errors in Platformio

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define BAUDRATE 9600

#define SDA_PIN D1
#define SCL_PIN D2

OLED_SSD1306_Chart display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char actualThickness;

void setup()
{
// put your setup code here, to run once:
#if defined ESP8266
  Wire.begin(SDA_PIN, SCL_PIN);
#else
  Wire.begin();
#endif

  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.clearDisplay();
  display.setChartCoordinates(0, 60);      //Chart lower left coordinates (X, Y)
  display.setChartWidthAndHeight(123, 55); //Chart width = 123 and height = 60
  display.setXIncrement(5);                //Distance between Y points will be 5px
  display.setYLimits(50, 100);             //Ymin = 0 and Ymax = 100
  display.setYLimitLabels("50", "100");    //Setting Y axis labels
  display.setYLabelsVisible(true);
  display.setAxisDivisionsInc(12, 6);    //Each 12 px a division will be painted in X axis and each 6px in Y axis
  display.setPlotMode(SINGLE_PLOT_MODE); //Set single plot mode
  // display.setPointGeometry(POINT_GEOMETRY_CIRCLE);
  actualThickness = NORMAL_LINE;
  display.setLineThickness(actualThickness);
  display.drawChart(); //Update the buffer to draw the cartesian chart
  display.display();
}

void loop()
{
  // put your main code here, to run repeatedly:
  auto value = random(50) + 50;
  if (!display.updateChart(value)) //Value between Ymin and Ymax will be added to chart
  {
    display.clearDisplay(); //If chart is full, it is drawn again
    if (actualThickness == NORMAL_LINE)
    {
      actualThickness = LIGHT_LINE;
    }
    else if (actualThickness == LIGHT_LINE)
    {
      actualThickness = NORMAL_LINE;
    }
    display.setLineThickness(actualThickness);
    display.drawChart();
  }
  delay(100);
}