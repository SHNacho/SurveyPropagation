#include <iostream>
#include <random>
#include <algorithm>
#include "SP.h"
#include "graph.h"
#include "random.h"

bool surveyPropagation(Graph* graph, int t_max, float precision){
	bool unconverged = false;

	vector<Edge*> edges = graph->getEdges();
	
	auto rng = std::default_random_engine {};

	for(Edge* e : edges){
		e->setSurvey(Randfloat(0.0, 1.0));
	}

	bool next = false;
	int t;
	for(t = 1; t <= t_max && !next; ++t){
		// Contador para contar el número de aristas que han
		// alcanzado al precisión deseada
		int counter = 0;
		// Recorremos las aristas de manera aleatoria
		shuffle(std::begin(edges), std::end(edges), rng);
		for(Edge* e : edges){
			// Actualizamos el mensaje para cada arista
			double prev_survey = e->getSurvey();
			e->setSurvey(SP_UPDATE(e));
			
			if(e->getSurvey() - prev_survey < precision)
				counter++;
		}

		if(counter == edges.size())
			next = true;	
	}

	if(t == t_max)
		unconverged = true;

	return unconverged;
}

double SP_UPDATE(Edge* edge){
	double survey = 1.0000;
	vector<Edge*> neigh = edge->getFunction()->getNeighborhood();

	if(neigh.size() != 0){
		for(Edge* n : neigh){
			Variable* var = n->getVariable();
			if(var != edge->getVariable()){
				n->calculateProducts();
			}
			
			survey = survey * (var->getPu()/
						  (var->getPu() + var->getPs() + var->getP0()));
		}
	}


	return survey;
}
