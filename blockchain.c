#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include <sys/stat.h>

// Define the Block structure
typedef struct Block {
    int index;
    time_t timestamp;
    char data[256]; // Stores file metadata or IP information
    char previousHash[SHA256_DIGEST_LENGTH * 2 + 1];
    char hash[SHA256_DIGEST_LENGTH * 2 + 1];
    struct Block *next;
} Block;

// Define the Blockchain structure
typedef struct Blockchain {
    Block *head;
    int size;
} Blockchain;

// Function to calculate SHA-256 hash
void calculateHash(Block *block, char *hashOutput) {
    char input[512];
    sprintf(input, "%d%ld%s%s", block->index, block->timestamp, block->data, block->previousHash);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)input, strlen(input), hash);

    // Convert hash to a hexadecimal string
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hashOutput + (i * 2), "%02x", hash[i]);
    }
    hashOutput[SHA256_DIGEST_LENGTH * 2] = '\0';
}

// Create the genesis block
Block *createGenesisBlock() {
    Block *block = (Block *)malloc(sizeof(Block));
    block->index = 0;
    block->timestamp = time(NULL);
    strcpy(block->data, "Genesis Block");
    strcpy(block->previousHash, "0");
    calculateHash(block, block->hash);
    block->next = NULL;
    return block;
}

// Add a new block to the blockchain
void addBlock(Blockchain *blockchain, const char *data) {
    Block *newBlock = (Block *)malloc(sizeof(Block));
    Block *lastBlock = blockchain->head;

    // Traverse to the last block
    while (lastBlock->next != NULL) {
        lastBlock = lastBlock->next;
    }

    newBlock->index = lastBlock->index + 1;
    newBlock->timestamp = time(NULL);
    strcpy(newBlock->data, data);
    strcpy(newBlock->previousHash, lastBlock->hash);
    calculateHash(newBlock, newBlock->hash);
    newBlock->next = NULL;

    lastBlock->next = newBlock;
    blockchain->size++;
}

// Extract file metadata (name, size, creation date)
void extractFileMetadata(const char *filename, char *metadata) {
    struct stat fileStat;
    if (stat(filename, &fileStat) != 0) {
        perror("Error reading file metadata");
        exit(EXIT_FAILURE);
    }

    sprintf(metadata, "File: %s, Size: %ld bytes, Created: %ld", filename, fileStat.st_size, fileStat.st_ctime);
}

// Validate the blockchain
int isBlockchainValid(Blockchain *blockchain) {
    Block *currentBlock = blockchain->head;
    while (currentBlock->next != NULL) {
        Block *nextBlock = currentBlock->next;
        char recalculatedHash[SHA256_DIGEST_LENGTH * 2 + 1];
        calculateHash(currentBlock, recalculatedHash);

        if (strcmp(currentBlock->hash, recalculatedHash) != 0) {
            return 0; // Tampering detected
        }
        if (strcmp(nextBlock->previousHash, currentBlock->hash) != 0) {
            return 0; // Chain broken
        }
        currentBlock = nextBlock;
    }
    return 1; // Blockchain is valid
}

// Print the blockchain
void printBlockchain(Blockchain *blockchain) {
    Block *currentBlock = blockchain->head;
    while (currentBlock != NULL) {
        printf("Index: %d\n", currentBlock->index);
        printf("Timestamp: %ld\n", currentBlock->timestamp);
        printf("Data: %s\n", currentBlock->data);
        printf("Previous Hash: %s\n", currentBlock->previousHash);
        printf("Hash: %s\n", currentBlock->hash);
        printf("-------------------------\n");
        currentBlock = currentBlock->next;
    }
}

int main() {
    // Initialize the blockchain
    Blockchain blockchain;
    blockchain.head = createGenesisBlock();
    blockchain.size = 1;

    // Add files to the blockchain
    char metadata[256];
    extractFileMetadata("file1.txt", metadata);
    addBlock(&blockchain, metadata);

    extractFileMetadata("file2.txt", metadata);
    addBlock(&blockchain, metadata);

    // Print the blockchain
    printf("Blockchain before tampering:\n");
    printBlockchain(&blockchain);

    // Check blockchain validity
    if (isBlockchainValid(&blockchain)) {
        printf("Blockchain is valid.\n");
    } else {
        printf("Blockchain is invalid.\n");
    }

    // Tampering simulation
    printf("\nTampering with data in Block 1...\n");
    strcpy(blockchain.head->next->data, "Tampered Data!");

    // Print blockchain after tampering
    printf("Blockchain after tampering:\n");
    printBlockchain(&blockchain);

    // Revalidate blockchain
    if (isBlockchainValid(&blockchain)) {
        printf("Blockchain is valid.\n");
    } else {
        printf("Blockchain is invalid.\n");
    }

    return 0;
}
