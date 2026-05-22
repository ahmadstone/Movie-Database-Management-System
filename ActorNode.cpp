#include "ActorNode.h"
#include "MovieNode.h"
#include <iostream>

using namespace std;

ActorNode::ActorNode(const string& actorName) : name(actorName), movieCount(0), capacity(10) {
    movies = new MovieNode*[capacity];
}

ActorNode::~ActorNode() {
    delete[] movies;
}

void ActorNode::addMovie(MovieNode* movie) {
    if (movieCount >= capacity) {
        resize();
    }
    movies[movieCount++] = movie;
}

void ActorNode::display() const {
    //cout << "Actor: " << name << endl;
    //cout << "Movies (" << movieCount << "): " << endl;
    cout << "\n-----------" << name << "'s Movies (" << movieCount << "): " << "-----------" << endl;

    for (int i = 0; i < movieCount; i++) {
        cout << i+1 << ". " << movies[i]->movie_title;
        if (i < movieCount - 1) cout << endl;
    }
    cout << endl;
}

void ActorNode::resize() {
    // Double the capacity
    capacity *= 2;
    MovieNode** newMovies = new MovieNode*[capacity];
    
    // Copy existing movies
    for (int i = 0; i < movieCount; i++) {
        newMovies[i] = movies[i];
    }
    
    // Delete old array
    delete[] movies;
    
    // Update pointer
    movies = newMovies;
}