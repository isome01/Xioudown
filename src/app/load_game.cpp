//Dependencies
#include "game.h"
#include "player_inventory/player_inventory.h"

//std::string *game_items = new std::string;

//any other things required for the game ~~~ 
int load_game(){
    //if initialially we weren't able to initialize properly.
    if ( !Xioudown_Cat->gameStatusOK() ){
        log("Gamestate unable to continue.")
        return -1;
    }

    
    int lvl_count = 10;

    //This is where we will start the game and other things
    Xioudown_Cat->setLevelCount(lvl_count);
    Xioudown_Cat->setLevel(0);
    Xioudown_Cat->setRoom(0);

    log("Generating levels");
    Room::Room **Levels = new Room::Room*[lvl_count]; //a reference holder for where we allocate our levels

    //Add levels to the register
    log("Adding levels to registry...")

    srand(time(NULL)); //Start the random seed.
    for (int i = 0; i < lvl_count; i++){

        int lvl_var = i >= 5 ? (rand()%i + 10) : rand()%3 +10;
        Levels[i] = new Room::Level(i);

        //extract the list of levels and rooms generated from indexed level**
        std::vector<Room::Room*> *levels = Levels[i]->getRegistry();

        Xioudown_Cat->addLeveltoRegistry(*levels);
        //resetRandom();
    }

    //The instantiation of our player
    Player *player = new Player();
    log("Passed player instantiation.")

    //Create the interface for the player
    UI::UI_Interface *menu = new UI::UI_Interface("default");
    
    for (int i = 0; i < 3; i++)
        menu->addUIObj( new UI::Options_Box( (char*)"defaultus") );

    player->getMenuScreen()->addInterface( menu );
    //bool(*f)(SDL_Event *) = [](SDL_Event*e){ return true; };

    //Assign keyboard actions to the player
    player->addtoKBRegistry(SDLK_UP, [](SDL_Event*e ){ return true; } );

    //add any external methods
    player->addPlayerAssets( [](){ return; } );

    //add player to the catalyst
    Xioudown_Cat->setPlayer(player);

    Xioudown_Cat->finalize();

    return 0;
}

