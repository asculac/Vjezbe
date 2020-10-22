#include <iostream>
using namespace std;


class ElementaryParticle{
	
 	public:	
		ElementaryParticle();
		~ElementaryParticle();
		string name;
		int mass;
		bool boson;
		void printInfo();
};
