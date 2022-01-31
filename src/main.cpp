#include <iostream>
#include "random.h"
#include "graph.h"

using namespace std;

int main (int argc, char *argv[]){
	Set_random(29);
	string file = argv[1];
	
	Graph g(file);
	return 0;
}
