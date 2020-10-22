#include <iostream>
#include "ElementaryParticle.h"
using namespace std;


int main() {
	ElementaryParticle *higgs,*topquark;
	higgs=new ElementaryParticle();
	topquark=new ElementaryParticle();
	higgs->name="higgs";
	higgs->mass=125;
	higgs->boson=true;

	topquark->name="top quark";
	topquark->mass=180;
	topquark->boson=false;

	higgs->printInfo();
	topquark->printInfo();
    
    return 0;
}
