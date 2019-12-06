#include "Headers.h"
void StreamsStart(unsigned* pci_mem,int num_chan)
{
	pci_mem+=0x00000700+1; //регистр запуска каналов
	*pci_mem=num_chan;

	pci_mem-=0x00000700+1;
	//_____запуск синхронного ввода вывода
	pci_mem+=0x00000200+0x0000010a; //регистр GO_SYNC_IO
	*pci_mem=0; //предварительный сброс синхронного ввода/вывода

	pci_mem-=0x0000010a;
	pci_mem+=+0x00000118; //регистр OUTSWAP_BFCTL
	*pci_mem=1;

	pci_mem-=0x00000118;
	pci_mem+=0x0000010c; // регистр PRELOAD_ADC (записываем 1 дважды)
	*pci_mem=1;
	*pci_mem=1;

	delay(5); //задержка перед запуском потока
	pci_mem-=0x0000010c;
	pci_mem+=0x0000010a;//регистр GO_SYNC_IO
	*pci_mem=1;
}

