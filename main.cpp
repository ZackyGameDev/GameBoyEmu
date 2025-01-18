#include <iostream>
// #include <SDL2/SDL.h>
#include "SM83.h"
#include "Bus.h"
#include <bitset>


int main() {
    std::cout << "HERE IS THE OUPTUT look at ME <--------------\n";

    Bus bus;

    while(true) {
        bus.cpu.clock();

        uint8_t keyin = bus.cpuRead(0xff00);
        std::cout << std::bitset<8>(keyin) << std::endl;

        Sleep(1);
    }

}


// #define SCREEN_WIDTH 1280 
// #define SCREEN_HEIGHT 720

// int main(int argc, char** argv){

//     std::cout << "HERE IT IS <---------------\n";

//     SM83 cpu;
//     cpu.clock();

// //     if(SDL_Init(SDL_INIT_VIDEO) < 0){
// //         printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
// //         return 1;
// //     }

// //     SDL_Window *window = SDL_CreateWindow("SLD test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
// //     if(!window){
// //         printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
// //         return 1;
// //     }

// //     SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
// //     if(!renderer){
// //         printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
// //         return 1;
// //     }

// //     bool running = true;
// //     while(running){
// //         SDL_Event event;
// //         while(SDL_PollEvent(&event)){
// //             switch(event.type){
// //                 case SDL_QUIT:
// //                     running = false;
// //                     break;

// //                 default:
// //                     break;
// //             }
// //         }

// //         SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
// //         SDL_RenderClear(renderer);

// //         SDL_RenderPresent(renderer);
// //     }

// //     return 0;
// }