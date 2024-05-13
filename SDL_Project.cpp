#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>
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

//load inidividual image as texture
SDL_Texture* loadTexture( std::string path);

//Avoid these global variables
//Render window
SDL_Window* gWindow = NULL;

//Window renderer
SDL_Renderer* gRenderer = NULL;

//Surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Image to load and show
SDL_Surface* gKeyPressSurface[ KEY_PRESS_SURFACE_TOTAL ];

//Current displayed image
SDL_Surface* gStretchedSurface = NULL;

//current texture
SDL_Texture* gTexture = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init(SDL_INIT_VIDEO)<0){
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success=false;
	}else{
		//set texture filtering to linear
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
			printf("Warning: Linear texture filtering not enabled!");
		}

		//create window
		gWindow = SDL_CreateWindow("SDL Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL){
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success=false;
		}else{
			//get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);

			//create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED);
			if(gRenderer ==NULL){
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success=false;
			}else{
				//initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags)&imgFlags)){
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success=false;
				}
			}

		}
	}
	return success;
}

bool loadMedia(){
	bool success = true;

	//rendering
	gTexture = loadTexture("images/4.bmp");
	if(gTexture==NULL){
		printf("Failed to load texture image!\n");
		success=false;
	}

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
	//Free loaded image
	SDL_DestroyTexture(gTexture);
	gTexture=NULL;

	//Deallocate Surface
	for(int i=0;i< KEY_PRESS_SURFACE_TOTAL; ++i){
		SDL_FreeSurface(gKeyPressSurface[i]);
		gKeyPressSurface[i] = NULL;
	}

	//Destroy Window
	SDL_DestroyWindow(gWindow);
	gWindow=NULL;

	//Quit SDl subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Surface* loadSurface( std::string path)
{
	//final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if(loadedSurface==NULL){
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}else{
		//convert to screen format
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
		if(optimizedSurface ==NULL){
			printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
}

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
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
			gStretchedSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];

			while(!quit){
				//Handle events on queue
				while(SDL_PollEvent( &e ) != 0){
					//user requests quit
					if(e.type==SDL_QUIT){
						quit=true;
					}else if(e.type==SDL_KEYDOWN){
						switch(e.key.keysym.sym){
							case SDLK_UP:
								gStretchedSurface = gKeyPressSurface[KEY_PRESS_SURFACE_UP];
								break;
							case SDLK_DOWN:
								gStretchedSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DOWN];
								break;
							case SDLK_LEFT:
								gStretchedSurface = gKeyPressSurface[KEY_PRESS_SURFACE_LEFT];
								break;
							case SDLK_RIGHT:
								gStretchedSurface = gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT];
								break;
							default:
								gStretchedSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];
								break;
						}
					}
				}

				//Apply image
				SDL_Rect stretchRect;
				stretchRect.x=0;
				stretchRect.y=0;
				stretchRect.w=SCREEN_WIDTH;
				stretchRect.h=SCREEN_HEIGHT;
				SDL_BlitScaled(gStretchedSurface, NULL, gScreenSurface, &stretchRect);
				//Update Surface
				SDL_UpdateWindowSurface(gWindow);
			}
		}
	}

	//Free resources and close
	close();

	return 0;
}