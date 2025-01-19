#pragma once

class Bus;

class PPU {

public:
    PPU();
    ~PPU();

private:
    std::vector<uint8_t> oam;
    uint8_t lcdc = 0x00;
    uint8_t stat = 0x00;
    uint8_t scy = 0x00;
    uint8_t scx = 0x00;
    uint8_t ly = 0x00;
    uint8_t lyc = 0x00;
    uint8_t dma = 0x00;
    uint8_t bgp = 0x00;
    uint8_t obp0 = 0x00;
    uint8_t obp1 = 0x00;
    uint8_t wy = 0x00;
    uint8_t wx = 0x00;


public:
    Bus *bus = nullptr;
    void connectBus(Bus *n) { bus = n; }

public:
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr);
    uint8_t* cpuReadPttr(uint16_t addr);

};