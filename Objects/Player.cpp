#include "Player.h"

Player::Player(Position pos)
{
	position = pos;

	// Load Sprite randomly from 1-4
	int num = rand() % 4 + 1;
	switch (num)
	{
		case 1:
			sprite = IMG_Load("Assets/bulbasaur.png");
			break;
		case 2:
			sprite = IMG_Load("Assets/charmander.png");
			break;
		case 3:
			sprite = IMG_Load("Assets/squirtle.png");
			break;
		case 4:
			sprite = IMG_Load("Assets/pikachu.png");
			break;
	}
}

Player::~Player()
{
}

void Player::draw(SDL_Renderer* renderer) const
{
}
