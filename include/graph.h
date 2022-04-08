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
	public:
		//==========================//
		// Declaración de variables //
		//==========================//
		vector<Edge*> edges;
		vector<Variable*> variables;
		vector<Function*> functions;

		int unassigned_vars;

		//==========================//
		// Declaración de funciones //
		//==========================//
		Graph(string file);
		~Graph();
		/**
		 * Devuelve el número de variables no asignadas
		 **/
		vector<Variable*> unassignedVars();
		/**
		 * Devuelve las aristas que aún continuan en el grafo
		 **/
		vector<Edge*> enabledEdges();		
		/**
		 * Devuelve las cláusulas que aún no han sido satisfechas
		 **/
		vector<Function*> unsatisfiedFunctions();		
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
		/**
		 * Dada una variable le asigna un valor, el cual
		 * dependerá de los sesgos calculados para dicha
		 * variable
		 **/
		void assignVar(Variable* assigned, int val = -1);
	
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
