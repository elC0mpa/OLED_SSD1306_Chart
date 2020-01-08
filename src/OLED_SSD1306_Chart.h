#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SINGLE_PLOT_MODE 0
#define DOUBLE_PLOT_MODE 1
#define POINT_GEOMETRY_NONE 2
#define POINT_GEOMETRY_CIRCLE 3
#define POINT_GEOMETRY_SQUARE 4
#define POINT_GEOMETRY_TRIANGLE 5

class OLED_SSD1306_Chart:public Adafruit_SSD1306
{
private:
    double _ox[2], _oy[2];      //Previous point coordinates
    double _gx, _gy;            //Chart lower left coordinates
    double _w, _h;              //Chart width and height
    double _ylo[2], _yhi[2];    //Y axis Min and max values
    double _xinc;               //X coordinate increment between values
    double _x;                  //Actual point x coordinate
    double _xincdiv, _yincdiv;  //X and Y axis distance between division
    double _dig;                
    char _mode;                 //Plot mode: single or double
    char _point_geometry[2];    //Point geometry
public:

    //Ctors
    OLED_SSD1306_Chart():Adafruit_SSD1306()
    {
        _mode = SINGLE_PLOT_MODE;
        _point_geometry[0] = POINT_GEOMETRY_NONE;
        _point_geometry[1] = POINT_GEOMETRY_NONE;
    }

    OLED_SSD1306_Chart(uint8_t w, uint8_t h, TwoWire *twi, int8_t rst_pin=-1,
        uint32_t clkDuring=400000UL, uint32_t clkAfter=100000UL):Adafruit_SSD1306(w, h, twi, rst_pin, clkDuring, clkAfter)
        {
            _mode = SINGLE_PLOT_MODE;
            _point_geometry[0] = POINT_GEOMETRY_NONE;
            _point_geometry[1] = POINT_GEOMETRY_NONE;
        }

    OLED_SSD1306_Chart(uint8_t w, uint8_t h, int8_t mosi_pin, int8_t sclk_pin,
        int8_t dc_pin, int8_t rst_pin, int8_t cs_pin):Adafruit_SSD1306(w, h, mosi_pin, sclk_pin, dc_pin, rst_pin, cs_pin)
        {
            _mode = SINGLE_PLOT_MODE;
            _point_geometry[0] = POINT_GEOMETRY_NONE;
            _point_geometry[1] = POINT_GEOMETRY_NONE;
        }

    OLED_SSD1306_Chart(uint8_t w, uint8_t h, SPIClass *spi,
        int8_t dc_pin, int8_t rst_pin, int8_t cs_pin, uint32_t bitrate=8000000UL):Adafruit_SSD1306(w, h, spi, dc_pin, rst_pin, cs_pin, bitrate)
        {
            _mode = SINGLE_PLOT_MODE;
            _point_geometry[0] = POINT_GEOMETRY_NONE;
            _point_geometry[1] = POINT_GEOMETRY_NONE;
        }

    void setPlotMode(char mode);
    void setChartCoordinates(double x, double y);
    void setChartWidthAndHeight(double w, double h);
    void setYLimits(double ylo, double yhi, uint8_t chart = 0);
    void setPointGeometry(char pointGeometry, uint8_t chart = 0); 
    void setXIncrement(double xinc);
    void setAxisDivisionsInc(double xinc, double yinc);

    void drawChart();
    bool updateChart(double firstValue, double secondValue = 0);
};
