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
	int mutation_probability = 0.1;
	
	for(int i = 0; i < generation_size; i++) {
		population_points.push_back(vector<Point *>());
		for(vector<Point *>::iterator it = points.begin(); it != points.end(); it++) {
			population_points.back().push_back(new Point((*it)->getX(), (*it)->getY()));
		}
		
		//TODO: Magic probability
		population.push_back(new RandomSpanner(population_points.back(), t, 0.75));
	}
	
	// Test GA
//	for (int i = 0; i < 10; i++) {
    double min_dil = INT_MAX;
    int iter_limit = 500;

    while (min_dil > t) {
        
        if (iter_limit < 0) { break;}
 //   while (iter_limit > 0) {

		fitnesses = computeFitnesses(population);
		sum = sumOfFitnesses(fitnesses);
        
//        cout << "sum " << sum << endl;
        
		mating_pool = generateMatingPool(fitnesses, sum, mating_pool_size);
		parents = pairParents(mating_pool);
		parent_strings = generateStringRepresentation(parents);

		for (multimap<Spanner *, string>::iterator it = parent_strings.begin(); it != parent_strings.end(); it++) {
			pair<Spanner*, string> mom = pair<Spanner *, string>((*it).first, (*it).second);
			it++;
			pair<Spanner*, string> dad = pair<Spanner *, string>((*it).first, (*it).second);
			GeneticSpanner::crossover(mom.second, dad.second);
			
			if(iter_limit == 0) {
				mutation_probability = 0.05;
			}
			
			GeneticSpanner::mutation(mutation_probability, mom.second);
			GeneticSpanner::mutation(mutation_probability, dad.second);
			fitnesses = this->computeFitnesses(population);
			for (multimap<Spanner*, string>::iterator parent_it = parent_strings.begin(); parent_it != parent_strings.end(); parent_it++) {
				float max_spanner = 0;
				Spanner *found;
				for (map<Spanner *, float>::iterator fit_it = fitnesses.begin(); fit_it != fitnesses.end(); fit_it++) {
					if((*fit_it).second > max_spanner) {
						max_spanner = (*fit_it).second;
						found = (*fit_it).first;
					}
				}
				*found = *(*parent_it).first;
			}
			mom.first->removeEdges();
			dad.first->removeEdges();
			
			mom.first->buildSpanner(mom.second);
			dad.first->buildSpanner(dad.second);
        }
            cout << "population size " << population.size() << endl;
			for (int i = 0; i < population.size(); i++) {
				double dil = population[i]->getMaxDilation();
                cout << "dil " << dil << endl;
				if(dil < min_dil && min_dil > 1) {
					min_dil = dil;
				}
			}
		
        cout << "iter_limit: " << iter_limit << "min_dil " << min_dil <<  " loop " << ((min_dil > t || iter_limit > 0)) << endl;

        iter_limit--;
	}
	cout << "Final Dilation: " << min_dil << endl;
    
	double min = INT_MAX;
    int min_dil_index = 0;
    int ii = 0;
	for (vector<Spanner *>::iterator it = population.begin(); it != population.end(); it++) {
		double dil = (*it)->getMaxDilation();
		if(dil < min) {
            min_dil_index = ii;
			min = dil;
		}
        ii++;
	}
	cout << "Final Dilation: " << min << endl;
    
    
    float min_fitness = INT_MAX;
    //Spanner *min_spanner = NULL;
	int min_spanner = 0;
	int index = 0;
	
	for (map<Spanner *, float>::iterator iter = fitnesses.begin(); iter != fitnesses.end(); iter++) {
        if ((*iter).second < min_fitness && (*iter).first->getMaxDilation() < t) {
            min_fitness = (*iter).second;
			min_spanner = index;
        }
		index++;
    }
    //cout << "max dilation " << min_spanner->getMaxDilation() << endl;
    cout << "Min fitness: " << min_fitness << endl;
