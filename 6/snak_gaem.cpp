// Credits to Mike Shah  -  https://youtu.be/yZl9X47cHi8
// g++ snak_gaem.cpp -O3 -o snak_gaem `pkg-config --libs --cflags sdl3`

// .
// .
// .

#include "snak_gaem.h"



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


std::tuple<float, float> generate_item(){

    std::tuple<Sint32, Sint32> current_item = item::generate_item();

    // Implicit conversion from Sint32 to float here...
    float item_x = std::get<0>(current_item) + (WINDOW_WIDTH  - PLAYFIELD_WIDTH)  / 2;
    float item_y = std::get<1>(current_item) + (WINDOW_HEIGHT - PLAYFIELD_HEIGHT) / 2;

    std::tuple<Sint32, Sint32> translated_item(item_x, item_y); // Moves the item (More info on translation here: https://en.wikipedia.org/wiki/Translation_(geometry))

	// SDL_Log("\n");
	// SDL_Log("item position:");
	// SDL_Log("(%d, %d)", std::get<0>(current_item), std::get<1>(current_item));
	// SDL_Log("\n");

    return translated_item;
}


std::tuple<Sint32, Sint32> current_item = generate_item();





struct SDL_Application{

    int score    =  0;
    int hiscore  =  0;



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
            SDL_SetRenderLogicalPresentation(mRenderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
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


	void Render(int currentFrame){

		// SDL_SetRenderDrawColor(mRenderer, 0xBB, 0xAA, 0xEE, 0xFF);
        SDL_SetRenderDrawColor(mRenderer, 0x9C, 0x65, 0x7D, 0xFF);
		SDL_RenderClear(mRenderer);

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

        // Renders the playfield (outer part).
		SDL_SetRenderDrawColor(mRenderer, 0xcc, 0x88, 0x88, 0xFF);
		SDL_RenderFillRect(mRenderer, &playfield_but_bigger);
        
        // Renders the playfield (inner part).
		SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(mRenderer, &playfield);




        // SDL_Log("(%d, %d)", std::get<0>(current_item), std::get<1>(current_item));

        // Old implementation - a bit buggy, as it was designed for int and not decimal coords... (you can only pick an item if your coords are an exact match).
        // bool item_got_picked_up = ((std::get<0>(current_item) == (int) std::get<0>(player_pos::current_pos)) && (std::get<1>(current_item) == (int) std::get<1>(player_pos::current_pos)));

        // New implementation - designed for decimal coords (you pick an item if you get close enough to it).
        bool item_got_picked_up = SDL_pow((SDL_pow((std::get<0>(current_item)) - std::get<0>(player_pos::current_pos), 2) + SDL_pow( (std::get<1>(current_item)) - std::get<1>(player_pos::current_pos), 2)), 0.5f) < 1;


        // SDL_Log("distance to item: %f", SDL_pow((SDL_pow(((float) (std::get<0>(current_item))) - std::get<0>(player_pos::current_pos), 2) + SDL_pow(((float) (std::get<1>(current_item))) - std::get<1>(player_pos::current_pos), 2)), 1/2));
        // SDL_Log("bool: %d", item_got_picked_up);

        if ((currentFrame == 0) || (item_got_picked_up)){

            game_speed *= 1.25;

            std::tuple<Sint32, Sint32> new_item = generate_item();

            std::get<0>(current_item) = std::get<0>(new_item);
            std::get<1>(current_item) = std::get<1>(new_item);

            if (item_got_picked_up){
                score++;
            }
        }


        // Renders items.
        SDL_SetRenderDrawColor(mRenderer, 0x00, 0xFF, 0x00, 0xFF);
    	SDL_RenderPoint(mRenderer, std::get<0>(current_item), std::get<1>(current_item));

        
		if (facing == "right") {
			(double) (std::get<0>(player_pos::current_pos) += game_speed);
		} else if (facing == "left") {
			(double) (std::get<0>(player_pos::current_pos) -= game_speed);
		} else if (facing == "up") {
			(double) (std::get<1>(player_pos::current_pos) -= game_speed);
		} else {	// facing == "down".
			(double) (std::get<1>(player_pos::current_pos) += game_speed);
		}

        // Renders the player		
        SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderPoint(mRenderer, std::get<0>(player_pos::current_pos), std::get<1>(player_pos::current_pos));
        
		SDL_RenderPresent(mRenderer);
	}

	// Every tick is one iteration of the game loop.
	void Tick(int currentFrame){
		Input();
		Update();
		Render(currentFrame);
	}

	void MainLoop(){
		Uint64 fps = 0;
		Uint64 lastTime = 0;
        Uint64 currentFrame = 0;

		while(running){
			Uint64 currentTick = SDL_GetTicks();
			Tick(currentFrame);
            currentFrame++;
			fps++;

			Uint64 deltaTime = SDL_GetTicks() - currentTick;
            
            SDL_Delay(16.666666 - deltaTime);       // Homemade VSync...      
            SDL_Delay(4.1666666 - deltaTime);       // Homemade 240fps cap...     

			if (currentTick > lastTime + 1000) {
				lastTime = currentTick;
				std::string title;
				title += "Current FPS: " + std::to_string(fps);
				SDL_SetWindowTitle(mWindow, title.c_str());
				fps = 0;
			}

			// SDL_Delay(16);
			
		}
	}
};


// Entry Point
int main(int argc, char* argv[]){
	SDL_Application app("FPS test! Current FPS: ");
	app.MainLoop();
	return 0;
}

