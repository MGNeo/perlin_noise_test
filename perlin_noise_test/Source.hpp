#pragma once

#include <memory>

extern const float MIN_SOURCE_VALUE;
extern const float MAX_SOURCE_VALUE;

extern const size_t MIN_SOURCE_WIDTH;
extern const size_t MIN_SOURCE_HEIGHT;

class Source
{
public:
	Source(const size_t _width, 
		   const size_t _height);

	void resize(const size_t _width,
				const size_t _height);

	void reset();

	size_t getWidth() const;
	size_t getHeight() const;

	float getPoint(const size_t _x,
			       const size_t _y) const;

	void setPoint(const size_t _x,
			      const size_t _y,
			      const float _value);
private:
	size_t width;
	size_t height;

	std::unique_ptr<std::unique_ptr<float[]>[]> points;
};

