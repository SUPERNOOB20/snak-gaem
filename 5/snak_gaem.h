#ifndef SNAK_GAEM_H
#define SNAK_GAEM_H

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


// You have to be moving SOMEWHERE when you first start the game, no?
int pre_input = 82;
std::string facing = "right";



#endif
