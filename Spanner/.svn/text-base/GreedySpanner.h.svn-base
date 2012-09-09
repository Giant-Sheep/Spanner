//
//  GreedySpanner.h
//  Spanner
//
//  Created by Matias Piispanen on 4/4/11.
//  Copyright 2011 TU/e. All rights reserved.
//
#pragma once

#include "Spanner.h"

using namespace std;

class GreedySpanner : public Spanner {
public:
    GreedySpanner(vector<Point *> points, double t);
    void constructSpanner();
    
private:
    std::vector<Edge *> sortedEdges;
};