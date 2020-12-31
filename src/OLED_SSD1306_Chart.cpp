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
    _previous_x_coordinate[0] = _x_lower_left_coordinate;
    _previous_x_coordinate[1] = _x_lower_left_coordinate;
    _previous_y_coordinate[0] = _y_lower_left_coordinate;
    _previous_y_coordinate[1] = _y_lower_left_coordinate - _chart_height / 2;
    _actual_x_coordinate = _x_lower_left_coordinate;
    _x_drawing_offset = 0;

    if (_y_labels_visible)
    {
        if (_mode == SINGLE_PLOT_MODE)
        {
            int16_t x, y;
            uint16_t w, h;

            setTextSize(1);
            setTextColor(WHITE);

            getTextBounds(_y_max_label[0], _x_lower_left_coordinate + 5, _y_lower_left_coordinate + 5 - _chart_height, &x, &y, &w, &h);
            _x_drawing_offset = w;

            // high label
            setCursor(_x_lower_left_coordinate, _y_lower_left_coordinate - _chart_height);
            write(_y_max_label[0]);

            getTextBounds(_y_min_label[0], _x_lower_left_coordinate + 5, _y_lower_left_coordinate - 5, &x, &y, &w, &h);

            // low label
            setCursor(_x_lower_left_coordinate, _y_lower_left_coordinate - (h / 2));
            write(_y_min_label[0]);

            if (w > _x_drawing_offset)
            {
                _x_drawing_offset = w;
            }

            // compensation for the y axis tick lines
            _x_drawing_offset += 4;
        }
        else if (_mode == DOUBLE_PLOT_MODE)
        {
            int16_t x, y;
            uint16_t w, h;

            setTextSize(1);
            setTextColor(WHITE);
            //Chart 1
            getTextBounds(_y_max_label[1], _x_lower_left_coordinate + 5, _y_lower_left_coordinate + 5 - _chart_height, &x, &y, &w, &h);
            _x_drawing_offset = w;

            // high label
            setCursor(_x_lower_left_coordinate, _y_lower_left_coordinate - _chart_height);
            write(_y_max_label[1]);

            getTextBounds(_y_min_label[1], _x_lower_left_coordinate + 5, _y_lower_left_coordinate - 5, &x, &y, &w, &h);

            // low label
            setCursor(_x_lower_left_coordinate, _y_lower_left_coordinate - _chart_height / 2 - (h / 2));
            write(_y_min_label[1]);

            if (w > _x_drawing_offset)
            {
                _x_drawing_offset = w;
            }

            //Chart 0
            getTextBounds(_y_max_label[0], _x_lower_left_coordinate + 5, _y_lower_left_coordinate + 5 - _chart_height, &x, &y, &w, &h);
            if (w > _x_drawing_offset)
            {
                _x_drawing_offset = w;
            }

            // high label
            setCursor(_x_lower_left_coordinate, _y_lower_left_coordinate - _chart_height / 2 + (h / 2));
            write(_y_max_label[0]);

            getTextBounds(_y_min_label[0], _x_lower_left_coordinate + 5, _y_lower_left_coordinate - 5, &x, &y, &w, &h);

            // low label
            setCursor(_x_lower_left_coordinate, _y_lower_left_coordinate - (h / 2));
            write(_y_min_label[0]);

            if (w > _x_drawing_offset)
            {
                _x_drawing_offset = w;
            }

            // compensation for the y axis tick lines
            _x_drawing_offset += 4;
        }
    }
    // draw y divisions
    for (i = _y_lower_left_coordinate; i <= _y_lower_left_coordinate + _chart_height; i += _yinc_div)
    {
        temp = (i - _y_lower_left_coordinate) * (_y_lower_left_coordinate - _chart_height - _y_lower_left_coordinate) / (_chart_height) + _y_lower_left_coordinate;
        if (i == _y_lower_left_coordinate)
        {
            drawFastHLine(_x_lower_left_coordinate - 3 + _x_drawing_offset, temp, _chart_width + 3 - _x_drawing_offset, WHITE);
        }
        else
        {
            drawFastHLine(_x_lower_left_coordinate - 3 + _x_drawing_offset, temp, 3, WHITE);
        }
    }
    // draw x divisions
    for (i = 0; i <= _chart_width - _x_drawing_offset; i += _xinc_div)
    {
        temp = (i) + _x_lower_left_coordinate + _x_drawing_offset;
        if (i == 0)
        {
            drawFastVLine(temp, _y_lower_left_coordinate - _chart_height, _chart_height + 3, WHITE);
        }
        else
        {
            drawFastVLine(temp, _y_lower_left_coordinate, 3, WHITE);
        }
    }
    if (_mid_line_visible && _mode == DOUBLE_PLOT_MODE)
    {
        drawFastHLine(_x_lower_left_coordinate + _x_drawing_offset, _y_lower_left_coordinate - _chart_height / 2, _chart_width - _x_drawing_offset, WHITE);
        for (i = 0; i <= _chart_width - _x_drawing_offset; i += _xinc_div)
        {
            drawFastVLine(i + _x_lower_left_coordinate + _x_drawing_offset, _y_lower_left_coordinate - _chart_height / 2, 3, WHITE);
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
        _y_min_values[chart] = ylo;
        _y_max_values[chart] = yhi;
    }
}

