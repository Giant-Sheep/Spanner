//
//  Spanner.cpp
//  Spanner
//
//  Created by Matias Piispanen on 4/4/11.
//  Copyright 2011 TU/e. All rights reserved.
//

#include <iostream>
#include "Spanner.h"

Spanner::Spanner(vector<Point *> points, double t) : points(points), t(t) {
    edges = std::vector<Edge *>();
}

int Spanner::getDepth() {
	return 0;
}

double Spanner::getMaxDilation() {
    double max = 0; 
    for (std::vector<Point *>::iterator i = points.begin(); i != points.end(); i++) {
        double tentative = getDilation(true, (*i));
        if(tentative > max) {
            max = tentative;
        }
    }
    
    return max;
}

double Spanner::getDilation(Point *first, Point *second) {
    return getDilation(false, first, second);
}

double Spanner::getDilation(bool maximum, Point * first, Point * second) {
    std::vector<double> dist(points.size(), INT_MAX);
    std::vector<Point *> nodes = points;
    Point * dest = NULL;
    double max = 0.0;
    
    //Set the distance of the source node to zero
    for (int i = 0; i < nodes.size(); i++) {
        if (first == nodes.at(i)) {
            dist[i] = 0;
            break;
        }
    }
    
    while (nodes.size() > 0) {
        double min = INT_MAX;
        int index = 0;
        
        //Find the node with the smallest distance
        for (int i = 0; i < dist.size(); i++) {
            if(dist[i] < min) {
                min = dist[i];
                index = i;
            }
        }
        
        // All remaining points unreachable
        if (dist[index] == INT_MAX) {
            if(maximum == false) {
                return INT_MAX;
            }
            
            break;
        }
        
        // Remove current node from the data structure
        Point * current = nodes[index];
        double currentDist = dist[index];
        dist.erase(dist.begin()+index);
        nodes.erase(nodes.begin()+index);
        
        // Terminate here if only calculating the dilation between two points
        if(max == false && current == second) {
            return currentDist / (sqrt((pow(first->getX() - current->getX(), 2.0) + pow(first->getY() - current->getY(), 2.0))));
        }
        
        // Keep track of the current maximum dilation
        double currentDilation = currentDist / (sqrt((pow(first->getX() - current->getX(), 2.0) + pow(first->getY() - current->getY(), 2.0))));
        if(currentDilation > max) {
            max = currentDilation;
            dest = current;
        }
        
        std::vector<Edge *> edges = current->getEdges();
        
        // Calculate dilations of the neighbour points
        for (std::vector<Edge *>::iterator j = edges.begin(); j != edges.end(); j++) {
            Point * neighbour = NULL;
            if((*j)->getFirst() == current) {
                neighbour = (*j)->getSecond();
            }
            else {
                neighbour = (*j)->getFirst();
            }
            
            bool found = false;
            int neighbourIndex = 0;
            for (int k = 0; k < nodes.size(); k++) {
                if(nodes.at(k) == neighbour) {
                    found = true;
                    neighbourIndex = k;
                    break;
                }
            }
            
            if(found) {
                double tentativeDist = currentDist + (*j)->getLength();
                
                if (tentativeDist < dist[neighbourIndex]) {
                    dist[neighbourIndex] = tentativeDist;
                }
            }
        }
    }
    
    return max;
}


