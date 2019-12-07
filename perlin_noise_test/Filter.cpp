#include "Filter.hpp"
#include "Source.hpp"

#include <set>

#include <stdexcept>
#include <iostream>

using namespace std;

Filter::~Filter()
{
}

LinearPosterizationFilter::LinearPosterizationFilter(const size_t _levels)
  :
  levels{ _levels }
{
  if (levels <= 1U)
  {
    throw invalid_argument("PosterizationFilter::PosterizationFilter(), levels <= 1U");
  }
}

void LinearPosterizationFilter::apply(Source& _source) const
{
  const size_t source_width = _source.getWidth();
  const size_t source_height = _source.getHeight();

  // Find the max and the min values.
  float min_value = MAX_SOURCE_VALUE;
  float max_value = MIN_SOURCE_VALUE;
  for (size_t x = 0; x < source_width; ++x)
  {
    for (size_t y = 0; y < source_height; ++y)
    {
      const float value = _source.getValue(x, y);
      if (value < min_value) min_value = value;
      if (value > max_value) max_value = value;
    }
  }

  // Apply the filter.
  const float range = max_value - min_value;

  // Control of division by zero.
  const float step_value = range / levels;
  if (step_value == 0.f)
  {
    throw invalid_argument("LinearPosterizationFilter::apply(), step_value == 0.f");
  }

  const float half_step_value = step_value / 2.f;

  for (size_t x = 0; x < source_width; ++x)
  {
    for (size_t y = 0; y < source_height; ++y)
    {
      const float value = _source.getValue(x, y);

      const float inter_value = value - min_value;

      const float new_value = min_value + step_value * floor(inter_value / step_value);

      _source.setValue(x, y, new_value);
    }
  }
}

CustomPosterizationFilter::CustomPosterizationFilter(initializer_list<float> _points)
  :
  count{ _points.size() }
{
  if (count == 0U)
  {
    throw invalid_argument("CustomPosterizationFilter::CustomPosterizationFilter(), count == 0U");
  }

  set<float> ordered_points{ _points };

  points = make_unique<float[]>(count);

  size_t p = 0;
  for (auto& op : ordered_points)
  {
    if (op < MIN_SOURCE_VALUE)
    {
      throw invalid_argument("CustomPosterizationFilter::CustomPosterizationFilter(), op < MIN_SOURCE_VALUE");
    }
    if (op > MAX_SOURCE_VALUE)
    {
      throw invalid_argument("CustomPosterizationFilter::CustomPosterizationFilter(), op >= MAX_SOURCE_VALUE");
    }
    points[p++] = op;
  }
}

void CustomPosterizationFilter::apply(Source& _source) const
{
  const size_t source_width = _source.getWidth();
  const size_t source_height = _source.getHeight();

  for (size_t x = 0; x < source_width; ++x)
  {
    for (size_t y = 0; y < source_height; ++y)
    {
      const float value = _source.getValue(x, y);

      float min_abs_value_dist = MAX_SOURCE_VALUE;
      size_t nearest_point = 0U;

      for (size_t p = 0U; p < count; ++p)
      {
        const float new_abs_value_dist = fabs(value - points[p]);
        if (new_abs_value_dist < min_abs_value_dist)
        {
          min_abs_value_dist = new_abs_value_dist;
          nearest_point = p;
        }
        else {
          break;
        }
      }
      _source.setValue(x, y, points[nearest_point]);
    }
  }
}

void RangeCorrectionFilter::apply(Source& _source) const
{
  const size_t width = _source.getWidth();
  const size_t height = _source.getWidth();

  float min_value = MAX_SOURCE_VALUE;
  float max_value = MIN_SOURCE_VALUE;

  // Find the max and the min values.
  for (size_t x = 0; x < width; ++x)
  {
    for (size_t y = 0; y < height; ++y)
    {
      const float value = _source.getValue(x, y);
      if (value < min_value) min_value = value;
      if (value > max_value) max_value = value;
    }
  }

  const float range = max_value - min_value;

  // Control of division by zero.
  if (range == 0.f)
  {
    throw invalid_argument("RangeCorrectionFilter::apply(), range == 0.f");
  }

  const float k = 1.f / range;

  // Correction.
  for (size_t x = 0; x < width; ++x)
  {
    for (size_t y = 0; y < height; ++y)
    {
      const float value = _source.getValue(x, y);
      const float new_value = (value - min_value) * k;
      _source.setValue(x, y, new_value);
    }
  }

}