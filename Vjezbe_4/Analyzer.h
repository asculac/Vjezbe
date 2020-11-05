#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

//ROOT headers
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TApplication.h>
#include <TString.h>

using namespace std;

// Declare class Analyzer
class Analyzer
{
  // public member declaration
  public:
    Analyzer( );
    ~Analyzer( );

    void ReadTxtFile(string);
    void ConvertTxtToRootFile(string, TString);

  // private member declaration
  private:
    int _EventNumber;

    string _DecayParticle1_Name;
    bool  _DecayParticle1_isBoson;
    float _DecayParticle1_mass;
    float _DecayParticle1_px;
    float _DecayParticle1_py;
    float _DecayParticle1_pz;
    float _DecayParticle1_E;
    float _DecayParticle1_pt;

    string _DecayParticle2_Name;
    bool  _DecayParticle2_isBoson;
    float _DecayParticle2_mass;
    float _DecayParticle2_px;
    float _DecayParticle2_py;
    float _DecayParticle2_pz;
    float _DecayParticle2_E;
    float _DecayParticle2_pt;

    ifstream myReadFile;
    string line;
    bool _skipFirstLine;
    TTree *tree;

};