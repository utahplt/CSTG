#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <string.h>
#include <unistd.h>

using namespace std;


#define tostring( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

const int MAXEDGE = 20;
const int MAXNODE = 20;

class Edge{
public:
	string source;
	string dest;
	//double start;
	//double end;
	Edge(const string &source, const string &dest, double start = -1, double end = -1){
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

string splitToken, line;
vector< map<string, int>*> levelCounter;
vector<string> queue;
map<Edge, int> edges;
map<Edge, vector<double> > edgesGrow;
map<string, int> nodes;
map<string, vector<double> > nodesGrow;
map<string, int> nodesID;
int ignore1 = 0;
double maxEdge = 0;
double maxNode = 0;

void printDotFile(const char *DOTFILENAME){
	// dot file
	FILE *pFile = fopen(DOTFILENAME, "w");
	fprintf(pFile, "digraph trace {\nnode [shape=box];\n");
	for (map<Edge, int>::const_iterator it = edges.begin(); it != edges.end(); ++it){
		if (it->second > ignore1)
			fprintf(pFile, "  \"%s\" -> \"%s\" [label=%d] [penwidth=%d] [color=gray];\n", it->first.source.c_str(), it->first.dest.c_str(), it->second, (int)(log(it->second)*MAXEDGE/maxEdge+1));
		//cout << "\""<< it->first.first << "\"" << " -> " << "\"" << it->first.second << "\"" << " [label=" << it->second << "];"  << endl;
	}
	/*for (int i = 0; i < levelCounter.size(); ++i){
		fprintf(pFile, "  { rank = same;");
		for (map<string, int>::iterator it = levelCounter[i]->begin(); it != levelCounter[i]->end(); ++it)
			fprintf(pFile, " \"%d: %s\";", i, it->first.c_str());
		fprintf(pFile, " }\n");
	}*/
	fprintf(pFile, "}");
	fclose(pFile);
}

void printGexfFile(const char *GEXFFILENAME){
	FILE *pFile = fopen(GEXFFILENAME, "w");
	fprintf(pFile, "<gexf xmlns=\"http://www.gexf.net/1.2draft\" version=\"1.2\">\n");
	fprintf(pFile, "\t<graph mode=\"dynamic\">\n");
	fprintf(pFile, "\t\t<attributes class=\"node\" mode=\"dynamic\">\n");
	fprintf(pFile, "\t\t\t<attribute id=\"weight\" title=\"Weight\" type=\"float\"/>\n");
	fprintf(pFile, "\t\t</attributes>\n");
	fprintf(pFile, "\t\t<attributes class=\"edge\" mode=\"dynamic\">\n");
	fprintf(pFile, "\t\t\t<attribute id=\"weight\" title=\"Weight\" type=\"float\"/>\n");
	fprintf(pFile, "\t\t</attributes>\n");
	fprintf(pFile, "\t\t<nodes>\n");
	for (map<string, int>::iterator it = nodesID.begin(); it != nodesID.end(); ++it){
		fprintf(pFile, "\t\t\t<node id=\"%d\" label=\"%d\" start=\"%lf\" >\n", it->second, it->second, nodesGrow[it->first].front());
		fprintf(pFile, "\t\t\t\t<attvalues>\n");
        for (int i = 0; i < nodesGrow[it->first].size(); ++i)
          fprintf(pFile, "\t\t\t\t\t<attvalue for=\"weight\" value=\"%d\" start=\"%lf\" /> \n", i+1, nodesGrow[it->first][i]);
        fprintf(pFile, "\t\t\t\t</attvalues>\n");
		fprintf(pFile, "\t\t\t</node>\n");
	}
	fprintf(pFile, "\t\t</nodes>\n");
	fprintf(pFile, "\t\t<edges>\n");
	for (map<Edge, int>::iterator it = edges.begin(); it != edges.end(); ++it){
		fprintf(pFile, "\t\t\t<edge source=\"%d\" target=\"%d\" start=\"%lf\" >\n", nodesID[it->first.source], nodesID[it->first.dest], edgesGrow[it->first].front());
		fprintf(pFile, "\t\t\t\t<attvalues>\n");
		for (int i = 0; i < edgesGrow[it->first].size(); ++i)
			fprintf(pFile, "\t\t\t\t\t<attvalue for=\"weight\" value=\"%d\" start=\"%lf\" /> \n", i+1, edgesGrow[it->first][i]);
		fprintf(pFile, "\t\t\t\t</attvalues>\n");
		fprintf(pFile, "\t\t\t</edge>\n");
	}
	fprintf(pFile, "\t\t</edges>\n");
	fprintf(pFile, "\t</graph>\n");
	fprintf(pFile, "</gexf>\n");
        fclose(pFile);
}

void processStack(double startTime){
	for (int i = levelCounter.size(); i < queue.size(); ++i)
		levelCounter.push_back(new map<string, int>());
	for (int i = queue.size()-1, j = 0; i >= 0; --i)
		(*levelCounter[j++])[queue[i]]++;
	for (int i = queue.size()-1, j = 0; i > 0; --i,  ++j){
		//edges[make_pair(tostring(j)+": "+queue[i], tostring(j+1)+": "+queue[i-1])]++;
		Edge e(queue[i], queue[i-1]);
		if (edges.find(e) == edges.end())
			edgesGrow[e].push_back(startTime);
		else if ( (int)(log(edges[e]+1)*MAXEDGE/maxEdge+1) > (int)(log(edges[e])*MAXEDGE/maxEdge+1) )
			edgesGrow[e].push_back(startTime);
		maxEdge = max(maxEdge, log(edges[e]++));

		string &n = queue[i];
		if (nodes.find(n) == nodes.end())
			nodesGrow[n].push_back(startTime);
		else if ( (int)(log(nodes[n]+1)*MAXNODE/maxNode+1) > (int)(log(nodes[n])*MAXNODE/maxNode+1) )
			if (nodesGrow[n].back() != startTime)
				nodesGrow[n].push_back(startTime);
		maxNode = max(maxNode, log(nodes[n]++));
		if (nodesID.find(n) == nodesID.end())
			nodesID[n] = nodesID.size();

		string &n2 = queue[i-1];
		if (nodes.find(n2) == nodes.end()){
			nodesGrow[n2].push_back(startTime);
			nodes[n2]++;
		}
		if (nodesID.find(n2) == nodesID.end())
			nodesID[n2] = nodesID.size();
	}
	queue.clear();
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
	char tmp[1000];
        char DOTFILENAME[100];
        char GEXFFILENAME[100];
	char PSFILENAME[100];
	char PDFFILENAME[100];
	ifstream inFile;

	// configuration
	if (argc != 2){
		inFile.open(argv[1], std::ifstream::in);
		sscanf(argv[2], "%s", DOTFILENAME);
		strcat(DOTFILENAME, ".dot");
		sscanf(argv[2], "%s", GEXFFILENAME);
		strcat(GEXFFILENAME, ".gexf");
		sscanf(argv[2], "%s", PSFILENAME);
		strcat(PSFILENAME, ".ps");
		sscanf(argv[2], "%s", PDFFILENAME);
		strcat(PDFFILENAME, ".pdf");
	}
	else{
		printf("Usage: TracerSum input_file output_file\n");
		return 1;
	}

	// read input
	double startTime = 0;
	getline(inFile, line);
	sscanf(line.c_str(), "%s %lf", tmp, &startTime);
	splitToken = tmp;
	while (getline(inFile, line)){
		sscanf(line.c_str(), "%s", tmp);
		if (tmp == splitToken){
			processStack(startTime);
			sscanf(line.c_str(), "%s %lf", tmp, &startTime);
			continue;
		}
		queue.push_back(line);
	}
	processStack(startTime);

	// intermediate output
	for (int i = 0; i < levelCounter.size(); ++i){
		for (map<string, int>::iterator it = levelCounter[i]->begin(); it != levelCounter[i]->end(); ++it){
			if (it->second > ignore1){
				for (int j = 0; j < i; ++j)
					cout << " ";
				cout << it->first << " -> " << it->second << endl;
			}
		}
	}

	// output files
	printDotFile(DOTFILENAME);
	printGexfFile(GEXFFILENAME);
	printPSFile(DOTFILENAME, PSFILENAME);
	printPDFFile(PSFILENAME, PDFFILENAME);

	return 0;
}
