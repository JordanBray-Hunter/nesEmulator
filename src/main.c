#include <raylib.h>
#include "cartridge.h"
#include "bus.h"
#include <stdio.h>

int main(int argc, char** argv){

    if(argc < 2){
        printf("Error: please spesify rom file\n");
        return 1;
    }

    const int screenWidth = 256;
    const int screenHeight = 240;
    const int scale = 3;

    Cartridge cartridge;
    Bus bus;

    cartridge_load(&cartridge, argv[1]);
    bus_init(&bus,&cartridge);


    SetTargetFPS(60);


    InitWindow(screenWidth*scale,screenHeight*scale,"EMULATOR");




    while(!WindowShouldClose()){

        BeginDrawing();
        ClearBackground(WHITE);

        

        EndDrawing();




    }

    CloseWindow();


    cartridge_unload(&cartridge);

    return 0;

}