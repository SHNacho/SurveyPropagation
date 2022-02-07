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
	bool converged = true;

	auto rng = std::default_random_engine {};

	vector<Edge*> edges = graph->getEnabledEdges();

	bool next = false;
	int t;
	for(t = 1; t <= t_max && !next; ++t){
		cout << "SP iter: " << t << endl;
		// Contador para contar el número de aristas que han
		// alcanzado la precisión deseada
		int counter = 0;
		// Recorremos las aristas de manera aleatoria
		shuffle(std::begin(edges), std::end(edges), rng);
		for(Edge* e : edges){
			// Si la arista no ha convergido aún
			if(!(e->hasConverged())){
				// Actualizamos 'survey'
				double prev_survey = e->getSurvey();
				SP_UPDATE(e);
				// Comprobamos si converge
				if(abs(e->getSurvey() - prev_survey) < precision){
					e->setConverged(true);
					counter++;
				}
			} else {
				counter++;
			}
		}
		
		// Si han convergido todas las aristas 
		if(counter == edges.size())
			next = true;	
	}

	if(t == t_max)
		converged = false;

	// Reestablecemos de nuevo la variable de convergencia de las aristas
	for(Edge* e : edges){
		e->setConverged(false);
	}

	return converged;
}

double SP_UPDATE(Edge* edge){
	double survey = 1.0000;
	vector<Edge*> neigh = edge->getFunction()->getEnabledNeighborhood();

	if(neigh.size() != 0){
		for(Edge* n : neigh){
			Variable* var = n->getVariable();
			if(var != edge->getVariable()){
				n->calculateProducts();
				survey = survey * (var->getPu() /
						  (var->getPu() + var->getPs() + var->getP0()));
			}	
		}
	}

	edge->setSurvey(survey);

//	cout << "Survey calculada: " << survey << endl;
	return survey;
}

result unitPropagation(Graph* graph){
	vector<Function*> functions = graph->getEnabledFunctions();
	// Para cada cláusula
	for(Function* f : functions){
		vector<Edge*> neigh = f->getEnabledNeighborhood();
		// Comprobamos si solo tiene una variable
		// y si es así, asignamos el valor de dicha
		// variable que satisfaga la cláusula
		if(neigh.size() == 1){
			Variable* var = neigh[0]->getVariable();
			if(neigh[0]->isNegated()){
				// Si la variable ya está asignada y
				// es distinta de la que se requiere,
				// hemos llegado a una contradicción
				if( var->isAssigned() && (var->getValue() != false) ) 
					return CONTRADICTION;

				graph->assignVar(var, false);
			} else {
				if( var->isAssigned() && (var->getValue() != true) ) 
					return CONTRADICTION;

				graph->assignVar(var, true);
			}
			graph->clean(var);
		}
		// Si la cláusula no tiene vecinos significa que se
		// han asignado todas sus variables y todavía no está
		// satisfecha, por lo que no podemos seguir
		else if(neigh.size() == 0){
			return CONTRADICTION;
		}
	}
	
	return NO_CONTRADICTION;
}

bool walksat(Graph* graph, int MAX_TRIES, int MAX_FLIPS){
	for (int i = 0; i < MAX_TRIES; ++i){
		// TODO:Asigación aleatoria

		for(int j = 0; j < MAX_FLIPS; ++j){
			if(graph->unassignedVars() == 0) return true;
			bool fliped = false;
			vector<Variable*> variables = graph->
			for(int k = 0; k < )
		}
	}
}

bool SID(Graph* graph, int t_max, float precision){
	vector<Edge*> edges = graph->getEnabledEdges();
	vector<Variable*> variables = graph->getUnassignedVariables();

	vector<int> assign(variables.size(), -1);

	// Inicializamos las "survey" de manera aleatoria
	for(Edge* e : edges){
		e->setSurvey(Randfloat(0.0, 1.0));
	}



	result result_unit_prop = NO_CONTRADICTION;

	int count = 0;
	while(graph->unassignedVars() > 0 &&
		  result_unit_prop == NO_CONTRADICTION)
	{
		
		if(!surveyPropagation(graph, t_max, precision)){
			cout << "Solución no encontrada: Survey Propagation no ha convergido" << endl;
			return false;
		}

		// Comprobamos si el problema es trivial
		bool trivial = true;
		for(int i = 0; i < edges.size() && trivial; ++i){
			if (edges[i]->getSurvey() != 0.00)
				trivial = false;
		}

		// Si no es trivial
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
		if(var->enabledNeighborhood().size() == 0 && !(var->isAssigned())){}
			graph->assignVar(var, 1);
	}

	return true;
}


