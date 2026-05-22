#ifndef MOVIEDATABASE_H
#define MOVIEDATABASE_H

#include "MovieList.h"
#include "AVLTree.h"
#include "ActorManager.h"
#include "Graph.h"
#include "HashTable.h"
#include <string>
using namespace std;

class MovieDatabase {
private:
    MovieList movieList;
    AVLTree avlTree;
    ActorManager actorManager;
    
    // Hash tables for efficient searching
    HashTable* actorHashTable;      // Maps actor names to movies
    HashTable* titleHashTable;      // Maps movie titles to movies
    HashTable* genreHashTable;      // Maps genres to movies
    HashTable* directorHashTable;   // Maps directors to movies
    HashTable* yearHashTable;       // Maps years to movies
    
public:
	//Loading function
    void loadFromCSV(const string& filename, int maxRecords = 1000);
    
    //Display functions
    void displayAllMovies();
    void displaySortedByScore();
    void displayActorInfo(const string& actorName);
    void displayAllActors();
    void displayStats();
    
    //Search functions
    void searchDirector(const string& director);
    void searchActor(const string& actor);
    void searchByTitle(const string& title);
    void searchByGenre(const string& genre);
    void searchByYear(int year);
    void searchByRatingRange(float minRating, float maxRating);

    //Graph functions
    void buildGraph(Graph& graph, int total);
    int getMovieIndexByTitle(const string& title);
    
    // Constructor and Destructor
    MovieDatabase();
    ~MovieDatabase();
};

#endif