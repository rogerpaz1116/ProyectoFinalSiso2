#include "FS.h"

void loadRootDirectory(directoryBlock *rootBlock)
{
	printstring("Loading Root Directory \n");
	readsector(ROOTBLOCK,(char*)rootBlock); //16 es el inicio del fs
}

char * getPath(char* pathName, int type)
{

 	int lengthName = len(pathName); 
	char *path = (char *)malloc(lengthName); 
	int tokens = qtytoken(pathName,'-'); 
	int posToken=1; 
	int i=0; 
	char *dirName = (char*)malloc(lengthName);
	
	while (posToken <= tokens-1) 
	{ 
		
		char *token = strtok(pathName, '-', posToken); 
		
		if(posToken == 1)
			path = token;
		if(posToken < tokens-1) path = mystrcat(path, "-"); 
		posToken++; 
	} 

	
	dirName = strtok(pathName, '-', tokens);
	if(type == 1)//retorna path sin la ultima carpeta o archivo
	{
		return path;
	}
	else
		return dirName;

}

/*void createEmptyDirectory(directoryBlock *newDirectory) 
{ 
  int i = 0; 
  while(i<DIRECTORYCOUNT) 
  { 
	newDirectory->entries[i].fileBlockStart = 0; 
	newDirectory->entries[i].fileBlockSize = 0;
	 i++;
  } 
}*/

void createEmptyDirectory(directoryBlock newDirectory)
{
	int i = 0;
	for(i;i<DIRECTORYCOUNT;i++)
	{
		newDirectory.entries[i].fileType = IS_FILE;
		newDirectory.entries[i].fileBlockStart = 0;
		newDirectory.entries[i].fileBlockSize = 0;
	}
}

int findFileInDirectory(directoryBlock directory, char * fileName)
{
	int i;
	for(i=0;i<DIRECTORYCOUNT;i++)
	if(myStrCmp(directory.entries[i].fileName,fileName)==0)
	return i;
	return -1;
}

int findFileBlockInPath(char* path)
{
	int tokens = qtytoken(path,'-'); 
	int posToken=1; 
	int directoryEntryNumber; 
	unsigned int directoryBlockNumber; 
	directoryBlock directory; 
	char *directoryName; 
	directoryName = strtok(path, '-', posToken); 
	posToken = posToken + 1; 
	directoryEntryNumber=0;
	 directoryBlockNumber = ROOTBLOCK; 
	 loadRootDirectory(directory); 
	 printstring("despues del leer el root"); 
	 while (posToken <= tokens) 
	 	{ 

	 		directoryEntryNumber = findFileInDirectory(directory,directoryName); 
	 		
	 		if(directoryEntryNumber==-1) 
	 		{
	 			return -1;
	 		}
	 		directoryBlockNumber = directory.entries[directoryEntryNumber].fileBlockStart; 
	 		readsector( directoryBlockNumber,(char *)&directory); 
	 		directoryName = strtok(path, '-', posToken); 
	 		posToken++; 
	 	}
	 	
	return directoryBlockNumber;

}


void loadSuperBlock(superBlock *spBlock)
{
	readsector(totalBlocksFS,(char*)spBlock);
}



unsigned int AllocateBlock()
{
superBlock spBlock;
unsigned int newFirstBlockNumber;
unsigned int allocatedBlockNumber;
unusedBlock  allocatedBlock;

loadSuperBlock(spBlock);
allocatedBlockNumber = spBlock.firstFreeBlock;
readsector(allocatedBlockNumber,(char*)&allocatedBlock);

newFirstBlockNumber = allocatedBlock.nextFreeBlock;
spBlock.firstFreeBlock = newFirstBlockNumber;
spBlock.totalFreeBlocks--;

writesector(totalBlocksFS,(char*)&spBlock);

printnumber(newFirstBlockNumber);
return allocatedBlockNumber;
}

