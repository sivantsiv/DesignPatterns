#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

struct Shape
{
    virtual std::string str() const = 0;
};

struct Circle : Shape
{
    float radius;

    Circle() {}

    explicit Circle(const float radius)
        : radius{ radius }
    {
    }

    void resize(float factor)
    {
        radius *= factor;
    }

    std::string str() const override
    {
        std::ostringstream oss;
        oss << "A circle of radius " << radius;
        return oss.str();
    }
};

struct Square : Shape
{
    float side;

    Square() {}

    explicit Square(const float side)
        : side{ side }
    {
    }

    std::string str() const override
    {
        std::ostringstream oss;
        oss << "A square of with side " << side;
        return oss.str();
    }
};

// We are not changing the base class of existing objects
// cannot make, e.g., ColoredSquare, ColoredCircle, etc.

struct ColoredShape : Shape
{
    Shape& shape;
    std::string color;

    ColoredShape(Shape& shape, const std::string& color)
        : shape{ shape },
        color{ color }
    {
    }

    std::string str() const override
    {
        std::ostringstream oss;
        oss << shape.str() << " has the color " << color;
        return oss.str();
    }
};

struct TransparentShape : Shape
{
    Shape& shape;
    uint8_t transparency;


    TransparentShape(Shape& shape, const uint8_t transparency)
        : shape{ shape },
        transparency{ transparency }
    {
    }

    std::string str() const override
    {
        std::ostringstream oss;
        oss << shape.str() << " has "
            << static_cast<float>(transparency) / 255.f * 100.f
            << "% transparency";
        return oss.str();
    }
};

//=============================================================================

// Note: class, not typename
template <typename T> struct ColoredShape2 : T
{
    static_assert(std::is_base_of<Shape, T>::value,
        "Template argument must be a Shape");

    std::string color;

    // need this (or not!)
    ColoredShape2() {}

    template <typename...Args>
    ColoredShape2(const std::string& color, Args ...args)
        : T(std::forward<Args>(args)...), color{ color }
        // you cannot call base class ctor here
        // b/c you have no idea what it is
    {
    }

    std::string str() const override
    {
        std::ostringstream oss;
        oss << T::str() << " has the color " << color;
        return oss.str();
    }
};

template <typename T> struct TransparentShape2 : T
{
    uint8_t transparency;

    template<typename...Args>
    TransparentShape2(const uint8_t transparency, Args ...args)
        : T(std::forward<Args>(args)...), transparency{ transparency }
    {
    }

    std::string str() const override
    {
        std::ostringstream oss;
        oss << T::str() << " has "
            << static_cast<float>(transparency) / 255.f * 100.f
            << "% transparency";
        return oss.str();
    }
};

//=============================================================================

struct Logger
{
    std::function<void()> func;
    std::string name;

    Logger(const std::function<void()>& func, const std::string& name)
        : func{ func },
        name{ name }
    {
    }

    void operator()() const
    {
        std::cout << "Entering " << name << std::endl;
        func();
        std::cout << "Exiting " << name << std::endl;
    }
};

template <typename Func>
struct Logger2
{
    Func func;
    std::string name;

    Logger2(const Func& func, const std::string& name)
        : func{ func },
        name{ name }
    {
    }

    void operator()() const
    {
        std::cout << "Entering " << name << std::endl;
        func();
        std::cout << "Exiting " << name << std::endl;
    }
};

template <typename Func> auto make_logger2(Func func,
    const std::string& name)
{
    return Logger2<Func>{ func, name };
}

// need partial specialization for this to work
template <typename> struct Logger3;

// return type and argument list
template <typename R, typename... Args>
struct Logger3<R(Args...)>
{
    Logger3(std::function<R(Args...)> func, const std::string& name)
        : func{ func },
        name{ name }
    {
    }

    R operator() (Args ...args)
    {
        std::cout << "Entering " << name << std::endl;
        R result = func(args...);
        std::cout << "Exiting " << name << std::endl;
        return result;
    }

    std::function<R(Args ...)> func;
    std::string name;
};

template <typename R, typename... Args>
auto make_logger3(R(*func)(Args...), const std::string& name)
{
    return Logger3<R(Args...)>(
        std::function<R(Args...)>(func),
        name);
}

double add(double a, double b)
{
    std::cout << a << "+" << b << "=" << (a + b) << std::endl;
    return a + b;
}

int main()
{
    // Dynamic decorator
    {
        Circle circle{ 5 };
        std::cout << circle.str() << std::endl;

        ColoredShape red_circle{ circle, "red" };
        std::cout << red_circle.str() << std::endl;

        TransparentShape red_half_visible_circle{ red_circle, 128 };
        std::cout << red_half_visible_circle.str() << std::endl << std::endl;
    }

    // Static decorator
    {
        // Won't work without a default constructor
        ColoredShape2<Circle> green_circle{ "green", 5 };
        std::cout << green_circle.str() << std::endl;

        TransparentShape2<ColoredShape2<Square>> blue_invisible_square{ 0, "blue", 10 };
        blue_invisible_square.color = "light blue";
        blue_invisible_square.side = 21;
        std::cout << blue_invisible_square.str() << std::endl << std::endl;
    }

    // Function decorator
    {
        Logger([]() {std::cout << "Hello" << std::endl; }, "HelloFunction")();

        // cannot do this
        //make_logger2([]() {std::cout << "Hello" << std::endl; }, "HelloFunction")();
        auto call = make_logger2([]() {std::cout << "Hello!" << std::endl; }, "HelloFunction");
        call();

        auto logged_add = make_logger3(add, "Add");
        auto result = logged_add(2, 3);
        std::cout << std::endl;
    }
    
    // Constructor forwarding
    {
        struct NotAShape
        {
            virtual std::string str() const { return std::string{}; }
        };

        // we don't want this to be legal, thus a static_assert above
        //ColoredShape2<NotAShape> legal;

        // no code completion for this case
        // can comment out argument, too! (default constructor)
        TransparentShape2<Square> hidden_square{ 1, 2 };
        std::cout << hidden_square.str() << std::endl;

        ColoredShape2<TransparentShape2<Square>> sq = { "red", 51, 5 };
        std::cout << sq.str() << std::endl;
    }

    return 0;
}
