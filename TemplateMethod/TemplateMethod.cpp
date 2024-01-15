#include <iostream>
#include <string>


class Game
{
public:
	explicit Game(int number_of_players)
		: number_of_players(number_of_players)
	{
	}

	void run()
	{
		start();
		while (!have_winner())
			take_turn();
		std::cout << "Player " << get_winner() << " wins." << std::endl;
	}

protected:
	virtual void start() = 0;
	virtual bool have_winner() = 0;
	virtual void take_turn() = 0;
	virtual int get_winner() = 0;

	unsigned int current_player{ 0 };
	unsigned int number_of_players;
};

class Chess : public Game
{
public:
	explicit Chess() : Game{ 2 } {}

protected:
	void start() override
	{
		std::cout << "Starting a game of chess with " << number_of_players << " players" << std::endl;
	}

	bool have_winner() override
	{
		return turns == max_turns;
	}

	void take_turn() override
	{
		std::cout << "Turn " << turns << " taken by player " << current_player << std::endl;
		turns++;
		current_player = (current_player + 1) % number_of_players;
	}

	int get_winner() override
	{
		return current_player;
	}

private:
	unsigned int turns{ 0 }, max_turns{ 10 };
};

int main()
{
	Chess chess;
	chess.run();

	return 0;
}
