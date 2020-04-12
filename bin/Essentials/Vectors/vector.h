#pragma once

namespace Xioudown { namespace Essentials {

  /* class-ic way of keeping track of x/y coordinates */
  class XYVector {
    public:
      XYVector(int x, int y);
      XYVector(int n);
      XYVector(XYVector *obj);
      void x(int x);
      void y(int y);
      int x() const;
      int y() const;
      //When added by another vector, returns a copy of a new vector
      XYVector* operator + (XYVector *obj);
      XYVector* operator + (int n);
      void operator += (XYVector *obj);
      void operator += (int n);
      XYVector* operator - (XYVector *obj);
      XYVector* operator - (int n);
      void operator -= (XYVector *obj);
      void operator -= (int n);
      void operator = (XYVector *obj);
      void operator = (int n);

    private:
      int m_x, m_y;
  };
}};