#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tree.h"
#include "utils.h"

static int compareNames(const char *a, const char *b) {
    int charA, charB;

    while (*a != '\0' && *b != '\0') {
        charA = tolower((unsigned char)*a);
        charB = tolower((unsigned char)*b);
        if (charA != charB) {
            return charA - charB;
        }
        a++;
        b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

TreeNode *insertTree(TreeNode *root, const Item *item, int *success) {
    if (root == NULL) {
        root = (TreeNode *)malloc(sizeof(TreeNode));
        if (root == NULL) {
            *success = 0;
            return NULL;
        }
        root->item = *item;
        root->left = NULL;
        root->right = NULL;
        return root;
    }
    if (compareNames(item->name, root->item.name) < 0) {
        root->left = insertTree(root->left, item, success);
    } else {
        root->right = insertTree(root->right, item, success);
    }
    return root;
}

TreeNode *searchTree(TreeNode *root, const char *name) {
    int comparison;

    while (root != NULL) {
        comparison = compareNames(name, root->item.name);
        if (comparison == 0) {
            return root;
        }
        root = (comparison < 0) ? root->left : root->right;
    }
    return NULL;
}

static TreeNode *findMinimum(TreeNode *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

TreeNode *deleteTree(TreeNode *root, const char *name, int id) {
    TreeNode *temp;
    TreeNode *successor;
    int comparison;

    if (root == NULL) {
        return NULL;
    }
    comparison = compareNames(name, root->item.name);
    if (comparison < 0) {
        root->left = deleteTree(root->left, name, id);
    } else if (comparison > 0 || root->item.id != id) {
        root->right = deleteTree(root->right, name, id);
    } else {
        if (root->left == NULL) {
            temp = root->right;
            free(root);
            return temp;
        }
        if (root->right == NULL) {
            temp = root->left;
            free(root);
            return temp;
        }
        successor = findMinimum(root->right);
        root->item = successor->item;
        root->right = deleteTree(root->right, successor->item.name, successor->item.id);
    }
    return root;
}

static void visitNode(const TreeNode *node) {
    printf("%s(ID %d)  ", node->item.name, node->item.id);
}

void preOrder(const TreeNode *root) {
    if (root == NULL) {
        return;
    }
    visitNode(root);
    preOrder(root->left);
    preOrder(root->right);
}

void inOrder(const TreeNode *root) {
    if (root == NULL) {
        return;
    }
    inOrder(root->left);
    visitNode(root);
    inOrder(root->right);
}

void postOrder(const TreeNode *root) {
    if (root == NULL) {
        return;
    }
    postOrder(root->left);
    postOrder(root->right);
    visitNode(root);
}

void freeTree(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

static TreeNode *buildTree(const ItemList *list) {
    TreeNode *root = NULL;
    int i, success;

    for (i = 0; i < list->count; i++) {
        success = 1;
        root = insertTree(root, &list->data[i], &success);
        if (!success) {
            printf("Memori tidak cukup untuk membangun BST.\n");
            freeTree(root);
            return NULL;
        }
    }
    return root;
}

static void removeFromList(ItemList *list, int id) {
    int index = findIndexById(list, id);
    int i;

    if (index < 0) {
        return;
    }
    for (i = index; i < list->count - 1; i++) {
        list->data[i] = list->data[i + 1];
    }
    list->count--;
}

int runTreeMenu(ItemList *list) {
    TreeNode *root = buildTree(list);
    TreeNode *found;
    Item target;
    char name[MAX_INPUT];
    int choice, changed = 0, success, oldCount;

    if (list->count > 0 && root == NULL) {
        return 0;
    }
    do {
        printf("\n--- KATALOG BST (kunci: nama item) ---\n");
        printf("1. Tambah item\n2. Cari item\n3. Hapus item\n");
        printf("4. Pre-order\n5. In-order (urut nama)\n6. Post-order\n0. Kembali\n");
        choice = readInt("Pilih: ", 0, 6);
        switch (choice) {
            case 1:
                oldCount = list->count;
                if (addItem(list) && list->count > oldCount) {
                    success = 1;
                    root = insertTree(root, &list->data[list->count - 1], &success);
                    if (!success) {
                        printf("Item tersimpan, tetapi gagal masuk BST (memori).\n");
                    }
                    changed = 1;
                }
                break;
            case 2:
                readLine("Nama item: ", name, sizeof(name));
                found = searchTree(root, name);
                if (found == NULL) {
                    printf("Item \"%s\" tidak ditemukan di BST.\n", name);
                } else {
                    printf("Ditemukan: ID %d | %s | %s | stok %d | harga %d\n",
                           found->item.id, found->item.name, found->item.category,
                           found->item.stock, found->item.price);
                }
                break;
            case 3:
                readLine("Nama item yang dihapus: ", name, sizeof(name));
                found = searchTree(root, name);
                if (found == NULL) {
                    printf("Item \"%s\" tidak ditemukan di BST.\n", name);
                } else {
                    target = found->item;
                    root = deleteTree(root, target.name, target.id);
                    removeFromList(list, target.id);
                    changed = 1;
                    printf("Item %s (ID %d) dihapus dari BST dan data.\n", target.name, target.id);
                }
                break;
            case 4:
                if (root == NULL) { printf("Katalog kosong.\n"); break; }
                printf("Pre-order : ");
                preOrder(root);
                printf("\n");
                break;
            case 5:
                if (root == NULL) { printf("Katalog kosong.\n"); break; }
                printf("In-order  : ");
                inOrder(root);
                printf("\n");
                break;
            case 6:
                if (root == NULL) { printf("Katalog kosong.\n"); break; }
                printf("Post-order: ");
                postOrder(root);
                printf("\n");
                break;
            default:
                break;
        }
    } while (choice != 0);

    freeTree(root);
    return changed;
}
