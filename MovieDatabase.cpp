#include "MovieDatabase.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

//helpers written to clean and parse the csv file
string cleanField(const string& field) {
    if (field.empty()) return "";

    string result = field;

	//this is for removing quotes (especially for movie titles)
    if (result.front() == '\"' && result.back() == '\"') {
        result = result.substr(1, result.length() - 2);
    }

    size_t pos;

	//replaces non-breaking spaces with regular spaces
    while ((pos = result.find('\xA0')) != string::npos) {
        result.replace(pos, 1, " ");
    }

    size_t start = result.find_first_not_of(" \t\n\r");
    size_t end = result.find_last_not_of(" \t\n\r");

    if (start != string::npos && end != string::npos) {
        return result.substr(start, end - start + 1);
    }

    return "";
}


int safeStoi(const string& str, int defaultValue = 0) {
    if (str.empty() || str.find_first_not_of("0123456789-") != string::npos) {
        return defaultValue;
    }
    try {
        return stoi(str);
    } catch (...) {
        return defaultValue;
    }
}

long long safeStoll(const string& str, long long defaultValue = 0) {
    if (str.empty() || str.find_first_not_of("0123456789-") != string::npos) {
        return defaultValue;
    }
    try {
        return stoll(str);
    } catch (...) {
        return defaultValue;
    }
}

float safeStof(const string& str, float defaultValue = 0.0f) {
    if (str.empty() || str.find_first_not_of("0123456789.-") != string::npos) {
        return defaultValue;
    }
    try {
        return stof(str);
    } catch (...) {
        return defaultValue;
    }
}

static string getNextField(const string& line, size_t& pos) {
    if (pos >= line.size()) {
        ++pos;
        return string();
    }

    string field="";
    if (line[pos] == '"') {
        //quoted field
		++pos; //to skip opening quote
        while (pos < line.size()) {
            char c = line[pos];
            if (c == '"') {
                //If next char is also a quote, it's an escaped quote -> append one quote
                if (pos + 1 < line.size() && line[pos + 1] == '"') {
                    field.push_back('"');
                    pos += 2;
                    continue;
                }
                else {
                    //end of quotes
                    ++pos;
                    break;
                }
            }
            else {
                field.push_back(c);
                ++pos;
            }
        }
        //after closing quote, skip until comma
        while (pos < line.size() && line[pos] != ',') ++pos;
		if (pos < line.size() && line[pos] == ',') ++pos; //and this skips the comma
    }
    else {
        //for unquoted fields just read until next comma or end
        while (pos < line.size() && line[pos] != ',') {
            field.push_back(line[pos]);
            ++pos;
        }
        if (pos < line.size() && line[pos] == ',') ++pos; //and this skips the comma
    }
    return field;
}

//safeTrim helper (small, uses cleanField for NBSP fixes but without quoting)
static string safeTrim(const string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}

string sanitizeUTF8(const string& s) {
    string out;
    for (char c : s) {
        if (c >= 0) { // simple filter: skip negative bytes (bad UTF-8 start)
            out.push_back(c);
        }
        else {
            out.push_back(' '); // replace invalid char with space
        }
    }
    return out;
}

MovieDatabase::MovieDatabase() {
    // Initialize hash tables with appropriate sizes
    actorHashTable = new HashTable(2000);      // Larger for many actors
    titleHashTable = new HashTable(5000);      // One per movie
    genreHashTable = new HashTable(500);       // Fewer genres
    directorHashTable = new HashTable(1000);   // Many directors
    yearHashTable = new HashTable(200);        // Years from ~1900-2020
}

MovieDatabase::~MovieDatabase() {
    delete actorHashTable;
    delete titleHashTable;
    delete genreHashTable;
    delete directorHashTable;
    delete yearHashTable;
}

