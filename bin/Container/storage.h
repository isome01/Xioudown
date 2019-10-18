#pragma once
#include <SDL.h>
#include <vector> 
#include "../Essentials/paradigm.h"
#include "../TangObj/Animated_Obj/animatedObj.h"
#include "../TangObj/Inanimate_Obj/inanimateObj.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  Storage classes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 

//An item storage for any animated object; Will be used for inventory and item equip.
namespace Xioudown {namespace Container{
    using std::vector;

    #ifndef log
    #define log(x) std::cout << x << std::endl;
    #endif
    
    class Storage{

    /*This is basically a glorified array*/
    private:
        vector<TangObj::InanimateObj*> ia_inv;
        vector<TangObj::AnimatedObj* > a_inv;
        int STO_MAX, index;

    public:
        Storage(int n = 0);
        void store(TangObj::AnimatedObj *a = NULL); 
        void store(TangObj::InanimateObj* ia = NULL);
        void destroy(int n);

        //Polymorphic methods in which an object is retrieved
        TangObj::AnimatedObj* retrieveAOBJ(int n){ return a_inv[n]; }
        TangObj::InanimateObj* retrieveIAOBJ(int n){ return ia_inv[n]; }
        bool slotFull(int n) const{ return a_inv[n] || ia_inv[n]; }
        int getSize() const{ return ia_inv.size() + a_inv.size(); }
        int getMAX() const{ return STO_MAX; }
    };

    class Inventory : public Storage{

        private:
            SDL_Rect *invFoundation;
            SDL_Rect *inv_frame;
            SDL_Rect *inv_slot;
            SDL_Rect *slot_image;

            Essentials::gwTexturePack *slot_textures;
            std::string name;
            bool invDisplay; //A boolean switch for when the player displays inventory
            int array_row_col;

        public:
            Inventory(std::string s = "");
            ~Inventory();
            void listInventory();

            bool getInvDisplay()const{ return invDisplay; }
            void setInvDisplay(bool b){ invDisplay = b; }
            int getInvMax(){ return this->getMAX(); }
            void renderInventory(SDL_Renderer *r);
    };

    //Exclusive to the player
    class Item_Equip : public Storage{

        private:
            SDL_Rect *eqpFoundation;
            SDL_Rect *eqpSlots;
            Essentials::gwTexturePack *eqp_slots;


        public:
            Item_Equip();
            ~Item_Equip();

            bool is_item_equipped(){
                if (this->getSize() <= 0)
                    return false;

                log("Size: " << this->getSize())
                int sz = getMAX();
                for (int i = 0; i < sz; i++){

                }
                return true;
            }
    };
};};