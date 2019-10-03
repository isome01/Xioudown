#pragma once
//This is the interface of our engine.

#include "Essentials/game_io/game_io.h"
#include "Essentials/timer.h"
#include "Essentials/paradigm.h"
#include "Essentials/textureManager.h"

#include "UI/uiObj/uiObj.h" //UI for the player to interact with

#include "Math/methods.h" //depends on paradigm.h
#include "Math/polygon.h" // .. paradigm.h
#include "Catalyst/gameWindow.h" //polygon.h
#include "Essentials/objTextureSheet.h" //timer and paradigm.h
#include "TangObj/tangibleObj.h" // timer, polygon, paradigm, and objTextureSheet.h
#include "TangObj/Animated_Obj/animatedObj.h" //tangibleObj.h, timer.h
#include "TangObj/Inanimate_Obj/inanimateObj.h" //animatedObj, tangibleObj.h, timer.h
#include "Container/storage.h" //Inanimate and animateObj.h

//~ Interactive UI at ease of the player
#include "UI/uiObj/uiObj.h"
#include "UI/uiObj/options_box.h"
#include "UI/viewport.h"
#include "UI/UI_Interface/ui_interface.h"
#include "UI/interfaceManager/interfaceManager.h"

//Player stuff
#include "Essentials/player/player.h" //relies on game_io, storage, and tangibleObj.h

//AoE actions n such
#include "ObjAction/game_actions.h" // timer, paradigm, animatedObj.h

#include "Room/room.h" //paradigm, objTextureSheet, animated, inanimate, game_actions.h
#include "Room/Level_Room_Sub/level_room_sub.h" //room.h
#include "Room/Level_Room/level_room.h" //room
#include "Room/Level/level.h"

#include "Catalyst/gameCatalyst.h"


//For the sake of usability, let's create a shortcut for the catalyst/singleton 
#ifndef Xioudown_Cat

//define catalyst instantiator
#define Inst_Catalyst(inst) gameCatalyst* gameCatalyst::world = inst

//define nickname for catalyst instance
#define Xioudown_Cat gameCatalyst::gWorld()

#endif


//Still need to add Environment and Environment Actions to a Folder