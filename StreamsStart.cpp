#include "Headers.h"
void StreamsStart(unsigned* pci_mem,int num_chan)
{
	pci_mem+=GEN_REGS_DMA+DMA_EN; //регистр запуска каналов
	*pci_mem=num_chan;

	pci_mem-=GEN_REGS_DMA+DMA_EN;
	//_____запуск синхронного ввода вывода
	pci_mem+=IO_HARD+GO_SYNC_IO; //регистр GO_SYNC_IO
	*pci_mem=0; //предварительный сброс синхронного ввода/вывода
	pci_mem-=GO_SYNC_IO; //в начало блока

	pci_mem+=OUTSWAP_BFCTL; //регистр OUTSWAP_BFCTL
	*pci_mem=1;
	pci_mem-=OUTSWAP_BFCTL; // в начало блока

	pci_mem+=PRELOAD_ADC; // регистр PRELOAD_ADC (записываем 1 дважды)
	*pci_mem=1;
	*pci_mem=1;
	pci_mem-=PRELOAD_ADC; // в начало блока

	delay(5); //задержка перед запуском потока

	pci_mem+=GO_SYNC_IO;//регистр GO_SYNC_IO
	*pci_mem=1;
}

