//
//  WSPDSpanner.h
//  Spanner
//
//  Created by Matias Piispanen on 5/20/11.
//  Copyright 2011 Aalto Yliopisto. All rights reserved.
//

#pragma once

#include <vector>
#include <cmath>
#include "Spanner.h"
#include "Point.h"
#include "QuadTreeNode.h"
#include <climits>

class WSPDSpanner : public Spanner {
public:
    WSPDSpanner(std::vector<Point *> points, double t, double x, double y);
    void connectPairs(QuadTreeNode first, QuadTreeNode second, double t);
	int getDepth();
    
private:
    double s;
	double smallest;
	double width;
};
