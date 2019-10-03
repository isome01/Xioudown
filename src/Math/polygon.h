#pragma once //to make sure that our header file is only included once in our compilation; header is the interface/library

#include "../Essentials/paradigm.h"
#include <SDL.h>
#include <cstddef>
#include <vector>
#include <math.h>
//#include <tuple> - need c++ 11 compiler or higher

#include <iostream>

#define log(x) std::cout << x << std::endl;

namespace Xioudown{ namespace Math {
    
    class Polygon{
        private:
            short *x, *y; //In order for us to be able to interact with SDL we need to make sure that we define short points
            int vertices, gw_min_x, gw_max_x, gw_min_y, gw_max_y;
        public:

            //returns a coordinate (x or y as specified) of the difference between two other coordinates.
            short returnPoints(short a, short b, int thickness){

                if (a < b) return a - thickness;

                else if (a > b) return a + thickness;

                return b;
            }


            short invreturnPoints(short a, short b, int thickness){

                if (a < b) return a + thickness;

                else if (a > b) return a - thickness;

                return b;
            }


            //Exclusively for line generation
            short thicken_line(const short a, const short b, int thickness){
                return ( a <= b )?(a-thickness):(a+thickness);
            }

            short r_thicken_line(const short b, const short a, int thickness){

                return ( b >= a)?(b + thickness):(b-thickness);
            }


             //or simply render a polygon from a series of lines.
            Polygon(const short *_x, const short *_y, int p) : vertices(p) {

                //plain and simple.
                x = new short[vertices];
                y = new short[vertices];
                for (int i = 0; i < vertices; i++){
                    x[i] = _x[i];
                    y[i] = _y[i];
                }

            }

            //Simply create a polygon-rect from 2 unique coordinates with default thickness
            Polygon(const short *_x, const short *_y){

                std::vector<short> xs, ys; //Declare our x and y vectors to store coordinates
                //determining xs

                //forwards
                xs.push_back( thicken_line( _x[0], _x[1], 12) );
                xs.push_back( thicken_line( _x[1], _x[0], 12) );
                ys.push_back( thicken_line( _y[0], _y[1], 12) );
                ys.push_back( thicken_line( _y[1], _y[0], 12) );


                double run = xs[1] - _x[1];
                double rise = ys[1] - _y[1];
                //We don't have to worry about adding thickness because the rise and run takes care of that
                xs.push_back( _x[1] + ( _x[1] >= xs[1] ? rise : (rise * -1) ) );
                ys.push_back( _y[1] + ( _y[1] >= ys[1] ? run : (run * -1) ) );
                //backwards
                xs.push_back( r_thicken_line( _x[0], _x[1], 12));
                ys.push_back( r_thicken_line( _y[0], _y[1], 12));

                x = new short[4];
                y = new short[4];

                vertices = 4;
                for (int i = 0; i < 4; i++){
                    x[i] = xs[i];
                    y[i] = ys[i];
                }

                resolveCenter();
            }

