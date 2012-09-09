//
//  GeneticSpanner.h
//  Spanner
//
//  Created by Matias Piispanen on 9/9/12.
//  Copyright (c) 2012 Aalto Yliopisto. All rights reserved.
//

#ifndef __Spanner__GeneticSpanner__
#define __Spanner__GeneticSpanner__

#include "Spanner.h"
#include "Point.h"
#include "Edge.h"
#include "WSPDSpanner.h"
#include <map>
#include <iostream>

class GeneticSpanner : public Spanner {
public:
	GeneticSpanner(vector<Point *> points, double t, size_t generation_size, double x, double y);
	map<Spanner *, float> computeFitnesses(vector<Spanner> spanners);
	
private:
	vector<Spanner> population;
	vector<vector<Point *> > population_points;
};

#endif /* defined(__Spanner__GeneticSpanner__) */
