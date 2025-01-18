#pragma once

class Bus;

class PPU {

public:
    PPU();
    ~PPU();

public:
    Bus *bus = nullptr;
    void connectBus(Bus *n) { bus = n; }

};