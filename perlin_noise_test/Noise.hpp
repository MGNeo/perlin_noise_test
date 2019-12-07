#pragma once

#include <memory>

extern const size_t MIN_NOISE_WIDTH;
extern const size_t MIN_NOISE_HEIGHT;

class Source;

class Noise
{
public:
  virtual void generate(Source& _source) const = 0;
  virtual ~Noise();
};

class PerlinNoise : public Noise
{
public:

  PerlinNoise(const size_t _width,
              const size_t _height,
              const bool _normed);

  void resize(const size_t _width,
              const size_t _height,
              const bool _normed);

  void reset();

  void generate(Source& _source) const override;

private:
  size_t width;
  size_t height;

  bool normed;

  struct Vector2f
  {
    float x, y;
  };

  std::unique_ptr<std::unique_ptr<Vector2f[]>[]> points;
};

