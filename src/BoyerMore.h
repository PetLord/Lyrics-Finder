#ifndef BoyerMore_H
#define BoyerMore_H

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>

using namespace std;

// keresi a tut a szenakazalban

class Boyer
{
private: 
	string text;
	string pattern;
	map<char, int> goodHeuristic;
	unordered_map<char, int> badHeuristic;

public:

	Boyer();
	Boyer(string, string);
	void setText(string);
	void setPattern(string);
	string getText();
	string getPattern();
	
	void setGoodHeuristic();
	void setBadHeuristic();
	void setGoodHeuristicLowerCase();
	void setBadHeuristicLowerCase();
	void printGoodHeuristic();
	//void printBadHeuristic();
	int getBHDistance(char);

	/*
	Keys for pattern search:
	0 - Standard
	1 - Any Case
	2 - Whole Word
	3 - Any Case Whole Word
	*/
	int stringSearch(int key);
	int findPattern();
	int findPatternLowerCase();
	int findPatternWholeWord();
	int findPatternWholeWordLowerCase();

	/*
	Keys for pattern search:
	0 - Standard
	1 - Any Case
	2 - Whole Word
	3 - Any Case Whole Word
	*/
	vector<int> stringSearchAll(int key);
	vector<int> findEveryPattern();
	vector<int> findEveryPatternWholeWord();
	vector<int> findEveryPatternLowerCase();
	vector<int> findEveryPatternLowerCaseWholeWord();

};

#endif // !"Boyer_More.H"
