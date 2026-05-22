#include "MovieList.h"
#include <iostream>

using namespace std;

MovieList::MovieList() : head(nullptr), count(0) {}

MovieList::~MovieList() {
    MovieNode* current = head;
    while (current) {
        MovieNode* next = current->next;
        delete current;
        current = next;
    }
}

void MovieList::fillArray(MovieNode** arr) {
    MovieNode* current = head;   // assuming your list has a head pointer
    int index = 0;

    while (current != nullptr) {
        arr[index++] = current;
        current = current->next;
    }
}


void MovieList::addMovie(MovieNode* movie) {
    if (!head) {
        head = movie;
    } else {
        MovieNode* current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = movie;
    }
    movie->next = nullptr;
    count++;
}

void MovieList::displayAll() {
    cout << "\n=== All Movies (" << count << ") ===\n";
    MovieNode* current = head;
    int i = 1;
    while (current) {
        cout << "\nMovie #" << i++ << ":";
        current->display();
        current = current->next;
    }
}

int MovieList::searchByDirector(const string& director, MovieNode** results, int maxResults) {
    int resultCount = 0;
    MovieNode* current = head;
    while (current && resultCount < maxResults) {
        if (current->director_name.find(director) != string::npos) {
            results[resultCount++] = current;
        }
        current = current->next;
    }
    return resultCount;
}

int MovieList::searchByActor(const string& actor, MovieNode** results, int maxResults) {
    int resultCount = 0;
    MovieNode* current = head;
    while (current && resultCount < maxResults) {
        if (current->actor_1_name.find(actor) != string::npos ||
            current->actor_2_name.find(actor) != string::npos ||
            current->actor_3_name.find(actor) != string::npos) {
            results[resultCount++] = current;
        }
        current = current->next;
    }
    return resultCount;
}

int MovieList::searchByTitle(const string& title, MovieNode** results, int maxResults) {
    int resultCount = 0;
    MovieNode* current = head;
    while (current && resultCount < maxResults) {
        if (current->movie_title.find(title) != string::npos) {
            results[resultCount++] = current;
        }
        current = current->next;
    }
    return resultCount;
}

int MovieList::searchByGenre(const string& genre, MovieNode** results, int maxResults) {
    int resultCount = 0;
    MovieNode* current = head;
    while (current && resultCount < maxResults) {
        if (current->genres.find(genre) != string::npos) {
            results[resultCount++] = current;
        }
        current = current->next;
    }
    return resultCount;
}

int MovieList::searchByYear(int year, MovieNode** results, int maxResults) {
    int resultCount = 0;
    MovieNode* current = head;
    while (current && resultCount < maxResults) {
        if (current->title_year == year) {
            results[resultCount++] = current;
        }
        current = current->next;
    }
    return resultCount;
}

int MovieList::searchByRatingRange(float minRating, float maxRating, MovieNode** results, int maxResults) {
    int resultCount = 0;
    MovieNode* current = head;
    while (current && resultCount < maxResults) {
        if (current->imdb_score >= minRating && current->imdb_score <= maxRating) {
            results[resultCount++] = current;
        }
        current = current->next;
    }
    return resultCount;
}

MovieNode* MovieList::getHead() {
    return head;
}

int MovieList::getCount() {
    return count;
}