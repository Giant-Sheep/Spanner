//
//  Edge.h
//  Spanner
//
//  Created by Matias Piispanen on 4/8/11.
//  Copyright 2011 TU/e. All rights reserved.
//
#pragma once

#include "Point.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class Point;

class Edge {
public:
    Edge(Point * first, Point* second);
    Point * getFirst();
    Point * getSecond();
	double getLength();
    void draw();
    
private:
    Point *first,  *second;
};