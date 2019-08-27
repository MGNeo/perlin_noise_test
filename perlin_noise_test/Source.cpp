#include "Source.hpp"

#include <stdexcept>
#include <iostream>//debug

using namespace std;

Source::Source(const size_t _width,
			   const size_t _height)
{
	resize(_width, _height);
}

// Resize (and reset).
void Source::resize(const size_t _width,
					const size_t _height)
{
	if (_width <= 1U)
	{
		throw invalid_argument("Source::resize(), _width <= 1U");
	}

	if (_height <= 1)
	{
		throw invalid_argument("Source::resize(), _height <= 1Ud");
	}

	// Exception safety.
	unique_ptr<unique_ptr<float[]>[]> new_points;
	new_points = make_unique<unique_ptr<float[]>[]>(_width);
	for (size_t x = 0; x < _width; ++x)
	{
		new_points[x] = make_unique<float[]>(_height);
		for (size_t y = 0; y < _height; ++y)
		{
			new_points[x][y] = 0.f;
		}
	}

	points.swap(new_points);
	width = _width;
	height = _height;
}

void Source::reset()
{
	for (size_t x = 0; x < width; ++x)
	{
		for (size_t y = 0; y < height; ++y)
		{
			points[x][y] = 0.f;
		}
	}
}

size_t Source::getWidth() const
{
	return width;
}

size_t Source::getHeight() const
{
	return height;
}

float Source::getPoint(const size_t _x,
				       const size_t _y) const
{
	if (_x >= width)
	{
		throw invalid_argument("Source::getPoint(), _x >= width");
	}

	if (_y >= height)
	{
		throw invalid_argument("Source::getPoint(), _y >= height");
	}

	return points[_x][_y];
}

void Source::setPoint(const size_t _x,
				      const size_t _y,
				      const float _value)
{
	if (_x >= width)
	{
		throw invalid_argument("Source::setPoint(), _x >= width");
	}

	if (_y >= height)
	{
		throw invalid_argument("Source::setPoint(), _y >= height");
	}

	if (_value < 0.f)
	{
		throw invalid_argument("Source::setPoint(), _value < 0.f");
	}

	if (_value >= 1.f)
	{
		throw invalid_argument("Source::setPoint(), _value > 1.f");
	}

	points[_x][_y] = _value;
}