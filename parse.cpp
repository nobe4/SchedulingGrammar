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

// useless functions we'll need later
void verifyTask(int taskNb, int constraint);
void addTask(int taskNb, int constraint);
void deleteTask(int taskNb, int constraint);

int main(int argc, char const *argv[])
{
    string s;
    getline(cin, s);
    vector<string> input = split(s);
    cout << "return from parse Input : " << parseInput(input) << endl;
    return 0;
}

bool parseInput(vector<string> const &input){
    // verifying the lenght of input : we accept 1 and 3
    if(input.size() == 1){
        // 1 input : quit the programm with "q" and restart it with "r"
        if(input[0] == "q"){
            cout << "implement function to quit here !" << endl;
        }else if(input[0] == "q"){
            cout << "implement fucntion to restart here !" << endl;
        }
    }else if(input.size() == 3){
        // 3 inputs : operation, task number and contraint
        // operation is either "a", "d" or "v"
        // task number and constraint are integers

        // ex : "v 1 2" will verify the task 1 with constraint 2

        // first : cast strings to integers   
        int taskNb = -1, constraint = -1; // variables for the operation we try to achieve
        // buffer to cast string to integer
        istringstream bufferTaskNb(input[1]);
        istringstream bufferConstraint(input[2]);

        // cast from the string to integer, if cast fails (string is not an integer) we got 0, 
        // so 0 will be assigned to verify so it wont change anything
        // floating point number will be rounded
        bufferTaskNb >> taskNb;
        bufferConstraint >> constraint;

        // next check operation and call the right function for each case
        if(input[0] == "a"){
            addTask(taskNb, constraint);
        }else if(input[0] == "d"){
            deleteTask(taskNb, constraint);
        }else if(input[0] == "v"){
            verifyTask(taskNb, constraint);
        }else{
            cout << "error : unknown operation !" << endl;
            return false;
        }
        return true;
    } else {
        cout << "error : bad number of input !" << endl;
        return false;
    }
}

vector<string> split(string const &input) {
    istringstream buffer(input);
    vector<string> ret((istream_iterator<string>(buffer)),istream_iterator<string>());
    return ret;
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