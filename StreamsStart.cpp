#include "Headers.h"
void StreamsStart(unsigned* pci_mem,int num_chan)
{
	pci_mem+=GEN_REGS_DMA+DMA_EN; //������� ������� �������
	*pci_mem=num_chan;

	pci_mem-=GEN_REGS_DMA+DMA_EN;
	//_____������ ����������� ����� ������
	pci_mem+=IO_HARD+GO_SYNC_IO; //������� GO_SYNC_IO
	*pci_mem=0; //��������������� ����� ����������� �����/������
	pci_mem-=GO_SYNC_IO; //� ������ �����

	pci_mem+=OUTSWAP_BFCTL; //������� OUTSWAP_BFCTL
	*pci_mem=1;
	pci_mem-=OUTSWAP_BFCTL; // � ������ �����

	pci_mem+=PRELOAD_ADC; // ������� PRELOAD_ADC (���������� 1 ������)
	*pci_mem=1;
	*pci_mem=1;
	pci_mem-=PRELOAD_ADC; // � ������ �����

	delay(5); //�������� ����� �������� ������

	pci_mem+=GO_SYNC_IO;//������� GO_SYNC_IO
	*pci_mem=1;
}

