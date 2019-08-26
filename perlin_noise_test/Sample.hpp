#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

namespace perlin_noise
{
	class Sample
	{
	public:
		Sample(const size_t _width,
			   const size_t _height);

		void resizeGrid(const size_t _width,
			            const size_t _height);

		void resetGrid();

		const sf::Vector2f &getPoint(const size_t _x,
			                         const size_t _y) const;

		size_t getWidth() const;
		size_t getHeight() const;
	private:
		size_t width_point_count;
		size_t height_point_count;
		std::unique_ptr<std::unique_ptr<sf::Vector2f[]>[]> points;
	};
};