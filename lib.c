/*lib.c
Michael Black, 2007
This file contains the various library functions called by the shell and 
user applications
*/

/*setup enables multitasking.  it should be called at the beginning of 
any user program*/
#include "lib.h"



void memcpy(){}


void setup()
{
	seti();
}


/*computes a mod b*/
int mod(int a, int b)
{
        while(a>=b)
                a=a-b;
        return a;
}


/*computes a / b using integer mydivision*/
int mydiv(int a, int b)  
{               
        int q=0;
        while (q*b<=a)
                q++;
        return q-1;
}               


/*reads a sector into the buffer*/
void readsector(int sectornumber, char* buffer)
{
        int sec,head,cyl;

	/*convert to CHS*/
        sec=mod(sectornumber,0x12)+1;
        head=mod(mydiv(sectornumber,0x12),2);
        cyl=mydiv(sectornumber,0x24);

        readsect(buffer,sec,head,cyl);
}


/*writes buffer to a sector*/
void writesector(int sectornumber, char* buffer)
{
        int sec,head,cyl;

	/*convert to CHS*/
        sec=mod(sectornumber,0x12)+1;
        head=mod(mydiv(sectornumber,0x12),2);
        cyl=mydiv(sectornumber,0x24);

        writesect(buffer,sec,head,cyl);
}



/*prints a character*/
void putchar(char c)
{
	printc(c);
}



/*reads a character*/
char getchar()
{
	return (char)readc();
}

/*prints a string terminated with 0*/
void printstring(char* string)
{
	int21(1,string);
}

/*prints an integer*/
void printnumber(int number)
{
	char num[12];
	char pnum[12];
	int d=1;
	int i=0;
	int j;

	/*convert the number to a string digit by digit*/
	while(i<10)
	{
		num[i]=mod(mydiv(number,d),10)+0x30;
		i++;
		d=d*10;
		if (mydiv(number,d)==0)
			break;
	}

	/*reverse it to read most significant to least significant*/
	j=0;
	for (d=i-1; d>=0; d--)
		pnum[j++]=num[d];
	pnum[j]=0;
	printstring(pnum);
}

/*read a line from the keyboard and put it in buffer*/
void readstring(char* buffer)
{
	int21(2,buffer);
}

/*read the file name[] into buffer[]*/
void readfile(char* name, char* buffer)
{
	int21(3,name,buffer);
}

/*write buffer[] to a file called name[]*/
void writefile(char* name, char* buffer, int sectorlength)
{
	int21(4,name,buffer,sectorlength);
}

/*delete the file name[]*/
void deletefile(char* name)
{
	int21(5,name);
}

/*execute the program*/
void executeprogram(char* buffer, int bytelength)
{
	int21(8,buffer,bytelength);
}

/*execute the program, but don't make the caller wait*/
void executeprogrambackground(char* buffer, int bytelength)
{
	int21(6,buffer,bytelength);
}

/*terminate program - this must be called by the program before ending*/
void exit()
{
	int21(7);
}

/*sets the video to 1 - graphics, 0 - text*/
void setvideo(int mode)
{
	if (mode==0)
		setvideotext();
	else
		setvideographics();
}

/*sets the pixel at columnxrow to color*/
void setpixel(int color, int column, int row)
{
	drawpixel(color,row*320+column);
}

/*sets the cursor position to row, column*/
void setcursor(int row, int column)
{
	interrupt(0x10,2*256,0,0,row*256+column);
}

/*clear the screen*/
void clearscreen()
{
        int r,c;
        for (c=0; c<80; c++)
                for (r=0; r<25; r++)
                {
                        putInMemory(0xB800,(r*80+c)*2,0x20);
                        putInMemory(0xB800,(r*80+c)*2+1,7);
                }
}

/*prints a character at a certain cursor position*/
void setchar(char c, char color, int row, int column)
{
	putInMemory(0xB800,(row*80+column)*2,c);
	putInMemory(0xB800,(row*80+column)*2+1,color);
}

/*prints a string at a certain cursor position*/
void setstring(char* c, char color, int row, int column)
{
	int i=0;
	while(c[i]!=0x0)
	{
		putInMemory(0xB800,(row*80+column+i)*2,c[i]);
		putInMemory(0xB800,(row*80+column+i)*2+1,color);
		i++;
	}
}

void ps()
{
    int21(17);
}


void *malloc(int sz) 
{

 int next_index = 0;
 unsigned char our_memory[1024 * 1024];  
    void *mem;

if(sizeof our_memory - next_index < sz)
  return '\0';

  mem = &our_memory[next_index];
  next_index += sz;
  return mem; 
}

int caracterValido(char caracter){
    int codigo = (int) caracter;
    return (codigo>=30 && codigo <=39 || codigo>=41 && codigo<=90 || codigo>=97 && codigo<=122);
    
}

