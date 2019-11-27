#include "Headers.h"

int main(int argc, char *argv[])
{
		void *hdl;
		struct pci_dev_info pci_info;
		unsigned vendor = VendorID, device =DeviceID;
		int pd;
		unsigned *pci_mem;

		//____����������� � PCI ����
		pd = pci_attach(0);//��������� ���������

		memset(&pci_info, 0, sizeof(pci_info));
		pci_info.VendorId = vendor;
		pci_info.DeviceId = device;

		hdl = pci_attach_device(NULL, PCI_SEARCH_VENDEV|PCI_SHARE, 0, &pci_info);

		if(hdl == NULL)
			{
				std::cout<<"Device not found"<<std::endl;;
				return (-1);
			}

		std::cout<<"Found PCI device "<<std::hex<<pci_info.VendorId<<","<<std::hex<<pci_info.DeviceId<<" ,Bus "<<static_cast<double>(pci_info.BusNumber)<<std::endl;

		pci_attach_device(hdl, PCI_INIT_ALL|PCI_INIT_ROM|PCI_SHARE, 0, &pci_info);

		pci_mem = (unsigned *)mmap_device_memory(NULL, pci_info.BaseAddressSize[0],PROT_READ|PROT_WRITE|PROT_NOCACHE,0, PCI_MEM_ADDR(pci_info.CpuBaseAddress[0]));

		//____DOUT ���������� ������ � ����������� ������
		/*AsyncDOUT(pci_mem,0x0000000);//�������� �����
		AsyncDOUT(pci_mem,0x00000000);/**/

		//____DOUT � ���������� ������
		//____���������� DOUT
		pci_mem+=0x00000700+1; //������� DMA_EN
		if((*pci_mem) & 0x00000002)
		{
			pci_mem+=1;
			*pci_mem=0x00000002;
			pci_mem-=1;
		}

		pci_mem+=5; //������� DMA_IRQ_DIS
		*pci_mem=0x0000003;
		pci_mem-=5;

		pci_mem+=3; //������� DMA_IRQ
		*pci_mem=0x00000002; //������������ 1 � �� ������ ������� �������
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
		*pci_mem=0x00000002;
		pci_mem-=0x00000700+3;

		//____�������� ������
		unsigned int impulse_array[100];
		unsigned int *p_ar=impulse_array;
		for(int i=0;i<100;i++)
		{
			impulse_array[i]=0x00000100;
		}

		//____��������� ����������� �������
		std::cout<<"RAM addr = "<<p_ar<<std::endl;
		pci_mem+=0x00000c00+16; //1-�� ������� 0-�� ��������
		*pci_mem=reinterpret_cast<unsigned int>(p_ar);

		pci_mem+=1;
		*pci_mem=0;

		pci_mem+=1;
		*pci_mem=100;

		pci_mem+=1;
		*pci_mem=0;

		//____���������� ������ ��������
		pci_mem-=3;
		pci_mem-=16;//������� � ������ ����� ������ ������
		pci_mem+=1; //������� ���������� DMA_CH_CMP_CNTR
		*pci_mem=50;

		pci_mem-=1;//������� ������ ������ ������ DMA_CH_CTL
		*pci_mem=0x00010020;

		pci_mem-=0x00000c00;
		pci_mem+=0x00000700+5; //������� DMA_IRQ_EN
		*pci_mem=0x00000002;

		pci_mem-=5;
		pci_mem+=1; //������� ������� �������
		*pci_mem=0x00000002;

		/*pci_mem+=1;//������� ��������� �������
		*pci_mem=0x00000003;

		pci_mem-=1;
		/**/

		//____����� ��������� �� �����
		for(int i = 1; i <= 6; i++)
			{
				std::cout<<"["<<i-1<<"] = "<<*(pci_mem + (i-1))<<" = "<<pci_mem + (i-1)<<std::endl;
			}
			/**/
}
