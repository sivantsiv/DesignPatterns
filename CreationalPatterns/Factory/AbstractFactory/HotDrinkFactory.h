#pragma once
#include "HotDrink.h"

struct HotDrinkFactory
{
  virtual std::unique_ptr<HotDrink> make() const = 0;
};