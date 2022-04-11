#include <iostream>
#include<string>
#include<fstream>
#include<set>
#include "SimpleGraph.h"

using std::cout;	using std::endl;
using namespace std;

void Welcome();
void read();

// Main method
int main() {
    Welcome();
    cout << "please input the filename" << endl;
    string filename;
    getline(cin,filename);
    set<int> rela;
    ifstream input;
    return 0;
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}

void read(ifstream &input,set<int> &relastions,string filename)
{
    input.open(filename);
    SimpleGraph graph;

}
