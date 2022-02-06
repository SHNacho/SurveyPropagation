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
		vector<Edge*>& getEdges(){return edges;}
		vector<Variable*>& getVariables(){return variables;}
		vector<Function*>& getFunctions(){return functions;}
		vector<Edge*> getEnabledEdges();
		/**
		 * Devuelve el número de variables no asignadas
		 **/
		int unassignedVars(){return unassigned_vars;}
		/**
		 * Añade una arista al grafo
		 **/
		void addEdge(Variable* var, Function* func, bool neg);
		/**
		 * Dada una variable, la cual ha sido fijada a un valor
		 * concreto, elimina del grafo todas las aristas que
		 * contengan dicha variable así como las aristas que 
		 * contengan clausulas que contengan la variable.
		 * Además elimina los vecinos de las cláusulas satisfechas
		 * y elimina solo el vecino que apunta a la variable de las
		 * cláusulas no satisfechas
		 **/
		void clean(Variable* fixed_var);
		void assignVar(Variable* assigned, bool val);
		/**
		 * Dada una variable le asigna un valor, el cual
		 * dependerá de los sesgos calculados para dicha
		 * variable
		 **/
		void assignVar(Variable* assigned);
	
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
