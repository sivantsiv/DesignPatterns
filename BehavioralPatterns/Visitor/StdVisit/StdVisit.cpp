#include <iostream>
#include <string>
#include <variant>

struct AddressPrinter
{
    void operator()(const std::string& house_name) const {
        std::cout << "A house called " << house_name << std::endl;
    }

    void operator()(const int house_number) const {
        std::cout << "House number " << house_number << std::endl;
    }
};

int main(int ac, char* av[])
{
    std::variant<std::string, int> house;
    // house = "Montefiore Castle";
    house = 221;

    AddressPrinter printer;
    std::visit(printer, house);

    // Visitor implementation using std::visit and lambda function
    std::visit([](auto& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, std::string>)
        {
            std::cout << "A house called " << arg.c_str() << std::endl;
        }
        else
        {
            std::cout << "House number " << arg << std::endl;
        }
        }, house);

    return 0;
}
