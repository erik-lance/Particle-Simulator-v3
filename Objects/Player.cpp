#include "Player.h"

Player::Player(Position pos)
{
	position = pos;

	// Initialize SDL Image
	if (IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) // Initialize SDL_image
	{
		std::cout << "SDL_image initialized!" << std::endl;
	}
	else
	{
		std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
	}
}

Player::~Player()
{
}

bool Player::loadSprite(SDL_Renderer* renderer)
{
	// Load Sprite randomly from 1-4
	int num = rand() % 4 + 1;
	char* filePath = new char[100];

	switch (num)
	{
		case 1:
			strcpy(filePath, "../Assets/bulbasaur.png");
			break;
		case 2:
			strcpy(filePath, "../Assets/charmander.png");
			break;
		case 3:
			strcpy(filePath, "../Assets/squirtle.png");
			break;
		case 4:
			strcpy(filePath, "../Assets/pikachu.png");
			break;
		default:
			strcpy(filePath, "../Assets/bulbasaur.png");
			break;
	}

	SDL_Surface* surface_sprite = IMG_Load(filePath); // Load the image into a surface
	sprite = SDL_CreateTextureFromSurface(renderer, surface_sprite); // Create a texture from the surface
	if (sprite == NULL)
	{
		std::cout << "Failed to load sprite! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}
	else
	{
		std::cout << "Sprite loaded successfully!" << std::endl;
		delete[] filePath;
		SDL_FreeSurface(surface_sprite);
		return true;
	}
}

bool Player::loadSpecifiedSprite(SDL_Renderer* renderer, const char* path)
{
	SDL_Surface* surface_sprite = IMG_Load(path); // Load the image into a surface
	sprite = SDL_CreateTextureFromSurface(renderer, surface_sprite); // Create a texture from the surface
	if (sprite == NULL)
	{
		std::cout << "Failed to load sprite! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}
	else
	{
		std::cout << "Sprite loaded successfully!" << std::endl;
		SDL_FreeSurface(surface_sprite);
		return true;
	}
}

void Player::draw(SDL_Renderer* renderer) const
{
	SDL_Rect destRect = { position.x, position.y, 50, 50 };
	SDL_RenderCopy(renderer, sprite, NULL, &destRect);
}
