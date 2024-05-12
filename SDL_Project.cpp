#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 1024;

//Key press surfaces constants
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

//Start SDL and create window
bool init();

//Load media
bool loadMedia();

//Free media and shut down SDL
void close();

//load individual image
SDL_Surface* loadSurface(std::string path);

//Avoid these global variables
//Render window
SDL_Window* gWindow = NULL;

//Surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Image to load and show
SDL_Surface* gKeyPressSurface[ KEY_PRESS_SURFACE_TOTAL ];

//Current displayed image
SDL_Surface* gCurrentSurface = NULL;

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

	gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT]=loadSurface("images/cat.bmp");
	if(gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT] == NULL){
		printf("Unable to load default image!\n");
		success = false;
	}

	gKeyPressSurface[KEY_PRESS_SURFACE_UP]=loadSurface("images/1.bmp");
	if(gKeyPressSurface[KEY_PRESS_SURFACE_UP]==NULL){
		printf("Unable to load up image!\n");
		success = false;
	}

	gKeyPressSurface[KEY_PRESS_SURFACE_DOWN]=loadSurface("images/20die.bmp");
	if(gKeyPressSurface[KEY_PRESS_SURFACE_DOWN]==NULL){
		printf("Unable to load down image!\n");
		success = false;
	}

	gKeyPressSurface[KEY_PRESS_SURFACE_LEFT]=loadSurface("images/2.bmp");
	if(gKeyPressSurface[KEY_PRESS_SURFACE_LEFT]==NULL){
		printf("Unable to load left image!\n");
		success = false;
	}

	gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT]=loadSurface("images/3.bmp");
	if(gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT]==NULL){
		printf("Unable to load right image!\n");
		success = false;
	}

	return success;
}

void close()
{
	//Deallocate Surface
	for(int i=0;i< KEY_PRESS_SURFACE_TOTAL; ++i){
		SDL_FreeSurface(gKeyPressSurface[i]);
		gKeyPressSurface[i] = NULL;
	}

	//Destroy Window
	SDL_DestroyWindow(gWindow);
	gWindow=NULL;

	//Quit SDl subsystems
	SDL_Quit();
}

SDL_Surface* loadSurface( std::string path)
{
	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if(loadedSurface==NULL){
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}

	return loadedSurface;
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
			//set default surface
			gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];

			while(!quit){
				//Handle events on queue
				while(SDL_PollEvent( &e ) != 0){
					//user requests quit
					if(e.type==SDL_QUIT){
						quit=true;
					}else if(e.type==SDL_KEYDOWN){
						switch(e.key.keysym.sym){
							case SDLK_UP:
								gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_UP];
								break;
							case SDLK_DOWN:
								gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DOWN];
								break;
							case SDLK_LEFT:
								gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_LEFT];
								break;
							case SDLK_RIGHT:
								gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT];
								break;
							default:
								gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];
								break;
						}
					}
				}

				//Apply image
				SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
				//Update Surface
				SDL_UpdateWindowSurface(gWindow);
			}
		}
	}

	//Free resources and close
	close();

	return 0;
}