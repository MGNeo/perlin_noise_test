#include <SFML/Graphics.hpp>
#include <iostream>// DEBUG
#include <memory>

#include "Source.hpp"
#include "Noise.hpp"
#include "Filter.hpp"

using namespace sf;
using namespace std;

const size_t WINDOW_SIZE = 800U;

int main(int argc, char **argv)
{
	RenderWindow render_window{ VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Perlin Noise Test", Style::Close };

	Source source{ WINDOW_SIZE, WINDOW_SIZE };
	PerlinNoise perlin_noise{ 20U, 20U, true};
	PosterizationFilter posterization_filter{ 9U };

	// Convert source to Image.
	Image image;
	image.create(source.getWidth(), source.getHeight(), Color::Black);

	Texture texture;

	Sprite sprite;
	
	while (render_window.isOpen() == true)
	{
		Event event;
		while (render_window.pollEvent(event) == true)
		{
			if (event.type == Event::Closed)
			{
				render_window.close();
			}
			if (event.type == Event::KeyPressed)
			{
				perlin_noise.reset();
				perlin_noise.generate(source);


				posterization_filter.apply(source);
				for (size_t x = 0; x < image.getSize().x; ++x)
				{
					for (size_t y = 0; y < image.getSize().y; ++y)
					{
						const Uint8 brightness = 255 * source.getPoint(x, y);
						const Color color{ brightness, brightness, brightness, 255U };
						image.setPixel(x, y, color);
					}
				}
				texture.loadFromImage(image);
				sprite.setTexture(texture, true);
			}
		}

		render_window.clear();
		render_window.draw(sprite);
		render_window.display();
	}

	return 0;
}