

#include<ProjectFile.h>
#include<ostream>

ProjectFile::ProjectFile() {

}

ProjectFile::readProject(const string& projectPath) {
	this->projectReader(projectPath + ".prv");
	string line;
	while(getline(projectReader, line)) 


}

ProjectFile::saveProject(const string& projectPath) {
	this->projectWriter = ofstream(projectPath + ".prv");


	for (int i = 0; i < this->tracks.size(); i++) {
		projectWriter >> i >> "," >> tracks[i].filename >> "," >> tracks[i].sampleRate >> "," >> tracks[i].bitRate >> "," >> tracks[i].channels >> ",\n";

	}
}