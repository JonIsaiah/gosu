#include <Gosu/Color.hpp>
#include <Gosu/Math.hpp>
#include <algorithm>

namespace
{
    struct HSV { double h, s, v; };
    
    HSV colorToHSV(const Gosu::Color& c)
    {
        double r = c.red() / 255.0;
        double g = c.green() / 255.0;
        double b = c.blue() / 255.0;
        
        double min = std::min(std::min(r, g), b);
        double max = std::max(std::max(r, g), b);

        if (max == 0)
        {
            HSV hsv = { 0, 0, 0 };
            return hsv;
        }
        
        // Value.
        HSV hsv = { -1, -1, max };
        
        // Saturation.
        double delta = max - min;
        hsv.s = delta / max;
        
        // Hue.
        if (r == max)
            hsv.h = (g - b) / delta + (g < b ? 6 : 0);
        else if (g == max)
            hsv.h = (b - r) / delta + 2;
        else
            hsv.h = (r - g) / delta + 4;
        hsv.h *= 60;
        
        return hsv;
    }
}

Gosu::Color Gosu::Color::fromHSV(double h, double s, double v)
{
    return fromAHSV(255, h, s, v);
}

Gosu::Color Gosu::Color::fromAHSV(Channel alpha, double h, double s, double v)
{
	if (s == 0)
		// Grey.
        return Color(alpha, v * 255, v * 255, v * 255);
    
    // Normalize hue
    h = ((static_cast<int>(h) % 360) + 360) % 360;
    
	int sector = h / 60;
    double factorial = h / 60 - sector;
    
	double p = v * (1 - s);
	double q = v * (1 - s * factorial);
	double t = v * (1 - s * (1 - factorial));
	
    switch (sector) 
    {
    case 0:
        return Color(alpha, v * 255, t * 255, p * 255);
    case 1:
        return Color(alpha, q * 255, v * 255, p * 255);
    case 2:
        return Color(alpha, p * 255, v * 255, t * 255);
    case 3:
        return Color(alpha, p * 255, q * 255, v * 255);
    case 4:
        return Color(alpha, t * 255, p * 255, v * 255);
    default: // sector 5
        return Color(alpha, v * 255, p * 255, q * 255);
    }
}

double Gosu::Color::hue() const
{
    return colorToHSV(*this).h;
}

void Gosu::Color::setHue(double h)
{
    *this = fromAHSV(alpha(), h, saturation(), value());
}

double Gosu::Color::saturation() const
{
    return colorToHSV(*this).s;
}

void Gosu::Color::setSaturation(double s)
{
    *this = fromAHSV(alpha(), hue(), s, value());
}

double Gosu::Color::value() const
{
    return colorToHSV(*this).v;
}

void Gosu::Color::setValue(double v)
{
    *this = fromAHSV(alpha(), hue(), saturation(), v);
}

Gosu::Color Gosu::interpolate(Color a, Color b, double weight)
{
    return Color(boundBy<int>(round(Gosu::interpolate(a.alpha(), b.alpha(), weight)), 0, 255),
                 boundBy<int>(round(Gosu::interpolate(a.red(),   b.red(),   weight)), 0, 255),
                 boundBy<int>(round(Gosu::interpolate(a.green(), b.green(), weight)), 0, 255),
                 boundBy<int>(round(Gosu::interpolate(a.blue(),  b.blue(),  weight)), 0, 255));
}

Gosu::Color Gosu::multiply(Color a, Color b)
{
    return Color(round(a.alpha() * b.alpha() / 255.0),
                 round(a.red()   * b.red()   / 255.0),
                 round(a.green() * b.green() / 255.0),
                 round(a.blue()  * b.blue()  / 255.0));
}
