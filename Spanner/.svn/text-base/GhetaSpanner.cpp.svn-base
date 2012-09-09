//
//  GhetaSpanner.cpp
//  Spanner
//
//  Created by Marieke Zantema on 4/4/11.
//  Copyright 2011 TU/e. All rights reserved.
//

#include "GhetaSpanner.h"
#include <algorithm>
#include <cmath>

#include <iostream>

std::vector<std::vector<double> > adjacencyGh;

GhetaSpanner::GhetaSpanner(vector<Point *> points, double t) : Spanner(points, t) {
	int n=points.size();	
	int k;
	k=max(6,(int)ceil(3.1415926535897932384626433832795028841971/(4*asin(0.5-1/(2*t)))));
	std::cout << "Number of cones is " << k << ".\n";

	std::vector<IndexedPunt> Punten(n);
	std::vector<Node> Tree(2*n-1);
	double newX, newY;
	IndexedPunt p;
	IndexedPunt LeftMostSoFar;
	int TreePointer;
	int Index;
	double slope;
	//bool nieuw; // I can't use the English word `new'...
	Point * a, * b;
	Edge * e;
	std::vector<pair<int, int> > shortcut;

	//Add 0's and -1's in adjacency matrix
	adjacencyGh.erase(adjacencyGh.begin(), adjacencyGh.end());
	std::vector<double> aux(n, -1);
	for(int i=0; i<n; i++) {
		aux[i]=0;
		adjacencyGh.push_back(aux);
		aux[i]=-1;
	}

	// Rotate the points for each of the k cones
//	int i=0;
	for (int i=0; i<k; i++) {
		//std::cout << "Starting with cone " << i+1 << ".\n";
		// Fill Punten with new (rotated) points
		for (int j=0; j<n; j++) {
			newX=(cos(0.1+2*i*3.14159265358979/k)*(double)points[j]->getX())-(sin(0.1+2*i*3.14159265358979/k)*(double)points[j]->getY());
			newY=(sin(0.1+2*i*3.14159265358979/k)*(double)points[j]->getX())+(cos(0.1+2*i*3.14159265358979/k)*(double)points[j]->getY());
			Punten[j]=make_pair(make_pair(newX,newY),j);
		}

		// Sort by r2 = -tan(Pi/k)
		slope=-tan(3.14159265358979/(double)k);
		std::sort(Punten.begin(), Punten.end(), LineSortG(slope));
		//std::cout << "Preparing status, Punten is now:\n";
		//for (int j=0; j<n; j++) {
		//	std::cout << "(" << points[Punten[j].second]->getX() << ", " << points[Punten[j].second]->getY() << ")\n";
		//}

		// Create Tree, this will be the status
		TreePointer=(int)pow(2,floor(log((double)2*n-2)/log((double)2)))-1;
		// Initialize leaves
		for (int j=0; j<n; j++) {
			Tree[TreePointer]=make_pair(Punten[j], Punten[j].first);
			if (TreePointer<2*n-2) {TreePointer++;}
			else {TreePointer=(int)floor(((double)TreePointer-0.5)/2)+1;}
			}
		// Recursively initialize internal nodes
		for (int j=n-2; j>=0; j--) {
			if(Tree[2*j+1].first.first.first<Tree[2*j+2].first.first.first) {
				p=Tree[2*j+1].first;
			} else {p=Tree[2*j+2].first;}
			Tree[j]=make_pair(p,Tree[2*j+1].second);
		}

		// Sort by r1 = tan(Pi/k), Punten will be the event queue
		slope=tan(3.14159265358979/(double)k);
		std::sort(Punten.begin(), Punten.end(), LineSortG(slope));
//		std::cout << "Preparing event queue, Punten is now:\n";
//		for (int j=0; j<n; j++) {
//			std::cout << "(" << points[Punten[j].second]->getX() << ", " << points[Punten[j].second]->getY() << ")\n";
//		}

		slope=-tan(3.14159265358979/(double)k);

		// Connect each point to the point that is leftmost in the cone
		for (int j=0; j<n; j++) {
			//std::cout << "Starting with point " << j+1 << " or (" << points[Punten[j].second]->getX() << ", " << points[Punten[j].second]->getY() << ")\n";
			//std::cout << "Tree is:\n";
			//for (int ã=0; ã<2*n-1; ã++) {
			//	std::cout << "(((" << Tree[ã].first.first.first << ", " << Tree[ã].first.first.second << "), " <<
			//		Tree[ã].first.second << "), (" << Tree[ã].second.first << ", " << Tree[ã].second.second << "))\n";
			//}

			// Remove Punten[j] from Tree
			TreePointer=0;
			p=Punten[j];
			// Walk from the root to p
			while ((TreePointer < n-1) && Tree[TreePointer].first.second != -1) {
				if(LineSortG(slope)(p,make_pair(Tree[TreePointer*2+2].second,-1))) {
					TreePointer=TreePointer*2+1;
				} else {TreePointer=TreePointer*2+2;}
			}
			// Now TreePointer should point to the leaf where p is stored
			if(p.second==Tree[TreePointer].first.second) {		// If everything goes well, this always happens
				Tree[TreePointer].first.second=-1;				// This `removes' p from Tree
				Index=-1;										// Index=Tree[TreePointer].first.second, `where we came from'
				TreePointer=(int)floor(((double)TreePointer-0.5)/2);
			}

			// Update Tree and search for the leftmost point; walk from p back to the root
			LeftMostSoFar=make_pair(make_pair(0.0,0.0),-1);
			while (TreePointer >= 0) {
				if(Tree[TreePointer*2+1].first.second == -1) {
					if (Tree[TreePointer*2+2].first.second == -1) {
						Tree[TreePointer].first.second = -1;
					} else {
						Tree[TreePointer]=Tree[TreePointer*2+2];
						if(Index==-1) {LeftMostSoFar=Tree[TreePointer*2+2].first;}
					}
				} else {
					if (Tree[TreePointer*2+2].first.second == -1) {
						Tree[TreePointer]=Tree[TreePointer*2+1];
					} else {
						if (Tree[TreePointer*2+1].first.first.first<Tree[TreePointer*2+2].first.first.first) {
							p=Tree[TreePointer*2+1].first;
						} else {
							p=Tree[TreePointer*2+2].first;
							}

						if(Index==Tree[TreePointer*2+1].first.second) {
							if(LeftMostSoFar.second == -1){
								LeftMostSoFar=Tree[TreePointer*2+2].first;
							} else {
								if(LeftMostSoFar.first.first>Tree[TreePointer*2+2].first.first.first) {
									LeftMostSoFar=Tree[TreePointer*2+2].first;
								}
							}
						}
						Tree[TreePointer] = make_pair(p,Tree[TreePointer*2+1].second);
					}
				}
					// Now Tree and LeftMostSoFar are updated up to the level where we are,
					//    change the index to the current node and let TreePointer point to the
					//    parent of the current node
				Index=Tree[TreePointer].first.second;
				TreePointer=(int)floor(((double)TreePointer-0.5)/2);
			}
			// Add edge if applicable
			//std::cout << "Size of adjacency matrix: " << adjacencyGh.size() <<  " x " << adjacencyGh[0].size() << "\n";
			//std::cout << "Size of points: " << points.size() << "\n";
			if(LeftMostSoFar.second != -1 && adjacencyGh[Punten[j].second][LeftMostSoFar.second] == -1) {	// A leftmost point is found and the edge is not yet added
				a=points[Punten[j].second];
				b=points[LeftMostSoFar.second];
//				nieuw=true;
//				for (unsigned int j=0; j<edges.size(); j++) {	// Check whether we already have the edge that is to be added
//					if(((edges[j]->getFirst()->getX()==a->getX() && edges[j]->getFirst()->getY()==a->getY())
//						&&(edges[j]->getSecond()->getX()==b->getX() && edges[j]->getSecond()->getY()==b->getY()))
//						||
//						((edges[j]->getFirst()->getX()==b->getX() && edges[j]->getFirst()->getY()==b->getY())
//						&&(edges[j]->getSecond()->getX()==a->getX() && edges[j]->getSecond()->getY()==a->getY()))
//						) {nieuw=false;}
//				}
//				if (nieuw) {
				e=new Edge(a, b);
				adjacencyGh[Punten[j].second][LeftMostSoFar.second]=e->getLength();
				adjacencyGh[LeftMostSoFar.second][Punten[j].second]=e->getLength();
                this->addEdge(e);
				//std::cout << "Added edge (" << a->getX() << ", " << a->getY() << ") - (" << b->getX() << ", " << b->getY() << ")\n";
//            }
			}
		}
	}
	//std::cout << "Starting shortcut edges...\n";
	shortcut=add(points,t);
	for(unsigned int i=0; i<shortcut.size(); i++) {
		a=points[shortcut[i].first];
		b=points[shortcut[i].second];
		e=new Edge(a, b);
		adjacencyGh[shortcut[i].first][shortcut[i].second]=e->getLength();
		adjacencyGh[shortcut[i].second][shortcut[i].first]=e->getLength();
        this->addEdge(e);
		//std::cout << "Added edge (" << a->getX() << ", " << a->getY() << ") - (" << b->getX() << ", " << b->getY() << ")\n";
	}
}