char *getCaracteresValidos(char *cadena)
{
    char *temp = malloc(mystrlen(cadena));
    int i=0;
    for(i=0; i<mystrlen(cadena); i++)
    {
        temp[i] = (caracterValido(cadena[i])) ? cadena[i] : '\0';
    }
    temp[i] = '\0';
    return temp;
}

char * getPathName(char * pathName, int type)
{
    int lengthName =  mystrlen(pathName); 
    char *path = malloc(lengthName);
    int tokens = qtytoken(pathName,'/'); 
    int posToken=2; 
    int i=0; 
    char *dirName = malloc(lengthName);
    if(tokens == 1 && type == 1)
    {
        path[0] = pathName[0];
        path[1] = '\0';
    }
    if(tokens == 2)
    {
        path[0] = '/';
        path =getCaracteresValidos(path);
    }
    while (posToken <= tokens-1) 
    { 
        char *token = mystrtok(pathName, '/', posToken); 
        
        if(posToken == 2)
            path = getCaracteresValidos(mistrcat("/",token));
        else
            path = getCaracteresValidos(mistrcat(path,token)); 

        if(posToken < tokens-1) 
        {
            path = getCaracteresValidos(path);
            path = mistrcat(path, "/"); 
            path = getCaracteresValidos(path);
        }
        posToken++; 
    } 
    
    if(type == 1)
    {
        return path;
    }
    else
    {
        dirName = mystrtok(pathName, '/', tokens);
        dirName = getCaracteresValidos(dirName);
        return dirName;
    }

}   

int mystrlen(char *cadena){
    int i = 0;

    while(cadena[i] !=  '\0'){
        i++;
    }
    return i;
}

char * mistrcat(char * destino,char * fuente)
{
 int i=0, j=0, pos = 0, strlen = mystrlen(destino) + mystrlen(fuente);

 char * temp = malloc(strlen);
 
    while(destino[i])
    {
         temp[i] = destino[i];
         i++;
    }
  
 while(fuente[j])
 {
  temp[i]=fuente[j];
  i++;
  j++;
 }
 
 destino = malloc(strlen);
  for(pos;pos<strlen;pos++)
  destino[pos] = temp[pos];
 return destino;
}

char * strcpy( char *to,char* from)
{
    int i=0;
    while (to[i]!='\0')
    {
       from[i]=to[i];
       i++;
    }
  return from;
}

int strCmp (char *s1, char *s2)
{
     unsigned char *p1 = (unsigned char *)s1;
     unsigned char *p2 = (unsigned char *)s2;

    while (*p1 != '\0') {
        if (*p2 == '\0') return  0;
        if (*p2 > *p1)   return 0;
        if (*p1 > *p2)   return  0;

        p1++;
        p2++;
    }

    if (*p2 != '\0') return 0;

    return 1;
}

int stlen(char * cadena, int start, char delim)
{
    int count = 0, pos = 0;
    
    while(cadena[pos]!='\0' && cadena[pos]!=delim){
        count++;
        pos++;
    }

    return count;
}

int qtytoken(char * token, char delim)
{
    int totalTokens = 1;
    int pos = 0;

    while(token[pos]!='\0')
    {
        if(token[pos]==delim){
        totalTokens++;
        }
        pos++;
    }
    return totalTokens;
}

char *mystrtok(char * str, char delim, int pos)
{
    
    int totalTokens = qtytoken(str,delim), count = 1, posStr = 0;
    int strlen = stlen(str, posStr, delim);
    char * token = malloc(strlen);
    int posToken = 0;
    if(strlen == 1)
        return str;

    
    while(str[posStr]!='\0')
    {
        if(str[posStr]==delim)
        {
            count++;
        
            if(count<pos)token = malloc(strlen);
        }
        else if(count==pos){
            token[posToken] = str[posStr];
            posToken++;
        }
        posStr++;
    }   
    return token;
}


char * strtok(char * str, char delim, int pos)
{
    
    int totalTokens = qtytoken(str,delim), count = 1, posStr = 0, posToken = 0;
    int strlen = stlen(str, posStr, delim);

    char * token;

    if(strlen==0)
        return '\0';
    
    token = malloc(strlen);

while(str[posStr]!='\0')
    {
        if(str[posStr]==delim)
        {
            int strlen = stlen(str, posStr, delim);
            count++;
         
            if(count<pos)token = malloc(strlen);
        }
        else if(count==pos){
            token[posToken] = str[posStr];
            posToken++;
        }
        posStr++;
    }   
    return token;
}




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

void loadRootDirectory(directoryBlock *rootBlock)
{
	printstring("Loading Root Directory \n");
	readsector(ROOTBLOCK,(char*)rootBlock); 
}

void loadSuperBlock(superBlock *spBlock)
{
	readsector(totalBlocksFS,(char*)spBlock);
}

