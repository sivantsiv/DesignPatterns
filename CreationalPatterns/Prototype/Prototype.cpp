#include <string>
#include <iostream>
#include <memory>
#include <functional>
#include <sstream>
#include <boost/serialization/serialization.hpp>

struct Address
{
    std::string street;
    std::string city;
    int suite;


    Address(const std::string& street, const std::string& city, const int suite)
        : street{ street },
        city{ city },
        suite{ suite }
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Address& obj)
    {
        return os
            << "street: " << obj.street
            << " city: " << obj.city
            << " suite: " << obj.suite;
    }
};

struct Contact
{
    std::string name;
    Address* address;

    Contact& operator=(const Contact& other)
    {
        if (this == &other)
            return *this;
        name = other.name;
        address = other.address;
        return *this;
    }

    Contact() : name(nullptr), address(nullptr)
    {} // required for serialization

    Contact(std::string name, Address* address)
        : name{ name }, address{ address }
    {
        //this->address = new Address{ *address };
    }

    Contact(const Contact& other)
        : name{ other.name }
        //, address{ new Address{*other.address} }
    {
        address = new Address(
            other.address->street,
            other.address->city,
            other.address->suite
        );
    }

private:
    friend class boost::serialization::access;

    template <class archive>
    void save(archive& ar, const unsigned version) const
    {
        ar << name;
        ar << address;
    }

    template <class archive>
    void load(archive& ar, const unsigned version)
    {
        ar >> name;
        ar >> address;
    }

public:
    ~Contact()
    {
        delete address;
    }


    friend std::ostream& operator<<(std::ostream& os, const Contact& obj)
    {
        return os << "name: " << obj.name
                  << " works at " << *obj.address; // note the star here
    }
};

struct EmployeeFactory
{
    static Contact main;
    static Contact aux;

    static std::unique_ptr<Contact> NewMainOfficeEmployee(std::string name, int suite)
    {
        // Static Contact p{ "", new Address{ "123 East Drive", "London", 0 } };
        return NewEmployee(name, suite, main);
    }

    static std::unique_ptr<Contact> NewAuxOfficeEmployee(std::string name, int suite)
    {
        return NewEmployee(name, suite, aux);
    }

private:
    static std::unique_ptr<Contact> NewEmployee(std::string name, int suite, Contact& proto)
    {
        auto result = std::make_unique<Contact>(proto);
        result->name = name;
        result->address->suite = suite;
        return result;
    }
};

Contact EmployeeFactory::main{ "", new Address{ "123 East Drive", "London", 0 } };
Contact EmployeeFactory::aux{ "", new Address{ "123 East Drive", "London", 0 } };

int main()
{
    // Prototype
    {
        Contact john{ "John Doe", new Address{"123 East Drive", "London", 123} };
        Contact jane{ john };    // shallow copy

        jane.name = "Jane Doe",
        jane.address->suite = 103;

        std::cout << john << std::endl << jane << std::endl;
    }

    // Prototype factory
    {
        auto john = EmployeeFactory::NewMainOfficeEmployee("John Doe", 123);
        auto jane = EmployeeFactory::NewMainOfficeEmployee("Jane Doe", 125);

        std::cout << std::endl << *john << std::endl << *jane << std::endl; // note the stars here
    }

    return 0;
}
