#include <iostream>
#include <memory>
#include <map>
#include "HotDrink.h"
#include "DrinkFactory.h"

std::unique_ptr<HotDrink> make_drink(std::string type, unsigned int volume = 200)
{
    std::unique_ptr<HotDrink> drink;
    if (type == "tea")
    {
        drink = std::make_unique<Tea>();
        drink->prepare(volume);
    }
    else
    {
        drink = std::make_unique<Coffee>();
        drink->prepare(volume);
    }
    return drink;
}

int main()
{
    auto d = make_drink("tea");

    DrinkFactory df;
    df.make_drink("coffee", 100);

    return 0;
}
