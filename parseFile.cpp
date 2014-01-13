#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>

using namespace std;

// Cell of the adjacency matrix
// This structure is used to store values in adjacency matrix
// The field "valid" is null when the value is not valid,
// and has any other value otherwise.
// The field "value" is the weight of the link.
typedef struct _adjCell {
    int value, valid;
} AdjCell;


void parseFile(string path, vector<vector<AdjCell> > &adjMatrix);
// file synthax (tabs are actually spaces)
// 3                       number of tasks
// 1   10  2   3   -1      lenght (10) et constraint (2 et 3) of task 1
// 2   20  3   -1          lenght (20) et constraint (3) of task 2
// 3   30 -1               lenght (30) et constraint (aucune) of task 3
// -1                      eof

vector<string> split(string const &input);

// parse string to int and return it
// careful : 0 is either "0" of an invalid string parsed
int parseInt(string s);

//print adjency matrix
void print(const vector<vector<AdjCell> > &adjMatrix, int size);
 
int main(int argc, char const *argv[]){
    vector< vector<AdjCell> > adjMatrix;
    parseFile("grammar.g", adjMatrix);
    return 0;
}

void parseFile(string path, vector<vector<AdjCell> > &adjMatrix){
    
    ifstream file(path.c_str()); // open the file
    
    string line = ""; // buffer that will contains every line once at the time

    int nbTasks = -1;       // number of tasks found on first line
    int nbTasksParsed = 0;  // number of tasks parsed to make a verification
    
    vector<string> splittedLine;   // default value to prevent cross OS errors ? ###############################

    // if the file successfully opened
    if(file.is_open()){
        // while we get a new line we put it in the line string
        while(getline(file, line)){
            cout << endl << "lenght(" <<  (split(line)).size() << ") : " << line << endl;
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

                // resize the adgency matrix and set all value to not valid
                adjMatrix.resize(nbTasks + 2); // we considere the initial and final task
                for (int i = 0; i < adjMatrix.size(); ++i){
                    adjMatrix[i].resize(nbTasks + 2);
                    for (int j = 0; j < adjMatrix[j].size(); ++j){
                        adjMatrix[i][j].valid = false;
                    }
                }

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

                    // check if the line contains at least 3 elements : task number length and -1
                    if(splittedLine.size() < 3){
                        cout << "error : line too short !" << endl;
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
                    cout <<  constraints.size() << " constraints parsed !"<< endl;
                    
                    // set the line to the length
                    for (int i = 0; i < adjMatrix.size(); ++i) {
                        adjMatrix[parseInt(splittedLine[0])][i].value = parseInt(splittedLine[1]);
                    }

                    // if there is no constraint we set the edge to be the successor of the initial edge
                    if(constraints.size() == 0){
                        cout << "task n°" << parseInt(splittedLine[0]) << " of lenght " << parseInt(splittedLine[1]) << " without constraint "<< endl;
                        adjMatrix[0][parseInt(splittedLine[0])].valid = true;
                    }else{
                        // add the constraint to the adjency matrix
                        for (int i = 0; i < constraints.size(); ++i){
                            cout << "task n°" << parseInt(splittedLine[0]) << " of lenght " << parseInt(splittedLine[1]) << " with constraint " << constraints[i] << " added !" << endl;
                            adjMatrix[constraints[i]][parseInt(splittedLine[0])].valid = true;
                        }
                    }
                    nbTasksParsed ++;
                }
            }
        }
        // after we parsed the adjency matrix 

        // we add all edge without successor to be predecessor of final edge
        for (int i = 1; i < adjMatrix.size() - 1; ++i) {
            bool filled = false;
            for (int j = 1; j < adjMatrix.size() - 1; ++j){
                filled = adjMatrix[i][j].valid || filled;
            }

            //if not filled then the edge has no successor
            if(!filled){
                cout << "node " << i << " has no sucessor : we add it to the predecessors of the final node" << endl;
                adjMatrix[i][adjMatrix.size() - 1].valid = true;
            }
        }

        // we add all edge without predecessors to be successor of initial edge
        for (int i = 1; i < adjMatrix.size() - 1; ++i) {
            bool filled = false;
            for (int j = 1; j < adjMatrix.size() - 1; ++j){
                filled = adjMatrix[j][i].valid || filled;
            }

            //if not filled then the edge has no successor
            if(!filled){
                cout << "node " << i << " has no predecessor : we add it to the successors of the initial node" << endl;
                adjMatrix[0][i].valid = true;
            }
        }

        // print the matrix created
        cout << "Matrix created : " << endl;
        print(adjMatrix,adjMatrix.size());

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

void print(const vector<vector<AdjCell> > &adjMatrix, int size) {
    for (int j = 0; j < size; ++j) {
        cout << "\t" << j;
    }
    for (int i = 0; i < size; ++i) {
        cout << endl << i << "\t";
        for (int j = 0; j < size; ++j) {
            if (adjMatrix[i][j].valid) {
                cout << adjMatrix[i][j].value << "\t";
            } else {
                cout << ".\t";
            }
        }
    }
    cout << endl;
}
