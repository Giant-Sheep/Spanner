//
//  RandomSpanner.cpp
//  Spanner
//
//  Created by Matias Piispanen on 9/12/12.
//  Copyright (c) 2012 Aalto Yliopisto. All rights reserved.
//

#include "RandomSpanner.h"

#include <cstdlib>
#include <ctime>

RandomSpanner::RandomSpanner(vector<Point *> points, double t, double probability) : Spanner(points, t) {
	
	for (vector<Point *>::iterator first_point = points.begin(); first_point != points.end(); first_point++) {
		for (vector<Point *>::iterator second_point = first_point + 1; second_point != points.end(); second_point++) {
			float lottery = (float)rand() / (float)RAND_MAX;
			
			if (lottery <= probability) {
				this->addEdge(new Edge(*first_point, *second_point));
			}
		}
	}
}
