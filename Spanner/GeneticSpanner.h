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
	GeneticSpanner(vector<Point *> points, double t, size_t generation_size, size_t mating_pool_size, double x, double y);
	map<Spanner *, float> computeFitnesses(vector<Spanner> spanners);
	float sumOfFitnesses(map<Spanner *, float> fitnesses);
	vector<Spanner *> generateMatingPool(map<Spanner *, float> fitnesses, float sum, size_t size);
	vector<pair<Spanner *, Spanner *> > pairParents(vector<Spanner *> mating_pool);
	map<Spanner *, string> generateStringRepresentation(vector<pair<Spanner *, Spanner *> > pairs); //työn alla, samalla removeEdges Spanneriin
	
private:
	vector<Spanner> population;
	vector<vector<Point *> > population_points;
	vector<Spanner *> mating_pool;
};

#endif /* defined(__Spanner__GeneticSpanner__) */
