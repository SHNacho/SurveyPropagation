#include <fstream>
#include <ios>
#include <string>
#include <numeric>
#include <cstdlib>
#include <algorithm>

#include "graph.h"
#include "variable.h"
#include "function.h"
#include "edge.h"


using namespace std;

Graph::Graph(string file){
	ifstream ifs;
	// n -> nº de variables
	// m -> nº de cláusulas
	// k -> nº variables por cláusula
	int n, m, k;

	ifs.open(file, ifstream::in);

	if(ifs.is_open()){
	}

	string line;
	getline(ifs, line, '=');
	// Obtenemos el número de variables
	getline(ifs, line);
	n = stoi(line); // Pasa char a int
	getline(ifs, line, '=');
	// Obtenemos el número de cláusulas
	getline(ifs, line);
	m = stoi(line); // Pasa char a int
	getline(ifs, line, '=');
	// Obtenemos el número de variables por cláusula
	getline(ifs, line);
	k = stoi(line); // Pasa char a int
	getline(ifs, line);
	getline(ifs, line);

	this->unassigned_vars = n;
	// reservamos espacio para las variables, clausulas y aristas
	this->variables.reserve(n);
	this->functions.reserve(m);
	this->edges.reserve(m * k);

	// inicializamos las variables y cláusulas
	// rellenando el vector de variables de 1 a n
	// y el de clausulas de 0 a m
	initVariables(n);
	initFunctions(m);

	// Pasamos a leer las clausulas
	for(int i = 0; i < m; ++i){
		int var;
		do{
			bool neg = true;
			
			ifs >> var;
			if(var > 0)	neg = false;
			
			if(var != 0){
				int var_pos = abs(var) - 1;
				addEdge(variables[var_pos], functions[i], neg);
			}
		} while(var != 0);
	}

	ifs.close();

	//cout << "Número de nodos variable: " << variables.size() << endl
	//	 << "Número de nodos cláusula: " << functions.size()  << endl
	//	 << "Número de aristas:        " << edges.size() << endl;
}

//----------------------------------------------//
void Graph::addEdge(Variable* var, Function* func, bool neg){
	Edge* e = new Edge(var, func, neg);
	edges.push_back(e);
	var->addNeighbor(e);
	func->addNeighbor(e);
}

//----------------------------------------------//
vector<Variable*> Graph::unassignedVars(){
	vector<Variable*> unassigned_vars;
	for(Variable* v : variables)
		if(v->value == Unassigned)
			unassigned_vars.push_back(v);
	return unassigned_vars;
}

//----------------------------------------------//
vector<Edge*> Graph::enabledEdges(){
	vector<Edge*> enabled_edges;
	for(Edge* e : edges)
		if(e->enabled)
			enabled_edges.push_back(e);
	return enabled_edges;
}

//----------------------------------------------//
vector<Function*> Graph::unsatisfiedFunctions(){
	vector<Function*> enabled_functions;
	for(Function* f : functions)
		if(!f->satisfied)
			enabled_functions.push_back(f);
	return enabled_functions;
}
//----------------------------------------------//
void Graph::assignVar(Variable* var, int val){
	if(val == -1)
		var->fix();
	else
		var->setValue(val);

	unassigned_vars--;
	//cout << "Variable " << var->Id << " asignada" << endl;
}

//----------------------------------------------//
void Graph::initVariables(int n_variables){
	for(int i = 1; i <= n_variables; ++i){
		this->variables.push_back(new Variable(i));
	}
}

//----------------------------------------------//
void Graph::initFunctions(int n_functions){
	for(int i = 0; i < n_functions; ++i){
		this->functions.push_back(new Function(i));
	}
}

//----------------------------------------------//
Graph::~Graph() {
  for (Function* clause : functions) delete clause;
  for (Variable* variable : variables) delete variable;
  for (Edge* edge : edges) delete edge;
}
















