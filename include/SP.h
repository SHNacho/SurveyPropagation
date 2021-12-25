#ifndef SP_H
#define SP_H

#include <iostream>
#include <cmath>
#include <vector>
#include "graph.h"

bool surveyPropagation(Graph grafo, int t_max, float precision);

double SP_UPDATE(Edge* edge);

#endif //SP_H
