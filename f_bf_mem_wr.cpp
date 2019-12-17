#include "Headers.h"

int f_bf_mem_wr(unsigned* pci_mem,uint32_t addr, const uint32_t* regs, uint32_t size)
{
	int err=0;
	pci_mem+=2;// BF_STATUS
	while(*pci_mem & 0x00000002) //ожидаем завершени€ предыдущей команды
	{
		//cout<<"Wait BF_REQ_BUSY..."<<endl;
	}
	pci_mem-=2; // в начало блока

	while(!err && size)
	{
		int i;
		int put_size = (size < 128) ? size : 128;

		//_____записываем блок данных в ѕЋ»—
		pci_mem+=128; // начало блока BF_REQ_DATA
		for (i=0; i < put_size; i++)
		{
			pci_mem+=i;
			*pci_mem=regs[i];
			pci_mem-=i;
		}
		pci_mem-=128; //в начало блока
		//_____записываем параметры передачи - размер и адрес в пам€ти BF
		pci_mem+=6; //BF_REQ_SIZE
		*pci_mem=put_size;
		pci_mem-=6; //в начало блока

		pci_mem+=5; //BF_REQ_ADDR
		*pci_mem= addr;
		pci_mem-=5; //в начало блока

		pci_mem+=1; //BF_CMD
		*pci_mem=0x0002;
		pci_mem-=1;// в начало блока

		//_____ожидание завершени€ операции
		pci_mem+=2;// BF_STATUS
		while(*pci_mem & 0x00000002) //ожидаем завершени€ предыдущей команды
		{
			//cout<<"Wait BF_REQ_BUSY..."<<endl;
		}
		pci_mem-=2; // в начало блока

		if(!err)
		{
			size-=put_size;
			regs+=put_size;
			addr+=put_size*4;
		}
	}
	return err;
}
