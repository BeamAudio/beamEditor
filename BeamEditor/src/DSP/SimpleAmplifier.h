#include<array>

using namespace std;

class SimpleAmplifier : public Component {
  private:
    double alpha;

  public:
    SimpleAmplifier(const double& alpha) {
      this-> alpha = alpha;
    }

    vector<double> processChunk(const vector<double>& inChunk) {
      vector<double> outChunk;
      
      for(size_t i = 0; i<inChunk.size(); i++) {
        outChunk.push_back(inChunk[i] * alpha);
      }
      return outChunk;
    }

    
        

    
