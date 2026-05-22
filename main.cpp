#include "MovieDatabase.h"
#include <iostream>

using namespace std;

void displayMenu() {
    cout << "\n========================================\n";
    cout << "      MOVIE DATABASE MANAGEMENT SYSTEM\n";
    cout << "========================================\n";
    cout << "1.  Display All Movies\n";
    cout << "2.  Display Movies Sorted by Title\n";
    cout << "3.  Search Movies (Title/Actor/Director/Genre/Year/Rating Range)\n";
    cout << "4.  Display Actor Information\n";
    cout << "5.  Display All Actors\n";
    cout << "6.  Display Database Statistics\n";
    cout << "7.  BFS Traversal (from movie index)\n";
    cout << "8.  DFS Traversal (from movie index)\n";
    cout << "9.  Shortest Path Between 2 Movies (by index)\n";
    cout << "10. Shortest Path Between 2 Movies (by title)\n";
    cout << "11. BFS Traversal (from movie title)\n";
    cout << "12. DFS Traversal (from movie title)\n";
    cout << "13. Display Graph Structure\n";
    cout << "14. Get Movie Recommendations (by genre/actors)\n";
    cout << "15. Exit\n";
    cout << "========================================\n";
    cout << "Enter your choice: ";
}

int getMovieIndexByTitle(MovieDatabase& database, const string& title) {
    int index = database.getMovieIndexByTitle(title);
    if (index == -1) {
        cout << "Movie not found: " << title << endl;
    }
    return index;
}

void handleBFSByIndex(Graph& graph, int totalMovies) {
    int index;
    cout << "Enter movie index (0 to " << (totalMovies - 1) << "): ";
    cin >> index;
    cin.ignore();

    if (index < 0 || index >= totalMovies) {
        cout << "Invalid index. Please enter a value between 0 and " << (totalMovies - 1) << ".\n";
        return;
    }

    graph.BFS(index);
}

void handleDFSByIndex(Graph& graph, int totalMovies) {
    int index;
    cout << "Enter movie index (0 to " << (totalMovies - 1) << "): ";
    cin >> index;
    cin.ignore();

    if (index < 0 || index >= totalMovies) {
        cout << "Invalid index. Please enter a value between 0 and " << (totalMovies - 1) << ".\n";
        return;
    }

    graph.DFS(index);
}

void handleShortestPathByIndex(Graph& graph, int totalMovies) {
    int m1, m2;
    cout << "Enter start movie index (0 to " << (totalMovies - 1) << "): ";
    cin >> m1;
    cout << "Enter end movie index (0 to " << (totalMovies - 1) << "): ";
    cin >> m2;
    cin.ignore();

    if (m1 < 0 || m1 >= totalMovies || m2 < 0 || m2 >= totalMovies) {
        cout << "Invalid index. Please enter values between 0 and " << (totalMovies - 1) << ".\n";
        return;
    }

    if (m1 == m2) {
        cout << "Start and end movies are the same.\n";
        return;
    }

    graph.BFS_ShortestPath(m1, m2);
}

void handleShortestPathByTitle(MovieDatabase& database, Graph& graph) {
    string title1, title2;
    cout << "Enter start movie title: ";
    getline(cin, title1);
    cout << "Enter end movie title: ";
    getline(cin, title2);

    int index1 = getMovieIndexByTitle(database, title1);
    int index2 = getMovieIndexByTitle(database, title2);

    if (index1 != -1 && index2 != -1) {
        if (index1 == index2) {
            cout << "Start and end movies are the same.\n";
            return;
        }
        graph.BFS_ShortestPath(index1, index2);
    }
}

void handleBFSByTitle(MovieDatabase& database, Graph& graph) {
    string title;
    cout << "Enter movie title for BFS traversal: ";
    getline(cin, title);

    int index = getMovieIndexByTitle(database, title);
    if (index != -1) {
        graph.BFS(index);
    }
}

void handleDFSByTitle(MovieDatabase& database, Graph& graph) {
    string title;
    cout << "Enter movie title for DFS traversal: ";
    getline(cin, title);

    int index = getMovieIndexByTitle(database, title);
    if (index != -1) {
        graph.DFS(index);
    }
}

