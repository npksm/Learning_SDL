#include <SDL2/SDL.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 1024;

//Start SDL and create window
bool init();

//Load media
bool loadMedia();

//Free media and shut down SDL
void close();

//Avoid these global variables
//Render window
SDL_Window* gWindow = NULL;

//Surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Image to load and show
SDL_Surface* gDice = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init(SDL_INIT_VIDEO)<0){
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success=false;
	}else{
		//create window
		gWindow = SDL_CreateWindow("SDL Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL){
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success=false;
		}else{
			//get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}
	return success;
}

bool loadMedia(){
	bool success = true;

	gDice = SDL_LoadBMP ("20die.bmp");
	if(gDice == NULL){
		printf("Unable to load image %s! SDL Error: %s\n", "20die.webp", SDL_GetError());
		success = false;
	}
	return success;
}

void close()
{
	//Deallocate Surface
	SDL_FreeSurface(gDice);
	gDice = NULL;

	//Destroy Window
	SDL_DestroyWindow(gWindow);
	gWindow=NULL;

	//Quit SDl subsystems
	SDL_Quit();
}

int main(int argc, char* args[])
{
	if(!init()){
		printf("Failed to initialize!\n");
	}else{
		//load media
		if(!loadMedia()){
			printf("Failed to load media!\n");
		}else{
			//loop flag
			bool quit = false;
			//event handler
			SDL_Event e;

			while(!quit){
				//Handle events on queue
				while(SDL_PollEvent( &e ) != 0){
					//user requests quit
					if(e.type==SDL_QUIT){
						quit=true;
					}
				}
				//Apply image
				SDL_BlitSurface(gDice, NULL, gScreenSurface, NULL);
				//Update Surface
				SDL_UpdateWindowSurface(gWindow);
			}
		}
	}

	//Free resources and close
	close();

	return 0;
}