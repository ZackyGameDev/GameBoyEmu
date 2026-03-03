#include <iostream>
#include <chrono>
#include <thread>
#include <filesystem>

#include "Bus.h"

constexpr double TARGET_RATE = EMULATION_CLOCK_SPEED; // M-cycles/sec
constexpr double WINDOW_SECONDS = 0.25;

int main() {

    std::cout << "Starting emulator\n";
    std::filesystem::path cwd = std::filesystem::current_path();
    std::cout << "Working dir: " << cwd << std::endl;

    Bus bus;
    bus.ppu.initLCD();

    using clock = std::chrono::high_resolution_clock;

    double speed_multiplier = 1.0;

    auto previous = clock::now();
    auto window_start = previous;

    double accumulator = 0.0;
    double base_cycles_this_window = 0.0;

    while (bus.running) {

        auto now = clock::now();
        double elapsed =
            std::chrono::duration<double>(now - previous).count();
        previous = now;

        accumulator += elapsed * TARGET_RATE * speed_multiplier;

        // Cap debt (100ms max)
        double max_debt = TARGET_RATE * 0.1;
        if (accumulator > max_debt)
            accumulator = max_debt;

        while (accumulator >= 1.0) {
            bus.cpu.clock();   // 1 M-cycle
            // Advance PPU 4 T-cycles
            bus.ppu.clock();
            bus.ppu.clock();
            bus.ppu.clock();
            bus.ppu.clock();

            accumulator -= 1.0;

            // Remove multiplier influence
            base_cycles_this_window += 1.0 / speed_multiplier;
        }

        double window_elapsed =
            std::chrono::duration<double>(now - window_start).count();

        if (window_elapsed >= WINDOW_SECONDS) {

            double base_rate =
                base_cycles_this_window / window_elapsed;

            if (base_rate > 0.0) {
                speed_multiplier = TARGET_RATE / base_rate;
            }

            // Clamp to [0.1, 15.0]
            if (speed_multiplier < 0.1) speed_multiplier = 0.1;
            if (speed_multiplier > 15.0) speed_multiplier = 15.0;

            std::cout << "Base Rate: " << base_rate
                      << " | Multiplier: "
                      << speed_multiplier << std::endl;

            base_cycles_this_window = 0.0;
            window_start = now;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(50));
    }

    return 0;
}

    // while (bus.running) {
    //     #ifdef INFINITE_CLOCK_SPEED
    //     bus.cpu.clock();
    //     bus.ppu.clock();
    //     continue;
    //     #endif
    //     auto now = std::chrono::high_resolution_clock::now();
    //     auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - last_time).count();

    //     if (elapsed >= ns_per_cycle) {
    //         bus.cpu.clock();  // Execute one clock cycle
    //         bus.ppu.clock();
    //         last_time = now;
    //     } else {
    //         std::this_thread::sleep_for(std::chrono::nanoseconds(static_cast<int>(ns_per_cycle - elapsed)));
    //     }
    // }

    // Reset timer granularity before exit.
    // timeEndPeriod(1);

    // // using namespace std::chrono;
    // auto last_time = std::chrono::high_resolution_clock::now();
    // double ns_per_cycle = 1'000'000'000.0 / CLOCK_SPEED;

    // using Clock = std::chrono::steady_clock;
    // constexpr auto NANOSECONDS_PER_SECOND = 1'000'000'000LL; // 1 second in nanoseconds
    // // const std::chrono::nanoseconds clock_period(NANOSECONDS_PER_SECOND / (long int)CLOCK_SPEED);
    
    // auto next_time = Clock::now();


    // using Clock = std::chrono::steady_clock;
    // // Calculate the period per clock tick based on CLOCK_SPEED.
    // const auto clock_period = std::chrono::nanoseconds(NANOSECONDS_PER_SECOND / (int)CLOCK_SPEED);

    // // Pin point: record the starting time and cycle count.
    // auto start_time = Clock::now();
    // uint64_t cycle_count = 0;

    // while (bus.running) {
    //     // Execute one emulation cycle.
    //     bus.cpu.clock();
    //     bus.ppu.clock();
    //     ++cycle_count;

    //     // Compute the expected time for the current cycle.
    //     auto expected_time = start_time + (clock_period * cycle_count);

    //     // Sleep until the expected time is reached.
    //     std::this_thread::sleep_until(expected_time);
    // }


    // while (bus.running) {
    //     bus.cpu.clock(); // Call the clock function
    //     bus.ppu.clock(); 

    //     // Schedule next tick
    //     next_time += clock_period;

    //     // Sleep until the next tick
    //     std::this_thread::sleep_until(next_time);
    // }

    // while (bus.running) {
    //     bus.cpu.clock();  // Execute one clock cycle
    //     bus.ppu.clock();
    // }

    // while (bus.running) {
    //     auto start = std::chrono::high_resolution_clock::now();
    //     bus.cpu.clock();  // Execute one clock cycle
    //     bus.ppu.clock();
    //     auto end = std::chrono::high_resolution_clock::now();
        
    //     auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    //     if (elapsed > ns_per_cycle) {
    //         // std::cout << "Cycle took too long: " << elapsed << " nanoseconds" << std::endl;
    //     } else {
    //         std::this_thread::sleep_for(std::chrono::nanoseconds(static_cast<int>(ns_per_cycle - elapsed)));
    //     }
    // }

    // while (bus.running) {
    //     auto now = std::chrono::high_resolution_clock::now();
    //     auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - last_time).count();

    //     if (elapsed >= ns_per_cycle) {
    //         bus.cpu.clock();  // Execute one clock cycle
    //         bus.ppu.clock();
    //         last_time = now;
    //     } else {
    //         std::this_thread::sleep_for(std::chrono::nanoseconds(static_cast<int>(ns_per_cycle - elapsed)));
    //     }
    // }

    
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

// }


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