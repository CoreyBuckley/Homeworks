#include "stdafx.h"
#include "bowlingplayer.h"
using namespace std;

int bowlingplayer::FRAME_COLUMN_WIDTH = 5; //initialize and define static variable
int bowlingplayer::NUMBER_OF_FRAMES = 10;
int bowlingplayer::NAME_COLUMN_WIDTH;
//int bowlingplayer::SCORECARD_WIDTH = getScorecardWidth();

#pragma region Constructors
//default will be all zero scores
bowlingplayer::bowlingplayer() : firstName("Robby"), lastName("Nobody"), throwScores(vector<int>(10, 0)) {
	populateFrameScores();
}

bowlingplayer::bowlingplayer(string first, string last, const vector<int>& scores) : firstName(first), lastName(last), throwScores(scores) {
	populateFrameScores();
}
#pragma endregion

#pragma region Accessors

const int& bowlingplayer::getTotalScore() const
{
	return totalScore;
}

const vector<int>& bowlingplayer::getThrowScores() const
{
	return throwScores;
}

const std::vector<int>& bowlingplayer::getFrameScores() const
{
	return frameScores;
}

const string bowlingplayer::getName() const
{
	return firstName + " " + lastName;
}

const std::string & bowlingplayer::getLastName() const
{
	return lastName;
}

const std::string & bowlingplayer::getFirstName() const
{
	return firstName;
}

const int& bowlingplayer::getScorecardWidth()
{
	int width = bowlingplayer::NAME_COLUMN_WIDTH + 2 + //2 pipes on each end
				(bowlingplayer::NUMBER_OF_FRAMES - 1)*bowlingplayer::FRAME_COLUMN_WIDTH + 7 + //frames before the last have width 5 and last is 7
				bowlingplayer::NUMBER_OF_FRAMES; //pipes dividing columns is frames-1 but there's a pipe on the right end so -1 is cancelled out
	return width;
}

const int& bowlingplayer::getNameColumnWidth()
{
	return bowlingplayer::NAME_COLUMN_WIDTH;
}
#pragma endregion 

#pragma region Mutators
void bowlingplayer::setNameColumnWidth(const int& width)
{
	bowlingplayer::NAME_COLUMN_WIDTH = width;
}

void setPastScore(const bool frameEnd, const int score, int& pastScore) {
	if (frameEnd || score == 10) { //don't let the pastScore be from a different frame. frameEnd is based on throwsLeft, need to check strike.
		pastScore = -1;
	}
	else {
		pastScore = score;
	}
}
#pragma endregion

#pragma region Insertion & Compare Functions
//return a scorecard for one player. Will chain for multiple players in main to form
//a larger scorecard
ostream& operator<<(ostream& out, const bowlingplayer plr)
{
	//9*5+7+11 = 63 spaces for the 'score' portion of the card (outside pipes included)
	//Where 9 is the number of frames that have 5 spaces (2 nums + middle + space on each side)
	//Where 7 is the number of spaces for the 10th frame because of 3 throws.
	//Where 11 is the number of pipes needed to enclose the throw results.
	//In examples, 12 spaces are used for containing the name on the score card, with a space on each side
	//so, that's 13 spaces including the left-hand pipe, then 15 for space on both sides,
	//therefore 15 = spaces for 'name' portion of the card
	string name = plr.getName();
	vector<int> scores = plr.getThrowScores();
	int numThrows = scores.size();
	//TOP DASHED LINE, will stack when chaining outputs,
	//so will have to do it once, externally
	//out << setfill('-') << setw(15 + 63) << "" << endl; //force the buffer to print since they're modifiers
	out << "| " << name << setfill(' ') << setw(bowlingplayer::NAME_COLUMN_WIDTH - name.length()) << "|"; // we want pipe at the end of that width
	int throwsLeft = 2;
	int frame = 1;
	int pastScore = -1, throws = 0, i = 0;
	while (frame <= bowlingplayer::NUMBER_OF_FRAMES) {
		throws++;
		bool strike = false;
		bool spare = false;
		bool frameEnd = throws % throwsLeft == 0;
		int score = scores[i];
		out << markScoreboard(score, strike, spare, pastScore, frame);
		if (frame < bowlingplayer::NUMBER_OF_FRAMES) {
			if (strike || frameEnd) {
				out << " |";
				nextFrame(frame, throws);
			}
		}
		else { //the tenth frame is special because strikes don't increment frame
			if (strike || spare) {
				throwsLeft = 3; //and you get 3 throws if you get a spare or strike.
			}
			if (frameEnd) { //print ending pipe for last frame
				frame++;
				out << setw(8 - throws * 2) << "|"; //each throw occupies 2 spaces on the board and then 8 is one past the inner bound of last frame
			}
		}
		setPastScore(frameEnd, score, pastScore);
		i++;
	}
	out << endl;
	out << "| " << setfill(' ') << setw(bowlingplayer::NAME_COLUMN_WIDTH) << "|"; //line under name column
	vector<int> frameScores = plr.getFrameScores();
	int len = frameScores.size();
	for (int i = 0; i < len; i++) {
		if (i != len - 1) {
			out << setw(4) << frameScores[i];
		}
		else {
			out << setw(6) << frameScores[i];
		}
		out << " |";
	}
	out << endl;
	out << string(bowlingplayer::getScorecardWidth(), '-');
	return out;
}

