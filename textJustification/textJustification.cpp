#include "stdafx.h"
#include <vector>
#include <iostream>
#include <iterator>
#include <string>
#include <iomanip>
#include <fstream>
#include <time.h>
using namespace std;

vector<string> fill_vector(istream &s);
string padChar(int, char);
string flush_left(vector<string> *, int);
string flush_right(vector<string> *, int);
string full_justify(vector<string> *, int);

int main(int argc, char* argv[])
{
	if (argc == 5) {
		ifstream inFile(argv[1]);
		ofstream outFile(argv[2]);
		if (inFile.good() && outFile.good()) {
			int columnWidth = atoi(argv[3]);
			string style = string(argv[4]);
			vector<string> text = fill_vector(inFile);
			string newText = "";
			if (style == "flush_left") {
				newText = flush_left(&text, columnWidth);
			}
			else if (style == "flush_right") {
				newText = flush_right(&text, columnWidth);
			}
			else if (style == "full_justify") {
				newText = full_justify(&text, columnWidth);
			}
			else {
				cerr << "Style provided is not supported. Please choose from: 'flush_left', 'flush_right', or 'full_justify'" << endl;
			}
			cout << newText;
			outFile << newText;
		}
	}
	else {
		//Program expects 4 arguments from users. The first argument, argv[0], is the provided executable from main 
		cerr << "Missing " << (5 - argc) << " arguments. Format is as follows: [input_file] [output_file] [line_width] [format_style]" << endl;
	}
	int x;
	cin >> x; //stall program to read output
}


vector<string> fill_vector(istream &fileStream) {
	vector<string> v;
	string word;
	while (fileStream >> word) {
		v.push_back(word);
	}
	return v;
}

string padChar(int count, char fill = ' ') {
	int i = 0;
	string spaces;
	while (i < count) {
		spaces += fill;
		i++;
	}
	return spaces;
}

string flush_left(vector<string> * x, int columnWidth) {
	string composite, word, spaces, line;
	vector<string>::iterator it = x->begin();
	for (; it != x->end(); it++) {
		word = *it + " ";
		int wordLen = word.length();
		int lineLen = line.length();
		if (lineLen + wordLen > columnWidth) { //don't assign currentLineLength because we want to preserve the remainder
			line = line.substr(0, line.length() - 1);
			spaces = padChar(columnWidth - line.length());
			composite += line + spaces + "\n";
			line = "";
			spaces = "";
		}
		line += word;
	}
	line = line.substr(0, line.length() - 1);
	spaces = padChar(columnWidth - line.length());
	composite += line + spaces;
	return composite;
}

string flush_right(vector<string> * x, int columnWidth) {
	time_t begin = clock();
	string composite, word, spaces, line;
	vector<string>::iterator it = x->begin();
	for (; it != x->end(); it++) {
		word = *it + " ";
		int wordLen = word.length();
		int lineLen = line.length();
		if (lineLen + wordLen > columnWidth) { //don't assign currentLineLength because we want to preserve the remainder
			line = line.substr(0, line.length() - 1);
			spaces = padChar(columnWidth - line.length());
			composite += spaces + line + "\n";
			line = "";
			spaces = "";
		}
		line += word;
	}
	line = line.substr(0, line.length() - 1);
	spaces = padChar(columnWidth - line.length());
	composite += spaces + line;
	double dur = double(clock() - begin) / CLOCKS_PER_SEC;
	cout << setprecision(10) << fixed << dur << endl;
	return composite;
}

string full_justify(vector<string> * x, int columnWidth) {
	return string("");
}