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
	SDL_FreeSurface(sprite);
}

void Player::draw(SDL_Renderer* renderer) const
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, sprite); // Create a texture from the surface
	SDL_Rect rect = {position.x, position.y, 32, 32}; // Create a rectangle
	SDL_RenderCopy(renderer, texture, NULL, &rect); // Draw the texture
	SDL_DestroyTexture(texture); // Destroy the texture (cleanup)
}