std::vector<pair<int, int> > GhetaSpanner::add(std::vector<Point *> points, double t) {
	int n=points.size();
	std::vector<pair<int, int> > shortcut;

	//Dijkstra's algorithm
	std::vector<pair<double, bool> > networkDistance(n);
	int count;
	int index;
	double dist;
	for(int i=0; i<n; i++) {
		//std::cout << i << " of " << n <<std::endl;
		for(int j=0; j<n; j++) {
			networkDistance[j]=make_pair(adjacencyGh[i][j],true);
		}
		networkDistance[i]=make_pair(networkDistance[i].first,false);
		for(int j=1; j<n; j++) {
			//std::cout << "Round " << j << ": ";
			//Search for next point; in the `real' Dijkstra algorithm this is implemented in a heap
			//    (running in log(n) time instead of linear time).
			count=0;
			while(count<n && (!networkDistance[count].second||(networkDistance[count].first<0))) {count++;}
			if(count==n) {/*std::cout << "########";*/}
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
				if((networkDistance[k].second)&&(adjacencyGh[index][k]>=0)&&(dist+adjacencyGh[index][k]<networkDistance[k].first || networkDistance[k].first<0)) {
					networkDistance[k]=make_pair(dist+adjacencyGh[index][k],true);
				}
			}
			networkDistance[index]=make_pair(networkDistance[index].first,false);
			}}
		
		//for(int j=0; j<n; j++) {std::cout << j << ": " << networkDistance[j].first << "\n";}

		for(int j=0; j<n; j++) {
			if((i<j)&&(t<networkDistance[j].first/(sqrt(pow((points[i]->getX() - points[j]->getX()), 2.0) + pow((points[i]->getY() - points[j]->getY()), 2.0)))))
				{shortcut.push_back(make_pair(i,j));}
			//std::cout << "Dilation between " << i+1 << " and " << j+1 << " is " << networkDistance[j].first/(sqrt(pow((points[i]->getX() - points[j]->getX()), 2.0) + pow((points[i]->getY() - points[j]->getY()), 2.0))) << std::endl;
		}
	}
	return shortcut;
}

vector<vector<double> > GhetaSpanner::getAdjacency() {
	return adjacencyGh;
}
