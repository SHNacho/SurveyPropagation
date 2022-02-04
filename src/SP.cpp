#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>
#include "SP.h"
#include "graph.h"
#include "random.h"


using namespace std;

enum result { 
	SP_UNCONVERGED, 
	SP_CONVERGED,
	CONTRADICTION,
	NO_CONTRADICTION
};

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

result unitPropagation(Graph* graph){
	vector<Function*> functions = graph->getFunctions();
	// Para cada cláusula
	for(Function* f : functions){
		// Comprobamos si solo tiene una variable
		// y si es así, asignamos el valor de dicha
		// variable que satisfaga la cláusula
		if(f->getEnabledEdges() == 1){
			Edge* neigh = f->getNeighborhood()[0];
			Variable* var = neigh->getVariable();
			if(neigh->isNegated()){
				// Si la variable ya está asignada y
				// es distinta de la que se requiere,
				// hemos llegado a una contradicción
				if(var->getValue() != 0) return CONTRADICTION;
				var->setValue(0);
			} else {
				if(var->getValue() != 1) return CONTRADICTION;
				var->setValue(1);
			}

			graph->clean(var);
		}
	}
	
	return NO_CONTRADICTION;
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

		result resultado_unit_prop = unitPropagation(graph);
		if(resultado_unit_prop == CONTRADICTION){
			cout << "Se han encontrado contradicciones durante Unit Propagation" << endl;
		}
	}
}


