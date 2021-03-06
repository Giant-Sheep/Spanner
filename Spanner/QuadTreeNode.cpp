//
//  QuadTreeNode.cpp
//  Spanner
//
//  Created by Matias Piispanen on 5/19/11.
//  Copyright 2011 Aalto Yliopisto. All rights reserved.
//

#include "QuadTreeNode.h"
#include <iostream>

QuadTreeNode::QuadTreeNode(std::vector<Point *> points, double x, double y, double width, double height) : points(points), position(std::make_pair(x, y)), width(width), height(height), radius(sqrt(pow(width, 2.0) + pow(height, 2.0))/2) {
    
}

std::vector<Point *> QuadTreeNode::getPoints() {
    return points;
}

double QuadTreeNode::getWidth() {
    return width;
}

double QuadTreeNode::getHeight() {
    return height;
}

double QuadTreeNode::getRadius() {
    double rad = 0.0;
	
	for (std::vector<Point *>::iterator i = points.begin(); i != points.end(); i++) {
		double dist = sqrt(pow(position.first - (*i)->getX(), 2.0) + sqrt(pow(position.second - (*i)->getY(), 2.0)));
		if(dist > rad) {
			rad = dist;
		}
	}
	
	return rad;
}

double QuadTreeNode::getNodeRadius() {
	return radius;
}

size_t QuadTreeNode::size() {
    return points.size();
}

Point * QuadTreeNode::getRandomRepresentative() {
	srand ((unsigned int)time(0));
	
    return points.at(rand() % points.size());
}

Point * QuadTreeNode::getRepresentative(std::pair<double, double> pos) {
	double dist = INT_MAX;
	Point * closest;
	
	for(std::vector<Point *>::iterator i = points.begin(); i != points.end(); i++) {
		double len = sqrt(pow(position.first - pos.first, 2.0) + pow(position.second - pos.second, 2.0));
		if(len < dist) {
			dist = len;
			closest = (*i);
		}
	}
	
	return closest;
}

pair<double, double> QuadTreeNode::getPosition() {
    return position;
}

std::vector<QuadTreeNode> QuadTreeNode::split() {
    std::vector<QuadTreeNode> newNodes;
    
    double newWidth = this->getWidth() / 2;
    double newHeight = this->getHeight() / 2;
    
    double widthOffset = this->getWidth() / 4;
    double heightOffset = this->getHeight() / 4;
    
    double x = this->getPosition().first;
    double y = this->getPosition().second;
    
    std::vector<Point *> topLeft;
    std::vector<Point *> topRight;
    std::vector<Point *> bottomLeft;
    std::vector<Point *> bottomRight;
    
    for (std::vector<Point *>::iterator i = points.begin(); i != points.end(); i++) {	
        if((*i)->getY() > y) {
            if((*i)->getX() <= x) {
                topLeft.push_back((*i));
            }
            else {
                topRight.push_back((*i));
            }
        }
        else {
            if((*i)->getX() <= x) {
                bottomLeft.push_back((*i));
            }
            else {
                bottomRight.push_back((*i));
            }
        }
    }
    
    newNodes.push_back(QuadTreeNode(topLeft, x-widthOffset, y+heightOffset, newWidth, newHeight));
    newNodes.push_back(QuadTreeNode(topRight, x+widthOffset, y+heightOffset, newWidth, newHeight));
    newNodes.push_back(QuadTreeNode(bottomLeft, x-widthOffset, y-heightOffset, newWidth, newHeight));
    newNodes.push_back(QuadTreeNode(bottomRight, x+widthOffset, y-heightOffset, newWidth, newHeight));
    
    return newNodes;
}