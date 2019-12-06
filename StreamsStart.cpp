#include "Headers.h"
void StreamsStart(unsigned* pci_mem,int num_chan)
{
	pci_mem+=0x00000700+1; //������� ������� �������
	*pci_mem=num_chan;

	pci_mem-=0x00000700+1;
	//_____������ ����������� ����� ������
	pci_mem+=0x00000200+0x0000010a; //������� GO_SYNC_IO
	*pci_mem=0; //��������������� ����� ����������� �����/������

	pci_mem-=0x0000010a;
	pci_mem+=+0x00000118; //������� OUTSWAP_BFCTL
	*pci_mem=1;

	pci_mem-=0x00000118;
	pci_mem+=0x0000010c; // ������� PRELOAD_ADC (���������� 1 ������)
	*pci_mem=1;
	*pci_mem=1;

	delay(5); //�������� ����� �������� ������
	pci_mem-=0x0000010c;
	pci_mem+=0x0000010a;//������� GO_SYNC_IO
	*pci_mem=1;
}

