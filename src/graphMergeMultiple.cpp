#include <string>
#include <iostream>
#include <stdio.h>
#include <cstdlib>

using namespace std;

std::string exec(const char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[1280];
    std::string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, 1280, pipe) != NULL){
		cout << buffer;
    		result += buffer;
	}
    }
    pclose(pipe);
    return result;
}

int main(int argc, char *argv[]){
	if (argc < 3){
		printf("Usage: graphMergeMultiple output files*\n");
		return 1;
	}
	string gm = "graphMerge ";
	if (system("command -v graphMerge >/dev/null 2>&1") != 0){
		gm = "./graphMerge ";
		if (system("command -v ./graphMerge >/dev/null 2>&1") != 0){
			printf("graphMerge is required to run graphMergeMultiple.\n");
			return 2;
		}
	}

	string cmd, output(argv[1]);
	system(("echo \"\" > " + output + ".dot").c_str());
	for (int i = 2; i < argc; ++i){
		cmd = gm + output + ".dot " + string(argv[i]) + " " + output + " > /dev/null 2>&1";
		if (system(cmd.c_str()) == 0)
			printf("File %s merged to %s.dot\n", argv[i], output.c_str());
		else
			printf("File %s was not merged. An error ocurred when trying to merge it.\n", argv[i]);
	}
	
	return 0;
}
