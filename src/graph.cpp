#include <fstream>
#include <ios>
#include <string>
#include <numeric>
#include <cstdlib>
#include <algorithm>
#include <set>

#include "graph.h"
#include "variable.h"
#include "function.h"
#include "edge.h"
#include "SP.h"


using namespace std;

Graph::Graph(string file){
	string nombre = file;
	ifstream ifs;
	// n -> nº de variables
	// m -> nº de cláusulas
	// k -> nº variables por cláusula
	int n, m, k;

	ifs.open(nombre, ifstream::in);

	if(!ifs.is_open()){
		cout << "Archivo no encontrado" << endl;
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
	// Obtenemos el número de variables por cláusula
	getline(ifs, line);
	getline(ifs, line);
	getline(ifs, line);

	this->unassigned_vars = n;
	// reservamos espacio para las variables, clausulas y aristas
	this->variables.reserve(n);
	this->functions.reserve(m);
	this->edges = vector<Edge*>();
	this->enabled_edges = 0;

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
				this->enabled_edges++;
			}
		} while(var != 0);
	}

	ifs.close();

	//cout << "Número de nodos variable: " << variables.size() << endl
	//	 << "Número de nodos cláusula: " << functions.size()  << endl
	//	 << "Número de aristas:        " << edges.size() << endl;
}

Graph::Graph(vector<Edge*> edges, vector<Variable*> variables, vector<Function*> clauses){
	this->functions = clauses;
	this->variables = variables;

	for(Edge* edge : edges){
		addEdge(edge->getVariable(), edge->getFunction(), edge->isNegated());
	}

	this->enabled_edges = edges.size();
	this->unassigned_vars = variables.size();
}

//----------------------------------------------//
vector<Edge*> Graph::getEnabledEdges(){
	vector<Edge*> v_enabled;
	
	for(Edge* e : edges){
		if(e->isEnabled())
			v_enabled.push_back(e);
	}

	return v_enabled;
}

//----------------------------------------------//
vector<Function*> Graph::getEnabledFunctions(){
	vector<Function*> v_functions;
	for(Function* f : functions){
		if(!f->isSatisfied()){
			v_functions.push_back(f);	
		}
	}

	return v_functions;
}

//----------------------------------------------//
vector<Variable*> Graph::getUnassignedVariables(){
	vector<Variable*> v_vars;
	for(Variable* v : variables){
		if (!(v->isAssigned())){
			v_vars.push_back(v);
		}
	}

	return v_vars;
}

//----------------------------------------------//
void Graph::addEdge(Variable* var, Function* func, bool neg){
	Edge* e = new Edge(var, func, neg);
	edges.push_back(e);
	var->addNeighbor(e);
	func->addNeighbor(e);
}

//----------------------------------------------//
void Graph::addEdge(Edge edge){
	edges.push_back(&edge);
}

//----------------------------------------------//
void Graph::addFunction(Function function){
	functions.push_back(&function);
}

//----------------------------------------------//
void Graph::addVariable(Variable variable){
	variables.push_back(&variable);
}

//----------------------------------------------//
void Graph::assignVar(Variable* var, bool val){
	bool ok = true;
	var->setValue(val);
	unassigned_vars--;
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
int Graph::Break(Variable* var){
	
}

//----------------------------------------------//
bool compareId(Variable* v1, Variable* v2){
	return (v1->getId() < v2->getId());
}

Graph* Graph::simplifiedFormula(){
	auto cmp = [](Variable* a, Variable* b) { return (a->getId() < b->getId());};
	auto cmp_c = [](Function* a, Function* b) { return (a->getId() < b->getId());};
	vector<Edge*> simp_edges = vector<Edge*>();
	set<Variable*, decltype(cmp)> simp_vars(cmp);
	set<Function*, decltype(cmp_c)> simp_clauses(cmp_c);

	for(Edge* edge : edges){
		if(edge->isEnabled()){
			Function* new_clause = nullptr;
			Variable* new_var = nullptr;
			Variable* var = edge->getVariable();
			Function* clause = edge->getFunction();
			set<Function*>::iterator index_clause = simp_clauses.find(clause);
			set<Variable*>::iterator index_var = simp_vars.find(var);
			if(index_clause == simp_clauses.end()){
				Function* new_clause = new Function(clause->getId());
				index_clause = simp_clauses.insert(new_clause).first;
			}
			if(index_var == simp_vars.end()){
				Variable* new_var = new Variable(var->getId());
				index_var = simp_vars.insert(new_var).first;
			}
			Edge* new_edge = new Edge(*index_var, *index_clause, edge->isNegated());
			simp_edges.push_back(new_edge);
		}
	}

	vector<Variable*> v_variables(simp_vars.size());
	vector<Function*> v_clauses(simp_clauses.size());
	copy(simp_clauses.begin(), simp_clauses.end(), v_clauses.begin());
	copy(simp_vars.begin(), simp_vars.end(), v_variables.begin());


	Graph* fg = new Graph(simp_edges, v_variables, v_clauses);
	return fg;
}

bool Graph::validate(vector<Variable*> asignacion){
	sort(asignacion.begin(), asignacion.end(), compareId);
	bool valida = true;
	int counter = 0;

	for(Function* f : functions){
		bool satisfied = false;
		for(Edge* e : f->getNeighborhood()){
			Variable* var = e->getVariable();
			int id = var->getId();
			bool var_value =var->getValue();
			if(e->isNegated() != var_value && var->isAssigned()) satisfied = true;
		}
		if(satisfied){
			counter++;
		}
	}

	cout << "Cláusulas satisfechas: " << counter << " de " << functions.size() << endl;
	if(counter == functions.size())
		return true;
	else
		return false;
}
















