//
//  Spanner.h
//  Spanner
//
//  Created by Matias Piispanen on 4/4/11.
//  Copyright 2011 TU/e. All rights reserved.
//
#pragma once

#include <vector>
#include <utility>
#include <cmath>
#include "Point.h"
#include "Edge.h"
#include <climits>
#include <algorithm>

using namespace std;

typedef pair<int, int> remove_Point;
typedef pair<Point, Point> remove_Edge;

class Spanner {
public:
    Spanner(vector<Point *> points, double t);
    
    double getDilation();
    double getMaxDilation();
    double getDilation(Point * first, Point * second);
    size_t getSize();
    double getWeight();
    size_t getMaxDegree();
    vector<Point *> getPoints();
    vector<Edge *> getEdges();
	void removeEdges();
    void addEdge(Edge* edge);
	bool hasEdge(Point * first, Point * second);
	virtual vector<vector<double> > getAdjacency();
	virtual int getDepth();
	void buildSpanner(string string);
    
protected:
    vector<Point *> points;
    vector<Edge *> edges;
    double t;
    
private:
    double getDilation(bool maximum, Point * first, Point * second = NULL);
};
