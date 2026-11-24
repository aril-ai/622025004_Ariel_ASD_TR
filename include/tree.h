#ifndef TREE_H
#define TREE_H

#include "item.h"

typedef struct TreeNode {
    Item item;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode *insertTree(TreeNode *root, const Item *item, int *success);
TreeNode *searchTree(TreeNode *root, const char *name);
TreeNode *deleteTree(TreeNode *root, const char *name, int id);
void preOrder(const TreeNode *root);
void inOrder(const TreeNode *root);
void postOrder(const TreeNode *root);
void freeTree(TreeNode *root);
int runTreeMenu(ItemList *list);

#endif
