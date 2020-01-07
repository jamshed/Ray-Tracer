#include "color.h"

Color::Color()
{
}

Color::Color(double red, double green, double blue)
{
    this->red = red, this->green = green, this->blue = blue;
}

Color Color::operator +(Color color)
{
    return Color(red+color.red, green+color.green, blue+color.blue);
}

void Color::operator +=(Color color)
{
    red += color.red, green += color.green, blue += color.blue;
}

Color Color::operator *(double scalar)
{
    return Color(scalar*red, scalar*green, scalar*blue);
}
