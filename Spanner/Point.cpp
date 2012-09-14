//
//  Point.cpp
//  Spanner
//
//  Created by Matias Piispanen on 4/8/11.
//  Copyright 2011 TU/e. All rights reserved.
//
#include "Point.h"

Point::Point(int x, int y) : x(x), y(y) {
    
}

int Point::getX() const {
    return x;
}

int Point::getY() const {
    return y;
}

void Point::draw() {
    double size = 5.0;
    glColor3f(1.0, 0.0, 0.0); 
	glBegin(GL_QUADS);
	glVertex2f(x-(size/2), y-(size/2));
	glVertex2f(x+(size/2), y-(size/2));
	glVertex2f(x+(size/2), y+(size/2));
	glVertex2f(x-(size/2), y+(size/2));
	glEnd();
}

vector<Edge *> Point::getEdges() {
    return edges;
}

void Point::removeEdges() {
	this->edges.clear();
}

void Point::addEdge(Edge* edge) {
    edges.push_back(edge);
}