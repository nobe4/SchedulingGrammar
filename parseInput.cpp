#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

using namespace std;

// function used to parse input string from keyboard
bool parseInput(vector<string> const &input);

// split the input string from console with whitespaces
vector<string> split(string const &input);

// parse string to int and return it
// careful : 0 is either "0" of an invalid string parsed
int parseInt(string s);

// useless functions we'll need later
void verifyTask(int taskNb, int constraint);
void addTask(int taskNb, int constraint);
void deleteTask(int taskNb, int constraint);

//int main(int argc, char const *argv[])
//{
//    string s;
//    getline(cin, s);
//    vector<string> input = split(s);
//    cout << "return from parse Input : " << parseInput(input) << endl;
//    return 0;
//}

bool parseInput(vector<string> const &input){
    
}

vector<string> split(string const &input) {
    istringstream buffer(input);
    vector<string> ret((istream_iterator<string>(buffer)),istream_iterator<string>());
    return ret;
}

int parseInt(string s){
    istringstream buffer(s);
    int r = 0;
    buffer >> r;
    return r;
}

void verifyTask(int taskNb, int constraint){
    cout << "task n°" << taskNb << " with constraint n°" << constraint << " verified !" << endl;
}

void addTask(int taskNb, int constraint){
    cout << "task n°" << taskNb << " with constraint n°" << constraint << " added !" << endl;
}

void deleteTask(int taskNb, int constraint){
    cout << "task n°" << taskNb << " with constraint n°" << constraint << " deleted !" << endl;
}

