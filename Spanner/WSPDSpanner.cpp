//
//  WSPDSpanner.cpp
//  Spanner
//
//  Created by Matias Piispanen on 5/20/11.
//  Copyright 2011 Aalto Yliopisto. All rights reserved.
//

#include "WSPDSpanner.h"
#include <iostream>

WSPDSpanner::WSPDSpanner(std::vector<Point *> points, double t, double x, double y) : Spanner(points, t), s(4*((t+1)/(t-1))), smallest(INT_MAX), width(x) {
    QuadTreeNode root(points, x/2, y/2, x, y);
	std::cout << x << " " << y << std::endl;
    this->connectPairs(root, root, t);
}

int WSPDSpanner::getDepth() {
	return (int)log2(width / smallest);
}

void WSPDSpanner::connectPairs(QuadTreeNode first, QuadTreeNode second, double t) {    
    QuadTreeNode fi = first;
    QuadTreeNode se = second;
	
	if(fi.getWidth() < smallest) {
		smallest = fi.getWidth();
	}
	
	if(se.getWidth() < smallest) {
		smallest = se.getWidth();
	}
    
    if(fi.size() == 0 || se.size() == 0) {
        return;
    }
    
    if(fi.size() == 1 && se.size() == 1 && fi.getRandomRepresentative() == se.getRandomRepresentative()) {
        return;
    }

    
    double fx = fi.getPosition().first;
    double fy = fi.getPosition().second;
    double sx = se.getPosition().first;
    double sy = se.getPosition().second;
    
    double distance = (sqrt((pow(fx-sx, 2.0) + pow(fy-sy, 2.0))) - fi.getRadius() - se.getRadius());
    
    if(distance >= (s * fi.getRadius()) && distance >= (s * se.getRadius())) {
        this->addEdge(new Edge(fi.getRandomRepresentative(), se.getRandomRepresentative()));
		return;
    }
    
    if (fi.getRadius() < se.getRadius()) {
        fi = second;
        se = first;
    }
    
    std::vector<QuadTreeNode> nodes = fi.split();
    
    for (std::vector<QuadTreeNode>::iterator i = nodes.begin(); i != nodes.end(); i++) {
		if((*i).size() > 0) {
			this->connectPairs((*i), se, t);
		}
    }
}
