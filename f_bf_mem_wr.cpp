#include "Headers.h"

int f_bf_mem_wr(unsigned* pci_mem,uint32_t addr, const uint32_t* regs, uint32_t size)
{
	int err=0;
	pci_mem+=2;// BF_STATUS
	while(*pci_mem & 0x00000002) //������� ���������� ���������� �������
	{
		//cout<<"Wait BF_REQ_BUSY..."<<endl;
	}
	pci_mem-=2; // � ������ �����

	while(!err && size)
	{
		int i;
		int put_size = (size < 128) ? size : 128;

		//_____���������� ���� ������ � ����
		pci_mem+=128; // ������ ����� BF_REQ_DATA
		for (i=0; i < put_size; i++)
		{
			pci_mem+=i;
			*pci_mem=regs[i];
			pci_mem-=i;
		}
		pci_mem-=128; //� ������ �����
		//_____���������� ��������� �������� - ������ � ����� � ������ BF
		pci_mem+=6; //BF_REQ_SIZE
		*pci_mem=put_size;
		pci_mem-=6; //� ������ �����

		pci_mem+=5; //BF_REQ_ADDR
		*pci_mem= addr;
		pci_mem-=5; //� ������ �����

		pci_mem+=1; //BF_CMD
		*pci_mem=0x0002;
		pci_mem-=1;// � ������ �����

		//_____�������� ���������� ��������
		pci_mem+=2;// BF_STATUS
		while(*pci_mem & 0x00000002) //������� ���������� ���������� �������
		{
			//cout<<"Wait BF_REQ_BUSY..."<<endl;
		}
		pci_mem-=2; // � ������ �����

		if(!err)
		{
			size-=put_size;
			regs+=put_size;
			addr+=put_size*4;
		}
	}
	return err;
}
