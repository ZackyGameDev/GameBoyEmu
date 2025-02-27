#include <iostream>
// #include <SDL2/SDL.h>
#include <chrono>
#include <thread>
#include <filesystem>
#include <bitset>

#include "Bus.h"

// const int CPU_CLOCK_SPEED = 4194304; // clock cycle speed
long long int CPU_CLOCK_SPEED = EMULATION_CLOCK_SPEED; // machine cycle speed 
#ifdef DEBUGMODE_
// const float DEBUG_UPDATE_SPEED = 0.1F;
const int DEBUG_UPDATE_CYCLES = 500;
int debug_cycles = 0;
#endif
// const int CPU_CLOCK_SPEED = 300;
const long double CYCLE_DURATION = 1.0 / (long double)CPU_CLOCK_SPEED;
// auto CYCLE_DURATION = std::chrono::nanoseconds(EMULATION_CLOCK_SPEED);

int main() {
    std::cout << "DID IT WORK <--------------\n";
    std::filesystem::path cwd = std::filesystem::current_path();
    std::cout << "Current working directory: " << cwd << std::endl;

    Bus bus;
    // bus.cpu.boot();
    // std::this_thread::sleep_for(std::chrono::duration<double>(3));
    bus.ppu.initLCD();
    #ifdef DEBUGMODE_
    CPU_CLOCK_SPEED *= DEBUG_CLOCK_SPEED;
    // bus.cpu.opDebug();
    #endif
    // Sleep(100);
    // std::cout << "CPU_CLOCK_SPEED: " << CPU_CLOCK_SPEED << std::endl;

    int test = 0;


    constexpr double CLOCK_SPEED = EMULATION_CLOCK_SPEED; // 4.194304 MHz

    // using namespace std::chrono;
    auto last_time = std::chrono::high_resolution_clock::now();
    double ns_per_cycle = 1'000'000'000.0 / CLOCK_SPEED;

    while (bus.running) {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - last_time).count();

        if (elapsed >= ns_per_cycle) {
            bus.cpu.clock();  // Execute one clock cycle
            bus.ppu.clock();
            last_time = now;
        } else {
            std::this_thread::sleep_for(std::chrono::nanoseconds(static_cast<int>(ns_per_cycle - elapsed)));
        }
    }

    
    // while (true) {
    //     // Get the start time of the cycle
    //     auto start_cycle_time = std::chrono::high_resolution_clock::now();

    //     #ifndef NO_CPU
    //     bus.cpu.clock();
    //     #endif
    //     bus.ppu.clock();

    //     // #ifdef DEBUGMODE_
    //     // if (debug_cycles == 0) {
    //     //     bus.cpu.drawDebug();
    //     //     debug_cycles = DEBUG_UPDATE_CYCLES;
    //     // }
    //     // debug_cycles--;
    //     // #endif

    //     // Get the end time of the cycle
        
    //     // Get the end time of the cycle
    //     auto end_cycle_time = std::chrono::high_resolution_clock::now();

    //     // Calculate the elapsed time for the cycle
    //     std::chrono::duration<double> elapsed_time = end_cycle_time - start_cycle_time;

    //     // Calculate the time to sleep to match the cycle duration
    //     double sleep_time = CYCLE_DURATION - elapsed_time.count();
    //     #ifdef DEBUGMODE_
    //     std::cout << "sleep_time: " << sleep_time << std::endl;
    //     #endif
    //     if (sleep_time > 0) {
    //         std::this_thread::sleep_for(std::chrono::duration<double>(sleep_time));
    //     } else {
    //         // std::cout << "Cycle took too long: " << elapsed_time.count() << " seconds" << std::endl;
    //     }

    //     if (bus.running == false) 
    //         break;
        
    // }

    // while(true) {
    //     bus.cpu.clock();

    //     uint8_t keyin = bus.cpuRead(0xff00);
    //     std::cout << std::bitset<8>(keyin) << std::endl;

    //     Sleep(1);
    // }

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