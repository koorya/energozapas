#define K_STURTUP_EXT_OSC_FREQ 10000000 //10MHz
#define K_STURTUP_INT_OSC_FREQ 8000000 //8MHz

typedef struct{
	uint32_t K_SYSCLK_FREQ; 
	uint32_t K_AHB_FREQ; 
	uint32_t K_APB_FREQ; 
} K_CLOC_CONTROL;	



typedef enum {
	mcoPLLCLK_DIV2,
	mcoHSI,
	mcoHSE,
	mcoSYS
} K_MCOSource;

typedef enum{
	cHSI,
	cHSE
} K_SOURCE;

extern uint32_t volatile K_EXT_OSC_FREQ;
extern uint32_t volatile K_INT_OSC_FREQ;
extern K_CLOC_CONTROL volatile K_CLOCK_CONTROL_STRUCT;



void K_updatePeriphFreq(void);

void K_SetExtClockPLL(char mult, char div);										

void K_SetExtClockDirect(void);

void K_SetIntClockDirect(void);

void K_SetIntClockPLL(char freq);

void K_initMCOoutput(K_MCOSource source);

void K_setClock(K_SOURCE source,
	char presc, //1-16; если mult == 1, то presc=1, иначе но если source == cHSI, то presc=2
	char kPLL, //1-16 (если mult==1, то умножитель не используется)
	char ahbpresc, // 1, 2, 4, 8, 16, (not 32) 64, 128, 256, 512
	char apbpresc, // 1, 2, 4, 8, 16
	char enable_mco);
	