void OLED_SSD1306_Chart::setLineThickness(char thickness, uint8_t chart)
{
    if (chart == 0 || chart == 1)
    {
        _lines_thickness[chart] = thickness;
    }
}

void OLED_SSD1306_Chart::setYLimitLabels(char *loLabel, char *hiLabel, uint8_t chart)
{
    if (chart == 0 || chart == 1)
    {
        _y_min_label[chart] = loLabel;
        _y_max_label[chart] = hiLabel;
    }
}

void OLED_SSD1306_Chart::setYLabelsVisible(bool yLabelsVisible)
{
    _y_labels_visible = yLabelsVisible;
}

void OLED_SSD1306_Chart::setMidLineVisible(bool lineVisible)
{
    _mid_line_visible = lineVisible;
}

void OLED_SSD1306_Chart::setPointGeometry(char pointGeometry, uint8_t chart)
{
    if (chart == 0 || chart == 1)
        _point_geometry[chart] = pointGeometry;
}

void OLED_SSD1306_Chart::setChartCoordinates(double x, double y)
{
    _x_lower_left_coordinate = x;
    _y_lower_left_coordinate = y;
}

void OLED_SSD1306_Chart::setChartWidthAndHeight(double w, double h)
{
    _chart_width = w;
    _chart_height = h;
}

void OLED_SSD1306_Chart::setAxisDivisionsInc(double xinc, double yinc)
{
    _xinc_div = xinc;
    _yinc_div = yinc;
}

void OLED_SSD1306_Chart::setXIncrement(double xinc)
{
    _x_inc = xinc;
}

void OLED_SSD1306_Chart::_drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color, uint8_t thickness)
{
    char linesToDraw = 0;
    if (thickness == LIGHT_LINE)
    {
        linesToDraw = 2;
        for (size_t i = 0; i < linesToDraw; i++)
            drawLine(x0, y0 - i, x1, y1, color);
    }
    else if (thickness == NORMAL_LINE)
    {
        linesToDraw = 5;
        for (size_t i = 0; i < linesToDraw; i++)
            drawLine(x0, y0 + 2 - i, x1, y1, color);
    }
}

bool OLED_SSD1306_Chart::updateChart(double firstValue, double secondValue)
{
    if (_actual_x_coordinate >= _x_lower_left_coordinate + _chart_width - _x_drawing_offset)
        return false;

    _actual_x_coordinate += _x_inc;

    if (firstValue < _y_min_values[0])
        firstValue = _y_min_values[0];

    if (firstValue > _y_max_values[0])
        firstValue = _y_max_values[0];

    if (_mode == SINGLE_PLOT_MODE)
    {
        double y = (firstValue - _y_min_values[0]) * (_y_lower_left_coordinate - _chart_height - _y_lower_left_coordinate) / (_y_max_values[0] - _y_min_values[0]) + _y_lower_left_coordinate;
        char linesToDraw = 0;
        _drawLine(_previous_x_coordinate[0] + _x_drawing_offset, _previous_y_coordinate[0], _actual_x_coordinate + _x_drawing_offset, y, WHITE, _lines_thickness[0]);

        _previous_x_coordinate[0] = _actual_x_coordinate;
        _previous_y_coordinate[0] = y;

        if (_point_geometry[0] == POINT_GEOMETRY_CIRCLE)
            fillCircle(_previous_x_coordinate[0] + _x_drawing_offset, _previous_y_coordinate[0], 2, WHITE);

        display();
        return true;
    }

    else
    {
        if (secondValue < _y_min_values[1])
            secondValue = _y_min_values[1];

        if (secondValue > _y_max_values[1])
            secondValue = _y_max_values[1];
        auto semiHeight = _chart_height / 2;
        double y = (firstValue - _y_min_values[0]) * (-semiHeight) / (_y_max_values[0] - _y_min_values[0]) + _y_lower_left_coordinate;
        double secondY = (secondValue - _y_min_values[1]) * (-semiHeight) / (_y_max_values[0] - _y_min_values[0]) + _y_lower_left_coordinate - semiHeight;

        _drawLine(_previous_x_coordinate[0] + _x_drawing_offset, _previous_y_coordinate[0], _actual_x_coordinate + _x_drawing_offset, y, WHITE, _lines_thickness[0]);
        _drawLine(_previous_x_coordinate[1] + _x_drawing_offset, _previous_y_coordinate[1], _actual_x_coordinate + _x_drawing_offset, secondY, WHITE, _lines_thickness[1]);

        _previous_x_coordinate[0] = _actual_x_coordinate;
        _previous_y_coordinate[0] = y;
        _previous_x_coordinate[1] = _actual_x_coordinate;
        _previous_y_coordinate[1] = secondY;

        if (_point_geometry[0] == POINT_GEOMETRY_CIRCLE)
            fillCircle(_previous_x_coordinate[0] + _x_drawing_offset, _previous_y_coordinate[0], 2, WHITE);
        if (_point_geometry[1] == POINT_GEOMETRY_CIRCLE)
            fillCircle(_previous_x_coordinate[1] + _x_drawing_offset, _previous_y_coordinate[1], 2, WHITE);

        display();
        return true;
    }
}