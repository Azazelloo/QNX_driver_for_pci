#include "Headers.h"
void StreamsEnable(unsigned* pci_mem,off64_t buf_addr,int addr_size, int interrupt, int num_chan)
{
	pci_mem+=0x00000c00+16; //1-�� ������� 0-�� �������� (������� 32 ���� ������)
	*pci_mem=buf_addr & 0xfffffffc;

	pci_mem+=1; //2-�� ������� 0-�� �������� (������� 32 ���� ������)
	*pci_mem=0;

	pci_mem+=1; //3-�� ������� 0-�� �������� (���-�� 32-� ����)
	*pci_mem=addr_size;

	pci_mem+=1; //4-�� ������� 0-�� �������� (������)
	*pci_mem=0;

	//____���������� ������ ��������
	pci_mem-=3;
	pci_mem-=16;//������� � ������ ����� ������ ������
	pci_mem+=1; //������� ���������� DMA_CH_CMP_CNTR
	*pci_mem=interrupt;

	pci_mem-=1;//������� ������ ������ ������ DMA_CH_CTL
	*pci_mem=0x80010020; //----> �����!

	pci_mem-=0x00000c00;
	pci_mem+=0x00000700+5; //������� DMA_IRQ_EN
	*pci_mem=0x00000002;

	/*while(*(pci_mem+3) & num_chan)
	{
		std::cout<<"Waiting data..."<<std::endl;
	}*/
}
