
// .
// .
// .

#include <SDL3/SDL.h>
#include <iostream>
#include <string>
#include <cassert>
#include <tuple>

#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 180

std::string facing = "right";

namespace pos
{

	/*
	// Tuple declaration.
	std::tuple<int, int> current_pos;

	// Assigning values to the tuple using make_tuple()
	current_pos = std::make_tuple(100, 100);		// Current position of the player.
	*/

	std::tuple<int, int> current_pos(100, 100);
}

int main(){

    printf("%d", std::get<0>(pos::current_pos));

    return 0;
}
