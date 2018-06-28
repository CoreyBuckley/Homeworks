// prereqProblemsDS.cpp : Defines the entry point for the console application.
//

/*

1 Count the number of times each letter appears in a file
2 Find the maximum value in an array or vector
3 Insert a new element into a sorted array in its correct place
4 Find the value closest to the average in a vector
5 Find the two closest values in a vector

*/

#include "stdafx.h"
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
using namespace std;

map<char, int> problem1();
int problem2(vector<int>);
int problem2_linear(vector<int>);
void problem3(int [], int);
int * problem3_linear(int[], int, int);
int * build_array(int[], int, int, int);
int problem4(vector<int>);
pair<int,int> problem5(vector<int>);

template <typename SequenceContainer>
void as(typename SequenceContainer, int);

int main()
{
	//PROBLEM 1
	map<char, int> freqs = problem1();
	map<char, int>::iterator it = freqs.begin();
	cout << "Problem 1: " << endl;
	for (; it != freqs.end(); it++) {
		char key = it->first;
		int value = it->second;
		cout << "'" << key << "':" << value << endl;
	}
	cout << endl;
	//----------------------------------------------//
	//PROBLEM 2
	int const len = 20;
	int const bound = 500;
	vector<int> unsortedNums(len);
	cout << "Problem 2: " << endl;
	for (int i = 0; i < len; i++) {
		int num = rand() % bound + 1; //between 1 and len
		unsortedNums[i] = num;
	}
	as<vector<int>>(unsortedNums, len);
	int max = problem2_linear(unsortedNums);
	cout << "Max: " << max << endl << endl;
	//----------------------------------------------//
	//PROBLEM 3
	int const len3 = 5;
	int const delta = 2;
	int sorted[len3];
	for (int i = 0; i < len3*delta; i+=delta) {
		sorted[i/delta] = i;
	}
	int test[] = { 1 };
	//int * test2 = problem3_linear(test, 1, 0);
	cout << "PROBLEM 3: " << endl;
	as<int[]>(sorted, len3);
	int * sorted2 = problem3_linear(sorted, len3, 62); //put in 0 for array
	//as<int[]>(test2, 2);
	as<int []>(sorted2, len3+1);
	//----------------------------------------------//
	//PROBLEM 4

	//----------------------------------------------//
	int x;
	cin >> x;
    return 0;
}

template <typename SequenceContainer>
void as<SequenceContainer>(SequenceContainer ar, int len) {
	cout << "[";
	for (int i = 0; i < len; i++) {
		int num = ar[i];
		cout << num;
		if (i != len - 1) {
			cout << ",";
		}
	}
	cout << "]" << endl;
}

map<char, int> problem1() { //return a dictionary/map for letter frequencies
	ifstream freqFile("frequency.txt");
	map<char, int> frequencies;
	if (freqFile.good()) {
		char symbol;
		while (freqFile.get(symbol)) {
			map<char, int>::iterator it = frequencies.find(symbol); //returns iterator to end if doesn't exist
			if (it != frequencies.end()) {
				pair<char, int> updatedFreq(it->first, it->second + 1);
				frequencies.erase(it);
				frequencies.insert(updatedFreq);
			}
			else {
				pair<char, int> charFreq(symbol, 1);
				frequencies.insert(charFreq);
			}		
		}
	}
	freqFile.close();
	return frequencies;
}

//Approaches: Linear search, sort
int problem2_linear(vector<int> nums) { //return max value
	int max = 0;
	vector<int>::iterator it = nums.begin();
	for (; it != nums.end(); it++) {
		int num = *it;
		if (num > max) {
			max = num;
		}
	}
	return max;
}

void problem3(int sortedarray[], int num) { //insert element into its correct place
	//construct a binary tree then step down thru nodes
	//if the parent node is > num and all the children nodes are < num, then that is 
	//where it should be inserted

}

//will return a new array of len+1
int * problem3_linear(int sortedarray[], int len, int num) { //Assuming ASC, if the element in front is greater than # and element behind is < #
	int i = 0;
	int * newarray = NULL;
	int matchingIndex = 0;
	if (len > 1) { 
		while (i < len-1) { // {3,5} insert 4
			int current = sortedarray[i];
			int ahead = sortedarray[i + 1];
			if (num < ahead) {
				if (num >= current) { //insert forward
					matchingIndex = i + 1;
				}
				else if (num < ahead && num < current) { //insert behind
					matchingIndex = i - 1;
					if (matchingIndex < 0) {
						matchingIndex = 0;
					}
				}
				return build_array(sortedarray, len, num, matchingIndex);
			}
			i++;
		}
		//if nothing returned above, then we know the number must be the biggest
		sortedarray[len] = num; //undefined behaviour. Am assigning outside the bounds of what was declared. Should prob not.
		return sortedarray;
	}
	else { //array of one element
		newarray = (int *) malloc(sizeof(int)*2);
		if (sortedarray[0] >= num) {
			newarray[0] = num;
			newarray[1] = sortedarray[0];
		}
		else {
			newarray[0] = sortedarray[0];
			newarray[1] = num;
		}
		return newarray;
	}
	return newarray;
}

int * build_array(int ar[], int len, int num, int replacedIndex) {
	int size = len + 1;
	int * newarray = (int *)malloc(sizeof(int)*size);
	for (int i = 0; i < size; i++) {
		if (i == replacedIndex) {
			newarray[i] = num;
		}
		else if (i > replacedIndex) {
			newarray[i] = ar[i - 1];
		}
		else {
			newarray[i] = ar[i];
		}
	}
	return newarray;
}

int problem4(vector<int> nums) { //return value closest to avg
	int closest = 0;

	return closest;
}

pair<int,int> problem5(vector<int> nums) { //return two closest values in a vector
	pair<int, int> closestValues;

	return closestValues;
}

