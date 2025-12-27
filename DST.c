#include <stdio.h>
#include <stdlib.h>

#define MAX_BITS 32   // For 32-bit integers

/* DST Node */
typedef struct DSTNode {
    int key;
    struct DSTNode *left;
    struct DSTNode *right;
} DSTNode;

/* Create node */
DSTNode* createNode(int key) {
    DSTNode *node = (DSTNode*)malloc(sizeof(DSTNode));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* Get bit at position (from MSB) */
int getBit(int key, int bit) {
    return (key >> (MAX_BITS - bit - 1)) & 1;
}

/* Insert */
DSTNode* insert(DSTNode *root, int key, int bit) {
    if (root == NULL)
        return createNode(key);

    if (bit >= MAX_BITS)
        return root;

    if (getBit(key, bit) == 0)
        root->left = insert(root->left, key, bit + 1);
    else
        root->right = insert(root->right, key, bit + 1);

    return root;
}

/* Search */
int search(DSTNode *root, int key, int bit) {
    if (root == NULL)
        return 0;

    if (root->key == key)
        return 1;

    if (bit >= MAX_BITS)
        return 0;

    if (getBit(key, bit) == 0)
        return search(root->left, key, bit + 1);
    else
        return search(root->right, key, bit + 1);
}

/* Find minimum (leftmost) */
DSTNode* findMin(DSTNode *root) {
    if (!root) return NULL;
    while (root->left)
        root = root->left;
    return root;
}

/* Delete */
DSTNode* deleteKey(DSTNode *root, int key, int bit) {
    if (root == NULL)
        return NULL;

    if (root->key == key) {
        if (!root->left && !root->right) {
            free(root);
            return NULL;
        }
        if (root->right) {
            DSTNode *temp = findMin(root->right);
            root->key = temp->key;
            root->right = deleteKey(root->right, temp->key, bit + 1);
        } else {
            DSTNode *temp = findMin(root->left);
            root->key = temp->key;
            root->left = deleteKey(root->left, temp->key, bit + 1);
        }
        return root;
    }

    if (bit >= MAX_BITS)
        return root;

    if (getBit(key, bit) == 0)
        root->left = deleteKey(root->left, key, bit + 1);
    else
        root->right = deleteKey(root->right, key, bit + 1);

    return root;
}

/* Inorder traversal */
void inorder(DSTNode *root) {
    if (root) {
        inorder(root->left);
        printf("%d ", root->key);
        inorder(root->right);
    }
}

/* Preorder traversal */
void preorder(DSTNode *root) {
    if (root) {
        printf("%d ", root->key);
        preorder(root->left);
        preorder(root->right);
    }
}

/* Main */
int main() {
    DSTNode *root = NULL;
    int choice, key;

    while (1) {
        printf("\n--- Digital Search Tree Menu ---\n");
        printf("1. Insert\n");
        printf("2. Search\n");
        printf("3. Delete\n");
        printf("4. Inorder Traversal\n");
        printf("5. Preorder Traversal\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter key: ");
                scanf("%d", &key);
                root = insert(root, key, 0);
                break;

            case 2:
                printf("Enter key: ");
                scanf("%d", &key);
                printf(search(root, key, 0) ? "Found\n" : "Not Found\n");
                break;

            case 3:
                printf("Enter key: ");
                scanf("%d", &key);
                root = deleteKey(root, key, 0);
                break;

            case 4:
                printf("Inorder: ");
                inorder(root);
                printf("\n");
                break;

            case 5:
                printf("Preorder: ");
                preorder(root);
                printf("\n");
                break;

            case 6:
                exit(0);
        }
    }
}
