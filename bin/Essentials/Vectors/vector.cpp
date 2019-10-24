#include "vector.h"

namespace Xioudown { namespace Essentials {
  XYVector::XYVector (int x, int y) : m_x(x), m_y(y) {}
  XYVector::XYVector (int n) : m_x(n), m_y(n) {}
  XYVector::XYVector (XYVector *obj) : m_x(obj->x()), m_y(obj->y()) {}

  XYVector* XYVector::operator+(XYVector *obj) {
    return new XYVector(
      this->m_x + obj->x(),
      this->m_y + obj->y()
    );
  }

  XYVector* XYVector::operator+(int n) {
    return new XYVector(
      this->m_x + n,
      this->m_y + n
    );
  }

  void XYVector::operator+=(XYVector *obj) {
    //just set x and why of initiating object
    this->x(this->x() + obj->x());
    this->y(this->y() + obj->y());
  }

  void XYVector::operator+=(int n) {
    //just set x and why of initiating object
    this->x(this->x() - n);
    this->y(this->y() - n);
  }

  XYVector* XYVector::operator-(XYVector *obj) {
    return new XYVector(
      this->m_x - obj->x(),
      this->m_y - obj->y()
    );
  }

  void XYVector::operator-=(XYVector *obj) {
    //just set x and why of initiating object
    this->x(m_x - obj->x());
    this->y(m_y - obj->y());
  }

  void XYVector::operator=(XYVector *obj) {
    this->x(obj->x());
    this->y(obj->y());
  }

  int XYVector::x() const{ return m_x; }
  int XYVector::y() const{ return m_y; }
  void XYVector::x(int x){ this->m_x = x; }
  void XYVector::y(int y){ this->m_y = y; }
}};
