#include <iostream>
#include <ostream>
#include <random>
#include <algorithm>
#include <iterator>
#include "SP.h"
#include "graph.h"
#include "random.h"


using namespace std;


//---------------------------------------------//
bool surveyPropagation(Graph* graph, int t_max, float precision, int & totalIt){
	bool converged = true;

	auto rng = std::default_random_engine {};

	vector<Edge*> edges = graph->getEnabledEdges();

	int t = 0;
	// Inicializamos las "survey" de manera aleatoria
	randomInit(graph);

	bool next = false;
	
	//calculateProducts(graph);

	for(t = 1; t <= t_max && !next; ++t){
		updateOldSurvey(graph);
		totalIt++;
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
	//			if(t > 200){
	//				cout << "Arista " << e->getFunction()->getId() << "-" << e->getVariable()->getId() << endl;
	//				cout << "Diferencia: " << e->getSurvey() << " - " << prev_survey << " = " << abs(e->getSurvey() - prev_survey) << endl;
	//			}
				if(abs(e->getSurvey() - prev_survey) < precision){
					e->setConverged(true);
					counter++;
				}
			} else {
				counter++;
			}
		}
		
		//cout << "SP Iter: " << t << endl;
		//cout << "Aristas convergidas: " << counter << endl;
		// Si han convergido todas las aristas 
		if(counter == edges.size())
			next = true;	
	}
	
	if(t >= t_max){
		converged = false;
		for(Edge* e : edges){
			if(abs(e->getSurvey() - e->oldSurvey) > precision){
				cout << "Arista " << e->getFunction()->getId() << "-" << e->getVariable()->getId() << endl;
				cout << "Diferencia: " << e->getSurvey() << " - " << e->oldSurvey << " = " << abs(e->getSurvey() - e->oldSurvey) << endl;
			}
		}
	}

	// Reestablecemos de nuevo la variable de convergencia de las aristas
	for(Edge* e : edges){
		e->setConverged(false);
	}

	return converged;
}

//---------------------------------------------//
void updateOldSurvey(Graph* graph){
	vector<Edge*> edges = graph->getEnabledEdges();
	for(Edge* e : edges){
		e->oldSurvey = e->getSurvey();
	}
}


//---------------------------------------------//
double SP_UPDATE(Edge* edge){
	double survey = 1.0000;
	vector<Edge*> neigh = edge->getFunction()->getEnabledNeighborhood();

	if(neigh.size() != 0){
		for(Edge* n : neigh){
			Variable* var = n->getVariable();
			if(var != edge->getVariable()){
				n->calculateProducts();
				if(var->getPu() <= 0.0000001) 
					survey = 0;
				else
					survey *= (var->getPu() / (var->getPu() + var->getPs() + var->getP0()));
				
			}	
		}
	}

	edge->setSurvey(survey);

//	cout << "Survey calculada: " << survey << endl;
	return survey;
}

//---------------------------------------------//
result unitPropagation(Graph* graph){
	vector<Function*> functions = graph->getEnabledFunctions();
	// Para cada cláusula
	for(Function* f : functions){
		if(f->isEnabled()){
			vector<Edge*> neigh = f->getEnabledNeighborhood();
			// Si la cláusula no tiene vecinos significa que se
			// han asignado todas sus variables y todavía no está
			// satisfecha, por lo que no podemos seguir
			if(neigh.size() == 0){
				cout << "CONTRADICTION" << endl;
				return CONTRADICTION;
			}
			// Comprobamos si solo tiene una variable
			// y si es así, asignamos el valor de dicha
			// variable que satisfaga la cláusula
			if(neigh.size() == 1 && !(f->isSatisfied())){
				Variable* var = neigh[0]->getVariable();
				if(neigh[0]->isNegated()){
					// Si la variable ya está asignada y
					// es distinta de la que se requiere,
					// hemos llegado a una contradicción
					if( var->isAssigned() && (var->getValue() != false) ){
						cout << "CONTRADICTION" << endl;
						return CONTRADICTION;
					}
						
					graph->assignVar(var, false);
				} else {
					if( var->isAssigned() && (var->getValue() != true) ){
						cout << "CONTRADICTION" << endl;
						return CONTRADICTION;
					}
					graph->assignVar(var, true);
				}
				graph->clean(var);
			}
		}
	}
	
	return NO_CONTRADICTION;
}