void setFreeBlock(unsigned int blockNumber) 
{ 
    	superBlock spBlock; 

    	unusedBlock newBlock; 
    	unusedBlock lastFreeBlock; 
    	unsigned int secondToLastBlock;
    	newBlock.nextFreeBlock = 0; 
    	loadSuperBlock(spBlock);
    	readsector(spBlock.lastFreeBlock,(char*)&lastFreeBlock); 
    	lastFreeBlock.nextFreeBlock = blockNumber; 
    	secondToLastBlock =spBlock.lastFreeBlock; 
    	spBlock.lastFreeBlock = blockNumber; 
    	spBlock.totalFreeBlocks++; 
    	writesector(FsStart,(char*)&spBlock); 
    	writesector(secondToLastBlock,(char*)&lastFreeBlock); 
    	writesector(blockNumber,(char*)&newBlock); 
}

directoryBlock assignDir(directoryBlock destinationDir, char* fileName, int dirBlockNumber, int pos){
	
	strcpy(fileName, destinationDir.entries[pos].fileName);
			 destinationDir.entries[pos].fileType=IS_DIRECTORY;
			 destinationDir.entries[pos].fileBlockStart = dirBlockNumber;
			 destinationDir.entries[pos].fileBlockSize = 1;

			 return destinationDir;
}

unsigned int createDir(char * path,char * fileName)
{
	directoryBlock destinationDir, newDirectory;
	unsigned int destination, pos, blockNumber;

	path = getCaracteresValidos(path);
	fileName = getCaracteresValidos(fileName);

	 destination = (strCmp(path,"/")) ? ROOTBLOCK : findFileBlockInPath(path);
	 
	 if(destination!=-1){
	 
	 read_sector(destination,&destinationDir);

	if(!FileExist(destinationDir,fileName)){
	 blockNumber = AllocateBlock();
	 createEmptyDirectory(newDirectory);
	 write_sector(blockNumber,&newDirectory);

	 pos=0;
	 while(pos<DIRECTORYCOUNT)
	 {
		 if(destinationDir.entries[pos].fileBlockStart==0)
		 {
			 assignDir(destinationDir, fileName, blockNumber, pos);
			 break;
		 }
		 pos++;
	 }

	 write_sector(destination,&destinationDir);

	 printstring("Directory successfully created! \n");

	 return blockNumber;
	}

	printstring("Failed: Directory exist.\n");
		return -1;
	 }

	  printstring("Failed: Directory does not exist.");
		 return destination;
}

void listDir(char * filePath)
{
	directoryBlock destinationDir;
	unsigned int blockNumber= findFileBlockInPath(filePath), pos;

	if(blockNumber!=-1)
	{
		read_sector(blockNumber,&destinationDir);

		pos = 0;
		while(pos<DIRECTORYCOUNT)
		{
			if(destinationDir.entries[pos].fileBlockStart!=0)
			{
				char *temp = getCaracteresValidos(destinationDir.entries[pos].fileName);
				if(destinationDir.entries[pos].fileType == IS_FILE)
				{
					printstring("File=>  %s\n",temp);
				}else if(destinationDir.entries[pos].fileType == IS_DIRECTORY)
				{
					printstring("Directory=>  %s\n",temp);
				}
			}
			pos++;
		}
		return;
	}
		 printstring("Failed: Directory does not exist.\n");
}


void renameDir(char *dirPath, char *dirName) 
{ 
	directoryBlock directoryBlock;
	int dirBlockNumber, entryNumber, lengthdirPath;
	char *oldName, *path;
	dirPath = getCaracteresValidos(dirPath);
	dirName = getCaracteresValidos(dirName);

	lengthdirPath=mystrlen(dirPath); 
	*path = (char*)malloc(lengthdirPath);

	path = getPathName(dirPath,1);

	oldName = getPathName(dirPath,2);

	dirBlockNumber = findFileBlockInPath(path); 

	if(dirBlockNumber!=-1){
		read_sector(dirBlockNumber,&directoryBlock); 
		entryNumber = findFileInDirectory(directoryBlock, oldName); 

		if(entryNumber == -1)
		{
			printstring("Failed: Directory does not exist."); 
			return; 
		}

		if(entryNumber!=-1){
		char *tempDirName = malloc(mystrlen(dirPath));
		tempDirName = getCaracteresValidos(directoryBlock.entries[entryNumber].fileName);

		if(strCmp(tempDirName,oldName) == 1) 
		{ 
			int pos = 0;
			while(pos< mystrlen(directoryBlock.entries[entryNumber].fileName))
			{
				directoryBlock.entries[entryNumber].fileName[pos] = ' ';
				pos++;
			}
			strcpy(directoryBlock.entries[entryNumber].fileName, dirName); 
			directoryBlock.entries[entryNumber].fileType = IS_DIRECTORY;
			write_sector(dirBlockNumber,(char*)&directoryBlock); 
		} 
		return;
		}else{
			printstring("Failed: Directory does not exist.");
		}

		return;
	}
	printstring("Failed: Directory does not exist."); 
}

