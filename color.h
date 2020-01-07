#ifndef COLOR_H
#define COLOR_H

class Color
{
public:

    double red, green, blue;

    Color();
    Color(double red, double green, double blue);
    Color operator +(Color color);
    void operator +=(Color color);
    Color operator *(double scalar);
};

#endif // COLOR_H
