#include <iostream>
#include <string>
#include <sstream>
#include <memory>

struct BankAccount
{
	virtual ~BankAccount() = default;
	virtual void deposit(int amount) = 0;
	virtual void withdraw(int amount) = 0;
};

struct CurrentAccount : BankAccount // checking
{
	explicit CurrentAccount(const int balance)
		: balance(balance)
	{
	}

	void deposit(int amount) override
	{
		balance += amount;
	}

	void withdraw(int amount) override
	{
		if (amount <= balance) balance -= amount;
	}

	friend std::ostream& operator<<(std::ostream& os, const CurrentAccount& obj)
	{
		return os << "Account balance: " << obj.balance;
	}

private:
	int balance;
};

// ===========================================

struct Image
{
	virtual ~Image() = default;
	virtual void draw() = 0;
};

struct Bitmap : Image
{
	Bitmap(const std::string& filename)
	{
		std::cout << "Loading image from " << filename << std::endl;
	}

	void draw() override
	{
		std::cout << "Drawing image" << std::endl;
	}
};

struct LazyBitmap : Image
{
	LazyBitmap(const std::string& filename) : filename(filename) {}
	~LazyBitmap() { delete bmp; }
	void draw() override
	{
		if (!bmp)
			bmp = new Bitmap(filename);
		bmp->draw();
	}

private:
	Bitmap* bmp{ nullptr };
	std::string filename;
};

void draw_image(Image& img)
{
	std::cout << "About to draw the image" << std::endl;
	img.draw();
	std::cout << "Done drawing the image" << std::endl;
}

// ===========================================

template <typename T> struct Property
{
	T value;
	Property(const T initialValue)
	{
		*this = initialValue;
	}
	operator T()
	{
		return value;
	}
	T operator =(T newValue)
	{
		return value = newValue;
	}
};

struct Creature
{
	Property<int> strength{ 10 };
	Property<int> agility{ 5 };
};

int main()
{
	// Smart pointers
	{
		BankAccount* a = new CurrentAccount(123);
		a->deposit(321);
		delete a;

		// << will not work if you make this a shared_ptr<BankAccount>
		auto b = std::make_shared<CurrentAccount>(123);

		BankAccount* actual = b.get(); // pointer's own operations on a .
		b->deposit(321); // underlying object's operations are on ->
		// note this expression is identical to what's above
		std::cout << *b << std::endl;
		// no delete
	}

	// Property proxy
	{
		Creature creature;
		creature.agility = 20;
		std::cout << std::endl << creature.agility << std::endl << std::endl;
	}

	// Virtual proxy
	{
		LazyBitmap img{ "pokemon.png" };
		draw_image(img); // loaded whether the bitmap is loaded or not
		draw_image(img);
	}

	return 0;
}
