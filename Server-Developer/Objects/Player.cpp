#include "Player.h"

Player::Player(std::string ID, Position pos)
{
	UUID = ID;
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
 * @param deltaTime The time since the last frame
 */
void Player::move(double deltaTime)
{
	if (currentDirection.x == 0 && currentDirection.y == 0) return;

	std::cout << "Moving player " << UUID << " in direction (" << currentDirection.x << ", " << currentDirection.y << ")" << std::endl;
	double newX = position.x + (currentDirection.x * moveSpeed * deltaTime);
	double newY = position.y + (currentDirection.y * moveSpeed * deltaTime);

	// Clamp
	if (newX < (spriteWidth/2)) newX = spriteWidth/2;
	if (newX > (1280 - spriteWidth/2)) newX = 1280 - spriteWidth/2;
	if (newY < (spriteHeight/2)) newY = spriteHeight/2;
	if (newY > (720 - spriteHeight/2)) newY = 720 - spriteHeight/2;

	position = { newX, newY };
}

void Player::loadSpriteFromNumber(SDL_Renderer* renderer, int num)
{
	char* filePath = new char[100];

	switch (num)
	{
		case 0:
			strcpy(filePath, "Assets/bulbasaur.png");
			break;
		case 1:
			strcpy(filePath, "Assets/charmander.png");
			break;
		case 2:
			strcpy(filePath, "Assets/squirtle.png");
			break;
		case 3:
			strcpy(filePath, "Assets/pikachu.png");
			break;
		default:
			strcpy(filePath, "Assets/bulbasaur.png");
			break;
	}

	SDL_Surface* surface_sprite = IMG_Load(filePath); // Load the image into a surface
	sprite = SDL_CreateTextureFromSurface(renderer, surface_sprite); // Create a texture from the surface
	if (sprite == NULL)
	{
		std::cout << "Failed to load sprite! SDL Error: " << SDL_GetError() << std::endl;
	}
	else
	{
		// Set sprite dimensions
		spriteWidth = surface_sprite->w;
		spriteHeight = surface_sprite->h;

		std::cout << "Player " << UUID << " sprite loaded successfully!" << std::endl;
		delete[] filePath;
		SDL_FreeSurface(surface_sprite);
	}
}

/**
 * Draws the player based on position and sprite
 */
void Player::draw(SDL_Renderer* renderer) const
{
	const int width = 16;
	const int height = 16;
	// Create a rectangle to draw the sprite
	int x = position.x - width / 2;
	int y = position.y - height / 2;
	SDL_Rect dest = { x, y, width, height };
	SDL_RenderCopy(renderer, sprite, NULL, &dest);
}
