// My own class
#include <Analyzer.h>

int main()
{
  // Declare Analyzer
  Analyzer *analyzer;

  analyzer = new Analyzer();

  analyzer->ReadTxtFile("Analysis.txt");
  analyzer->ConvertTxtToRootFile("Analysis.txt","Analysis.root");

  delete analyzer;

  return 0;
}