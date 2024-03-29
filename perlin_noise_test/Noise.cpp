#include "Noise.hpp"
#include "Source.hpp"

#include <stdexcept>
#include <random>
#include <time.h>

using namespace std;

const float PI     = 3.141592f;
const float PI_2   = PI * 2.f;
const float SQRT_2 = sqrt(2.f);

const size_t MIN_NOISE_WIDTH  = 1U;
const size_t MIN_NOISE_HEIGHT = 1U;

Noise::~Noise()
{
}

PerlinNoise::PerlinNoise(const size_t _width,
                         const size_t _height,
                         const bool _normed)
{
  resize(_width, _height, _normed);

  reset();
}

// Resize (and reset).
void PerlinNoise::resize(const size_t _width,
                         const size_t _height,
                         const bool _normed)
{
  if (_width < MIN_NOISE_WIDTH)
  {
    throw invalid_argument("PerlinNoise::resize(), _width < MIN_NOISE_WIDTH");
  }

  if (_height < MIN_NOISE_HEIGHT)
  {
    throw invalid_argument("PerlinNoise::resize(), _height < MIN_NOISE_HEIGHT");
  }

  // Exception safety.
  unique_ptr<unique_ptr<Vector2f[]>[]> new_points;
  new_points = make_unique<unique_ptr<Vector2f[]>[]>(_width + 1U);
  for (size_t x = 0; x < _width + 1U; ++x)
  {
    new_points[x] = make_unique<Vector2f[]>(_height + 1U);
  }

  points.swap(new_points);
  width = _width;
  height = _height;
}

void PerlinNoise::reset()
{
  default_random_engine dre{ static_cast<unsigned int>(time(NULL)) };
  uniform_real_distribution<float> angle_distribution{ 0.f, PI_2 };

  if (normed == true)
  {
    for (size_t x = 0; x < width + 1U; ++x)
    {
      for (size_t y = 0; y < height + 1U; ++y)
      {
        const float angle_rad = angle_distribution(dre);
        points[x][y].x = cos(angle_rad);
        points[x][y].y = sin(angle_rad);
      }
    }
  }
  else {
    uniform_real_distribution<float> size_distribution{ 0.f, 1.f };
    for (size_t x = 0; x < width + 1U; ++x)
    {
      for (size_t y = 0; y < height + 1U; ++y)
      {
        const float angle_rad = angle_distribution(dre);
        const float size = size_distribution(dre);
        points[x][y].x = size * cos(angle_rad);
        points[x][y].y = size * sin(angle_rad);
      }
    }
  }
}

void PerlinNoise::generate(Source& _source) const
{
  const size_t noise_width = width;
  const size_t source_width = _source.getWidth();

  if (source_width < noise_width)
  {
    throw invalid_argument("PerlinNoise::generate(), source_width < noise_width");
  }

  const size_t noise_height = height;
  const size_t source_height = _source.getHeight();

  if (source_height < noise_height)
  {
    throw invalid_argument("PerlinNoise::generate(), source_height < noise_height");
  }

  const size_t k_width  = static_cast<size_t>(ceil(static_cast<float>(source_width ) / noise_width ));
  const size_t k_height = static_cast<size_t>(ceil(static_cast<float>(source_height) / noise_height));

  for (size_t cell_x = 0; cell_x < noise_width; ++cell_x)
  {
    for (size_t cell_y = 0; cell_y < noise_height; ++cell_y)
    {
      const Vector2f& top_left_cell     = points[cell_x    ][cell_y    ];
      const Vector2f& top_right_cell    = points[cell_x + 1][cell_y    ];
      const Vector2f& bottom_left_cell  = points[cell_x    ][cell_y + 1];
      const Vector2f& bottom_right_cell = points[cell_x + 1][cell_y + 1];

      for (size_t point_x = 0; point_x < k_width; ++point_x)
      {
        const float normed_point_x = (point_x + 0.5f) / k_width;
        for (size_t point_y = 0; point_y < k_height; ++point_y)
        {
          const float normed_point_y = (point_y + 0.5f) / k_height;

          Vector2f vector_to_top_left    { normed_point_x      , normed_point_y       };
          Vector2f vector_to_top_right   { normed_point_x - 1.f, normed_point_y       };
          Vector2f vector_to_bottom_left { normed_point_x      , normed_point_y - 1.f };
          Vector2f vector_to_bottom_right{ normed_point_x - 1.f, normed_point_y - 1.f };

          // Dot product.
          const float top_left_value     = top_left_cell.x     * vector_to_top_left.x     + top_left_cell.y     * vector_to_top_left.y;
          const float top_right_value    = top_right_cell.x    * vector_to_top_right.x    + top_right_cell.y    * vector_to_top_right.y;
          const float bottom_left_value  = bottom_left_cell.x  * vector_to_bottom_left.x  + bottom_left_cell.y  * vector_to_bottom_left.y;
          const float bottom_right_value = bottom_right_cell.x * vector_to_bottom_right.x + bottom_right_cell.y * vector_to_bottom_right.y;

          auto curve_fixer = [](const float _value)
          {
            return _value * _value * _value * (_value * (_value * 6 - 15) + 10);
          };

          const float top_value    = top_left_value    + curve_fixer(normed_point_x) * (top_right_value    - top_left_value   );
          const float bottom_value = bottom_left_value + curve_fixer(normed_point_x) * (bottom_right_value - bottom_left_value);
                float value        = top_value         + curve_fixer(normed_point_y) * (bottom_value       - top_value        );

          value = (1.f + value) / 2.f;

          const size_t source_x = cell_x * k_width  + point_x;
          const size_t source_y = cell_y * k_height + point_y;

          _source.setValue(source_x, source_y, value);
        }
      }

    }
  }
}