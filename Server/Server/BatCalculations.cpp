#include "BatCalculations.h"



BatCalculations::BatCalculations(float x, float y)
{
	position.x = x;
	position.y = y;
}

void BatCalculations::moveLeft()
{
	position.x -= batSpeed;
}


void BatCalculations::moveRight()
{
	position.x += batSpeed;
}

BatCalculations::~BatCalculations()
{
}
