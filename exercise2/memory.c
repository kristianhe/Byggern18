#include "memory.h"
#include <avr/io.h>

void XRAM_memory_masking(void)            // Funksjonen muliggjør JTAG interface ved å maskere de fire MSBs fra PORTC
{
  unsigned char *p = (unsigned char *) (OFFSET + 1);

  DDRC = 0xFF;
  PORTC = 0x00;

  SFIOR = (1<<XMM1) | (1<<XMM0);
  *p = 0xaa;
  SFIOR = 0x00;
  *p = 0x55;
}
