static const int CS = 4;
static const int CK = 5;
static const int mMOSI = 6;
static const int mMISO = 7;

void atmelbbio_setup(void);  // pins setup
uint32_t write_N_MSB_first(uint32_t data, int n);
uint8_t readEEByte(uint8_t addr);
void writeEEByte(uint8_t addr, uint8_t val, bool confim);
void ewen();
void ewds();


