//
//  GeneticSpanner.cpp
//  Spanner
//
//  Created by Matias Piispanen on 9/9/12.
//  Copyright (c) 2012 Aalto Yliopisto. All rights reserved.
//

#include "GeneticSpanner.h"
#include <cstdlib>
#include <ctime>

GeneticSpanner::GeneticSpanner(vector<Point *> points, double t, size_t generation_size, size_t mating_pool_size, double x, double y) : Spanner(points, t) {
	population_points = vector<vector<Point *> >();
	population = vector<Spanner>();
	map<Spanner *, float> fitnesses;
	vector<pair<Spanner *, Spanner *> > parents;
	float sum = 0.f;
	mating_pool = vector<Spanner *>();
	
	for(int i = 0; i < generation_size; i++) {
		population_points.push_back(vector<Point *>());
		for(vector<Point *>::iterator it = points.begin(); it != points.end(); it++) {
			population_points.back().push_back(new Point((*it)->getX(), (*it)->getY()));
		}
		
		population.push_back(WSPDSpanner(population_points.back(), t, x, y));
	}
	
	fitnesses = computeFitnesses(population);
	sum = sumOfFitnesses(fitnesses);
	mating_pool = generateMatingPool(fitnesses, sum, mating_pool_size);
	parents = pairParents(mating_pool);
	cout << parents.size();
}

map<Spanner *, float> GeneticSpanner::computeFitnesses(vector<Spanner> spanners) {
	map<Spanner *, float> fitnesses = map<Spanner *, float>();
	
	for(vector<Spanner>::iterator it = spanners.begin(); it != spanners.end(); it++) {
		float fitness = 0.f;
		float offset = 0.f;
		bool feasible = true;
		
		vector<Point *> spanner_points = (*it).getPoints();
		
		for (vector<Point *>::iterator first_point = spanner_points.begin(); first_point != spanner_points.end(); first_point++) {
			for (vector<Point *>::iterator second_point = first_point + 1; second_point != spanner_points.end(); second_point++) {
				float dilation = (*it).getDilation((*first_point), (*second_point));
				
				offset += dilation;
				if(dilation > t) {
					feasible = false;
				}
				else {
					fitness += dilation;
				}
			}
		}
		
		if(!feasible) {
			fitness += offset;
		}
		
		fitness += (*it).getEdges().size();
		
		fitnesses[&(*it)] = fitness;		
	}
	
	return fitnesses;
}

float GeneticSpanner::sumOfFitnesses(map<Spanner *, float> fitnesses) {
	float sum = 0.f;
	
	for(map<Spanner *, float>::iterator it = fitnesses.begin(); it != fitnesses.end(); it++) {
		sum += (*it).second;
	}
	
	return sum;
}

vector<Spanner *> GeneticSpanner::generateMatingPool(map<Spanner *, float> fitnesses, float sum, size_t size) {
	vector<Spanner *> mating_pool = vector<Spanner *>();
	
	srand ( (unsigned int)time(NULL) );
	
	for (int i = 0; i < size; i++) {
		float lottery = (float)rand() / ((float)RAND_MAX/sum);
		float limit;
		
		map<Spanner *, float>::iterator it = fitnesses.begin();
		limit = (*it).second;
		
		for (int j = 0; limit <= sum && j < fitnesses.size(); j++) {
			if (lottery > limit) {
				it++;
				limit += (*it).second;
			}
			else {
				mating_pool.push_back((*it).first);
				break;
			}
		}
	}
	
	return mating_pool;
}

vector<pair<Spanner *, Spanner *> > GeneticSpanner::pairParents(vector<Spanner *> mating_pool) {
	vector<pair<Spanner *, Spanner *> > pairs;
	int lottery = 0;
	while (mating_pool.size() >= 2) {
		lottery = mating_pool.size() * rand() / (RAND_MAX);
		Spanner *temp_spanner = mating_pool.at(lottery);
		mating_pool.erase(mating_pool.begin() + lottery);
		
		lottery = mating_pool.size() * rand() / (RAND_MAX);
		pairs.push_back(pair<Spanner *, Spanner *>(temp_spanner, mating_pool.at(lottery)));
		mating_pool.erase(mating_pool.begin() + lottery);
	}
	// Odd parent in mating_pool gets discarded
	
	return pairs;
}
