//
//  ThetaSpanner.cpp
//  Spanner
//
//  Created by Marieke Zantema on 4/4/11.
//  Copyright 2011 TU/e. All rights reserved.
//

#include "ThetaSpanner.h"
#include <algorithm>
#include <cmath>

#include <iostream>

std::vector<std::vector<double> > adjacencyTh;

ThetaSpanner::ThetaSpanner(vector<Point *> points, double t) : Spanner(points, t) {
	size_t n=points.size();
	int k;
	k=max(6,(int)ceil(3.1415926535897932384626433832795028841971/asin(0.5-1/(2*t))));
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

	//Add 0's and -1's in adjacency matrix
	adjacencyTh.erase(adjacencyTh.begin(), adjacencyTh.end());
	std::vector<double> aux(n, -1);
	for(int i=0; i<n; i++) {
		aux[i]=0;
		adjacencyTh.push_back(aux);
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
		std::sort(Punten.begin(), Punten.end(), LineSort(slope));
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
		for (int j=(int)n-2; j>=0; j--) {
			if(Tree[2*j+1].first.first.first<Tree[2*j+2].first.first.first) {
				p=Tree[2*j+1].first;
			} else {p=Tree[2*j+2].first;}
			Tree[j]=make_pair(p,Tree[2*j+1].second);
		}

		// Sort by r1 = tan(Pi/k), Punten will be the event queue
		slope=tan(3.14159265358979/(double)k);
		std::sort(Punten.begin(), Punten.end(), LineSort(slope));
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
				if(LineSort(slope)(p,make_pair(Tree[TreePointer*2+2].second,-1))) {
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
			//std::cout << "Size of adjacency matrix: " << adjacencyTh.size() <<  " x " << adjacencyTh[0].size() << "\n";
			//std::cout << "Size of points: " << points.size() << "\n";
			if(LeftMostSoFar.second != -1 && adjacencyTh[Punten[j].second][LeftMostSoFar.second] == -1) {	// A leftmost point is found and the edge is not yet added
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
				adjacencyTh[Punten[j].second][LeftMostSoFar.second]=e->getLength();
				adjacencyTh[LeftMostSoFar.second][Punten[j].second]=e->getLength();
                this->addEdge(e);
				//std::cout << "Added edge (" << a->getX() << ", " << a->getY() << ") - (" << b->getX() << ", " << b->getY() << ")\n";
//            }
			}
		}
	}
}

vector<vector<double> > ThetaSpanner::getAdjacency() {
	return adjacencyTh;
}
