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

#define VendorID 0x1172
#define DeviceID 0x502

void AsyncDOUT(unsigned* p_pci_mem,int state_dout); //Асинхронное управление DOUT
void InitSetDev(unsigned* pci_mem, int num_chan); //первоначальная настройка канала (АЦП/ЦАП)
void StreamsEnable(unsigned* pci_mem,off64_t buf_addr,int addr_size, int interrupt, int num_chan); //interrupt - количество сэмплов
void StreamsStart(unsigned* pci_mem,int num_chan);
void StreamsStop(unsigned* pci_mem,int num_chan);
