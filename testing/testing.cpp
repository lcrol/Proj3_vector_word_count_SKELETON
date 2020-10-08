//============================================================================
// Name        : testing.cpp
// Author      : KP
// Version     :
// Copyright   : Your copyright notice
// Description : tester, for arrayfunctions.cpp
//				 to autorun on linux, from the parent folder
//				 ./make clean
//				 ./make
//				 ./Debug/Proj3_vector_word_count
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include "../includes/fileio.h"
#include "../includes/constants.h"
#include "../includes/utilities.h"
#include "../includes/array_functions.h"

using namespace std;
using namespace constants;
using namespace KP;

//tracks how many points you will get out of 100
//yeah, yeah its a global
int total_points =0;

//this is a template class, its mostly here as a helper for me
//the T and U are generic params, I can substitute any type for them
//they must be comparable with ==, templates are extremely hard to get right BTW
template<typename T, typename U>
bool EXPECT_EQ(T expectedVal, U actualVal,string testnumb = "", int pts=ONE_POINTS){
	bool bout = (expectedVal == actualVal);
	if (bout){
		total_points+=pts;
		cout<<"SUCCESS "+testnumb<<" points:"<<total_points;
	}
	else{
		cout<<"FAIL exp: "<<expectedVal<<" act: "<<actualVal<<" test: "<<testnumb;
	}
	cout<<endl;
	return bout;
}

//runs a diff command, I'm using (GNU diffutils) 3.3
//this code cadged from stack overflow
bool diff_files(string testoutput,string correctfile, string testnumb = "" ){

	const int SMALL_BUFFER = 512;
	if (testoutput.length()== 0 || correctfile.length()==0)
		return false;

	//build shell command
	string command ="diff "+  testoutput + " " + correctfile;

	FILE *in;
	char buff[SMALL_BUFFER];

	if(!(in = popen(command.c_str(), "r")))
		return false;

	if (fgets(buff, sizeof(buff), in)==NULL)
		return true;
	else
		cout << buff;

	pclose(in);
	return false;
}

//gets rid of the legacy .out files
void remove_file_debris(){
	popen("rm ./output/*.out", "r");
}

//worth 17 points
void test_ProcessToken(){
	std::vector<constants::entry>  entries;
	clear(entries);
	EXPECT_EQ(0, getSize(entries),"1");

	//make sure you strip the rubbish chars
	string mystring = " ";
	processToken(entries,mystring);
	EXPECT_EQ(0, getSize(entries),"2");

	mystring = "\r ";
	processToken(entries,mystring);
	EXPECT_EQ(0, getSize(entries),"3");

	mystring = ".\r";
	processToken(entries,mystring);
	EXPECT_EQ(0, getSize(entries),"4");

	//start adding real chars
	mystring = "z";
	processToken(entries,mystring);
	EXPECT_EQ(1, getSize(entries),"5");

	mystring = "z";
	processToken(entries,mystring);
	EXPECT_EQ(1, getSize(entries),"6");

	//add 1 more so there are 3 z's
	mystring = "z";
	processToken(entries,mystring);

	//seen z twice now
	EXPECT_EQ((string)"z", getWordAt(entries,0),"7");
	EXPECT_EQ(3, getNumbOccurAt(entries,0),"8");

	//add some different words
	mystring = "lizard";
	processToken(entries,mystring);
	mystring = "is";
	processToken(entries,mystring);
	mystring = "green";
	processToken(entries,mystring);
	mystring = "green";
	processToken(entries,mystring);

	EXPECT_EQ(4, getSize(entries),"9");

	sort(entries,ASCENDING);
	EXPECT_EQ("green", getWordAt(entries,0),"10_A");
	EXPECT_EQ(2, getNumbOccurAt(entries,0),"11_A");
	EXPECT_EQ((string)"is", getWordAt(entries,1),"12_A");
	EXPECT_EQ(1, getNumbOccurAt(entries,1),"13_A");
	EXPECT_EQ((string)"lizard", getWordAt(entries,2),"14_A");
	EXPECT_EQ(1, getNumbOccurAt(entries,2),"15_A");
	EXPECT_EQ((string)"z", getWordAt(entries,3),"16_A");
	EXPECT_EQ(3, getNumbOccurAt(entries,3),"17_A");

	sort(entries,DESCENDING);
	EXPECT_EQ("z", getWordAt(entries,0),"10_D");
	EXPECT_EQ(3, getNumbOccurAt(entries,0),"11_D");
	EXPECT_EQ((string)"lizard", getWordAt(entries,1),"12_D");
	EXPECT_EQ(1, getNumbOccurAt(entries,1),"13_D");
	EXPECT_EQ((string)"is", getWordAt(entries,2),"14_D");
	EXPECT_EQ(1, getNumbOccurAt(entries,2),"15_D");
	EXPECT_EQ((string)"green", getWordAt(entries,3),"16_D");
	EXPECT_EQ(2, getNumbOccurAt(entries,3),"17_D");

	sort(entries,NUMBER_OCCURRENCES);
	EXPECT_EQ((string)"z", getWordAt(entries,0),"16_N");
	EXPECT_EQ(3, getNumbOccurAt(entries,0),"17_N");
	EXPECT_EQ("green", getWordAt(entries,1),"10_N");
	EXPECT_EQ(2, getNumbOccurAt(entries,1),"11_N");
}

