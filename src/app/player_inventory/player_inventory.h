#include "../game.h"

class Player_1 : public Player{

    public:
        Player_1(){}
        ~Player_1(){}
};

class Player_Inventory : public interfaceManager{

    private:
        char* player_name;
        Astral::Container::Storage *player_inventory;
    
    public:
        Player_Inventory(char* _player_name) : player_name(_player_name), interfaceManager(){
            
            //initialize our storage space
            player_inventory = new Astral::Container::Storage(4);
        }
        ~Player_Inventory();

        void renderCurrentInterface(SDL_Renderer *r){}
};

class Storage_Obj : public UI_Obj {

    private:
        InanimateObj *obj_a;
        AnimatedObj *obj_b;

    public:
        Storage_Obj(char* _obj_name) : UI_Obj(_obj_name) {

        }

        ~Storage_Obj();

    public:
        //setter for the containing object
        void renderUI(SDL_Renderer *r){
            //render accordingly 
        }

        void onChoose(){
            if (obj_a){
                obj_a->use();
                obj_a = NULL;
            }
        }
};