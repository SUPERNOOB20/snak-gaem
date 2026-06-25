// Credits to Mike Shah  -  https://youtu.be/yZl9X47cHi8
// g++ snak_gaem.cpp -O3 -o snak_gaem `pkg-config --libs --cflags sdl3`

// .
// .
// .

#include <SDL3/SDL.h>
#include <string>
#include <cassert>
#include <tuple>

#include "snak_gaem.h"

#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 180

#define GAME_SPEED 1

// Swaps inputs 79 and 82 to ensure correct parity checks.
int remap(int input){

	switch (input) {
		case 82:
			input = 79;
			break;
			
		case 79:
			input = 82;
			break;
			
		default:
			int nop_nop = 0;
	} 

	// Returns the remapped input.
	return input;
}




struct SDL_Application{

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    
    bool running = true;

    // Constructor
    SDL_Application(const char* title){
	    SDL_Init(SDL_INIT_VIDEO);
	    mWindow = SDL_CreateWindow(title, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);    
	    mRenderer = SDL_CreateRenderer(mWindow, nullptr);
	    if (mRenderer == nullptr){
		    assert (0 && "ERROR: Hardware acceleration not supported :c");
	    } else {
		    SDL_Log("Current renderer: %s", SDL_GetRendererName(mRenderer));
		    SDL_Log("Available renderer drivers:");
		    for (int i = 0; i > SDL_GetNumRenderDrivers(); i++) {
			    SDL_Log("%d, %s", i + 1, SDL_GetRenderDriver(i));
		    }
            SDL_SetRenderLogicalPresentation(mRenderer, 320, 180, SDL_LOGICAL_PRESENTATION_LETTERBOX);
	    }
    }
	    
	// Destructor
	~SDL_Application(){
		SDL_Quit();
	}
	
	void Input(){

		int nop_casero = 0;			// Homemade nop instruction.

		SDL_Event event;

		while (SDL_PollEvent(&event)){
			if (event.type == SDL_EVENT_QUIT){
				running = false;
			} else if (event.type == SDL_EVENT_KEY_DOWN) {

				auto input = event.button.button;

				input = remap(input);

				// Consider replacing this with an enum in the future, for tidier code...
				if ((pre_input % 2) != (input % 2)){
					pre_input = input;		// Moving backwards will not be allowed!
				}

				// std::string pre_input_str = static_cast<std::string>(pre_input);
				// SDL_Log(pre_input_str.c_str());


				SDL_Log("\n");
				SDL_Log("input:");
				SDL_Log("%d", input);
				SDL_Log("\n");


            	switch (pre_input) {
					
	            	case 79:
	            		// facing = "right";
	            		facing = "up";
	            		break;
	            		
            		case 80:
            			facing = "left";
            			break;
            			
					case 81:
						facing = "down";
						break;
						
            		case 82:
						// facing = "up";
						facing = "right";
						break;

					default:
		                // SDL_Log("Congratulations! You just pressed the %d key!!! :3", event.button.button);
						nop_casero = 0;		// Homemade nop instruction.
				}
            }
		}
	}

   
	void Update(){
	}


	void Render(){

		SDL_SetRenderDrawColor(mRenderer, 0xBB, 0xAA, 0xEE, 0xFF);
		SDL_RenderClear(mRenderer);

        SDL_FRect rect{
            .x = 50,
            .y = 50,
            .w = 160,
            .h = 90
        };

		SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(mRenderer, &rect);

		SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);



		// SDL_Log(facing.c_str());

		if (facing == "right") {
			std::get<0>(pos::current_pos) += GAME_SPEED;
		} else if (facing == "left") {
			std::get<0>(pos::current_pos) -= GAME_SPEED;
		} else if (facing == "up") {
			std::get<1>(pos::current_pos) -= GAME_SPEED;
		} else {	// facing == "down".
			std::get<1>(pos::current_pos) += GAME_SPEED;
		}

		
		SDL_RenderPoint(mRenderer, std::get<0>(pos::current_pos), std::get<1>(pos::current_pos));
        SDL_RenderRect(mRenderer, &rect);
        
		SDL_RenderPresent(mRenderer);
	}

	// Every tick is one iteration of the game loop.
	void Tick(){
		Input();
		Update();
		Render();
	}

	void MainLoop(){
		Uint64 fps = 0;
		Uint64 lastTime = 0;
		while(running){
			Uint64 currentTick = SDL_GetTicks();
			Tick();
			fps++;

			Uint64 deltaTime = SDL_GetTicks() - currentTick;
			if (currentTick > lastTime + 1000) {
				lastTime = currentTick;
				std::string title;
				title += "Current FPS: " + std::to_string(fps);
				SDL_SetWindowTitle(mWindow, title.c_str());
				fps = 0;
			}

			SDL_Delay(16);
			
		}
	}
};


// Entry Point
int main(int argc, char* argv[]){
	SDL_Application app("FPS test! Current FPS: ");
	app.MainLoop();
	return 0;
}

