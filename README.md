## Simple Blockchain Implementation in C

This project is a simple blockchain implementation in C that allows you to create and verify a basic blockchain containing metadata about files. Each block in the blockchain stores data, a timestamp, and hashes to ensure data integrity. It uses SHA-256 for cryptographic hashing, leveraging the OpenSSL library to calculate hashes.

### Features

##### Genesis Block Creation: Initializes the blockchain with the first block (genesis block).

##### Adding Blocks: Adds new blocks with file metadata, timestamps, and hash linkage to previous blocks.

##### Blockchain Validation: Checks the integrity of the blockchain by ensuring the hashes match.

##### Tampering Simulation: Allows for block tampering to demonstrate how blockchain validation detects data alteration.


### Prerequisites
C Compiler: This project requires a C compiler, such as gcc.
OpenSSL Library: The project uses OpenSSL for SHA-256 hashing. Install OpenSSL with:

###### sudo apt-get update
###### sudo apt-get install libssl-dev


### Files
blockchain.c: Contains the main code to create, add, verify, and print the blockchain.
Makefile: Used to compile the project.


#### Setup
Clone the repository:


##### git clone <repository-url>
##### cd <repository-directory>

Compile the code using the Makefile:


##### make

This will produce an executable named blockchain.

### Usage
#### Running the Program
To run the blockchain program, use the following command:



###### ./blockchain


### Expected Output

The program performs the following actions and prints output at each step:

Creates a blockchain with a genesis block.
Adds two blocks with sample file metadata.
Prints the blockchain before and after tampering to show its validity.
Tamper simulation: Modifies data in one block and checks blockchain validity again.
Sample output:

plaintext
Copy code
Blockchain before tampering:
Block #0
Timestamp: 1698756954
Data: Genesis Block
Previous Hash: 0
Hash: abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890

Block #1
Timestamp: 1698756955
Data: File metadata: file1.txt, Size: 1KB, Created: 2024-10-01
Previous Hash: abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890
Hash: 1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef

Blockchain is valid

Tampering with block #1...

Blockchain after tampering:
Blockchain is invalid


### Code Explanation
The code performs the following main tasks:

##### Calculate SHA-256 Hash: Each block's data is hashed to generate a unique identifier.

##### Genesis Block: Initializes the blockchain with a starting block.

##### Add New Blocks: Adds subsequent blocks with links to previous blocks.

##### Validation: Checks if all blocks are correctly linked by their hashes.

##### Tampering Detection: Any modification in data will break the hash chain, marking the blockchain as invalid.


### Cleaning Up
To remove the executable, use the following command:



###### make clean