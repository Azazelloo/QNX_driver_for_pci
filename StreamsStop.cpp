#include "Headers.h"
void StreamsStop(unsigned* pci_mem,int num_chan)
{
	pci_mem+=IO_HARD+GO_SYNC_IO;
	*pci_mem=0;

	pci_mem-=IO_HARD+GO_SYNC_IO;
	//____������� DMA
	pci_mem+=GEN_REGS_DMA;
	pci_mem+=DMA_IRQ_DIS; //DMA_IRQ_DIS - ������ ���������� �� ������
	*pci_mem=num_chan;
	pci_mem-=DMA_IRQ_DIS; //� ������ �����

	pci_mem+=DMA_DIS;//DMA_DIS - ������� ��������� �������
	*pci_mem=num_chan;
	pci_mem-=DMA_DIS;// � ������ �����

	pci_mem+=DMA_IRQ; //DMA_IRQ
	*pci_mem=num_chan;
}
