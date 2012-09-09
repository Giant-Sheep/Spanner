//
//  GeneticSpanner.cpp
//  Spanner
//
//  Created by Matias Piispanen on 9/9/12.
//  Copyright (c) 2012 Aalto Yliopisto. All rights reserved.
//

#include "GeneticSpanner.h"

GeneticSpanner::GeneticSpanner(vector<Point *> points, double t, size_t generation_size, double x, double y) : Spanner(points, t) {
	population_points = vector<vector<Point *> >();
	population = vector<Spanner>();
	
	for(int i = 0; i < generation_size; i++) {
		population_points.push_back(vector<Point *>());
		for(vector<Point *>::iterator it = points.begin(); it != points.end(); it++) {
			population_points.back().push_back(new Point((*it)->getX(), (*it)->getY()));
		}
		
		population.push_back(WSPDSpanner(population_points.back(), t, x, y));
	}
	
	computeFitnesses(population);
}

map<Spanner *, float> GeneticSpanner::computeFitnesses(vector<Spanner> spanners) {
	map<Spanner *, float> fitnesses = map<Spanner *, float>();
	//TODO: Magic is magic.
	float MAGIC_NUMBER = 10.0;
	
	for(vector<Spanner>::iterator it = spanners.begin(); it != spanners.end(); it++) {
		float fitness = 0.f;
		float offset = 0.f;
		bool feasible = true;
		
		float dilation = (*it).getDilation();
		
		//TODO: offset should probably be based on amount of edges
		offset += dilation;
		if(dilation > t) {
			feasible = false;
		}
		else {
			fitness += dilation;
		}
		
		if(!feasible) {
			fitness += offset;
		}
		
		fitness += (*it).getEdges().size() * MAGIC_NUMBER;
		
		fitnesses[&(*it)] = fitness;
		
		cout << "fitness: " << fitness << endl;
	}
	
	return fitnesses;
}
