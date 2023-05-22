#include <cstdio>
#include <map>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <cstring>
#include <unistd.h>

using namespace std;

class Edge{
public:
	string source;
	string dest;
	//double start;
	//double end;
	Edge(const string &source, const string &dest){
		this->source = source;
		this->dest = dest;
		//this->start = start;
		//this->end = end;
	}
	bool operator < (const Edge &b) const {
		if (source != b.source)
			return source < b.source;
        	return dest < b.dest;
	}
};

const int MAXEDGE = 20;
double maxEdge = 1;
map<Edge, int> edges1, edges2, edges3;
set<string> nodes1, nodes2;


int readEdges(map<Edge, int> &edges, set<string> &nodes, char *fileName){
	char line[1000], node1[1000], node2[1000];
	FILE *pFile = fopen(fileName, "r");
	stringstream ss;
	fgets(line, 1000, pFile);
	fgets(line, 1000, pFile);
	while(fgets(line, 1000, pFile) != NULL){
		int label;
		ss << line;
		if (sscanf(line, "  \"%[^\"]\" -> \"%[^\"]\" [label=%d", node1, node2, &label) != 3)
			break;
		string n1(node1);
		string n2(node2);
		edges[Edge(n1,n2)] = label;
		nodes.insert(n1);
		nodes.insert(n2);
		//printf("%s -> %s = %d\n", n1.c_str(), n2.c_str(), label);
	}
	if (fgets(line, 1000, pFile) != NULL){
		printf("Error reading input files. This program currently does not support some dot graphs.\n");
		return 1;
	}
	fclose(pFile);
	return 0;
}

void printDotFile(const char *DOTFILENAME){
	// dot file
	FILE *pFile = fopen(DOTFILENAME, "w");
	//FILE *pFile = stdout;
	fprintf(pFile, "digraph trace {\nnode [shape=box];\n");

	vector<string> tmp(nodes1.size()+nodes2.size());
	vector<string>::iterator tmp_it;

	tmp_it = set_difference(nodes1.begin(), nodes1.end(), nodes2.begin(), nodes2.end(), tmp.begin());
	for (vector<string>::iterator it = tmp.begin(); it!=tmp_it; ++it)
		fprintf(pFile, "  \"%s\" [color = green]\n", (*it).c_str());

	tmp_it = set_difference(nodes2.begin(), nodes2.end(), nodes1.begin(), nodes1.end(), tmp.begin());
	for (vector<string>::iterator it = tmp.begin(); it!=tmp_it; ++it)
		fprintf(pFile, "  \"%s\" [color = red]\n", (*it).c_str());

	for (map<Edge, int>::iterator it = edges1.begin(); it != edges1.end(); ++it){
		int label1 = it->second, label2 = 0;
		if (edges2.find(it->first) != edges2.end())
			label2 = edges2[it->first];
		int label = label1 - label2;
		if (label > 0)
			fprintf(pFile, "  \"%s\" -> \"%s\" [label=%d] [penwidth=%d] [color=green];\n", it->first.source.c_str(), it->first.dest.c_str(), label, (int)(log(label)*MAXEDGE/maxEdge+1));
		else if (label < 0)
			fprintf(pFile, "  \"%s\" -> \"%s\" [label=%d] [penwidth=%d] [color=red];\n", it->first.source.c_str(), it->first.dest.c_str(), label, (int)(log(-label)*MAXEDGE/maxEdge+1));
		else
			fprintf(pFile, "  \"%s\" -> \"%s\" [label=%d] [penwidth=%d] [color=gray];\n", it->first.source.c_str(), it->first.dest.c_str(), 0, 1);
		}

	for (map<Edge, int>::iterator it = edges2.begin(); it != edges2.end(); ++it){
		int label = it->second;
		if (edges1.find(it->first) == edges1.end())
			fprintf(pFile, "  \"%s\" -> \"%s\" [label=%d] [penwidth=%d] [color=red];\n", it->first.source.c_str(), it->first.dest.c_str(), -label, (int)(log(label)*MAXEDGE/maxEdge+1));
	}

	fprintf(pFile, "}");
	fclose(pFile);
}

void findMaxEdge(){
	for (map<Edge, int>::iterator it = edges1.begin(); it != edges1.end(); ++it)
		maxEdge = max(maxEdge, log(abs(it->second - edges2[it->first])));
	for (map<Edge, int>::iterator it = edges2.begin(); it != edges2.end(); ++it)
		maxEdge = max(maxEdge, log(abs(it->second - edges1[it->first])));
}

void printPSFile(char *DOTFILENAME, char *PSFILENAME){
	char cmd[1000], pwd[1000];
	getwd(pwd);
	sprintf(cmd, "dot -Tps2 %s/%s -o %s/%s", pwd, DOTFILENAME, pwd, PSFILENAME);
	printf("%s\n", cmd);
	system(cmd);
}

void printPDFFile(char *PSFILENAME, char *PDFFILENAME){
	char cmd[1000], pwd[1000];
        getwd(pwd);
	sprintf(cmd, "ps2pdf %s/%s %s/%s", pwd, PSFILENAME, pwd, PDFFILENAME);
	printf("%s\n", cmd);
	system(cmd);
}

int main(int argc, char *argv[]){
	char DOTFILENAME[100];
	char PSFILENAME[100];
	char PDFFILENAME[100];

	// read command input
	if (argc < 4){
		printf("Usage: graphDiff file1.dot file2.dot output\n");
		return 1;
	}
	sscanf(argv[3], "%s", DOTFILENAME);
	strcat(DOTFILENAME, ".dot");
	sscanf(argv[3], "%s", PSFILENAME);
	strcat(PSFILENAME, ".ps");
	sscanf(argv[3], "%s", PDFFILENAME);
	strcat(PDFFILENAME, ".pdf");

	// read graphs
	if (readEdges(edges1, nodes1, argv[1]))
		return 1;
	if (readEdges(edges2, nodes2, argv[2]))
		return 1;

	findMaxEdge();

	// print output
	printDotFile(DOTFILENAME);
	printPSFile(DOTFILENAME, PSFILENAME);
	printPDFFile(PSFILENAME, PDFFILENAME);

	return 0;
}
