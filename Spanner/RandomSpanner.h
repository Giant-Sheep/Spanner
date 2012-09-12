//
//  RandomSpanner.h
//  Spanner
//
//  Created by Matias Piispanen on 9/12/12.
//  Copyright (c) 2012 Aalto Yliopisto. All rights reserved.
//

#ifndef __Spanner__RandomSpanner__
#define __Spanner__RandomSpanner__

#include <iostream>

#include "Spanner.h"

class RandomSpanner : public Spanner {
public:
	RandomSpanner(vector<Point *> points, double t, double probability);
};

#endif /* defined(__Spanner__RandomSpanner__) */
