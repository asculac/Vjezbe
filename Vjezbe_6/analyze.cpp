// C++
#include <iostream>
#include <fstream>

// My own class
#include <Analyzer.h>

using namespace std;

int main()
{
  // Declare Analyzer
  Analyzer *analyzer;

  analyzer = new Analyzer();

  analyzer->PlotHistogram("/home/public/data/ggH125/ZZ4lAnalysis.root");
  analyzer->PlotMass();

  delete analyzer;

  return 0;
}