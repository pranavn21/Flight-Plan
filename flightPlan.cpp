// Pranav Nair
// CS 3345.501
// Professor: Kamran Khan
// Due Date: December 6, 2021

#include "DataStructures.h"
#include <fstream>
#include <iostream>

// IMPORTANT NOTE (to TA): Professor Khan allowed using built-in stacks according to his email reply to me.

// Function that takes the flight info file name and returns the linked list of vertices (complete graph data)
Vertex* readInput(char* fname){
    ifstream iFile(fname);
    if (!iFile){
        cout << "ERROR: Unable to open input file" << endl;
        return NULL;
    }

    char line[50];
    iFile.getline(line, 49);
    int n = atoi(line);

    Vertex* list = NULL;
    string src, dst;
    int time;
    float cost;

    for(int i = 0; i < n; i++){
        iFile.getline(line, 49, '|');
        src = line;

        iFile.getline(line, 49, '|');
        dst = line;

        iFile.getline(line, 49, '|');
        cost = atof(line);

        iFile.getline(line, 49);
        time = atoi(line);

        //Create edges for both directions
        Edge* s2d = new Edge(dst, time, cost);
        Edge* d2s = new Edge(src, time, cost);

        Vertex *tmp;
        // Add source to destination
        for(tmp = list; tmp; tmp = tmp->getNext()){
            if (tmp->getSourceCity() == src){
                break;
            }
        }
        if (tmp){
            tmp->addDestination(s2d);
        } else{
            Vertex* v = new Vertex(src);
            v->addDestination(s2d);
            v->setNext(list);
            list = v;
        }

        // Add destination to source
        for(tmp = list; tmp; tmp = tmp->getNext()){
            if (tmp->getSourceCity() == dst){
                break;
            }
        }
        if (tmp){
            tmp->addDestination(d2s);
        } else{
            Vertex* v = new Vertex(dst);
            v->addDestination(d2s);
            v->setNext(list);
            list = v;
        }
    }

    return list;
}

// To free a linked list of vertices (dynamically allocated memory)
void freeVertexList(Vertex *v){
    while(v){
        Vertex* temp = v;
        v = v->getNext();
        Edge* e = temp->getDestinations();

        while(e){
            Edge* tempE = e;
            e = e->getNext();
            delete(tempE);
        }
    
        delete(temp);
    }
}

/* Function that implements the algorithm for one source and destination pair
 Arguments: the graph data (Vertex *), the source and destination names, and the sorting basis - T or C
 The last three arguments are by reference as they are used to give back the three best paths */
