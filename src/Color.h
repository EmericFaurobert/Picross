#pragma once

using uchar = unsigned char;

enum class CustomColor : char { white, gray, black, red, green, blue };

struct Color {
	Color(uchar r = 255, uchar g = 255, uchar b = 255) : red(r), green(g), blue(b) {}
	
	Color(CustomColor color)
	{
		switch (color)
		{
			case CustomColor::black:
				red = 0; green = 0; blue = 0;
				break;
			case CustomColor::gray:
				red = 128; green = 128; blue = 128;
				break;
			case CustomColor::red:
				red = 255; green = 0; blue = 0;
				break;
			case CustomColor::green:
				red = 0; green = 255; blue = 0;
				break;
			case CustomColor::blue:
				red = 0; green = 0; blue = 255;
				break;
			case CustomColor::white:
			default:
				red = 255; green = 255; blue = 255;
				break;
		}
	}

	friend bool operator==(Color const& c1, Color const& c2) { return c1.red == c2.red && c1.green == c2.green && c1.blue == c2.blue; }
	friend bool operator!=(Color const& c1, Color const& c2) { return !(c1 == c2); }
	
	QString ToRgb() const { return QString("rgb(%1, %2, %3)").arg(red).arg(green).arg(blue); }
	QString ToHex() const { return QColor(red, green, blue).name(); }

	uchar red = 0; 
	uchar green = 0; 
	uchar blue = 0; 
};