//will be used to sort by last name, then first name if equivalent.
bool cmpAlpha(const bowlingplayer& left, const bowlingplayer& right)
{
	string leftLastName = left.getLastName();
	string rightLastName = right.getLastName();
	if (leftLastName < rightLastName) {
		return true;
	}
	else if (leftLastName == rightLastName) {
		string leftFirstName = left.getFirstName();
		string rightFirstName = right.getFirstName();
		if (leftFirstName < rightFirstName) {
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}

//sorts by player's total score; sorted ascending
bool cmpScore(const bowlingplayer& left, const bowlingplayer& right)
{
	int totalLeft = left.getTotalScore();
	int totalRight = right.getTotalScore();
	if (totalLeft < totalRight) {
		return true;
	}
	else {
		return false;
	}
}

bool cmpNameLength(const bowlingplayer & left, const bowlingplayer & right)
{
	int leftLen = left.getName().length();
	int rightLen = right.getName().length();
	if (leftLen < rightLen) {
		return true;
	}
	else {
		return false;
	}
}
#pragma endregion

#pragma region Helpers
const std::string bowlingplayer::toString()
{
	string singleLineOutput = getName() + " ";
	char score[3];
	for (int i = 0; i < frameScores.size(); i++) {
		_itoa_s(frameScores[i], score, 10); //have to convert from int to char
		singleLineOutput += score;
		if (i != frameScores.size() - 1) {
			singleLineOutput += " ";
		}
	}
	return singleLineOutput;
}

void nextFrame(int& frame, int& throws) {
	frame += 1;
	throws = 0;
}

string markScoreboard(const int & score, bool & strike, bool & spare, const int & pastScore, const int & frame)
{
	string symbol = " "; //we will have a space preceed the symbol b/c we're starting after a pipe
	char textRepForNum[3];
	textRepForNum[2] = '\0';
	if (score == 0) {
		symbol += "-"; // '-' represents gutter ball
	}
	//pastScore of -1 means first throw
	else if (score == 10 && pastScore == -1) { //how we represent the 'X' will vary based on frame
		strike = true;
		if (frame < bowlingplayer::NUMBER_OF_FRAMES) {
			string spaces(bowlingplayer::FRAME_COLUMN_WIDTH - 2, ' '); //Strike is placed one before the end of the frame width
																 //Subtract 2 because we're going to add 
																 //our 'X' to that width-1 spot
			symbol = spaces + "X";
		}
		else {
			symbol += "X";
		}
	}
	else if (score + pastScore == 10) {
		spare = true;
		symbol += "/"; // '/' represents spare
	}
	else {
		_itoa_s(score, textRepForNum, 10);
		symbol += textRepForNum;
	}
	return symbol;
}

void bowlingplayer::populateFrameScores() {
	int len = throwScores.size();
	int score, total, pastScore, i, throws;
	int frame = 1;
	pastScore = -1;
	score = total = i = throws = 0;
	int maxThrows = 2;
	while (frame <= NUMBER_OF_FRAMES) {
		throws++;
		score = throwScores[i];
		bool strike = score == 10;
		bool spare = pastScore + score == 10;
		bool frameEnd = throws % maxThrows == 0;
		if (frame < NUMBER_OF_FRAMES) {
			int lookahead = throwScores[i + 1];
			if (frameEnd || strike) { //frame end or strike
				if (score == 10) {
					if (lookahead == 10) {
						score = 30; //double the 10, add to current score
					}
					else {
						score = 10 + lookahead + throwScores[i + 2];
					}
				}
				else if (frameEnd) { //spares happen on end of frame
					if (spare) {
						score += pastScore + lookahead;
					}
					else {
						score += pastScore;
					}
				}
				nextFrame(frame, throws);
				total += score;
				frameScores.push_back(total);
			}
		}
		else {
			total += score;
			if (strike || spare) {
				maxThrows = 3;
			}
			if (frameEnd) {
				frameScores.push_back(total);
				frame++;
			}
		}
		setPastScore(frameEnd, score, pastScore);
		i++;
	}
	totalScore = total;
}
#pragma endregion
