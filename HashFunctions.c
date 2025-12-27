#include <stdio.h>
#include <math.h>

#define MAX 50

int hashTable[MAX];
int tableSize;
int hashChoice;

/* Initialize hash table */
void initTable() {
    for (int i = 0; i < tableSize; i++)
        hashTable[i] = -1;
}

/* 1. Division Method */
int divisionHash(int key) {
    return key % tableSize;
}

/* 2. Multiplication Method */
int multiplicationHash(int key) {
    float A = 0.618033;
    float temp = key * A;
    temp = temp - (int)temp;
    return (int)(tableSize * temp);
}

/* 3. Mid-Square Method */
int midSquareHash(int key) {
    long square = (long)key * key;
    square = square / 10;
    return (square % 100) % tableSize;
}

/* 4. Folding Method */
int foldingHash(int key) {
    int sum = 0;
    while (key > 0) {
        sum += key % 100;
        key /= 100;
    }
    return sum % tableSize;
}

/* Get hash value based on user choice */
int getHash(int key) {
    switch (hashChoice) {
        case 1: return divisionHash(key);
        case 2: return multiplicationHash(key);
        case 3: return midSquareHash(key);
        case 4: return foldingHash(key);
        default: return 0;
    }
}

/* Insert using Linear Probing */
void insert(int key) {
    int index = getHash(key);

    if (hashTable[index] == -1) {
        hashTable[index] = key;
    } else {
        int i = (index + 1) % tableSize;
        while (i != index) {
            if (hashTable[i] == -1) {
                hashTable[i] = key;
                return;
            }
            i = (i + 1) % tableSize;
        }
        printf("Hash table is full!\n");
    }
}

/* Display hash table */
void display() {
    printf("\n--- HASH TABLE ---\n");
    for (int i = 0; i < tableSize; i++) {
        if (hashTable[i] != -1)
            printf("Index %d : %d\n", i, hashTable[i]);
        else
            printf("Index %d : EMPTY\n", i);
    }
}

/* Main function */
int main() {
    int choice, key;

    printf("Enter hash table size: ");
    scanf("%d", &tableSize);

    printf("\nSelect Hash Function:");
    printf("\n1. Division Method");
    printf("\n2. Multiplication Method");
    printf("\n3. Mid-Square Method");
    printf("\n4. Folding Method");
    printf("\nEnter choice: ");
    scanf("%d", &hashChoice);

    initTable();

    while (1) {
        printf("\n--- MENU ---");
        printf("\n1. Insert key");
        printf("\n2. Display hash table");
        printf("\n3. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter key: ");
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