//    cout << "Spanner edges : " << min_spanner->getEdges().size() << endl;
    
    /*for(unsigned int x = 0; x < min_spanner->getEdges().size(); x++) {
        min_spanner->getEdges().at(x)->draw();
    }*/
    min_spanner = min_dil_index;
    
    if (min_fitness < INT_MAX) {
		vector<pair<Spanner *, Spanner *> > spannerpair = vector<pair<Spanner *, Spanner *> >();
		spannerpair.push_back(pair<Spanner *, Spanner *>(population[min_spanner], population[min_spanner]));
		multimap<Spanner *, string> tempmap = this->generateStringRepresentation(spannerpair);
		cout << tempmap.find(population[min_spanner])->second << endl;
        this->buildSpanner(tempmap.find(population[min_spanner])->second);
		cout << "Dilation of found spanner: " << population[min_spanner]->getMaxDilation() << endl;
    }
}

map<Spanner *, float> GeneticSpanner::computeFitnesses(vector<Spanner *> spanners) {
	map<Spanner *, float> fitnesses = map<Spanner *, float>();
    float offset = 0.f;

	for(vector<Spanner *>::iterator it = spanners.begin(); it != spanners.end(); it++) {
		float fitness = 0.f;
		bool feasible = true;
		float max_dil = 0;
        offset = 1;
		
		vector<Point *> spanner_points = (*it)->getPoints();
		
		for (vector<Point *>::iterator first_point = spanner_points.begin(); first_point != spanner_points.end(); first_point++) {
			for (vector<Point *>::iterator second_point = first_point + 1; second_point != spanner_points.end(); second_point++) {
				float dilation = (*it)->getDilation((*first_point), (*second_point));
                
				if(dilation > max_dil) {
					max_dil = dilation;
				}
//				cout << "dilation: " << dilation << endl;
				
				/*if(dilation < INT_MAX) {
					offset += dilation;
				}*/
				
				//offset = (*it)->getEdges().size();
				if(dilation == INT_MAX) {
					fitness += offset*10; //TODO magic number
					feasible = false;
				}
                else if (dilation > t) {
					feasible = false;
                }
				else {
					//fitness += dilation;
				}
                //cout << "Fitness: " << fitness << " Offset: " << offset << " Dilation: " << dilation << " Feasible: " << feasible << endl;
			}
		}
		
		if(!feasible) {
			//fitness += offset;
			if(max_dil != INT_MAX) {
				float dist_from_t = (max_dil - this->t);
				/*if(dist_from_t > 10*t) {
					fitness += (max_dil - this->t) * (*it)->getEdges().size()*10;
				}
				if(dist_from_t > 6*t) {
					fitness += (max_dil - this->t) * (*it)->getEdges().size() * 8;
				}
				else if(dist_from_t > 4*t) {
					fitness += (max_dil - this->t) * (*it)->getEdges().size() * 6;
				}
				else if( dist_from_t > 2*t) {
					fitness += (max_dil - this->t) * (*it)->getEdges().size() * 4;
				}
				else {
					fitness += (max_dil - this->t) * (*it)->getEdges().size() * 2;
				}*/
				fitness += dist_from_t * dist_from_t * dist_from_t * offset;
			}
		}
        
		//fitness += (*it)->getEdges().size();
		
		//cout << "Fitness: " << fitness << " Offset: " << offset << " Dilation: " << (*it)->getMaxDilation() << " Feasible: " << feasible << endl;

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
	
	srand ( (unsigned int)time(NULL) );
	
	for (map<Spanner *, float>::iterator it = fitnesses.begin(); it != fitnesses.end(); it++) {
		(*it).second /= sum;
		(*it).second = 1.0f/(*it).second;
	}
	
	float fit_sum = this->sumOfFitnesses(fitnesses);
	for (map<Spanner *, float>::iterator it = fitnesses.begin(); it != fitnesses.end(); it++) {
		(*it).second /= fit_sum;
	}
	
	
	map<Spanner *, float>::iterator it;
	
	for (int i = 0; i < size; i++) {
		float lottery = (float)rand() / ((float)RAND_MAX);
		float limit;
		
		it = fitnesses.begin();
		limit = (*it).second;
		
		for (int j = 0; limit <= 1.f && j < fitnesses.size(); j++) {
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

		return true;
	}
	return false;
}


void GeneticSpanner::crossover(string &a, string &b) {

	int crossover_point = (int)(rand() % (a.length()-1)) + 1; // crossover at 0 would change strings completly, so force it to min 1.

	string a2 (a, 0, crossover_point);
	a2.append(b, crossover_point, b.length()-crossover_point);
	string b2 (b, 0, crossover_point);
	b2.append(a, crossover_point, a.length()-crossover_point);

	a=a2;
	b=b2;
}
