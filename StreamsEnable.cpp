#include "Headers.h"
void StreamsEnable(unsigned* pci_mem,off64_t buf_addr,int addr_size, int interrupt, int num_chan)
{
	pci_mem+=DMA_OUT+16; //1-ый регистр 0-ой страницы (младшие 32 бита адреса)
	*pci_mem=buf_addr & 0xfffffffc; // выравниваем (последние два бита равны 0)

	pci_mem+=1; //2-ой регистр 0-ой страницы (старшие 32 бита адреса)
	*pci_mem=0;

	pci_mem+=1; //3-ий регистр 0-ой страницы (кол-во 32-х слов)
	*pci_mem=addr_size;

	pci_mem+=1; //4-ый регистр 0-ой страницы (резерв)
	*pci_mem=0;

	//____Выставляем нужные регистры
	pci_mem-=3;
	pci_mem-=16;//возврат в начало блока памяти канала
	pci_mem+=DMA_CH_CMP_CNTR; //регистр прерываний DMA_CH_CMP_CNTR
	*pci_mem=interrupt;

	pci_mem-=DMA_CH_CMP_CNTR;// в начало блока (регистр режима работы канала DMA_CH_CTL)
	*pci_mem=0x80010020; //----> ВАЖНО! 0x80------ (одиночный запуск) 0x00------ (циклический запуск)

	pci_mem-=DMA_OUT;
	pci_mem+=GEN_REGS_DMA+DMA_IRQ_EN; //регистр DMA_IRQ_EN
	*pci_mem=0x00000002;

	/*while(*(pci_mem+3) & num_chan)
	{
		std::cout<<"Waiting data..."<<std::endl;
	}*/
}