void MovieDatabase::loadFromCSV(const string& filename, int maxRecords) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening CSV file.\n";
        return;
    }

    string line;
    getline(file, line); //skip header

    int count = 0;

    while (getline(file, line) && count < maxRecords) {
        size_t pos = 0;

        MovieNode* movie = new MovieNode();

        string token = ""; //the ones to ignore

        token = getNextField(line, pos);
        movie->director_name = cleanField(getNextField(line, pos));
        token = getNextField(line, pos);

        movie->duration = safeStoi(getNextField(line, pos));
        token = getNextField(line, pos);
        token = getNextField(line, pos);

        movie->actor_2_name = cleanField(getNextField(line, pos));
        token = getNextField(line, pos);

        movie->gross = safeStoll(getNextField(line, pos));

        movie->genres = cleanField(getNextField(line, pos));
        movie->actor_1_name = cleanField(getNextField(line, pos));
        movie->movie_title = sanitizeUTF8(cleanField(getNextField(line, pos)));

        token = getNextField(line, pos);
        token = getNextField(line, pos);

        movie->actor_3_name = cleanField(getNextField(line, pos));

        token = getNextField(line, pos);
        token = getNextField(line, pos);
        token = getNextField(line, pos);
        token = getNextField(line, pos);
        token = getNextField(line, pos);

        movie->country = cleanField(getNextField(line, pos));

        token = getNextField(line, pos);  // content_rating
        token = getNextField(line, pos);  // budget
        movie->title_year = safeStoi(getNextField(line, pos));  // title_year
        token = getNextField(line, pos);  // actor_2_facebook_likes

        movie->imdb_score = safeStof(getNextField(line, pos));

		//to ensure proper data saving, the following code skips incomplete records
        if (movie->movie_title.empty()) continue;

        if (movie->actor_1_name.empty() && movie->actor_2_name.empty() && movie->actor_3_name.empty()) continue;

		//linking actors to movies
        movie->actor1 = actorManager.getActorNode(movie->actor_1_name);
        movie->actor2 = actorManager.getActorNode(movie->actor_2_name);
        movie->actor3 = actorManager.getActorNode(movie->actor_3_name);

        if (movie->actor1) movie->actor1->addMovie(movie);
        if (movie->actor2) movie->actor2->addMovie(movie);
        if (movie->actor3) movie->actor3->addMovie(movie);

		//and storing the movie in the main structures
        movieList.addMovie(movie);
        avlTree.insert(movie);
        
        // Populate hash tables for efficient searching
        if (!movie->actor_1_name.empty()) {
            actorHashTable->insert(movie->actor_1_name, movie);
        }
        if (!movie->actor_2_name.empty()) {
            actorHashTable->insert(movie->actor_2_name, movie);
        }
        if (!movie->actor_3_name.empty()) {
            actorHashTable->insert(movie->actor_3_name, movie);
        }
        if (!movie->movie_title.empty()) {
            titleHashTable->insert(movie->movie_title, movie);
        }
        if (!movie->genres.empty()) {
            // Insert for each genre (genres might be comma-separated)
            string genreStr = movie->genres;
            size_t start = 0;
            size_t pos = genreStr.find('|');
            while (pos != string::npos) {
                string genre = genreStr.substr(start, pos - start);
                if (!genre.empty()) {
                    genreHashTable->insert(genre, movie);
                }
                start = pos + 1;
                pos = genreStr.find('|', start);
            }
            // Add the last genre
            if (start < genreStr.length()) {
                string genre = genreStr.substr(start);
                if (!genre.empty()) {
                    genreHashTable->insert(genre, movie);
                }
            }
        }
        if (!movie->director_name.empty()) {
            directorHashTable->insert(movie->director_name, movie);
        }
        if (movie->title_year > 0) {
            yearHashTable->insert(to_string(movie->title_year), movie);
        }

        count++;
    }

    file.close();
    cout << "Loaded " << count << " records from CSV." << endl;
}

