#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include "BoyerMore.H"

using namespace std;

void Boyer::setBadHeuristic() {
	int m = pattern.length();

	// Initialize all characters with the length of the pattern
	for (int i = 0; i < 256; i++) {
		badHeuristic[(char)i] = m;
	}

	// Set the value for each character in the pattern
	for (int i = 0; i < m - 1; i++) {
		badHeuristic[pattern[i]] = m - 1 - i;
	}
}

void Boyer::setGoodHeuristic()
{
	int m = pattern.length();
	vector<int> suffixes(m, 0);
	vector<bool> prefix(m, false);

	// Step 1: Fill the suffixes array
	for (int i = m - 2; i >= 0; i--)
	{
		int j = i;
		while (j >= 0 && pattern[j] == pattern[m - 1 - i + j])
		{
			suffixes[i]++;
			j--;
		}
	}

	// Step 2: Fill the prefix array
	for (int i = 1; i < m; i++)
	{
		if (suffixes[i] == i)
		{
			prefix[i] = true;
		}
		else
		{
			prefix[i] = prefix[suffixes[i]];
		}
	}

	// Step 3: Fill the goodHeuristic map
	for (int i = 0; i < m; i++)
	{
		goodHeuristic[i] = m; // Initialize all positions with the length of the pattern
	}

	// Case 1: Suffix exists in the pattern
	for (int i = m - 1; i >= 0; i--)
	{
		if (suffixes[i] == i + 1) // Suffix from pattern[0..i] exists
		{
			for (int j = 0; j < m - 1 - i; j++)
			{
				if (goodHeuristic[j] == m) // If this position is not set yet
				{
					goodHeuristic[j] = m - 1 - i;
				}
			}
		}
	}

	// Case 2: Prefix of suffix exists at the end of the pattern
	for (int i = 0; i <= m - 2; i++)
	{
		goodHeuristic[m - 1 - suffixes[i]] = m - 1 - i;
	}
}

void Boyer::setBadHeuristicLowerCase()
{
	int m = pattern.length();

	// Initialize all characters with the length of the pattern
	for (int i = 0; i < 256; i++) {
		badHeuristic[(char)i] = m;
	}

	for (int i = 0; i < m - 1; i++) {
		badHeuristic[tolower(pattern[i])] = pattern.length() - 1 - i;
	}
}

void Boyer::setGoodHeuristicLowerCase()
{
	vector<int> suffixes;
	vector<bool> prefix;

	int m = pattern.length();
	suffixes.resize(m, 0);
	prefix.resize(m, false);

	// Step 1: Fill the suffixes array
	for (int i = m - 2; i >= 0; i--)
	{
		int j = i;
		while (j >= 0 && tolower(pattern[j]) == tolower(pattern[m - 1 - i + j]))
		{
			suffixes[i]++;
			j--;
		}
	}

	// Step 2: Fill the prefix array
	for (int i = 1; i < m; i++)
	{
		if (suffixes[i] == i)
		{
			prefix[i] = true;
		}
		else
		{
			prefix[i] = prefix[suffixes[i]];
		}
	}

	// Step 3: Fill the goodHeuristic map
	for (int i = 0; i < m; i++)
	{
		if (suffixes[i] == i)
		{
			goodHeuristic[i] = m - 1 - i;
		}
		else
		{
			goodHeuristic[i] = m - 1 - suffixes[i];
		}
	}
}

//void Boyer::printBadHeuristic()
//{
//	map<char, int>::iterator it;
//	for (it = badHeuristic.begin(); it != badHeuristic.end(); it++)
//	{
//		cout << it->first << " " << it->second << endl;
//	}
//}

void Boyer::printGoodHeuristic()
{
	cout << "Good Suffix table:" << endl << "{";
	for (int i = 0; i < goodHeuristic.size() - 1; i++)
	{
		cout << goodHeuristic[i] << ", ";
	}

	if (goodHeuristic.size() != 0)
	{
		cout << goodHeuristic[goodHeuristic.size() - 1] << "}";
	}

}

