#include "Source.hpp"

#include <stdexcept>
#include <iostream>//debug

using namespace std;

const float MIN_SOURCE_VALUE = 0.f;
const float MAX_SOURCE_VALUE = 1.f;

const size_t MIN_SOURCE_WIDTH = 1U;
const size_t MIN_SOURCE_HEIGHT = 1U;


Source::Source(const size_t _width,
               const size_t _height)
{
  resize(_width, _height);
}

// Resize (and reset).
void Source::resize(const size_t _width,
                    const size_t _height)
{
  if (_width <= MIN_SOURCE_WIDTH)
  {
    throw invalid_argument("Source::resize(), _width <= MIN_SOURCE_WIDTH");
  }

  if (_height <= MIN_SOURCE_HEIGHT)
  {
    throw invalid_argument("Source::resize(), _height <= MIN_SOURCE_HEIGHT");
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

float Source::getValue(const size_t _x,
  const size_t _y) const
{
  if (_x >= width)
  {
    throw invalid_argument("Source::getValue(), _x >= width");
  }

  if (_y >= height)
  {
    throw invalid_argument("Source::getValue(), _y >= height");
  }

  return points[_x][_y];
}

void Source::setValue(const size_t _x,
                      const size_t _y,
                      const float _value)
{
  if (_x >= width)
  {
    throw invalid_argument("Source::setValue(), _x >= width");
  }

  if (_y >= height)
  {
    throw invalid_argument("Source::setValue(), _y >= height");
  }

  if (_value < MIN_SOURCE_VALUE)
  {
    throw invalid_argument("Source::setValue(), _value < MIN_SOURCE_VALUE");
  }

  if (_value > MAX_SOURCE_VALUE)
  {
    throw invalid_argument("Source::setValue(), _value > MAX_SOURCE_VALUE");
  }

  points[_x][_y] = _value;
}