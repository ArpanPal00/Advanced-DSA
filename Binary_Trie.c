#include <stdio.h>
#include <stdlib.h>

#define BITS 8   // Number of bits (change to 16 or 32 if needed)

/* Node type */
typedef enum {
    BRANCH,
    ELEMENT
} NodeType;

/* Trie Node */
typedef struct TrieNode {
    NodeType type;
    int value;                  // Only used for ELEMENT nodes
    struct TrieNode *left;      // bit = 0
    struct TrieNode *right;     // bit = 1
} TrieNode;

/* Create branch node */
TrieNode* createBranchNode() {
    TrieNode *node = (TrieNode*)malloc(sizeof(TrieNode));
    node->type = BRANCH;
    node->value = -1;
    node->left = node->right = NULL;
    return node;
}

/* Create element (leaf) node */
TrieNode* createElementNode(int value) {
    TrieNode *node = (TrieNode*)malloc(sizeof(TrieNode));
    node->type = ELEMENT;
    node->value = value;
    node->left = node->right = NULL;
    return node;
}

/* Insert value */
TrieNode* insert(TrieNode *root, int value) {
    if (!root)
        root = createBranchNode();

    TrieNode *curr = root;

    for (int i = BITS - 1; i >= 0; i--) {
        int bit = (value >> i) & 1;

        if (bit == 0) {
            if (!curr->left)
                curr->left = createBranchNode();
            curr = curr->left;
        } else {
            if (!curr->right)
                curr->right = createBranchNode();
            curr = curr->right;
        }
    }

    /* Attach element node at leaf */
    if (curr->left)
        free(curr->left);  // overwrite if exists

    curr->left = createElementNode(value);
    return root;
}

/* Search value */
int search(TrieNode *root, int value) {
    if (!root) return 0;

    TrieNode *curr = root;

    for (int i = BITS - 1; i >= 0; i--) {
        int bit = (value >> i) & 1;

        if (bit == 0)
            curr = curr->left;
        else
            curr = curr->right;

        if (!curr)
            return 0;
    }

    return (curr->left && curr->left->type == ELEMENT &&
            curr->left->value == value);
}

/* Delete value */
int deleteHelper(TrieNode *node, int value, int bitPos) {
    if (!node)
        return 0;

    if (bitPos < 0) {
        if (node->left && node->left->type == ELEMENT &&
            node->left->value == value) {
            free(node->left);
            node->left = NULL;
            return 1;
        }
        return 0;
    }

    int bit = (value >> bitPos) & 1;
    int deleted;

    if (bit == 0)
        deleted = deleteHelper(node->left, value, bitPos - 1);
    else
        deleted = deleteHelper(node->right, value, bitPos - 1);

    /* Remove empty branch nodes */
    if (deleted) {
        if (node->left && node->left->type == BRANCH &&
            !node->left->left && !node->left->right) {
            free(node->left);
            node->left = NULL;
        }

        if (node->right && node->right->type == BRANCH &&
            !node->right->left && !node->right->right) {
            free(node->right);
            node->right = NULL;
        }
    }

    return deleted;
}

void deleteValue(TrieNode *root, int value) {
    if (!deleteHelper(root, value, BITS - 1))
        printf("Value not found\n");
}

/* Display elements */
void display(TrieNode *node) {
    if (!node) return;

    if (node->type == ELEMENT) {
        printf("%d ", node->value);
        return;
    }

    display(node->left);
    display(node->right);
}

/* Main */
int main() {
    TrieNode *root = NULL;
    int choice, value;

    while (1) {
        printf("\n--- Binary Trie Menu ---\n");
        printf("1. Insert\n");
        printf("2. Search\n");
        printf("3. Delete\n");
        printf("4. Display\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter integer: ");
                scanf("%d", &value);
                root = insert(root, value);
                break;

            case 2:
                printf("Enter integer: ");
                scanf("%d", &value);
                printf(search(root, value) ? "Found\n" : "Not Found\n");
                break;

            case 3:
                printf("Enter integer: ");
                scanf("%d", &value);
                deleteValue(root, value);
                break;

            case 4:
                printf("Elements in Binary Trie: ");
                display(root);
                printf("\n");
                break;

            case 5:
                exit(0);
        }
    }
}
