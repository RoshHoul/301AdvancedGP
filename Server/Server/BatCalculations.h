#pragma once
#include <SFML\Network.hpp>
class BatCalculations
{
	sf::Vector2f position;
	float batSpeed = 3.0f;
public:
	BatCalculations(float x, float y);
	void moveLeft();
	void moveRight();
	~BatCalculations();
};

