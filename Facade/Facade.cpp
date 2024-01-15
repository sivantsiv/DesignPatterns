#include <iostream>

class SubsystemA 
{
public:
    void operationA() const 
    {
        std::cout << "Subsystem A operation" << std::endl;
    }
};

class SubsystemB 
{
public:
    void operationB() const 
    {
        std::cout << "Subsystem B operation" << std::endl;
    }
};

class Facade 
{
public:
    Facade() : subsystemA(), subsystemB() {}

    void operation() const 
    {
        std::cout << "Facade operation:" << std::endl;
        subsystemA.operationA();
        subsystemB.operationB();
    }

private:
    SubsystemA subsystemA;
    SubsystemB subsystemB;
};

int main() 
{
    Facade facade;
    facade.operation();

    return 0;
}
