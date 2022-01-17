#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "altera_avalon_sem_regs.h"
#include "alt_types.h"
#include "system.h"

#define TIME_STATES 16 

const alt_u32 divisors [TIME_STATES] = {
		10,
		50,
		150,
		250,
		20,
		100,
		300,
		500,
		30,
		120,
		230,
		310,
		450,
		10,
		50,
		100

};

int main()
{ 
	int i,j;
	volatile alt_u32 *p;
	alt_u32 tmp;
	alt_u32 divisor;


	//program divisors
	p = (alt_u32*) SEM_RAM_SLAVE_BASE;
	for (i=0; i<TIME_STATES; i++)
	{
		tmp = 0;
		for (j=TIME_STATES; j>0; j--)
		{
			tmp = divisors[i];
		} 
		*p=tmp;
		p++;
	}
	//since we use pointers (cached data access) to write divisor RAM, 
	//and not direct i/o access with IOWR, we need to flush cache
	alt_dcache_flush();

	//select timeset and run semafor
	IOWR_ALTERA_AVALON_SEM_DIVSET(SEM_CTL_SLAVE_BASE, 0x00);
	IOWR_ALTERA_AVALON_SEM_CTL(SEM_CTL_SLAVE_BASE, 0x01);

	printf("Ready\n");

	while (1)
	{
		;
	}

	return 0;
}
