#ifndef MOVIELIST_H
#define MOVIELIST_H

#include "MovieNode.h"
using namespace std;

class MovieList {
private:
    MovieNode* head;
    int count;
    
public:
    // Constructor and Destructor
    MovieList();
    ~MovieList();
    
    // Public interface
    void addMovie(MovieNode* movie);
    void displayAll();
    int searchByDirector(const string& director, MovieNode** results, int maxResults);
    int searchByActor(const string& actor, MovieNode** results, int maxResults);
    int searchByTitle(const string& title, MovieNode** results, int maxResults);
    int searchByGenre(const string& genre, MovieNode** results, int maxResults);
    int searchByYear(int year, MovieNode** results, int maxResults);
    int searchByRatingRange(float minRating, float maxRating, MovieNode** results, int maxResults);
    void fillArray(MovieNode** arr);
    
    // Getters
    MovieNode* getHead();
    int getCount();
};

#endif