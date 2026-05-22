#include "MovieNode.h"
#include "ActorNode.h"
#include <iostream>

using namespace std;

//class for storing the details of all the movies

MovieNode::MovieNode() : duration(0), gross(0), title_year(0), imdb_score(0.0f), next(nullptr), actor1(nullptr), actor2(nullptr), actor3(nullptr) {}

void MovieNode::display() const {
    cout << endl << "Movie: " << movie_title << endl;
    cout << "Director: " << director_name << endl;
    cout << "Year: " << title_year << endl;
    cout << "Duration: " << duration << " minutes" << endl;
    cout << "IMDB Score: " << imdb_score << endl;
    cout << "Gross: $" << gross << endl;
    cout << "Country: " << country << endl;
    cout << "Genres: " << genres << endl;
    cout << "Actors: " << actor_1_name << ", " << actor_2_name << ", " << actor_3_name << endl;
}