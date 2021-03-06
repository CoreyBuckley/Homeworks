/*
Corey Buckley
7/7/2018
Homework 2; Bowling

Spare: Get bonus on that frame equal to the number of pins 
	   knocked down in the next frame on first throw score.

	    FramePoints = 10+nextFirstThrow

Strike: Get bonus points equal to the number of pins 
		knocked down in the next frame for the 
		1st & 2nd throws.

		FramePoints = 10+nextFirstThrow+nextSecondThrow

Problems enoutputScoreTableered: When making a comparator function for sort(), I kept getting referenced function is deleted. This is because
					 in the comparator function, the copy constructor is used which does a member-by-member wise company. Originally,
					 in the bowlingplayer.h there was a private const member variable representing the maximum and minimum number of throws.
					 Removing this fixed the issue. This site was helpful: http://www.cplusplus.com/forum/beginner/190834/

Reference for scoring: http://slocums.homestead.com/gamescore.html

*/

#include "stdafx.h"
#include "bowlingplayer.h"
using namespace std;

void readPlayers(ifstream& inFile, vector<bowlingplayer>& plrs);
void readName(ifstream& inFile, string& first, string& last);
void readScores(ifstream& inFile, vector<int>& scoresV);

int main(int argc, char * argv[])
{
	//the program's command format will follow this format: [input_file] [output_file] [mode]
	//the input text file will follow this format: [first_name] [last_name] throw1 throw2 ...
	//allowed to assume that the number of frames inputted will be valid
	if (argc == 4) {
		ifstream inputScores(argv[1]);
		ofstream outputScoreTable(argv[2]);
		string mode = string(argv[3]);
		if (inputScores.good() && outputScoreTable.good()) {
			vector<bowlingplayer> players;
			readPlayers(inputScores, players);
			sort(players.begin(), players.end(), cmpNameLength);
			int longestNameLen = (players.end() - 1)->getName().length();
			bowlingplayer::setNameColumnWidth(longestNameLen+2); //make the name field fit everyone. Stored in static var. +2 because spaces on ends
			int scorecardWidth = bowlingplayer::getScorecardWidth();
			int nameColumnWidth = bowlingplayer::getNameColumnWidth();
			sort(players.begin(), players.end(), cmpAlpha);
			outputScoreTable << string(scorecardWidth, '-') << endl; //top divider
			for (int i = 0; i < players.size(); i++) {
				outputScoreTable << players[i] << endl;
			}
			sort(players.begin(), players.end(), cmpScore);
			outputScoreTable << endl;
			for (int i = players.size()-1; i > -1; i--) {
				//std::left and std::right are from iomanip, meant for text alignment (where 'right' means where the TEXT should be, not spaces)
				outputScoreTable << left << setw(nameColumnWidth-1) << players[i].getName() << right << setw(3) << players[i].getTotalScore() << endl;
			}
			if (mode == "custom") {
				//Median score for a bowling game is 78
				outputScoreTable << endl;
				outputScoreTable << "According to Balmoral Software, the median bowling score for all possible games is 78." << endl;
				outputScoreTable << right << setw(84) << "Below is the relative performance of these players as compared to the median score" << endl << endl;
				int center = (4 + longestNameLen / 2) / 2;
				outputScoreTable << string(center, ' ');
				outputScoreTable << "NAME" << string(longestNameLen-center-2, ' ');
				outputScoreTable << "PERFORMANCE" << endl << endl;
				float relativeSkill = 0.0f;
				for (int i = players.size() - 1; i > -1; i--) {
					bowlingplayer plr = players[i];
					int totalScore = players[i].getTotalScore();
					relativeSkill = totalScore / 78.0f;
					outputScoreTable << left << setw(longestNameLen) << plr.getName() << " " << setprecision(2) << fixed << right << setw(4) << relativeSkill << "x ";
					if (relativeSkill < 1) {
						outputScoreTable << "worse ";
					}
					else {
						outputScoreTable << "better ";
					}
					outputScoreTable << endl;
				}
			}
		}
	}
	else {
		cerr << "Must provide 4 arguments as follows, where mode is either 'standard' or 'custom': [input_file] [output_file] [mode]" << endl;
	}
	int x;
	cin >> x;
    return 0;
}

void readPlayers(ifstream& inFile, vector<bowlingplayer>& plrs) {
	string name;
	int frameScore = 0;
	int i = 0;
	while (!inFile.eof()) { //not guaranteed that the file will have lines equal to # of plrs. Doesn't work rn.
		vector<int> scores;
		string firstName, lastName;
		readName(inFile, firstName, lastName);
		if (!firstName.empty() && !lastName.empty()) { //so long as there is content
			readScores(inFile, scores);
			plrs.push_back(bowlingplayer(firstName, lastName, scores));
		}
	}
}

void readName(ifstream& inFile, string& firstName, string& lastName) {
	string nameToken;
	string name;
	for (int i = 0; i < 2; i++) {
		inFile >> nameToken;
		name += nameToken;
		if (i % 2 == 0) {
			firstName = nameToken;
		}
		else {
			lastName = nameToken;
		}
	}
}

//perfect game max throws: 12 -> 10 frames
//reg game max throws: 20, -> 10 frames
void readScores(ifstream& inFile, vector<int>& scoresV) {
	int score = 0;
	int currentFrame = 1;
	int throwCount = 0;
	while (currentFrame < 10) {
		inFile >> score;
		if (score < 10) {
			throwCount += 1;
		}
		if (throwCount % 2 == 0 || score == 10) {
			currentFrame += 1;
			throwCount = 0;
		}
		scoresV.push_back(score);
	}
	//on 10th frame, special rules needed. If a strike or spare is made, then you get a bonus throw.
	int temp; //store the first throw so we can calculate a spare if needed
	int throwsLeft = 1;
	inFile >> temp;
	if (temp == 10) {
		throwsLeft = 2;
	}
	scoresV.push_back(temp);
	for (int i = 0; i < throwsLeft; i++) {
		inFile >> score;
		if (score + temp == 10) {
			throwsLeft = 2;
		}
		scoresV.push_back(score);
	}
}