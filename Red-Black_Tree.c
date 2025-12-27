#include <stdio.h>
#include <stdlib.h>

#define RED 1
#define BLACK 0

/* Node structure */
typedef struct RBNode {
    int data;
    int color;
    struct RBNode *left, *right, *parent;
} RBNode;

RBNode *root = NULL;
RBNode *NIL;

/* Create NIL node */
void initNIL() {
    NIL = (RBNode*)malloc(sizeof(RBNode));
    NIL->color = BLACK;
    NIL->left = NIL->right = NIL->parent = NULL;
}

/* Create new node */
RBNode* createNode(int data) {
    RBNode *node = (RBNode*)malloc(sizeof(RBNode));
    node->data = data;
    node->color = RED;
    node->left = node->right = node->parent = NIL;
    return node;
}

/* Left Rotate */
void leftRotate(RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;

    if (y->left != NIL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NIL)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

/* Right Rotate */
void rightRotate(RBNode *y) {
    RBNode *x = y->left;
    y->left = x->right;

    if (x->right != NIL)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == NIL)
        root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;
}

/* Fix insertion */
void insertFix(RBNode *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode *y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        } else {
            RBNode *y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

/* Insert */
void insert(int data) {
    RBNode *z = createNode(data);
    RBNode *y = NIL;
    RBNode *x = root;

    while (x != NIL) {
        y = x;
        if (z->data < x->data)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;

    if (y == NIL)
        root = z;
    else if (z->data < y->data)
        y->left = z;
    else
        y->right = z;

    insertFix(z);
}

/* Transplant */
void transplant(RBNode *u, RBNode *v) {
    if (u->parent == NIL)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

/* Minimum */
RBNode* minimum(RBNode *node) {
    while (node->left != NIL)
        node = node->left;
    return node;
}

/* Fix deletion */
void deleteFix(RBNode *x) {
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBNode *w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        } else {
            RBNode *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

/* Delete */
void deleteNode(int key) {
    RBNode *z = root;
    while (z != NIL && z->data != key) {
        if (key < z->data)
            z = z->left;
        else
            z = z->right;
    }

    if (z == NIL) {
        printf("Key not found\n");
        return;
    }

    RBNode *y = z;
    int yOriginalColor = y->color;
    RBNode *x;

    if (z->left == NIL) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == NIL) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimum(z->right);
        yOriginalColor = y->color;
        x = y->right;

        if (y->parent == z)
            x->parent = y;
        else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (yOriginalColor == BLACK)
        deleteFix(x);

    free(z);
}

/* Search */
RBNode* search(RBNode *node, int key) {
    if (node == NIL || node->data == key)
        return node;
    if (key < node->data)
        return search(node->left, key);
    return search(node->right, key);
}

/* Inorder traversal */
void inorder(RBNode *node) {
    if (node != NIL) {
        inorder(node->left);
        printf("%d(%s) ", node->data,
               node->color == RED ? "R" : "B");
        inorder(node->right);
    }
}

/* Main */
int main() {
    initNIL();
    root = NIL;

    int choice, key;

    while (1) {
        printf("\n--- Red Black Tree Menu ---\n");
        printf("1. Insert\n2. Delete\n3. Search\n4. Inorder\n5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter key: ");
                scanf("%d", &key);
                insert(key);
                break;

            case 2:
                printf("Enter key: ");
                scanf("%d", &key);
                deleteNode(key);
                break;

            case 3:
                printf("Enter key: ");
                scanf("%d", &key);
                printf(search(root, key) != NIL ? "Found\n" : "Not Found\n");
                break;

            case 4:
                inorder(root);
                printf("\n");
                break;

            case 5:
                exit(0);
        }
    }
}
