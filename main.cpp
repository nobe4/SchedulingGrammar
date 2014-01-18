//
//
//  tp4graphes
//  Created by Matthieu de La Roche Saint-Andre and Victor Haffreingue on 01/13/2014.
//

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
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

// Link in the graph.
// This structure is used to store a link.
// The field "value" is the weight of the link.
// The field "edge1" is the first extremity of the link.
// The field "value" is the second extremity of the link.
typedef struct _link {
    int value, edge1, edge2;
} Link;

typedef vector<vector<AdjCell> > AdjMatrix;

void print(const AdjMatrix &adjMatrix); // prints adjacency matrix

bool circuit(const AdjMatrix &adjMatrix); // checks circuits presence in adjacency matrix

/*
 * Parses a file, and stores the result in adjMatrix.
 */
bool parseFile(string path, AdjMatrix &adjMatrix);

bool hasPredecessor(int node, const AdjMatrix &adjMatrix); // checks whether node has predecessor(s)
bool hasSuccessor(int node, const AdjMatrix &adjMatrix); // checks whether node has successor(s)

/*
 * Reads a line from user and modify adjMatrix based on that order.
 * You can either add (a), delete (d), verify constraint existence (v), stop (s)
 * (If ever you do not have any modification to perform.)
 *
 * Returns true if an operation has been performed, return false otherwise.
 */
bool executeCommand(AdjMatrix &adjMatrix);

/*
 * Computes earliest and latest dates based on adjMatrix.
 */
void computeDates(const AdjMatrix &adjMatrix, vector<int> &earliestDates, vector<int> &latestDates);

/*
 * Prints earliest and latest dates.
 */
void printCalendar(vector<int> &earliestDates, vector<int> &latestDates);

// split the input string from console with whitespaces
vector<string> split(string const &input);

// parse string to int and return it
// careful : 0 is either "0" of an invalid string parsed
int parseInt(string s);

int main(int argc, const char * argv[]) {
    string path;
    //    if (argc == 2) {
    //        path = argv[1];
    //    } else {
    //        cout << "Which file do you want to open ? ";
    //        cin >> path;
    //        cout << endl;
    //    }
    //    path = "/Users/matthieudelaro/Documents/classes/L3/S5/theorie_des_graphes/TP/tp33/Haffreingue-de_La_Roche_Saint_Andre-EFREI-L3-TG-TP3-5.txt";
    // path = "/Users/matthieudelaro/Documents/classes/L3/S5/theorie_des_graphes/TP/tp4/grammar.g";
  path = "grammar.g";
    // path = "/Users/matthieudelaro/Documents/classes/L3/S5/theorie_des_graphes/TP/tp4/grammar.g";
    //   path = "grammars.g";
    
    AdjMatrix adjMatrix;
    cout << "Parsing file " << path << " :" << endl;

    if(!parseFile(path, adjMatrix)){
        cout << "error while parsing the file, exiting ... " << endl;
        return 1;
    }
    
    bool goOn = true;
    string input = "";
    while (goOn) {
        print(adjMatrix);
        if (circuit(adjMatrix)) {
            cout << "There is one or several circuits. Please delete them." << endl;
            executeCommand(adjMatrix);
        } else {
            cout << "There is not any circuit." << endl;
            vector<int> latestDates, earliestDates;
            computeDates(adjMatrix, earliestDates, latestDates);
            
            cout << "Here is the calendar :" << endl;
            printCalendar(earliestDates, latestDates);
            cout << endl;
            
            cout << "Would you like to operate on the matrix ? (y/n)" << endl;
            getline(cin, input);
            if (input == "y" || input == "yes") {
                goOn = executeCommand(adjMatrix);
            }
            else if(input == "n" || input == "no"){
                goOn = false;
            }
        }
    }

    cout << "Thanks for using this programm, see you soon ! " << endl << endl;

    return 0;
}

bool circuit(const AdjMatrix &adjMatrix) {
    long size = adjMatrix.size();
    
    // Create an array the size of the matrix.
    // For each node i, deleted[i]
    // stores whether i has been deleted.
    vector<bool> deleted(size, false);
    
    // Each time we find a new node,
    // we incremente the value of deletedQuantity.
    // This way, we know when we reach the end
    // of the algorithm (deletedQuantity == size).
    int deletedQuantity = 0;
    
    // Used to store previous
    // value of foundQuantity.
    int previousDeletedQuantity = 0;
    
    int current = 0;
    bool goOn = true;
    while (deletedQuantity < size && goOn) {
        if (deleted[current] == false) {
            // if the node is an entry point in the graph,
            // we delete this node.
            bool foundPredecessor = false;
            int i = 0;
            while (!foundPredecessor && i < adjMatrix.size() ) {
                if (adjMatrix[i][current].valid && !deleted[i]) {
                    foundPredecessor = true;
                }
                i++;
            }
            
            
            if (!foundPredecessor) {
                deleted[current] = true;
                deletedQuantity++;
                // cout << current << " has at least one predecessor. We delete it." << endl;
            }
        }
        
        if (current == size - 1) {
            if (deletedQuantity == previousDeletedQuantity) {
                goOn = false;
            } else {
                previousDeletedQuantity = deletedQuantity;
                current = 0;
            }
        } else {
            current++;
        }
    }
    
    cout << endl;
    cout << endl;
    if (deletedQuantity == size) {
       // cout << "Each node has been deleted." << endl;
        return false;
    } else {
        // cout << "Some node(s) has not been deleted." << endl;
        return true;
    }

    return true;
}

