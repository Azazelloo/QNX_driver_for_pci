#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <iostream>
#include <hw/pci.h>
#include <sys/neutrino.h>
#include <sys/slog.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <cstdio>

using namespace std;

typedef struct st_bf_ldr_pkt
{
    uint8_t res;
    uint8_t dma_mode;
    uint16_t flags;
    uint32_t addr;
    uint32_t size;
    uint32_t arg;
} t_bf_ldr_pkt;


#define VendorID 0x1172
#define DeviceID 0x502

/*блоки памяти*/
#define GEN_REGS_DMA 0x00000700
#define DMA_IN 		 0x00000800
#define DMA_OUT		 0x00000c00
#define IO_HARD		 0x00000200
/*общие регистры*/
#define DMA_EN  	1
#define DMA_DIS     2
#define DMA_RST     3
#define DMA_IRQ     4
#define DMA_IRQ_EN  5
#define DMA_IRQ_DIS 6
/*регистры каналов*/
#define DMA_CH_CTL 		0
#define DMA_CH_CMP_CNTR 1
#define DMA_CH_CUR_CNTR 2
#define DMA_CH_CUR_POS  3
#define DMA_CH_PC_POS	4
/*регистры управления вводом/выводом*/
#define GO_SYNC_IO 	  0x0000010a
#define OUTSWAP_BFCTL 0x00000118
#define PRELOAD_ADC	  0x0000010c

void AsyncDOUT(unsigned* p_pci_mem,int state_dout); //Асинхронное управление DOUT
void InitSetDev(unsigned* pci_mem, int num_chan); //первоначальная настройка канала (АЦП/ЦАП)
void StreamsEnable(unsigned* pci_mem,off64_t buf_addr,int addr_size, int interrupt, int num_chan); //interrupt - количество сэмплов
void StreamsStart(unsigned* pci_mem,int num_chan);
void StreamsStop(unsigned* pci_mem,int num_chan);
