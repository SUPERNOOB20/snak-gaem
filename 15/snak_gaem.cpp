// Credits to Mike Shah  -  https://youtu.be/yZl9X47cHi8
// g++ snak_gaem.cpp -O3 -o snak_gaem `pkg-config --libs --cflags sdl3 sdl3-ttf`

// .
// .
// .

#include "snak_gaem.h"

bool starting_speed = 0;

bool game_is_paused = false;
bool game_over      = false;

bool scene          = 1;

Uint64 score    =  0;
Uint64 hiscore  =  0;






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

void pick_up_items(int currentFrame, bool item_got_picked_up){
    if ((currentFrame == 0) || (item_got_picked_up)){

        game_speed *= chosen_game_speed_increment;

        std::tuple<Sint32, Sint32> new_item = generate_item();

        std::get<0>(current_item) = std::get<0>(new_item);
        std::get<1>(current_item) = std::get<1>(new_item);

        if (item_got_picked_up){
            score++;
        }
    }
}



struct SDL_Application{

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;

    SDL_Texture* easy_button_texture     =  nullptr;
    SDL_Texture* medium_button_texture   =  nullptr;
    SDL_Texture* hard_button_texture     =  nullptr;

    
    TTF_Font* mFont                      =  nullptr;
    TTF_Font* elegantFont                =  nullptr;
    SDL_Texture* score_text_texture      =  nullptr;
    SDL_Texture* game_over_text_texture  =  nullptr;

    SDL_Texture* easy_text_texture       =  nullptr;
    SDL_Texture* medium_text_texture     =  nullptr;
    SDL_Texture* hard_text_texture       =  nullptr;
    
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

        // Initialize SDL_TTF.

        if(!TTF_Init()){
            assert(0 && "ERROR: SDL_TTF could not initialize :c");
        }

        mFont = TTF_OpenFont("./font/Minecraft.ttf", 12.0);

        if(mFont == nullptr){
            assert(0 && "ERROR: Font file \"Minecraft.ttf\" not found :c");
        }



        elegantFont = TTF_OpenFont("./font/Peach Club Script TTF.ttf", 36.0);

        if(mFont == nullptr){
            assert(0 && "ERROR: Font file \"Minecraft.ttf\" not found :c");
        }



        std::string text_shown = "GAME OVER";
        SDL_Surface* textSurface2 = TTF_RenderText_Solid(mFont, text_shown.c_str(), 0, SDL_Color{255, 0, 0, 255});

        game_over_text_texture = SDL_CreateTextureFromSurface(mRenderer, textSurface2);
        SDL_SetTextureScaleMode(game_over_text_texture, SDL_SCALEMODE_NEAREST);        // Also called "nearest neighbour". Suitable for pixel-art textures, like the pixel-art font we are using (no interpolation or antialiasing).

        SDL_DestroySurface(textSurface2);


        text_shown = "Easy";
        SDL_Surface* easy_text_surface = TTF_RenderText_Solid(elegantFont, text_shown.c_str(), 0, SDL_Color{0, 90, 20, 255});

        text_shown = "Medium";
        SDL_Surface* medium_text_surface = TTF_RenderText_Solid(elegantFont, text_shown.c_str(), 0, SDL_Color{90, 90, 0, 255});

        text_shown = "Hard";
        SDL_Surface* hard_text_surface = TTF_RenderText_Solid(elegantFont, text_shown.c_str(), 0, SDL_Color{90, 0, 0, 255});

            
        easy_text_texture = SDL_CreateTextureFromSurface(mRenderer, easy_text_surface);
        SDL_SetTextureScaleMode(easy_text_texture, SDL_SCALEMODE_LINEAR);        // Also called "nearest neighbour". Suitable for pixel-art textures, like the pixel-art font we are using (no interpolation or antialiasing).
        SDL_DestroySurface(easy_text_surface);

        medium_text_texture = SDL_CreateTextureFromSurface(mRenderer, medium_text_surface);
        SDL_SetTextureScaleMode(medium_text_texture, SDL_SCALEMODE_LINEAR);        // Also called "nearest neighbour". Suitable for pixel-art textures, like the pixel-art font we are using (no interpolation or antialiasing).
        SDL_DestroySurface(medium_text_surface);

