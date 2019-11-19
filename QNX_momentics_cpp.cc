#include <cstdlib>
#include <string.h>
#include <iostream>
#include <hw/pci.h>
#include <sys/neutrino.h>
#include <sys/slog.h>
#include <sys/mman.h>

#define VendorID 0x1172
#define DeviceID 0x502
#define BAR 0xf0200000

#define RG_VME_BASE_ADD_RAM	*((volatile unsigned char*)((unsigned)VLK_RG_va+0x314))


int main(int argc, char *argv[]) {
	/* PCI device specific stuff */
	   typedef struct _device_context_t
	   {
	      struct pci_dev_info  pcidevice_info;
	      void*                pcidevhandle;
	   } device_context_t;

	   device_context_t device;
	   int32_t pcihandle=0x00000000;
	   uint32_t pciindex=0;

	/* Clear PCI device structure */
	   memset(&device.pcidevice_info, 0x00, sizeof(struct pci_dev_info));

	   pcihandle=pci_attach(0);
	   std::cout<<"pcihandle:"<<pcihandle<<std::endl;
	    if (pcihandle==-1)
	        {
	           std::cout<<"XXXXXX: Can't attach to pci server"<<std::endl;
	           return -1;
	        }
	 /* Find compatible PCI device */
	       device.pcidevice_info.VendorId=VendorID;//4332;
	       device.pcidevice_info.DeviceId=DeviceID;//33128;
	       device.pcidevhandle=pci_attach_device(NULL, PCI_SEARCH_VENDEV, pciindex, &device.pcidevice_info);
	       std::cout<<"Search device:"<<device.pcidevhandle<<std::endl;

	       if (device.pcidevhandle==NULL)
	       {
	           if (pciindex==0)
	           {
	               pci_detach(pcihandle);
	               std::cout<<"XXXXX: Couldn't find XXXXXX device"<<std::endl;
	               return -1;
	          }
	       }
	 /* Attach to PCI device */
	        device.pcidevhandle=pci_attach_device(device.pcidevhandle, PCI_INIT_ALL, pciindex, &device.pcidevice_info);
	        if (device.pcidevhandle==NULL)
	        {
	           //xxxxxxxx_signal_handler(-1);
	           std::cout<<"XXXXXX: Couldn't allocate PCI device resources"<<std::endl;
	           return -1;
	        }
	        std::cout<<"Connect to device:"<<device.pcidevhandle<<std::endl;
	 /*Config to PCI device*/
	        struct _pci_cardbus_config_regs cfg_space;
	        pci_read_config(device.pcidevhandle,0,0x40,4,&cfg_space);//заполняем структуру конфигурации устройства
	        void* ptr;//указатель на начало адресного пространства
	        pci_read_config(device.pcidevhandle,0x10,2,4,&ptr);

	        void* p_space1;
	        p_space1 = mmap_device_memory(0, 0x100, PROT_READ | PROT_WRITE | PROT_NOCACHE,0,BAR);
	    	if (p_space1 == MAP_FAILED){

	    		std::cout<<"MAP_FAILED"<<std::endl;
	    		return EXIT_SUCCESS;
	    	}
	    	//char* p=static_cast<char*>(VLK_RG_va);
	    	//*p=100;
	    	//char tt =RG_VME_BASE_ADD_RAM;
	        //RG_VME_BASE_ADD_RAM=0;


	        /*unsigned devfunc;
	        struct _pci_cardbus_config_regs cfg_space;
	        pci_read_config(device.pcidevhandle,0,0x40,4,&cfg_space);//заполняем структуру конфигурации устройства
	        //void* ptr;//указатель на начало адресного пространства
	        //pci_read_config(device.pcidevhandle,0x10,2,4,&ptr);
	        unsigned long b_ptr;
	        pci_read_config32(1,devfunc,0x10,2,&b_ptr);
	        b_ptr|=0x114;
	        pci_write_config32(1,devfunc,0x10,2,&b_ptr);
	        pci_read_config32(1,devfunc,0x10,2,&b_ptr);*/
	        //pci_write_config(device.pcidevhandle,0x10,2,4,&ptr|0x114);
	        //pci_write_config(device.pcidevhandle,0x10,2,4,&cfg_space);
	        //pci_read_config(device.pcidevhandle,0,0x40,4,&cfg_space);
///////////////
	   std::cout<<"pci_detach:"<<pci_detach(pcihandle)<<std::endl;
	   return EXIT_SUCCESS;
}
