/*
 * The MIT License
 *
 * Copyright (c) 1997-2012 The University of Utah
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef STACKTRACER_H
#define STACKTRACER_H

/* Writing to files */

void setVariable(const char *name, bool value);
bool getVariable(const char *name);
void printStacktraceIfTrue(const char *fileName, const char *lastNode, const char *conditionalVariable);
void printStacktrace(const char *fileName, const char *lastNode, int skipInStackTop = 1);

/* Creating in memory */

#include <set>
#include <map>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

class cstg{
public:
	void addStacktrace(const char *lastNode);
	void write(const char *fileName);
	void write(const string &fileName);
	void generatePDF(const char *dotFileName, const char *pdfFileName);
	void generatePDF(const string &dotFileName, const string &pdfFileName);
	void append(const char *fileName);
	void append(const string &fileName);
private:
	class Node{
	public:
		string name;
		Node(const string &name = ""){
			this->name = name;
		}
		Node(const char *name){
			this->name = string(name);
		}
		Node(const Node &n) {
			name = n.name;
		}
		bool operator< (const Node &n) const {
			return name < n.name;
		}
		bool operator== (const Node &n) const {
			return name == n.name;
		}
		bool operator!= (const Node &n) const {
			return name != n.name;
		}
	};

	class Edge{
	public:
		Node src;
		Node dst;
		Edge(const Node &src, const Node &dst){
			this->src = src;
			this->dst = dst;
		}
		bool operator< (const Edge &e) const {
			if (src != e.src)
				return src < e.src;
		    return dst < e.dst;
		}
		bool operator== (const Edge &e) const {
			return src == e.src && dst == e.dst;
		}
		bool operator!= (const Edge &e) const {
			return src != e.src || dst != e.dst;
		}
	};

	// change this to "unsorted" set and map when compiling with C++11
	map<Edge, int> edges;
	set<Node> nodes;
};

/***** Auxiliary functions ******/

template <typename T>
string numberToString ( T Number )
{
	ostringstream ss;
	ss << Number;
	return ss.str();
}

#endif

