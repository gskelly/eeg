#define ADS1299_REG_ID			0x0000			// Read-only
#define ADS1299_REG_CONFIG1		0x0001
#define ADS1299_REG_CONFIG2		0x0002
#define ADS1299_REG_CONFIG3		0x0003
#define ADS1299_REG_LOFF		0x0004
#define ADS1299_REG_CH1SET		0x0005
#define ADS1299_REG_CH2SET		0x0006
#define ADS1299_REG_CH3SET		0x0007
#define ADS1299_REG_CH4SET		0x0008
#define ADS1299_REG_CH5SET		0x0009
#define ADS1299_REG_CH6SET		0x000A
#define ADS1299_REG_CH7SET		0x000B
#define ADS1299_REG_CH8SET		0x000C
#define ADS1299_REG_BIAS_SENSP	0x000D
#define ADS1299_REG_BIAS_SENSN	0x000E
#define ADS1299_REG_LOFF_SENSP	0x000F
#define ADS1299_REG_LOFF_SENSN	0x0010
#define ADS1299_REG_LOFF_FLIP	0x0011
#define ADS1299_REG_LOFF_STATP	0x0012			// Read-only
#define ADS1299_REG_LOFF_STATN	0x0013			// Read-only
#define ADS1299_REG_GPIO		0x0014
#define ADS1299_REG_MISC1		0x0015
#define ADS1299_REG_MISC2		0x0016
#define ADS1299_REG_CONFIG4		0x0017

/* OPCODES */
#define ADS1299_OPC_WAKEUP		0x0002			// Wake up from standby
#define ADS1299_OPC_STANDBY		0x0004			// Enter standby
#define ADS1299_OPC_RESET		0x0006			// Reset all registers
#define ADS1299_OPC_START		0x0008			// Start data conversions
#define ADS1299_OPC_STOP		0x000A			// Stop data conversions
#define ADS1299_OPC_RDATAC		0x0010			// Read data continuously
// Registers cannot be r/w in this mode
#define ADS1299_OPC_SDATAC		0x0011			// Stop continuous data read
#define ADS1299_OPC_RDATA		0x0012			// Read single data value

// For RREG and WREG opcodes:
// This first byte (opcode) must be ORed with the register address (name).
// The command is completed with a second byte 000n nnnn, where n nnnn
// is (# registers to read) - 1.
#define ADS1299_OPC_RREG		0x0020			// Read register value
#define ADS1299_OPC_WREG		0x0040			// Write register value