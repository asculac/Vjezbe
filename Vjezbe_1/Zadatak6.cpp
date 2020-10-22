#include <iostream>
using namespace std;

class Elementary_Particle{
 	public:	
		string name;
		int mass;
		bool boson;
		void printInfo(){
			cout<<"name: "<< name << endl;
			cout<< "mass: " << mass << endl;
			cout << "boson: " << boson << endl;
	
		};
};


int main() {
	Elementary_Particle *higgs,*topquark;
	higgs=new Elementary_Particle();
	topquark=new Elementary_Particle();
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

