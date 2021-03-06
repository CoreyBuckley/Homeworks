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
string distribute_spaces(vector<string> *, int, int, int);
vector<string> breakdown_word(string, int);
string format_line(vector<string> *, string *, int *, int, bool);

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
	time_t begin = clock();
	vector<string>::iterator it = x->begin();
	vector<string>::iterator end = x->end();
	vector<string> line;
	string word, composite;
	bool lastWord = false;
	int lineLen = 0;
	for (; it != end-1; it++) { //intentionally not going to the end because we need to manually take
								//out the remaining word of our line vector on the last call
								//(since there's nothing to overflow it and push it to our composite)
		word = *it;
		composite += format_line(&line, &word, &lineLen, columnWidth, lastWord);
	}
	word = *it; //(*it) is now the last word
	lastWord = true;
	composite += format_line(&line, &word, &lineLen, columnWidth, lastWord);
	double dif = double(clock() - begin) / CLOCKS_PER_SEC;
	cout << setprecision(20) << fixed << dif << endl;
	return composite;
}

string distribute_spaces(vector<string> * lineV, int spaces, int remain, int columnWidth) {
	string line, word;
	vector<string>::iterator it = lineV->begin();
	vector<string>::iterator end = lineV->end();
	for (; it != end; it++) {
		word = *it;
		line += word;
		if (it != end - 1) {
			line += " " + padChar(spaces);
		}
		else {
			line += padChar(columnWidth - line.length());
		}
		if (remain > 0) {
			line += " ";
			remain -= 1;
		}
	}
	return line;
}

vector<string> breakdown_word(string word, int columnWidth) { //return the last broken down word to be put into our line
	vector<string> brokenDownWords;
	string brokenDown;
	int len = word.length();
	int i = 0;
	int passes = len / (columnWidth-1); //columnWidth-1 because there's that many letters EXCLUDING the hyphen at the end
	for (; i < passes; i++) {
		string wordPart = word.substr((columnWidth-1)*i, columnWidth - 1);
		brokenDown += wordPart + "-";
		if (i != passes - 1) {
			brokenDown += "\n";
		}
	}
	string rest = word.substr((columnWidth - 1)*i);
	brokenDownWords.push_back(brokenDown);
	brokenDownWords.push_back(rest);
	return brokenDownWords;
}

string format_line(vector<string> * lineV, string * word, int *lineLen, int columnWidth, bool lastWord) {
	string line = "";
	*word;
	int possibleLineLength, gaps, spaces, remainder;
	spaces = remainder = possibleLineLength = 0;  //initialize to 0
	gaps = lineV->size(); //theoretically, if we add this word, then our gaps would be (linePre-1)+1, therefore cancelling out
	possibleLineLength = *lineLen + word->length();
	if (gaps > 0) {
		possibleLineLength += gaps; //representing spaces
	}
	if (possibleLineLength > columnWidth) {
		int difference, words;
		gaps -= 1; //the word didn't work out so we'll lose a gap
		words = gaps + 1; //representing the words in our line vector
		if (words > 0) { //meaning there's 1 word on the line
			difference = columnWidth - (*lineLen + gaps);
			if (words > 1) { //meaning there exists a gap. Else, the distribute_spaces will left-justify the word
				spaces = difference / gaps;
				remainder = difference % gaps;
			}
			line += distribute_spaces(lineV, spaces, remainder, columnWidth);
			line += "\n";
		}
		if (word->length() > columnWidth) { //before we add the word that just overflowed our line to the vector
											//make sure it can fit into the line. If it doesn't, break it down
											//until we can
			vector<string> brokenDownWords = breakdown_word(*word, columnWidth);
			string wordPart1 = brokenDownWords[0];
			string wordPart2 = brokenDownWords[1];
			line += wordPart1 + "\n";
			*word = wordPart2; //part of the word that starts the next line
		}
		lineV->clear();
		*lineLen = 0;
	}
	if (lastWord) { //when the vector of words end, there's nothing to overflow our line and thus print it out.
						//so we need to manually take out the word inside our line.
		line += *word + padChar(columnWidth-word->length());
	}
	else {
		*lineLen += word->length();
		lineV->push_back(*word);
	}
	return line;
}