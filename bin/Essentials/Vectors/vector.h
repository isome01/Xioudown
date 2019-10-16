#pragma once

namespace Xioudown { namespace Essentials {

  /* class-ic way of keeping track of x/y coordinates */
  class XYVetor {
    public:
      XYVector(int x = 0, int y = 0);
      void x(int x);
      void y(int y);
      int x() const;
      int y() const;
      //When added by another vector, returns a copy of a new vector
      XYVetor operator + (XYVetor &obj) const;

    private:
      int m_x, m_y;

  };
}};