bool executeCommand(AdjMatrix &adjMatrix) {
    string s;
    bool goOn = true;
    while(goOn) {
        print(adjMatrix);
        cout << "Would you like to add (a), delete (d), or verify (v) the existence of a constraint (help : h) ?" << endl; 
        cout << "$ input > ";
        getline(cin, s);
        if (s == "stop" || s == "s") {
            goOn = false;
            return false;
        }else if (s == "help" || s == "h")  {
            cout << "######################################################" << endl;
            cout << "#####                  HELP                       ####" << endl;
            cout << "######################################################" << endl;
            cout << "3 inputs : operation, task number and contraint" << endl;
            cout <<  "operation is either 'a', 'd' or 'v'" << endl;
            cout << "task number and constraint are integers" << endl;
            cout << "ex : 'v 1 2' will verify the task 1 with constraint 2" << endl;
            cout << "     'a 1 2' will add the constraint 1 to 2" << endl;
            cout << "     'd 1 2' will remove the constraint 1 to 2" << endl << endl << endl << endl;
        } 
        else {
            // split the input by whitespaces
            vector<string> input = split(s);
            // verifying the lenght of input : we accept 1 and 3
            if(input.size() == 3){
                // 3 inputs : operation, task number and contraint
                // operation is either "a", "d" or "v"
                // task number and constraint are integers

                // ex : "v 1 2" will verify the task 1 with constraint 2
                //      "a 1 2" will add the constraint 1 to 2
                //      "d 1 2" will remove the constraint 1 to 2

                // first : cast strings to integers   
                int taskNb = -1, constraint = -1; // variables for the operation we try to achieve
               
                // cast from the string to integer, if cast fails (string is not an integer) we got 0, 
                // so 0 will be assigned to verify so it wont change anything
                // floating point number will be rounded
                constraint = parseInt(input[1]);
                taskNb = parseInt(input[2]);

                // next check operation and call the right function for each case
                if(input[0] == "a"){
                    adjMatrix[constraint][taskNb].valid = true;
                    cout << "added " << constraint << " as a contraint of " << taskNb << endl;
                }else if(input[0] == "d"){

                    // check before deleting :
                    // (1) the constraint exists
                    // (2) the constraint has a successor and the task has a predecessor
                    // In this case, we can delete the constraint. Otherwise, we cannot

                    if(adjMatrix[constraint][taskNb].valid){
                        bool filled = false;
                        // check if the task has at least one predecessor
                        for (int i = 1; i < adjMatrix.size() - 1; ++i) {
                            filled = adjMatrix[i][taskNb].valid || filled;
                        }
                        //if filled we can check for the constraint to have at least one successor
                        if(filled){
                            filled = false;
                            for (int i = 1; i < adjMatrix.size() - 1; ++i) {
                                filled = adjMatrix[constraint][i].valid || filled;
                            }   
                            if(filled){
                                adjMatrix[constraint][taskNb].valid = false;
                                cout << "constraint between " << constraint << " and " << taskNb << " deleted" << endl;
                            }else{
                                cout << "cannot delete constraint : " << constraint << " doesn't have another successor" << endl;
                            }
                        }else{
                            cout << "cannot delete constraint : " << taskNb << " doesn't have another predecessor" << endl;
                        }
                    }else{
                        cout << "constraint doesn't exist" << endl;
                    }

                    
                }else if(input[0] == "v"){
                    if(adjMatrix[constraint][taskNb].valid){
                        cout << constraint << " is a contraint of " << taskNb << " !" << endl;
                    }else{
                        cout << constraint << " is not a contraint of " << taskNb << " !" << endl;
                    }
                }else{
                    cout << "error : unknown operation !" << endl;
                    // return false;
                    continue;
                }
                return true;
            } else {
                cout << "error : bad number of input !" << endl;
                // return false;
                continue;
            }
            return true;
        }
    }
    return true;
}

