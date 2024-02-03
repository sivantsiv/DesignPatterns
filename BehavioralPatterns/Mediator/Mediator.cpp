#include <iostream>
#include <string>
#include <vector>
#include <boost/signals2.hpp>

struct Game;

struct EventData
{
    virtual ~EventData() = default;
    virtual void print() const = 0;
};

struct Player;
struct PlayerScoredData : EventData
{
    std::string player_name;
    unsigned int goals_scored_so_far;

    PlayerScoredData(const std::string& player_name, const unsigned int goals_scored_so_far): player_name(player_name), 
        goals_scored_so_far(goals_scored_so_far)
    {
    }

    void print() const override
    {
        std::cout << player_name << " has scored! (their " << goals_scored_so_far << " goal)" << std::endl;
    }
};

struct Game
{
    boost::signals2::signal<void(EventData*)> events; // Observer
};

struct Player
{
    std::string name;
    unsigned int goals_scored = 0;
    Game& game;


    Player(const std::string& name, Game& game) : name(name), game(game)
    {
    }

    void score()
    {
        goals_scored++;
        PlayerScoredData ps{ name, goals_scored };
        game.events(&ps);
    }
};

struct Coach
{
    Game& game;

    explicit Coach(Game& game) : game(game)
    {
        // Celebrate if player has scored <3 goals
        game.events.connect([](EventData* e)
        {
            PlayerScoredData* ps = dynamic_cast<PlayerScoredData*>(e);
            if (ps && ps->goals_scored_so_far < 3)
            {
               std::cout << "Coach says: well done, " << ps->player_name << std::endl;
            }
        });
    }
};

int main()
{
    Game game;
    Player player{ "Sam", game };
    Coach coach{ game };

    player.score();
    player.score();
    player.score(); // Ignored by coach

    return 0;
}