void processOnePath(Vertex *list, string src, string dst, char trc, stackNode &p1, stackNode& p2, stackNode &p3){
    stack <stackNode> myStack;
    // start the stack with only the source on it
    stackNode node(new Vertex(src));
    myStack.push(node); // IMPORTANT NOTE (to TA): Professor Khan allowed using built-in stacks according to his email reply to me.
        
    //Iterative Backtracking algorithm
    while (!myStack.empty()){
        node = myStack.top(); // Get the path at the top of the stack
        myStack.pop();
        if(node.getPath()->getSourceCity() == dst){ // Check whether a solution is found
            // If this node is better than the lowest cost node (if C) or shortest time node (if T)
            if(p1.getCost() == -1 || (trc == 'T' && node.getTime() < p1.getTime()) || (trc == 'C' && node.getCost() < p1.getCost())){ // Check if these are the top 3 conditions
                if(p3.getCost() != -1){
                    freeVertexList(p3.getPath());
                }
                p3 = p2;
                p2 = p1;
                p1 = node;
            }

                // If this node is better than the 2nd lowest cost node (if C) or 2nd shortest time node (if T)
            else if(p2.getCost() == -1 || (trc == 'T' && node.getTime() < p2.getTime()) || (trc == 'C' && node.getCost() < p2.getCost())){
                if(p3.getCost() != -1){
                    freeVertexList(p3.getPath());
                }
                p3 = p2;
                p2 = node;
            }

                // If this node is better than the 3rd lowest cost node (if C) or 3rd shortest time node (if T)
            else if(p3.getCost() == -1 || (trc == 'T' && node.getTime() < p3.getTime()) || (trc == 'C' && node.getCost() < p3.getCost())){
                if(p3.getCost() != -1){
                    freeVertexList(p3.getPath());
                }
                p3 = node;
            }

            else{
                freeVertexList(node.getPath()); // Get rid of the node as it's not needed
            }
        }

        else{ // If a solution is not found, augment the path
            Vertex *vsrc = list;
            for(; vsrc; vsrc = vsrc->getNext()){ // Find the last vertex in the path
                if(vsrc->getSourceCity() == node.getPath()->getSourceCity()){
                    break;
                }
            }

            Edge *esrc = vsrc->getDestinations(); // Get the next flights possible
            for(; esrc; esrc = esrc->getNext()){
                bool repeat = false;
                for(Vertex *v = node.getPath(); v; v = v->getNext()){
                    if(v->getSourceCity() == esrc->getDestCity()){ // Check for cycles (if it's already been in the path)
                        repeat = true;
                        break;
                    }
                }

                if (!repeat){ // Consider this flight only if it's not a cycle
                    int t = node.getTime() + esrc->getTime();
                    float c = node.getCost() + esrc->getCost();
                    Vertex *ver = new Vertex(esrc->getDestCity());
                    Vertex* newv = ver;
                    for(Vertex* v = node.getPath(); v; v = v->getNext()){
                        newv->setNext(new Vertex(v->getSourceCity()));
                        newv = newv->getNext();
                    }

                    stackNode s1(ver, t, c); // Construct the augmented path and push it into the stack for the next cycle
                    myStack.push(s1); 
                }
            }

            freeVertexList(node.getPath()); // The popped node is no longer needed, so we can delete it
        }
    }
}

// Takes the graph data, the path file name and the output file name
// It processes the path file and writes the results to output file
void processPaths(Vertex* list, char* pfilename, char* ofilename){
    ifstream pathfile(pfilename);
    ofstream outfile(ofilename);
    if (!pathfile || !outfile){
        cout << "ERROR: One or both of the files cannot be opened " << endl;
        return;
    }    

    char line[50];
    pathfile.getline(line, 49);
    int n = atoi(line); // Converting to integer
    string src, dst;
    char trc;

    // Getting the various variables from the input given
    for(int i = 0; i < n; i++){
        pathfile.getline(line, 49, '|');
        src = line;
        pathfile.getline(line,49,'|');
        dst = line;
        pathfile.getline(line,49);
        trc = line[0];

        stackNode p1(NULL), p2(NULL), p3(NULL);
        processOnePath(list, src, dst, trc, p1, p2, p3);

        outfile << "Flight " << i+1 << ": " << src << ", " << dst;
        if (trc == 'T'){
            outfile << " (Time)" << endl;
        }
        else{
            outfile << " (Cost)" << endl;
        }

        // Before printing any path to the file, make sure it's a valid path
        if (p1.getCost() == -1 && p2.getCost() == -1 && p3.getCost() == -1){ // If no path found
            outfile << "ERROR: No path found from " << src << " to " << dst << " (or vice versa)" << endl;
        }

        if (p1.getCost() != -1){
            outfile << "Path 1: " << p1.getPathString() << "Time: " << p1.getTime() << " Cost: " << setprecision(2) << fixed << p1.getCost() << endl;
            freeVertexList(p1.getPath());
        }
        if (p2.getCost() != -1){
            outfile << "Path 2: " <<  p2.getPathString() << "Time: " << p2.getTime() << " Cost: " << setprecision(2) << fixed << p2.getCost() << endl;
            freeVertexList(p2.getPath());
        }
        if (p3.getCost() != -1){
            outfile << "Path 3: " << p3.getPathString() << "Time: " << p3.getTime() << " Cost: " << setprecision(2) << fixed << p3.getCost() << endl;
            freeVertexList(p2.getPath());
        }
        outfile << endl;
    }
}
int main(int argc, char *argv[]){
    Vertex* list = readInput(argv[1]);

    processPaths(list, argv[2], argv[3]);
    freeVertexList(list);

    return 0;
}