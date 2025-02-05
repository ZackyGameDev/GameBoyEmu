f = open("ROMS/bootromwithjump.gb", "wb")
bootrom = open("ROMS/dmg_boot.bin", "rb")
some_game = open("ROMS/Tetris (Japan) (En).gb", "rb")
bootrombytes = bootrom.read()
gamebytes = some_game.read()
to_write_bytes = []
for i in range(0, 0x8000):
    if (i < 0x100):
        to_write_bytes.append(bootrombytes[i])
    else:
        to_write_bytes.append(gamebytes[i])


f.write(bytes(to_write_bytes))



f.close()


















