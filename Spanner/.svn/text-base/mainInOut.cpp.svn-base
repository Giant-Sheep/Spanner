//
//  main.cpp
//  Spanner
//
//  Created by Matias Piispanen on 4/4/11.
//  Copyright 2011 TU/e. All rights reserved.
//
#include <iostream>
#include <fstream>
//#include <QtGui/qgraphicsscene.h>

#include "ThetaSpanner.h"

int main (int argc, const char * argv[])
{
	int n,a,b,x,y; //ä; <- What is this?
	double t;
	std::vector<Point> points;
	std::ifstream in("test.txt");
	if(in){
		in >> n;
		std::cout << "n is " << n << ".\n";
		points.reserve(n);
		Point p(0,0);
		in >> a;
		in >> b;
		t=(double)a/(double)b;
		std::cout << "t is " << t << ".\n";
		for (int i = 0; i<n; i++) {
			in >> x;
			in >> y;
			p = Point(x,y);
			points.push_back(p);
		}
	}

	std::cout << "These are the points:\n";
	for (int i = 0; i<n; i++) {
		std::cout << "(" << points[i].getX() << "," << points[i].getY() << ")\n";
	}

	ThetaSpanner S(points,t);
	std::vector<Edge> edges;
	edges=S.getEdges();
	std::cout <<"These are the edges:\n";
	for (unsigned int i=0; i<edges.size(); i++) {
		std::cout << "(" << edges[i].getFirst().getX() << "," << edges[i].getFirst().getY() << ") - ("
			<< edges[i].getSecond().getX() << "," << edges[i].getSecond().getY() << ")\n";
	}

    std::cout << "Enter a number to quit. ";
	std::cin >> ä;
    return 0;
}

