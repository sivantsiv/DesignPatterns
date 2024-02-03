#include <cstdio>
#include <vector>
#include <iostream>
#include <array>
#include <numeric>
#include <boost/predef/library/c.h>

// Neural network =============================================================

struct Neuron;

template <typename Self>
struct SomeNeurons
{
    template <typename T> void connect_to(T& other)
    {
        for (Neuron& from : *static_cast<Self*>(this))
        {
            for (Neuron& to : other)
            {
                from.out.push_back(&to);
                to.in.push_back(&from);
            }
        }
    }
};

struct Neuron : SomeNeurons<Neuron>
{
    std::vector<Neuron*> in, out;
    unsigned int id;

    Neuron()
    {
        static int id = 1;
        this->id = id++;
    }

    Neuron* begin() { return this; }
    Neuron* end() { return this + 1; }

    friend std::ostream& operator<<(std::ostream& os, const Neuron& obj)
    {
        for (Neuron* n : obj.in)
        {
            os << n->id << "\t-->\t[" << obj.id << "]" << std::endl;
        }

        for (Neuron* n : obj.out)
        {
            os << "[" << obj.id << "]\t-->\t" << n->id << std::endl;
        }
        return os;
    }
};

struct NeuronLayer : std::vector<Neuron>, SomeNeurons<NeuronLayer>
{
    NeuronLayer(int count)
    {
        while (count-- > 0)
            emplace_back(Neuron{});
    }

    friend std::ostream& operator<<(std::ostream& os, NeuronLayer& obj)
    {

        for (auto& n : obj) os << n;
        return os;
    }
};

// Array backed Properties ====================================================

class Creature
{
    enum Abilities { str, agl, intl, count };
    std::array<int, count> abilities;
public:
    int get_strength() const { return abilities[str]; }
    void set_strength(int value) { abilities[str] = value; }

    int get_agility() const { return abilities[agl]; }
    void set_agility(int value) { abilities[agl] = value; }

    int get_intelligence() const { return abilities[intl]; }
    void set_intelligence(int value) { abilities[intl] = value; }

    int sum() const {
        return std::accumulate(abilities.begin(), abilities.end(), 0);
    }

    double average() const {
        return sum() / (double)count;
    }

    int max() const {
        return *std::max_element(abilities.begin(), abilities.end());
    }
};

int main()
{
    // Test neural network
    {
        Neuron n1, n2;
        n1.connect_to(n2);

        std::cout << n1 << n2 << std::endl;

        NeuronLayer l1{ 5 };
        Neuron n3;
        l1.connect_to(n3);

        std::cout << "Neuron " << n3.id << std::endl << n3 << std::endl;
        std::cout << "Layer " << std::endl << l1 << std::endl;

        NeuronLayer l2{ 2 }, l3{ 3 };
        l2.connect_to(l3);
        std::cout << "Layer l2" << std::endl << l2;
        std::cout << "Layer l3" << std::endl << l3;
    }

    // Test array backed properties
    {
        Creature orc;
        orc.set_strength(16);
        orc.set_agility(11);
        orc.set_intelligence(9);

        std::cout << "The orc has an average stat of "
                  << orc.average()
                  << " and a maximum stat of "
                  << orc.max()
                  << std::endl;
    }

    return 0;
}
