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
#include <sys/time.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "ThetaSpanner.h"
#include "GreedySpanner.h"
#include "ThetaSpanner.h"
#include "TestSpanner.h"
#include "WSPDSpanner.h"
#include "GeneticSpanner.h"
#include "Point.h"
#include "Edge.h"

int win_x = 640;
int win_y = 480;

int xmax=0;
int ymax=0;

int k,n,a,b,mx,my;
int lastspanner = 0;

bool reset = false;

TestSpanner * testspanner;
ThetaSpanner * thetaspanner;
GreedySpanner * greedyspanner;

std::vector<Point *> points;
std::vector<Point> ActualPoints;

double t=10.0;

Spanner * spanner;

void clear() {
    for(unsigned int x = 0; x < spanner->getEdges().size(); x++) {
        if(spanner->getEdges().at(x) != NULL) {
            delete spanner->getEdges().at(x);
			spanner->getEdges().at(x) = NULL;
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

	srand((unsigned int)time(0));

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
	datafile.open ("data.txt");
	datafile << "" << v.size() << "\n";
	datafile << "" << a << " " << b << "\n";
	for (unsigned int i=0; i<v.size(); i++) {
		datafile << "" << v[i].first << " " << v[i].second << "\n";
	}
	datafile.close();
}

static void ReadData() {
	int n,a,b,x,y;
	std::ifstream in("data.txt");
    cout << "Reading data" << endl;
	if(in){
		in >> n;
		std::cout << "Number of points is " << n << ".\n";
		in >> a;
        cout << "a " << a << endl;
		in >> b;
        cout << "b " << b << endl;
		t=(double)a/(double)b;
		std::cout << "Maximum dilation is " << t << ".\n";
		for (int i = 0; i<n; i++) {
            cout << "i " << i;
			in >> x;
            x = 800 -x ;
            cout << " x " << x ; // TODO why x and y are not correct??
			if (x*1.1>xmax) {xmax=x*1.1;}
			in >> y;
            cout << " y " << y << endl;
            y =  y; //TODO flipping y coordinates for the supermarket data
			if (y*1.1>ymax) {ymax=y*1.1;}

            bool ok=true;
			for(int j=0; j<points.size(); j++) {if (points[j]->getX()==x && points[j]->getY()==y) {ok=false;}}
			if(ok) {points.push_back(new Point(x,y));}
		}

	}
}

void printMetrics() {
    cout << "Size: " << spanner->getSize() << std::endl; 
    cout << "Weight: " << spanner->getWeight() << std::endl;
    cout << "Max Degree: " << spanner->getMaxDegree() << std::endl;
	//cout << "Max Dilation (Marieke): " << spanner->getDilation() << std::endl;
    cout << "Max Dilation (Matias): " << spanner->getMaxDilation() << std::endl << std::endl;
}

static void draw() {
    glClear ( GL_COLOR_BUFFER_BIT );
    
    for (std::vector<Point *>::iterator i = points.begin(); i != points.end(); i++) {
        (*i)->draw();
    }
    
    for(unsigned int x = 0; x < spanner->getEdges().size(); x++) {
        spanner->getEdges().at(x)->draw();
    }
    
    glutSwapBuffers();
}

void resetSpanner() {
	ReadData();
	
	delete spanner;
    
    switch (lastspanner) {
        case 0:
            spanner = new TestSpanner(points, t);
            break;
        case 1:
            spanner = new ThetaSpanner(points, t);
            break;
        case 2:
            spanner = new GreedySpanner(points, t);
            break;
        case 3:
            spanner = new WSPDSpanner(points, t, xmax, ymax);
            break;
		case 4:
			spanner = new GeneticSpanner(points, t, 80, 20, xmax, ymax);
        default:
            break;
    }
}

void resetData() {
    clear();
    
    if(!n) {
        GenerateData(n, a, b, mx, my);
    }
    
    resetSpanner();
}

static void update() {
    draw();
}

static void initGraphics() {
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(win_x, win_y);
    glutCreateWindow("Spanners");
    
    glViewport ( 0, 0, xmax, ymax );
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ();
	gluOrtho2D ( 0.0f, xmax, 0.0f, ymax );
	glClearColor ( 1.0f, 1.0f, 1.0f, 1.0f );
	glClear ( GL_COLOR_BUFFER_BIT );
}

void runTests() {
	timeval start, end, diff;
	double thetatime = 0, greedytime = 0, wspdtime = 0;
	
	for (int i = 0; i < 100; i++) {		
		//Theta-Spanner testing
		lastspanner = 1;
		clear();
		GenerateData(n, a, b, mx, my);
		
		gettimeofday(&start, NULL);
		
		resetSpanner();
		
		gettimeofday(&end, NULL);
		timersub(&end, &start, &diff);
		
		thetatime += diff.tv_sec + diff.tv_usec/1000000.0;
		
		//Greedy-Spanner testing
		lastspanner = 2;
		clear();
		GenerateData(n, a, b, mx, my);
		
		gettimeofday(&start, NULL);
		
		resetSpanner();
		
		gettimeofday(&end, NULL);
		timersub(&end, &start, &diff);
		
		greedytime += diff.tv_sec + diff.tv_usec/1000000.0;
		
		//WSPD-Spanner testing
		lastspanner = 3;
		clear();
		
		gettimeofday(&start, NULL);
		
		resetSpanner();
		
		gettimeofday(&end, NULL);
		timersub(&end, &start, &diff);
		
		wspdtime += (double)diff.tv_sec + (double)diff.tv_usec/1000000.0;
	}
	
	std::cout << "Theta: " << thetatime << std::endl;
	std::cout << "Greedy: " << greedytime << std::endl;
	std::cout << "WSPD: " << wspdtime << std::endl;
}

void getStats() {	
//	ofstream wdspfile("wspd.txt", ios::out);
//	
	double wspdtime;
//	
	timeval start, end, diff;
//	
//	lastspanner = 3;
//	
//	for(int i = 1; i < 300; i++) {
//		n = i;
//		clear();
//		GenerateData(n, a, b, mx, my);
//		
//		gettimeofday(&start, NULL);
//		
//		resetSpanner();
//		
//		gettimeofday(&end, NULL);
//		
//		timersub(&end, &start, &diff);
//		
//		wspdtime = (double)diff.tv_sec + (double)diff.tv_usec/1000000.0;
//		
//		wdspfile << n << "," << wspdtime << std::endl;
//	}
	
	
//	wdspfile.close();
	
	ofstream greedyfile("greedy.txt", ios::out);
		
	lastspanner = 2;
	
	for(int i = 1; i < 100; i++) {
		n = i;		
		clear();
		GenerateData(n, a, b, mx, my);
		
		gettimeofday(&start, NULL);
		
		resetSpanner();
		
		gettimeofday(&end, NULL);
		
		timersub(&end, &start, &diff);
		
		wspdtime = (double)diff.tv_sec + (double)diff.tv_usec/1000000.0;
		
		greedyfile << n << "," << wspdtime << std::endl;	
	}
	
	greedyfile.close();
	
	cout << "done" << endl;
}

static void key_func(unsigned char key, int x, int y) {
    switch (key) {
        case 'q':
        case 'Q':
            clear();
            exit(0);
            break;
        case '1':
            lastspanner = 0;
            clear();
            resetSpanner();
            cout << "Spanner: Test Spanner" << std::endl;
            printMetrics();
            break;
        case '2':
            lastspanner = 1;
            clear();
            resetSpanner();
            cout << "Spanner: Theta Spanner" << std::endl;
            printMetrics();
            break;
        case '3':
            lastspanner = 2;
            clear();
            resetSpanner();
            cout << "Spanner: Greedy Spanner" << std::endl;
            printMetrics();
            break;
        case '4':
            lastspanner = 3;
            clear();
            resetSpanner();
            cout << "Spanner: WSPD Spanner" << std::endl;
            printMetrics();
			cout << "Depth: " << spanner->getDepth() << endl;
            break;
		case '5':
            lastspanner = 4;
            clear();
            resetSpanner();
            cout << "Spanner: Genetic Spanner" << std::endl;
            printMetrics();
            break;
        case 'r':
        case 'R':
            resetData();
            printMetrics();
            break;
		case 't':
			runTests();
			break;
		case 'y':
			getStats();
			break;
        default:
            break;
    }
}

int main (int argc, char** argv)
{
    if(argc == 1) {
        n = 45;
        a = 15;
        b = 10;
        mx = 640;
        my = 480;
// kommentoitu pois, jotta sain pakotettua supermarket datan sisään        
//        GenerateData(n, a, b, mx, my);
    }
    
    if(argc > 1 && atoi(argv[1]) == 1) {
        n = atoi(argv[2]);
        a = atoi(argv[3]);
        b = atoi(argv[4]);
        mx = atoi(argv[5]);
        my = atoi(argv[6]);
        GenerateData(n, a, b, mx, my);
    }
  
//	std::cout << "Read file (0) or generate random (1): ";
//	std::cin>>k;
//	if(k==1){
//	std::cout << "Number of points: ";
//	std::cin >> n;
//	std::cout << "Maximal dilation t=a/b, a= ";
//	std::cin >> a;
//	std::cout << "Maximal dilation t=a/b, b= ";
//	std::cin >> b;
//	std::cout << "Maximal x and y coordinate: ";
//	std::cin >> m;
//	GenerateData(n, a, b, m);}

	ReadData();
        
    thetaspanner = new ThetaSpanner(points, t);
    spanner = thetaspanner;
    lastspanner = 1;
    
    printMetrics();

    glutInit(&argc, argv);
    
    initGraphics();
    
    glutDisplayFunc(&draw);
    glutIdleFunc(&update);
    glutKeyboardFunc(&key_func);

	glutMainLoop();  
    
    return 0;
}



