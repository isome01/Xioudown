#include <stdlib.h>
#include "../Catalyst/gameCatalyst.h"
#include "../Essentials/objTextureSheet.h"
#include "../Math/methods.h"
#include "../Math/polygon.h"
#include "../TangObj/Animated_Obj/animatedObj.h"
#include "../TangObj/Inanimate_Obj/inanimateObj.h"
#include "storage.h"

using std::vector;

const int SCREEN_WIDTH = Astral::Catalyst::gameCatalyst::gWorld()->getWindow()->getWidth();
const int SCREEN_HEIGHT = Astral::Catalyst::gameCatalyst::gWorld()->getWindow()->getHeight();

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Storage implementation and methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace Astral { namespace Container{

    using namespace Math;
    using namespace TangObj;

    Storage::Storage(int n) : STO_MAX(n), index(0) {
        //initialize all values to NULL
        /* Cannot index an empty vector at further indexes: 'pushback' method is optimal*/

        log("Created storage space.")
    }

    void Storage::store(AnimatedObj *a){

        if ( int(a_inv.size()) + int(ia_inv.size()) < STO_MAX){

            //We iterate based on the current index.
            if (a){
                a_inv[index] = a;
                index++;
            } else log("Unable to add item. Item NULL")
        }else log("Storage is full.")
    }

    void Storage::store(InanimateObj* ia){

        if ( int(a_inv.size()) + int(ia_inv.size()) < STO_MAX){

            if (ia){
                ia_inv[index] = ia;
                index++;
            } else log("Unable to add item. Item NULL")
        }else log("Storage is full.")
    }

    void Storage::destroy(int n) {

        if (a_inv[n]){

            a_inv.erase( a_inv.begin() + n );
            a_inv[n] = NULL;

            index = 0;//reset index
            //reiterate the vector and place index at the leftmost index.
            while(index < STO_MAX){
                if (a_inv[index] != NULL || ia_inv[index]!= NULL){
                    break;
                } else index++;
            }log("Animated object destroyed in inventory.")
        } else if (ia_inv[n]){

            ia_inv.erase( ia_inv.begin() + n );
            ia_inv[n] = NULL;

            index = 0;//reset index
            //reiterate the vector and place index at the leftmost index.
            while(index < STO_MAX){
                if (a_inv[index] != NULL || ia_inv[index]!= NULL){
                    break;
                } else index++;
            }log("Inanimate object destroyed in inventory.")
        } else log("Destruction invalid.")
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Player inventory graphics and methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Inventory::Inventory(std::string s) : Storage(16), name(s), invDisplay(false){

        log("Building inventory.")

        //Storing max values from storage
        array_row_col = cl_SQRT(this->getMAX());
        int _max = this->getMAX();

        //Creating the inventory foundation UI
        invFoundation = new SDL_Rect;

        invFoundation->x = 9;
        invFoundation->y = 9;
        invFoundation->w = SCREEN_HEIGHT / 2;
        invFoundation->h = SCREEN_WIDTH / 2;

        //Creating the frame for inventory slots
        inv_frame = new SDL_Rect;
        inv_frame->x = 5000;
        inv_frame->y = -100;
        inv_frame->w = invFoundation->w;
        inv_frame->h = SCREEN_WIDTH / 2 - 100;

        inv_frame = p_centerSDLRect(*invFoundation, *inv_frame);

        gwTexture_Sheet src = { {0, 0, 100, 100}, Catalyst::gameCatalyst::gWorld()->getManager()->getTexture(3) };
        this->slot_textures = new Essentials::gwTexturePack( inv_frame, &src, 4 , 4 );

        //determine sizes for inventory slots
        this->slot_image = new SDL_Rect[_max];
        this->inv_slot = new SDL_Rect[_max];
        for (int i = 0; i < _max; i++){

            //set inventory slots based off of the inventory frame, where inventory slots are arrayed
            inv_slot[i].w = inv_frame->w/array_row_col;
            inv_slot[i].h = inv_slot[i].w;

            //likewise, for images of those items in inventory
            slot_image[i].w = inv_slot[i].w-5; //we'll set it to a percentage of inventory slot
            slot_image[i].h = slot_image[i].w;
        }

        for (int r = 0, run = 0; r < array_row_col; r++){

            for (int c = 0; c < array_row_col; c++, run++){

                //array each slot according to it's spot in inventory, and center it's image
                inv_slot[run].x = inv_frame->x + (c * inv_slot[run].w);
                inv_slot[run].y = inv_frame->y + (r * inv_slot[run].h);

                slot_image[run] = centerSDLRect(inv_slot[run], slot_image[run]);
            }
        }
    }

    Inventory::~Inventory(){

        delete invFoundation;
        delete inv_frame;

        if (this->getMAX()){
            delete [] inv_slot;
            delete [] slot_image;
        }
    }

    void Inventory::renderInventory(SDL_Renderer *r){


        if (invDisplay){

            int _max = getMAX();

            //Render the foundation
            SDL_SetRenderDrawColor(r, 165, 42, 42, 10);
            SDL_RenderFillRect(r, invFoundation);
            /*
            //Render the slots and then
            //render the slot images
            for (int i = 0; i < _max; i++){

                if (!slotFull(i)){
                    SDL_SetRenderDrawColor(r, 0xff, 0xff, 125, 0xff);
                    SDL_RenderFillRect(r, &slot_image[i]);
                } else continue;
            }*/

            slot_textures->render(r);

            //reset display flag
            invDisplay = false;
        }

        return;
    }

    void Inventory::listInventory(){
        return;
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Item Equipment Storage ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    Item_Equip::Item_Equip() : Storage(1) {

    }

};};