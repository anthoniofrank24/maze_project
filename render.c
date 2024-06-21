#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Map dimensions and map definition
#define MAP_WIDTH 8
#define MAP_HEIGHT 8

#define MAP_WALL 1

// Player position and direction
double posX = 2.0, posY = 2.0;
double dirX = -1.0, dirY = 0.0; // Initial direction vector
double planeX = 0.0, planeY = 0.66; // Camera plane

int getMapValue(int x, int y) {
    // Define your map layout here
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        return MAP_WALL; // Assuming anything outside the map boundary is a wall
    }

    if ((x == 0 || x == MAP_WIDTH - 1) || (y == 0 || y == MAP_HEIGHT - 1)) {
        return MAP_WALL; // Border walls
    }

    if ((x == 3 || x == 4) && (y == 2 || y == 5)) {
        return MAP_WALL; // Inner walls
    }

    return 0; // Assuming non-wall spaces are represented by 0
}

void render(SDL_Renderer* renderer) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        // Calculate ray position and direction
        double cameraX = 2 * x / (double)SCREEN_WIDTH - 1;
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        // Length of ray from current position to next x or y side
        double sideDistX, sideDistY;

        // Length of ray from one x or y side to next x or y side
        double deltaDistX = fabs(1 / rayDirX);
        double deltaDistY = fabs(1 / rayDirY);
        double perpWallDist;

        // Direction to go in x and y
        int stepX, stepY;

        int hit = 0; // Was there a wall hit?
        int side; // Was a NS or a EW wall hit?

        // Calculate step and initial sideDist
        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (posX - (int)posX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = ((int)posX + 1.0 - posX) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (posY - (int)posY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = ((int)posY + 1.0 - posY) * deltaDistY;
        }

        // Perform DDA
        while (hit == 0) {
            // Jump to next map square, OR in x-direction, OR in y-direction
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
               posX = (int)posX + stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
               posY = (int)posY + stepY;
                side = 1;
            }
            // Check if ray has hit a wall
            if (getMapValue((int)posX, (int)posY) > 0) hit = 1;
        }

        // Calculate distance projected on camera direction (perpendicular distance)
        if (side == 0) perpWallDist = (fabs((int)posX - posX) + (1 - stepX) / 2) / rayDirX;
        else          perpWallDist = (fabs((int)posY - posY) + (1 - stepY) / 2) / rayDirY;

        // Calculate height of line to draw on screen
        int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

        // Calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        printf("drawStart: %d\n", drawStart);
        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;
        printf("drawEnd: %d\n", drawEnd);
        
        // Choose wall color
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        if (side == 1) { SDL_SetRenderDrawColor(renderer, 127, 0, 0, 255); } // Darker for y-side

        // Draw the vertical stripe
        SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
    }
}

int main(int argc, char* args[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("Raycaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Main loop flag
    int quit = 0;

    // Event handler
    SDL_Event e;

    // While application is running
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render scene
        render(renderer);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Destroy window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}
