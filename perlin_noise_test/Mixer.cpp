#include "Mixer.hpp"

#include <stdexcept>

using namespace perlin_noise;
using namespace std;
using namespace sf;

void Mixer::push_back(unique_ptr<Sample> &_sample,
				      unique_ptr<Mode> &_mode)
{
	if (_sample == nullptr)
	{
		throw invalid_argument("Mixer::add(), _sample == nullptr");
	}
	
	if (_mode == nullptr)
	{
		throw invalid_argument("Mixer::add(), _mode == nullptr");
	}

	samples.push_back(move(_sample));
	modes.push_back(move(_mode));

}

// Будет ли тут копирование, что в sfml с семантикой перемещения?
Texture Mixer::generateTexture(const size_t _width,
							   const size_t _height)
{
	if (_width == 0)
	{
		throw invalid_argument("Mixer::generateTexture(), _width == 0");
	}

	if (_height == 0)
	{
		throw invalid_argument("Mixer::generateTexture(), _height == 0");
	}

	Image image;
	image.create(_width, _height, Color::Black);

	// Первому сэмплу не нужен режим.

	// Нужно првоерять, чтобы клеток в семпле не было больше, чем пикселей в текстуре.
	// И подумать над ситуацией, когда точка внутри квадрата попадает ни к селу, ни к городу.

	auto is = samples.begin();
	auto im = modes.begin();
	
	while (is != samples.end())
	{
		const size_t cells_per_width = (*is)->getWidth() - 1U;
		const size_t cells_per_height = (*is)->getHeight() - 1U;
		
		const size_t points_per_cell_width = _width / cells_per_width;
		const size_t points_per_cell_height = _height / cells_per_height;

		for (size_t cell_x = 0; cell_x < cells_per_width; ++cell_x)
		{
			for (size_t cell_y = 0; cell_y < cells_per_height; ++cell_y)
			{
				const Vector2f &top_left_cell     = (*is)->getPoint(cell_x    , cell_y    );
				const Vector2f &top_right_cell    = (*is)->getPoint(cell_x + 1, cell_y    );
				const Vector2f &bottom_left_cell  = (*is)->getPoint(cell_x    , cell_y + 1);
				const Vector2f &bottom_right_cell = (*is)->getPoint(cell_x + 1, cell_y + 1);

				for (size_t point_x = 0; point_x < points_per_cell_width; ++point_x)
				{
					const float normed_point_x = point_x / (float) points_per_cell_width;
					for (size_t point_y = 0; point_y < points_per_cell_width; ++point_y)
					{
						const float normed_point_y = point_y / (float)points_per_cell_height;

						const Vector2f vector_to_top_left     { normed_point_x      , normed_point_y       };
						const Vector2f vector_to_top_right    { normed_point_x - 1.f, normed_point_y       };
						const Vector2f vector_to_bottom_left  { normed_point_x      , normed_point_y - 1.f };
						const Vector2f vector_to_bottom_right { normed_point_x - 1.f, normed_point_y - 1.f };
						
						// Dot product.
						const float top_left_value     = top_left_cell.x     * vector_to_top_left.x     + top_left_cell.y     * vector_to_top_left.y    ;
						const float top_right_value    = top_right_cell.x    * vector_to_top_right.x    + top_right_cell.y    * vector_to_top_right.y   ;
						const float bottom_left_value  = bottom_left_cell.x  * vector_to_bottom_left.x  + bottom_left_cell.y  * vector_to_bottom_left.y ;
						const float bottom_right_value = bottom_right_cell.x * vector_to_bottom_right.x + bottom_right_cell.y * vector_to_bottom_right.y;

						auto curve_fixer = [](const float _value)
						{
							return _value * _value * _value * (_value * (_value * 6 - 15) + 10);
						};

						const float top_value    = top_left_value    + curve_fixer(normed_point_x) * (top_right_value    - top_left_value   );
						const float bottom_value = bottom_left_value + curve_fixer(normed_point_x) * (bottom_right_value - bottom_left_value);
						const float value        = top_value         + curve_fixer(normed_point_y) * (bottom_value       - top_value        );

						
					}
				}
				
			}
		}

		++is;
		++im;
	}
}