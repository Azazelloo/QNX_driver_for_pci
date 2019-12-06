#include "Headers.h"
void InitSetDev(unsigned* pci_mem, int num_chan)
{
	pci_mem+=0x00000700+1; //������� DMA_EN
	if((*pci_mem) & num_chan)
	{
		pci_mem+=1;
		*pci_mem=num_chan;
		pci_mem-=1;
	}

	pci_mem+=5; //������� DMA_IRQ_DIS
	*pci_mem=num_chan;
	pci_mem-=5;

	pci_mem+=3; //������� DMA_IRQ
	*pci_mem=num_chan; //������������ 1 � �� ������ ������� �������
	pci_mem-=3;

	pci_mem-=0x00000700+1; //������������ � ������ ����� ������
	pci_mem+=0x00000c00+2; //������� DMA_CH_CUR_CNTR (������� ���� ��������)
	*pci_mem=0;

	pci_mem+=1; //������� DMA_CH_CUR_POS (����� �������� � �������� ������ �������� ��� ��������� ��������)
	*pci_mem=0;

	pci_mem+=1; //������� DMA_CH_PC_POS (��������� ����� �������� � �������� ����-�� �������� �� ��������� ���������������������� �� ������.)
	*pci_mem=0;

	pci_mem-=2;
	pci_mem-=0x00000c00+2; //������������ � ������ ����� ������
	pci_mem+=0x00000700+3; //������� DMA_RST
	*pci_mem=0x00000003; //����� 1-��� � 2-��� �������
	//pci_mem-=0x00000700+3;
}

