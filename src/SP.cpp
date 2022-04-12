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
	//randomInit(graph);

	bool next = false;
	

	for(t = 1; t <= t_max && !next; ++t){
		computeSubProducts(graph);
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
	
	if(t >= t_max) converged = false;
	

	// Reestablecemos de nuevo la variable de convergencia de las aristas
	for(Edge* e : edges)
		e->setConverged(false);
	

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
	double survey = 1.0;
	double product_u, product_s, product_null;
	vector<Edge*> neigh = edge->getFunction()->getEnabledNeighborhood();

	if(neigh.size() != 0){
		for(Edge* n : neigh){
			Variable* j = n->getVariable();
			if(j != edge->getVariable()){
				computeProducts(n, product_u, product_s, product_null);
				if(product_u <= ZERO) 
					survey = 0.0;
				else
					survey *= (product_u / (product_s + product_u + product_null));
			}	
		}
	}

	edge->setSurvey(survey);

	return survey;
}

//---------------------------------------------//
void computeProducts(Edge* edge, double & product_u, double & product_s, double & product_null){

	//------- Cálculo de los productos ------//
	Variable* var = edge->getVariable();
	if(edge->isEnabled() && !var->isAssigned()){
		double pu, ps, pt;
		// Si la variable está negada
		if(edge->isNegated()){
			if(var->positive_nulls > 0)
				pu = 0.0;
			else
				pu = var->positive_prod;

			if(var->negative_nulls > 0){
				if(var->negative_nulls == 1 && edge->nullMessage)
					ps = var->negative_prod; 
				else
					ps = 0.0;
			} else
				ps = var->negative_prod / (1 - edge->getSurvey());
		}
		// Si la variable no está negada
		else{
			if(var->negative_nulls > 0)
				pu = 0.0;
			else
				pu = var->negative_prod;

			if(var->positive_nulls > 0){
				if(var->positive_nulls == 1 && edge->nullMessage)
					ps = var->positive_prod; 
				else
					ps = 0.0;
			} else
				ps = var->positive_prod / (1 - edge->getSurvey());
		}
		product_u = (1.0 - pu) * ps;
		product_s = (1.0 - ps) * pu;
		product_null = pu*ps;
	}
	
}

//---------------------------------------------//
result unitPropagation(Graph* fg, Function* clause){
	vector<Edge*> enabled_vars = clause->getEnabledNeighborhood();

	if(!clause->isSatisfied()){
		if(enabled_vars.size() == 1 && !clause->isSatisfied()){
			bool val = enabled_vars[0]->isNegated() ? false : true;
			assignVar(fg, enabled_vars[0]->getVariable(), val);
			return(NO_CONTRADICTION);
		} 
		else if(enabled_vars.size() == 0){
			cout << "Cláusula no satisfecha" << endl;
			return CONTRADICTION;
		} 
	}

	return (NO_CONTRADICTION);
}

//---------------------------------------------//
bool assignVar(Graph* fg, Variable* var, bool val){
	if(var->isAssigned()){
		//cout << "Variable ya asignada" << endl;
		return false;
	}
	return fg->assignVar(var, val);
}

//---------------------------------------------//
bool walksat(Graph* graph, int MAX_TRIES, int MAX_FLIPS){
	for (int i = 0; i < MAX_TRIES; ++i){
		// TODO:Asigación aleatoria

		for(int j = 0; j < MAX_FLIPS; ++j){
			if(graph->unassignedVars() == 0) return true;
			vector<Variable*> variables = graph->getUnassignedVariables();
			//TODO: Buscar la variable que produzca la mejor variación
			for(unsigned k = 0; k < variables.size(); ++i){
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

	//result result_unit_prop = unitPropagation(graph);
	randomInit(graph);


	int count = 0;
	while(graph->unassignedVars() > 0)
	{
		count ++;
		//cout << "Iteración SID: " << count << endl;
		if(!surveyPropagation(graph, t_max, precision, totalIt)){
			//cout << "Solución no encontrada: Survey Propagation no ha convergido" << endl;
			cout << "UNCONVERGED" << endl;
			return false;
		}

		// Comprobamos si el problema es trivial
		bool trivial = true;
		for(int i = 0; i < edges.size() && trivial; ++i){
			if (edges[i]->getSurvey() > ZERO)
				trivial = false;
		}

		// Si no es trivial
		if(trivial == false){
			computeSubProducts(graph);
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
				Variable* var = variables[i];
				if(!var->isAssigned()){
					bool val = var->getPosBias() > var->getNegBias() ? true : false;
					assignVar(graph, var, val);
				}
			}
		}
		else{
			
			//walksat()
			cout << "WALKSAT" << endl;
			return true;
		}

		// Posiblemente hayan quedado variables que no son necesarias, por lo que
		// les asignamos cualquier valor
		for(Variable* var : variables){
			if(var->enabledNeighborhood().size() == 0 && !(var->isAssigned())){
				assignVar(graph, var, 1);
			}
		}

		//cout << "Aristas restantes: " << graph->getEnabledEdges().size() << endl;
		//cout << "Variables restantes: " << graph->unassignedVars() << endl;
	}

	//cout << "SAT, " << totalIt << endl;
	
	return true;
}

//----------------------------------------------//
void computeSubProducts(Graph* fg){
	for(Variable* var : fg->getVariables()){
		if(!var->isAssigned()){
			// Se inicializan los productos a 1.0
			var->positive_prod = 1.0;
			var->negative_prod = 1.0;
			var->total_prod = 1.0;
			var->positive_nulls = 0;
			var->negative_nulls = 0;

			for(Edge* edge : var->getNeighborhood()){
				if(edge->isEnabled()){
					// Calculamos la differencia
					double diff = 1 - edge->getSurvey();
					// Si no vale 0
					if(diff > ZERO){
						edge->nullMessage=false;
						if(edge->isNegated())
							// Actualiza producto negativo (V_+)
							var->negative_prod *= diff;
						else
							// Actualiza producto positivo (V_-)
							var->positive_prod *= diff;

						var->total_prod *= diff;
					}
					// Si vale 0, marcamos que el mensaje de esa arista
					// va a dar 0
					else{
						edge->nullMessage = true;
						if(edge->isNegated()) 
							var->negative_nulls++;
						else 
							var->positive_nulls++;
					}
				}
			}
		}
	}
}

//---------------------------------------------//
void randomInit(Graph* graph){
	for(Edge* e : graph->getEnabledEdges()){
		e->initRandomSurvey();
	}
}

