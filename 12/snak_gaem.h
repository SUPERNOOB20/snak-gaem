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

double game_speed                  = 0.015;      // fine-tuned initial player speed value after a lot of testplaying... (MY own testplaying ":3)
double chosen_game_speed_increment = 1.1;       //  fine-tuned initial player speed value after a lot of testplaying... (MY own testplaying ":3)



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


SDL_FRect playfield{
    .x = (WINDOW_WIDTH - PLAYFIELD_WIDTH) / 2,
    .y = (WINDOW_HEIGHT - PLAYFIELD_HEIGHT) / 2,
    .w = PLAYFIELD_WIDTH,
    .h = PLAYFIELD_HEIGHT
};

float scale_factor = 0.9f;

SDL_FRect playfield_but_bigger{
    .x = (WINDOW_WIDTH - PLAYFIELD_WIDTH - scale_factor) / 2,
    .y = (WINDOW_HEIGHT - PLAYFIELD_HEIGHT - scale_factor) / 2,
    .w = PLAYFIELD_WIDTH * 1.0f + scale_factor,
    .h = PLAYFIELD_HEIGHT * 1.0f + scale_factor
};


namespace main_menu
{
    SDL_FRect easyTextRect{
        .x = WINDOW_WIDTH   / 2.75f,                    //  Eyeballed center of the screen...
        .y = WINDOW_HEIGHT  / 6.2f,
        .w = WINDOW_WIDTH   / 3.2f,
        .h = WINDOW_HEIGHT  / 8.0f
    };

    SDL_FRect mediumTextRect{
        .x = WINDOW_WIDTH   / 3.0f,                     //  Eyeballed center of the screen...
        .y = WINDOW_HEIGHT  / 2.2f,
        .w = WINDOW_WIDTH   / 3.2f,
        .h = WINDOW_HEIGHT  / 8.0f
    };

    SDL_FRect hardTextRect{
        .x = WINDOW_WIDTH   / 2.75f,                    //  Eyeballed center of the screen...
        .y = WINDOW_HEIGHT  / 1.32f,
        .w = WINDOW_WIDTH   / 3.2f,
        .h = WINDOW_HEIGHT  / 8.0f
    };




    SDL_FRect easyButtonRect{
        .x = WINDOW_WIDTH   / 3.5f,                     //  Eyeballed center of the screen...
        .y = WINDOW_HEIGHT  * (0.9f / 6.0f),
        .w = WINDOW_WIDTH   / 2.2f,
        .h = WINDOW_HEIGHT  * (1.0f / 6.0f)
    };

    SDL_FRect mediumButtonRect{
        .x = WINDOW_WIDTH   / 3.5f,        //  Eyeballed center of the screen...
        .y = WINDOW_HEIGHT  * (2.65f / 6.0f),
        .w = WINDOW_WIDTH   / 2.2f,
        .h = WINDOW_HEIGHT  * (1.0f / 6.0f)
    };

    SDL_FRect hardButtonRect{
        .x = WINDOW_WIDTH   / 3.5f,        //  Eyeballed center of the screen...
        .y = WINDOW_HEIGHT  * (4.4f / 6.0f),
        .w = WINDOW_WIDTH   / 2.2f,
        .h = WINDOW_HEIGHT  * (1.0f / 6.0f)
    };
}


// SCORE COUNTER
SDL_FRect scoreTextRect{
    .x = WINDOW_WIDTH  / 2.0f,
    .y =                 0.0f,
    .w = WINDOW_WIDTH  / 2.2f,
    .h = WINDOW_HEIGHT / 5.0f,
};

// GAME OVER SIGN.
SDL_FRect gameOverTextRect{
    .x = WINDOW_WIDTH   / 3.2f,        //  Eyeballed center of the screen...
    .y = WINDOW_HEIGHT  / 2.2f,       //   Eyeballed center of the screen...
    .w = WINDOW_WIDTH   / 2.2f,
    .h = WINDOW_HEIGHT  / 5.0f
};

// You have to be moving SOMEWHERE when you first start the game, no?
int pre_input = 82;
std::string facing = "right";




// Mouse handling (choosing the difficulty)

float mouse_pos_x = 0;
float mouse_pos_y = 0;

bool player_has_clicked = false;

/*
bool is_player_hovering_over_obj(SDL_FRect* &rect){
    return ((mouse_pos_x >= rect->x && mouse_pos_x <= rect->x + rect->w) && (mouse_pos_y >= rect->y && mouse_pos_y <= rect->y + rect->h));           // (player is within horizontal bounds) && (player is within vertical bounds)
}
*/

bool is_player_hovering_over_obj(SDL_FRect rect){
    return ((mouse_pos_x >= rect.x && mouse_pos_x <= rect.x + rect.w) && (mouse_pos_y >= rect.y && mouse_pos_y <= rect.y + rect.h));           // (player is within horizontal bounds) && (player is within vertical bounds)
}

bool player_is_hovering_easy    =  is_player_hovering_over_obj(main_menu::easyButtonRect);
bool player_is_hovering_medium  =  is_player_hovering_over_obj(main_menu::mediumButtonRect);
bool player_is_hovering_hard    =  is_player_hovering_over_obj(main_menu::hardButtonRect);


bool easy_is_clicked   = player_has_clicked && player_is_hovering_easy;
bool medium_is_clicked = player_has_clicked && player_is_hovering_medium;
bool hard_is_clicked   = player_has_clicked && player_is_hovering_hard;



#endif