int MovieDatabase::getMovieIndexByTitle(const string& title) {
    if (title.empty()) return -1;
    
    // Use hashtable to find the movie (try exact match first, then partial)
    const int MAX_RESULTS = 10;
    MovieNode* results[MAX_RESULTS];
    
    // First try exact match
    int count = titleHashTable->search(title, results, MAX_RESULTS);
    
    // If no exact match, try partial match
    if (count == 0) {
        count = titleHashTable->searchPartial(title, results, MAX_RESULTS);
    }
    
    if (count == 0) {
        return -1; // Movie not found
    }
    
    // Use the first result (most relevant match)
    MovieNode* targetMovie = results[0];
    
    // Find the index of this movie in the list
    MovieNode* current = movieList.getHead();
    int index = 0;
    
    while (current != nullptr) {
        if (current == targetMovie) {
            return index;
        }
        current = current->next;
        index++;
    }
    
    return -1; // Should not reach here if hashtable is consistent
}


// Helper function to check if two movies share at least one genre
static bool shareGenre(const string& genresA, const string& genresB) {
    if (genresA.empty() || genresB.empty()) return false;
    
    // Parse genres from movieA (genres are separated by '|')
    size_t startA = 0;
    size_t posA = genresA.find('|');
    while (true) {
        string genreA;
        if (posA != string::npos) {
            genreA = genresA.substr(startA, posA - startA);
        } else {
            genreA = genresA.substr(startA);
        }
        
        // Trim whitespace
        size_t start = genreA.find_first_not_of(" \t");
        size_t end = genreA.find_last_not_of(" \t");
        if (start != string::npos && end != string::npos) {
            genreA = genreA.substr(start, end - start + 1);
        }
        
        if (!genreA.empty()) {
            // Check if this genre exists in genresB
            if (genresB.find(genreA) != string::npos) {
                return true;
            }
        }
        
        if (posA == string::npos) break;
        startA = posA + 1;
        posA = genresA.find('|', startA);
    }
    
    return false;
}

void MovieDatabase::buildGraph(Graph& graph, int total) {
    MovieNode** movieArray=new MovieNode *[5000];
    movieList.fillArray(movieArray);
	graph.setMovieArray(movieArray);

    for (int i = 0; i < total; i++) {
        MovieNode* movieA = movieArray[i];
        for (int j = i + 1; j < total; j++) {
            MovieNode* movieB = movieArray[j];
            
			//this checks if two movies share at least one actor
            bool sharedActor = false;
            if (!movieA->actor_1_name.empty() && (movieA->actor_1_name == movieB->actor_1_name ||
                                                 movieA->actor_1_name == movieB->actor_2_name ||
                                                 movieA->actor_1_name == movieB->actor_3_name)) {
                sharedActor = true;
            }
            if (!sharedActor && !movieA->actor_2_name.empty() && (movieA->actor_2_name == movieB->actor_1_name ||
                                                                 movieA->actor_2_name == movieB->actor_2_name ||
                                                                 movieA->actor_2_name == movieB->actor_3_name)) {
                sharedActor = true;
            }
            if (!sharedActor && !movieA->actor_3_name.empty() && (movieA->actor_3_name == movieB->actor_1_name ||
                                                                 movieA->actor_3_name == movieB->actor_2_name ||
                                                                 movieA->actor_3_name == movieB->actor_3_name)) {
                sharedActor = true;
            }
            
            // Check if movies share at least one genre
            bool sharedGenre = shareGenre(movieA->genres, movieB->genres);
            
            // Connect movies if they share actors OR genres
            if (sharedActor || sharedGenre) {
                graph.addEdge(i, j);
            }
        }
    }
}

void MovieDatabase::displayAllMovies() {
    movieList.displayAll();
}

void MovieDatabase::displaySortedByScore() {
    avlTree.displaySorted();  // Now displays sorted by title
}

