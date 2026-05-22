# Movie Database Management System

A console-based C++ application that loads ~5,000 movies from an IMDb dataset and provides fast search, sorted display, graph-based similarity traversal, and movie recommendations using core data structures.

Built as a Data Structures course project by **Muhammad Ahmad (24i-2548)** and **Khadija Zaidi (24i-6506)** at FAST National University, Pakistan.

---

## Features

1. **CSV Data Loading** — Robust parser handling quoted fields, escaped quotes, non-breaking spaces, UTF-8 sanitization, and incomplete records.
2. **Display All Movies** — Iterates the linked list and prints every movie's full details.
3. **Display Sorted by Title** — Uses AVL tree in-order traversal (case-insensitive alphabetical order).
4. **Search Movies** — By Title, Actor, Director, Genre, Year, or IMDB Rating Range via hash tables (O(1) average lookup).
5. **Actor Information** — Shows all movies for a given actor.
6. **Display All Actors** — Lists every unique actor in the database.
7. **Database Statistics** — Total movie count and total actor count.
8. **BFS Traversal** — By movie index or movie title.
9. **DFS Traversal** — By movie index or movie title.
10. **Shortest Path** — Between any two movies using BFS with parent tracking.
11. **Movie Recommendations** — BFS-based; returns up to 20 similar movies via shared actors/genres.
12. **Display Graph Structure** — Shows the adjacency list for every movie vertex.

---

## Menu Options

```
========================================
      MOVIE DATABASE MANAGEMENT SYSTEM
========================================
1.  Display All Movies
2.  Display Movies Sorted by Title
3.  Search Movies (Title/Actor/Director/Genre/Year/Rating Range)
4.  Display Actor Information
5.  Display All Actors
6.  Display Database Statistics
7.  BFS Traversal (from movie index)
8.  DFS Traversal (from movie index)
9.  Shortest Path Between 2 Movies (by index)
10. Shortest Path Between 2 Movies (by title)
11. BFS Traversal (from movie title)
12. DFS Traversal (from movie title)
13. Display Graph Structure
14. Get Movie Recommendations (by genre/actors)
15. Exit
========================================
```

Option **3** opens a sub-menu with 6 search types: Title, Actor, Director, Genre, Year, and Rating Range.

---

## Acknowledgments

The movie dataset used in this project is sourced from the [IMDB 5000 Movie Dataset](https://www.kaggle.com/datasets/carolzhangdc/imdb-5000-movie-dataset) on Kaggle, originally compiled by Carol Zhang.

---

## Core Data Structures

| Data Structure | Purpose |
|---|---|
| **Singly Linked List** (`MovieList`) | Stores all movie objects in insertion order; provides sequential traversal |
| **AVL Tree** (`AVLTree`/`AVLNode`) | Self-balancing BST keyed by movie title (case-insensitive); enables O(log n) sorted display and score-based queries |
| **Hash Table with Chaining** (`HashTable`) | Five separate tables indexing movies by actor name, title, genre, director, and year. Uses DJB2 hash function for O(1) average search |
| **Undirected Graph** (`Graph`) | Movies as vertices, edges connecting movies that share actors or genres. Uses adjacency list (custom `LinkedList` of `ListNode`) |
| **Dynamic Array** (`ActorManager`/`ActorNode`) | Resizable array storing all unique actors; each actor holds a dynamic array of pointers to their movies |

---

## Getting Started

### Prerequisites
- A C++ compiler (e.g., GCC, MSVC, Clang) supporting C++11 or higher.
- Ensure `movie_metadata.csv` is present in the project root directory.

### Compilation

You can compile the project using standard C++ compilation commands, for example with g++:

```
g++ *.cpp -o MovieDB
```

### Running the System

```
./MovieDB
```

(On Windows Command Prompt or PowerShell, run `.\MovieDB.exe`)

---

The program loads `movie_metadata.csv` on startup. By default it processes up to 1000 records — change the `records` variable in `main.cpp` (line ~136) to load more (up to ~5044).