void computeDates(const AdjMatrix &adjMatrix, vector<int> &earliestDates, vector<int> &latestDates) {
    long size = adjMatrix.size();
    
    ///////////////////////
    ///////// Compute earliest dates
    ///////////////////////
    cout << "Computing earliest dates :" << endl;
    
    // initialisation
    vector<bool> used(size, false);
    int usedQuantity = 0;
    earliestDates.resize(size);
    for (int i = 0; i < size; ++i) {
        earliestDates[i] = -1;
    }
    
    // initialisation : find entry nodes
    cout << "Entry nodes : ";
    for (int node = 0; node < size; ++node) {
        if (!hasPredecessor(node, adjMatrix)) {
            earliestDates[node] = 0;
            
            cout << node << " ";
        }
    }
    cout << endl;
    
    // While some nodes has not been found,
    // iterate over more recently found nodes,
    // and look for their successors.
    while (usedQuantity < size) {
        for (int node = 0; node < size && usedQuantity < size; ++node) {
            // if it is a node we already found, but never used to look for
            // other nodes
            if (earliestDates[node] != -1 && !used[node]) {
                cout << endl << "Looking for " << node << "'s successors." << endl;
                // we look for its successors
                for (int newNode = 0; newNode < size; ++newNode) {
                    if (adjMatrix[node][newNode].valid) {
                        int value = adjMatrix[node][newNode].value + earliestDates[node];
                        if (value > earliestDates[newNode] || earliestDates[newNode] == -1) {
                            earliestDates[newNode] = value;
                            cout << newNode << "(" << earliestDates[newNode] << " through " << node << ") ";
                        }
                    }
                }
                used[node] = true;
                usedQuantity++;
                cout << endl;
            }
        }
        cout << endl;
    }

    
    ///////////////////////
    ///////// Compute latest dates
    ///////////////////////
    cout << endl << "Looking for latest dates :" << endl;
    // initialisation
    usedQuantity = 0;
    latestDates.resize(size);
    for (int i = 0; i < size; ++i) {
        latestDates[i] = -1;
        used[i] = false;
    }
    
    cout << "Entry nodes : ";
    // initialisation : find entry nodes
    for (int node = 0; node < size; ++node) {
        if (!hasSuccessor(node, adjMatrix)) {
            latestDates[node] = earliestDates[node];
            
            cout << node << " ";
        }
    }
    cout << endl;
    
    // While some nodes has not been found,
    // iterate over more recently found nodes,
    // and look for their successors.
    while (usedQuantity < size - 1) {
        for (int node = 0; node < size && usedQuantity < size; ++node) {
            // if it is a node we just found
            if (latestDates[node] != -1 && !used[node]) {
                cout << endl << "Looking for " << node << "'s predecessors." << endl;
                // we look for its successors
                for (int newNode = 0; newNode < size; ++newNode) {
                    if (adjMatrix[newNode][node].valid) {
                        int value = latestDates[node] - adjMatrix[newNode][node].value;
                        if (value < latestDates[newNode] || latestDates[newNode] == -1) {
                            if (latestDates[newNode] != -1) {
                                used[newNode] = false;
                                usedQuantity--;
                            }
                            
                            latestDates[newNode] = value;
                            cout << newNode << "(" << latestDates[newNode] << " through " << node << ") ";
                            cout << "";
                        }
                    }
                }
                used[node] = true;
                usedQuantity++;
                cout << endl;
            }
        }
        cout << endl;
    }
}

void printCalendar(vector<int> &earliestDates, vector<int> &latestDates) {
    long size = earliestDates.size();
    cout << "Nodes" << "\t\t\t";
    for (int node = 0; node < size; ++node) {
        cout << "\t" << node;
    }
    cout << endl;
    
    cout << "Earliest dates\t";
    for (int node = 0; node < size; ++node) {
        cout << "\t" << earliestDates[node];
    }
    cout << endl;
    
    cout << "latestDates dates";
    for (int node = 0; node < size; ++node) {
        cout << "\t" << latestDates[node];
    }
    cout << endl;
}

bool hasPredecessor(int node, const AdjMatrix &adjMatrix) {
    int i = 0;
    while (i < adjMatrix.size()) {
        if (adjMatrix[i][node].valid) {
            return true;
        }
        i++;
    }
    return false;
}

bool hasSuccessor(int node, const AdjMatrix &adjMatrix) {
    int i = 0;
    while (i < adjMatrix.size()) {
        if (adjMatrix[node][i].valid) {
            return true;
        }
        i++;
    }
    return false;
}

bool parseFile(string path, AdjMatrix &adjMatrix) {
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
                    return false;
                }
                cout << "number of tasks to be parsed : " << nbTasks << endl;
                
                // resize the adgency matrix and set all value to not valid
                adjMatrix.resize(nbTasks + 2); // we considere the initial and final task
                for (int i = 0; i < adjMatrix.size(); ++i){
                    adjMatrix[i].resize(nbTasks + 2);
                    for (int j = 0; j < adjMatrix[j].size(); ++j){
                        adjMatrix[i][j].valid = false; //  each cell is invalid for now, we will change that while parsing the file
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
                        return false;
                    }
                    
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
                        return false;
                    }
                    
                    // check if the line contains at least 3 elements : task number length and -1
                    if(splittedLine.size() < 3){
                        cout << "error : line too short !" << endl;
                        return false;
                    }
                    
                    // verify that the last one is "-1"
                    if(splittedLine[splittedLine.size() -1] != "-1"){
                        cout << "error : missing -1 at the end of the line" << endl;
                        return false;
                    }
                    
                    // assuming here that the line is good, we can parse it
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
        // we close the file
        file.close();
    } else {
        cout << "cannot open file !" << endl;
        return false;
    }
    return true; //  everything is  OK
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

void print(const AdjMatrix &adjMatrix) {
    long size = adjMatrix.size();
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
