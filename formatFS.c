#include "stdio.h"
#include "formatFS.h"

DirectoryBlock createEmptyDirectory()
{
	DirectoryBlock newDirectory;
	int i;
	for(i=0;i<DIRECTORYENTRYCOUNT;i++)
	{
		newDirectory.entries[i].fileBlockStart = 0;
		newDirectory.entries[i].fileBlockSize = 0;
	}
	return newDirectory;
}

void writeBlock(FILE * file, unsigned int blockNumber, void * blockData)
{
	
	fseek(file,blockNumber*BLOCKSIZE,SEEK_SET);
	fwrite(blockData,1,BLOCKSIZE,file);
}


void format(char * device)
{
	FILE *floppy;
	unsigned int totalBlocks,FsBegin, fsBlockSize, index; 
	int blockNumber;
	UnusedBlock lastFreeBlock;
	DirectoryBlock rootDirectory;
	SuperBlock superBlock;
	
	floppy = fopen(device, "rw+");
	if(floppy==0){
		printf("floppy.img not found\n");
		return;
	}

	fseek(floppy, 0, SEEK_END);
	totalBlocks = ftell(floppy)/BLOCKSIZE;

	fseek(floppy, BLOCKSIZE*FsStart,SEEK_SET);

	FsBegin = ftell(floppy)/BLOCKSIZE;

	fsBlockSize = totalBlocks - FsBegin;
	index = FsBegin - 1;

	
	superBlock.magicNumber = MAGICNUMBER;
	superBlock.totalBlocksInDisk = totalBlocks; 
	superBlock.totalFreeBlocks = superBlock.totalBlocksInDisk  - 3;
	superBlock.firstFreeBlock = 3;
	superBlock.lastFreeBlock = superBlock.totalBlocksInDisk -1;
	
	
	for(blockNumber = superBlock.firstFreeBlock; blockNumber < superBlock.lastFreeBlock;blockNumber++)
	{
		UnusedBlock freeBlock;
		freeBlock.nextFreeBlock = blockNumber + 1;
		writeBlock(floppy, blockNumber, &freeBlock);
	}
	
	lastFreeBlock.nextFreeBlock=0;
	writeBlock(floppy, superBlock.lastFreeBlock, &lastFreeBlock);
	
	
	rootDirectory = createEmptyDirectory();
    writeBlock(floppy, ROOTBLOCK, &rootDirectory);
    
	writeBlock(floppy, SUPERBLOCK,&superBlock);
}


int main()
{
	char * floppyImg = "floppya.img";
format(floppyImg);
}
