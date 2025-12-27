#include <stdio.h>

#define MAX 50

int hashTable[MAX];
int tableSize;
int collisionChoice;

/* Initialize hash table */
void initTable() {
    for (int i = 0; i < tableSize; i++)
        hashTable[i] = -1;
}

/* Division method hash function */
int hashFunction(int key) {
    return key % tableSize;
}

/* Second hash function (for double hashing) */
int secondHash(int key) {
    return 7 - (key % 7);   // 7 is a prime < tableSize
}

/* Insert using selected collision resolution technique */
void insert(int key) {
    int index = hashFunction(key);

    /* Linear Probing */
    if (collisionChoice == 1) {
        int i = index;
        while (hashTable[i] != -1) {
            i = (i + 1) % tableSize;
            if (i == index) {
                printf("Hash table is full!\n");
                return;
            }
        }
        hashTable[i] = key;
    }

    /* Quadratic Probing */
    else if (collisionChoice == 2) {
        int i = 0;
        while (i < tableSize) {
            int newIndex = (index + i * i) % tableSize;
            if (hashTable[newIndex] == -1) {
                hashTable[newIndex] = key;
                return;
            }
            i++;
        }
        printf("Hash table is full!\n");
    }

    /* Double Hashing */
    else if (collisionChoice == 3) {
        int i = 0;
        int step = secondHash(key);
        while (i < tableSize) {
            int newIndex = (index + i * step) % tableSize;
            if (hashTable[newIndex] == -1) {
                hashTable[newIndex] = key;
                return;
            }
            i++;
        }
        printf("Hash table is full!\n");
    }
}

/* Display hash table */
void display() {
    printf("\n--- HASH TABLE ---\n");
    for (int i = 0; i < tableSize; i++) {
        if (hashTable[i] == -1)
            printf("Index %d : EMPTY\n", i);
        else
            printf("Index %d : %d\n", i, hashTable[i]);
    }
}

/* Main function */
int main() {
    int choice, key;

    printf("Enter hash table size: ");
    scanf("%d", &tableSize);

    printf("\nSelect Collision Resolution Technique:");
    printf("\n1. Linear Probing");
    printf("\n2. Quadratic Probing");
    printf("\n3. Double Hashing");
    printf("\nEnter choice: ");
    scanf("%d", &collisionChoice);

    initTable();

    while (1) {
        printf("\n--- MENU ---");
        printf("\n1. Insert");
        printf("\n2. Display");
        printf("\n3. Exit");
        printf("\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter key to insert: ");
                scanf("%d", &key);
                insert(key);
                break;

            case 2:
                display();
                break;

            case 3:
                return 0;

            default:
                printf("Invalid choice!\n");
        }
    }
}