double Spanner::getDilation() {
	double maxDilation=0.0;
	size_t n=points.size();
	int f,s;
	int x,y;
	pair<int, int> ploep;
	//Initialize adjacency matrix
	std::vector<std::vector<double> > adjacency;
	adjacency=this->getAdjacency();
	if(adjacency[0][0] == -1) {
		std::cout << "Calculating adjacency matrix\n";
		for (int i=0; i<n; i++) {adjacency[i][i]=0;}
		for(std::vector<Edge *>::iterator e = edges.begin(); e != edges.end(); e++) {
			for(int i=0; i<n; i++) {
				if((*e)->getFirst()==points[i]) {f=i;}
				if((*e)->getSecond()==points[i]) {s=i;}
			}
			adjacency[s][f]=(*e)->getLength();
			adjacency[f][s]=(*e)->getLength();
		}
	}

	/*//The slow thing:
	for(int i=0; i<n; i++) {
		for(int j=0; j<i; j++) {
			//std::cout << "Adjacency matrix: " << i << ", " << j << std::endl;
			for(std::vector<Edge *>::iterator e = edges.begin(); e != edges.end(); e++) {
				if(((*e)->getFirst() == points[i] && (*e)->getSecond() == points[j])
					|| ((*e)->getFirst() == points[j] && (*e)->getSecond() == points[i]))
				{adjacency[i][j]=(*e)->getLength();
				adjacency [j][i]=adjacency[i][j];}
			}
		}
	}*/

	//Print adjacency matrix; this matrix seems to be initialized corectly.
	/*std::cout << "Adjacency matrix:\n";
	for(int i=0; i<adjacency.size(); i++) {
		for(int j=0; j<adjacency[adjacency.size()-1].size(); j++) {
			std::cout << adjacency[i][j] << " ";
		}
		std::cout << "\n";
	}*/

	//Dijkstra's algorithm
	std::vector<pair<double, bool> > networkDistance(n);
	std::vector<std::vector<int> > from(n,std::vector<int>(n));
	int count;
	int index;
	double dist;
	for(int i=0; i<n; i++) {
		//std::cout << i << " of " << n <<std::endl;
		for(int j=0; j<n; j++) {
			networkDistance[j]=make_pair(adjacency[i][j],true);
			if(adjacency[i][j]>=0) {from[i][j]=i;}
		}
		networkDistance[i]=make_pair(networkDistance[i].first,false);
		for(int j=1; j<n; j++) {
			//std::cout << "Round " << j << ": ";
			//Search for next point; in the `real' Dijkstra algorithm this is implemented in a heap
			//    (running in log(n) time instead of linear time).
			count=0;
			while(count<n && (!networkDistance[count].second||(networkDistance[count].first<0))) {count++;}
			if(count==n) {maxDilation=1e10; /*std::cout << "########";*/}
			else {dist=networkDistance[count].first;}
				index=count;
				count++;
			while(count<n) {
				if((dist>networkDistance[count].first)&&(networkDistance[count].second)&&(networkDistance[count].first>=0)) {
					dist=networkDistance[count].first;
					index=count;
				}
				count++;
			}
			// The point to which index refers, is the closest unhandled point.
			// The distances to all points to which this point has an edge can be calculated.
			//for(int h=0; h<n; h++) {std::cout << (int)networkDistance[h].first << "," << networkDistance[h].second << ".\n";}
			//std::cout << "index=" << index << "\n";
			if(index<n) {for(int k=0; k<n; k++) {
				if((networkDistance[k].second)&&(adjacency[index][k]>=0)&&(dist+adjacency[index][k]<networkDistance[k].first || networkDistance[k].first<0)) {
					networkDistance[k]=make_pair(dist+adjacency[index][k],true);
					from[i][k]=index;
				}
			}
			networkDistance[index]=make_pair(networkDistance[index].first,false);
			}}
		
		//for(int j=0; j<n; j++) {std::cout << j << ": " << networkDistance[j].first << "\n";}

		for(int j=0; j<n; j++) {
			if((i!=j)&&(maxDilation<networkDistance[j].first/(sqrt(pow((points[i]->getX() - points[j]->getX()), 2.0) + pow((points[i]->getY() - points[j]->getY()), 2.0)))))
				{maxDilation=(networkDistance[j].first/(sqrt(pow((points[i]->getX() - points[j]->getX()), 2.0) + pow((points[i]->getY() - points[j]->getY()), 2.0))));
				ploep=make_pair(i,j);}
			//std::cout << "Dilation between " << i+1 << " and " << j+1 << " is " << networkDistance[j].first/(sqrt(pow((points[i]->getX() - points[j]->getX()), 2.0) + pow((points[i]->getY() - points[j]->getY()), 2.0))) << std::endl;
		}
	}
	x=points[ploep.first]->getX();
	y=points[ploep.first]->getY();
//	std::cout << "Largest dilation from (" << x << ", " << y << ")";
//	double size = 5.0;
//  glColor3f(0.0, 0.0, 0.0); 
//	glBegin(GL_QUADS);
//	glVertex2f(x-(size/2), y-(size/2));
//	glVertex2f(x+(size/2), y-(size/2));
//	glVertex2f(x+(size/2), y+(size/2));
//	glVertex2f(x-(size/2), y+(size/2));
//	glEnd();
	x=points[ploep.second]->getX();
	y=points[ploep.second]->getY();
//	std::cout <<  " to (" << x << ", " << y << ").\n";
//	glColor3f(0.0, 0.0, 0.0); 
//	glBegin(GL_QUADS);
//	glVertex2f(x-(size/2), y-(size/2));
//	glVertex2f(x+(size/2), y-(size/2));
//	glVertex2f(x+(size/2), y+(size/2));
//	glVertex2f(x-(size/2), y+(size/2));
//	glEnd();
	index=ploep.second;
	while(from[ploep.first][index]!=ploep.first) {
		index=from[ploep.first][index];
		x=points[index]->getX();
		y=points[index]->getY();
//		std::cout << "Via (" << points[index]->getX() << ", " << points[index]->getY() << ")\n";
//		glColor3f(0.0, 0.0, 0.0); 
//		glBegin(GL_QUADS);
//		glVertex2f(x-(size/2), y-(size/2));
//		glVertex2f(x+(size/2), y-(size/2));
//		glVertex2f(x+(size/2), y+(size/2));
//		glVertex2f(x-(size/2), y+(size/2));
//		glEnd();
	}
//	/*std::cout << "From-matrix:\n";
//	for(int i=0; i<n; i++) {
//		for(int j=0; j<n; j++) {
//			std::cout << from[i][j] << " ";
//		}
//		std:cout << std::endl;
//	}*/
    return maxDilation;
}

