#pragma once

#include <memory>

class Source;

class Noise
{
public:
	virtual void generate(Source &_source) const = 0;
	virtual ~Noise();
};

class PerlinNoise : public Noise
{
public:
	PerlinNoise(const size_t _width,
				const size_t _height);

	void resize(const size_t _width,
				const size_t _height);

	void reset();

	void generate(Source &_source) const override;
private:
	size_t width;
	size_t height;

	struct Vector2f
	{
		float x, y;
	};

	std::unique_ptr<std::unique_ptr<Vector2f[]>[]> points;
};

