#ifndef PART6_H
#define PART6_H

typedef struct {
   double x;
   double y;
} Point;

typedef struct {
   Point topLeft;
   Point botRight;
} Rectangle;

Point create_point(double x, double y);

int isSquare(Rectangle r);

#endif
