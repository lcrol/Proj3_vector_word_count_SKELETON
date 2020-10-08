#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "../includes/constants.h"
#include "../includes/fileio.h"

bool is_empty(std::fstream& pFile);

/*if you are debugging the file must be in the project parent directory
  in this case Project2 with the .project and .cProject files*/
bool openFile(std::fstream& myfile, const std::string& myFileName,
		std::ios_base::openmode mode)
{
	myfile.open(myFileName, mode);
	if(is_empty(myfile)){
		closeFile(myfile);
		return false;
	}
	if(myfile.is_open())
		return true;
	return false;
}

/*iff myfile is open then close it*/
void closeFile(std::fstream& myfile)
{
	myfile.close();
}

/* serializes all content in entries to file outputfilename
 * check out utils for helpful type conversion functions
 * returns  FAIL_FILE_DID_NOT_OPEN if cannot open outputfilename
 * 			FAIL_NO_ARRAY_DATA if there are 0 entries in myEntryArray
 * 			SUCCESS if all data is written and outputfilename closes OK
 * */
int writetoFile(std::vector<constants::entry>  &entries, const std::string &outputfilename)
{
	std::ofstream my_ofile;
	my_ofile.open(outputfilename);
		if(!my_ofile.is_open())
			return constants::FAIL;

	for(int i=0; i<entries.size(); i++){
			my_ofile<<entries[i].word<<" "<<entries[i].number_occurences<<std::endl;
	}
	return constants::SUCCESS;

}

bool is_empty(std::fstream& pFile)
{
    return pFile.peek() == std::fstream::traits_type::eof();
}
