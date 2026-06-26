#ifndef SNAK_GAEM_H
#define SNAK_GAEM_H

#include <SDL3/SDL.h>
#include <string>
#include <cassert>
#include <tuple>



// When you are coding in C/C++,
// do you use #define or do you use const...?
// 
// Personally, I don't mind either way.
// Let's use #define,
// to be more explicit about our
// preprocessor usage! ^_^

#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 180

#define PLAYFIELD_WIDTH 240
#define PLAYFIELD_HEIGHT 120


#define GAME_SPEED 1


namespace item
{
    std::tuple<Sint32, Sint32> generate_item(){

        Sint32 item_x = SDL_rand(PLAYFIELD_WIDTH  - 2);
        Sint32 item_y = SDL_rand(PLAYFIELD_HEIGHT - 2);
        
	    std::tuple<Sint32, Sint32> item_pos(item_x, item_y);
            return item_pos;
    }
}


namespace player_pos
{

	/*
	// Tuple declaration.
	std::tuple<int, int> current_pos;

	// Assigning values to the tuple using make_tuple()
	current_pos = std::make_tuple(100, 100);		// Current position of the player.
	*/

	std::tuple<int, int> current_pos(100, 100);
}


// You have to be moving SOMEWHERE when you first start the game, no?
int pre_input = 82;
std::string facing = "right";



#endif
