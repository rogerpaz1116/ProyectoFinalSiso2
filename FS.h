#include "basic.h"

#define BlockSize 512
#define FsStart 34
#define magic 1837083 
#define totalBlocksDeviceSize 2880
#define totalBlocksFS 34
#define DIRECTORYCOUNT 16
#define IS_DIRECTORY 1
#define IS_FILE 2
#define ROOTBLOCK 35
#define BLOCKSIZE  4096

typedef struct
{ 
	unsigned int magicNumber;
	unsigned int totalBlocks;
	unsigned int totalFreeBlocks; 
	unsigned int firstFreeBlock; 
	unsigned int lastFreeBlock; 
	char wastedSpace[492]; 
}superBlock; 

typedef struct
{ 
	char fileName[20]; 
	unsigned int fileType; 
	unsigned int fileBlockStart; 
	unsigned int fileBlockSize; 
} directoryEntry; 

typedef struct 
{ 
	directoryEntry entries[DIRECTORYCOUNT]; 
}directoryBlock;

 typedef struct 
{ 
	unsigned int nextFreeBlock; 
	char wastedSpace[508]; 
}unusedBlock;

typedef struct 
{
	char fileName[20];
	unsigned int type;
	unsigned int size;//
	char acessRights[3];//xwr
	char data [481];
}fileBlock;

void loadRootDirectory(directoryBlock* directory);
void createEmptyDirectory(directoryBlock directory);
int findFileInDirectory(directoryBlock directory, char * fileName);
int findFileBlockInPath(char* path);
void loadSuperBlock(superBlock *superBlock);
unsigned int AllocateBlock();

void *malloc(int sz);
directoryBlock assignDir(directoryBlock destinationDir, char* fileName, int dirBlockNumber, int pos);

void getDirectoryBlock(directoryBlock directoryBlock1, int pos, int blockDir, char * dirName);
unsigned int createDir(char* filePath,char* fileName);
void listDir(char * filePath);
void renamePathDir(char *dirPath, char *newDirName);
void copyDirAux(int firstBlock, int secondBlock, char *destinationPath);
void copyDir(char *firstPath, char *secondPath, char *dirCopy);
void deleteDirFromBlock(int block);
void removeDir(char * path);


void move(char *firstPath, char *secondPath);
void setFreeBlock(unsigned int blockNumber); 

int fileExist(directoryBlock dirBlock, char *fileName);
void getDirectoryFile(directoryBlock dirDest, int pos, fileBlock file, int blockNumber);
void writeNewFile(int start, fileBlock *file, char d