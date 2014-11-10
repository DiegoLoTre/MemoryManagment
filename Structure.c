#include <assert.h>                      /* Used for the assert macro */
#include <stdlib.h>                      /* Used for EXIT_FAILURE */

unsigned int Translate(unsigned int vAddr, unsigned int offset, int PAGE_TABLE_SIZE)
{
	unsigned int pageNumber;

	/* Check if the page number makes sense */
#ifdef DEBUG
	assert(offset);
#else
	if(offset <= 0)
	{
		ErrorMsg("Translate", "the offset es menor a 0");
		exit (EXIT_FAILURE);
	}
#endif

	/* Make surethe address is valid */
	pageNumber = vAddr >> offset;

#ifdef DEBUG
	assert(pageNumber < PAGE_TABLE_SIZE);
#else
	if(pageNumber >= PAGE_TABLE_SIZE)
	{
		ErrorMsg("Translate", "the page number is out of bounds");
		exit (EXIT_FAILURE);
	}
#endif

	return (pageNumber);
}