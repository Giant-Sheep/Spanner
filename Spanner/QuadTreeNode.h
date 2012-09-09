//
//  QuadTreeNode.h
//  Spanner
//
//  Created by Matias Piispanen on 5/19/11.
//  Copyright 2011 Aalto Yliopisto. All rights reserved.
//

#pragma once

#include <vector>
#include "Point.h"
#include <cmath>
#include <ctime>
#include <climits>

class QuadTreeNode {
public:
    QuadTreeNode(std::vector<Point *> points, double x, double y, double width, double height);
    std::vector<Point *> getPoints();
    double getWidth();
    double getHeight();
    double getRadius();
	double getNodeRadius();
    Point* getRandomRepresentative();
	Point* getRepresentative(std::pair<double, double> pos);
    size_t size();
    pair<double, double> getPosition();
    std::vector<QuadTreeNode> split();
    
private:
    std::vector<Point *> points;
    double width, height;
    double radius;
    pair<double, double> position;
};