#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>

using namespace std;

void parseFile(string path);
// file synthax (tabs are actually spaces)
// 3                       number of tasks
// 1   10  2   3   -1      lenght (10) et constraint (2 et 3) of task 1
// 2   20  3   -1          lenght (20) et constraint (3) of task 2
// 3   30 -1               lenght (30) et constraint (aucune) of task 3
// -1                      eof

// add a task to the configuration
void addTask(int taskNb, int lenght, vector<int> constraints);

vector<string> split(string const &input);

// parse string to int and return it
// careful : 0 is either "0" of an invalid string parsed
int parseInt(string s);
 
int main(int argc, char const *argv[])
{
    parseFile("grammar.g");
    return 0;
}

void parseFile(string path){
    
    ifstream file(path.c_str()); // open the file
    
    string line = ""; // buffer that will contains every line once at the time

    int nbTasks = -1;       // number of tasks found on first line
    int nbTasksParsed = 0;  // number of tasks parsed to make a verification
    
    vector<string> splittedLine;   // default value to prevent cross OS errors ? ###############################

    // if the file successfully opened
    if(file.is_open()){
        // while we get a new line we put it in the line string
        while(getline(file, line)){
            cout << "lenght(" <<  (split(line)).size() << ") : " << line << endl;
            //  first line : we get the number of tasks and put it in a variables
            //  if negative or null, return because of error
            if(nbTasks == -1){
                // we cast the line to int 
                nbTasks = parseInt(line); 
                if(nbTasks < 1){
                    cout << "error : bad number of tasks !" << endl;
                    return;
                }
                cout << "number of tasks to be parsed : " << nbTasks << endl;
            } else { // not the first turn
                // we check for the eof
                if(line == "-1"){
                    cout << "end of file detected" << endl;
                    // process the end of file : 
                    // (1) check the number of parsed tasks 
                    if(nbTasksParsed != nbTasks){
                        cout << "error : not right number of tasks : " << nbTasksParsed << " instead of " << nbTasks << " !" << endl;
                        return;
                    }

                    // (2) what to do next ? ##############################

                    cout << "right number of tasks parsed !" << endl;
                }
                else{ // if not end of file
                    cout << "new line to be splitted : " << line << endl;
                    // we split the line into a vector to get each 
                    splittedLine = split(line);
                    cout << splittedLine.size() << " symbols splitten" << endl;
                    
                    // check if the line is not empty
                    if(splittedLine.size() == 0){
                        cout << "error : line empty !" << endl;
                        return;
                    }

                    // verify that the last one is "-1"
                    if(splittedLine[splittedLine.size() -1] != "-1"){
                        cout << "error : missing -1 at the end of the line" << endl;
                        return;
                    }

                    // someting else to check directly ? ############################

                    vector<int> constraints;
                    for (int i = 2; i < splittedLine.size() - 1 ; ++i) {
                        constraints.push_back(parseInt(splittedLine[i]));
                    }
                    cout <<  constraints.size() << " constraints parsed"<< endl;
                    addTask(parseInt(splittedLine[0]),parseInt(splittedLine[0]),constraints);

                    nbTasksParsed ++;
                }
            }
        }
        // we close the file
        file.close();
    }

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

void addTask(int taskNb, int lenght, vector<int> constraints){
    cout << "task n°" << taskNb << " of lenght " << lenght << " with " << constraints.size() << " constraints added !" << endl;
}

