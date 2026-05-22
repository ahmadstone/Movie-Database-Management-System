#include "AVLNode.h"
#include "MovieNode.h"

AVLNode::AVLNode(MovieNode* m) : movie(m), left(nullptr), right(nullptr), height(1) {}