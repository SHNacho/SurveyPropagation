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
	NO_CONTRADICTION,
	SAT
};

bool SID(Graph* graph, int t_max, float precision, float f);
bool surveyPropagation(Graph grafo, int t_max, float precision, int & totalIt);
double SP_UPDATE(Edge* edge);
result unitPropagation(Graph* graph, Function* clause);
void calculateProducts(Graph* graph);
void updateOldSurvey(Graph* graph);
void randomInit(Graph* graph);

#endif //SP_H