int Boyer::getBHDistance(char c)
{
	// If the character exists in the bad suffix heuristic table
	if (badHeuristic.count(c) != 0)
	{
		return badHeuristic[c];
	}
	else
	{
		return pattern.length();
	}
}

Boyer::Boyer() {
	text = "";
	pattern = "";
}

Boyer::Boyer(string t, string p)
{
	text = t;
	pattern = p;

	setGoodHeuristic();
	setBadHeuristic();

}

void Boyer::setText(string t)
{
	text = t;
}

void Boyer::setPattern(string p)
{
	pattern = p;
}

string Boyer::getText()
{
	return text;
}

string Boyer::getPattern()
{
	return pattern;
}

/*
	Keys for pattern search:
	0 - Standard
	1 - Any Case
	2 - Whole Word
	3 - Any Case Whole Word
*/
int Boyer::stringSearch(int key)
{
	switch (key)
	{
	case 1:
		
			return findPatternLowerCase();
			break;
		
	case 2:
		
			return findPatternWholeWord();
			break;
		
	case 3:
		
			return findPatternWholeWordLowerCase();
			break;
		
	default:
		
			return findPattern();
			break;
		
	}
		
}

int Boyer::findPattern()
{
	int patternStart = 0; // ezen az indexen van az elso elem
	int patternEnd = pattern.size() - 1; // ezen az indexen van az utolso elem

	while (patternEnd < text.length())
	{
		bool match = true;
		int elementAtPattern = pattern.length() - 1;

		int i;
		for (i = patternEnd; i >= patternStart; i--)
		{
			if (text[i] != pattern[elementAtPattern])
			{
				match = false;
				break;
			}
			elementAtPattern--;
		}

		if (match)
		{
			return patternStart;
		}
		else
		{
			int badHeuristicDistance = getBHDistance(text[patternEnd]);
			int goodHeuristicDistance = goodHeuristic[elementAtPattern + 1];

			patternStart += max(badHeuristicDistance, goodHeuristicDistance);
			patternEnd += max(badHeuristicDistance, goodHeuristicDistance);
		}
	}

	
	return -1;
}

int Boyer::findPatternLowerCase()
{
	string originalText;
	string originalPattern;
	map<char, int> OriginalGoodHeuristic;
	OriginalGoodHeuristic.insert(goodHeuristic.begin(), goodHeuristic.end());
	map<char, int> OriginalBadHeuristic;
	OriginalBadHeuristic.insert(badHeuristic.begin(), badHeuristic.end());
	setBadHeuristicLowerCase();
	setGoodHeuristicLowerCase();


	originalText = getText();
	originalPattern = getPattern();

	for (int i = 0; i < text.length(); i++)
	{
		text[i]=(tolower(text[i]));
	}

	for (int i = 0; i < pattern.length(); i++)
	{
		pattern[i]=(tolower(pattern[i]));
	}

	int patternStart = 0; // ezen az indexen van az elso elem
	int patternEnd = pattern.size() - 1; // ezen az indexen van az utolso elem

	while (patternEnd < text.length())
	{
		bool match = true;
		int elementAtPattern = pattern.length() - 1;

		int i;
		for (i = patternEnd; i >= patternStart; i--)
		{
			if (text[i] != pattern[elementAtPattern])
			{
				match = false;
				break;
			}
			elementAtPattern--;
		}

		if (match)
		{
			setText(originalText);
			setPattern(originalPattern);
			return patternStart;
		}
		else
		{
			int badHeuristicDistance = getBHDistance(text[patternEnd]);
			int goodHeuristicDistance = goodHeuristic[elementAtPattern + 1];

			patternStart += max(badHeuristicDistance, goodHeuristicDistance);
			patternEnd += max(badHeuristicDistance, goodHeuristicDistance);
		}
	}


	goodHeuristic.insert(OriginalGoodHeuristic.begin(), OriginalGoodHeuristic.end());
	badHeuristic.insert(OriginalBadHeuristic.begin(), OriginalBadHeuristic.end());
	setGoodHeuristic();
	setBadHeuristic();
	setText(originalText);
	setPattern(originalPattern);
	return -1;
}

