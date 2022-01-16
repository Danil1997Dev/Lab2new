#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "altera_avalon_sem_regs.h"
#include "alt_types.h"
#include "system.h"

#define TIME_STATES 16
//#define TIME_STATES	4
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
		//nanosleep(20680);
		*p=tmp;
		p++;
	}
	//since we use pointers (cached data access) to write divisor RAM, 
	//and not direct i/o access with IOWR, we need to flush cache
	alt_dcache_flush();

	printf("Ready load\n");
	
	//IOWR_ALTERA_AVALON_SEM_CTL(SEM_CTL_SLAVE_BASE,0x01);//данная строка указана в закоментированном коде в строке 48
	//printf("Transmit CTL 0x01\n");

	// вариант подачи сигнала выбора набора времен через функцию usleep
	//select timeset and run semafor
	IOWR_ALTERA_AVALON_SEM_CTL(SEM_CTL_SLAVE_BASE,0x01);
    printf("Transmit CTL 0x01\n");
	IOWR_ALTERA_AVALON_SEM_DIVSET(SEM_CTL_SLAVE_BASE,0x00);
	printf("Transmit 0x00\n");
	usleep(1);
	IOWR_ALTERA_AVALON_SEM_DIVSET(SEM_CTL_SLAVE_BASE,0x01);
	printf("Transmit 0x01\n");
	usleep(1);
	IOWR_ALTERA_AVALON_SEM_DIVSET(SEM_CTL_SLAVE_BASE,0x02);
	printf("Transmit 0x02\n");
	usleep(1);
    IOWR_ALTERA_AVALON_SEM_DIVSET(SEM_CTL_SLAVE_BASE,0x03);
    printf("Transmit 0x03\n");

	while (1)
	{
		// вариант подачи сигнала выбора набора времен через цикл for
		/*for (int m=0; m<4; m++){
		for (double k=0; k<(ALT_CPU_CPU_FREQ/(500000*ALT_CPU_CPU_FREQ)); k+=0.000001); // небольшая задержка
		divisor = (char)m;
		IOWR_ALTERA_AVALON_SEM_DIVSET(SEM_CTL_SLAVE_BASE,divisor);
		printf("divisor = %x\n",divisor);//переданное значение делителя
		};
		printf("Ready\n");*/
	}
	
	return 0;
}
