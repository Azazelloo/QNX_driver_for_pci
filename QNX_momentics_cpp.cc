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
		AsyncDOUT(pci_mem,0x00000000);//�������� DOUT
		/*AsyncDOUT(pci_mem,0x00000000);/**/

		//____DOUT � ���������� ������
		/*InitSetDev(pci_mem,0x00000002); //���������� �������������� ��������� DOUT

		//____�������� ������
		int buf_frame=1;
		const int buf_size=10;
		int impulse_array[buf_size];
		impulse_array[0]=0x00000001;
		impulse_array[1]=0x00000001;
		impulse_array[2]=0x00000001;
		impulse_array[3]=0x00000001;
		impulse_array[4]=0x00000000;
		impulse_array[5]=0x00000000;
		impulse_array[6]=0x00000000;
		impulse_array[7]=0x00000000;
		impulse_array[8]=0x00000000;
		impulse_array[9]=0x00000000;
		//____��������� ����������� �������
		off64_t offset;
		mem_offset64(&impulse_array,NOFD,sizeof(impulse_array),&offset,0);//�������� ���������� ����� ������
		StreamsEnable(pci_mem,offset,buf_size,buf_frame*buf_size,0x00000002); //��������� ��������� ����� �� ������
		StreamsStart(pci_mem,0x00000002);
		delay(1);
		StreamsStop(pci_mem,0x00000002);
		/**/
		//____�������� BLACKFIN

		//____��������������� ����� � ������ bf
		pci_mem+=0; // BF_CTL
		*pci_mem=0x00000000;
		delay(1);
		*pci_mem=0x00000002;

		while(*pci_mem & 0x00000004)
		{
			cout<<"BF_HWAIT..."<<endl;
			//������� ���� � �������� BF_HWAIT
		}

		//___�������� �������� BF_REQ_BUSY (!!!)

		//��������� ���� �������� � ���������� ��������������� ��������
		FILE * ptrFile = fopen("/home/l502-bf.ldr", "rb");
		int r_buff[4];
		fread(r_buff,sizeof(r_buff[0]),4,ptrFile);

		pci_mem+=128;//BF_REQ_DATA (1-�� �������)
		*pci_mem=r_buff[0];
		/**/
		//____����� ��������� �� �����
		for(int i = 1; i <=8; i++)
			{
				std::cout<<"["<<i-1<<"] = "<<*(pci_mem + (i-1))<<" = "<<pci_mem + (i-1)<<std::endl;
			}
			/**/
}
