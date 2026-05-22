#ifndef ACTORNODE_H
#define ACTORNODE_H

#include <string>
using namespace std;

class MovieNode;

class ActorNode {
public:
    string name;
    
    // Array of pointers to movies
    MovieNode** movies;
    int movieCount;
    int capacity;
    
    // Constructor
    ActorNode(const string& actorName);
    
    // Destructor
    ~ActorNode();
    
    // Add movie to actor's filmography
    void addMovie(MovieNode* movie);
    
    // Display actor information
    void display() const;
    
private:
    // Helper function to resize array
    void resize();
};

#endif