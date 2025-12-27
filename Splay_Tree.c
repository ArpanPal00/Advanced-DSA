#include <stdio.h>
#include <stdlib.h>

/* Node structure */
struct Node {
    int key;
    struct Node *left, *right;
};

/* Create a new node */
struct Node* newNode(int key) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->key = key;
    node->left = node->right = NULL;
    return node;
}

/* Right rotation */
struct Node* rightRotate(struct Node* x) {
    struct Node* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

/* Left rotation */
struct Node* leftRotate(struct Node* x) {
    struct Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

/* Top-Down Splay operation */
struct Node* splay(struct Node* root, int key) {
    if (root == NULL)
        return NULL;

    struct Node header;
    header.left = header.right = NULL;

    struct Node *LeftTreeMax = &header;
    struct Node *RightTreeMin = &header;

    while (1) {
        if (key < root->key) {
            if (root->left == NULL)
                break;

            if (key < root->left->key) {
                root = rightRotate(root);
                if (root->left == NULL)
                    break;
            }

            RightTreeMin->left = root;
            RightTreeMin = root;
            root = root->left;
        }
        else if (key > root->key) {
            if (root->right == NULL)
                break;

            if (key > root->right->key) {
                root = leftRotate(root);
                if (root->right == NULL)
                    break;
            }

            LeftTreeMax->right = root;
            LeftTreeMax = root;
            root = root->right;
        }
        else
            break;
    }

    /* Reassemble trees */
    LeftTreeMax->right = root->left;
    RightTreeMin->left = root->right;
    root->left = header.right;
    root->right = header.left;

    return root;
}

/* Insert */
struct Node* insert(struct Node* root, int key) {
    if (root == NULL)
        return newNode(key);

    root = splay(root, key);

    if (root->key == key)
        return root;

    struct Node* node = newNode(key);

    if (key < root->key) {
        node->right = root;
        node->left = root->left;
        root->left = NULL;
    } else {
        node->left = root;
        node->right = root->right;
        root->right = NULL;
    }

    return node;
}

/* Search */
struct Node* search(struct Node* root, int key) {
    return splay(root, key);
}

/* Delete */
struct Node* deleteNode(struct Node* root, int key) {
    struct Node* temp;

    if (root == NULL)
        return NULL;

    root = splay(root, key);

    if (root->key != key)
        return root;

    if (root->left == NULL) {
        temp = root;
        root = root->right;
    } else {
        temp = root;
        root = splay(root->left, key);
        root->right = temp->right;
    }

    free(temp);
    return root;
}

/* Inorder traversal */
void inorder(struct Node* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->key);
        inorder(root->right);
    }
}

/* Main function (Menu Driven) */
int main() {
    struct Node* root = NULL;
    int choice, key;

    while (1) {
        printf("\n\n--- TOP DOWN SPLAY TREE MENU ---");
        printf("\n1. Insert");
        printf("\n2. Search");
        printf("\n3. Delete");
        printf("\n4. Inorder Traversal");
        printf("\n5. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter key to insert: ");
                scanf("%d", &key);
                root = insert(root, key);
                break;

            case 2:
                printf("Enter key to search: ");
                scanf("%d", &key);
                root = search(root, key);
                if (root != NULL && root->key == key)
                    printf("Key found and splayed to root.\n");
                else
                    printf("Key not found.\n");
                break;

            case 3:
                printf("Enter key to delete: ");
                scanf("%d", &key);
                root = deleteNode(root, key);
                break;

            case 4:
                printf("Inorder Traversal: ");
                inorder(root);
                break;

            case 5:
                printf("Exiting program.\n");
                exit(0);

            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}
