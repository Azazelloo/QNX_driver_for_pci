#include "Headers.h"
void AsyncDOUT(unsigned* p_pci_mem,int state_dout)
{
	p_pci_mem+=0x200+0x112; // ���������� ��������� �� ���� ������ ���������� ����������� ������� + ����� ��������
	*p_pci_mem=state_dout;
}
