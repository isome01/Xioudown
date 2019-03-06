#include "polygon.h"
#include <iostream>

#define log(x) std::cout << x << std::endl;

namespace Astral{ namespace Math {
    //these functions comprise version 2 of our collision detection
    bool Polygon::inside(int vert_x , int vert_y){ //To find if a certain point is inside
        int vertex_1, vertex_2;
        bool flag = false; //boolean flag

        //convert x and y coordinates to short values
        short vx = short(vert_x);
        short vy = short(vert_y);

        //our index and jump variable handles the first and last vertices as first case
        for (vertex_1 = 0, vertex_2 = vertices - 1; vertex_1 < vertices ; vertex_2 = vertex_1++) {

            //So, if our point has crossed line segments (including the fact that our point may be on the line)...
            if ( ((y[vertex_1] > vy) != (y[vertex_2] > vy)) &&
                 ( vx < ( x[vertex_2] - x[vertex_1] ) * ( vy - y[vertex_1] ) / ( y[vertex_2] - y[vertex_1] ) + x[vertex_1])
               ){
                   //then we flip the flag's boolean value
                    flag = !( flag ); //Even # of times flipped - should revert the flag to false.
                                          //Odd # of times flipped  - should revert the flag to true.
            }
        }
        return flag;
    }

    bool Polygon::inside(short vx , short vy){ //To find if a certain point is inside
        int vertex_1, vertex_2;
        bool flag = false; //boolean flag

        //our index and jump variable handles the first and last vertices as first case
        for (vertex_1 = 0, vertex_2 = vertices - 1; vertex_1 < vertices ; vertex_2 = vertex_1++) {

            //So, if our point has crossed line segments (including the fact that our point may be on the line)...
            if ( ((y[vertex_1] > vy) != (y[vertex_2] > vy)) &&
                 ( vx < ( x[vertex_2] - x[vertex_1] ) * ( vy - y[vertex_1] ) / ( y[vertex_2] - y[vertex_1] ) + x[vertex_1])
               ){
                   //then we flip the flag's boolean value
                    flag = !( flag ); //Even # of times flipped - should revert the flag to false.
                                          //Odd # of times flipped  - should revert the flag to true.
            }
        }
        return flag;
    }
}};
