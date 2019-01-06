#pragma once
#include <SFML\Network.hpp>

class BallCalculations
{
	sf::Vector2f position;
	float xVelocity = 3.0f;
	float yVelocity = 3.0f;

public:
	BallCalculations();
	~BallCalculations();
};

