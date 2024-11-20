#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>

#define MAX_DATA_SIZE 256

typedef struct Block {
    int index;
    time_t timestamp;
    char data[MAX_DATA_SIZE];
    char previousHash[SHA256_DIGEST_LENGTH * 2 + 1];
    char hash[SHA256_DIGEST_LENGTH * 2 + 1];
    struct Block* next;
} Block;

typedef struct {
    Block* head;
    int length;
} Blockchain;

// Function to calculate SHA-256 hash for a block
void calculateHash(Block* block) {
    char input[MAX_DATA_SIZE + sizeof(int) + sizeof(time_t) + SHA256_DIGEST_LENGTH];
    snprintf(input, sizeof(input), "%d%ld%s%s", block->index, block->timestamp, block->data, block->previousHash);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input, strlen(input), hash);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(block->hash + (i * 2), "%02x", hash[i]);
    }
    block->hash[SHA256_DIGEST_LENGTH * 2] = '\0';
}

// Function to create the genesis block
Block* createGenesisBlock() {
    Block* block = (Block*)malloc(sizeof(Block));
    block->index = 0;
    block->timestamp = time(NULL);
    strcpy(block->data, "Genesis Block");
    strcpy(block->previousHash, "0");
    calculateHash(block);
    block->next = NULL;
    return block;
}

// Function to add a block to the blockchain
void addBlock(Blockchain* blockchain, const char* data) {
    Block* newBlock = (Block*)malloc(sizeof(Block));
    newBlock->index = blockchain->length;
    newBlock->timestamp = time(NULL);
    strncpy(newBlock->data, data, MAX_DATA_SIZE - 1);
    newBlock->data[MAX_DATA_SIZE - 1] = '\0';

    Block* lastBlock = blockchain->head;
    while (lastBlock->next != NULL) {
        lastBlock = lastBlock->next;
    }
    strncpy(newBlock->previousHash, lastBlock->hash, SHA256_DIGEST_LENGTH * 2);
    newBlock->previousHash[SHA256_DIGEST_LENGTH * 2] = '\0';

    calculateHash(newBlock);
    newBlock->next = NULL;
    lastBlock->next = newBlock;
    blockchain->length++;
}

// Function to verify the integrity of the blockchain
int isBlockchainValid(Blockchain* blockchain) {
    Block* currentBlock = blockchain->head;
    while (currentBlock != NULL && currentBlock->next != NULL) {
        Block* nextBlock = currentBlock->next;
        calculateHash(currentBlock);

        if (strcmp(currentBlock->hash, nextBlock->previousHash) != 0) {
            return 0;  // Blockchain has been tampered with
        }

        currentBlock = nextBlock;
    }
    return 1;
}

// Function to initialize the blockchain with a genesis block
Blockchain* createBlockchain() {
    Blockchain* blockchain = (Blockchain*)malloc(sizeof(Blockchain));
    blockchain->head = createGenesisBlock();
    blockchain->length = 1;
    return blockchain;
}

// Function to simulate tampering
void tamperBlock(Block* block, const char* newData) {
    strncpy(block->data, newData, MAX_DATA_SIZE - 1);
    block->data[MAX_DATA_SIZE - 1] = '\0';
}

// Function to print the blockchain
void printBlockchain(Blockchain* blockchain) {
    Block* current = blockchain->head;
    while (current != NULL) {
        printf("Block #%d\n", current->index);
        printf("Timestamp: %ld\n", current->timestamp);
        printf("Data: %s\n", current->data);
        printf("Previous Hash: %s\n", current->previousHash);
        printf("Hash: %s\n\n", current->hash);
        current = current->next;
    }
}

int main() {
    // Create blockchain
    Blockchain* blockchain = createBlockchain();

    // Add blocks
    addBlock(blockchain, "File metadata: file1.txt, Size: 1KB, Created: 2024-10-01");
    addBlock(blockchain, "File metadata: file2.txt, Size: 2KB, Created: 2024-10-02");

    // Print blockchain before tampering
    printf("Blockchain before tampering:\n");
    printBlockchain(blockchain);
    printf("Blockchain is %s\n", isBlockchainValid(blockchain) ? "valid" : "invalid");

    // Tamper with a block
    printf("\nTampering with block #1...\n");
    tamperBlock(blockchain->head->next, "Tampered data");

    // Print blockchain after tampering
    printf("\nBlockchain after tampering:\n");
    printBlockchain(blockchain);
    printf("Blockchain is %s\n", isBlockchainValid(blockchain) ? "valid" : "invalid");

    // Free memory
    Block* current = blockchain->head;
    while (current != NULL) {
        Block* next = current->next;
        free(current);
        current = next;
    }
    free(blockchain);

    return 0;
}