//worth 12 points
void test_ProcessLine(){
	std::vector<constants::entry>  entries;
	clear(entries);
	EXPECT_EQ(0, getSize(entries),"18",TWO_POINTS);

	string mystring = "I would like a. bit of butter and A slice of toast.\r";
	processLine(entries,mystring);
	EXPECT_EQ(10, getSize(entries),"19",TWO_POINTS);

	//make sure 2 a's
	EXPECT_EQ("a", getWordAt(entries,3),"20",TWO_POINTS);
	EXPECT_EQ(2, getNumbOccurAt(entries,3),"21",TWO_POINTS);

	//make sure toast is correct
	EXPECT_EQ("toast", getWordAt(entries,9),"22",TWO_POINTS);
	EXPECT_EQ(1, getNumbOccurAt(entries,9),"23",TWO_POINTS);
}

//worth 20 points
void test_File_OpenClose(){
	fstream myfstream;

	//test bogus file
	string filename = TEST_DATA_NON_EXISTANT;
	EXPECT_EQ (false, openFile(myfstream, filename),"24",FIVE_POINTS);
	EXPECT_EQ (false, myfstream.is_open(),"25",FIVE_POINTS);

	//test real file
	filename = TEST_DATA_FULL;
	EXPECT_EQ (true, openFile(myfstream, filename),"26",FIVE_POINTS);
	EXPECT_EQ (true, myfstream.is_open(),"27",FIVE_POINTS);
	closeFile(myfstream);
}

//see if we can open, process, sort, and write the file back to disk
bool test_File(string filename, sortOrder mysortorder= NONE){
	std::vector<constants::entry>  entries;
	clear(entries);
	fstream myfstream;
	if (!openFile(myfstream, filename))
		return false;

	if (!processFile(entries,myfstream))
		return false;

	sort(entries,mysortorder);
	string name = TEST_DATA_FULL_OUT;
	if ( writetoFile(entries,name)!= SUCCESS)
		return false;

	//NOTE: C++ does not support try-finally
	//it expects you to use destructors instead
	//fine for object oriented programming
	//very restrictive for structured programming
	closeFile(myfstream);
	return true;
}

//worth 25
void test_system(){
	//worth 5
	EXPECT_EQ(false, test_File(TEST_DATA_EMPTY), "28",FIVE_POINTS);

	//worth 10 (no sorting)
	EXPECT_EQ(true,test_File(TEST_DATA_FULL),"29",FIVE_POINTS);

	//worth 10 (correct output)
	if (does_file_exist(TEST_DATA_FULL_OUT))
		EXPECT_EQ(true,diff_files(TEST_DATA_FULL_OUT, TEST_DATA_FULL_PROCESSED),"30",FIVE_POINTS);
	else
		cout<<"ERROR 30 "<<TEST_DATA_FULL_OUT<< " does not exist"<<endl;
		
	//worth 10 (sorting)
	EXPECT_EQ(true,test_File(TEST_DATA_FULL, ASCENDING),"31",TEN_POINTS);
	
	if (does_file_exist(TEST_DATA_FULL_OUT))
		EXPECT_EQ(true,diff_files(TEST_DATA_FULL_OUT,TEST_DATA_FULL_PROCESSED_SORTED),"32",THIRTEEN_POINTS);
	else
		cout<<"ERROR 32 "<<TEST_DATA_FULL_OUT<< " does not exist"<<endl;
}

//runs all tests, returns the score
int run_all_test(){
	//need this to make it equal 100
	total_points+=1;

	test_ProcessToken();
	test_ProcessLine();
	test_File_OpenClose();
	test_system();
	//remove_file_debris();
	return total_points;
}

