#include "Headers.h"
void InitSetDev(unsigned* pci_mem, int num_chan)
{
	pci_mem+=0x00000700+1; //регистр DMA_EN
	if((*pci_mem) & num_chan)
	{
		pci_mem+=1;
		*pci_mem=num_chan;
		pci_mem-=1;
	}

	pci_mem+=5; //регистр DMA_IRQ_DIS
	*pci_mem=num_chan;
	pci_mem-=5;

	pci_mem+=3; //регистр DMA_IRQ
	*pci_mem=num_chan; //записываются 1 в те каналы которые активны
	pci_mem-=3;

	pci_mem-=0x00000700+1; //возвращаемся в начало блока памяти
	pci_mem+=0x00000c00+2; //регистр DMA_CH_CUR_CNTR (сколько слов передано)
	*pci_mem=0;

	pci_mem+=1; //регистр DMA_CH_CUR_POS (номер страницы и смещение внутри страницы для следующей передачи)
	*pci_mem=0;

	pci_mem+=1; //регистр DMA_CH_PC_POS (Указывает номер страницы и смещение внут-ри страницы за последним обработаннымпрограммой ПК словом.)
	*pci_mem=0;

	pci_mem-=2;
	pci_mem-=0x00000c00+2; //возвращаемся в начало блока памяти
	pci_mem+=0x00000700+3; //регистр DMA_RST
	*pci_mem=0x00000003; //сброс 1-ого и 2-ого каналов
	//pci_mem-=0x00000700+3;
}

