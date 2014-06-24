
void readsector(int, char*);
void writesector(int, char*);
void putchar(char);
char getchar();
void printstring(char*);
void printnumber(int);
void readstring(char*);
void readfile(char*, char*);
void writefile(char*, char*, int);
void deletefile(char*);
void exit();
void executeprogram(char*, int,char*);
void allow_preemption();
int mod(int,int);
int mydiv(int,int);
void setvideo(int);
void setpixel(int,int,int);
void clearscreen();
void setcursor(int,int);
void setchar(char,char,int,int);
void setstring(char*,char,int,int);
void getnumberstring(char*,int);

#define BlockSize 512
#define FsStart 34
#define magic 1837083
#define totalBlocksDeviceSize 2880
#define totalBlocksFS 34
#define DIRECTORYCOUNT 16
#define IS_DIRECTORY 1
#define IS_FILE 2
#define ROOTBLOCK 35
#define BLOCKSIZE  512

//funciones de utilerias
int caracterValido(char caracter);

char *getCaracteresValidos(char *cadena);

char * getPathName(char * pathName, int type);

int mystrlen(char *cadena);

char * mistrcat(char * destino,char * fuente);

char * strcpy( char *to,char* from);

int strCmp (char *s1, char *s2);

int stlen(char * cadena, int start, char delim);

int qtytoken(char * token, char delim);

char *mystrtok(char * str, char delim, int pos);

char * strtok(char * str, char delim, int pos);

void *malloc(int sz);

//funciones del FS


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
void loadSuperBlock(superBlock *superBlock);
void createEmptyDirectory(directoryBlock directory);
int findFileInDirectory(directoryBlock directory, char * fileName);
int findFileBlockInPath(char* path);
unsigned int AllocateBlock();

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
void writeAFile(int start, fileBlock *file, char data[]);
void rmFile(char *path, char *fileName);

void ps();
