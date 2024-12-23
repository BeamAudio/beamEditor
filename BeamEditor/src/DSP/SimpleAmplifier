#include<array>

using namespace std;

class SimpleAmplifier : public Component {
  private:
    double alpha;

  public:
    SimpleAmplifier(const double& alpha) {
      this-> alpha = alpha;
    }

    array<double,100> processChunk(const array<double,100>& inChunk) {
      array<double,100> outChunk;
      outChunk.fill(0);
      for(size_t i = 0; i<inChunk.size(); i++) {
        outChunk[i] = inChunk[i] * alpha;
      }
      return outChunk;
    }

    
        

    