void getDirectoryBlock(directoryBlock directoryBlock1, int pos, int blockDir, char * dirName)
{
	directoryBlock1.entries[pos].fileName[0] = '\0';
	strcpy(dirName, directoryBlock1.entries[pos].fileName);
	directoryBlock1.entries[pos].fileType = IS_DIRECTORY;
	directoryBlock1.entries[pos].fileBlockStart = blockDir;	
	directoryBlock1.entries[pos].fileBlockSize = 1;	
}

void printFileInfo(char fileName[20], unsigned int type, unsigned int size, char accessRight[3], char data[481]){
		printstring("Name: ");
		printstring(fileName);

		printstring(" \nSize File: ");
		printnumber(size);

		printstring(" \nAccess File: ");
		printstring(accessRight);

		printstring(" \nType File: ");
		printnumber(type);

		printstring("\n Data: ");
		printstring(data);
		printstring("\n");
}

int fileExist(directoryBlock dirBlock, char *fileName)
{
	int i = 0;
	for(i; i<DIRECTORYCOUNT; i++)
	{
		if(strCmp(getCaracteresValidos(dirBlock.entries[i].fileName),fileName))
		{
			return 1;
		}
	}
	return 0;	
}


void deleteDirFromBlock(int block) 
{ 
	directoryBlock dir; 
	int i,j; 
	int fileStart; 
	int fileSize; 
	read_sector(block, (char *)&dir); 
	
	if(block!=0){


		for(i=0; i<DIRECTORYCOUNT;i++) 
		{
			if(dir.entries[i].fileType == IS_DIRECTORY) 
			{ 
				deleteDirFromBlock(dir.entries[i].fileBlockStart); 
				setFreeBlock(i); 
			} 
			else if(dir.entries[i].fileType == IS_FILE) 
			{ 
				fileStart = dir.entries[i].fileBlockStart; 
				fileSize = dir.entries[i].fileBlockSize; 
				for(j=fileStart; j<fileSize; j++ ) 
				{ 
					setFreeBlock(j);
				}
				if(i == DIRECTORYCOUNT -1 )
					return;
			} 
		} 
}
}

void removeDir(char * path)
{
	unsigned int j, blockOfDir, blockToRemove;
	int fileStart;
	int fileSize;
	directoryBlock directoryBlock;
	char *blockPath = getPathName(path,1), *dirToRemove = getPathName(path,2);
	blockToRemove = findFileBlockInPath(blockPath);
	readBlock(blockToRemove,(char*)&directoryBlock);
	
	blockOfDir = findFileInDirectory(directoryBlock,dirToRemove);
	
	if(blockOfDir!=-1){
		int i;
		if(directoryBlock.entries[blockOfDir].fileType== IS_DIRECTORY)
		{
			deleteDirFromBlock(directoryBlock.entries[blockOfDir].fileBlockStart);
			setFreeBlock(directoryBlock.entries[blockOfDir].fileBlockStart);
		}
		else if(directoryBlock.entries[blockOfDir].fileType == IS_FILE) 
		{ 
			fileStart = directoryBlock.entries[blockOfDir].fileBlockStart;
			fileSize = directoryBlock.entries[blockOfDir].fileBlockSize; 
			j= fileStart;
			while(j<fileSize) 
			{ 
			 	setFreeBlock(j);
				j++;
			} 
		} 
		for(i=0; i<sizeof(directoryBlock.entries[blockOfDir].fileName); i++)
				directoryBlock.entries[blockOfDir].fileName[i] = '\0';

		directoryBlock.entries[blockOfDir].fileBlockSize = 0;
		directoryBlock.entries[blockOfDir].fileBlockStart = 0;
		write_sector(blockToRemove,(char*)&directoryBlock);
		return;
	}
	printstring("Directory does not exist.\n");
}

