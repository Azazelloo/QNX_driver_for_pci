#include "Headers.h"
void InitSetDev(unsigned* pci_mem, int num_chan)
{
	pci_mem+=GEN_REGS_DMA+DMA_EN; //������� DMA_EN
	if((*pci_mem) & num_chan) //���� ����� ������� -> ����������
	{
		pci_mem+=1; //�������� � DMA_DIS
		*pci_mem=num_chan;
		pci_mem-=1;
	}
	pci_mem-=DMA_EN; // � ������ �����

	pci_mem+=DMA_IRQ_DIS; //������� DMA_IRQ_DIS
	*pci_mem=num_chan;
	pci_mem-=DMA_IRQ_DIS; // � ������ �����

	pci_mem+=DMA_IRQ; //������� DMA_IRQ
	*pci_mem=num_chan; //������������ 1 � �� ������ ������� �������
	pci_mem-=DMA_IRQ; //� ������ �����

	pci_mem-=GEN_REGS_DMA; //������������ � ������ ����� ������
	pci_mem+=DMA_OUT+DMA_CH_CUR_CNTR; //������� DMA_CH_CUR_CNTR (������� ���� ��������)
	*pci_mem=0;
	pci_mem-=DMA_CH_CUR_CNTR; //� ������ �����

	pci_mem+=DMA_CH_CUR_POS; //������� DMA_CH_CUR_POS (����� �������� � �������� ������ �������� ��� ��������� ��������)
	*pci_mem=0;
	pci_mem-=DMA_CH_CUR_POS; // � ������ �����

	pci_mem+=DMA_CH_PC_POS; //������� DMA_CH_PC_POS (��������� ����� �������� � �������� ����-�� �������� �� ��������� ���������������������� �� ������.)
	*pci_mem=0;
	pci_mem-=DMA_CH_PC_POS; // � ������ �����

	pci_mem-=DMA_OUT; //������������ � ������ ����� ������
	pci_mem+=GEN_REGS_DMA+DMA_RST; //������� DMA_RST
	*pci_mem=0x00000003; //����� 1-��� � 2-��� �������
}

