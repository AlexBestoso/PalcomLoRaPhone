#include <cstdint>
#define DBG_PERF_SPI_0 0x3000 // yes, the memory is stored this way.
#define DBG_PERF_SPI_1 0x2000
#define DBG_PERF_SPI_2 0x24000
#define DBG_PERF_SPI_3 0x25000
#define DBG_PERF_SPI_BASE 0x60000000
#define DBG_PERF_READ(reg) (*(volatile uint32_t *)(DBG_PERF_SPI_BASE+reg))
#define DBG_PERF_WRITE(reg, val) ((*(volatile uint32_t *)(DBG_PERF_SPI_BASE+reg)) = (val))

#include "./spiDebug.structs.h"

/*
struct dbg_spi_reg_X{};
struct dbg_spi_reg_X{};
struct dbg_spi_reg_X{};
struct dbg_spi_reg_X{};
struct dbg_spi_reg_X{};
struct dbg_spi_reg_X{};
struct dbg_spi_reg_X{};
struct dbg_spi_reg_X{};
struct dbg_spi_reg_X{};
struct dbg_spi_reg_X{};
struct dbg_spi_reg_X{};
struct dbg_spi_reg_X{};
struct dbg_spi_reg_X{};
struct dbg_spi_reg_X{};
struct dbg_spi_reg_X{};
struct dbg_spi_reg_X{};
struct dbg_spi_reg_X{};
*/

class SpiDebug{
  private:
    
  
/*



CPU-controlled data buffer
SPI_W0_REG SPI CPU-controlled buffer0 0x0098 0x0098 R/W/SS
SPI_W1_REG SPI CPU-controlled buffer1 0x009C 0x009C R/W/SS
SPI_W2_REG SPI CPU-controlled buffer2 0x00A0 0x00A0 R/W/SS
SPI_W3_REG SPI CPU-controlled buffer3 0x00A4 0x00A4 R/W/SS
SPI_W4_REG SPI CPU-controlled buffer4 0x00A8 0x00A8 R/W/SS
SPI_W5_REG SPI CPU-controlled buffer5 0x00AC 0x00AC R/W/SS
SPI_W6_REG SPI CPU-controlled buffer6 0x00B0 0x00B0 R/W/SS
SPI_W7_REG SPI CPU-controlled buffer7 0x00B4 0x00B4 R/W/SS
SPI_W8_REG SPI CPU-controlled buffer8 0x00B8 0x00B8 R/W/SS
SPI_W9_REG SPI CPU-controlled buffer9 0x00BC 0x00BC R/W/SS
SPI_W10_REG SPI CPU-controlled buffer10 0x00C0 0x00C0 R/W/SS
SPI_W11_REG SPI CPU-controlled buffer11 0x00C4 0x00C4 R/W/SS
SPI_W12_REG SPI CPU-controlled buffer12 0x00C8 0x00C8 R/W/SS
SPI_W13_REG SPI CPU-controlled buffer13 0x00CC 0x00CC R/W/SS
SPI_W14_REG SPI CPU-controlled buffer14 0x00D0 0x00D0 R/W/SS
SPI_W15_REG SPI CPU-controlled buffer15 0x00D4 0x00D4 R/W/SS
Version register
SPI_DATE_REG Version control 0x00F0 0x00F0 R/W
  */

	void regWrite(uint32_t reg, uint32_t val);
	uint32_t regRead(uint32_t reg);

	// Helps isoltate fields within the 32 bit registers.
	bool regGet(uint32_t val, int pos);
	int regGet(uint32_t val, int x, int y);

	// Helps convert register structs into single register values.
	uint32_t regSet(uint32_t reg, bool val, int pos);
	uint32_t regSet(uint32_t reg, int val, int x, int y);

	bool modeValidate(void);

	public:
    /* User-defined control registers */
    struct dbg_spi_reg_cmd sr_cmd;
    struct dbg_spi_reg_addr sr_addr;
    struct dbg_spi_reg_user sr_user;
    struct dbg_spi_reg_user1 sr_user1;
    struct dbg_spi_reg_user2 sr_user2;

    /* Control and configuration registers */
    struct dbg_spi_reg_ctrl sr_ctrl;
    struct dbg_spi_reg_ms_dlen sr_msdlen;
    struct dbg_spi_reg_misc sr_misc;
    struct dbg_spi_reg_dma_conf sr_dmaconf;
    struct dbg_spi_reg_slave sr_slave;
    struct dbg_spi_reg_slave1 sr_slave1;
   
    /* Clock control registers */
    struct dbg_spi_reg_clock sr_clock;
    struct dbg_spi_reg_gate sr_gate;
  
    /* Timing registers */
    struct dbg_spi_reg_din_mode sr_dinmode;
    struct dbg_spi_reg_din_num sr_dinnum;
    struct dbg_spi_reg_dout_mode sr_doutmode;

    /* Interrupt registers */
    struct dbg_spi_reg_dma_int_ena sr_dmaintena;
    struct dbg_spi_reg_dma_int_clr sr_dmaintclr;
    struct dbg_spi_reg_dma_int_raw sr_dmaintraw;
    struct dbg_spi_reg_dma_int_st sr_dmaintst;
    struct dbg_spi_reg_dma_int_set sr_dmaintset;

