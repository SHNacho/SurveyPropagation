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
				graph->assignVar(var, 0);
			} else {
				if(var->getValue() != 1) return CONTRADICTION;
				graph->assignVar(var, 1);
			}

			graph->clean(var);
		}
	}
	
	return NO_CONTRADICTION;
}

bool SID(Graph* graph, int t_max, float precision){
	vector<Edge*> edges = graph->getEdges();
	vector<Variable*> variables = graph->getVariables();

	vector<int> assign(variables.size(), -1);

	// Inicializamos las "survey" de manera aleatoria
	for(Edge* e : edges){
		e->setSurvey(Randfloat(0.0, 1.0));
	}



	result result_unit_prop = NO_CONTRADICTION;
	bool converge = true;

	int count = 0;
	while(graph->unassignedVars() > 0 &&
		  result_unit_prop == NO_CONTRADICTION &&
		  converge)
	{
		converge = surveyPropagation(graph, t_max, precision);
		if(converge == false){
			cout << "Solución no encontrada: Survey Propagation no ha convergido" << endl;
			return false;
		}

		bool trivial = true;

		for(int i = 0; i < edges.size() && trivial; ++i){
			if (edges[i]->getSurvey() != 0.00)
				trivial = false;
		}

		cout << "No es trivial" << endl;

		// Si hay "surveys" que no sean triviales
		double max_bias = 0.0;
		double pos_max_bias = 0;
		if(trivial == false){
			for(int i = 0; i < variables.size(); ++i){
				if(variables[i]->getValue() == -1){
					double bias = variables[i]->calculateBias();
					if(abs(bias) >= max_bias){
						max_bias = abs(bias);
						pos_max_bias = i;
					}
				}
			}

			Variable* aux = variables[pos_max_bias];
			graph->assignVar(aux);

			

			// Introducimos la asignación en el vector solución
			assign[aux->getId()] = aux->getValue();
			
			graph->clean(variables[pos_max_bias]);
			cout << "Se limpia el grafo. Número de aristas: " << graph->getEdges().size() << endl; 
		}
		else{
			//walksat()
			cout << "walksat" << endl;
			return true;
		}

		result_unit_prop = unitPropagation(graph);
		if(result_unit_prop == CONTRADICTION){
			cout << "Se han encontrado contradicciones durante Unit Propagation" << endl;
			return false;
		}
		cout << "Variables no asignadas: " << graph->unassignedVars() << endl;
	}

	for(Variable* var : variables){
		if(var->getNeighborhood().size() == 0 && var->getValue() == -1)
			graph->assignVar(var, 1);
	}

	return true;
}


