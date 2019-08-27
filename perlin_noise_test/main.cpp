#include <SFML/Graphics.hpp>
#include <iostream>// DEBUG
#include <memory>

#include "Source.hpp"
#include "Noise.hpp"

using namespace sf;
using namespace std;

const size_t WINDOW_SIZE = 800U;

int main(int argc, char **argv)
{
	RenderWindow render_window{ VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Perlin Noise Test", Style::Close };

	Source source{ WINDOW_SIZE, WINDOW_SIZE };
	PerlinNoise perlinNoise{ 21U, 21U };
	
	perlinNoise.generate(source);

	// Convert source to Image.
	Image image;
	image.create(source.getWidth(), source.getHeight(), Color::Black);
	for (size_t x = 0; x < image.getSize().x; ++x)
	{
		for (size_t y = 0; y < image.getSize().y; ++y)
		{
			const Uint8 brightness = 255 * source.getPoint(x, y);
			const Color color{ brightness, brightness, brightness, 255U };
			image.setPixel(x, y, color);
		}
	}
	Texture texture;
	texture.loadFromImage(image);

	Sprite sprite;
	sprite.setTexture(texture, true);
	
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
		render_window.draw(sprite);
		render_window.display();
	}

	return 0;
}