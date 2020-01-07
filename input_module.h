#ifndef INPUT_MODULE_H
#define INPUT_MODULE_H

#include <iostream>
#include <vector>
#include <string>
#include "shape.h"
using namespace std;

void input(int &recDepth, int &pixels, vector<Shape *> &shapes, vector<string> &shapeTypes,
           vector<Point> &lights);


#endif // INPUT_MODULE_H
