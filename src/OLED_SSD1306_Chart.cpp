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
    _oy[1] = _gy + _h / 2;
    _x = _gx;
    _xDrawingOffset = 0;

    if (_yLabelsVisible)
    {
        if (_mode == SINGLE_PLOT_MODE)
        {
            int16_t x, y;
            uint16_t w, h;

            setTextSize(1);
            setTextColor(WHITE);

            getTextBounds(_yLabelHi[0], _gx + 5, _gy + 5 - _h, &x, &y, &w, &h);
            _xDrawingOffset = w;

            // high label
            setCursor(_gx, _gy - _h);
            write(_yLabelHi[0]);

            getTextBounds(_yLabelLo[0], _gx + 5, _gy - 5, &x, &y, &w, &h);

            // low label
            setCursor(_gx, _gy - (h / 2));
            write(_yLabelLo[0]);

            if (w > _xDrawingOffset)
            {
                _xDrawingOffset = w;
            }

            // compensation for the y axis tick lines
            _xDrawingOffset += 4;
        }
        else if (_mode == DOUBLE_PLOT_MODE)
        {
            int16_t x, y;
            uint16_t w, h;

            setTextSize(1);
            setTextColor(WHITE);
            //Chart 1
            getTextBounds(_yLabelHi[1], _gx + 5, _gy + 5 - _h, &x, &y, &w, &h);
            _xDrawingOffset = w;

            // high label
            setCursor(_gx, _gy - _h);
            write(_yLabelHi[1]);

            getTextBounds(_yLabelLo[1], _gx + 5, _gy - 5, &x, &y, &w, &h);

            // low label
            setCursor(_gx, _gy - _h / 2 - (h / 2));
            write(_yLabelLo[1]);

            if (w > _xDrawingOffset)
            {
                _xDrawingOffset = w;
            }

            //Chart 0
            getTextBounds(_yLabelHi[0], _gx + 5, _gy + 5 - _h, &x, &y, &w, &h);
            _xDrawingOffset = w;

            // high label
            setCursor(_gx, _gy - _h / 2 + (h / 2));
            write(_yLabelHi[0]);

            getTextBounds(_yLabelLo[0], _gx + 5, _gy - 5, &x, &y, &w, &h);

            // low label
            setCursor(_gx, _gy - (h / 2));
            write(_yLabelLo[0]);

            if (w > _xDrawingOffset)
            {
                _xDrawingOffset = w;
            }

            // compensation for the y axis tick lines
            _xDrawingOffset += 4;
        }
    }

    for (i = _gy; i <= _gy + _h; i += _yincdiv)
    {
        temp = (i - _gy) * (_gy - _h - _gy) / (_h) + _gy;
        if (i == _gy)
        {
            drawFastHLine(_gx - 3 + _xDrawingOffset, temp, _w + 3 - _xDrawingOffset, WHITE);
        }
        else
        {
            drawFastHLine(_gx - 3 + _xDrawingOffset, temp, 3, WHITE);
        }
    }
    // draw x scale
    for (i = 0; i <= _w - _xDrawingOffset; i += _xincdiv)
    {
        temp = (i) + _gx + _xDrawingOffset;
        if (i == 0)
        {
            drawFastVLine(temp, _gy - _h, _h + 3, WHITE);
        }
        else
        {
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
    if (chart == 0 || chart == 1)
    {
        _ylo[chart] = ylo;
        _yhi[chart] = yhi;
    }
}

void OLED_SSD1306_Chart::setYLimitLabels(char *loLabel, char *hiLabel, uint8_t chart)
{
    if (chart == 0 || chart == 1)
    {
        _yLabelLo[chart] = loLabel;
        _yLabelHi[chart] = hiLabel;
    }
}

void OLED_SSD1306_Chart::setYLabelsVisible(bool yLabelsVisible)
{
    _yLabelsVisible = yLabelsVisible;
}

void OLED_SSD1306_Chart::setPointGeometry(char pointGeometry, uint8_t chart)
{
    if (chart == 0 || chart == 1)
        _point_geometry[chart] = pointGeometry;
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
    if (_x >= _gx + _w - _xDrawingOffset)
        return false;

    _x += _xinc;

    if (_mode == SINGLE_PLOT_MODE)
    {
        double y = (firstValue - _ylo[0]) * (_gy - _h - _gy) / (_yhi[0] - _ylo[0]) + _gy;
        drawLine(_ox[0] + _xDrawingOffset, _oy[0], _x + _xDrawingOffset, y, WHITE);
        drawLine(_ox[0] + _xDrawingOffset, _oy[0] - 1, _x + _xDrawingOffset, y - 1, WHITE);

        _ox[0] = _x;
        _oy[0] = y;

        if (_point_geometry[0] == POINT_GEOMETRY_CIRCLE)
            fillCircle(_ox[0] + _xDrawingOffset, _oy[0], 2, WHITE);

        display();
        return true;
    }

    else
    {
        auto semiHeight = _h / 2;
        double y = (firstValue - _ylo[0]) * (-semiHeight) / (_yhi[0] - _ylo[0]) + _gy;
        double secondY = (secondValue - _ylo[1]) * (-semiHeight) / (_yhi[0] - _ylo[0]) + _gy - semiHeight;

        drawLine(_ox[0] + _xDrawingOffset, _oy[0], _x + _xDrawingOffset, y, WHITE);
        drawLine(_ox[0] + _xDrawingOffset, _oy[0] - 1, _x + _xDrawingOffset, y - 1, WHITE);

        drawLine(_ox[1] + _xDrawingOffset, _oy[1], _x + _xDrawingOffset, secondY, WHITE);
        drawLine(_ox[1] + _xDrawingOffset, _oy[1] - 1, _x + _xDrawingOffset, secondY - 1, WHITE);

        _ox[0] = _x;
        _oy[0] = y;
        _ox[1] = _x;
        _oy[1] = secondY;

        if (_point_geometry[0] == POINT_GEOMETRY_CIRCLE)
            fillCircle(_ox[0] + _xDrawingOffset, _oy[0], 2, WHITE);
        if (_point_geometry[1] == POINT_GEOMETRY_CIRCLE)
            fillCircle(_ox[1] + _xDrawingOffset, _oy[1], 2, WHITE);

        display();
        return true;
    }
}