#include "OLED_SSD1306_Chart.h"

/*!
    @brief  Updates the internal buffer to draw the cartesian graph
    @note   Call the object's begin() function before use -- buffer allocation is performed there!
            Call the object's configureChart() function before use -- params are updated there
*/
void OLED_SSD1306_Chart::drawChart()
{
    double i, temp;
    _dig = 0;
    _ox[0] = _gx;
    _ox[1] = _gx;
    _oy[0] = _gy;
    _oy[1] = _gy + _h/2;
    _x = _gx;


    for (i = _gy; i <= _gy + _h; i += _yincdiv) {
    temp =  (i - _gy) * (_gy - _h - _gy) / (_h ) + _gy;
    if (i == _gy)
    {
        drawFastHLine(_gx - 3, temp, _w + 3, WHITE);
    } 
    else 
    {
        drawFastHLine(_gx - 3, temp, 3, WHITE);
    }
    }
    // draw x scale
    for (i = 0; i <= _w; i += _xincdiv) {
    temp =  (i) + _gx;
    if (i == 0) {
        drawFastVLine(temp, _gy - _h, _h + 3, WHITE);
    }
    else {
        drawFastVLine(temp, _gy, 3, WHITE);
    }
    }
}

void OLED_SSD1306_Chart::setPlotMode(char mode)
{
    if (mode == SINGLE_PLOT_MODE || mode == DOUBLE_PLOT_MODE)
        _mode = mode;
}

void OLED_SSD1306_Chart::setYLimits(double ylo, double yhi, uint8_t chart)
{
    if(chart == 0 || chart == 1)
    {
        _ylo[chart] = ylo;
        _yhi[chart] = yhi;
    }
}

void OLED_SSD1306_Chart::setChartCoordinates(double x, double y)
{
    _gx = x;
    _gy = y;
}

void OLED_SSD1306_Chart::setChartWidthAndHeight(double w, double h)
{
    _w = w;
    _h = h;
}

void OLED_SSD1306_Chart::setAxisDivisionsInc(double xinc, double yinc)
{
    _xincdiv = xinc;
    _yincdiv = yinc;
}

void OLED_SSD1306_Chart::setXIncrement(double xinc)
{
    _xinc = xinc;
}

bool OLED_SSD1306_Chart::updateChart(double firstValue, double secondValue)
{
    if (_x >= _gx + _w)
        return false;

    _x+=_xinc;

    
    if (_mode == SINGLE_PLOT_MODE)
    {
        double y =  (firstValue - _ylo[0]) * (_gy - _h - _gy) / (_yhi[0] - _ylo[0]) + _gy;
        drawLine(_ox[0], _oy[0], _x, y, WHITE);
        drawLine(_ox[0], _oy[0] - 1, _x, y - 1, WHITE); 

        _ox[0] = _x;
        _oy[0] = y;

        fillCircle(_ox[0], _oy[0], 2, WHITE);
        
        display();
        return true;
    }

    else 
    {
        auto semiHeight = _h / 2;
        double y =  (firstValue - _ylo[0]) * (- semiHeight) / (_yhi[0] - _ylo[0]) + _gy;
        double secondY = (secondValue - _ylo[1]) * (- semiHeight ) / (_yhi[0] - _ylo[0]) + _gy - semiHeight; 

        drawLine(_ox[0], _oy[0], _x, y, WHITE);
        drawLine(_ox[0], _oy[0] - 1, _x, y - 1, WHITE); 

        drawLine(_ox[1], _oy[1], _x, secondY, WHITE);
        drawLine(_ox[1], _oy[1] - 1, _x, secondY - 1, WHITE); 

        _ox[0] = _x;
        _oy[0] = y;
        _ox[1] = _x;
        _oy[1] = secondY;

        fillCircle(_ox[0], _oy[0], 2, WHITE);
        fillCircle(_ox[1], _oy[1], 2, WHITE);

        display();
        return true;
    }
}