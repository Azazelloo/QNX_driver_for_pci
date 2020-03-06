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

#define BF_ldr_filename "/home/l502-bf.ldr"

/*ошибки*/
#define L502_ERR_LDR_FILE_FORMAT 			 -1
#define L502_ERR_LDR_FILE_UNSUP_FEATURE 	 -2
#define L502_ERR_LDR_FILE_UNSUP_STARTUP_ADDR -3
#define L502_ERR_LDR_FILE_READ				 -4
#define L502_ERR_BF_INVALID_ADDR			 -5
#define L502_ERR_LDR_FILE_OPEN				 -6

/*блоки памяти*/
#define GEN_REGS_DMA 0x00000700
#define DMA_IN 		 0x00000800
#define DMA_OUT		 0x00000c00
#define IO_HARD		 0x00000200

/*общие регистры каналов*/
#define DMA_EN  	1
#define DMA_DIS     2
#define DMA_RST     3
#define DMA_IRQ     4
#define DMA_IRQ_EN  5
#define DMA_IRQ_DIS 6

/*отдельные регистры каналов*/
#define DMA_CH_CTL 		0
#define DMA_CH_CMP_CNTR 1
#define DMA_CH_CUR_CNTR 2
#define DMA_CH_CUR_POS  3
#define DMA_CH_PC_POS	4

/*регистры управления вводом/выводом*/
#define GO_SYNC_IO 	  0x0000010a
#define OUTSWAP_BFCTL 0x00000118
#define PRELOAD_ADC	  0x0000010c

/*регистры BF_control*/
#define BF_CTL 		  0
#define BF_CMD		  1
#define BF_STATUS	  2
#define BF_REQ_DATA	  128
#define BF_REQ_SIZE	  6
#define BF_REQ_ADDR	  5

/*флаги для запуска прошивки (значение неизвестно)*/
#define BF_LDR_FLAG_FILL 	0x0100
#define BF_LDR_FLAG_FINAL	0x8000
#define BF_LDR_FLAG_IGNORE	0x1000

//____проверка адреса память bf  который происходит запись (смотри ADSP-BF52x Memory Map)
#define BF_CHECK_ADDR(addr)  (((addr) < 0xFFA0C000) && ((addr)>= 0xFFA0000)) || \
    (((addr) < 0xFF908000) && ((addr) >=0xFF900000)) || \
    (((addr) < 0xFF808000) && ((addr) >=0xFF800000)) || \
    (((addr) < 0x2000000)) ? 0 : L502_ERR_BF_INVALID_ADDR
//____проверка адреса со сдвигом на размер буфера
#define BF_CHECK_ADDR_SIZE(addr, size) BF_CHECK_ADDR(addr) ? L502_ERR_BF_INVALID_ADDR : \
		BF_CHECK_ADDR(addr+size*4-1) ? L502_ERR_BF_INVALID_ADDR : 0

/*прототипы функций*/
void AsyncDOUT(unsigned* p_pci_mem,int state_dout); //Асинхронное управление DOUT
void InitSetDev(unsigned* pci_mem, int num_chan); //первоначальная настройка канала (АЦП/ЦАП)
void StreamsEnable(unsigned* pci_mem,off64_t buf_addr,int addr_size, int interrupt, int num_chan); //interrupt - количество сэмплов
void StreamsStart(unsigned* pci_mem,int num_chan);
void StreamsStop(unsigned* pci_mem,int num_chan);
int BF_loader(unsigned* pci_mem);
int f_parse_ldr_hdr(uint8_t* hdr, t_bf_ldr_pkt* pkt);
int f_bf_mem_wr(unsigned* pci_mem,uint32_t addr, const uint32_t* regs, uint32_t size);


