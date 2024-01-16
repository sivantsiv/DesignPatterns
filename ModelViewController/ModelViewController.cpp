#include <iostream>
#include <string>
#include <vector>

#include "ModelViewController.h"

class Model {
public:
    Model() : data_() {}

    void addData(const std::string& data) 
    {
        data_.push_back(data);
    }

    const std::vector<std::string>& getData() const 
    {
        return data_;
    }

private:
    std::vector<std::string> data_;
};

class View {
public:
    void displayData(const std::vector<std::string>& data) const 
    {
        std::cout << "Data:" << std::endl;
        for (const auto& d : data) 
        {
            std::cout << d << std::endl;
        }
    }
};

class Controller {
public:
    Controller(Model& model, View& view) : model_(model), view_(view) {}

    void addData(const std::string& data) 
    {
        model_.addData(data);
    }

    void displayData() const 
    {
        view_.displayData(model_.getData());
    }

private:
    Model& model_;
    View& view_;
};

int main() 
{
    Model model;
    View view;
    Controller controller(model, view);

    controller.addData("Hello");
    controller.addData("World");
    controller.displayData();

    return 0;
}
