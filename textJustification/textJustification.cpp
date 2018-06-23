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
string flush(string, vector<string> *, int);
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
				newText = flush("left", &text, columnWidth);
			}
			else if (style == "flush_right") {
				newText = flush("right", &text, columnWidth);
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

string flush(string style, vector<string> * x, int columnWidth) {
	time_t begin = clock();
	// sentenceStructure contains the line and the spaces, where the corresponding index matches the style
	string composite, word, sentenceStructure[2], *line, *spaces;
	vector<string>::iterator it = x->begin();
	int binaryIndexForSentenceStructure = (style == "left"); //indicies for spaces and line will be adjusted relative to this
	spaces = &sentenceStructure[binaryIndexForSentenceStructure];
	line = &sentenceStructure[!binaryIndexForSentenceStructure];
	for (; it != x->end(); it++) {
		word = *it + " ";
		int possibleLineLength = line->length() + word.length();
		if (possibleLineLength > columnWidth) { //don't assign to line because we want to preserve the remainder and we'd have to chop off the word we just added
			*line = line->substr(0, line->length() - 1); //get rid of extra space at end
			*spaces = padChar(columnWidth - line->length());
			composite += sentenceStructure[0] + sentenceStructure[1] + "\n";
			*line = "";
			*spaces = "";
		}
		*line += word;
	}
	*line = line->substr(0, line->length() - 1);
	*spaces = padChar(columnWidth - line->length());
	composite += sentenceStructure[0] + sentenceStructure[1];
	double dif = double(clock() - begin) / CLOCKS_PER_SEC;
	cout << setprecision(20) << fixed << dif << endl;
	return composite;
}

string full_justify(vector<string> * x, int columnWidth) {
	return string("");
}