//---------------------------------------------//
// TODO: Completar este UP para que se llame cuando se asigne
// una variable
result unitPropagation(Function* clause){
	vector<Edge*> enabled_vars = clause->getEnabledNeighborhood();

	if(enabled_vars.size() == 1 && !(clause->isEnabled)){

	}
}

//---------------------------------------------//
bool walksat(Graph* graph, int MAX_TRIES, int MAX_FLIPS){
	for (int i = 0; i < MAX_TRIES; ++i){
		// TODO:Asigación aleatoria

		for(int j = 0; j < MAX_FLIPS; ++j){
			if(graph->unassignedVars() == 0) return true;
			vector<Variable*> variables = graph->getUnassignedVariables();
			//TODO: Buscar la variable que produzca la mejor variación
			for(int k = 0; k < variables.size(); ++i){
			}

			// TODO: Flip la variable que produzca la mejor variación
		}
	}

	// TODO: cambiar
	return true;
}

//---------------------------------------------//
bool SID(Graph* graph, int t_max, float precision, float f){
	int totalIt = 0;
	vector<Edge*> edges = graph->getEnabledEdges();
	vector<Variable*> variables = graph->getUnassignedVariables();

	// Inicializamos las "survey" de manera aleatoria
	//for(Edge* e : edges){
	//	e->setSurvey(Randfloat(0.0, 1.0));
	//}

	result result_unit_prop = unitPropagation(graph);

	int count = 0;
	while(graph->unassignedVars() > 0 &&
		  result_unit_prop == NO_CONTRADICTION)
	{
		count ++;
		cout << "Iteración SID: " << count << endl;
		if(!surveyPropagation(graph, t_max, precision, totalIt)){
			//cout << "Solución no encontrada: Survey Propagation no ha convergido" << endl;
			cout << "UNCONVERGED" << endl;
			return false;
		}

		// Comprobamos si el problema es trivial
		bool trivial = true;
		for(int i = 0; i < edges.size() && trivial; ++i){
			if (edges[i]->getSurvey() != 0.00)
				trivial = false;
		}

		// Si no es trivial
		if(trivial == false){
			for(int i = 0; i < variables.size(); ++i){
				if(!(variables[i]->isAssigned())){
					variables[i]->calculateBias();
				}
			}

			sort(variables.begin(), variables.end(), compareVars);

			int unassigned_vars = graph->unassignedVars();
			int aux = unassigned_vars * f;
			int n_vars_fix = (1 < aux) ? aux : 1;
			
			for(int i = 0; i < n_vars_fix; ++i){
				graph->assignVar(variables[i]);
				graph->clean(variables[i]);
			}
		}
		else{
			
			//walksat()
			cout << "WALKSAT" << endl;
			return true;
		}

		result_unit_prop = unitPropagation(graph);
		if(result_unit_prop == CONTRADICTION){
			//cout << "Se han encontrado contradicciones durante Unit Propagation" << endl;
			cout << "CONTRADICTION" << endl;
			return false;
		}

		// Posiblemente hayan quedado variables que no son necesarias, por lo que
		// les asignamos cualquier valor
		for(Variable* var : variables){
			if(var->enabledNeighborhood().size() == 0 && !(var->isAssigned())){
				graph->clean(var);
				graph->assignVar(var, 1);
			}
		}

		//cout << "Aristas restantes: " << graph->getEnabledEdges().size() << endl;
		//cout << "Variables restantes: " << graph->unassignedVars() << endl;
	}

	cout << "SAT, " << totalIt << endl;
	
	return true;
}

//---------------------------------------------//
void randomInit(Graph* graph){
	for(Edge* e : graph->getEnabledEdges()){
		e->initRandomSurvey();
	}
}

