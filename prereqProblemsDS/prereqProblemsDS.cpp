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

ofstream outputFile("output.txt"); //global output stream; will send to file instead of std::cout

map<char, int> problem1(string);
int problem2(vector<int>);
int problem2_linear(vector<int>);
void problem3(int [], int);
int * problem3_linear(int[], int, int);
int * build_array(int[], int, int, int);
int problem4(vector<int>);
int compute_avg(vector<int>);
pair<int,int> problem5(vector<int>);

template <typename SequenceContainer>
void as(typename SequenceContainer, int);

int main()
{
	//PROBLEM 1
	outputFile << "Problem 1: " << endl;
	map<char, int> freqs = problem1("frequency.txt");
	map<char, int>::iterator it = freqs.begin();
	for (; it != freqs.end(); it++) {
		char key = it->first;
		int value = it->second;
		outputFile << "'" << key << "':" << value << endl;
	}
	outputFile << endl;
	//----------------------------------------------//
	//PROBLEM 2
	outputFile << "Problem 2: " << endl;
	int const len = 20;
	int const bound = 500;
	vector<int> unsortedNums(len);
	for (int i = 0; i < len; i++) {
		int num = rand() % bound + 1; //between 1 and len
		unsortedNums[i] = num;
	}
	as<vector<int>>(unsortedNums, len);
	int max = problem2_linear(unsortedNums);
	outputFile << "Max: " << max << endl;
	outputFile << endl;
	//----------------------------------------------//
	//PROBLEM 3
	outputFile << "PROBLEM 3: " << endl;
	int const len3 = 1;
	int const delta = 5;
	int sorted[len3];
	for (int i = 0; i < len3*delta; i+=delta) { //fill with incrementing values ('sorted' ASC)
		sorted[i/delta] = i;
	}
	as<int[]>(sorted, len3);
	int * sorted2 = problem3_linear(sorted, len3, -2); //Insert -2 into its correct place. Will return an array of len3+1
	as<int[]>(sorted2, len3 + 1);
	outputFile << endl;
	//----------------------------------------------//
	//PROBLEM 4
	outputFile << "PROBLEM 4: " << endl;
	vector<int> n = {8,39,201,394,10,3,1,22,30};
	as<vector<int>>(n, n.size());
	int closest = problem4(n);
	outputFile << "Closest to avg is: " << closest << endl;
	outputFile << endl;
	//----------------------------------------------//
	//PROBLEM 5
	outputFile << "PROBLEM 5: " << endl;
	vector<int> numsRelative = { 20,22,44,24,45 };
	pair<int, int> closestValues = problem5(numsRelative);
	outputFile << "<" << closestValues.first << "," << closestValues.second << ">" << endl;
	//----------------------------------------------//
	outputFile.close();
    return 0;
}

//Helper function for printing out array-like containers.
template <typename SequenceContainer>
void as<SequenceContainer>(SequenceContainer ar, int len) {
	outputFile << "[";
	for (int i = 0; i < len; i++) {
		int num = ar[i];
		outputFile << num;
		if (i != len - 1) {
			outputFile << ",";
		}
	}
	outputFile << "]" << endl;
}

//return a map for letter frequencies given a text file
map<char, int> problem1(string fileName) {
	ifstream freqFile(fileName);
	map<char, int> frequencies;
	if (freqFile.good()) {
		char symbol;
		while (freqFile.get(symbol)) {
			map<char, int>::iterator it = frequencies.find(symbol); //returns iterator to end if doesn't exist
			if (it != frequencies.end()) {
				it->second += 1;
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
//this function assumes that the sorted array passed in is sorted ascending.
int * problem3_linear(int sortedarray[], int len, int num) {
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
				else if (num < current) { //insert behind
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

//Find the value closest to the average in a vector
//you can't do a one-pass linear approach without sorting the vector first, else
//there's too much unpredicatibility to be able to say some number is the closest
int problem4(vector<int> nums) { //return value closest to avg
	int closest = 0;
	double avg = compute_avg(nums);
	sort(nums.begin(), nums.end()); //will sort ASC
	vector<int>::iterator it = nums.begin();
	vector<int>::iterator end = nums.end();
	for (; it != end-1; it++) { //linear pass
		int next = *(it + 1);
		int distCurrent = abs(avg - *it);
		int distNext = abs(avg - next);
		if (distCurrent <= distNext) {
			return *it;
		}
	}
	return *it; //if nothing returned, then last element must be closest
}

int compute_avg(vector<int> nums) {
	vector<int>::iterator it = nums.begin();
	vector<int>::iterator end = nums.end();
	double sum = 0.0;
	int count = nums.size();
	for (; it != end; it++) {
		sum += *it;
	}
	//can natural round the number because it's a vector of ints. Don't need a decimal.
	int avg = (int)round(sum / count);
	outputFile << "AVG IS: " << avg << endl;
	return avg;
}

//Find the two closest values in a vector
pair<int,int> problem5(vector<int> nums) { //return two closest values in a vector
	int dif = 0; //want closest to 0
	int firstNumber = 0;
	int secondNumber = 0;
	int len = nums.size();
	if (len > 2) {
		sort(nums.begin(), nums.end()); // ASC
		dif = nums[1] - nums[0];
		for (int i = 0; i < len-1; i++) {
			int base = nums[i];
			for (int j = i+1; j < len; j++) {
				int combo = nums[j];
				int distance = combo - base;
				if (distance < dif) {
					dif = distance;
					firstNumber = base;
					secondNumber = combo;
				}
			}
		}
	} 
	else if (len == 2) {
		firstNumber = nums[0];
		secondNumber = nums[1];
	}
	pair<int, int> closestValues(firstNumber, secondNumber);
	return closestValues;
}