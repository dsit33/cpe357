#include "part6.h"
#include <math.h>

Point create_point(double x, double y)
{
   Point p = {x, y};
   return p;
}

int isSquare(Rectangle r)
{
   return pow(r.topLeft.x - r.botRight.x, 2.0) == 
      pow(r.topLeft.y - r.botRight.y, 2.0);
}
