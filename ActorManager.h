#ifndef ACTORMANAGER_H
#define ACTORMANAGER_H

#include "ActorNode.h"
using namespace std;

class ActorManager {
private:
    ActorNode** actors;
    int actorCount;
    int actorCapacity;
    
    // Helper function to resize array
    void resize();
    
public:
    // Constructor and Destructor
    ActorManager();
    ~ActorManager();
    
    // Public interface
    ActorNode* getActorNode(const string& name);
    ActorNode* findActor(const string& name);
    void displayAll();
    
    // Getters
    int getCount();
};

#endif