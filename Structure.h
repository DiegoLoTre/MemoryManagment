unsigned int Translate (unsigned int vAddr, unsigned int offset, int PAGE_TABLE_SIZE);
enum ACCESS
{
	READ,
	WRITE
};
enum STATUS
{
	INVALID,
	VALID,
	DIRT,
	CLEAN
};

struct reference
{
	unsigned int address;
	long int frame;
	int lastUse;
	int access;
	int status;
};

typedef struct reference reference;