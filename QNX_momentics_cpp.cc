#include "Headers.h"

int main(int argc, char *argv[])
{
		void *hdl;
		struct pci_dev_info pci_info;
		unsigned vendor = VendorID, device =DeviceID;
		int pd;
		unsigned *pci_mem;

		//____ПОДКЛЮЧЕНИЕ К PCI ШИНЕ
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

		//____DOUT выставляем уровни в асинхронном режиме
		/*AsyncDOUT(pci_mem,0x0000000);//обнуляем плату
		AsyncDOUT(pci_mem,0x00000000);/**/

		//____DOUT в импульсном режиме
		//____подготовка DOUT
		pci_mem+=0x00000700+1; //регистр DMA_EN
		if((*pci_mem) & 0x00000002)
		{
			pci_mem+=1;
			*pci_mem=0x00000002;
			pci_mem-=1;
		}

		pci_mem+=5; //регистр DMA_IRQ_DIS
		*pci_mem=0x0000003;
		pci_mem-=5;

		pci_mem+=3; //регистр DMA_IRQ
		*pci_mem=0x00000002; //записываются 1 в те каналы которые активны
		pci_mem-=3;

		pci_mem-=0x00000700+1; //возвращаемся в начало блока памяти
		pci_mem+=0x00000c00+2; //регистр DMA_CH_CUR_CNTR (сколько слов передано)
		*pci_mem=0;

		pci_mem+=1; //регистр DMA_CH_CUR_POS (номер страницы и смещение внутри страницы для следующей передачи)
		*pci_mem=0;

		pci_mem+=1; //регистр DMA_CH_PC_POS (Указывает номер страницы и смещение внут-ри страницы за последним обработаннымпрограммой ПК словом.)
		*pci_mem=0;

		pci_mem-=2;
		pci_mem-=0x00000c00+2; //возвращаемся в начало блока памяти
		pci_mem+=0x00000700+3; //регистр DMA_RST
		*pci_mem=0x00000002;
		pci_mem-=0x00000700+3;

		//____Создание буфера
		unsigned int impulse_array[100];
		unsigned int *p_ar=impulse_array;
		for(int i=0;i<100;i++)
		{
			impulse_array[i]=0x00000100;
		}

		//____Заполняем дескрипторы страниц
		std::cout<<"RAM addr = "<<p_ar<<std::endl;
		pci_mem+=0x00000c00+16; //1-ый регистр 0-ой страницы
		*pci_mem=reinterpret_cast<unsigned int>(p_ar);

		pci_mem+=1;
		*pci_mem=0;

		pci_mem+=1;
		*pci_mem=100;

		pci_mem+=1;
		*pci_mem=0;

		//____Выставляем нужные регистры
		pci_mem-=3;
		pci_mem-=16;//возврат в начало блока памяти канала
		pci_mem+=1; //регистр прерываний DMA_CH_CMP_CNTR
		*pci_mem=50;

		pci_mem-=1;//регистр режима работы канала DMA_CH_CTL
		*pci_mem=0x00010020;

		pci_mem-=0x00000c00;
		pci_mem+=0x00000700+5; //регистр DMA_IRQ_EN
		*pci_mem=0x00000002;

		pci_mem-=5;
		pci_mem+=1; //регистр запуска каналов
		*pci_mem=0x00000002;

		/*pci_mem+=1;//регистр остановки каналов
		*pci_mem=0x00000003;

		pci_mem-=1;
		/**/

		//____Вывод регистров на экран
		for(int i = 1; i <= 6; i++)
			{
				std::cout<<"["<<i-1<<"] = "<<*(pci_mem + (i-1))<<" = "<<pci_mem + (i-1)<<std::endl;
			}
			/**/
}
