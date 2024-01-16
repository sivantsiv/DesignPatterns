#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

// Adaptee class
class Adaptee 
{
public:
    void specificRequest() 
    {
        std::cout << "Adaptee::specificRequest() executed" << std::endl;
    }
};

// Target interface
class Target 
{
public:
    virtual ~Target() {}
    virtual void request() = 0;
};

// Caching Adapter class
class Adapter : public Target 
{
public:
    Adapter(Adaptee* adaptee) : adaptee_(adaptee) {}
    void request() override 
    {
        if (cache_.find("specificRequest") == cache_.end()) 
        {
            adaptee_->specificRequest();
            cache_["specificRequest"] = true;
        }
        else
        {
            std::cout << "Cached: nothing to be done" << std::endl;
        }
    }
private:
    Adaptee* adaptee_;
    std::unordered_map<std::string, bool> cache_;
};

// Client code
int main() 
{
    std::unique_ptr<Adaptee> adaptee(new Adaptee());
    std::unique_ptr<Adapter> target(new Adapter(adaptee.get()));
    target->request();
    target->request();

    return 0;
}
