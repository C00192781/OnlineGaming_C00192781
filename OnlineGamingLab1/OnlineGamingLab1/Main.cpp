#include "Net.h"
#include <iostream>


void playerA()
{
	std::cout << "Player A" << std::endl;
}

void playerB()
{
	std::cout << "Player B" << std::endl;
}


int main()
{
	std::string input;

	Net playerOne = Net();
	playerOne.initialise();
	playerOne.setupUDP(28000,"127.0.0.1");
	std::cout << playerOne.portNum << " "<< playerOne.getSenderIP() << std::endl;

	Net playerTwo = Net();
	playerTwo.initialise();
	playerTwo.setupUDP(28001, "127.0.0.1");
	std::cout << playerTwo.portNum << " " << playerTwo.getSenderIP() << std::endl;

	std::cin >> input;
	if (input == "a")
	{
		playerA();
	}
	else if (input == "b")
	{
		playerB();
	}
	
	

	system("PAUSE");
	return 0;
}