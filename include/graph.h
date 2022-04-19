// Ignacio Sánchez Herrera

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <string>
#include "variable.h"
#include "edge.h"
#include "function.h"

using namespace std;

/**
 * Grafo que representa el problema SAT
 */
class Graph 
{
	private:
		vector<Edge*> edges;
		vector<Variable*> variables;
		vector<Function*> functions;

		int enabled_edges;
		int unassigned_vars;

	public:
		Graph(string file);
		Graph(vector<Edge*> edges, vector<Variable*> variables, vector<Function*> clauses);
		vector<Edge*>& getEdges(){return edges;}
		vector<Variable*>& getVariables(){return variables;}
		vector<Function*>& getFunctions(){return functions;}
		vector<Edge*> getEnabledEdges();
		vector<Function*> getEnabledFunctions();
		vector<Variable*> getUnassignedVariables();
		/**
		 * Devuelve el número de variables no asignadas
		 **/
		int unassignedVars(){return unassigned_vars;}
		/**
		 * Añade una arista al grafo
		 **/
		void addEdge(Variable* var, Function* func, bool neg);
		void addEdge(Edge edge);
		/**
		 * Añade una cláusula al grafo
		 **/
		void addFunction(Function function);
		/**
		 * Añade una variable al grafo
		 **/
		void addVariable(Variable variable);

		
		bool clean(Variable* fixed_var);
		void assignVar(Variable* assigned, bool val);
		Graph* simplifiedFormula();
		/**
		 * Calcula el número de cláusulas que pasan de estar
		 * satisfechas a insatisfechas al realizar un 'flip'
		 * sobre la variable 'var'
		 **/
		int Break(Variable* var);
		/**
		 * Valida si una asignación es válida
		 **/
		bool validate(vector<Variable*> asignacion);
	
	private:
		/**
		 * Inicializa el vector de variables
		 **/
		void initVariables(int n_variables);
		/**
		 * Inicializa el vector de cláusulas
		 **/
		void initFunctions(int n_functions);
};

# endif //GRAPH_H
