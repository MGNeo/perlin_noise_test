#pragma once

#include <vector>
#include <initializer_list>
#include <memory>

class Source;

class Filter
{
public:
  virtual void apply(Source& _source) const = 0;
  virtual ~Filter();
};

class LinearPosterizationFilter : public Filter
{
public:
  LinearPosterizationFilter(const size_t _levels = 10U);

  void apply(Source& _source) const override;
private:
  size_t levels;
};

class CustomPosterizationFilter : public Filter
{
public:
  CustomPosterizationFilter(std::initializer_list<float> _points);

  void apply(Source& _source) const override;
private:
  size_t count;
  std::unique_ptr<float[]> points;
};

class RangeCorrectionFilter : public Filter
{
public:
  void apply(Source& _source) const override;
};

// Пороговый фильтр.

// Просветляющий фильтр.

// Затемняющий фильтр.
