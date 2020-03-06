#include "Headers.h"

int f_parse_ldr_hdr(uint8_t* hdr, t_bf_ldr_pkt* pkt)
{
	int err=0;
	uint32_t* pdw_buff = reinterpret_cast<uint32_t*> (hdr);
	uint8_t xor_ch = 0;
	int i;
	for (i=0; i < 16; i++)
	{
	    xor_ch ^= hdr[i];
	}

	if ((xor_ch!=0) || (hdr[3] != 0xAD))
	{
	    err=L502_ERR_LDR_FILE_FORMAT;
	}
	else
	{
	    pkt->res = 0;
	    pkt->dma_mode = pdw_buff[0]&0xF;
	    pkt->flags = pdw_buff[0]&0xFFF0;
	    pkt->addr = pdw_buff[1];
	    pkt->size = pdw_buff[2];
	    pkt->arg = pdw_buff[3]; // данные для заполнения (?)

	    if ((pkt->flags & 0x0800) && (pkt->flags & 0x0100))
	        err = L502_ERR_LDR_FILE_FORMAT;
	    else if (pkt->flags & (0x0400 | 0x2000 | 0x0800))
	        err = L502_ERR_LDR_FILE_UNSUP_FEATURE;
	     else if ((pkt->flags & 0x0800) && (pkt->addr != 0xFFA00000))
	        err = L502_ERR_LDR_FILE_UNSUP_STARTUP_ADDR;
	  }
	return err;
}
