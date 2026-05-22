#ifndef MOVIENODE_H
#define MOVIENODE_H

#include <string>
#include <iostream>
using namespace std;    

// Forward declaration
class ActorNode;

class MovieNode {
public:
    string director_name;
    int duration;
    string actor_2_name;
    long long gross;
    string genres;
    string actor_1_name;
    string movie_title;
    string actor_3_name;
    string country;
    int title_year;
    float imdb_score;
    
    // Linked list pointer
    MovieNode* next;
    
    // Pointers to ActorNode objects
    ActorNode* actor1;
    ActorNode* actor2;
    ActorNode* actor3;
  
    MovieNode();
    
    //displays movie information
    void display() const;
};

#endif