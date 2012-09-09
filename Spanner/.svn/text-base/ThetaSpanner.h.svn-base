//
//  ThetaSpanner.h
//  Spanner
//
//  Created by Marieke Zantema on 4/4/11.
//  Copyright 2011 TU/e. All rights reserved.
//
#pragma once

#include <functional>
#include "Spanner.h"

using namespace std;

typedef pair<double,double> Punt; //The Dutch word for `point'
typedef pair<Punt, int> IndexedPunt;
typedef pair<IndexedPunt, Punt> Node;

class ThetaSpanner : public Spanner {
public: ThetaSpanner(vector<Point *> points, double t);
		vector<vector<double> > getAdjacency();
/*protected: double slope;
		bool LineSort(IndexedPunt one, IndexedPunt two) {
			double Xone=one.first.first-one.first.second/slope;
			double Xtwo=two.first.first-two.first.second/slope;
			if (Xone<Xtwo) {
				return true;
			} else {
				if (Xone>Xtwo) {
					return false;
				} else {
					return (one.first.second>two.first.second);}}
		} */
};

struct LineSort : binary_function<IndexedPunt, IndexedPunt, bool >
{
const double slope;
LineSort( const double& slope ): slope( slope ) { }
bool operator()( const IndexedPunt& one, const IndexedPunt& two ) const
{
// do whatever you need to do here
	double Xone=one.first.first-one.first.second/slope;
	double Xtwo=two.first.first-two.first.second/slope;
	if (Xone<Xtwo) {
		return true;
	} else {
		if (Xone>Xtwo) {
			return false;
		} else {
			return (one.first.second>two.first.second);}}
}
};
