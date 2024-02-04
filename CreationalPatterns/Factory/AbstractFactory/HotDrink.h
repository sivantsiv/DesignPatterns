#pragma once
#include <memory>
#include <map>
#include <string>

struct HotDrink
{
  virtual ~HotDrink() = default;

  virtual void prepare(unsigned int volume) = 0;
};

struct Tea : HotDrink
{
  void prepare(unsigned int volume) override
  {
    std::cout << "Take tea bag, boil water, pour " << volume << "ml, add some lemon" << std::endl;
  }
};

struct Coffee : HotDrink
{
  void prepare(unsigned int volume) override
  {
      std::cout << "Grind some beans, boil water, pour " << volume << "ml, add cream, enjoy!" << std::endl;
  }
};