void copyDirAux(int firstBlock, int secondBlock, char *destinationPath)
{
	directoryBlock firstDir, secondDir;
	int i, j, start, size, blockDir;
	char *dirName;	

	if(firstBlock != 0){
		read_sector(firstBlock, (char *)&firstDir);
		read_sector(secondBlock, (char *)&secondDir);
		for(i=0; i<DIRECTORYCOUNT;i++)
		{

			if(firstDir.entries[i].fileType == IS_DIRECTORY)
			{
				blockDir = createDir(destinationPath, firstDir.entries[i].fileName);
				dirName = firstDir.entries[i].fileName;

				for(j=0; j<DIRECTORYCOUNT;j++)
				{
					if(secondDir.entries[j].fileBlockStart == 0)
					{
						getDirectoryBlock(secondDir, j, blockDir, dirName);
						break;
					}
				}
				write_sector(secondBlock, (char *)&secondDir);
				copyDirAux(firstDir.entries[i].fileBlockStart, blockDir,dirName);
			}
			else 
			{
				start = firstDir.entries[i].fileBlockStart;
				size = firstDir.entries[i].fileBlockSize;

				if(i == DIRECTORYCOUNT -1 )
					return;
			}
		}
	}

}

void copyDir(char *firstPath, char *secondPath, char *dirCopy)
{
	directoryBlock firstDir, secondDir;
	int firstBlock, secondBlock, blockDir, j;
	char *pathName, *dirName;

	firstPath = getCaracteresValidos(firstPath);
	secondPath = getCaracteresValidos(secondPath);
	dirCopy = getCaracteresValidos(dirCopy);

	firstBlock = findFileBlockInPath(firstPath);
	secondBlock = findFileBlockInPath(secondPath);

	if(firstBlock!=0 && secondBlock!=0)
	{
	pathName = getPathName(firstPath,2);

	read_sector(firstBlock, (char *)&firstDir);
	read_sector(secondBlock, (char *)&secondDir);

	{
		blockDir = createDir(secondPath, dirCopy);
		dirName = (strCmp(getCaracteresValidos(secondPath), "/")) ? getCaracteresValidos(secondPath) : mistrcat(secondPath, "/"), mistrcat(getCaracteresValidos(dirName), dirCopy), getCaracteresValidos(dirName);

		j = 0;
		while(j<DIRECTORYCOUNT)
		{
			if(secondDir.entries[j].fileBlockStart == 0)
			{
				getDirectoryBlock(secondDir, j, blockDir, dirCopy);
				break;
			}
			j++;
		}

		write_sector(secondBlock, (char *)&secondDir);
		copyDirAux(firstBlock, blockDir, dirName);		
	}
	return;
}

}

void move(char *firstPath, char *secondPath)
{
	char *name = malloc(mystrlen(getCaracteresValidos(firstPath)));
	
	name = getPathName(getCaracteresValidos(firstPath),2);

	name = getCaracteresValidos(name);

	copyDir(firstPath, secondPath,name);
	removeDir(firstPath);
}

void writeNewFile(int start, fileBlock *file, char data[])
{
	int dataLenght, aux, blocks, pos, i, j = 0;
	char temp[512];
	dataLenght = mystrlen(data);
	blocks = dataLenght / BLOCKSIZE;
	aux = mod(blocks, BLOCKSIZE);
	pos = 481;
	blocks = (aux == 0) ? blocks : blocks + 1;
	blocks = (blocks == 0) ? 1 : blocks;

	file->size = blocks;

	for(i = 0; i < blocks; i++)
	{
		if (i == 0)
		{
			write_sector(start, (char *)&file);
			start++;
		}
		else
		{
			j = pos;
			for (j; j < pos + 512; j++)
			{
				temp[j] = data[j];
			}

			write_sector(start, temp);
			pos += 512;
			start++;
			
		}
	}
}

void getDirectoryFile(directoryBlock dirDest, int pos, fileBlock file, int blockNumber){
	
	strcpy(dirDest.entries[pos].fileName, file.fileName);
	dirDest.entries[pos].fileType = IS_FILE;
	dirDest.entries[pos].fileBlockStart = blockNumber;
	dirDest.entries[pos].fileBlockSize = file.size;
}

