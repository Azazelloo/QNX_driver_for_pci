#include <cstdlib>
#include <string.h>
#include <iostream>
#include <hw/pci.h>
#include <sys/neutrino.h>
#include <sys/slog.h>
#include <sys/mman.h>
#include <stdlib.h>

#define VendorID 0x1172
#define DeviceID 0x502

void AsyncDOUT(unsigned* p_pci_mem,int state_dout);
