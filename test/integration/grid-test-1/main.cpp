#include "../../../include/Xioudown/Xioudown.h"
#include <stdlib.h>
#include <iostream>
#define EXIT_SUCCESS 0
#define STANDBY system("pause")

Inst_Catalyst(NULL);
using namespace Essentials;
using std::cout;
using std::endl;

int main (int argc, char *argv[]) {

  XYVector *v = new XYVector(12, -3);
  XYVector *w = new XYVector(5, 4);

  cout << "Vector v: (" << v->x() << ", " << v->y() << ")" << endl;
  cout << "Vector w: (" << w->x() << ", " << w->y() << ")" << endl;

  *v = *v + w;

  cout << "Vector v after sum: (" << v->x() << ", " << v->y() << ")" << endl;

  STANDBY;
  return EXIT_SUCCESS;
}