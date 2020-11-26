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

  analyzer->PlotHistogram("/eos/user/t/tsculac/BigStuff/4Ana/ggH125/ZZ4lAnalysis.root");
  analyzer->PlotHistogram("/eos/user/t/tsculac/BigStuff/4Ana/qqZZ/ZZ4lAnalysis.root");
  
  analyzer->PlotMass();
  analyzer->PlotDkin();

  delete analyzer;

  return 0;
}