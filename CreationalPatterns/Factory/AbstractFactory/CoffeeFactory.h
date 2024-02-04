#pragma once
#include <memory>
#include "HotDrink.h"
#include "HotDrinkFactory.h"

struct HotDrink;

struct CoffeeFactory : HotDrinkFactory
{
  std::unique_ptr<HotDrink> make() const override
  {
    return std::make_unique<Coffee>();
  }
};