#pragma once

class bowlingplayer {
public:
	static int FRAME_COLUMN_WIDTH; //used for the score card representation of the player
	static int NUMBER_OF_FRAMES;
	static int NAME_COLUMN_WIDTH;
	//static int SCORECARD_WIDTH;

	//CONSTRUCTORS
	bowlingplayer();
	bowlingplayer(std::string first, std::string last, const std::vector<int>& scores);

	//ACCESSORS
	int getTotalScore() const;
	const std::vector<int>& getThrowScores() const;
	const std::vector<int>& getFrameScores() const;
	const std::string getName() const;
	const std::string& getLastName() const;
	const std::string& getFirstName() const;

	static const int getScorecardWidth();
	static const int getNameColumnWidth();

	void populateFrameScores();

	//MUTATORS
	static void setNameColumnWidth(int width);

	//HELPERS
	const std::string toString();

private:
	int totalScore, turn;
	std::vector<int> throwScores;
	std::vector<int> frameScores;
	std::string firstName;
	std::string lastName;
};

std::ostream& operator<<(std::ostream& out, const bowlingplayer plr);

//COMPARE FUNCTIONS
bool cmpAlpha(const bowlingplayer& left, const bowlingplayer& right);
bool cmpScore(const bowlingplayer& left, const bowlingplayer& right);
bool cmpNameLength(const bowlingplayer& left, const bowlingplayer& right);

//HELPERS
void nextFrame(int& frame, int& throws, int& pastScore);
void setPastScore(const bool frameEnd, const int score, int& pastScore);
std::string markScoreboard(const int& score, bool& strike, bool& spare, const int& pastScore, const int& frame);