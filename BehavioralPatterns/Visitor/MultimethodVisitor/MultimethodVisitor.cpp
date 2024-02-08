#include <iostream>
#include <string>
#include <typeindex>
#include <map>
#include <functional>

struct GameObject;
void collide(GameObject& first, GameObject& second);

struct GameObject
{
	virtual ~GameObject() = default;
	virtual  std::type_index type() const = 0;

	virtual void collide(GameObject& other) { ::collide(*this, other); }
};

template <typename T> struct GameObjectImpl : GameObject
{
	std::type_index type() const override
	{
		return typeid(T);
	}
};

struct Planet : GameObjectImpl<Planet> {};
struct Asteroid : GameObjectImpl<Asteroid> {};
struct Spaceship : GameObjectImpl<Spaceship> {};
struct ArmedSpaceship : Spaceship
{
	std::type_index type() const override {
		return typeid(ArmedSpaceship); // required for collision to function
	}
}; // model limitation

void spaceship_planet() { std::cout << "spaceship lands on planet" << std::endl; }
void asteroid_planet() { std::cout << "asteroid burns up in atmosphere" << std::endl; }
void asteroid_spaceship() { std::cout << "asteroid hits and destroys spaceship" << std::endl; }
void asteroid_armed_spaceship() { std::cout << "spaceship shoots asteroid" << std::endl; }

std::map<std::pair<std::type_index, std::type_index>, void(*)(void)> outcomes{
	{{typeid(Spaceship), typeid(Planet)}, spaceship_planet},
	{{typeid(Asteroid),typeid(Planet)}, asteroid_planet},
	{{typeid(Asteroid),typeid(Spaceship)}, asteroid_spaceship},
	{{typeid(Asteroid), typeid(ArmedSpaceship)}, asteroid_armed_spaceship}
};

void collide(GameObject& first, GameObject& second)
{
	auto it = outcomes.find({ first.type(), second.type() });
	if (it == outcomes.end())
	{
		it = outcomes.find({ second.type(), first.type() });
		if (it == outcomes.end())
		{
			std::cout << "objects pass each other harmlessly" << std::endl;
			return;
		}
	}
	it->second();
}

int main(int argc, char* argv[])
{
	ArmedSpaceship spaceship;
	Asteroid asteroid;
	Planet planet;

	collide(planet, spaceship);
	collide(planet, asteroid);
	collide(spaceship, asteroid);
	collide(planet, planet);

	std::cout << "Member collision:" << std::endl;
	planet.collide(asteroid);

	// but this won't work
	spaceship.collide(planet);

	return 0;
}