int Boyer::findPatternWholeWord()
{
	int patternStart = 0; // ezen az indexen van az elso elem
	int patternEnd = pattern.size() - 1; // ezen az indexen van az utolso elem

	while (patternEnd < text.length())
	{
		bool match = true;
		int elementAtPattern = pattern.length() - 1;

		int i;
		for (i = patternEnd; i >= patternStart; i--)
		{
			if (text[i] != pattern[elementAtPattern])
			{
				match = false;
				break;
			}
			elementAtPattern--;
		}

		if (match)
		{
			bool spaceBefore = false;
			bool spaceAfter = false;

			if (patternStart == 0 || text[patternStart - 1] == ' ')
			{
				spaceBefore = true;
			}

			if (patternEnd == text.length() - 1 || text[patternEnd + 1] == ' ' || text[patternEnd + 1] == '.' || text[patternEnd + 1] == ',' || text[patternEnd + 1] == '!' || text[patternEnd + 1] == '?')
			{
				spaceAfter = true;
			}

			if (spaceBefore && spaceAfter)
			{
				return patternStart;
			}
			else
			{
				int badHeuristicDistance = getBHDistance(text[patternEnd]);
				int goodHeuristicDistance = goodHeuristic[elementAtPattern + 1];

				patternStart += max(badHeuristicDistance, goodHeuristicDistance);
				patternEnd += max(badHeuristicDistance, goodHeuristicDistance);
			}
		}
		else
		{
			int badHeuristicDistance = getBHDistance(text[patternEnd]);
			int goodHeuristicDistance = goodHeuristic[elementAtPattern + 1];

			patternStart += max(badHeuristicDistance, goodHeuristicDistance);
			patternEnd += max(badHeuristicDistance, goodHeuristicDistance);
		}
	}

	return -1;
}

int Boyer::findPatternWholeWordLowerCase()
{
	string originalText;
	string originalPattern;
	map<char, int> OriginalGoodHeuristic;
	OriginalGoodHeuristic.insert(goodHeuristic.begin(), goodHeuristic.end());
	map<char, int> OriginalBadHeuristic;
	OriginalBadHeuristic.insert(badHeuristic.begin(), badHeuristic.end());
	setBadHeuristicLowerCase();
	setGoodHeuristicLowerCase();


	originalText = getText();
	originalPattern = getPattern();

	for (int i = 0; i < text.length(); i++)
	{
		text[i] = (tolower(text[i]));
	}

	for (int i = 0; i < pattern.length(); i++)
	{
		pattern[i] = (tolower(pattern[i]));
	}

	int patternStart = 0; // ezen az indexen van az elso elem
	int patternEnd = pattern.size() - 1; // ezen az indexen van az utolso elem

	while (patternEnd < text.length())
	{
		bool match = true;
		int elementAtPattern = pattern.length() - 1;

		int i;
		for (i = patternEnd; i >= patternStart; i--)
		{
			if (text[i] != pattern[elementAtPattern])
			{
				match = false;
				break;
			}
			elementAtPattern--;
		}

		if (match)
		{
			bool spaceBefore = false;
			bool spaceAfter = false;

			if (patternStart == 0 || text[patternStart - 1] == ' ')
			{
				spaceBefore = true;
			}

			if (patternEnd == text.length() - 1 || text[patternEnd + 1] == ' ' || text[patternEnd + 1] == '.' || text[patternEnd + 1] == ',' || text[patternEnd + 1] == '!' || text[patternEnd + 1]=='?')
			{
				spaceAfter = true;
			}

			if (spaceBefore && spaceAfter)
			{
				setText(originalText);
				setPattern(originalPattern);
				return patternStart;
			}
			else
			{
				int badHeuristicDistance = getBHDistance(text[patternEnd]);
				int goodHeuristicDistance = goodHeuristic[elementAtPattern + 1];

				patternStart += max(badHeuristicDistance, goodHeuristicDistance);
				patternEnd += max(badHeuristicDistance, goodHeuristicDistance);
			}
		}
		else
		{
			int badHeuristicDistance = getBHDistance(text[patternEnd]);
			int goodHeuristicDistance = goodHeuristic[elementAtPattern + 1];

			patternStart += max(badHeuristicDistance, goodHeuristicDistance);
			patternEnd += max(badHeuristicDistance, goodHeuristicDistance);
		}
	}

	goodHeuristic.insert(OriginalGoodHeuristic.begin(), OriginalGoodHeuristic.end());
	badHeuristic.insert(OriginalBadHeuristic.begin(), OriginalBadHeuristic.end());
	setGoodHeuristic();
	setBadHeuristic();
	setText(originalText);
	setPattern(originalPattern);
	return -1;
}

