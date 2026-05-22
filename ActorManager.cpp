#include "ActorManager.h"
#include <iostream>

using namespace std;

ActorManager::ActorManager() : actorCount(0), actorCapacity(100) {
    actors = new ActorNode*[actorCapacity];
}

ActorManager::~ActorManager() {
    for (int i = 0; i < actorCount; i++) {
        delete actors[i];
    }
    delete[] actors;
}

ActorNode* ActorManager::getActorNode(const string& name) {
    // Check if actor already exists
    for (int i = 0; i < actorCount; i++) {
        if (actors[i]->name == name) {
            return actors[i];
        }
    }
    
    // Create new actor
    if (actorCount >= actorCapacity) {
        resize();
    }
    
    ActorNode* newActor = new ActorNode(name);
    actors[actorCount++] = newActor;
    return newActor;
}

ActorNode* ActorManager::findActor(const string& name) {
    for (int i = 0; i < actorCount; i++) {
        if (actors[i]->name == name) {
            return actors[i];
        }
    }
    return nullptr;
}

void ActorManager::displayAll() {
    cout << "\n=== All Actors (" << actorCount << ") ===\n";
    for (int i = 0; i < actorCount; i++) {
        cout << i + 1 << ". " << actors[i]->name << endl;
    }
}

int ActorManager::getCount() {
    return actorCount;
}

void ActorManager::resize() {
    // Double the capacity
    actorCapacity *= 2;
    ActorNode** newActors = new ActorNode*[actorCapacity];
    
    // Copy existing actors
    for (int i = 0; i < actorCount; i++) {
        newActors[i] = actors[i];
    }
    
    // Delete old array
    delete[] actors;
    
    // Update pointer
    actors = newActors;
}