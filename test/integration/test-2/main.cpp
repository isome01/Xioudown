#include <iostream>
#include <stdlib.h>
#include "aspects.h"

gInstancedApp *app = new gInstancedApp();

using namespace std;

int main (int argc, char **argv) {
    
    cout << "hello" << endl;
    app->render();

    // system("pause");
    return EXIT_SUCCESS;
}