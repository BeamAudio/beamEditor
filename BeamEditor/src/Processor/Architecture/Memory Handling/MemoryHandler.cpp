#include "MemoryHandler.h"

MemoryHandler::MemoryHandler()
{
}

void MemoryHandler::loadProject(const string& projectPath)
{
}

void MemoryHandler::saveProject(const string& projectPath, const string& projectName)
{
}

vector<vector<vector<vector<double>>>> MemoryHandler::getChunks()

{
	

	return utils.makeChunks(this->memory.getChannels());;
}

vector<vector<vector<double>>> MemoryHandler::getChannels()
{
	return this->memory.getChannels();
}