// Helper function to convert int to string
string intToString(int value) {
    if (value == 0) return "0";
    bool negative = value < 0;
    if (negative) value = -value;
    
    string result = "";
    while (value > 0) {
        result = char('0' + (value % 10)) + result;
        value /= 10;
    }
    if (negative) result = "-" + result;
    return result;
}

void MovieDatabase::searchDirector(const string& director) {
    const int MAX_RESULTS = 100;
    MovieNode* results[MAX_RESULTS];
    // Use hashtable for exact match, fallback to partial search
    int count = directorHashTable->search(director, results, MAX_RESULTS);
    if (count == 0) {
        count = directorHashTable->searchPartial(director, results, MAX_RESULTS);
    }
    
    cout << "\n=== Movies by Director '" << director << "' (" << count << ") ===\n";
    for (int i = 0; i < count; i++) {
        results[i]->display();
    }
}

void MovieDatabase::searchActor(const string& actor) {
    const int MAX_RESULTS = 100;
    MovieNode* results[MAX_RESULTS];
    // Use hashtable for exact match, fallback to partial search
    int count = actorHashTable->search(actor, results, MAX_RESULTS);
    if (count == 0) {
        count = actorHashTable->searchPartial(actor, results, MAX_RESULTS);
    }
    
    cout << "\n=== Movies with Actor '" << actor << "' (" << count << ") ===\n";
    for (int i = 0; i < count; i++) {
        results[i]->display();
    }
}

void MovieDatabase::searchByTitle(const string& title) {
    const int MAX_RESULTS = 100;
    MovieNode* results[MAX_RESULTS];
    // Use hashtable for partial search (titles might be partial matches)
    int count = titleHashTable->searchPartial(title, results, MAX_RESULTS);
    
    cout << "\n=== Movies with Title containing '" << title << "' (" << count << ") ===\n";
    for (int i = 0; i < count; i++) {
        results[i]->display();
    }
}

void MovieDatabase::searchByGenre(const string& genre) {
    const int MAX_RESULTS = 100;
    MovieNode* results[MAX_RESULTS];
    // Use hashtable for exact match, fallback to partial search
    int count = genreHashTable->search(genre, results, MAX_RESULTS);
    if (count == 0) {
        count = genreHashTable->searchPartial(genre, results, MAX_RESULTS);
    }
    
    cout << "\n=== Movies with Genre containing '" << genre << "' (" << count << ") ===\n";
    for (int i = 0; i < count; i++) {
        results[i]->display();
    }
}

void MovieDatabase::searchByYear(int year) {
    const int MAX_RESULTS = 100;
    MovieNode* results[MAX_RESULTS];
    // Use hashtable with year as string key
    string yearStr = intToString(year);
    int count = yearHashTable->search(yearStr, results, MAX_RESULTS);
    
    cout << "\n=== Movies from Year " << year << " (" << count << ") ===\n";
    for (int i = 0; i < count; i++) {
        results[i]->display();
    }
}

void MovieDatabase::searchByRatingRange(float minRating, float maxRating) {
    const int MAX_RESULTS = 100;
    MovieNode* results[MAX_RESULTS];
    int count = movieList.searchByRatingRange(minRating, maxRating, results, MAX_RESULTS);
    
    cout << "\n=== Movies with IMDB Score between " << minRating << " and " << maxRating << " (" << count << ") ===\n";
    for (int i = 0; i < count; i++) {
        results[i]->display();
    }
}

void MovieDatabase::displayActorInfo(const string& actorName) {
    ActorNode* actor = actorManager.findActor(actorName);
    
    if (actor) {
        actor->display();
    } else {
        cout << "Actor not found: " << actorName << endl;
    }
}

void MovieDatabase::displayAllActors() {
    actorManager.displayAll();
}

void MovieDatabase::displayStats() {
    cout << "\n=== Database Statistics ===\n";
    cout << "Total Movies: " << movieList.getCount() << endl;
    cout << "Total Actors: " << actorManager.getCount() << endl;
}