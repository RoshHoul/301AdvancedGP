#pragma once
#include <SFML\Graphics.hpp>

class BallCalculations
{
	sf::Vector2f position;
	sf::RectangleShape ballShape;

	float xVelocity = 3.0f;
	float yVelocity = 3.0f;

public:
	BallCalculations(float startX, float startY);
	sf::FloatRect getPosition();
	sf::RectangleShape getShape();
	sf::Vector2f getVecPosition();
	float getXVelocity();
	void reboundSides();
	void rebountBatOrTop();
	void hitBottom();
	void update(sf::Time time);
};