vector<int> Boyer::stringSearchAll(int key)
{
	switch (key)
	{
	case 1:

		return findEveryPatternLowerCase();
		break;

	case 2:

		return findEveryPatternWholeWord();
		break;

	case 3:

		return findEveryPatternLowerCaseWholeWord();
		break;

	default:
		return findEveryPattern();
		break;
	}
}

vector<int> Boyer::findEveryPattern()
{
	vector<int> solution;
	int patternStart = 0; // ezen az indexen van az elso elem
	int patternEnd = pattern.size() - 1; // ezen az indexen van az utolso elem

	while (patternEnd < text.length())
	{
		bool match = true;
		int elementAtPattern = pattern.length() - 1;

		int i;
		for (i = patternEnd; i >= patternStart; i--)
		{
			if (text[i] != pattern[elementAtPattern])
			{
				match = false;
				break;
			}
			elementAtPattern--;
		}

		if (match)
		{
			solution.push_back(patternStart);
		}

		int badHeuristicDistance = getBHDistance(text[patternEnd]);
		int goodHeuristicDistance = goodHeuristic[elementAtPattern + 1];

		patternStart += max(badHeuristicDistance, goodHeuristicDistance);
		patternEnd += max(badHeuristicDistance, goodHeuristicDistance);

	}

	return solution;
}

vector<int> Boyer::findEveryPatternLowerCase()
{
	string originalText;
	string originalPattern;
	map<char, int> OriginalGoodHeuristic;
	OriginalGoodHeuristic.insert(goodHeuristic.begin(), goodHeuristic.end());
	map<char, int> OriginalBadHeuristic;
	OriginalBadHeuristic.insert(badHeuristic.begin(), badHeuristic.end());
	setBadHeuristicLowerCase();
	setGoodHeuristicLowerCase();

	originalText = getText();
	originalPattern = getPattern();

	for (int i = 0; i < text.length(); i++)
	{
		text[i] = (tolower(text[i]));
	}

	for (int i = 0; i < pattern.length(); i++)
	{
		pattern[i] = (tolower(pattern[i]));
	}

	vector<int> solution;
	int patternStart = 0; // ezen az indexen van az elso elem
	int patternEnd = pattern.size() - 1; // ezen az indexen van az utolso elem

	while (patternEnd < text.length())
	{
		bool match = true;
		int elementAtPattern = pattern.length() - 1;

		int i;
		for (i = patternEnd; i >= patternStart; i--)
		{
			if (text[i] != pattern[elementAtPattern])
			{
				match = false;
				break;
			}
			elementAtPattern--;
		}

		if (match)
		{
			solution.push_back(patternStart);
		}

		int badHeuristicDistance = getBHDistance(text[patternEnd]);
		int goodHeuristicDistance = goodHeuristic[elementAtPattern + 1];

		patternStart += max(badHeuristicDistance, goodHeuristicDistance);
		patternEnd += max(badHeuristicDistance, goodHeuristicDistance);

	}

	goodHeuristic.insert(OriginalGoodHeuristic.begin(), OriginalGoodHeuristic.end());
	badHeuristic.insert(OriginalBadHeuristic.begin(), OriginalBadHeuristic.end());
	setGoodHeuristic();
	setBadHeuristic();
	setText(originalText);
	setPattern(originalPattern);
	return solution;
}