        hard_text_texture = SDL_CreateTextureFromSurface(mRenderer, hard_text_surface);
        SDL_SetTextureScaleMode(hard_text_texture, SDL_SCALEMODE_LINEAR);        // Also called "nearest neighbour". Suitable for pixel-art textures, like the pixel-art font we are using (no interpolation or antialiasing).
        SDL_DestroySurface(hard_text_surface);



        SDL_Surface* easy_button_surface    =  SDL_LoadPNG("./easy_button.png");
        SDL_Surface* medium_button_surface  =  SDL_LoadPNG("./medium_button.png");
        SDL_Surface* hard_button_surface    =  SDL_LoadPNG("./hard_button.png");

        easy_button_texture    =  SDL_CreateTextureFromSurface(mRenderer, easy_button_surface);
        medium_button_texture  =  SDL_CreateTextureFromSurface(mRenderer, medium_button_surface);
        hard_button_texture    =  SDL_CreateTextureFromSurface(mRenderer, hard_button_surface);

        SDL_DestroySurface(easy_button_surface);
        SDL_DestroySurface(medium_button_surface);
        SDL_DestroySurface(hard_button_surface);


    }
	    
	// Destructor
	~SDL_Application(){
        TTF_Quit();
		SDL_Quit();
	}
	
	void Input(){

		int nop_casero = 0;			// Homemade nop instruction.

		SDL_Event event;

		while (SDL_PollEvent(&event)){
			if (event.type == SDL_EVENT_QUIT){
				running = false;
            } else if (event.type == SDL_EVENT_MOUSE_MOTION) {

                mouse_pos_x = event.motion.x;
                mouse_pos_y = event.motion.y;

			} else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
    
                player_has_clicked = true;
                
            } else if (event.type == SDL_EVENT_KEY_DOWN) {


                // SDL_KeyboardEvent input = event.key.key;
				// auto input = event.button.button;
                auto input = event.button.button;

                // Quit the game when pressing escape.
                // 41 is the escape key       (you can remap it if you want :3)
                if (input == 41){  
                    if (scene == 1){        // Playfield ---> Main menu
                        scene -= 1;
                    } else {               //  Main menu ---> Exit the game
                        SDL_Quit();
                    }    
                }
                
                if (scene == 1) {
                    // Pause with the "Q" key.
                    if (input == 20 && !game_over){
                        game_is_paused = !game_is_paused;
                    }
                    

                    // Use the "R" key to restart after a game over.
                    if (input == 21 && game_over){
                        score = 0;

                        game_is_paused = false;
                        game_over = false;
                        input = 79;             // When you respawn, facing = "right";
                    }

                    // Useful development/debug tool to check keycodes.
                    // SDL_Log("You pressed the %d key.", input);

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
		                    SDL_Log("Congratulations! You just pressed the %d key!!! :3", event.button.button);
						    nop_casero = 0;		// Homemade nop instruction.
				    }
                }
            }
		}
	}

   
	void Update(){
        
        int current_window_width;
        int current_window_height;
        SDL_GetRenderOutputSize(mRenderer, &current_window_width, &current_window_height);
        // SDL_Log("window_size: (%d, %d)", current_window_width, current_window_height);
        
        // Main menu. Ugh, I really should learn enums
        // so I can use them for scene IDs... e.e
        if (scene == 0) {
            if (hard_is_clicked(mouse_pos_x, mouse_pos_y, current_window_width, current_window_height)){
                   
                    game_speed                  = 0.05;          //  Hard
                    starting_speed              = 0.05;         //   Hard
                    chosen_game_speed_increment = 1.5;         //   Hard
                    scene += 1;

            } else if (medium_is_clicked(mouse_pos_x, mouse_pos_y, current_window_width, current_window_height)) {
                    
                    game_speed                  = 0.03;         //  Medium
                    starting_speed              = 0.03;        //  Medium
                    chosen_game_speed_increment = 1.25;       //   Medium
                    scene += 1;

            } else if (easy_is_clicked(mouse_pos_x, mouse_pos_y, current_window_width, current_window_height)) {

                    game_speed                  = 0.015;       //  Easy
                    starting_speed              = 0.015;       //  Easy
                    chosen_game_speed_increment = 1.1;        //   Easy
                    scene += 1;
            }
        }

        player_has_clicked = false;
	}


	void Render(int currentFrame){

        // SDL_SetRenderDrawColor(mRenderer, 0xBB, 0xAA, 0xEE, 0xFF);
        SDL_SetRenderDrawColor(mRenderer, 0x9C, 0x65, 0x7D, 0xFF);
        SDL_RenderClear(mRenderer);
        
        switch(scene) {

            case 0:
        		SDL_RenderTexture(mRenderer, easy_button_texture,   nullptr, &main_menu::easyButtonRect);
        		SDL_RenderTexture(mRenderer, medium_button_texture, nullptr, &main_menu::mediumButtonRect);
        		SDL_RenderTexture(mRenderer, hard_button_texture,   nullptr, &main_menu::hardButtonRect);

                SDL_RenderTexture(mRenderer, easy_text_texture,     nullptr, &main_menu::easyTextRect);
                SDL_RenderTexture(mRenderer, medium_text_texture,   nullptr, &main_menu::mediumTextRect);
                SDL_RenderTexture(mRenderer, hard_text_texture,     nullptr, &main_menu::hardTextRect);
                break;

            case 1:

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


                pick_up_items(currentFrame, item_got_picked_up);


                // Renders items.
                SDL_SetRenderDrawColor(mRenderer, 0x00, 0xFF, 0x00, 0xFF);
            	SDL_RenderPoint(mRenderer, std::get<0>(current_item), std::get<1>(current_item));

                if (!game_is_paused){
		            if (facing == "right") {
			            (double) (std::get<0>(player_pos::current_pos) += game_speed);
		            } else if (facing == "left") {
			            (double) (std::get<0>(player_pos::current_pos) -= game_speed);
		            } else if (facing == "up") {
			            (double) (std::get<1>(player_pos::current_pos) -= game_speed);
		            } else {	// facing == "down".
			            (double) (std::get<1>(player_pos::current_pos) += game_speed);
		            }
                }

                // If the player crashes onto the borders of the playfield, it's
                // game over.
                if (((std::get<0>(player_pos::current_pos) <  playfield.x) || (std::get<1>(player_pos::current_pos) <  playfield.y))
                    || ((std::get<0>(player_pos::current_pos) >  (playfield.x + playfield.w)) || (std::get<1>(player_pos::current_pos) >  (playfield.y + playfield.h)))){
                    if (score > hiscore){
                        hiscore = score;
                    }
                    game_is_paused = true;
                    game_over = true;

                    std::get<0>(player_pos::current_pos) = WINDOW_WIDTH  / 3.5;
                    std::get<1>(player_pos::current_pos) = WINDOW_HEIGHT / 2.0;
                    game_speed = 0.015;
                }

                // Renders the player		
                SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		        SDL_RenderPoint(mRenderer, std::get<0>(player_pos::current_pos), std::get<1>(player_pos::current_pos));
                
                // Render the text showing the current score.
                std::string text_shown = "score: " + std::to_string(score);
                SDL_Surface* textSurface = TTF_RenderText_Solid(mFont, text_shown.c_str(), 0, SDL_Color{255, 255, 0, 255});

                score_text_texture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
                SDL_SetTextureScaleMode(score_text_texture, SDL_SCALEMODE_NEAREST);        // Also called "nearest neighbour". Suitable for pixel-art textures, like the pixel-art font we are using (no interpolation or antialiasing).

                SDL_DestroySurface(textSurface);

                SDL_RenderTexture(mRenderer, score_text_texture, nullptr, &scoreTextRect);

                if (game_over){
                    // Render the game over text showing the current score.
                    SDL_RenderTexture(mRenderer, game_over_text_texture, nullptr, &gameOverTextRect);
                }
            
        }

        // Puts everything on the screen.
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
            
            // SDL_Delay(16.666666 - deltaTime);       // Homemade VSync...      
            
            if ((4.1666666 - deltaTime) > 0){
                SDL_Delay(4.1666666 - deltaTime);       // Homemade 240fps cap...     
            }

			if (currentTick > lastTime + 1000) {
				lastTime = currentTick;
				std::string title;
				title += "Current FPS: " + std::to_string(fps);
				SDL_SetWindowTitle(mWindow, title.c_str());
				fps = 0;
			}
		}
	}
};


// Entry Point
int main(int argc, char* argv[]){
	SDL_Application app("FPS test! Current FPS: ");
	app.MainLoop();
	return 0;
}

