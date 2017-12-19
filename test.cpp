#include <iostream>

class Hero
{
public:
	Hero() {}

	Hero(int health, int damage = 3) 
	{
		std::cout<<"Health "<<health<<"\n";
		std::cout<<"Damage "<<damage<<"\n";
	}
};

class Castle
{
public:
	mutable int damage_;

	int health_;

	Castle() {}
};

void attack(int health, int damage = 3)
{
	std::cout<<"HEALTH "<<health<<"\n";
	std::cout<<"DAMAGE "<<damage<<"\n";
}

int main()
{
	const Castle castle;

	castle.damage_ = 5;

	return 0;
}
