// Pranav Nair
// CS 3345.501
// Professor: Kamran Khan
// Due Date: December 6, 2021

#include <string>
using namespace std;

/* Edge will be used for a linked list representing the outgoing edges of a vertex. It keeps track of the destination city, time and cost */
class Edge {
    private:
    string destCity;
    int time;
    float cost;
    Edge* next;

    public:
    string getDestCity(){
        return destCity;
    }
    int getTime(){
        return time;
    }
    float getCost(){
        return cost;
    }
    Edge* getNext(){
        return next;
    }
    void setNext(Edge* e){
        next = e;
    }

    Edge(string d, int t, float c){
        destCity = d;
        time = t;
        cost = c;
        next = NULL;
    }

};

/* Class for the linked list of vertices. This is the "linked lists of linked lists." 
In addition to a link for the next vertex, each node also has a pointer to an Edge which represents the adjacency list of the vertex.
The same class can be used to represent paths as well, by keeping adjacency lists as empty */
class Vertex{
    private:
    string sourceCity;
    Edge* destinations;
    Vertex* next;

    public:
    string getSourceCity(){
        return sourceCity;
    }
    Vertex* getNext(){
        return next;
    }
    void setNext(Vertex* v){
        next = v;
    }
    Edge* getDestinations(){
        return destinations;
    }
    void setDestinations(Edge* d){
        destinations = d;
    }
    void addDestination(Edge* d){
        d->setNext(destinations);
        destinations = d;
    }

    Vertex(string s){
        sourceCity = s;
        destinations = NULL;
        next = NULL;
    }
};

// IMPORTANT NOTE (to TA): Professor Khan allowed using built-in stacks according to his email reply to me.
/* Class used for the backtracking algorithm
This class represents a path with the cumulative time and cost */
 class stackNode{
    private:
    Vertex* v;
    int time;
    float cost;

    public:
    stackNode(Vertex* ver){
        v = ver;
        if (ver != NULL){ // If there is a vertex and it isn't an empty path
            time = 0; 
            cost = 0;
        }
        else{
            time = -1;
            cost = -1; 
        }

    }

    stackNode(Vertex* ver, int t, float c){
        v = ver;
        time = t;
        cost = c;
    }

    int getTime(){
        return time;
    }

    float getCost(){
        return cost;
    }

    Vertex* getPath(){
        return v;
    }

    // Returns a formatted path in string
    string getPathString(){
        string s;
        s = v->getSourceCity() + ". ";

        for(Vertex* ver = v->getNext(); ver; ver = ver->getNext()){
            s = ver->getSourceCity() + " -> " + s;
        }
        return s;
    }
};