#pragma once

#ifndef XIOUDOWN_H
#define XIOUDOWN_H
#endif

//This is the interface of our engine.
#include "../../bin/Essentials/paradigm.h"
#include "../../bin/Essentials/game_io/game_io.h"
#include "../../bin/Essentials/timer.h"
#include "../../bin/Essentials/textureManager.h"
#include "../../bin/Essentials/Vectors/vector.h"

#include "../../bin/UI/uiObj/uiObj.h" //UI for the player to interact with

#include "../../bin/Math/methods.h" //depends on paradigm.h
#include "../../bin/Math/polygon.h" // .. paradigm.h
#include "../../bin/Catalyst/gameWindow.h" //polygon.h
#include "../../bin/Essentials/objTextureSheet.h" //timer and paradigm.h
#include "../../bin/TangObj/tangibleObj.h" // timer, polygon, paradigm, and objTextureSheet.h
#include "../../bin/TangObj/Animated_Obj/animatedObj.h" //tangibleObj.h, timer.h
#include "../../bin/TangObj/Inanimate_Obj/inanimateObj.h" //animatedObj, tangibleObj.h, timer.h
#include "../../bin/Container/storage.h" //Inanimate and animateObj.h

//~ Interactive UI at ease of the player
#include "../../bin/UI/uiObj/uiObj.h"
#include "../../bin/UI/uiObj/options_box.h"
#include "../../bin/UI/viewport.h"
#include "../../bin/UI/UI_Interface/ui_interface.h"
#include "../../bin/UI/interfaceManager/interfaceManager.h"

//Player stuff
#include "../../bin/Essentials/player/player.h" //relies on game_io, storage, and tangibleObj.h

//AoE actions n such
#include "../../bin/ObjAction/game_actions.h" // timer, paradigm, animatedObj.h

#include "../../bin/Room/room.h" //paradigm, objTextureSheet, animated, inanimate, game_actions.h
#include "../../bin/Room/Level_Room_Sub/level_room_sub.h" //room.h
#include "../../bin/Room/Level_Room/level_room.h" //room
#include "../../bin/Room/Level/level.h"

#include "../../bin/Catalyst/gameCatalyst.h"
#include "../../bin/Essentials/Grid/grid.h"


//For the sake of usability, let's create a shortcut for the catalyst/singleton 
#ifndef Xioudown_Cat
using namespace Xioudown;
using namespace Catalyst;
//define catalyst instantiator
#define Inst_Catalyst(inst) gameCatalyst* gameCatalyst::world = inst

//define nickname for catalyst instance
#define Xioudown_Cat gameCatalyst::gWorld()

#endif


//Still need to add Environment and Environment Actions to a Folder