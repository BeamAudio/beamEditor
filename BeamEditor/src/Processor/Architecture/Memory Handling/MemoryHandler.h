#ifndef MEMORY_HANDLER_H
#define MEMORY_HANDLER_H

#include<Processor/Architecture/FILE IO/MemoryLoader.h>
#include<MemoryUnit/MemoryUnit.h>
using namespace std;

class MemoryHandler {
private:

	MemoryLoader loader;
	MemoryUnit memory;

public: 
	MemoryHandler();
	void loadProject(const string& projectPath);
	void saveProject(const string& projectPath, const string& projectName);
	vector<vector<vector<vector<double>>>> getChunks();
	vector<vector<vector<double>>> getChannels();
	
};