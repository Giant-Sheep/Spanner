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
	double mutation_probability = 0.1;
	int mutations = 50;
	
	srand ( (unsigned int)time(NULL) );
	
	for(int i = 0; i < generation_size; i++) {
		population_points.push_back(vector<Point *>());
		for(vector<Point *>::iterator it = points.begin(); it != points.end(); it++) {
			population_points.back().push_back(new Point((*it)->getX(), (*it)->getY()));
		}
		
		//TODO: Magic probability
		//population.push_back(new RandomSpanner(population_points.back(), t, ((i+0.1)*1.0)/(generation_size+0.2)));
		population.push_back(new RandomSpanner(population_points.back(), t, 0.95));

	}
	
	// Test GA
	//	for (int i = 0; i < 10; i++) {
    double min_dil = INT_MAX;
    int iter_limit = 500;
	int mutation_limit = iter_limit - 10;
	int min_iterations = 30;
	
    while (min_iterations != 0 || min_dil > t) {
        
        if (iter_limit < 0) { break;}
		//   while (iter_limit > 0) {
		
		fitnesses = computeFitnesses(population);
		sum = sumOfFitnesses(fitnesses);
        
		//        cout << "sum " << sum << endl;
        
		mating_pool = generateMatingPool(fitnesses, sum, mating_pool_size);
		parents = pairParents(mating_pool);
		parent_strings = generateStringRepresentation(parents);
		
		pair<Spanner*, string> mom;
		pair<Spanner*, string> dad;
			
		fitnesses = computeFitnesses(population);
		
		for(int i = 0; i < population.size(); i++) {
			int first_child_ind = rand() % parent_strings.size();
			int second_child_ind = rand() % parent_strings.size();
			
			multimap<Spanner *, string>::iterator first_it = parent_strings.begin();
			
			for(int j = 0; j < first_child_ind; j++ ) {
				first_it++;
			}
			mom = pair<Spanner *, string>((*first_it).first, (*first_it).second);
			
			multimap<Spanner *, string>::iterator second_it = parent_strings.begin();
			
			for(int j = 0; j < second_child_ind; j++ ) {
				second_it++;
			}
			dad = pair<Spanner *, string>((*second_it).first, (*second_it).second);
			
			
            //			GeneticSpanner::crossover(mom.second, dad.second);
			GeneticSpanner::crossover2(mom.second, dad.second);
			//GeneticSpanner::binaryCrossover(mom.second, dad.second, 0.5);
			
			if(iter_limit == mutation_limit) {
				mutation_probability = 0.05;
			}
			//            mom.first->removeEdges();
			//			dad.first->removeEdges();
			GeneticSpanner::mutation(mutation_probability, mom.second, mutations);
			GeneticSpanner::mutation(mutation_probability, dad.second, mutations);
			
			population[i]->removeEdges();
			population[i]->buildSpanner(mom.second);
			
			if(i != population.size() - 1) {
				i++;
				population[i]->removeEdges();
				population[i]->buildSpanner(dad.second);
			}
		}
			
			
//			for (int j = 0; j < 2; j++) {
//				int i = 0;
//				int max_fit_ind = 0;
//				double max_fit = 0.0;
//				computeFitnesses(population);
//				for (map<Spanner *, float>::iterator fit_it = fitnesses.begin(); fit_it != fitnesses.end(); fit_it++) {
//					if ((*fit_it).second > max_fit) {
//						max_fit = (*fit_it).second;
//						max_fit_ind = i;
//					}
//					i++;
//				}
//				cout << "max fit: " << max_fit_ind << endl;
//				if(j == 0) {
//					population[max_fit_ind]->removeEdges();
//					population[max_fit_ind]->buildSpanner(mom.second);
//				}
//				else {
//					population[max_fit_ind]->removeEdges();
//					population[max_fit_ind]->buildSpanner(dad.second);
//				}
//			}
		
		//			mom.first->buildSpanner(mom.second);
		//			dad.first->buildSpanner(dad.second);
		/*			fitnesses = this->computeFitnesses(population);
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
		 } */
		
		cout << "population size " << population.size() << endl;
		
		double sum_dil = 0;
		min_dil = INT_MAX;
		for (int i = 0; i < population.size(); i++) {
			double dil = population[i]->getMaxDilation();
			sum_dil += dil;
			if(dil < min_dil && min_dil > 1) {
				min_dil = dil;
			}
			
			cout << " " << dil;
		}
		cout << endl;
		/*if (iter_limit%2==1) {
		 *population[max_dil_ind] = *population[min_dil_ind];
		 }
		 else {
		 *population[max_dil_ind] = *population[rand()%population.size()];
		 }*/
		cout << " " << endl;
		
		cout << "iter_limit: " << iter_limit << "min_dil " << min_dil <<  " loop " << ((min_dil > t || iter_limit > 0)) << " avg_dil " << (sum_dil/population.size()) << endl;
		
		iter_limit--;
		if(min_iterations != 0) {
			min_iterations--;
		}
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
	float final_dilation = 0;
	
	fitnesses = computeFitnesses(population);
	
	for (vector<Spanner *>::iterator it = population.begin(); it != population.end(); it++) {
		double dil = (*it)->getMaxDilation();
		if(dil < t && fitnesses.find((*it))->second < min_fitness) {
			min_fitness = fitnesses.find((*it))->second;
			final_dilation = fitnesses.find((*it))->first->getMaxDilation();
			min_spanner = index;
		}
		index++;
	}
	
	/*for (map<Spanner *, float>::iterator iter = fitnesses.begin(); iter != fitnesses.end(); iter++) {
		if ((*iter).first->getMaxDilation() < t && (*iter).second < min_fitness) {
			min_fitness = (*iter).second;
			final_dilation = (*iter).first->getMaxDilation();
			min_spanner = index;
		}
		index++;
    }*/
    //cout << "max dilation " << min_spanner->getMaxDilation() << endl;
    cout << "Min fitness: " << min_fitness << " Min Dilation: " << final_dilation << endl;
//    cout << "Spanner edges : " << min_spanner->getEdges().size() << endl;
    
    /*for(unsigned int x = 0; x < min_spanner->getEdges().size(); x++) {
        min_spanner->getEdges().at(x)->draw();
    }*/

    
    // TODO kumpi ois paarempi: min_fitness vai  min_dilation spanner ?
    //min_spanner = min_dil_index;
    
    if (min_fitness < INT_MAX) {
		vector<pair<Spanner *, Spanner *> > spannerpair = vector<pair<Spanner *, Spanner *> >();
		spannerpair.push_back(pair<Spanner *, Spanner *>(population[min_spanner], population[0]));
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
                
				/*				if(dilation > max_dil) {
				 max_dil = dilation;
				 }*/
				//				cout << "dilation: " << dilation << endl;
				
				/*if(dilation < INT_MAX) {
				 offset += dilation;
				 }*/
				
				//offset = (*it)->getEdges().size();
				if(dilation == INT_MAX) {
					//					fitness += offset*1000; //TODO magic number
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
        double gmd = (*it)->getMaxDilation();
		double edge_penalty = (*it)->getWeight() / (double)282935 * 5;
		edge_penalty *= edge_penalty;
		
		if(!feasible) {
			if(gmd != INT_MAX) {
				gmd = gmd * gmd;
			}
			else {
				gmd = 10000.0;
			}
			//fitness += offset;
			/*			if(max_dil != INT_MAX) {
			 float dist_from_t = (max_dil - this->t);
			 
			 fitness += dist_from_t *dist_from_t *dist_from_t * dist_from_t * offset;
			 }*/
		}
        
		//fitness += (*it)->getEdges().size();
		//		cout << "Fitness: " << fitness << " Offset: " << offset << " Dilation: " << gmd << " Feasible: " << feasible << endl;
		fitnesses[(*it)] = gmd + edge_penalty; //fitness;
        
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
	
	/*for (map<Spanner *, float>::iterator it = fitnesses.begin(); it != fitnesses.end(); it++) {
		(*it).second /= sum;
		(*it).second = 1.0f/(*it).second;
	}
	
	float fit_sum = this->sumOfFitnesses(fitnesses);
	for (map<Spanner *, float>::iterator it = fitnesses.begin(); it != fitnesses.end(); it++) {
		(*it).second /= fit_sum;
	}*/
	
	for (map<Spanner *, float>::iterator it = fitnesses.begin(); it != fitnesses.end(); it++) {
		(*it).second /= sum;
		(*it).second = 1.0f - (*it).second;
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


void GeneticSpanner::mutation(double probability, string &s, int t) {
	for (int j=0; j<t; j++) {
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
		}}
}

void GeneticSpanner::binaryCrossover(string &a, string &b, double probability) {
	string a2 = "";
	string b2 = "";
	
	double r = (double)rand() / (double)INT_MAX;
	
	for (int i = 0; i < a.length(); i++) {
		if(r < probability) {
			a2.append(b, i, 1);
			b2.append(a, i, 1);
		}
		else {
			a2.append(a, i, 1);
			b2.append(b, i, i);
		}
	}
	
	a = a2;
	b = b2;
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


void GeneticSpanner::crossover2(string &a, string &b) {
    
	int crossover_point = (int)(rand() % (a.length()-1)) + 1; // crossover at 0 would change strings completly, so force it to min 1.
    int crossover_point2 = (int)(rand() % (a.length()-1)) + 1; // crossover at 0 would change strings completly, so force it to min 1.
    if (crossover_point>crossover_point2) {
        int temp = crossover_point;
        crossover_point = crossover_point2;
        crossover_point2 = temp;
    }
	string a2 (a, 0, crossover_point);
	a2.append(b, crossover_point, crossover_point2);
    a2.append(a, crossover_point2, a.length()-crossover_point2);
    
	string b2 (b, 0, crossover_point);
	b2.append(a, crossover_point, crossover_point2);
    b2.append(b, crossover_point2, b.length()-crossover_point2);
    
	a=a2;
	b=b2;
}
