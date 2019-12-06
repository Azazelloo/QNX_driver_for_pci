#include "Headers.h"
void StreamsStop(unsigned* pci_mem,int num_chan)
{
	pci_mem+=0x00000200+0x0000010a;
	*pci_mem=0;

	pci_mem-=0x00000200+0x0000010a;
	//____останов DMA
	pci_mem+=0x00000700;
	pci_mem+=6; //DMA_IRQ_DIS - запрет прерываний по каналу
	*pci_mem=num_chan;

	pci_mem-=4;//DMA_DIS - регистр остановки каналов
	*pci_mem=num_chan;

	pci_mem+=2; //DMA_IRQ
	*pci_mem=num_chan;
}
