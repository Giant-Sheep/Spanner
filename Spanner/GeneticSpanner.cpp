//
//  GeneticSpanner.cpp
//  Spanner
//
//  Created by Matias Piispanen on 9/9/12.
//  Copyright (c) 2012 Aalto Yliopisto. All rights reserved.
//

#include "GeneticSpanner.h"
#include "RandomSpanner.h"
#include <cstdlib>
#include <ctime>

GeneticSpanner::GeneticSpanner(vector<Point *> points, double t, size_t generation_size, size_t mating_pool_size, double x, double y) : Spanner(points, t), generation_size(generation_size) {
	population_points = vector<vector<Point *> >();
	population = vector<Spanner *>();
	map<Spanner *, float> fitnesses;
	vector<pair<Spanner *, Spanner *> > parents;
	multimap<Spanner *, string> parent_strings;
	float sum = 0.f;
	mating_pool = vector<Spanner *>();
	
	for(int i = 0; i < generation_size; i++) {
		population_points.push_back(vector<Point *>());
		for(vector<Point *>::iterator it = points.begin(); it != points.end(); it++) {
			population_points.back().push_back(new Point((*it)->getX(), (*it)->getY()));
		}
		
		//TODO: Magic probability
		population.push_back(new RandomSpanner(population_points.back(), t, 0.25));
	}
	
	// Test GA
	for (int i = 0; i < 10; i++) {
		fitnesses = computeFitnesses(population);
		sum = sumOfFitnesses(fitnesses);
		mating_pool = generateMatingPool(fitnesses, sum, mating_pool_size);
		parents = pairParents(mating_pool);
		parent_strings = generateStringRepresentation(parents);
		for (multimap<Spanner *, string>::iterator it = parent_strings.begin(); it != parent_strings.end(); it++) {
			pair<Spanner*, string> mom = pair<Spanner *, string>((*it).first, (*it).second);
			it++;
			pair<Spanner*, string> dad = pair<Spanner *, string>((*it).first, (*it).second);
			GeneticSpanner::crossover(mom.second, dad.second);
			GeneticSpanner::mutation(0.1, mom.second);
			GeneticSpanner::mutation(0.1, dad.second);
			
			cout << "Before size: " << mom.first->getEdges().size() << endl;
			mom.first->removeEdges();
			dad.first->removeEdges();
			
			mom.first->buildSpanner(mom.second);
			dad.first->buildSpanner(dad.second);
			
			cout << "After size: " << mom.first->getEdges().size() << endl;
			
			double min = INT_MAX;
			for (vector<Spanner *>::iterator it = population.begin(); it != population.end(); it++) {
				double dil = (*it)->getMaxDilation();
				if(dil < min) {
					min = dil;
				}
			}
			
			cout << "Dilation: " << min << endl;
		}
	}
	double min = INT_MAX;
	for (vector<Spanner *>::iterator it = population.begin(); it != population.end(); it++) {
		double dil = (*it)->getMaxDilation();
		if(dil < min) {
			min = dil;
		}
	}
	cout << "Final Dilation: " << min << endl;
}

map<Spanner *, float> GeneticSpanner::computeFitnesses(vector<Spanner *> spanners) {
	map<Spanner *, float> fitnesses = map<Spanner *, float>();
	
	for(vector<Spanner *>::iterator it = spanners.begin(); it != spanners.end(); it++) {
		float fitness = 0.f;
		float offset = 0.f;
		bool feasible = true;
		
		vector<Point *> spanner_points = (*it)->getPoints();
		
		for (vector<Point *>::iterator first_point = spanner_points.begin(); first_point != spanner_points.end(); first_point++) {
			for (vector<Point *>::iterator second_point = first_point + 1; second_point != spanner_points.end(); second_point++) {
				float dilation = (*it)->getDilation((*first_point), (*second_point));
				
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
		
		fitness += (*it)->getEdges().size();
		
		fitnesses[(*it)] = fitness;		
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
	map<Spanner *, float>::iterator it;
	
	srand ( (unsigned int)time(NULL) );
	
	for (int i = 0; i < size; i++) {
		float lottery = (float)rand() / ((float)RAND_MAX/sum);
		float limit;
		
		it = fitnesses.begin();
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
		lottery = rand() % mating_pool.size();
		Spanner *first_spanner = mating_pool.at(lottery);
		mating_pool.erase(mating_pool.begin() + lottery);
		
		lottery = rand() % mating_pool.size();
		Spanner *second_spanner = mating_pool.at(lottery);
		mating_pool.erase(mating_pool.begin() + lottery);
		pairs.push_back(pair<Spanner *, Spanner *>(first_spanner, second_spanner));
	}
	// Odd parent in mating_pool gets discarded
	
	return pairs;
}

multimap<Spanner *, string> GeneticSpanner::generateStringRepresentation(vector<pair<Spanner *, Spanner *> > pairs) {
	multimap<Spanner *, string> strings = multimap<Spanner *, string>();
	string string_representation = "";
	vector<Point *> point_list;
	
	for (vector<pair<Spanner *, Spanner *> >::iterator it = pairs.begin(); it != pairs.end(); it++) {
		point_list = (*it).first->getPoints();
		string_representation = "";
		for (vector<Point *>::const_iterator first_point = point_list.begin(); first_point != point_list.end(); first_point++) {
			for (vector<Point *>::const_iterator second_point = first_point + 1; second_point != point_list.end(); second_point++) {
				if((*it).first->hasEdge(*first_point, *second_point)) {
					string_representation += "1";
				}
				else {
					string_representation += "0";
				}
			}
		}
		
		strings.insert(pair<Spanner *, string>((*it).first, string_representation));
		
		point_list = (*it).second->getPoints();
		string_representation = "";
		for (vector<Point *>::const_iterator first_point = point_list.begin(); first_point != point_list.end(); first_point++) {
			for (vector<Point *>::const_iterator second_point = first_point + 1; second_point != point_list.end(); second_point++) {
				if((*it).second->hasEdge(*first_point, *second_point)) {
					string_representation += "1";
				}
				else {
					string_representation += "0";
				}
			}
		}
		
		strings.insert(pair<Spanner *, string>((*it).second, string_representation));
	}
	
	
	return strings;
}

void initial_string_generator(double prob_for_1, string &s) {
	for (int i=0; i<s.length(); i++) {
		int r = rand() % 10000;
		double r2=r/10000.0;

		if (r2 < prob_for_1) {
			s[i] = '1';
		}
		else {
			s[i] = '0';
		}
	}
}


 bool GeneticSpanner::mutation(double probability, string &s) {

	int r = rand() % 10000;
	double r2=r/10000.0;

	if (r2 < probability) {

		int mutation_point = (int)(rand() % s.length());

		if (s[mutation_point] == '1') {
			s[mutation_point] = '0';
		}
		else {
			s[mutation_point] = '1';
		}

		//if(DEBUG) {cout << "m: " << s << " prob: " << r2 << " point: " << mutation_point << endl;}

		return true;
	}
	return false;
}


void GeneticSpanner::crossover(string &a, string &b) {

	int crossover_point = (int)(rand() % (a.length()-1)) + 1; // crossover at 0 would change strings completly, so force it to min 1.

	//if(DEBUG) {cout << "crossover_point: " << crossover_point << endl;}

	string a2 (a, 0, crossover_point);
	a2.append(b, crossover_point, b.length()-crossover_point);
	string b2 (b, 0, crossover_point);
	b2.append(a, crossover_point, a.length()-crossover_point);

	a=a2;
	b=b2;
}
