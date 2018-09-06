
// Setter opp oscillatoren, som da setter opp CPU-frekvensen. + Baudrate og UBRR
#define F_OSC 4915200
#define F_CPU F_OSC
#define BAUD 9600
#define MYUBRR (F_OSC-250)/16/BAUD-1		// ER DETTE KORREKT?????
//#define MYUBRR ((F_OSC)/(16*BAUD))-1		// HVA MED DETTE???????

// Nyttige forenklinger. Funnet i labmanualen. Forenkler det � sette bits i registere.
#define set_bit(reg,bit) (reg |= (1 << bit))
#define clear_bit(reg,bit) (reg &= ~(1 << bit))
#define test_bit(reg,bit) (reg & (1 << bit))
