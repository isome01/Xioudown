#include "vector.h"

namespace Xioudown { namespace Essentials {
  XYVector::XYVector (int x = 0, int y = 0) : m_x(x), m_y(y) {}

  XYVector XYVector::operator+(XYVector *obj) {
    XYVector *vector = new XYVector(
      this->m_x + obj->x(),
      this->m_y + obj->y()
    ); 
    return *vector;
  }

  int XYVector::x() const{ return m_x; }
  int XYVector::y() const{ return m_y; }
  void XYVector::x(int x){ this->m_x = x; }
  void XYVector::y(int y){ this->m_y = y; }
}};
