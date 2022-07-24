#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>
#include "SP.h"
#include "graph.h"
#include "random.h"


using namespace std;

SolverSP::SolverSP(){}


result SolverSP::SID(Graph* graph, int t_max, float precision, float f){

	totalIt = 0;
	vector<Edge*> edges = graph->enabledEdges();

	int unassigned_vars = graph->unassigned_vars;
	int n_vars_fix = unassigned_vars * f;
	n_vars_fix = (1 < n_vars_fix) ? n_vars_fix : 1;

	while(true){
		totalIt++;
		cout << totalIt << endl;
		// Llamamos a la rutina SP
		result result_sp = surveyPropagation(graph, t_max, precision);
		if(result_sp == SP_UNCONVERGED) return result_sp;

		// Comprobamos si el problema es trivial
		bool trivial = true;
		for(int i = 0; i < edges.size() && trivial; ++i){
			//cout << edges[i]->survey << endl;
			if (edges[i]->survey >= ZERO)
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

				// Recalculamos los biases ya que la anterior asignación puede
				// provocar que se modifiquen
    		  	computeBias(var);


				// Calculamos el valor que se le va a asignar a la variable en función
				// de sus sesgos
    		  	lbool val= var->negativeBias > var->positiveBias ? False : True;
				
				// Asignamos la variable
    		  	if (!assignVariable(var, val)) {
					// Error si la variable ya ha sido asignada
					// cout << "Contradicción aqui" << endl;
    		  	  	return CONTRADICTION;
    		  	}
    		}
    	} else {
			//TODO hacer walksat
			cout << "Walksat" << endl;
			return WALKSAT;
		}

		//TODO Comprobar si se ha satisfecho la fórmula
	}
}

result SolverSP::surveyPropagation(Graph* graph, int t_max, float precision){

	randomInit(graph);

	auto rng = std::default_random_engine {};

	// Calculamos los subproductos necesarios
  	computeSubProducts(graph);
  	for (int i = 0; i < t_max; i++) {

  	  	vector<Clause*> unsatisfiedClauses = graph->unsatisfiedClauses();
  	  	shuffle(unsatisfiedClauses.begin(), unsatisfiedClauses.end(), rng);

  	  	// Calculamos los surveys
  	  	double maxConvergeDiff = 0.0;
  	  	for (Clause* clause : unsatisfiedClauses) {
  	  	  	double maxConvDiffInClause = SP_UPDATE(clause);

  	  	  	// Guardamos la mayor convergencia
  	  	  	if (maxConvDiffInClause > maxConvergeDiff)
  	  	  	  	maxConvergeDiff = maxConvDiffInClause;
  	  	}

  	  	// Comprobar si converge
  	  	if (maxConvergeDiff <= precision) {
  	  	  	return SP_CONVERGED;
  	  	}
  	}
	cout << "No converge" << endl;
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

double SolverSP::SP_UPDATE(Clause* clause){
	double maxConvDiffClause = 0.0;
	int zeros = 0;
	double allSubSurveys = 1.0;
  	vector<double> subSurveys;

  // ==================================================================
  // Calculate subProducts of all literals and keep track of wich are 0
  // ==================================================================
  for (Edge* edge : clause->neighborhood) {
    Variable* var = edge->variable;
    if (edge->enabled && var->value == Unassigned) {
      double m, p, wn, wt;

      // Si la arista está negada:
      if (edge->negated) {
        m = var->nzero ? 0.0 : var->ns;
        if (var->pzero == 0)
          p = var->ps / (1.0 - edge->survey);
        else if (var->pzero == 1 && (1.0 - edge->survey) < ZERO)
          p = var->ps;
        else
          p = 0.0;

        wn = p * (1.0 - m);
        wt = m;
      }
      // Si la arista no está negada
      else {
        p = var->pzero ? 0.0 : var->ps;
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
            var->ps *= (1.0 - newSurvey);
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
	if(var->value == Unassigned){
		// cout << "Unassigned" << endl;
		var->setValue(value);
		return clean(var);
	} else if (var->value == value){
		return clean(var);
	} else{
		// cout << "Asignación ya hecha" << endl;
		return false;
	}
}

bool SolverSP::clean(Variable* var){
	for(Edge* edge : var->neighborhood){
		if(edge->enabled){
			if((edge->negated && var->value == False)
				||
			   (!edge->negated && var->value == True)){
				edge->clause->satisfy();
			}
			else{
				edge->Dissable();
				if(!unitPropagation(edge->clause)){
					// cout << "Error UP" << endl;
					return false;
				}
			}
		}
	}

	return true;
}


bool SolverSP::unitPropagation(Clause* clause){
	bool ok = true;

	if(clause->satisfied) return ok;
	
	vector<Edge*> edges = clause->enabledNeighborhood();

	int n_enabled = 0;

	for(Edge* e : clause->neighborhood){
		if(e->enabled)
			n_enabled++;
	}

	if(n_enabled == 1){
		lbool value = True;
		if (edges[0]->negated) value = False;
		if (!assignVariable(edges[0]->variable, value))
			ok = false;
	}
	else if(n_enabled == 0)
		ok = false;

	return ok;
}

void SolverSP::computeBias(Variable* var){
	double pp, pn, pnull;
	pp = var->nzero ? 0 : var->ns;
	pn = var->pzero ? 0 : var->ps;

	var->nullBias = pp * pn;
	var->negativeBias = pn - var->nullBias;
	var->positiveBias = pp - var->nullBias;

	double sum = var->nullBias + var->negativeBias + var->positiveBias;

	var->positiveBias /= sum;
	var->negativeBias /= sum;
	var->nullBias = 1 - var->positiveBias - var->negativeBias;
}

void SolverSP::randomInit(Graph* graph){
	for(Edge* e : graph->enabledEdges()){
		e->initRandomSurvey();
	}
}



