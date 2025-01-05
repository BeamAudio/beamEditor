#ifndef PROJECT_FILE_H
#define PROJECT_FILE_H

#include<vector>
#include<string>
#include<AudioData.h>
#include<map>
#include<fstream>



using namespace std;

class ProjectFile {

private:
	
	map<int, AudioData> tracks;

public: 

	ProjectFile();
	void saveProject();
	void readProject();



};

#endif PROJECT_FILE_H