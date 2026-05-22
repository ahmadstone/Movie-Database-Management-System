#include "AVLTree.h"
#include <iostream>

using namespace std;

AVLTree::AVLTree() : root(nullptr) {}
AVLTree::~AVLTree() { destroyTree(root); }

int AVLTree::max(int a, int b) {
    return (a > b) ? a : b;
}

int AVLTree::getHeight(AVLNode* node) {
    return node ? node->height : 0;
}

int AVLTree::getBalance(AVLNode* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

//rotations for balancing the tree

AVLNode* AVLTree::rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

	//performing rotation
    x->right = y;
    y->left = T2;

	//updating heights
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

AVLNode* AVLTree::leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

	//performing rotation
    y->left = x;
    x->right = T2;

	//updating heights
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

//insertion with balancing into the AVL tree

//function for handling duplicates and sorting criteria
//Sorts by movie title (case-insensitive)
bool AVLTree::compareMovies(MovieNode* a, MovieNode* b) {
    // Convert titles to lowercase for case-insensitive comparison
    string titleA = a->movie_title;
    string titleB = b->movie_title;
    
    // Convert to lowercase
    for (size_t i = 0; i < titleA.length(); i++) {
        if (titleA[i] >= 'A' && titleA[i] <= 'Z') {
            titleA[i] = titleA[i] - 'A' + 'a';
        }
    }
    for (size_t i = 0; i < titleB.length(); i++) {
        if (titleB[i] >= 'A' && titleB[i] <= 'Z') {
            titleB[i] = titleB[i] - 'A' + 'a';
        }
    }
    
    return titleA < titleB;  // Sort by title alphabetically
}

AVLNode* AVLTree::insert(AVLNode* node, MovieNode* movie) {
    if (!node) { 
        return new AVLNode(movie); 
    }

    if (compareMovies(movie, node->movie)) {
        node->left = insert(node->left, movie);
    }
    else {
        node->right = insert(node->right, movie);
    }

	//updating height
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

	//balance factor
    int balance = getBalance(node);

    //left left imbalance
    if (balance > 1 && compareMovies(movie, node->left->movie))
        return rightRotate(node);

	//right right imbalance
    if (balance < -1 && !compareMovies(movie, node->right->movie))
        return leftRotate(node);

	//left right imbalance
    if (balance > 1 && !compareMovies(movie, node->left->movie)) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

	//right left imbalance
    if (balance < -1 && compareMovies(movie, node->right->movie)) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void AVLTree::insert(MovieNode* movie) {
    root = insert(root, movie);
}

//AVL traversal for displaying sorted movies

void AVLTree::inorder(AVLNode* node) {
    if (!node) return;
    inorder(node->left);
    node->movie->display();
    inorder(node->right);
}

void AVLTree::displaySorted() {
    cout << "\n=== Movies Sorted by Title ===\n";
    inorder(root);
}



//searching and counting movies by minimum score

void AVLTree::searchByScoreHelper(AVLNode* node, float minScore, MovieNode** results, int& count, int maxSize) {
    if (!node || count >= maxSize) return;
    if (node->movie->imdb_score >= minScore) {
        searchByScoreHelper(node->left, minScore, results, count, maxSize);
        if (count < maxSize) { 
            results[count++] = node->movie; 
        }
        searchByScoreHelper(node->right, minScore, results, count, maxSize);
    }
    else {
        searchByScoreHelper(node->right, minScore, results, count, maxSize);
    }
}

int AVLTree::searchByScore(float minScore, MovieNode** results, int maxResults) {
    int count = 0;
    searchByScoreHelper(root, minScore, results, count, maxResults);
    return count;
}

int AVLTree::countByScore(AVLNode* node, float minScore) {
    if (!node) return 0;

    int count = 0;
    if (node->movie->imdb_score >= minScore) {
        count += countByScore(node->left, minScore);
        count++;
        count += countByScore(node->right, minScore);
    }
    else {
        count += countByScore(node->right, minScore);
    }
    return count;
}

int AVLTree::countByScore(float minScore) {
    return countByScore(root, minScore);
}


void AVLTree::destroyTree(AVLNode* node) {
    if (!node) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

int AVLTree::getTreeHeight() {
    return getHeight(root);
}