vector<int> Boyer::findEveryPatternWholeWord()
{
	vector<int> solution;
	int patternStart = 0; // ezen az indexen van az elso elem
	int patternEnd = pattern.size() - 1; // ezen az indexen van az utolso elem

	while (patternEnd < text.length())
	{
		bool match = true;
		int elementAtPattern = pattern.length() - 1;

		int i;
		for (i = patternEnd; i >= patternStart; i--)
		{
			if (text[i] != pattern[elementAtPattern])
			{
				match = false;
				break;
			}
			elementAtPattern--;
		}

		if (match)
		{
			bool spaceBefore = false;
			bool spaceAfter = false;

			if (patternStart == 0 || text[patternStart - 1] == ' ')
			{
				spaceBefore = true;
			}

			if (patternEnd == text.length() - 1 || text[patternEnd + 1] == ' ' || text[patternEnd + 1] == '.' || text[patternEnd + 1] == ',' || text[patternEnd + 1] == '!' || text[patternEnd + 1] == '?')
			{
				spaceAfter = true;
			}

			if (spaceBefore && spaceAfter)
			{
				solution.push_back(patternStart);
			}
		}

		int badHeuristicDistance = getBHDistance(text[patternEnd]);
		int goodHeuristicDistance = goodHeuristic[elementAtPattern + 1];

		patternStart += max(badHeuristicDistance, goodHeuristicDistance);
		patternEnd += max(badHeuristicDistance, goodHeuristicDistance);

	}

	return solution;
}

vector<int> Boyer::findEveryPatternLowerCaseWholeWord()
{
	string originalText;
	string originalPattern;
	map<char, int> OriginalGoodHeuristic;
	OriginalGoodHeuristic.insert(goodHeuristic.begin(), goodHeuristic.end());
	map<char, int> OriginalBadHeuristic;
	OriginalBadHeuristic.insert(badHeuristic.begin(), badHeuristic.end());
	setBadHeuristicLowerCase();
	setGoodHeuristicLowerCase();

	originalText = getText();
	originalPattern = getPattern();

	for (int i = 0; i < text.length(); i++)
	{
		text[i] = (tolower(text[i]));
	}

	for (int i = 0; i < pattern.length(); i++)
	{
		pattern[i] = (tolower(pattern[i]));
	}

	vector<int> solution;
	int patternStart = 0; // ezen az indexen van az elso elem
	int patternEnd = pattern.size() - 1; // ezen az indexen van az utolso elem

	while (patternEnd < text.length())
	{
		bool match = true;
		int elementAtPattern = pattern.length() - 1;

		int i;
		for (i = patternEnd; i >= patternStart; i--)
		{
			if (text[i] != pattern[elementAtPattern])
			{
				match = false;
				break;
			}
			elementAtPattern--;
		}

		if (match)
		{
			bool spaceBefore = false;
			bool spaceAfter = false;

			if (patternStart == 0 || text[patternStart - 1] == ' ')
			{
				spaceBefore = true;
			}

			if (patternEnd == text.length() - 1 || text[patternEnd + 1] == ' ' || text[patternEnd + 1] == '.' || text[patternEnd + 1] == ',' || text[patternEnd + 1] == '!' || text[patternEnd + 1] == '?')
			{
				spaceAfter = true;
			}

			if (spaceBefore && spaceAfter)
			{
				solution.push_back(patternStart);
			}
		}

		int badHeuristicDistance = getBHDistance(text[patternEnd]);
		int goodHeuristicDistance = goodHeuristic[elementAtPattern + 1];

		patternStart += max(badHeuristicDistance, goodHeuristicDistance);
		patternEnd += max(badHeuristicDistance, goodHeuristicDistance);


	}

	goodHeuristic.insert(OriginalGoodHeuristic.begin(), OriginalGoodHeuristic.end());
	badHeuristic.insert(OriginalBadHeuristic.begin(), OriginalBadHeuristic.end());
	setGoodHeuristic();
	setBadHeuristic();
	setText(originalText);
	setPattern(originalPattern);

	return solution;
}