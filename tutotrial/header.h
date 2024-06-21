#ifndef HEADER_H
#define HEADER_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdio.h>

//Screen dimension constants
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
extern SDL_Window* gWindow;
    
//The surface contained by the window
extern SDL_Surface* gScreenSurface;

//The image we will load and show on the screen
extern SDL_Surface* gHelloWorld;
#endif /* HEADER_H*/