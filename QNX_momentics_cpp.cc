#include "Headers.h"

int main(int argc, char *argv[])
{
		void *hdl;
		struct pci_dev_info pci_info;
		unsigned vendor = VendorID, device =DeviceID;
		int pd;
		unsigned *pci_mem;

		/*ПОДКЛЮЧЕНИЕ К PCI ШИНЕ*/
		pd = pci_attach(0);//расписать подробнее

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

		/*DOUT в асинхронном режиме*/
		//*pci_mem=0x00000001; //сброс l502 для повторного сбора данных ????
		AsyncDOUT(pci_mem,0x00000000);
		//pci_mem+=0x0200+0x0114; //переходим в начало нужного блока + регистр
		//*pci_mem=0x00000001; //включаем канал 1 (DOUT)
		/*Вывод регистров на экран*/
		pci_mem+=0x200+0x112;
		for(int i = 1; i <= 6; i++)
			{
				std::cout<<"["<<i-1<<"] = "<<*(pci_mem + (i-1))<<" = "<<pci_mem + (i-1)<<std::endl;
			}
			/**/
}
