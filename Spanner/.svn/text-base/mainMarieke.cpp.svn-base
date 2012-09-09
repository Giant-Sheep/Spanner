//
//  main.cpp
//  Spanner
//
//  Created by Matias Piispanen on 4/4/11.
//  Copyright 2011 TU/e. All rights reserved.
//
#include <iostream>
#include <cstdio>
#include <fstream>
#include <ctime>
#include <sstream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "ThetaSpanner.h"
#include "GreedySpanner.h"
#include "ThetaSpanner.h"
#include "TestSpanner.h"
#include "Point.h"
#include "Edge.h"

int win_x = 640;
int win_y = 480;

int xmax=0;
int ymax=0;

ThetaSpanner * thetaspanner;

std::vector<Point *> points;
std::vector<Point> ActualPoints;

double t=10.0;

std::string filename;

Spanner * spanner;

void clear() {
    for(unsigned int x = 0; x < spanner->getEdges().size(); x++) {
        if(spanner->getEdges().at(x) != NULL) {
            spanner->getEdges().at(x) = NULL;
            delete spanner->getEdges().at(x);
        }
    }
    
    for(std::vector<Point *>::iterator i = points.begin(); i != points.end(); i++) {
        delete (*i);
    }
    
    points = std::vector<Point *>();
    
    if(spanner != NULL) {
        delete spanner;
        spanner = NULL;
    }
}

static void GenerateData(unsigned int NrOfPoints, int a, int b, int maxX, int maxY) {
	int x,y;
	pair<int,int> integers;
	std::vector<pair<int, int> > v;
	v.reserve(NrOfPoints);
	bool ok;

	srand(time(0));

	//b=rand()/maxt;
	//a=(int)(((double)rand()*(double)(maxt-1)*(double)b/(double)RAND_MAX)+b);

	while (v.size()<NrOfPoints) {
		x=rand()%maxX;
		y=rand()%maxY;
		ok=true;
		for (unsigned int i=0; i<v.size(); i++) {
			if(x==v[i].first && y==v[i].second) {ok=false;}
		}
		if (ok) {
			v.push_back(make_pair(x,y));
		}
	}
	ofstream datafile;
	datafile.open (filename.c_str());
	datafile << "" << v.size() << "\n";
	datafile << "" << a << " " << b << "\n";
	for (unsigned int i=0; i<v.size(); i++) {
		datafile << "" << v[i].first << " " << v[i].second << "\n";
	}
	datafile.close();
}

static void ReadData() {
	int n,a,b,x,y;
	std::ifstream in(filename.c_str());
    
	if(in){
		in >> n;
		std::cout << "Number of points is " << n << ".\n";
		in >> a;
		in >> b;
		t=(double)a/(double)b;
		std::cout << "Maximum dilation is " << t << ".\n";
//		points.reserve(n);
//		ActualPoints.reserve(n);
//		Point p(0,0);
//		Point * PointerToPoint = new Point(0,0);
		for (int i = 0; i<n; i++) {
			in >> x;
			if (x*1.1>xmax) {xmax=x*1.1;}
			in >> y;
			if (y*1.1>ymax) {ymax=y*1.1;}
//			p = Point(x,y);
//			ActualPoints.push_back(p);
			points.push_back(new Point(x,y));
		}
		/*for (int i = 0; i<n; i++) {
			PointerToPoint=&ActualPoints[i];
			points.push_back(PointerToPoint);
		}*/
		in.close();
	}
}

void printMetrics() {
    cout << "Size: " << spanner->getSize() << std::endl; 
    cout << "Weight: " << spanner->getWeight() << std::endl;
    cout << "Max Degree: " << spanner->getMaxDegree() << std::endl;
	cout << "Max Dilation (Marieke): " << spanner->getDilation() << std::endl;
    cout << "Max Dilation (Matias): " << spanner->getMaxDilation() << std::endl << std::endl;
}

static void draw() {
    glClear ( GL_COLOR_BUFFER_BIT );
    
    for(unsigned int x = 0; x < spanner->getEdges().size(); x++) {
		spanner->getEdges().at(x)->draw();
    }
		    
    glutSwapBuffers();
}


static void update() {
    }

static void initGraphics() {
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(xmax, ymax);
    glutCreateWindow("Spanners");
    
    glViewport ( 0, 0, xmax, ymax );
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ();
	gluOrtho2D ( 0.0f, xmax, 0.0f, xmax );
	glClearColor ( 1.0f, 1.0f, 1.0f, 1.0f );
	glClear ( GL_COLOR_BUFFER_BIT );
}

static void key_func(unsigned char key, int x, int y) {
    switch (key) {
        case 'q':
        case 'Q':
            clear();
			exit(0);
            break;
        default:
            break;
    }
}

int main (int argc, char** argv)
{
	int k;
	std::cout << "Test rounds (0) or drawing (1): ";
	std::cin >>k;
	if(k==1){
	filename="data.txt";
	int n,a,b,m;
	std::cout << "Read file (0) or generate random (1): ";
	std::cin>>k;
	if(k==1){
		std::cout << "Number of points: ";
		std::cin >> n;
		std::cout << "Maximal dilation t=a/b, a= ";
		std::cin >> a;
		std::cout << "Maximal dilation t=a/b, b= ";
		std::cin >> b;
		std::cout << "Maximal x and y coordinate: ";
		std::cin >> m;
		GenerateData(n, a, b, m, m);
	}

	ReadData();

	std::cout << points.size() << "\n";
        
    thetaspanner = new ThetaSpanner(points, t);
    spanner = thetaspanner;

	std::cout << "thetaspanner\n";
    
    draw();
	std::cout << "draw\n";
    printMetrics();

    initGraphics();
    
    glutDisplayFunc(&draw);
    glutIdleFunc(&update);
    glutKeyboardFunc(&key_func);

	glutMainLoop();
	} else {
	//Test rounds
	int r;
	int n,a,b=1000,m=600;
	std::cout << "Number of runs: ";
	std::cin >> r;
	std::string name="Data\\data";
	
	ofstream log;
	log.open ("Data\\log.txt");
	srand(time(0));
	double dilationZ;
	double dilationP;
	for(int i=1; i<=r; i++) {
		std::cout << "Round " << i << " of " << r << "\n";
		n=(int)((double)rand()*990/(double)RAND_MAX)+10;
		a=(int)((double)rand()*1000/(double)RAND_MAX)+1000;

		std::stringstream number;
		number << "" << i;
		filename=name;
		filename.append(number.str());
		filename.append(".txt");
		
		GenerateData(n,a,b,m, m);
		ReadData();
		
		thetaspanner = new ThetaSpanner(points, t);
		spanner = thetaspanner;

		dilationZ = spanner->getDilation();
		dilationP = spanner->getMaxDilation();
		if(dilationZ>t || dilationP>t || dilationZ!=dilationP) {log << "### ";}
		else {log << "    ";}
		log << "Run " << i << ": required dilation was " << t << ", Marieke's dilation was " << dilationZ << " and Matias's dilation was " << dilationP << ".\n";

		points.erase(points.begin(),points.end());
		ActualPoints.erase(ActualPoints.begin(), ActualPoints.end());
	}
	log.close();
	delete thetaspanner;}
    return 0;
}



