#ifndef SP_H
#define SP_H

#include <iostream>
#include <cmath>
#include <vector>
#include "graph.h"

enum result { 
	SP_UNCONVERGED, 
	SP_CONVERGED,
	CONTRADICTION,
	NO_CONTRADICTION
};

bool surveyPropagation(Graph grafo, int t_max, float precision, int & totalIt);

result unitPropagation(Graph* graph);

double SP_UPDATE(Edge* edge);

bool SID(Graph* graph, int t_max, float precision, float f);

#endif //SP_H