    int32_t spiMode;
    
    SpiDebug(void);
    ~SpiDebug();
    
    bool setSpiMode(int mode);

    void initUserDefRegs(void);
    void init_sr_cmd(void);
    void init_sr_addr(void);
    void init_sr_user(void);
    void init_sr_user1(void);
    void init_sr_user2(void);

    void initCtrlConfRegs(void);
    void init_sr_ctrl(void);
    void init_sr_msdlen(void);
    void init_sr_misc(void);
    void init_sr_dmaconf(void);
    void init_sr_slave(void);
    void init_sr_slave1(void);

    void initClockRegs(void);
    void init_sr_clock(void);
    void init_sr_gate(void);

    void initTimingRegs(void);
    void init_sr_dinmode(void);
    void init_sr_dinnum(void);
    void init_sr_doutmode(void);

    void initInteruptRegs(void);
    void init_sr_dmaintena(void);
    void init_sr_dmaintclr(void);
    void init_sr_dmaintraw(void);
    void init_sr_dmaintst(void);
    void init_sr_dmaintset(void);

    // Identical functions, diff names for ease of use.
    void init(void);
    void refresh(void);

    // printout functions
    void printAll(bool onlyCoreVals);
    void printInterupts(bool onlyCoreVals);
    void printTimings(bool onlyCoreVals);
    void printClocks(bool onlyCoreVals);
    void printCtrlConfs(bool onlyCoreVals);
	void printUserDefs(bool onlyCoreVals);

    // register write functions
	void patchUserDef(bool writeMode);
	void patch_sr_cmd(bool writeMode);
    void patch_sr_addr(bool writeMode);
    void patch_sr_user(bool writeMode);
    void patch_sr_user1(bool writeMode);
    void patch_sr_user2(bool writeMode);

	void patchCtrlConf(bool writeMode);
	void patch_sr_ctrl(bool writeMode);
    void patch_sr_msdlen(bool writeMode);
    void patch_sr_misc(bool writeMode);
    void patch_sr_dmaconf(bool writeMode);
    void patch_sr_slave(bool writeMode);
    void patch_sr_slave1(bool writeMode);

	void patchClock(bool writeMode);
	void patch_sr_clock(bool writeMode);
    void patch_sr_gate(bool writeMode);

	void patchTiming(bool writeMode);
	void patch_sr_dinmode(bool writeMode);
    void patch_sr_dinnum(bool writeMode);
    void patch_sr_doutmode(bool writeMode);

	void patchInterupts(bool writeMode);
	void patch_sr_dmaintena(bool writeMode);
    void patch_sr_dmaintclr(bool writeMode);
    void patch_sr_dmaintraw(bool writeMode);
    void patch_sr_dmaintst(bool writeMode);
    void patch_sr_dmaintset(bool writeMode);

	void patchAll(bool writeMode);

	bool compUserDef(SpiDebug cmp, bool printout);
	bool comp_sr_cmd(SpiDebug cmp, bool printout);
    bool comp_sr_addr(SpiDebug cmp, bool printout);
    bool comp_sr_user(SpiDebug cmp, bool printout);
    bool comp_sr_user1(SpiDebug cmp, bool printout);
    bool comp_sr_user2(SpiDebug cmp, bool printout);

	bool compCtrlConf(SpiDebug cmp, bool printout);
	bool comp_sr_ctrl(SpiDebug cmp, bool printout);
    bool comp_sr_msdlen(SpiDebug cmp, bool printout);
    bool comp_sr_misc(SpiDebug cmp, bool printout);
    bool comp_sr_dmaconf(SpiDebug cmp, bool printout);
    bool comp_sr_slave(SpiDebug cmp, bool printout);
    bool comp_sr_slave1(SpiDebug cmp, bool printout);

	bool compClock(SpiDebug cmp, bool printout);
	bool comp_sr_clock(SpiDebug cmp, bool printout);
    bool comp_sr_gate(SpiDebug cmp, bool printout);

	bool compTiming(SpiDebug cmp, bool printout);
	bool comp_sr_dinmode(SpiDebug cmp, bool printout);
    bool comp_sr_dinnum(SpiDebug cmp, bool printout);
    bool comp_sr_doutmode(SpiDebug cmp, bool printout);

	bool compInterupts(SpiDebug cmp, bool printout);
	bool comp_sr_dmaintena(SpiDebug cmp, bool printout);
    bool comp_sr_dmaintclr(SpiDebug cmp, bool printout);
    bool comp_sr_dmaintraw(SpiDebug cmp, bool printout);
    bool comp_sr_dmaintst(SpiDebug cmp, bool printout);
    bool comp_sr_dmaintset(SpiDebug cmp, bool printout);

	bool compAll(SpiDebug cmp, bool printout);
	void printComp(const char *n1, int v1, int v2, const char *n2);
    // Helpful Functions
    bool isSlave(void);

};
