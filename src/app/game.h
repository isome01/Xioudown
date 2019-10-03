#pragma once

//Dependencies
#include "../Xioudown.h"

#include <stdlib.h>
#include <iostream>
#include <ctime>

//game items and types, along with variable types.
#include <string>

#ifndef GAME_ITEMS
#define GAME_ITEMS

static std::string *game_items;
enum game_items {

};
#endif

using namespace Xioudown;
using namespace Catalyst;
using namespace UI;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int load_game(); //The main load_game function -> Where we instantiate objects in the catalyst
bool load_fonts(); //a function that handles loading fonts into the manager
bool load_texturing(); //a function that handles loading textures into the manager
