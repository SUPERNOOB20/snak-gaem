#ifndef SNAK_GAEM_H
#define SNAK_GAEM_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

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

#define WINDOW_WIDTH 64
#define WINDOW_HEIGHT 36

#define PLAYFIELD_WIDTH 48
#define PLAYFIELD_HEIGHT 24

double game_speed = 0.115;      // fine-tuned initial player speed value after a lot of testplaying... (MY own testplaying ":3)




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

    // Start near the center of the screen idk
	std::tuple<double, double> current_pos(WINDOW_WIDTH / 3.5, WINDOW_HEIGHT / 2.0);
}


// You have to be moving SOMEWHERE when you first start the game, no?
int pre_input = 82;
std::string facing = "right";



#endif
