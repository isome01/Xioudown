#include "xioudown.h"
#include "aspects.h"

#ifndef CLIENT
#define CLIENT

using namespace Xioudown;
using namespace Essentials;

class Client {
    // Simple client class that generalizes a user/player
    private:
        // A client can have a keyboard
        // A client can have an avatar
        // A client can own statistics and user configs
        // A client can be a user/player

    public:
        Client();
        ~Client();
};

#endif
