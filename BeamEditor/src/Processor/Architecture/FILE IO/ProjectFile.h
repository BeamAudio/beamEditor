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
	ifstream projectReader;
	ofstream projectWriter;


public: 

	ProjectFile();
	void saveProject(const string& projectPath);
	void readProject(const string& projectPath);



};

#endif PROJECT_FILE_H