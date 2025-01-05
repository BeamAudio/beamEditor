#ifndef MEMORY_UNIT_H
#define MEMORY_UNIT_H

#include<vector>

using namespace std;

//MemUnit is active during the whole session, so we can return references to the variables stored inside

class MemoryUnit {

private:

	vector<vector<vector<double>>> channels;
	double sampleRate;
	
public: 
	MemoryUnit();
	MemoryUnit(vector<vector<vector<double>>> channels, double sampleRate);

	vector<vector<vector<double>>>& getChannels();
	double& getSampleRate();




};

#endif MEMORY_UNIT_H