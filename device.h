	#define BLOCKSIZE  4096

	Device(string filePath);
	~Device(void);
	void createDevice(unsigned int sizeInMb);
	void readBlock(unsigned int blockNumber,void * blockData);
	void writeBlock(unsigned int blockNumber,void * blockData);
	unsigned int getTotalBlocks();