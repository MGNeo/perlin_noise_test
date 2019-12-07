#include "Mixer.hpp"
#include "Source.hpp"

#include <stdexcept>

using namespace std;

Mixer::~Mixer()
{
}

SimpleMixer::SimpleMixer(const float _k)
  :
  k{ _k }
{
  if (_k < 0.f)
  {
    throw invalid_argument("SimpleMixer::SimpleMixer(), k < 0.f");
  }

  if (_k > 1.f)
  {
    throw invalid_argument("SimpleMixer::SimpleMixer(), k > 1.f");
  }
}

void SimpleMixer::mix(const Source& _first_source,
                      const Source& _second_source,
                            Source& _result_source) const
{
  if (_first_source.getWidth() != _second_source.getWidth())
  {
    throw invalid_argument("SimpleMixer::mix(), _first_source.getWidth() != _second_source.getWidth()");
  }
  if (_first_source.getWidth() != _result_source.getWidth())
  {
    throw invalid_argument("SimpleMixer::mix(), _first_source.getWidth() != _result_source.getWidth()");
  }
  if (_first_source.getHeight() != _second_source.getHeight())
  {
    throw invalid_argument("SimpleMixer::mix(), _first_source.getHeight() != _second_source.getHeight()");
  }
  if (_first_source.getHeight() != _result_source.getHeight())
  {
    throw invalid_argument("SimpleMixer::mix(), _first_source.getHeight() != _result_source.getHeight()");
  }

  const size_t width = _first_source.getWidth();
  const size_t height = _first_source.getHeight();

  for (size_t x = 0; x < width; ++x)
  {
    for (size_t y = 0; y < height; ++y)
    {
      float new_value = _first_source.getValue(x, y) + k * (_second_source.getValue(x, y) - _first_source.getValue(x, y));
      if (new_value < MIN_SOURCE_VALUE)
      {
        new_value = MIN_SOURCE_VALUE;
      }
      if (new_value > MAX_SOURCE_VALUE)
      {
        new_value = MAX_SOURCE_VALUE;
      }
      _result_source.setValue(x, y, new_value);
    }
  }
}