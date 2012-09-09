//
//  Edge.cpp
//  Spanner
//
//  Created by Matias Piispanen on 4/8/11.
//  Copyright 2011 TU/e. All rights reserved.
//

#include <cmath>
#include "Edge.h"


Edge::Edge(Point * first, Point * second) : first(first), second(second) {
    
}

Point * Edge::getFirst() {
    return first;
}

Point * Edge::getSecond() {
    return second;
}

double Edge::getLength() {
	return sqrt(pow((first->getX() - second->getX()), 2.0) + pow(first->getY() - second->getY(), 2.0));
}

void Edge::draw() {
    glBegin( GL_LINES );
    glColor3f(0.6, 0.7, 0.8);
    glVertex2f( this->getFirst()->getX(), this->getFirst()->getY() );
    glColor3f(0.6, 0.7, 0.8);
    glVertex2f( this->getSecond()->getX(), this->getSecond()->getY() );
    glEnd();
}