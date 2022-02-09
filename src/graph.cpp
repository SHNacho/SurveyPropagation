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
			}
		} while(var != 0);
	}

	ifs.close();

	//cout << "Número de nodos variable: " << variables.size() << endl
	//	 << "Número de nodos cláusula: " << functions.size()  << endl
	//	 << "Número de aristas:        " << edges.size() << endl;
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
		if(f->isEnabled()){
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
	var->setValue(val);
	unassigned_vars--;
}

void Graph::assignVar(Variable* var){
	var->fix();
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
void Graph::clean(Variable* fixed_var){
	bool val = fixed_var->getValue();
	for(Edge* e : fixed_var->getNeighborhood()){
		if(e->isEnabled()){
			if(val != e->isNegated())
				e->getFunction()->dissable();
			else 
				e->dissable();
		}
	}
}


//----------------------------------------------//
Graph Graph::simplifiedFormula(){
	ofstream outfile("data/simplified.txt");
	int n = 0,
		m = 0;

	outfile << "c" << endl;
	for(Variable* v : variables){
		if(!(v->isAssigned())){
			++n;
		}
	}
	outfile << "c\tvalue n = " << variables.size() << endl;
	for(Function* f : functions){
		if(f->isEnabled()){
			m++;
		}
	}
	outfile << "c\tvalue m = " << m << endl;
	outfile << "c" << endl;
	outfile << "p" << endl;
	for(Function* f : functions){
		if(f->isEnabled()){
			vector <Edge*> neigh = f->getEnabledNeighborhood();
			for(Edge* e : neigh){
				if(e->isEnabled()){
					if(e->isNegated())
						outfile << "-" << e->getVariable()->getId() << " ";
					else
						outfile << e->getVariable()->getId() << " ";
				}
			}
			outfile << "0" << endl;
		}
	}

	outfile.close();

	Graph g("data/simplified.txt");

	return g;
}

//----------------------------------------------//
int Graph::Break(Variable* var){
	
}

//----------------------------------------------//
bool compareId(Variable* v1, Variable* v2){
	return (v1->getId() < v2->getId());
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
			bool var_value = asignacion[id-1]->getValue();
			if(e->isNegated() != var_value && asignacion[id-1]->isAssigned()) satisfied = true;
		}
		if(satisfied){
			counter++;
		}
	}

	cout << "Cláusulas satisfechas: " << counter << endl;
	if(counter == functions.size())
		return true;
	else
		return false;
}
















