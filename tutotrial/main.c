#include "header.h"

int main(int argc, char* args[]) {
    //Start up SDL and create window
    if (!init()) {
        printf("Failed to initialize!\n");
        return 1;
    }

    //Load media
    if (!loadMedia()) {
        printf("Failed to load media!\n");
        close(); // Ensure cleanup before returning
        return 1;
    }

    //Apply the image
    SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
    //Update the surface
    SDL_UpdateWindowSurface(gWindow);

    //Hack to get window to stay up
    SDL_Event e; 
    bool quit = false; 
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) 
                quit = true; 
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}
