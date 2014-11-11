unsigned int Translate (unsigned int vAddr, unsigned int offset, int PAGE_TABLE_SIZE);
enum ACCESS
{
	READ,
	WRITE
};

struct reference
{
	unsigned int address;
	long int frame;
	int lastUse;
	int access;
	char status[2];
};

typedef struct reference reference;