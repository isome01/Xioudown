#include <iostream>
#include <stdlib.h>
#include <SDL.h>
#include "aspects.h"

gInstancedApp *app = new gInstancedApp();

int main (int argc, char **argv) {
    
    app->render();

    system("pause");
    return EXIT_SUCCESS;
}