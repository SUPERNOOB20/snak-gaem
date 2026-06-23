// Credits to Mike Shah  -  https://youtu.be/yZl9X47cHi8
// g++ letterbox_demo.cpp -O3 -o letterbox_demo_testprogram `pkg-config --libs --cflags sdl3`

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
		SDL_Event event;

		while (SDL_PollEvent(&event)){
			if (event.type == SDL_EVENT_QUIT){
				running = false;
			} else if (event.type == SDL_EVENT_KEY_DOWN) {
                // SDL_Log("Congratulations! You just pressed the %d key!!! :3", event.button.button);
            	if (event.button.button == 79) {
            		facing = "right";
            	} else if (event.button.button == 80) {
            		facing = "left";
            	} else if (event.button.button == 81) {
					facing = "down";
            	} else if (event.button.button == 82) {
					facing = "up";
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