void createFile(char * path, char* fileName, char accessFile[], char *data)
{
	directoryBlock destinationDir;
	int i;
	unsigned int newDirBlockNumber,destinationDirBlockNumber;
	fileBlock file;

	path = getCaracteresValidos(path);
	fileName = getCaracteresValidos(fileName);
	accessFile = getCaracteresValidos(accessFile);
	data = getCaracteresValidos(data);
	

	destinationDirBlockNumber = (strCmp(path, "/")) ? ROOTBLOCK : findFileBlockInPath(path);

	if (destinationDirBlockNumber == -1)
	{
		printstring("Failed: Path incorrect.");
		return;
	}
	readBlock(destinationDirBlockNumber, (char*)&destinationDir);
	
	if(!fileExist(destinationDir, fileName)){
	newDirBlockNumber = AllocateBlock();

	file.size = 0;
	file.type = IS_FILE;
	file.data[0] = '\0';

	strcpy(fileName, file.fileName);
	strcpy(accessFile, file.acessRights);
	strcpy(data, file.data);
	file.type = IS_FILE;
	writeNewFile(&file, data, newDirBlockNumber);
	i = 0;
	while (i<DIRECTORYCOUNT)
	{
		if (destinationDir.entries[i].fileBlockStart == 0)
		{
			getDirectoryFile(destinationDir, i, file, newDirBlockNumber);
			break;
		}
		i++;
	}
	write_sector(destinationDirBlockNumber, (char*)&destinationDir);
	return;
	}
	printstring("Failed: File already exist.\n");
}

void removeFile(char *path, char *fileName)
{
	unsigned int i, j, rmBlock, blockNumber;
	int fileStart, fileSize;
	directoryBlock directoryBlock;
	path = getCaracteresValidos(path);
	fileName = getCaracteresValidos(fileName);
	blockNumber = findFileBlockInPath(fileName);

	if(blockNumber != -1){
	readBlock(blockNumber, (char*)&directoryBlock);
	
	
	rmBlock = findFileInDirectory(directoryBlock, fileName);
	if (rmBlock == -1)
	{
		printstring("File does not exist");
		return;
	}
	if (directoryBlock.entries[rmBlock].fileType == IS_FILE)
	{
		fileStart = directoryBlock.entries[rmBlock].fileBlockStart;
		fileSize = directoryBlock.entries[rmBlock].fileBlockSize;
		for (j = 0; j<fileSize; j++)
		{
			setFreeBlock(fileStart);
			fileStart++;
		}
		for(i=0; i<sizeof(directoryBlock.entries[rmBlock].fileName); i++)
		directoryBlock.entries[rmBlock].fileName[i] = '\0';
		directoryBlock.entries[rmBlock].fileBlockSize = 0;
		directoryBlock.entries[rmBlock].fileBlockStart = 0;
		write_sector(blockNumber, (char*)&directoryBlock);
	}
	return;
	}
	printstring("Directory does not exist");
}

void loadFile(char *path, char *fileName)
{
	unsigned int j, fileBlockNumber, blockNumber;
	int fileStart, fileSize;
	fileBlock file;
	directoryBlock directoryBlock1;
	char *temData;

	path = getCaracteresValidos(path);
	fileName = getCaracteresValidos(fileName);
	
	
	blockNumber = findFileBlockInPath(fileName);

	read_sector(blockNumber, (char*)&directoryBlock1);
	fileBlockNumber = findFileInDirectory(directoryBlock1, fileName);
	j = 0;
	if (blockNumber == -1)
	{
		printstring("El archivo %s no existe", fileName);
	}
	if (directoryBlock1.entries[fileBlockNumber].fileType == IS_FILE)
	{
		fileStart = directoryBlock1.entries[fileBlockNumber].fileBlockStart;
		fileSize = directoryBlock1.entries[fileBlockNumber].fileBlockSize;

		while (j<fileSize)
		{
			if (j == 0)
			{
				read_sector(fileStart, (char *)&file);
				printFileInfo(file.fileName, file.type, file.size, file.acessRights, file.data);
			}
			else
			{
				read_sector(fileStart, temData);
				printstring(temData);
			}
			j++;
			fileStart++;
		}
	}
	else
		printstring("El archivo %s no existe", fileName);
}