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
	SDL_DestroyTexture(sprite);
}

/**
 * Moves the player, called in the game loop.
 * @param dir The direction to move the player. 0 = up, 1 = down, 2 = left, 3 = right
 * @param deltaTime The time since the last frame
 */
void Player::move(int dir, double deltaTime)
{
	switch (dir)
	{
		case 0:
			position.y -= moveSpeed * deltaTime;
			if (position.y < 0) { position.y = 0; }
			break;
		case 1:
			position.y += moveSpeed * deltaTime;
			if (position.y > 720) { position.y = 720; }
			break;
		case 2:
			position.x -= moveSpeed * deltaTime;
			if (position.x < 0) { position.x = 0; }
			break;
		case 3:
			position.x += moveSpeed * deltaTime;
			if (position.x > 1280) { position.x = 1280; }
			break;
		default:
			break;
	}
}

bool Player::loadSprite(SDL_Renderer* renderer)
{
	// Load Sprite randomly from 1-4
	int num = rand() % 4 + 1;
	char* filePath = new char[100];

	switch (num)
	{
		case 1:
			strcpy(filePath, "Assets/bulbasaur.png");
			break;
		case 2:
			strcpy(filePath, "Assets/charmander.png");
			break;
		case 3:
			strcpy(filePath, "Assets/squirtle.png");
			break;
		case 4:
			strcpy(filePath, "Assets/pikachu.png");
			break;
		default:
			strcpy(filePath, "Assets/bulbasaur.png");
			break;
	}

	SDL_Surface* surface_sprite = IMG_Load(filePath); // Load the image into a surface
	if (surface_sprite == NULL)
	{
		std::cout << "Failed to load surface sprite! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}

	sprite = SDL_CreateTextureFromSurface(renderer, surface_sprite); // Create a texture from the surface
	if (sprite == NULL)
	{
		std::cout << "Failed to load texture sprite! SDL Error: " << SDL_GetError() << std::endl;
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
	SDL_Rect destRect = { position.x, position.y, 32, 32 };
	SDL_RenderCopy(renderer, sprite, NULL, &destRect);
}
