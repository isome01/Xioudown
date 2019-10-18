#pragma once

namespace Xioudown { namespace Essentials {

  /* class-ic way of keeping track of x/y coordinates */
  class XYVector {
    public:
      XYVector(int x, int y);
      void x(int x);
      void y(int y);
      int x() const;
      int y() const;
      //When added by another vector, returns a copy of a new vector
      XYVector operator + (XYVector *obj);

    private:
      int m_x, m_y;

  };
}};