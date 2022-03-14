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
					// Si la arista no está negada, actualizamos el subproducto
					// de los vecinos V(+)
					if(edge->negated){
						if(diff > ZERO)
							var->ps *= diff; 
						else
							var->pzero++;
					}
					// Si la arista está negada, actualizamos el subproducto
					// de los vecinos V(-)
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
	double maxConvDiffClause = 0.0;
	int zeros;
	double allSubSurveys = 1.0;
  vector<double> subSurveys;

  // ==================================================================
  // Calculate subProducts of all literals and keep track of wich are 0
  // ==================================================================
  for (Edge* edge : clause->neighborhood) {
    if (edge->enabled && edge->variable->value == Unassigned) {
      Variable* var = edge->variable;
      double m, p, wn, wt;

      // If edge is negative:
      if (edge->negated) {
        m = var->nzero ? 0 : var->ns;
        if (var->pzero == 0)
          p = var->ps / (1.0 - edge->survey);
        else if (var->pzero == 1 && (1.0 - edge->survey) < ZERO)
          p = var->ps;
        else
          p = 0.0;

        wn = p * (1.0 - m);
        wt = m;
      }
      // If edge is positive
      else {
        p = var->pzero ? 0 : var->ps;
        if (var->nzero == 0)
          m = var->ns / (1.0 - edge->survey);
        else if (var->nzero == 1 && (1.0 - edge->survey) < ZERO)
          m = var->ns;
        else
          m = 0.0;

        wn = m * (1 - p);
        wt = p;
      }

      // Calculate subSurvey
      double subSurvey = wn / (wn + wt);
      subSurveys.push_back(subSurvey);

      // If subsurvey is 0 keep track but don't multiply
      if (subSurvey < ZERO) {
        zeros++;
        if (zeros == 2) break;
      } else
        allSubSurveys *= subSurvey;
    }
  }

  // =========================================================
  // Calculate the survey for each edge with the previous data
  // =========================================================
  int i = 0;
  for (Edge* edge : clause->neighborhood) {
    if (edge->enabled && edge->variable->value == Unassigned) {
      // ---------------------------------------------
      // Calculate new survey from sub survey products
      // ---------------------------------------------
      double newSurvey;
      // If there where no subSurveys == 0, proceed normaly
      if (!zeros) newSurvey = allSubSurveys / subSurveys[i];
      // If this subsurvey is the only one that is 0
      // consider the new survey as the total subSurveys
      else if (zeros == 1 && subSurveys[i] < ZERO)
        newSurvey = allSubSurveys;
      // If there where more that one subSurveys == 0, the new survey is 0
      else
        newSurvey = 0.0;

      // ----------------------------------------------------
      // Update the variable subproducts with new survey info
      // ----------------------------------------------------
      Variable* var = edge->variable;
      // If edge is negative update positive subproduct
      if (edge->negated) {
        // If previous survey != 1 (with an epsilon margin)
        if (1.0 - edge->survey > ZERO) {
          // If new survey != 1, update the sub product with the difference
          if (1.0 - newSurvey > ZERO)
            var->ps *= ((1.0 - newSurvey) / (1.0 - edge->survey));
          // If new survey == 1, update the subproduct by remove the old survey
          // and keep track of the new survey == 1 (pzero++)
          else {
            var->ps /= (1.0 - edge->survey);
            var->pzero++;
          }
        }
        // If previous survey == 1
        else {
          // If new survey == 1, don't do anything (both surveys are the same)
          // If new survey != 1, update subproduct
          if (1.0 - newSurvey > ZERO) {
            var->p *= (1.0 - newSurvey);
            var->pzero--;
          }
        }
      }
      // If edge is positive, update negative subproduct
      else {
        // If previous survey != 1 (with an epsilon margin)
        if (1.0 - edge->survey > ZERO) {
          // If new survey != 1, update the sub product with the difference
          if (1.0 - newSurvey > ZERO)
            var->ns *= ((1.0 - newSurvey) / (1.0 - edge->survey));
          // If new survey == 1, update the subproduct by remove the old survey
          // and keep track of the new survey == 1 (pzero++)
          else {
            var->ns /= (1.0 - edge->survey);
            var->nzero++;
          }
        }
        // If previous survey == 1
        else {
          // If new survey == 1, don't do anything (both surveys are the same)
          // If new survey != 1, update subproduct
          if (1.0 - newSurvey > ZERO) {
            var->ns *= (1.0 - newSurvey);
            var->nzero--;
          }
        }
      }

      // ----------------------------------------------------
      // Store new survey and update max clause converge diff
      // ----------------------------------------------------
      double edgeConvDiff = std::abs(edge->survey - newSurvey);
      if (maxConvDiffClause < edgeConvDiff)
        maxConvDiffClause = std::abs(edgeConvDiff);

      edge->survey = newSurvey;
      i++;
    }
  }

  return maxConvDiffClause;
}

bool SolverSP::assignVariable(Variable* var, lbool value){
	if(var->value != Unassigned && var->value != value){
		return false;
	}
	
	var->setValue(value);
	return clean(var);
}

bool SolverSP::clean(Variable* var){
	for(Edge* edge : var->neighborhood){
		if(edge->enabled){
			if((edge->negated && var->value == False)
				||
			   (!edge->negated && var->value == True)){
				edge->function->satisfy();
			}
			else{
				edge->Dissable();
				if(!unitPropagation(edge->function))
					return false;
			}
		}
	}

	return true;
}


bool SolverSP::unitPropagation(Function* function){
	bool ok = true;
	vector<Edge*> edges = function->enabledNeighborhood();

	if(edges.size() == 1){
		lbool value = True;
		if (edges[0]->negated) value = False;
		assignVariable(edges[0]->variable, value);
	}
	else if(edges.size() == 0)
		ok = false;

	return ok;
}