int findFileInDirectory(directoryBlock directory, char * fileName)
{
	int i;
	for(i=0;i<DIRECTORYCOUNT;i++)
	if(strCmp(directory.entries[i].fileName,fileName)==0)
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
	 
	 readsector(destination,&destinationDir);

	if(!fileExist(destinationDir,fileName)){
	 blockNumber = AllocateBlock();
	 createEmptyDirectory(newDirectory);
	 writesector(blockNumber,&newDirectory);

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

	 writesector(destination,&destinationDir);

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
		readsector(blockNumber,&destinationDir);

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
		readsector(dirBlockNumber,&directoryBlock); 
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
			writesector(dirBlockNumber,(char*)&directoryBlock); 
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
	readsector(block, (char *)&dir); 
	
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
	readsector(blockToRemove,(char*)&directoryBlock);
	
	blockOfDir = findFileInDirectory(directoryBlock,dirToRemove);
	
	if(blockOfDir!=-1){
		int i;
		
		if(directoryBlock.entries[blockOfDir].fileType == IS_FILE) 
		{ 
			fileStart = directoryBlock.entries[blockOfDir].fileBlockStart;
			fileSize = directoryBlock.entries[blockOfDir].fileBlockSize; 
			j= fileStart;
			while(j<fileSize) 
			{ 
			 	setFreeBlock(j);
				j++;
			} 
		}else if(directoryBlock.entries[blockOfDir].fileType== IS_DIRECTORY)
		{
			deleteDirFromBlock(directoryBlock.entries[blockOfDir].fileBlockStart);
			setFreeBlock(directoryBlock.entries[blockOfDir].fileBlockStart);
		} 
		for(i=0; i<sizeof(directoryBlock.entries[blockOfDir].fileName); i++)
				directoryBlock.entries[blockOfDir].fileName[i] = '\0';

		directoryBlock.entries[blockOfDir].fileBlockSize = 0;
		directoryBlock.entries[blockOfDir].fileBlockStart = 0;
		writesector(blockToRemove,(char*)&directoryBlock);
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
		readsector(firstBlock, (char *)&firstDir);
		readsector(secondBlock, (char *)&secondDir);
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
				writesector(secondBlock, (char *)&secondDir);
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

	dirCopy = getCaracteresValidos(dirCopy);
	firstPath = getCaracteresValidos(firstPath);
	firstBlock = findFileBlockInPath(firstPath);
	secondPath = getCaracteresValidos(secondPath);
	secondBlock = findFileBlockInPath(secondPath);

	if(firstBlock!=0 && secondBlock!=0)
	{
	pathName = getPathName(firstPath,2);
	readsector(firstBlock, (char *)&firstDir);
	readsector(secondBlock, (char *)&secondDir);

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

		writesector(secondBlock, (char *)&secondDir);
		copyDirAux(firstBlock, blockDir, dirName);		
	}
	return;
}

}

void move(char *firstPath, char *secondPath)
{
	char *objectName = malloc(mystrlen(getCaracteresValidos(firstPath)));
	
	objectName = getPathName(getCaracteresValidos(firstPath),2);

	objectName = getCaracteresValidos(objectName);
	copyDir(firstPath, secondPath,objectName);
	removeDir(firstPath);
}

void writeAFile(int start, fileBlock *file, char data[])
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
			writesector(start, (char *)&file);
			start++;
		}
		else
		{
			j = pos;
			for (j; j < pos + 512; j++)
			{
				temp[j] = data[j];
			}

			writesector(start, temp);
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
	readsector(destinationDirBlockNumber, (char*)&destinationDir);
	
	if(!fileExist(destinationDir, fileName)){
	newDirBlockNumber = AllocateBlock();

	file.size = 0;
	file.type = IS_FILE;
	file.data[0] = '\0';

	strcpy(fileName, file.fileName);
	strcpy(accessFile, file.acessRights);
	strcpy(data, file.data);
	file.type = IS_FILE;
	writeAFile(&file, data, newDirBlockNumber);
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
	writesector(destinationDirBlockNumber, (char*)&destinationDir);
	return;
	}
	printstring("Failed: File already exist.\n");
}

void rmFile(char *path, char *fileName)
{
	unsigned int i, j, rmBlock, blockNumber;
	int fileStart, fileSize;
	directoryBlock directoryBlock;
	path = getCaracteresValidos(path);
	fileName = getCaracteresValidos(fileName);
	blockNumber = findFileBlockInPath(fileName);

	if(blockNumber != -1){
	readsector(blockNumber, (char*)&directoryBlock);
	
	
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
		writesector(blockNumber, (char*)&directoryBlock);
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

	readsector(blockNumber, (char*)&directoryBlock1);
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
				readsector(fileStart, (char *)&file);
				printFileInfo(file.fileName, file.type, file.size, file.acessRights, file.data);
			}
			else
			{
				readsector(fileStart, temData);
				printstring(temData);
			}
			j++;
			fileStart++;
		}
	}
	else
		printstring("El archivo %s no existe", fileName);
}