int main() {
    MovieDatabase database;

    cout << "========================================\n";
    cout << "   Initializing Movie Database System\n";
    cout << "========================================\n";

    // Loading data from the CSV file
    string filename = "movie_metadata.csv";
    int records = 1000;
    cout << "\nLoading data from " << filename << "...\n";
    database.loadFromCSV(filename, records);

    // Building the graph
    cout << "\nBuilding movie connection graph...\n";
    Graph graph(records);
    database.buildGraph(graph, records);
    cout << "Graph construction completed.\n";

    // Get actual number of movies loaded
    int totalMovies = records;

    int choice;
    string searchTerm;

    do {
        displayMenu();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        cin.ignore();
        cout << endl;

        switch (choice) {
        case 1:
            database.displayAllMovies();
            break;

        case 2:
            database.displaySortedByScore();
            break;

        case 3: {
            int searchChoice;
            cout << "\n=== Search Movies ===\n";
            cout << "1. Search by Title\n";
            cout << "2. Search by Actor\n";
            cout << "3. Search by Director\n";
            cout << "4. Search by Genre\n";
            cout << "5. Search by Year\n";
            cout << "6. Search by Rating Range\n";
            cout << "Enter search type: ";

            if (!(cin >> searchChoice)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input. Please enter a number.\n";
                break;
            }
            cin.ignore();
            cout << endl;

            switch (searchChoice) {
            case 1: {
                cout << "Enter movie title (or part of title): ";
                getline(cin, searchTerm);
                if (!searchTerm.empty()) {
                    database.searchByTitle(searchTerm);
                }
                else {
                    cout << "Title cannot be empty.\n";
                }
                break;
            }
            case 2: {
                cout << "Enter actor name: ";
                getline(cin, searchTerm);
                if (!searchTerm.empty()) {
                    database.searchActor(searchTerm);
                }
                else {
                    cout << "Actor name cannot be empty.\n";
                }
                break;
            }
            case 3: {
                cout << "Enter director name to search: ";
                getline(cin, searchTerm);
                if (!searchTerm.empty()) {
                    database.searchDirector(searchTerm);
                }
                else {
                    cout << "Director name cannot be empty.\n";
                }
                break;
            }
            case 4: {
                cout << "Enter genre: ";
                getline(cin, searchTerm);
                if (!searchTerm.empty()) {
                    database.searchByGenre(searchTerm);
                }
                else {
                    cout << "Genre cannot be empty.\n";
                }
                break;
            }
            case 5: {
                int year;
                cout << "Enter year: ";
                if (cin >> year) {
                    cin.ignore();
                    if (year >= 1900 && year <= 2100) {
                        database.searchByYear(year);
                    }
                    else {
                        cout << "Invalid year. Please enter a year between 1900 and 2100.\n";
                    }
                }
                else {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input. Please enter a number.\n";
                }
                break;
            }
            case 6: {
                float minRating, maxRating;
                cout << "Enter minimum rating (0.0 to 10.0): ";
                if (cin >> minRating) {
                    cout << "Enter maximum rating (0.0 to 10.0): ";
                    if (cin >> maxRating) {
                        cin.ignore();
                        if (minRating >= 0.0f && minRating <= 10.0f &&
                            maxRating >= 0.0f && maxRating <= 10.0f &&
                            minRating <= maxRating) {
                            database.searchByRatingRange(minRating, maxRating);
                        }
                        else {
                            cout << "Invalid rating range. Please enter values between 0.0 and 10.0, with min <= max.\n";
                        }
                    }
                    else {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << "Invalid input. Please enter a number.\n";
                    }
                }
                else {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input. Please enter a number.\n";
                }
                break;
            }
            default:
                cout << "Invalid search type. Please enter a number between 1 and 6.\n";
            }
            break;
        }

        case 4:
            cout << "Enter actor name: ";
            getline(cin, searchTerm);
            if (!searchTerm.empty()) {
                database.displayActorInfo(searchTerm);
            }
            else {
                cout << "Actor name cannot be empty.\n";
            }
            break;

        case 5:
            database.displayAllActors();
            break;

        case 6:
            database.displayStats();
            break;

        case 7:
            handleBFSByIndex(graph, totalMovies);
            break;

        case 8:
            handleDFSByIndex(graph, totalMovies);
            break;

        case 9:
            handleShortestPathByIndex(graph, totalMovies);
            break;

        case 10:
            handleShortestPathByTitle(database, graph);
            break;

        case 11:
            handleBFSByTitle(database, graph);
            break;

        case 12:
            handleDFSByTitle(database, graph);
            break;

        case 13:
            cout << "\n=== Graph Structure ===\n";
            graph.display();
            break;

        case 14: {
            string title;
            cout << "Enter movie title for recommendations: ";
            getline(cin, title);

            int index = getMovieIndexByTitle(database, title);
            if (index != -1) {
                graph.recommendMovies(index, 20);
            }
            break;
        }

        case 15:
            cout << "\nThank you for using Movie Database Management System!\n";
            cout << "Exiting program...\n";
            break;

        default:
            cout << "Invalid choice. Please enter a number between 1 and 15.\n";
        }

        if (choice != 15) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }

    } while (choice != 15);

    return 0;
}