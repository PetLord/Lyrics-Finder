#include "LyricsFinder.h"
#include "BoyerMore.h"
#include <cstring>
#include <string>
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


Ideas to add:
UI
Print lyrics
Pop/Found depending if it was the lyrics to be found
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

using namespace std;

namespace fs = filesystem;

string getArtistFromArtistPath(string ArtistPath)
{
	int pos = ArtistPath.find("\\");
	string artist = ArtistPath.substr(pos + 1, ArtistPath.length() - pos - 1);
	return artist;
}

string getSongNameFromSongPath(string songPath)
{
	string artist = getArtistFromArtistPath(songPath);
	int pos = artist.find("\\");
	string song = artist.substr(pos + 1, artist.length() - pos - 1 - 4); // cut '.txt' and artist name
	return song;
}

LyricsFinder::LyricsFinder(string bF)
{
	baseFolder = bF;
	loadAllSongs(); // fills the songs map
	boyer = Boyer();
}

void LyricsFinder::loadAllSongs()
{
	//ofstream out("log.txt");
	string artistPath;
	try {
		if (fs::exists(baseFolder) && fs::is_directory(baseFolder)) {
			for (const auto& artistEntry : fs::directory_iterator(baseFolder)) {
				if (fs::is_directory(artistEntry.path())) {
					artistPath = artistEntry.path().string();
					for (const auto& songEntry : fs::directory_iterator(artistEntry.path())) {
						if (songEntry.path().extension() == ".txt") {
							string songPath = songEntry.path().string();
							ifstream lyricsFile(songPath);
								if (lyricsFile.is_open()) {
									string lyrics;
									string line;
									while (!lyricsFile.eof())
									{
										std::getline(lyricsFile, line);
										lyrics += line + "\n";
									}

								lyricsFile.close();
								songs[getArtistFromArtistPath(artistPath)][getSongNameFromSongPath(songPath)] = lyrics;
								//out << "(\"" << getArtistFromArtistPath(artistPath) << "\",\"" << getSongNameFromSongPath(songPath) << "\")," << endl;
							}
						}
					}
				}
			}
		}
	}
	catch (const fs::filesystem_error& e) {
		cerr << "Filesystem error: " << e.what() << endl;
	}
	catch (const exception& e) {
		cout << artistPath << endl;
		cerr << "General error: " << e.what() << endl;
	}
}

void LyricsFinder::printSongsLoaded()
{
	for (const auto& artist : songs) {
		cout << artist.first << endl;
		for (const auto& song : artist.second) {
			cout << song.first << endl;
			//cout << song.second << endl;
		}
	}
}

void LyricsFinder::findLyrics(string keywords, int mode)
{
	latestMatches = priority_queue<pair<int, pair<string, string>>>();
	for (const auto& artist : songs) {
		for (const auto& song : artist.second)
		{
			boyer.setText(song.second);
			boyer.setPattern(keywords);
			vector<int> occurences;
			if (mode == 0)
			{
				occurences = boyer.findEveryPatternLowerCase();
			}
			else
			{
				occurences = boyer.findEveryPatternWholeWord();
			}

			if (occurences.size() > 0)
			{
				latestMatches.push(make_pair(occurences.size(), make_pair(artist.first, song.first)));
			}
		}
	}
}

void LyricsFinder::printTopHits(int k)
{
	int i = 0;
	while (!latestMatches.empty() && i < k)
	{
		cout << latestMatches.top().second.first << ": " << latestMatches.top().second.second << ": " << latestMatches.top().first << " occurences" << endl;
		latestMatches.pop();
		i++;
	}
}

// -1 returns all occurences
vector<pair<int, pair<string, string>>> LyricsFinder::getTopHits(int k)
{
	if (k == -1)
	{
		k=latestMatches.size();
	}

	vector<pair<int, pair<string, string>>> topHits;
	int i = 0;
	while (!latestMatches.empty() && i < k)
	{
		topHits.push_back(make_pair(latestMatches.top().first, latestMatches.top().second));
		latestMatches.pop();
		i++;
	}
	return topHits;
}

string LyricsFinder::getLyrics(string artist, string song)
{
	return songs[artist][song];
}