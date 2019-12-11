#include "Headers.h"
void InitSetDev(unsigned* pci_mem, int num_chan)
{
	pci_mem+=GEN_REGS_DMA+DMA_EN; //регистр DMA_EN
	if((*pci_mem) & num_chan) //если канал включен -> сбрасываем
	{
		pci_mem+=1; //попадаем в DMA_DIS
		*pci_mem=num_chan;
		pci_mem-=1;
	}
	pci_mem-=DMA_EN; // в начало блока

	pci_mem+=DMA_IRQ_DIS; //регистр DMA_IRQ_DIS
	*pci_mem=num_chan;
	pci_mem-=DMA_IRQ_DIS; // в начало блока

	pci_mem+=DMA_IRQ; //регистр DMA_IRQ
	*pci_mem=num_chan; //записываются 1 в те каналы которые активны
	pci_mem-=DMA_IRQ; //в начало блока

	pci_mem-=GEN_REGS_DMA; //возвращаемся в начало блока памяти
	pci_mem+=DMA_OUT+DMA_CH_CUR_CNTR; //регистр DMA_CH_CUR_CNTR (сколько слов передано)
	*pci_mem=0;
	pci_mem-=DMA_CH_CUR_CNTR; //в начало блока

	pci_mem+=DMA_CH_CUR_POS; //регистр DMA_CH_CUR_POS (номер страницы и смещение внутри страницы для следующей передачи)
	*pci_mem=0;
	pci_mem-=DMA_CH_CUR_POS; // в начало блока

	pci_mem+=DMA_CH_PC_POS; //регистр DMA_CH_PC_POS (Указывает номер страницы и смещение внут-ри страницы за последним обработаннымпрограммой ПК словом.)
	*pci_mem=0;
	pci_mem-=DMA_CH_PC_POS; // в начало блока

	pci_mem-=DMA_OUT; //возвращаемся в начало блока памяти
	pci_mem+=GEN_REGS_DMA+DMA_RST; //регистр DMA_RST
	*pci_mem=0x00000003; //сброс 1-ого и 2-ого каналов
}

