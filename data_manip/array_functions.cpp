#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

#include <vector>
#include "../includes/array_functions.h"
#include "../includes/constants.h"

using namespace std;
using namespace constants;

namespace KP {

	bool compareNameAsc(const entry &thisEntry, const entry &otherEntry);
	bool compareNameDec(const entry &thisEntry, const entry &otherEntry);
	bool compareNumberOfOccurances(const entry &thisEntry, const entry &otherEntry);
	string trim(const string& str, const string& whitespace);

	//zero out vector that tracks words and their occurrences
	void clear(vector<entry> &entries) {
		entries.clear();
	}

	//how many unique words are in the vector
	int getSize(vector<entry> &entries) {
		return entries.size();
	}

	//get data at a particular location, if i>size() then get the last value in the vector
	//(this is lazy, should throw an exception instead)
	string getWordAt(vector<entry> &entries, int i) {
		if(entries.size() == 0) return "";
		if ((unsigned) i >= entries.size()) {
			i = entries.size() - 1;
		}
		return entries.at(i).word;
	}

	int getNumbOccurAt(std::vector<constants::entry> &entries, int i) {
		if(entries.size() == 0) return FAIL;

		if ((unsigned) i >= entries.size()) {
			i = entries.size() - 1;
		}
		return entries.at(i).number_occurences;
	}

	/*loop through whole file, one line at a time
	 * call processLine on each line from the file
	 * returns false: myfstream is not open
	 *         true: otherwise*/
	bool processFile(std::vector<constants::entry> &entries,std::fstream &myfstream)
	{
		if(!myfstream.is_open())
				return false;

		string line;
		while(!myfstream.eof()){
			getline(myfstream, line);
			processLine(entries, line);
		}

		return true;
	}

	/*take 1 line and extract all the tokens from it
	 feed each token to processToken for recording*/
	void processLine(std::vector<constants::entry> &entries,std::string &myString)
	{
		string token;
		stringstream iss(myString);
		while(!iss.eof()){
			getline(iss, token, constants::CHAR_TO_SEARCH_FOR);
			processToken(entries, token);
		}

	}

	/*takes and input token and updates how many times it has been seen*/
	void processToken(vector<entry> &entries, string &token)
	{
		token = trim(token, " ");
		token = trim(token, "\r");
		token = trim(token, ".\r");
		token = trim(token, ".");
		token = trim(token, ",");

		if(token == "")
			return;
		string tokenUpper = token;
		bool isInEntries = false;
		transform(tokenUpper.begin(), tokenUpper.end(), tokenUpper.begin(), ::toupper);
		for (int i = 0; (unsigned) i < entries.size(); i++) {
			if (tokenUpper == entries[i].word_uppercase) {
				entries[i].number_occurences++;
				isInEntries = true;
				continue;
			}
		}
		if (!isInEntries) {
			entry ent;
			ent.word = token;
			ent.word_uppercase = tokenUpper;
			ent.number_occurences = 1;
			entries.push_back(ent);
		}
	}

	/*
	 * Sort myEntryArray based on so enum value.
	 * Please provide a solution that sorts according to the enum
	 * The presence of the enum implies a switch statement based on its value
	 * See the course lectures and demo project for how to sort a vector of structs
	 */
	void sort(vector<entry> &entries, sortOrder so) {
		if (so == ASCENDING) {
			sort(entries.begin(), entries.end(), compareNameAsc);
		} else if (so == DESCENDING) {
			sort(entries.begin(), entries.end(), compareNameDec);
		} else if (so == NUMBER_OCCURRENCES) {
			sort(entries.begin(), entries.end(), compareNumberOfOccurances);
		}
	}

	//utils
	bool compareNameAsc(const entry &thisEntry, const entry &otherEntry) {
		return thisEntry.word_uppercase <= otherEntry.word_uppercase;
	}
	bool compareNameDec(const entry &thisEntry, const entry &otherEntry) {
		return thisEntry.word_uppercase >= otherEntry.word_uppercase;
	}
	bool compareNumberOfOccurances(const entry &thisEntry,
			const entry &otherEntry) {
		return thisEntry.number_occurences >= otherEntry.number_occurences;
	}

	string trim(const string& str, const string& whitespace)
	{
	    const auto strBegin = str.find_first_not_of(whitespace);
	    if (strBegin == std::string::npos)
	        return "";

	    const auto strEnd = str.find_last_not_of(whitespace);
	    const auto strRange = strEnd - strBegin + 1;

	    return str.substr(strBegin, strRange);
	}
}
