// Include classes
#include <Analyzer.h>

// Declaring default constructor
Analyzer::Analyzer()
{
}

// Destructor declaration
Analyzer::~Analyzer()
{
}

// Function for reading txt files
void Analyzer::ReadTxtFile(string filename)
{
  ifstream myReadFile;
  myReadFile.open(filename.c_str());
  string line;

  _skipFirstLine = true;

  if (myReadFile.is_open())
  {
    // Read the file line by line
    while(getline(myReadFile, line))
    {
        stringstream   linestream(line);

        // Since we know that first line of the file only describes data skip reading it into variables
        if (_skipFirstLine)
        {
          _skipFirstLine = false;
          continue;
        }

        // Read output and send it to dedicated variables
        linestream >> _EventNumber >> _DecayParticle1_Name >> _DecayParticle1_isBoson >> _DecayParticle1_mass >> _DecayParticle1_px >> _DecayParticle1_py >> _DecayParticle1_pz >> _DecayParticle1_E >> _DecayParticle1_pt >>
                                     _DecayParticle2_Name >> _DecayParticle2_isBoson >> _DecayParticle2_mass >> _DecayParticle2_px >> _DecayParticle2_py >> _DecayParticle2_pz >> _DecayParticle2_E >> _DecayParticle2_pt;

    }
  }

  myReadFile.close();
}




// Function for converting txt files to root files
void Analyzer::ConvertTxtToRootFile(string input_filename, TString output_filename)
{

  myReadFile.open(input_filename.c_str());

  _skipFirstLine = true;

  // Create a ROOT file for storing data
  TFile *hfile = TFile::Open(output_filename,"RECREATE");

  // Create a tree that will store our Higgs events
  tree = new TTree("Tree","My Higgs boson decay data");

  // Declare a branch for every variable that will go in the tree, set it to point to corresponding variables that will be used to fill the TTree
  tree->Branch("EventNumber",&_EventNumber,"_EventNumber/I");

  tree->Branch("DecayParticle1_Name",&_DecayParticle1_Name);//Use default leaflist for strings
  tree->Branch("DecayParticle1_isBoson",&_DecayParticle1_isBoson,"_DecayParticle1_isBoson/I");
  tree->Branch("DecayParticle1_mass",&_DecayParticle1_mass,"_DecayParticle1_mass/F");
  tree->Branch("DecayParticle1_px",&_DecayParticle1_px,"_DecayParticle1_px/F");
  tree->Branch("DecayParticle1_py",&_DecayParticle1_py,"_DecayParticle1_py/F");
  tree->Branch("DecayParticle1_pz",&_DecayParticle1_pz,"_DecayParticle1_pz/F");
  tree->Branch("DecayParticle1_E",&_DecayParticle1_E,"_DecayParticle1_E/F");
  tree->Branch("DecayParticle1_pt",&_DecayParticle1_pt,"_DecayParticle1_pt/F");

  tree->Branch("DecayParticle2_Name",&_DecayParticle2_Name);//Use default leaflist for strings
  tree->Branch("DecayParticle2_isBoson",&_DecayParticle2_isBoson,"_DecayParticle2_isBoson/I");
  tree->Branch("DecayParticle2_mass",&_DecayParticle2_mass,"_DecayParticle2_mass/F");
  tree->Branch("DecayParticle2_px",&_DecayParticle2_px,"_DecayParticle2_px/F");
  tree->Branch("DecayParticle2_py",&_DecayParticle2_py,"_DecayParticle2_py/F");
  tree->Branch("DecayParticle2_pz",&_DecayParticle2_pz,"_DecayParticle2_pz/F");
  tree->Branch("DecayParticle2_E",&_DecayParticle2_E,"_DecayParticle2_E/F");
  tree->Branch("DecayParticle2_pt",&_DecayParticle2_pt,"_DecayParticle2_pt/F");

  if (myReadFile.is_open())
  {
    while(getline(myReadFile, line))
    {
        stringstream linestream(line);

        // Since we know that first line of the file only describes data skip reading it into variables
        if (_skipFirstLine)
        {
          _skipFirstLine = false;
          continue;
        }

        // Read the file line and store it in dedicated variables
        linestream >> _EventNumber >> _DecayParticle1_Name >> _DecayParticle1_isBoson >> _DecayParticle1_mass >> _DecayParticle1_px >> _DecayParticle1_py >> _DecayParticle1_pz >> _DecayParticle1_E >> _DecayParticle1_pt >>
                                     _DecayParticle2_Name >> _DecayParticle2_isBoson >> _DecayParticle2_mass >> _DecayParticle2_px >> _DecayParticle2_py >> _DecayParticle2_pz >> _DecayParticle2_E >> _DecayParticle2_pt;

        // Fill one instance in the tree with branch values taking their values from addresses of dedicated variables
        tree->Fill();
    }
  }

  // Write our TTree in the currently opened ROOT file
  tree->Write();

  delete hfile;
}