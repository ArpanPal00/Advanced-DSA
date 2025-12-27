#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 256

/* Suffix Tree Node */
typedef struct SuffixTreeNode {
    struct SuffixTreeNode *children[MAX_CHAR];
    struct SuffixTreeNode *suffixLink;
    int start;
    int *end;
    int suffixIndex;
} Node;

char text[1000];
Node *root = NULL;
Node *lastNewNode = NULL;
Node *activeNode = NULL;

int activeEdge = -1;
int activeLength = 0;
int remainingSuffixCount = 0;
int leafEnd = -1;
int *rootEnd = NULL;
int size = -1;

/* Create node */
Node* newNode(int start, int *end) {
    Node *node = (Node*)malloc(sizeof(Node));
    for (int i = 0; i < MAX_CHAR; i++)
        node->children[i] = NULL;

    node->suffixLink = root;
    node->start = start;
    node->end = end;
    node->suffixIndex = -1;
    return node;
}

/* Edge length */
int edgeLength(Node *n) {
    return *(n->end) - n->start + 1;
}

/* Walk down */
int walkDown(Node *currNode) {
    if (activeLength >= edgeLength(currNode)) {
        activeEdge += edgeLength(currNode);
        activeLength -= edgeLength(currNode);
        activeNode = currNode;
        return 1;
    }
    return 0;
}

/* Extend suffix tree */
void extendSuffixTree(int pos) {
    leafEnd = pos;
    remainingSuffixCount++;
    lastNewNode = NULL;

    while (remainingSuffixCount > 0) {
        if (activeLength == 0)
            activeEdge = pos;

        if (!activeNode->children[(unsigned char)text[activeEdge]]) {
            activeNode->children[(unsigned char)text[activeEdge]] =
                newNode(pos, &leafEnd);

            if (lastNewNode) {
                lastNewNode->suffixLink = activeNode;
                lastNewNode = NULL;
            }
        } else {
            Node *next = activeNode->children[(unsigned char)text[activeEdge]];
            if (walkDown(next))
                continue;

            if (text[next->start + activeLength] == text[pos]) {
                if (lastNewNode && activeNode != root) {
                    lastNewNode->suffixLink = activeNode;
                    lastNewNode = NULL;
                }
                activeLength++;
                break;
            }

            int *splitEnd = (int*)malloc(sizeof(int));
            *splitEnd = next->start + activeLength - 1;

            Node *split = newNode(next->start, splitEnd);
            activeNode->children[(unsigned char)text[activeEdge]] = split;

            split->children[(unsigned char)text[pos]] =
                newNode(pos, &leafEnd);

            next->start += activeLength;
            split->children[(unsigned char)text[next->start]] = next;

            if (lastNewNode)
                lastNewNode->suffixLink = split;

            lastNewNode = split;
        }

        remainingSuffixCount--;

        if (activeNode == root && activeLength > 0) {
            activeLength--;
            activeEdge = pos - remainingSuffixCount + 1;
        } else if (activeNode != root) {
            activeNode = activeNode->suffixLink;
        }
    }
}

/* Build suffix tree */
void buildSuffixTree() {
    size = strlen(text);
    rootEnd = (int*)malloc(sizeof(int));
    *rootEnd = -1;

    root = newNode(-1, rootEnd);
    activeNode = root;

    for (int i = 0; i < size; i++)
        extendSuffixTree(i);
}

/* Set suffix indices */
void setSuffixIndexByDFS(Node *n, int labelHeight) {
    if (!n) return;

    int leaf = 1;
    for (int i = 0; i < MAX_CHAR; i++) {
        if (n->children[i]) {
            leaf = 0;
            setSuffixIndexByDFS(n->children[i],
                labelHeight + edgeLength(n->children[i]));
        }
    }

    if (leaf)
        n->suffixIndex = size - labelHeight;
}

/* Display tree */
void printTree(Node *n, int height) {
    if (!n) return;

    if (n->start != -1) {
        for (int i = 0; i < height; i++) printf(" ");
        for (int i = n->start; i <= *(n->end); i++)
            printf("%c", text[i]);
        printf("\n");
    }

    for (int i = 0; i < MAX_CHAR; i++)
        printTree(n->children[i], height + 2);
}

/* Pattern search */
int doTraversal(Node *n, char *pat, int idx) {
    if (!n) return 0;

    if (n->start != -1) {
        for (int i = n->start; i <= *(n->end) && pat[idx]; i++, idx++) {
            if (text[i] != pat[idx])
                return 0;
        }
    }

    if (!pat[idx])
        return 1;

    return doTraversal(n->children[(unsigned char)pat[idx]], pat, idx);
}

int search(char *pattern) {
    return doTraversal(root, pattern, 0);
}

/* Free tree */
void freeTree(Node *n) {
    if (!n) return;

    for (int i = 0; i < MAX_CHAR; i++)
        freeTree(n->children[i]);

    if (n->end && n->end != rootEnd)
        free(n->end);

    free(n);
}

/* Main */
int main() {
    printf("Enter string (end with $): ");
    scanf("%s", text);

    buildSuffixTree();
    setSuffixIndexByDFS(root, 0);

    printf("\nSuffix Tree:\n");
    printTree(root, 0);

    char pattern[100];
    printf("\nEnter pattern to search: ");
    scanf("%s", pattern);

    if (search(pattern))
        printf("Pattern found\n");
    else
        printf("Pattern not found\n");

    freeTree(root);
    return 0;
}
