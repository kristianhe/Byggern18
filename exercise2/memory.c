#include "setup.h"
#include "memory.h"
#include <avr/io.h>

#define OFFSET 0x1000                     // Markerer staren på de eksterne minneadressene

void XRAM_memory_masking(void)            // Funksjonen muliggjør JTAG interface ved å maskere de fire MSBs fra PORTC
{
  unsigned char *p = (unsigned char *) (OFFSET + 1);

  pinC = 0x00;

  SFIOR = (1<<XMM1) | (1<<XMM0);
  *p = 0xAA;
  SFIOR = 0x00;
  *p = 0x55;
}
