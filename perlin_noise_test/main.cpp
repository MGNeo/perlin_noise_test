#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

const float PI = 3.141592653589793238463f;
const float GRAD_TO_RAD_K = 180U / PI;

const size_t WINDOW_SIZE = 800U;

const size_t POINT_COUNT = 21;
const size_t CELL_SIZE = WINDOW_SIZE / (POINT_COUNT - 1);
const size_t CELL_D = sqrt(2.f) * CELL_SIZE;

class PerlinNoise
{
public:
	PerlinNoise()
	{
		image.create(WINDOW_SIZE, WINDOW_SIZE, Color::Black);

		for (size_t point_x = 0; point_x < POINT_COUNT; ++point_x)
		{
			for (size_t point_y = 0; point_y < POINT_COUNT; ++point_y)
			{
				/*const float angle_grad = rand() % 360U;
				const float angle_rad = angle_grad * GRAD_TO_RAD_K;
				points[point_x][point_y].x = cos(angle_rad);
				points[point_x][point_y].y = sin(angle_rad);*/
				
				points[point_x][point_y].x = rand() % 3 - 1;
				points[point_x][point_y].y = rand() % 3 - 1;
				
			}
		}

float max = 0.f;
float min = 0.f;

		for (size_t point_x = 0; point_x < POINT_COUNT - 1U; ++point_x)
		{
			for (size_t point_y = 0; point_y < POINT_COUNT - 1U; ++point_y)
			{
				const Vector2f &top_left_gradient     = points[point_x    ][point_y    ];
				const Vector2f &top_right_gradient    = points[point_x + 1][point_y    ];
				const Vector2f &bottom_left_gradient  = points[point_x    ][point_y + 1];
				const Vector2f &bottom_right_gradient = points[point_x + 1][point_y + 1];

				for (size_t pixel_x = 0; pixel_x < CELL_SIZE; ++pixel_x)
				{
					for (size_t pixel_y = 0; pixel_y < CELL_SIZE; ++pixel_y)
					{
						// Потом поправить учет + 0.5f;
						const float local_x_float = (pixel_x + 0.5f) / (float)CELL_SIZE;
						const float local_y_float = (pixel_y + 0.5f) / (float)CELL_SIZE;

						Vector2f vector_to_top_left     = { local_x_float    , local_y_float     };
						Vector2f vector_to_top_right    = { local_x_float - 1, local_y_float     };
						Vector2f vector_to_bottom_left  = { local_x_float    , local_y_float - 1 };
						Vector2f vector_to_bottom_right = { local_x_float - 1, local_y_float - 1 };

						const float top_left_value     = top_left_gradient.x     * vector_to_top_left.x     + top_left_gradient.y     * vector_to_top_left.y    ;
						const float top_right_value    = top_right_gradient.x    * vector_to_top_right.x    + top_right_gradient.y    * vector_to_top_right.y   ;
						const float bottom_left_value  = bottom_left_gradient.x  * vector_to_bottom_left.x  + bottom_left_gradient.y  * vector_to_bottom_left.y ;
						const float bottom_right_value = bottom_right_gradient.x * vector_to_bottom_right.x + bottom_right_gradient.y * vector_to_bottom_right.y;

						auto cc = [](const float _t)->float
						{
							return _t * _t * _t * (_t * (_t * 6 - 15) + 10);
						};

						const float top_value    = top_left_value    + cc(local_x_float) * (top_right_value    - top_left_value   );
						const float bottom_value = bottom_left_value + cc(local_x_float) * (bottom_right_value - bottom_left_value);

						const float value = top_value + cc(local_y_float) * (bottom_value - top_value);

						if (value > max) max = value;
						if (value < min) min = value;

						// Почему value не попадает в интервал [0.f; 1.f] ???

						Uint8 brightness = 128 + value * 128;

						const size_t x = point_x * CELL_SIZE + pixel_x;
						const size_t y = point_y * CELL_SIZE + pixel_y;

						image.setPixel(x, y, Color(brightness, brightness, brightness, brightness));
					}
				}
			}
		}

		cout << "min: " << min << endl;
		cout << "max: " << max << endl;

		texture.loadFromImage(image);
		sprite.setTexture(texture, true);
	}

	void draw(RenderWindow &_render_window)
	{
		_render_window.draw(sprite);
	}
private:
	Vector2f points[POINT_COUNT][POINT_COUNT];
	Image image;
	Texture texture;
	Sprite sprite;
};

int main(int argc, char **argv)
{
	srand(time(NULL));

	RenderWindow render_window{ VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Perlin Noise Test", Style::Close };

	PerlinNoise perlin_noise;

	while (render_window.isOpen() == true)
	{
		Event event;
		while (render_window.pollEvent(event) == true)
		{
			if (event.type == Event::Closed)
			{
				render_window.close();
			}
		}

		render_window.clear();
		perlin_noise.draw(render_window);
		render_window.display();
	}

	return 0;
}