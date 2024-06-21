#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_events.h>

// Define constants for window dimensions and map size
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAP_WIDTH 24
#define MAP_HEIGHT 24

// define colors for walls, ground and ceiling
#define WALL_COLOR      0xFF0000 // Red
#define GROUND_COLOR    0x00FF00 // Green
#define CEILING_COLOR   0x0000FF // Blue

// Define map layout
int map[MAP_WIDTH][MAP_HEIGHT] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// Function to initialize SDL and creat window
bool initSDL(SDL_Window **window, SDL_Renderer ** renderer) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create window
    *window = SDL_CreateWindow("Raycasting Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create renderer
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) {
        printf("Renderer could not be created! SDL_ERROR: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

// Function to render scene
void render(SDL_Renderer *renderer) {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render walls, ground, and ceiling based on map data
    for (int x = 0; x < MAP_WIDTH; ++x) {
        for (int y = 0; y < MAP_HEIGHT; ++y) {
            // Calculate position and size of the xurrent wall rectangle
            SDL_Rect wallRect = {
                x * (SCREEN_WIDTH / MAP_WIDTH), // x-cordinate
                y * (SCREEN_HEIGHT / MAP_HEIGHT), // y-cordinate
                SCREEN_WIDTH / MAP_WIDTH,
                SCREEN_HEIGHT / MAP_HEIGHT
            };

            // Set color based on map data
            int mapValue = map[y][x];
            if (mapValue == 1) {
                SDL_SetRenderDrawColor(renderer, (WALL_COLOR >> 16) & 0xFF, (WALL_COLOR >> 8) & 0xFF, WALL_COLOR & 0xFF, 255); // Red
            }
            else if (mapValue == 2) {
                SDL_SetRenderDrawColor(renderer, (GROUND_COLOR >> 16) & 0xFF, (GROUND_COLOR >> 8) & 0xFF, GROUND_COLOR & 0xFF, 255); //Green
            }
            else if (mapValue == 3) {
                SDL_SetRenderDrawColor(renderer, (CEILING_COLOR >> 16) & 0xFF, (CEILING_COLOR >> 8) & 0xFF, CEILING_COLOR & 0xFF, 255); //Blue
            }

            // Draw the wall rectangle
            SDL_RenderFillRect(renderer, &wallRect);
        }
    }

    // Present the renderer
    SDL_RenderPresent(renderer);
}

// Function to handle user input
void handleInput(bool *quit) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *quit = true;
        }
        else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_LEFT) {

            }
            else if (event.key.keysym.sym == SDLK_RIGHT) {

            }
        }
    }
}

// Main function 
int main (int argc, char *argv[]) {
    // Initialize SDL
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if (!initSDL(&window, &renderer)) {
        return 1;
    }

    // Main loop
    bool quit = false;
    while (!quit) {
        // Handle events
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Handle user input
        handleInput(&quit);

        // Render scene
        render(renderer);
    }

    // Cleanup and exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
