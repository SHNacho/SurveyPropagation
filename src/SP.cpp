#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>
#include "SP.h"
#include "graph.h"
#include "random.h"


using namespace std;


result SolverSP::SID(Graph* graph, int t_max, float precision, float f){

	totalIt = 0;
	vector<Edge*> edges = graph->enabledEdges();

	int unassigned_vars = graph->unassigned_vars;
	int n_vars_fix = unassigned_vars * f;
	n_vars_fix = (1 < n_vars_fix) ? n_vars_fix : 1;

	while(true){
		totalIt++;
		// Llamamos a la rutina SP
		result result_sp = surveyPropagation(graph, t_max, precision);
		if(result_sp != SP_CONVERGED) return result_sp;

		// Comprobamos si el problema es trivial
		bool trivial = true;
		for(int i = 0; i < edges.size() && trivial; ++i){
			if (edges[i]->survey != 0.00)
				trivial = false;
		}

		// Si no es trivial
		if(!trivial){
			vector<Variable*> unassignedVars = graph->unassignedVars();
			// Para cada variable no asignada calculamos los sesgos
			for(Variable* var : unassignedVars){
				computeBias(var);
			}
			// Las ordenamos en función de sus sesgos
			sort(unassignedVars.begin(), unassignedVars.end(), compareVars);
			
			int auxAssign = n_vars_fix;
			// Fijamos las n variables que mayor sesgo tengan
    		for (int i = 0; i < auxAssign; i++) {
				// Las variables pueden haber sido asignadas por UP ya que al
				// asignar una variable, se limpia el grafo y se llama a UP
    		  	if (unassignedVars[i]->value != Unassigned) {
    		  	  auxAssign++;  
    		  	  continue;
    		  	}

    		  	Variable* var = unassignedVars[i];

    		  	// Recalculate biases for same reason, previous assignations clean the
    		  	// graph and change relations
				// Recalculamos los biases ya que la anterior asignación puede
				// provocar que se modifiquen
    		  	computeBias(var);

				// Calculamos el valor que se le va a asignar a la variable en función
				// de sus sesgos
    		  	lbool val= var->negativeBias > var->positiveBias ? False : True;
				
				// Asignamos la variable
    		  	if (!assignVariable(var, val)) {
					// Error si la variable ya ha sido asignada
    		  	  	return CONTRADICTION;
    		  	}
    		}
    	}

		//TODO Comprobar si se ha satisfecho la fórmula
	}
}

result SolverSP::surveyPropagation(Graph* graph, int t_max, float precision){

	auto rng = std::default_random_engine {};

	// Calculate subproducts of all variables
  	computeSubProducts(graph);
  	for (int i = 0; i < t_max; i++) {
  	  	// Randomize clause iteration
  	  	vector<Function*> unsatisfiedClauses = graph->unsatisfiedFunctions();
  	  	shuffle(unsatisfiedClauses.begin(), unsatisfiedClauses.end(), rng);

  	  	// Calculate surveys
  	  	double maxConvergeDiff = 0.0;
  	  	for (Function* clause : unsatisfiedClauses) {
  	  	  	double maxConvDiffInClause = SP_UPDATE(clause);

  	  	  	// Save max convergence diff
  	  	  	if (maxConvDiffInClause > maxConvergeDiff)
  	  	  	  	maxConvergeDiff = maxConvDiffInClause;
  	  	}

  	  	// Check if converged
  	  	if (maxConvergeDiff <= precision) {
  	  	  	return SP_CONVERGED;
  	  	}
  	}
  	// cout << ":-(" << endl;
  	// Max itertions reach without convergence
  	return SP_UNCONVERGED;
}

void SolverSP::computeSubProducts(Graph* graph){
	for(Variable* var : graph->variables){
		if(var->value == Unassigned){
			var->ps = 1.0;
			var->ns = 1.0;
			var->pzero = 0;
			var->nzero = 0;
		

			//TODO ¿Tendría que actualizar ps cuando la arista es positiva??
			for(Edge* edge : var->neighborhood){
				if(edge->enabled){
					double diff = 1.0 - edge->survey;
					if(edge->negated){
						if(diff > ZERO)
							var->ps *= diff; 
						else
							var->pzero++;
					}
					else{
						if(diff > ZERO)
							var->ns *= diff;
						else
							var->nzero++;
					}
				}
			}
		}
	}
}

double SolverSP::SP_UPDATE(Function* clause){

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




