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
		AsyncDOUT(pci_mem,0x00000000);//обнуляем DOUT
		/*AsyncDOUT(pci_mem,0x00000000);/**/

		//____DOUT в импульсном режиме
		/*InitSetDev(pci_mem,0x00000002); //производим первоначальную настройку DOUT

		//____Создание буфера
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
		//____Заполняем дескрипторы страниц
		off64_t offset;
		mem_offset64(&impulse_array,NOFD,sizeof(impulse_array),&offset,0);//получаем физический адрес буфера
		StreamsEnable(pci_mem,offset,buf_size,buf_frame*buf_size,0x00000002); //запускаем одиночный поток на запись
		StreamsStart(pci_mem,0x00000002);
		delay(1);
		StreamsStop(pci_mem,0x00000002);
		/**/
		//____ПРОШИВКА BLACKFIN

		//____предварительный сброс и запуск bf
		pci_mem+=0; // BF_CTL
		*pci_mem=0x00000000;
		delay(5);
		*pci_mem=0x00000002;

		while(*pci_mem & 0x00000004)
		{
			cout<<"BF_HWAIT..."<<endl;
			//ожидаем нуля в регистре BF_HWAIT
		}
		//____разбираем файл прошивки и записываем соответствующие регистры
		FILE * ptrFile = fopen("/home/l502-bf.ldr", "rb");
		fseek(ptrFile, 0, SEEK_END);   // устанавливаем позицию в конец файла
		long lSize = ftell(ptrFile)/4; // получаем размер в байтах, делим на размер int
		rewind(ptrFile);

		unsigned int* r_buff=new unsigned int [lSize];
		fread(r_buff,sizeof(r_buff[0]),lSize,ptrFile);
		//____конвертим в BIG ENDIAN
		/*for(int i=0; i<lSize;i++)
		{
			r_buff[i]=(r_buff[i] << 24) | (((r_buff[i] << 16) >> 24) << 16) | (((r_buff[i] << 8) >> 24) << 8) | (r_buff[i] >> 24);
		}/**/

		int num_full_rec = lSize / 128; // количество полных записей в буфер по 128 слов
		int balance_rec = lSize - (num_full_rec * 128); // остаток СЛОВ (не байт!) не вошедших в последний полный буфер

		if (balance_rec) // есть остаток буфера
		{
			int counter_ldr_array = 0,i;
			for (i = 1; i <= num_full_rec; i++) //записываем полные буферы
			{
				//____непосредственно, запись
				pci_mem+=2; //BF_STATUS
				while(*pci_mem & 0x00000002) // ожидаем окончания предыдущей записи
					{
						cout<<"BF_REQ_BUSY..."<<endl;
						//ожидаем нуля в регистре BF_REQ_BUSY
					}
				cout<<"BF_REQ_BUSY done:"<<*pci_mem<<endl;

				pci_mem-=2; // в начало блока
				pci_mem+=128; //первый регистр BF_REQ_DATA

				//____запись 128 регистров
				for (int j=counter_ldr_array;j<i*128;j++)
				{
					pci_mem+=(j-(i-1)*128); //двигаем на регистр
					*pci_mem=r_buff[j];
					//cout <<dec<<j<<" = "<< hex << r_buff[j] << endl;
					pci_mem-=(j-(i-1)*128); // в начало блока
				}

				counter_ldr_array+=128; //двигаем на 128 по массиву для записи следующего блока регистров

				pci_mem-=128; //в начало блока

				pci_mem+=6; //BF_REQ_SIZE
				*pci_mem=128;
				pci_mem-=6; // в начало блока

				pci_mem+=5; // BF_REQ_ADDR
				*pci_mem=0xFF8007FF+2;//0x00000128;
				pci_mem-=5; // в начало блока

				pci_mem+=1; //BF_CMD
				*pci_mem=2; //код L502_BF_CMD_WRITE
				pci_mem-=1; //в начало блока

			}
			//___подаем необходимую команду
			pci_mem+=1; //BF_CMD
			*pci_mem=4; // L502_BF_CMD_HIRQ
			pci_mem-=1; //в начало блока

			//___записываем остаток
			pci_mem+=2; //BF_STATUS
			while(*pci_mem & 0x00000002) // ожидаем окончания предыдущей записи
				{
					cout<<"BF_REQ_BUSY_Balance..."<<endl;
					//ожидаем нуля в регистре BF_REQ_BUSY
				}
			cout<<"BF_REQ_BUSY_LAST done:"<<*pci_mem<<endl;

			pci_mem-=2; // в начало блока
			pci_mem+=128; //первый регистр BF_REQ_DATA
			int counter=0;
			for (int j=counter_ldr_array;j<(counter_ldr_array+balance_rec);j++)
				{
					pci_mem+=counter; //двигаем на регистр
					*pci_mem=r_buff[j];
					//cout <<dec<<j<<" = "<< hex << r_buff[j] << endl;
					pci_mem-=counter; // в начало блока
					counter++;
				}
			pci_mem-=128; //в начало блока

			pci_mem+=6; //BF_REQ_SIZE
			*pci_mem=balance_rec;
			pci_mem-=6; // в начало блока

			pci_mem+=5; // BF_REQ_ADDR
			*pci_mem=0xFF8007FF+2;//0x00000128;
			pci_mem-=5; // в начало блока

			pci_mem+=1; //BF_CMD
			*pci_mem=2; //код L502_BF_CMD_WRITE
			pci_mem-=1; //в начало блока
		}
		/**/
		//____Вывод регистров на экран
		for(int i = 1; i <=256; i++)// выходим за область на 1 регистр для наглядности
			{
				std::cout<<"["<<i-1<<"] = "<<*(pci_mem + (i-1))<<" = "<<pci_mem + (i-1)<<std::endl;
			}

		delete[] r_buff;

}