size_t Spanner::getSize() {
    return edges.size();
}

double Spanner::getWeight() {
    double sum = 0.0;
    
    for (std::vector<Edge *>::iterator i = edges.begin(); i != edges.end(); i++) {
        sum += (*i)->getLength();
    }
    
    return sum;
}

size_t Spanner::getMaxDegree() {
    int maxdegree = 0;
    
    for (std::vector<Point *>::iterator i = points.begin(); i != points.end(); i++) {
        int degrees = 0;
        for (std::vector<Edge *>::iterator j = edges.begin(); j != edges.end(); j++) {
            if((*j)->getFirst() == (*i) || (*j)->getSecond() == (*i)) {
                degrees += 1;
            }
        }
        
        if(degrees > maxdegree) {
            maxdegree = degrees;
        }
    }
    
    return maxdegree;
}

vector<Point *> Spanner::getPoints() {
    return points;
}

vector<Edge *> Spanner::getEdges() {
    return edges;
}

void Spanner::removeEdges() {
	for (vector<Point *>::iterator it = points.begin(); it != points.end(); it++) {
		(*it)->removeEdges();
	}
	
	edges.clear();
}

void Spanner::addEdge(Edge* edge) {
    std::vector<Edge *> pEdges = edge->getFirst()->getEdges();
    bool found = false;
    
    for (std::vector<Edge *>::iterator i = pEdges.begin(); i != pEdges.end(); i++) {
        if(((*i)->getFirst() == edge->getFirst() || (*i)->getFirst() == edge->getSecond()) && ((*i)->getSecond() == edge->getFirst() || (*i)->getSecond() == edge->getSecond())) {
            found = true;
        }
    }
	
	pEdges = edge->getSecond()->getEdges();
    
    for (std::vector<Edge *>::iterator i = pEdges.begin(); i != pEdges.end(); i++) {
        if(((*i)->getFirst() == edge->getFirst() || (*i)->getFirst() == edge->getSecond()) && ((*i)->getSecond() == edge->getFirst() || (*i)->getSecond() == edge->getSecond())) {
            found = true;
        }
    }
	    
    if(found == false) {
        edges.push_back(edge);
        edge->getFirst()->addEdge(edge);
        edge->getSecond()->addEdge(edge);
    }
}

bool Spanner::hasEdge(Point *first, Point *second) {
	bool found = false;
	
	for (int i = 0; i < edges.size(); i++) {
		if(edges[i]->getFirst() == first) {
			if(edges[i]->getSecond() == second) {
				found = true;
				break;
			}
		}
		else if(edges[i]->getSecond() == first) {
			if(edges[i]->getFirst() == second) {
				found = true;
				break;
			}
		}
	}
	
	return found;
}

vector<vector<double> > Spanner::getAdjacency() {
	std::vector<std::vector<double> > adjacency(points.size(), std::vector<double>(points.size(), -1));
	return adjacency;
}