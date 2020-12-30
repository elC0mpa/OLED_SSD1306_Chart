#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SINGLE_PLOT_MODE 0
#define DOUBLE_PLOT_MODE 1
#define POINT_GEOMETRY_NONE 2
#define POINT_GEOMETRY_CIRCLE 3
#define POINT_GEOMETRY_SQUARE 4
#define POINT_GEOMETRY_TRIANGLE 5
#define LIGHT_LINE 6
#define NORMAL_LINE 7

class OLED_SSD1306_Chart : public Adafruit_SSD1306
{
private:
    double _previous_x_coordinate[2], _previous_y_coordinate[2]; //Previous point coordinates
    double _x_lower_left_coordinate, _y_lower_left_coordinate;   //Chart lower left coordinates
    double _chart_width, _chart_height;                          //Chart width and height
    double _y_min_values[2], _y_max_values[2];                   //Y axis Min and max values
    double _x_inc;                                               //X coordinate increment between values
    double _actual_x_coordinate;                                 //Actual point x coordinate
    double _xinc_div, _yinc_div;                                 //X and Y axis distance between division
    double _dig;
    char _mode;               //Plot mode: single or double
    char _point_geometry[2];  //Point geometry
    bool _y_labels_visible;   //Determines if the y labels should be shown
    char *_y_min_label[2];    //Labels of the lower y value
    char *_y_max_label[2];    //Labels of the higher y value
    double _x_drawing_offset; //Used to draw the char after the labels are applied
    bool _mid_line_visible;   //Determines if the mid line should be shown in Double plot mode
    char _lines_thickness[2]; //Line thickness
    void _drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color, uint8_t thickness);

public:
    //Ctors
    OLED_SSD1306_Chart() : Adafruit_SSD1306()
    {
        _mode = SINGLE_PLOT_MODE;
        _point_geometry[0] = POINT_GEOMETRY_NONE;
        _point_geometry[1] = POINT_GEOMETRY_NONE;
        _y_labels_visible = false;
        _mid_line_visible = true;
        _lines_thickness[0] = NORMAL_LINE;
        _lines_thickness[1] = NORMAL_LINE;
        _y_min_label[0] = "";
        _y_min_label[1] = "";
        _y_max_label[0] = "";
        _y_max_label[1] = "";
    }

    OLED_SSD1306_Chart(uint8_t w, uint8_t h, TwoWire *twi, int8_t rst_pin = -1,
                       uint32_t clkDuring = 400000UL, uint32_t clkAfter = 100000UL) : Adafruit_SSD1306(w, h, twi, rst_pin, clkDuring, clkAfter)
    {
        _mode = SINGLE_PLOT_MODE;
        _point_geometry[0] = POINT_GEOMETRY_NONE;
        _point_geometry[1] = POINT_GEOMETRY_NONE;
        _y_labels_visible = false;
        _mid_line_visible = true;
        _lines_thickness[0] = NORMAL_LINE;
        _lines_thickness[1] = NORMAL_LINE;
        _y_min_label[0] = "";
        _y_min_label[1] = "";
        _y_max_label[0] = "";
        _y_max_label[1] = "";
    }

    OLED_SSD1306_Chart(uint8_t w, uint8_t h, int8_t mosi_pin, int8_t sclk_pin,
                       int8_t dc_pin, int8_t rst_pin, int8_t cs_pin) : Adafruit_SSD1306(w, h, mosi_pin, sclk_pin, dc_pin, rst_pin, cs_pin)
    {
        _mode = SINGLE_PLOT_MODE;
        _point_geometry[0] = POINT_GEOMETRY_NONE;
        _point_geometry[1] = POINT_GEOMETRY_NONE;
        _y_labels_visible = false;
        _mid_line_visible = true;
        _lines_thickness[0] = NORMAL_LINE;
        _lines_thickness[1] = NORMAL_LINE;
        _y_min_label[0] = "";
        _y_min_label[1] = "";
        _y_max_label[0] = "";
        _y_max_label[1] = "";
    }

    OLED_SSD1306_Chart(uint8_t w, uint8_t h, SPIClass *spi,
                       int8_t dc_pin, int8_t rst_pin, int8_t cs_pin, uint32_t bitrate = 8000000UL) : Adafruit_SSD1306(w, h, spi, dc_pin, rst_pin, cs_pin, bitrate)
    {
        _mode = SINGLE_PLOT_MODE;
        _point_geometry[0] = POINT_GEOMETRY_NONE;
        _point_geometry[1] = POINT_GEOMETRY_NONE;
        _y_labels_visible = false;
        _mid_line_visible = true;
        _lines_thickness[0] = NORMAL_LINE;
        _lines_thickness[1] = NORMAL_LINE;
        _y_min_label[0] = "";
        _y_min_label[1] = "";
        _y_max_label[0] = "";
        _y_max_label[1] = "";
    }

    void setPlotMode(char mode);
    void setChartCoordinates(double x, double y);
    void setChartWidthAndHeight(double w, double h);
    void setYLimits(double ylo, double yhi, uint8_t chart = 0);
    void setYLimitLabels(char *loLabel, char *hiLabel, uint8_t chart = 0);
    void setYLabelsVisible(bool yLabelsVisible);
    void setPointGeometry(char pointGeometry, uint8_t chart = 0);
    void setXIncrement(double xinc);
    void setAxisDivisionsInc(double xinc, double yinc);
    void setMidLineVisible(bool lineVisible);
    void setLineThickness(char thickness, uint8_t chart = 0);

    void drawChart();
    bool updateChart(double firstValue, double secondValue = 0);
};