            //Create an open-non-overlapping polygon from coordinates and thickness
            Polygon(const short *_x, const short *_y, int p, int t) : vertices(p) {

                //When we decide to upgrade our compiler.
                //vector< std::tuple<short, short> > xy; //Create a vector of x and y coordinates

                std::vector<short> xs, ys; //Declare our x and y vectors to store coordinates

                int thickness = t; //Default thickness to 25

                //Could be called recursively for efficiency but... We kinda want this to just work for now.
                for (int j = vertices - 1, i = 0; i < vertices; j = i++){ //iterate forward

                    if( i == 0 ){ //set the first point based on line differences.
                        short back_x = returnPoints(_x[i], _x[j], thickness);
                        short frontx = returnPoints(_x[i], _x[i+1], thickness);
                        xs.push_back( _x[i] != back_x ? back_x : frontx);

                        short back_y = returnPoints(_y[i], _y[j], thickness);
                        short fronty = returnPoints(_y[i], _y[i+1], thickness);
                        ys.push_back( _y[i] != back_y ? back_y : fronty);

                        if (vertices == 2 && i == 0){
                            xs.push_back(thicken_line(_x[0], _x[1], thickness ));
                            ys.push_back(thicken_line(_y[0], _y[1], thickness ));
                        }
                    }

                    else { //set points based on previous points

                        short thick_x = returnPoints( _x[i], _x[j], thickness), thick_y = returnPoints( _y[i], _y[j], thickness);
                        xs.push_back( thick_x == _x[i] ? xs[j] : thick_x );
                        ys.push_back( thick_y == _y[i] ? ys[j] : thick_y );
                    }

                }

                //Now iterate backwards
                int backwards = vertices;

                for (int i = vertices - 1, j = 0; i >= 0; j = i--){

                    //This is where we create the pivot point for an open polygon: which comes from the last point.
                    if (i == vertices - 1){
                        //Since the rise from pivot to the last points in the vector could be negative
                        //then we say "double" instead of "short"
                        double run = xs[i] - _x[i];
                        double rise = ys[i] - _y[i];

                        //We don't have to worry about adding thickness because the rise and run takes care of that
                        xs.push_back( _x[i] + ( _x[i] >= xs[i] ? rise : (rise * -1) ) );
                        ys.push_back( _y[i] + ( _y[i] >= ys[i] ? run : (run * -1) ) );


                    } /*else if( i == 0 ){ //setting the last point of iteration

                        xs.push_back( returnPoints() );
                        ys.push_back(  );

                    }*/ else { //from this point we will iterate with inverse thickness operations

                        short thick_x = invreturnPoints( _x[i], _x[j], thickness);
                        short thick_y = invreturnPoints( _y[i], _y[j], thickness);

                        xs.push_back( thick_x == _x[i] ? xs[backwards] : thick_x );
                        ys.push_back( thick_y == _y[i] ? ys[backwards] : thick_y );
                        backwards++;
                    }
                }

                //Then allocate our polygon coordinates accordingly.
                vertices *= 2;
                x = new short[vertices];
                y = new short[vertices];

                for (int i = 0; i < vertices; i++){
                    x[i] = xs[i];
                    y[i] = ys[i];
                }
                resolveCenter();
            }

            //This function builds a closed-overlapping polygon-rect.
            Polygon(const SDL_Rect r) : vertices(10){
                x = new short[vertices];
                y = new short[vertices];
                std::vector<short> xs;
                std::vector<short> ys;

                //obnoxiously enough, we MUST create the points manually.
                xs.push_back(r.x - 25);
                xs.push_back(r.x - 25);
                xs.push_back(r.x + r.w + 25);
                xs.push_back(r.x + r.w + 25);
                xs.push_back(r.x - 25);
                xs.push_back(r.x - 25);
                xs.push_back(r.x + r.w - 25);
                xs.push_back(r.x + r.w - 25);
                xs.push_back(r.x + 25);
                xs.push_back(r.x + 25);

                ys.push_back(r.y + r.h + 25);
                ys.push_back(r.y - 25);
                ys.push_back(r.y - 25);
                ys.push_back(r.y + r.h + 25);
                ys.push_back(r.y + r.h + 25);
                ys.push_back(r.y + r.h - 25);
                ys.push_back(r.y + r.h - 25);
                ys.push_back(r.y + 25);
                ys.push_back(r.y + 25);
                ys.push_back(r.y + r.h);

                for (int i = 0; i < vertices; i++){
                    x[i] = xs[i];
                    y[i] = ys[i];
                }

                resolveCenter();
            }

            ~Polygon() { delete [] x; delete [] y; }
            bool inside(int vert_x, int vert_y); //Check to see if a particular point is inside the polygon with integer values.
            bool inside(short vx, short vy); //Check to see if a particular point is inside the polygon.
            short* X() const{ return x; }
            void X(const double n) { for (int i = 0; i < vertices; i++) x[i]+= n; }
            short* Y() const{ return y; }
            void Y(const double n) { for (int i = 0; i < vertices; i++) y[i]+= n; }
            int getVertices()const{ return vertices; }
            void resolveCenter(){
                gw_min_x = x[0];
                gw_min_y = y[0];
                gw_max_x = x[0];
                gw_max_y = y[0];

                for (int i = 1; i < vertices; i++){
                    if (x[i] < gw_min_x) gw_min_x = x[i];
                    if (x[i] > gw_max_x) gw_max_x = x[i];
                    if (y[i] < gw_min_y) gw_min_y = y[i];
                    if (y[i] > gw_max_y) gw_max_y = y[i];
                }
            }
            int getCenterX() const{ return gw_max_x - gw_min_x; }
            int getCenterY() const{ return gw_max_y - gw_min_y; }
    };
};};
