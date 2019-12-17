#include "Headers.h"

int BF_loader(unsigned* pci_mem)
{
	//____ПРОШИВКА BLACKFIN
	//____считываем файл прошивки, выделяем память
	int err=0,next_err=0,stop=0,rd_size=0;
	FILE * ptrFile = fopen("/home/l502-bf.ldr", "rb");
	if(ptrFile==NULL)
	{
		err=L502_ERR_LDR_FILE_OPEN;
	}
	uint32_t* ldr_buff=new uint32_t [4096];

	//____предварительный сброс и запуск bf
	pci_mem+=0; // BF_CTL
	*pci_mem=0x00000010 | (*pci_mem & 0xf00); // сброс BF_RST
	delay(1);
	*pci_mem=0x00000010 | 0x00000002 | (*pci_mem & 0xf00);
	pci_mem-=0; //в начало блока

	while(*pci_mem & 0x00000008)
	{
		//cout<<"BF_HWAIT..."<<endl;
		//ожидаем нуля в регистре BF_HWAIT
	}

	//____разбираем заголовок блока LDR- формата и сохраняем в структуре
	uint8_t hdr[16];
	t_bf_ldr_pkt pkt, pkt_next;

	err=fread(hdr,1,16,ptrFile)== 16 ? f_parse_ldr_hdr(hdr,&pkt): L502_ERR_LDR_FILE_READ; // читаем 16 байт заголовка ldr и парсим в структуру

	 while (!err && !stop)
	 {
		 //_____
		    if (next_err)
		    {
		    	err = next_err;
		    }

		    else if (((pkt.flags & 0x0100) == 0) && (pkt.size!=0))
		    {
		         int r_size = (pkt.size > 4096) ? 4096 : pkt.size;
		         rd_size = static_cast<int>(fread(ldr_buff, 1, r_size, ptrFile));
		         if (rd_size!=r_size)
		         {
		        	 err = L502_ERR_LDR_FILE_READ;
		         }
		    }
		  //_____
		    if(!err)
		    {
		    	if (pkt.size > 4096) //???
		    	{
		    		pkt_next = pkt;
		    		pkt_next.addr += 4096;
		    		pkt_next.size -= 4096;
		    		pkt.size = 4096;
		    	}
		    	else
		    	{
		    		next_err= fread(hdr, 1, 16, ptrFile) == 16 ? f_parse_ldr_hdr(hdr, &pkt_next): L502_ERR_LDR_FILE_READ;
		    		if(next_err)
		    		{
		    			pkt_next.size=0;
		    		}
		    	}

		    	if (pkt.size!=0)
		    	{
		    		uint32_t size = ((pkt.size+31)/(32))*8;
		    		if (pkt.flags & 0x0100)
						 {
							uint32_t i;
							for (i=0; i < size; i++)
								{
									ldr_buff[i] = pkt.arg;
								}
						  }
		    		if ((pkt.flags & 0x8000) || ((pkt_next.flags & 0x8000) && (pkt_next.size==0)))
		    		{
		    			uint32_t buf_pos = 0;
		    			err = BF_CHECK_ADDR_SIZE(pkt.addr, size);

		    			if (!err && (size > 8))
		    			{
		    				err = f_bf_mem_wr(pci_mem, pkt.addr, ldr_buff, size-8);
		    				pkt.addr+=4*(size-8);
		    				size = 8;
		    				buf_pos = size-8;
		    			}

		    			if(!err)
		    			{
		    				pci_mem+=1; //BF_CMD
		    				*pci_mem=0x0004; // пишем команду L502_BF_CMD_HIRQ
		    				pci_mem-=1; //в начало блока
		    			}
		    			if(!err)
		    			{
		    				err=f_bf_mem_wr(pci_mem, pkt.addr, &ldr_buff[buf_pos], size);
		    			}
		    			stop=1;

		    			pci_mem+=0; //BF_CTL
		    			*pci_mem= 0x00000012;
		    			pci_mem-=0; //в начало блока
		    		}
		    		else if(!(pkt.flags & 0x1000))
		    		{
		    			err = BF_CHECK_ADDR_SIZE(pkt.addr, size);
		    			if(!err)
		    			{
		    				err = f_bf_mem_wr(pci_mem, pkt.addr, ldr_buff, size);
		    			}
		    		}
		    	}
		    	pkt=pkt_next;
		    }
	 }
	 delete[] ldr_buff;
	 fclose(ptrFile);
	 return err;
}

