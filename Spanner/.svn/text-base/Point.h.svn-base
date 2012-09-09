//
//  Point.h
//  Spanner
//
//  Created by Matias Piispanen on 4/8/11.
//  Copyright 2011 TU/e. All rights reserved.
//
#pragma once

using namespace std;

#include "Edge.h"
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class Edge;

class Point {
public:
    Point(int x, int y);
    int getX() const;
    int getY() const;
    void draw();
    void addEdge(Edge* edge);
    vector<Edge *> getEdges();
    
private:
    int x, y;
    std::vector<Edge *> edges;
    
};