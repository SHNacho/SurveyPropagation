#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>
#include "SP.h"
#include "graph.h"
#include "random.h"


using namespace std;

bool surveyPropagation(Graph* graph, int t_max, float precision){
	bool unconverged = false;

	auto rng = std::default_random_engine {};

	vector<Edge*> edges = graph->getEdges();

	bool next = false;
	int t;
	for(t = 1; t <= t_max && !next; ++t){
		// Contador para contar el número de aristas que han
		// alcanzado la precisión deseada
		int counter = 0;
		// Recorremos las aristas de manera aleatoria
		shuffle(std::begin(edges), std::end(edges), rng);
		for(Edge* e : edges){
			// Actualizamos el mensaje para cada arista
			double prev_survey = e->getSurvey();
			e->setSurvey(SP_UPDATE(e));
			
			if((e->getSurvey() - prev_survey) < precision)
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
	// TODO: habría que eliminar los vecinos de las funciones que
	// 		 ya se hayan asignado
	vector<Edge*> neigh = edge->getFunction()->getNeighborhood();

	if(neigh.size() != 0){
		for(Edge* n : neigh){
			Variable* var = n->getVariable();
			if(var != edge->getVariable()){
				n->calculateProducts();
			}
			
			survey = survey * (var->getPu() /
						  (var->getPu() + var->getPs() + var->getP0()));
		}
	}


	return survey;
}

void unitPropagation(Graph* graph){

}

vector<int> SID(Graph* graph, int t_max, float precision){
	vector<Edge*> edges = graph->getEdges();
	vector<Variable*> variables = graph->getVariables();

	vector<int> assign(variables.size(), -1);

	// Inicializamos las "survey" de manera aleatoria
	for(Edge* e : edges){
		e->setSurvey(Randfloat(0.0, 1.0));
	}

	while(true){
		bool converge = surveyPropagation(graph, t_max, precision);

		bool trivial = true;

		for(Edge* e : edges){
			if (e->getSurvey() != 0.00)
				trivial = false;
		}

		// Si hay "surveys" que no sean triviales
		double max_bias = 0.0;
		double pos_max_bias = 0;
		if(trivial == false){
			for(int i = 0; i < variables.size(); ++i){
				double bias = variables[i]->calculateBias();
				if(abs(bias) > max_bias){
					max_bias = abs(bias);
					pos_max_bias = i;
				}
			}

			Variable* aux = variables[pos_max_bias];
			aux->fix();
			// Introducimos la asignación en el vector solución
			assign[aux->getId()] = aux->getValue();
			
			graph->clean(variables[pos_max_bias]);
		}
		else{
			//walksat()
		}

		unitPropagation(graph);
	}
}
