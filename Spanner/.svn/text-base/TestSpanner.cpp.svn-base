//
//  TestSpanner.cpp
//  Spanner
//
//  Created by Matias Piispanen on 4/8/11.
//  Copyright 2011 TU/e. All rights reserved.
//

#include "TestSpanner.h"

std::vector<std::vector<double> > adjacency;

TestSpanner::TestSpanner(vector<Point *> points, double t) : Spanner(points, t) {
	Edge * e;
	//Add 0's and -1's in adjacency matrix
	std::vector<double> aux(points.size(), -1);
	for(int i=0; i<points.size(); i++) {
		aux[i]=0;
		adjacency.push_back(aux);
		aux[i]=-1;
	}

    for (int i = 0; i <points.size(); i++) {
        for (int j = i; j < points.size(); j++) {
            if(i != j) {
				e=new Edge(points[i], points[j]);
                this->addEdge(e);
				adjacency[i][j]=e->getLength();
				adjacency[j][i]=e->getLength();
            }
        }
    }
}

vector<vector<double> > TestSpanner::getAdjacency() {
	return adjacency;
}