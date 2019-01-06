#pragma once
#include <SFML\Graphics.hpp>

class Ball
{
	sf::Vector2f position;
	sf::RectangleShape ballShape;

	float xVelocity = 3.0f;
	float yVelocity = 3.0f;

public:
	Ball(float startX, float startY);
	sf::FloatRect getPosition();
	sf::RectangleShape getShape();
	float getXVelocity();
	void reboundSides();
	void rebountBatOrTop();
	void hitBottom();
	void update();
};

