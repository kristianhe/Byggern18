#define led PORTB 

char arrA [18];
char arrB [18];
char arrC [6];
char arrD [6];
char arsendata [21];
volatile unsigned int freq, add, spenning=0;//frekvensen til buzzern, spenning til ADC



void buzzer (void){
	
	if (arrD[0] == '0'){
		freq = 0;
		TIMSK1=0x00; //Bit 1: (Output Compare A Match Interrupt Enable) - av
	}
	if (arrD[0] == '1'){
		add=(arrD[1]-'0')*1000+(arrD[2]-'0')*100+(arrD[3]-'0')*10+(arrD[4]-'0');
		freq = F_CPU/2/8/add; //hvis add = 1000: 14745600/2/8/1000 = 460 Hz pr nivå.
		TIMSK1=0x02; //Bit 1: (Output Compare A Match Interrupt Enable) - på
	}
}

void LED (void){
	volatile unsigned char i=0;
	
	if(arrC[0]=='1') i+=1; //lys led 1
	if(arrC[1]=='1') i+=2; //lys led 2
	if(arrC[2]=='1') i+=4; //lys led 3
	if(arrC[3]=='1') i+=8; //lys led 4
	
	led = (i<<4)|0x0f; // bitskifter med 4 for å få lysene på rette pinnene
}

void tekst (void){
arsendata[6] = 'B';
arsendata[7] = 'T';
arsendata[8] = 'o';
arsendata[9] = 'r';
arsendata[10] = 's';
arsendata[11] = 'k';
arsendata[12] = '\n';
}

void dipbryt (void){
	arsendata[13] = 'C';

	PORTB |= 0x0f;
	if (PINB &= 1) arsendata[14]=(1+'0'); else arsendata[14]='0';
	if (PINB &= 2) arsendata[15]=(1+'0'); else arsendata[15]='0';
	if (PINB &= 4) arsendata[16]=(1+'0'); else arsendata[16]='0';
	if (PINB &= 8) arsendata[17]=(1+'0'); else arsendata[17]='0'; //Pinnene ANDes slik at alle kombinasjoner av bryterene aksepteres.
	arsendata[18] = '\n';
	
}



void voltmtr (void){
	volatile unsigned char tall_1000, tall_100, tall_10, tall_1, tmp1, tmp2, tmp3;
	
	spenning = ((long) ADCW*5000/1023);
	//eks spenning = 4896
	
	tall_1000 = (spenning/1000)+'0'; //konverterer 1000 potensen og lagrer tallet i D_0 etter 'A', tall_1000 = 4
	
	tall_100 = spenning/100; //konverterer 100 potensen og lagrer tallet i D_1 etter 'A', tall_100 = 48
	tmp1 = tall_100%10; //tall_100 = 8
	tall_100 = tmp1+'0';
	
	tall_10 = spenning%100; //konverterer 10 potensen og lagrer tallet i D_2 etter 'A', tall_10 = 96
	tmp2 = tall_10/10; //tall_10 = 9
	tall_10 = tmp2+'0';
	
	tall_1 = spenning%100; //konverterer 1 potensen og lagrer tallet i D_3 etter 'A', tall_1 = 96
	tmp3 = tall_1%10; //tall_1 = 6
	tall_1 = tmp3+'0';
	//totalt tall_1000 = 4, tall_100 = 8, tall_10 = 9, tall_1 = 6
	
	arsendata[0] = 'A';
	arsendata[1] = tall_1000;
	arsendata[2] = tall_100;
	arsendata[3] = tall_10;
	arsendata[4] = tall_1;
	arsendata[5] = '\n';
	
}


void vribryt (void){
	
		char speil=0;
		speil += ((PINC&0b0001)<<3);		
		speil += ((PINC&0b0010)<<1);
		speil += ((PINC&0b0100)>>1);
		speil += ((PINC&0b1000)>>3);
		arsendata[19] = 'D';
		if(speil>9) arsendata[20]='A'+speil-10;	
		else arsendata[20] = '0'+speil;			
		arsendata[21]= 0x0A;
}