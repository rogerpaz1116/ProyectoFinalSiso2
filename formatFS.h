#define BLOCKSIZE 512
#define MAGICNUMBER 1837083
#define DIRECTORYENTRYCOUNT 16
#define ISDIRECRY 1
#define ISFILE 2
#define ROOTBLOCK 35
#define SUPERBLOCK 34
#define FsStart 15

typedef struct{
	int magicNumber;
	unsigned int totalBlocksInDisk;
	unsigned int totalFreeBlocks;
	unsigned int firstFreeBlock;
	unsigned int lastFreeBlock;
	char wastedSpace[492];
}SuperBlock;


typedef struct
{
	unsigned int nextFreeBlock;
	char wastedSpace[492];
}UnusedBlock;

typedef struct
{
	char  fileName[20];
	char fileType;
	//unsigned int fileBlock;
	unsigned int fileBlockStart;
	unsigned int fileBlockSize;

}DirectoryEntry;

typedef struct 
{
	DirectoryEntry entries[16];
}DirectoryBlock;


DirectoryBlock createEmptyDirectory();

void format();
void writeBlock(FILE * file, unsigned int blockNumber,void * blockData);