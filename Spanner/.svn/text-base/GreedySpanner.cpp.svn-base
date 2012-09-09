//
//  GreedySpanner.cpp
//  Spanner
//
//  Created by Matias Piispanen on 4/4/11.
//  Copyright 2011 TU/e. All rights reserved.
//

#include "GreedySpanner.h"
#include <iostream>

GreedySpanner::GreedySpanner(vector<Point *> points, double t) : Spanner(points, t) {
    for(std::vector<Point *>::iterator i = points.begin(); i != points.end(); i++) {
        for(std::vector<Point *>::iterator j = i+1; j != points.end(); j++) {
            Edge *edge = new Edge(*i, *j);
            if(sortedEdges.size() > 0) {
                for(std::vector<Edge *>::iterator k = sortedEdges.begin(); k != sortedEdges.end(); k++) {
                    if(edge->getLength() < (*k)->getLength()) {
                        sortedEdges.insert(k, edge);
                        break;
                    }
                    
                    if(k == sortedEdges.end() - 1) {
                        sortedEdges.push_back(edge);
                        break;
                    }
                }
            }
            else {
                sortedEdges.push_back(edge);
            }
        }
    }
    
    this->constructSpanner();
}

void GreedySpanner::constructSpanner() {
    for (std::vector<Edge *>::iterator i = sortedEdges.begin(); i != sortedEdges.end(); i++) {
        if(getDilation((*i)->getFirst(), (*i)->getSecond()) > t) {
            addEdge((*i));
        }
    }
    return;
}