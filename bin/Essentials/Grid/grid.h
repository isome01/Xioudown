#pragma once
#include <stdlib.h>
#include <map>
#include "../paradigm.h"

namespace Xioudown { namespace Essentials {

  #ifndef GRID_NAME_DEFAULT
  #define GRID_NAME_DEFAULT "GRID_NAME_DEFAULT"
  #define GRID_UNIT_NAME_DEFAULT "GRID_UNIT_NAME_DEFAULT"
  #endif
  /* This is anything that can be tracked on the grid */
  class GridUnit {
    private:
      static int m_grid_unit_count; //running count id for any unit on a grid
      std::string m_grid_unit_id; //unique id for a grid unit
      RenderColorA unitRGBA; //separate unit RGBA values
      int *m_x, *m_y;

    public:
      GridUnit();
      ~GridUnit();
      void itemId(std::string);
      std::string itemId() const;
      void render(SDL_Renderer *r); // render function for individual grid units

      //Robust getters and setters for our SDL_Rect/boundaries and alpha
      virtual void x(int n);
      virtual int x()const;
      virtual void y(int n);
      virtual int y()const;
      virtual int getCenter();

      virtual void h(int n) const {}
      virtual int h()const{ return 0; }
      virtual void w(int n) const {}
      virtual int w()const{ return 0; }
  };

  /* This class is the foundation/base class for all levels */
  class Grid {
    private:
      std::map<std::string, GridUnit*> m_grid_units;
      static int m_grid_count; //running count for instantiated grids
      std::string m_grid_id; //Id tracker for all grids instantiated
      SDL_Rect *m_grid_line_model; //used to display for an array of units in a grid
      
    public:
      Grid();
      ~Grid();
      void render(SDL_Renderer *r); //renders all grid units

      //setter and getter for the visible pixel grids
      void gridSteps(int n);
      int gridSteps() const;
  };
}};