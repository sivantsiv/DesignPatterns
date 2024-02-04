#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

enum class PointType
{
    cartesian,
    polar
};

// Factory Method
class Point
{
    Point(const float x, const float y)
        : x{ x },
        y{ y }
    {
    }

public:
    float x, y;


    friend std::ostream& operator<<(std::ostream& os, const Point& obj)
    {
        return os
            << "x: " << obj.x
            << "; y: " << obj.y;
    }

    static Point NewCartesian(float x, float y)
    {
        return{ x,y };
    }
    static Point NewPolar(float r, float theta)
    {
        return{ r * cos(theta), r * sin(theta) };
    }
};

// Creational Factory
class Point1
{
public:
    Point1(float x, float y) : x(x), y(y) {}
    float x, y;

    friend std::ostream& operator<<(std::ostream& os, const Point1& obj)
    {
        return os
            << "x: " << obj.x
            << "; y: " << obj.y;
    }
};

class PointFactory
{
public:
    static Point1 NewCartesian(float x, float y)
    {
        return { x,y };
    }

    static Point1 NewPolar(float r, float theta)
    {
        return { r * cos(theta), r * sin(theta) };
    }
};

// Inner factory
class Point2
{
    Point2(float x, float y) : x(x), y(y) {}

public:
    class PointFactory
    {
        PointFactory() {}
    public:
        static Point2 NewCartesian(float x, float y)
        {
            return { x,y };
        }
        static Point2 NewPolar(float r, float theta)
        {
            return{ r * cos(theta), r * sin(theta) };
        }
    };

    float x, y;
    static PointFactory Factory;

    friend std::ostream& operator<<(std::ostream& os, const Point2& obj)
    {
        return os
            << "x: " << obj.x
            << "; y: " << obj.y;
    }
};

int main()
{
    // Test factory method
    {
        auto point = Point::NewPolar(5.0, static_cast<float>(M_PI_4));
        std::cout << point << std::endl;
    }

    // Test creational factory
    {
        auto point = PointFactory::NewPolar(4.0, static_cast<float>(M_PI_2));
        std::cout << point << std::endl;
    }

    // Test inner factory
    {
        auto point = Point2::PointFactory::NewCartesian(2.0, 3.0);
        std::cout << point << std::endl;
    }

    return 0;
}
