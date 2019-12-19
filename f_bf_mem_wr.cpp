#include "Headers.h"

int f_bf_mem_wr(unsigned* pci_mem,uint32_t addr, const uint32_t* regs, uint32_t size)
{
	int err=0;
	pci_mem+=BF_STATUS;// BF_STATUS
	while(*pci_mem & 0x00000002) //ожидаем завершения предыдущей команды
	{
		//cout<<"Wait BF_REQ_BUSY..."<<endl;
	}
	pci_mem-=BF_STATUS; // в начало блока

	while(!err && size)
	{
		int i;
		int put_size = (size < 128) ? size : 128; //128 регистров - максимальный блок для записи

		//_____записываем блок данных в ПЛИС
		pci_mem+=BF_REQ_DATA; // начало блока BF_REQ_DATA
		for (i=0; i < put_size; i++)
		{
			pci_mem+=i;
			*pci_mem=regs[i]; // пишем int (!)
			pci_mem-=i;
		}
		pci_mem-=BF_REQ_DATA; //в начало блока
		//_____записываем параметры передачи - размер и адрес в памяти BF
		pci_mem+=BF_REQ_SIZE; //BF_REQ_SIZE
		*pci_mem=put_size;
		pci_mem-=BF_REQ_SIZE; //в начало блока

		pci_mem+=BF_REQ_ADDR; //BF_REQ_ADDR
		*pci_mem= addr;
		pci_mem-=BF_REQ_ADDR; //в начало блока

		pci_mem+=BF_CMD; //BF_CMD
		*pci_mem=0x0002;
		pci_mem-=BF_CMD;// в начало блока

		//_____ожидание завершения операции
		pci_mem+=BF_STATUS;// BF_STATUS
		while(*pci_mem & 0x00000002) //ожидаем завершения предыдущей команды
		{
			//cout<<"Wait BF_REQ_BUSY..."<<endl;
		}
		pci_mem-=BF_STATUS; // в начало блока

		if(!err)
		{
			size-=put_size;
			regs+=put_size;
			addr+=put_size*4;
		}
	}
	return err;
}
