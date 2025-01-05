#include<MemoryUnit.h>

vector<vector<vector<double>>>& MemoryUnit::getChannels() {
	return this->channels;
}

MemoryUnit::MemoryUnit() {
	this->sampleRate = 0.0;

}

MemoryUnit::MemoryUnit(vector<vector<vector<double>>> channels, double sampleRate) {
	this->channels = channels;
	this->sampleRate = sampleRate;
}

double MemoryUnit::getSampleRate() {
	return this->sampleRate;
}

