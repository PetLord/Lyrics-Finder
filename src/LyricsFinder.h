/*
Project Description - LyricsFinder:
Given a dataset of music lyrics in a folder system (songs/ArtistName/SongName.txt), 
this program is made to find songs by a given keyword or key sentence that has atleast 1 occurence in the lyrics.
In LyricsFinder class constructor, I want to load every lyrics to a multimap.
The Songs map consists of the Artist name and a map
that consists of SongName and Lyrics (Map<ArtistName, Map<SongName,Lyrics>>).
LyricsFinder will have a call function that initializes
a Boyer Moore class with pattern="keywords" and run through
the map of Lyrics and set the Text as the Boyer More's Text and run the algorithm. 
It counts the occurences and stores the pair of pairs: 
pair<NumberOfOccurencesInLyrics, pair<ArtistName, SongName>> in a priority queue,
sorted by the Number Of Occurences. At the end prints the top 3 hits.
*/

#ifndef LyricsFinder_H
#define LyricsFinder_H

#include "BoyerMore.h"
#include <vector>
#include <map>
#include <queue>

class LyricsFinder{
private:
	Boyer boyer;
	string baseFolder;
	map<string, map<string, string>> songs;
	priority_queue<pair<int, pair<string, string>>> latestMatches;
	

public:
	LyricsFinder(string baseFolder);
	void loadAllSongs();
	void printSongsLoaded();
	void findLyrics(string keywords, int mode);
	void printTopHits(int k);

	// Occurences, Artist, Song
	vector<pair<int, pair<string, string>>> getTopHits(int k);
	string getLyrics(string artist, string song);
};

#endif