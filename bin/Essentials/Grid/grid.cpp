#include "grid.h"
#include "paradigm.h"

namespace Xioudown { namespace Essentials {
  int Grid::m_grid_count = 0;
  int GridUnit::m_grid_unit_count = 0;

  /*
  * ~~~~~~~~~~~~~~~~~~~~~~~
  * GridUnit implementation
  * ~~~~~~~~~~~~~~~~~~~~~~~ 
  */
  GridUnit::GridUnit() {
    m_grid_unit_id = GRID_UNIT_NAME_DEFAULT + (char)m_grid_unit_count++;
  }
  GridUnit::~GridUnit() {}

  void GridUnit::render(SDL_Renderer *r) {

  }

  int GridUnit::x () const { return 0; }
  void GridUnit::x (int n) { }
  
  /*
  * ~~~~~~~~~~~~~~~~~~~~
  *  Grid implementation
  * ~~~~~~~~~~~~~~~~~~~~
  * */
  Grid::Grid() {
    m_grid_id = GRID_NAME_DEFAULT + (char)(m_grid_count++);
  }

  Grid::~Grid() {
    delete m_grid_line_model;

    // erase all items in grid
    std::map<std::string, GridUnit*>::iterator it = m_grid_units.begin();
    m_grid_units.erase(it, m_grid_units.end());
  }

  /* Render grid and grid items accordingly. */
  void Grid::render(SDL_Renderer *r) {

  }

  int Grid::gridSteps() const{}
  void Grid::gridSteps(int n) {}
};};