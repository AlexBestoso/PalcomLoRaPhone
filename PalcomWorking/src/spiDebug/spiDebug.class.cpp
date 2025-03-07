#include "./spiDebug.class.h"
#include <Arduino.h>
/* (D/C)tors */
SpiDebug::SpiDebug() {
  this->setSpiMode(2);
}

SpiDebug::~SpiDebug() {
}

/* Private */

void SpiDebug::regWrite(uint32_t reg, uint32_t val) {
  reg = this->spiMode + reg;
  DBG_PERF_WRITE(reg, val);
}

uint32_t SpiDebug::regRead(uint32_t reg) {
  reg = this->spiMode + reg;
  return DBG_PERF_READ(reg);
}

bool SpiDebug::regGet(uint32_t val, int pos) {
  int grabber = 1 << pos;
  int result = (val & grabber) >> pos;
  return result == 1 ? true : false;
}

int SpiDebug::regGet(uint32_t val, int x, int y) {
  int retSize = (y - x) + 1;
  int grabber = 0;
  for (int i = 0; i < retSize; i++)
    grabber = (grabber << 1) + 1;

  int result = (val & (grabber << x)) >> x;
  return result;
}


uint32_t SpiDebug::regSet(uint32_t reg, bool val, int pos){
	int v = val ? 1 : 0;
	reg &= ~(1<<pos);
	reg += (v<<pos);
	return reg;
}
uint32_t SpiDebug::regSet(uint32_t reg, int val, int x, int y){
	int s = y - x;
	int g = 0;
	for(int i=0; i<s; i++)
		g = (g << 1) + 1;
	
	val &= g;
	reg &= ~(1<<x);
	reg += (val << x);
	return reg;
}


/* Public */

/// Sets spiMode to negative 1 on error.
bool SpiDebug::setSpiMode(int mode){
  switch(mode){
    case 0:
      this->spiMode = DBG_PERF_SPI_0;
      return true;
    case 1:
      this->spiMode = DBG_PERF_SPI_1;
      return true;
    case 2:
      this->spiMode = DBG_PERF_SPI_2;
      return true;
    case 3:
      this->spiMode = DBG_PERF_SPI_3;
      return true;
    default:
      this->spiMode = DBG_PERF_SPI_2; // SPI 0 is used for some critical stuff.
      return false;
  }
}

void SpiDebug::init_sr_cmd(void) {
  uint32_t base = this->sr_cmd.base_2;
  this->sr_cmd.val = this->regRead(base);

  this->sr_cmd.spi_user = this->regGet(this->sr_cmd.val, this->sr_cmd.spi_user_pos);
  this->sr_cmd.spi_update = this->regGet(this->sr_cmd.val, this->sr_cmd.spi_update_pos);
}

void SpiDebug::init_sr_addr(void) {
  uint32_t base = this->sr_addr.base_2;
  this->sr_addr.val = this->regRead(base);
  this->sr_addr.spi_usr_addr_value = this->sr_addr.val;
}

void SpiDebug::init_sr_user(void) {
  uint32_t base = this->sr_user.base_2;
  this->sr_user.val = this->regRead(base);
  this->sr_user.spi_usr_command = this->regGet(this->sr_user.val, this->sr_user.spi_usr_command_pos);
  this->sr_user.spi_usr_addr = this->regGet(this->sr_user.val, this->sr_user.spi_usr_addr_pos);
  this->sr_user.spi_usr_dummy = this->regGet(this->sr_user.val, this->sr_user.spi_usr_dummy_pos);
  this->sr_user.spi_usr_miso = this->regGet(this->sr_user.val, this->sr_user.spi_usr_miso_pos);
  this->sr_user.spi_usr_mosi = this->regGet(this->sr_user.val, this->sr_user.spi_usr_mosi_pos);
  this->sr_user.spi_usr_dummy_idle = this->regGet(this->sr_user.val, this->sr_user.spi_usr_dummy_idle_pos);
  this->sr_user.spi_usr_mosi_highpart = this->regGet(this->sr_user.val, this->sr_user.spi_usr_mosi_highpart_pos);
  this->sr_user.spi_usr_miso_highpart = this->regGet(this->sr_user.val, this->sr_user.spi_usr_miso_highpart_pos);
  this->sr_user.spi_sio = this->regGet(this->sr_user.val, this->sr_user.spi_sio_pos);
  this->sr_user.r_spi_usr_conf_nxt = this->regGet(this->sr_user.val, this->sr_user.r_spi_usr_conf_nxt_pos);
  this->sr_user.r_spi_fwrite_oct = this->regGet(this->sr_user.val, this->sr_user.r_spi_fwrite_oct_pos);
  this->sr_user.spi_fwrite_quad = this->regGet(this->sr_user.val, this->sr_user.spi_fwrite_quad_pos);
  this->sr_user.spi_fwrite_dual = this->regGet(this->sr_user.val, this->sr_user.spi_fwrite_dual_pos);
  this->sr_user.spi_usr_ck_out_edge = this->regGet(this->sr_user.val, this->sr_user.spi_usr_ck_out_edge_pos);
  this->sr_user.spi_rsck_i_edge = this->regGet(this->sr_user.val, this->sr_user.spi_rsck_i_edge_pos);
  this->sr_user.spi_cs_setup = this->regGet(this->sr_user.val, this->sr_user.spi_cs_setup_pos);
  this->sr_user.spi_cs_hold = this->regGet(this->sr_user.val, this->sr_user.spi_cs_hold_pos);
  this->sr_user.spi_tsck_i_edge = this->regGet(this->sr_user.val, this->sr_user.spi_tsck_i_edge_pos);
  this->sr_user.r_spi_opi_mode = this->regGet(this->sr_user.val, this->sr_user.r_spi_opi_mode_pos);
  this->sr_user.spi_qpi_mode = this->regGet(this->sr_user.val, this->sr_user.spi_qpi_mode_pos);
  this->sr_user.spi_doutdin = this->regGet(this->sr_user.val, this->sr_user.spi_doutdin_pos);
}

void SpiDebug::init_sr_user1(void) {
  uint32_t base = this->sr_user1.base_2;
  this->sr_user1.val = this->regRead(base);

  this->sr_user1.spi_usr_addr_bitlen = this->regGet(this->sr_user1.val,
                                                    this->sr_user1.spi_usr_addr_bitlen_x,
                                                    this->sr_user1.spi_usr_addr_bitlen_y);
  this->sr_user1.spi_cs_hold_time = this->regGet(this->sr_user1.val,
                                                 this->sr_user1.spi_cs_hold_time_x,
                                                 this->sr_user1.spi_cs_hold_time_y);
  this->sr_user1.spi_cs_setup_time = this->regGet(this->sr_user1.val,
                                                  this->sr_user1.spi_cs_setup_time_x,
                                                  this->sr_user1.spi_cs_setup_time_y);
  this->sr_user1.spi_mst_wfull_err_end_en = this->regGet(this->sr_user1.val, this->sr_user1.spi_mst_wfull_err_end_en_pos);
  this->sr_user1.spi_usr_dummy_cyclelen = this->regGet(this->sr_user1.val,
                                                       this->sr_user1.spi_usr_dummy_cyclelen_x,
                                                       this->sr_user1.spi_usr_dummy_cyclelen_y);
}

void SpiDebug::init_sr_user2(void) {
  uint32_t base = this->sr_user2.base_2;
  this->sr_user2.val = this->regRead(base);

  this->sr_user2.spi_usr_command_bitlen = this->regGet(this->sr_user2.val,
                                                       this->sr_user2.spi_usr_command_bitlen_x,
                                                       this->sr_user2.spi_usr_command_bitlen_y);

  this->sr_user2.spi_mst_rempty_err_end_en = this->regGet(this->sr_user2.val, this->sr_user2.spi_mst_rempty_err_end_en_pos);

  this->sr_user2.spi_usr_command_value = this->regGet(this->sr_user2.val,
                                                      this->sr_user2.spi_usr_command_value_x,
                                                      this->sr_user2.spi_usr_command_value_y);
}

void SpiDebug::initUserDefRegs() {
  this->init_sr_cmd();
  this->init_sr_addr();
  this->init_sr_user();
  this->init_sr_user1();
  this->init_sr_user2();
}

void SpiDebug::init_sr_ctrl(void) {
  uint32_t base = this->sr_ctrl.base_2;
  this->sr_ctrl.val = this->regRead(base);

  this->sr_ctrl.spi_wr_bit_order = this->regGet(this->sr_ctrl.val,
                                                this->sr_ctrl.spi_wr_bit_order_x,
                                                this->sr_ctrl.spi_wr_bit_order_y);

  this->sr_ctrl.spi_rd_bit_order = this->regGet(this->sr_ctrl.val,
                                                this->sr_ctrl.spi_rd_bit_order_x,
                                                this->sr_ctrl.spi_rd_bit_order_y);

  this->sr_ctrl.spi_wp_pol = this->regGet(this->sr_ctrl.val, this->sr_ctrl.spi_wp_pol_pos);
  this->sr_ctrl.spi_hold_pol = this->regGet(this->sr_ctrl.val, this->sr_ctrl.spi_hold_pol_pos);
  this->sr_ctrl.spi_d_pol = this->regGet(this->sr_ctrl.val, this->sr_ctrl.spi_d_pol_pos);
  this->sr_ctrl.spi_q_pol = this->regGet(this->sr_ctrl.val, this->sr_ctrl.spi_q_pol_pos);
  this->sr_ctrl.r_spi_fread_oct = this->regGet(this->sr_ctrl.val, this->sr_ctrl.r_spi_fread_oct_pos);
  this->sr_ctrl.spi_fread_quad = this->regGet(this->sr_ctrl.val, this->sr_ctrl.spi_fread_quad_pos);
  this->sr_ctrl.spi_fread_dual = this->regGet(this->sr_ctrl.val, this->sr_ctrl.spi_fread_dual_pos);
  this->sr_ctrl.r_spi_fcmd_oct = this->regGet(this->sr_ctrl.val, this->sr_ctrl.r_spi_fcmd_oct_pos);
  this->sr_ctrl.spi_fcmd_quad = this->regGet(this->sr_ctrl.val, this->sr_ctrl.spi_fcmd_quad_pos);
  this->sr_ctrl.spi_fcmd_dual = this->regGet(this->sr_ctrl.val, this->sr_ctrl.spi_fcmd_dual_pos);
  this->sr_ctrl.r_spi_faddr_oct = this->regGet(this->sr_ctrl.val, this->sr_ctrl.r_spi_faddr_oct_pos);
  this->sr_ctrl.spi_faddr_quad = this->regGet(this->sr_ctrl.val, this->sr_ctrl.spi_faddr_quad_pos);
  this->sr_ctrl.spi_faddr_dual = this->regGet(this->sr_ctrl.val, this->sr_ctrl.spi_faddr_dual_pos);
  this->sr_ctrl.spi_dummy_out = this->regGet(this->sr_ctrl.val, this->sr_ctrl.spi_dummy_out_pos);
}

void SpiDebug::init_sr_msdlen(void) {
  uint32_t base = this->sr_msdlen.base_2;
  this->sr_msdlen.val = this->regRead(base);

  this->sr_msdlen.spi_ms_data_bitlen = this->regGet(this->sr_msdlen.val,
                                                    this->sr_msdlen.spi_ms_data_bitlen_x,
                                                    this->sr_msdlen.spi_ms_data_bitlen_y);
}

void SpiDebug::init_sr_misc(void) {
  uint32_t base = this->spiMode == 2 ? this->sr_misc.base_2 : this->sr_misc.base_3;
  this->sr_misc.val = this->regRead(base);

  this->sr_misc.spi_quad_din_pin_swap = this->regGet(this->sr_misc.val, this->sr_misc.spi_quad_din_pin_swap_pos);
  this->sr_misc.spi_cs_keep_active = this->regGet(this->sr_misc.val, this->sr_misc.spi_cs_keep_active_pos);
  this->sr_misc.spi_ck_idle_edge = this->regGet(this->sr_misc.val, this->sr_misc.spi_ck_idle_edge_pos);
  this->sr_misc.spi_dqs_idle_edge = this->regGet(this->sr_misc.val, this->sr_misc.spi_dqs_idle_edge_pos);
  this->sr_misc.spi_slave_cs_pol = this->regGet(this->sr_misc.val, this->sr_misc.spi_slave_cs_pol_pos);
  this->sr_misc.spi_cmd_dtr_en = this->regGet(this->sr_misc.val, this->sr_misc.spi_cmd_dtr_en_pos);
  this->sr_misc.spi_addr_dtr_en = this->regGet(this->sr_misc.val, this->sr_misc.spi_addr_dtr_en_pos);
  this->sr_misc.spi_data_dtr_en = this->regGet(this->sr_misc.val, this->sr_misc.spi_data_dtr_en_pos);
  this->sr_misc.spi_clk_data_dtr_en = this->regGet(this->sr_misc.val, this->sr_misc.spi_clk_data_dtr_en_pos);

  this->sr_misc.spi_master_cs_pol = this->regGet(this->sr_misc.val,
                                                 this->sr_misc.spi_master_cs_pol_x,
                                                  this->spiMode == 2 ? this->sr_misc.spi_master_cs_pol_y : this->sr_misc.spi_master_cs_pol_y3);

  this->sr_misc.spi_ck_dis = this->regGet(this->sr_misc.val, this->sr_misc.spi_ck_dis_pos);
  this->sr_misc.spi_cs5_dis = this->regGet(this->sr_misc.val, this->sr_misc.spi_cs5_dis_pos);
  this->sr_misc.spi_cs4_dis = this->regGet(this->sr_misc.val, this->sr_misc.spi_cs4_dis_pos);
  this->sr_misc.spi_cs3_dis = this->regGet(this->sr_misc.val, this->sr_misc.spi_cs3_dis_pos);
  this->sr_misc.spi_cs2_dis = this->regGet(this->sr_misc.val, this->sr_misc.spi_cs2_dis_pos);
  this->sr_misc.spi_cs1_dis = this->regGet(this->sr_misc.val, this->sr_misc.spi_cs1_dis_pos);
  this->sr_misc.spi_cs0_dis = this->regGet(this->sr_misc.val, this->sr_misc.spi_cs0_dis_pos);
}

void SpiDebug::init_sr_dmaconf(void) {
  uint32_t base = this->sr_dmaconf.base_2;
  this->sr_dmaconf.val = this->regRead(base);

  this->sr_dmaconf.spi_dma_afifo_rst = this->regGet(this->sr_dmaconf.val, this->sr_dmaconf.spi_dma_afifo_rst_pos);
  this->sr_dmaconf.spi_buf_afifo_rst = this->regGet(this->sr_dmaconf.val, this->sr_dmaconf.spi_buf_afifo_rst_pos);
  this->sr_dmaconf.spi_rx_afifo_rst = this->regGet(this->sr_dmaconf.val, this->sr_dmaconf.spi_rx_afifo_rst_pos);
  this->sr_dmaconf.spi_dma_tx_ena = this->regGet(this->sr_dmaconf.val, this->sr_dmaconf.spi_dma_tx_ena_pos);
  this->sr_dmaconf.spi_dma_rx_ena = this->regGet(this->sr_dmaconf.val, this->sr_dmaconf.spi_dma_rx_ena_pos);
  this->sr_dmaconf.spi_rx_eof_en = this->regGet(this->sr_dmaconf.val, this->sr_dmaconf.spi_rx_eof_en_pos);
  this->sr_dmaconf.spi_slv_tx_seg_trans_clr_en = this->regGet(this->sr_dmaconf.val, this->sr_dmaconf.spi_slv_tx_seg_trans_clr_en_pos);
  this->sr_dmaconf.spi_slv_rx_seg_trans_clr_en = this->regGet(this->sr_dmaconf.val, this->sr_dmaconf.spi_slv_rx_seg_trans_clr_en_pos);
  this->sr_dmaconf.spi_dma_slv_seg_trans_en = this->regGet(this->sr_dmaconf.val, this->sr_dmaconf.spi_dma_slv_seg_trans_en_pos);
  this->sr_dmaconf.spi_dma_infifo_full = this->regGet(this->sr_dmaconf.val, this->sr_dmaconf.spi_dma_infifo_full_pos);
  this->sr_dmaconf.spi_dma_outfifo_empty = this->regGet(this->sr_dmaconf.val, this->sr_dmaconf.spi_dma_outfifo_empty_pos);
}

void SpiDebug::init_sr_slave(void) {
  uint32_t base = this->sr_slave.base_2;
  this->sr_slave.val = this->regRead(base);

  this->sr_slave.r_spi_usr_conf = this->regGet(this->sr_slave.val, this->sr_slave.r_spi_usr_conf_pos);
  this->sr_slave.spi_soft_reset = this->regGet(this->sr_slave.val, this->sr_slave.spi_soft_reset_pos);
  this->sr_slave.spi_slave_mode = this->regGet(this->sr_slave.val, this->sr_slave.spi_slave_mode_pos);

  this->sr_slave.r_spi_dma_seg_magic_value = this->regGet(this->sr_slave.val,
                                                          this->sr_slave.r_spi_dma_seg_magic_value_x,
                                                          this->sr_slave.r_spi_dma_seg_magic_value_y);

  this->sr_slave.spi_slv_wrbuf_bitlen_en = this->regGet(this->sr_slave.val, this->sr_slave.spi_slv_wrbuf_bitlen_en_pos);
  this->sr_slave.spi_slv_rdbuf_bitlen_en = this->regGet(this->sr_slave.val, this->sr_slave.spi_slv_rdbuf_bitlen_en_pos);
  this->sr_slave.spi_slv_wrdma_bitlen_en = this->regGet(this->sr_slave.val, this->sr_slave.spi_slv_wrdma_bitlen_en_pos);
  this->sr_slave.spi_slv_rddma_bitlen_en = this->regGet(this->sr_slave.val, this->sr_slave.spi_slv_rddma_bitlen_en_pos);
  this->sr_slave.spi_rsck_data_out = this->regGet(this->sr_slave.val, this->sr_slave.spi_rsck_data_out_pos);
  this->sr_slave.spi_clk_mode_13 = this->regGet(this->sr_slave.val, this->sr_slave.spi_clk_mode_13_pos);

  this->sr_slave.spi_clk_mode = this->regGet(this->sr_slave.val,
                                             this->sr_slave.spi_clk_mode_x,
                                             this->sr_slave.spi_clk_mode_y);
}

void SpiDebug::init_sr_slave1(void) {
  uint32_t base = this->sr_slave1.base_2;
  this->sr_slave1.val = this->regRead(base);

  this->sr_slave1.spi_slv_last_addr = this->regGet(this->sr_slave1.val,
                                                   this->sr_slave1.spi_slv_last_addr_x,
                                                   this->sr_slave1.spi_slv_last_addr_y);

  this->sr_slave1.spi_slv_last_command = this->regGet(this->sr_slave1.val,
                                                      this->sr_slave1.spi_slv_last_command_x,
                                                      this->sr_slave1.spi_slv_last_command_y);

  this->sr_slave1.spi_slv_data_bitlen = this->regGet(this->sr_slave1.val,
                                                     this->sr_slave1.spi_slv_data_bitlen_x,
                                                     this->sr_slave1.spi_slv_data_bitlen_y);
}

void SpiDebug::initCtrlConfRegs(void) {
  this->init_sr_ctrl();
  this->init_sr_msdlen();
  this->init_sr_misc();
  this->init_sr_dmaconf();
  this->init_sr_slave();
  this->init_sr_slave1();
}

void SpiDebug::init_sr_clock(void) {
  uint32_t base = this->sr_clock.base_2;
  this->sr_clock.val = this->regRead(base);

  this->sr_clock.spi_clk_equ_sysclk = this->regGet(this->sr_clock.val, this->sr_clock.spi_clk_equ_sysclk_pos);

  this->sr_clock.spi_clkdiv_pre = this->regGet(this->sr_clock.val,
                                               this->sr_clock.spi_clkdiv_pre_x,
                                               this->sr_clock.spi_clkdiv_pre_y);

  this->sr_clock.spi_clkcnt_n = this->regGet(this->sr_clock.val,
                                             this->sr_clock.spi_clkcnt_n_x,
                                             this->sr_clock.spi_clkcnt_n_y);

  this->sr_clock.spi_clkcnt_h = this->regGet(this->sr_clock.val,
                                             this->sr_clock.spi_clkcnt_h_x,
                                             this->sr_clock.spi_clkcnt_h_y);

  this->sr_clock.spi_clkcnt_l = this->regGet(this->sr_clock.val,
                                             this->sr_clock.spi_clkcnt_l_x,
                                             this->sr_clock.spi_clkcnt_l_y);
}

void SpiDebug::init_sr_gate(void) {
  uint32_t base = this->sr_gate.base_2;
  this->sr_gate.val = this->regRead(base);

  this->sr_gate.spi_mst_clk_sel = this->regGet(this->sr_gate.val, this->sr_gate.spi_mst_clk_sel_pos);
  this->sr_gate.spi_mst_clk_active = this->regGet(this->sr_gate.val, this->sr_gate.spi_mst_clk_active_pos);
  this->sr_gate.spi_clk_en = this->regGet(this->sr_gate.val, this->sr_gate.spi_clk_en_pos);
}

void SpiDebug::initClockRegs(void) {
  this->init_sr_clock();
  this->init_sr_gate();
}

void SpiDebug::init_sr_dinmode(void) {
  uint32_t base = this->sr_dinmode.base_2;
  this->sr_dinmode.val = this->regRead(base);

  this->sr_dinmode.spi_timing_hclk_active = this->regGet(this->sr_dinmode.val, this->sr_dinmode.spi_timing_hclk_active_pos);

  this->sr_dinmode.r_spi_din7_mode = this->regGet(this->sr_dinmode.val,
                                                  this->sr_dinmode.r_spi_din7_mode_x,
                                                  this->sr_dinmode.r_spi_din7_mode_y);

  this->sr_dinmode.r_spi_din6_mode = this->regGet(this->sr_dinmode.val,
                                                  this->sr_dinmode.r_spi_din6_mode_x,
                                                  this->sr_dinmode.r_spi_din6_mode_y);

  this->sr_dinmode.r_spi_din5_mode = this->regGet(this->sr_dinmode.val,
                                                  this->sr_dinmode.r_spi_din5_mode_x,
                                                  this->sr_dinmode.r_spi_din5_mode_y);

  this->sr_dinmode.r_spi_din4_mode = this->regGet(this->sr_dinmode.val,
                                                  this->sr_dinmode.r_spi_din4_mode_x,
                                                  this->sr_dinmode.r_spi_din4_mode_y);

  this->sr_dinmode.spi_din3_mode = this->regGet(this->sr_dinmode.val,
                                                this->sr_dinmode.spi_din3_mode_x,
                                                this->sr_dinmode.spi_din3_mode_y);

  this->sr_dinmode.spi_din2_mode = this->regGet(this->sr_dinmode.val,
                                                this->sr_dinmode.spi_din2_mode_x,
                                                this->sr_dinmode.spi_din2_mode_y);

  this->sr_dinmode.spi_din1_mode = this->regGet(this->sr_dinmode.val,
                                                this->sr_dinmode.spi_din1_mode_x,
                                                this->sr_dinmode.spi_din1_mode_y);

  this->sr_dinmode.spi_din0_mode = this->regGet(this->sr_dinmode.val,
                                                this->sr_dinmode.spi_din0_mode_x,
                                                this->sr_dinmode.spi_din0_mode_y);
}

void SpiDebug::init_sr_dinnum(void) {
  uint32_t base = this->sr_dinnum.base_2;
  this->sr_dinnum.val = this->regRead(base);

  this->sr_dinnum.r_spi_din7_num = this->regGet(this->sr_dinmode.val,
                                                this->sr_dinnum.r_spi_din7_num_x,
                                                this->sr_dinnum.r_spi_din7_num_y);

  this->sr_dinnum.r_spi_din6_num = this->regGet(this->sr_dinmode.val,
                                                this->sr_dinnum.r_spi_din6_num_x,
                                                this->sr_dinnum.r_spi_din6_num_y);

  this->sr_dinnum.r_spi_din5_num = this->regGet(this->sr_dinmode.val,
                                                this->sr_dinnum.r_spi_din5_num_x,
                                                this->sr_dinnum.r_spi_din5_num_y);

  this->sr_dinnum.r_spi_din4_num = this->regGet(this->sr_dinmode.val,
                                                this->sr_dinnum.r_spi_din4_num_x,
                                                this->sr_dinnum.r_spi_din4_num_y);

  this->sr_dinnum.spi_din3_num = this->regGet(this->sr_dinmode.val,
                                              this->sr_dinnum.spi_din3_num_x,
                                              this->sr_dinnum.spi_din3_num_y);

  this->sr_dinnum.spi_din2_num = this->regGet(this->sr_dinmode.val,
                                              this->sr_dinnum.spi_din2_num_x,
                                              this->sr_dinnum.spi_din2_num_y);

  this->sr_dinnum.spi_din1_num = this->regGet(this->sr_dinmode.val,
                                              this->sr_dinnum.spi_din1_num_x,
                                              this->sr_dinnum.spi_din1_num_y);

  this->sr_dinnum.spi_din0_num = this->regGet(this->sr_dinmode.val,
                                              this->sr_dinnum.spi_din0_num_x,
                                              this->sr_dinnum.spi_din0_num_y);
}

void SpiDebug::init_sr_doutmode(void) {
  uint32_t base = this->sr_doutmode.base_2;
  this->sr_doutmode.val = this->regRead(base);

  this->sr_doutmode.r_spi_d_dqs_mode = this->regGet(this->sr_doutmode.val, this->sr_doutmode.r_spi_d_dqs_mode_pos);
  this->sr_doutmode.r_spi_dout7_mode = this->regGet(this->sr_doutmode.val, this->sr_doutmode.r_spi_dout7_mode_pos);
  this->sr_doutmode.r_spi_dout6_mode = this->regGet(this->sr_doutmode.val, this->sr_doutmode.r_spi_dout6_mode_pos);
  this->sr_doutmode.r_spi_dout5_mode = this->regGet(this->sr_doutmode.val, this->sr_doutmode.r_spi_dout5_mode_pos);
  this->sr_doutmode.r_spi_dout4_mode = this->regGet(this->sr_doutmode.val, this->sr_doutmode.r_spi_dout4_mode_pos);
  this->sr_doutmode.spi_dout3_mode = this->regGet(this->sr_doutmode.val, this->sr_doutmode.spi_dout3_mode_pos);
  this->sr_doutmode.spi_dout2_mode = this->regGet(this->sr_doutmode.val, this->sr_doutmode.spi_dout2_mode_pos);
  this->sr_doutmode.spi_dout1_mode = this->regGet(this->sr_doutmode.val, this->sr_doutmode.spi_dout1_mode_pos);
  this->sr_doutmode.spi_dout0_mode = this->regGet(this->sr_doutmode.val, this->sr_doutmode.spi_dout0_mode_pos);
}

void SpiDebug::initTimingRegs(void) {
  this->init_sr_dinmode();
  this->init_sr_dinnum();
  this->init_sr_doutmode();
}

void SpiDebug::init_sr_dmaintena(void) {
  uint32_t base = this->sr_dmaintena.base_2;
  this->sr_dmaintena.val = this->regRead(base);

  this->sr_dmaintena.spi_app1_int_ena = this->regGet(this->sr_dmaintena.val, this->sr_dmaintena.spi_app1_int_ena_pos);
  this->sr_dmaintena.spi_app2_int_ena = this->regGet(this->sr_dmaintena.val, this->sr_dmaintena.spi_app2_int_ena_pos);
  this->sr_dmaintena.spi_mst_tx_afifo_rempty_err_int_ena = this->regGet(this->sr_dmaintena.val, this->sr_dmaintena.spi_mst_tx_afifo_rempty_err_int_ena_pos);
  this->sr_dmaintena.spi_mst_rx_afifo_wfull_err_int_ena = this->regGet(this->sr_dmaintena.val, this->sr_dmaintena.spi_mst_rx_afifo_wfull_err_int_ena_pos);
  this->sr_dmaintena.spi_slv_cmd_err_int_ena = this->regGet(this->sr_dmaintena.val, this->sr_dmaintena.spi_slv_cmd_err_int_ena_pos);
  this->sr_dmaintena.r_spi_seg_magic_err_int_ena = this->regGet(this->sr_dmaintena.val, this->sr_dmaintena.r_spi_seg_magic_err_int_ena_pos);
  this->sr_dmaintena.spi_dma_seg_trans_done_int_ena = this->regGet(this->sr_dmaintena.val, this->sr_dmaintena.spi_dma_seg_trans_done_int_ena_pos);
  this->sr_dmaintena.spi_trans_done_int_ena = this->regGet(this->sr_dmaintena.val, this->sr_dmaintena.spi_trans_done_int_ena_pos);
  this->sr_dmaintena.spi_slv_wr_buf_done_int_ena = this->regGet(this->sr_dmaintena.val, this->sr_dmaintena.spi_slv_wr_buf_done_int_ena_pos);
  this->sr_dmaintena.spi_slv_rd_buf_done_int_ena = this->regGet(this->sr_dmaintena.val, this->sr_dmaintena.spi_slv_rd_buf_done_int_ena_pos);
  this->sr_dmaintena.spi_slv_wr_dma_done_int_ena = this->regGet(this->sr_dmaintena.val, this->sr_dmaintena.spi_slv_wr_dma_done_int_ena_pos);
  this->sr_dmaintena.spi_slv_rd_dma_done_int_ena = this->regGet(this->sr_dmaintena.val, this->sr_dmaintena.spi_slv_rd_dma_done_int_ena_pos);
  this->sr_dmaintena.spi_slv_cmda_int_ena = this->regGet(this->sr_dmaintena.val, this->sr_dmaintena.spi_slv_cmda_int_ena_pos);
  this->sr_dmaintena.spi_slv_cmd9_int_ena = this->regGet(this->sr_dmaintena.val, this->sr_dmaintena.spi_slv_cmd9_int_ena_pos);
  this->sr_dmaintena.spi_slv_cmd8_int_ena = this->regGet(this->sr_dmaintena.val, this->sr_dmaintena.spi_slv_cmd8_int_ena_pos);
  this->sr_dmaintena.spi_slv_cmd7_int_ena = this->regGet(this->sr_dmaintena.val, this->sr_dmaintena.spi_slv_cmd7_int_ena_pos);
  this->sr_dmaintena.spi_slv_en_qpi_int_ena = this->regGet(this->sr_dmaintena.val, this->sr_dmaintena.spi_slv_en_qpi_int_ena_pos);
  this->sr_dmaintena.spi_slv_ex_qpi_int_ena = this->regGet(this->sr_dmaintena.val, this->sr_dmaintena.spi_slv_ex_qpi_int_ena_pos);
  this->sr_dmaintena.spi_dma_outfifo_empty_err_int_ena = this->regGet(this->sr_dmaintena.val, this->sr_dmaintena.spi_dma_outfifo_empty_err_int_ena_pos);
  this->sr_dmaintena.spi_dma_infifo_full_err_int_ena = this->regGet(this->sr_dmaintena.val, this->sr_dmaintena.spi_dma_infifo_full_err_int_ena_pos);
}

void SpiDebug::init_sr_dmaintclr(void) {
  uint32_t base = this->sr_dmaintclr.base_2;
  this->sr_dmaintclr.val = this->regRead(base);

  this->sr_dmaintclr.spi_app1_int_clr = this->regGet(this->sr_dmaintclr.val, this->sr_dmaintclr.spi_app1_int_clr_pos);
  this->sr_dmaintclr.spi_app2_int_clr = this->regGet(this->sr_dmaintclr.val, this->sr_dmaintclr.spi_app2_int_clr_pos);
  this->sr_dmaintclr.spi_mst_tx_afifo_rempty_err_int_clr = this->regGet(this->sr_dmaintclr.val, this->sr_dmaintclr.spi_mst_tx_afifo_rempty_err_int_clr_pos);
  this->sr_dmaintclr.spi_mst_rx_afifo_wfull_err_int_clr = this->regGet(this->sr_dmaintclr.val, this->sr_dmaintclr.spi_mst_rx_afifo_wfull_err_int_clr_pos);
  this->sr_dmaintclr.spi_slv_cmd_err_int_clr = this->regGet(this->sr_dmaintclr.val, this->sr_dmaintclr.spi_slv_cmd_err_int_clr_pos);
  this->sr_dmaintclr.r_spi_seg_magic_err_int_clr = this->regGet(this->sr_dmaintclr.val, this->sr_dmaintclr.r_spi_seg_magic_err_int_clr_pos);
  this->sr_dmaintclr.spi_dma_seg_trans_done_int_clr = this->regGet(this->sr_dmaintclr.val, this->sr_dmaintclr.spi_dma_seg_trans_done_int_clr_pos);
  this->sr_dmaintclr.spi_trans_done_int_clr = this->regGet(this->sr_dmaintclr.val, this->sr_dmaintclr.spi_trans_done_int_clr_pos);
  this->sr_dmaintclr.spi_slv_wr_buf_done_int_clr = this->regGet(this->sr_dmaintclr.val, this->sr_dmaintclr.spi_slv_wr_buf_done_int_clr_pos);
  this->sr_dmaintclr.spi_slv_rd_buf_done_int_clr = this->regGet(this->sr_dmaintclr.val, this->sr_dmaintclr.spi_slv_rd_buf_done_int_clr_pos);
  this->sr_dmaintclr.spi_slv_wr_dma_done_int_clr = this->regGet(this->sr_dmaintclr.val, this->sr_dmaintclr.spi_slv_wr_dma_done_int_clr_pos);
  this->sr_dmaintclr.spi_slv_rd_dma_done_int_clr = this->regGet(this->sr_dmaintclr.val, this->sr_dmaintclr.spi_slv_rd_dma_done_int_clr_pos);
  this->sr_dmaintclr.spi_slv_cmda_int_clr = this->regGet(this->sr_dmaintclr.val, this->sr_dmaintclr.spi_slv_cmda_int_clr_pos);
  this->sr_dmaintclr.spi_slv_cmd9_int_clr = this->regGet(this->sr_dmaintclr.val, this->sr_dmaintclr.spi_slv_cmd9_int_clr_pos);
  this->sr_dmaintclr.spi_slv_cmd8_int_clr = this->regGet(this->sr_dmaintclr.val, this->sr_dmaintclr.spi_slv_cmd8_int_clr_pos);
  this->sr_dmaintclr.spi_slv_cmd7_int_clr = this->regGet(this->sr_dmaintclr.val, this->sr_dmaintclr.spi_slv_cmd7_int_clr_pos);
  this->sr_dmaintclr.spi_slv_en_qpi_int_clr = this->regGet(this->sr_dmaintclr.val, this->sr_dmaintclr.spi_slv_en_qpi_int_clr_pos);
  this->sr_dmaintclr.spi_slv_ex_qpi_int_clr = this->regGet(this->sr_dmaintclr.val, this->sr_dmaintclr.spi_slv_ex_qpi_int_clr_pos);
  this->sr_dmaintclr.spi_dma_outfifo_empty_err_int_clr = this->regGet(this->sr_dmaintclr.val, this->sr_dmaintclr.spi_dma_outfifo_empty_err_int_clr_pos);
  this->sr_dmaintclr.spi_dma_infifo_full_err_int_clr = this->regGet(this->sr_dmaintclr.val, this->sr_dmaintclr.spi_dma_infifo_full_err_int_clr_pos);
}

void SpiDebug::init_sr_dmaintraw(void) {
  uint32_t base = this->sr_dmaintraw.base_2;
  this->sr_dmaintraw.val = this->regRead(base);

  this->sr_dmaintraw.spi_app1_int_raw = this->regGet(this->sr_dmaintraw.val, this->sr_dmaintraw.spi_app1_int_raw_pos);
  this->sr_dmaintraw.spi_app2_int_raw = this->regGet(this->sr_dmaintraw.val, this->sr_dmaintraw.spi_app2_int_raw_pos);
  this->sr_dmaintraw.spi_mst_tx_afifo_rempty_err_int_raw = this->regGet(this->sr_dmaintraw.val, this->sr_dmaintraw.spi_mst_tx_afifo_rempty_err_int_raw_pos);
  this->sr_dmaintraw.spi_mst_rx_afifo_wfull_err_int_raw = this->regGet(this->sr_dmaintraw.val, this->sr_dmaintraw.spi_mst_rx_afifo_wfull_err_int_raw_pos);
  this->sr_dmaintraw.spi_slv_cmd_err_int_raw = this->regGet(this->sr_dmaintraw.val, this->sr_dmaintraw.spi_slv_cmd_err_int_raw_pos);
  this->sr_dmaintraw.r_spi_seg_magic_err_int_raw = this->regGet(this->sr_dmaintraw.val, this->sr_dmaintraw.r_spi_seg_magic_err_int_raw_pos);
  this->sr_dmaintraw.spi_dma_seg_trans_done_int_raw = this->regGet(this->sr_dmaintraw.val, this->sr_dmaintraw.spi_dma_seg_trans_done_int_raw_pos);
  this->sr_dmaintraw.spi_trans_done_int_raw = this->regGet(this->sr_dmaintraw.val, this->sr_dmaintraw.spi_trans_done_int_raw_pos);
  this->sr_dmaintraw.spi_slv_wr_buf_done_int_raw = this->regGet(this->sr_dmaintraw.val, this->sr_dmaintraw.spi_slv_wr_buf_done_int_raw_pos);
  this->sr_dmaintraw.spi_slv_rd_buf_done_int_raw = this->regGet(this->sr_dmaintraw.val, this->sr_dmaintraw.spi_slv_rd_buf_done_int_raw_pos);
  this->sr_dmaintraw.spi_slv_wr_dma_done_int_raw = this->regGet(this->sr_dmaintraw.val, this->sr_dmaintraw.spi_slv_wr_dma_done_int_raw_pos);
  this->sr_dmaintraw.spi_slv_rd_dma_done_int_raw = this->regGet(this->sr_dmaintraw.val, this->sr_dmaintraw.spi_slv_rd_dma_done_int_raw_pos);
  this->sr_dmaintraw.spi_slv_cmda_int_raw = this->regGet(this->sr_dmaintraw.val, this->sr_dmaintraw.spi_slv_cmda_int_raw_pos);
  this->sr_dmaintraw.spi_slv_cmd9_int_raw = this->regGet(this->sr_dmaintraw.val, this->sr_dmaintraw.spi_slv_cmd9_int_raw_pos);
  this->sr_dmaintraw.spi_slv_cmd8_int_raw = this->regGet(this->sr_dmaintraw.val, this->sr_dmaintraw.spi_slv_cmd8_int_raw_pos);
  this->sr_dmaintraw.spi_slv_cmd7_int_raw = this->regGet(this->sr_dmaintraw.val, this->sr_dmaintraw.spi_slv_cmd7_int_raw_pos);
  this->sr_dmaintraw.spi_slv_en_qpi_int_raw = this->regGet(this->sr_dmaintraw.val, this->sr_dmaintraw.spi_slv_en_qpi_int_raw_pos);
  this->sr_dmaintraw.spi_slv_ex_qpi_int_raw = this->regGet(this->sr_dmaintraw.val, this->sr_dmaintraw.spi_slv_ex_qpi_int_raw_pos);
  this->sr_dmaintraw.spi_dma_outfifo_empty_err_int_raw = this->regGet(this->sr_dmaintraw.val, this->sr_dmaintraw.spi_dma_outfifo_empty_err_int_raw_pos);
  this->sr_dmaintraw.spi_dma_infifo_full_err_int_raw = this->regGet(this->sr_dmaintraw.val, this->sr_dmaintraw.spi_dma_infifo_full_err_int_raw_pos);
}

void SpiDebug::init_sr_dmaintst(void) {
  uint32_t base = this->sr_dmaintst.base_2;
  this->sr_dmaintst.val = this->regRead(base);

  this->sr_dmaintst.spi_app1_int_st = this->regGet(this->sr_dmaintst.val, this->sr_dmaintst.spi_app1_int_st_pos);
  this->sr_dmaintst.spi_app2_int_st = this->regGet(this->sr_dmaintst.val, this->sr_dmaintst.spi_app2_int_st_pos);
  this->sr_dmaintst.spi_mst_tx_afifo_rempty_err_int_st = this->regGet(this->sr_dmaintst.val, this->sr_dmaintst.spi_mst_tx_afifo_rempty_err_int_st_pos);
  this->sr_dmaintst.spi_mst_rx_afifo_wfull_err_int_st = this->regGet(this->sr_dmaintst.val, this->sr_dmaintst.spi_mst_rx_afifo_wfull_err_int_st_pos);
  this->sr_dmaintst.spi_slv_cmd_err_int_st = this->regGet(this->sr_dmaintst.val, this->sr_dmaintst.spi_slv_cmd_err_int_st_pos);
  this->sr_dmaintst.r_spi_seg_magic_err_int_st = this->regGet(this->sr_dmaintst.val, this->sr_dmaintst.r_spi_seg_magic_err_int_st_pos);
  this->sr_dmaintst.spi_dma_seg_trans_done_int_st = this->regGet(this->sr_dmaintst.val, this->sr_dmaintst.spi_dma_seg_trans_done_int_st_pos);
  this->sr_dmaintst.spi_trans_done_int_st = this->regGet(this->sr_dmaintst.val, this->sr_dmaintst.spi_trans_done_int_st_pos);
  this->sr_dmaintst.spi_slv_wr_buf_done_int_st = this->regGet(this->sr_dmaintst.val, this->sr_dmaintst.spi_slv_wr_buf_done_int_st_pos);
  this->sr_dmaintst.spi_slv_rd_buf_done_int_st = this->regGet(this->sr_dmaintst.val, this->sr_dmaintst.spi_slv_rd_buf_done_int_st_pos);
  this->sr_dmaintst.spi_slv_wr_dma_done_int_st = this->regGet(this->sr_dmaintst.val, this->sr_dmaintst.spi_slv_wr_dma_done_int_st_pos);
  this->sr_dmaintst.spi_slv_rd_dma_done_int_st = this->regGet(this->sr_dmaintst.val, this->sr_dmaintst.spi_slv_rd_dma_done_int_st_pos);
  this->sr_dmaintst.spi_slv_cmda_int_st = this->regGet(this->sr_dmaintst.val, this->sr_dmaintst.spi_slv_cmda_int_st_pos);
  this->sr_dmaintst.spi_slv_cmd9_int_st = this->regGet(this->sr_dmaintst.val, this->sr_dmaintst.spi_slv_cmd9_int_st_pos);
  this->sr_dmaintst.spi_slv_cmd8_int_st = this->regGet(this->sr_dmaintst.val, this->sr_dmaintst.spi_slv_cmd8_int_st_pos);
  this->sr_dmaintst.spi_slv_cmd7_int_st = this->regGet(this->sr_dmaintst.val, this->sr_dmaintst.spi_slv_cmd7_int_st_pos);
  this->sr_dmaintst.spi_slv_en_qpi_int_st = this->regGet(this->sr_dmaintst.val, this->sr_dmaintst.spi_slv_en_qpi_int_st_pos);
  this->sr_dmaintst.spi_slv_ex_qpi_int_st = this->regGet(this->sr_dmaintst.val, this->sr_dmaintst.spi_slv_ex_qpi_int_st_pos);
  this->sr_dmaintst.spi_dma_outfifo_empty_err_int_st = this->regGet(this->sr_dmaintst.val, this->sr_dmaintst.spi_dma_outfifo_empty_err_int_st_pos);
  this->sr_dmaintst.spi_dma_infifo_full_err_int_st = this->regGet(this->sr_dmaintst.val, this->sr_dmaintst.spi_dma_infifo_full_err_int_st_pos);
}

void SpiDebug::init_sr_dmaintset(void) {
  uint32_t base = this->sr_dmaintset.base_2;
  this->sr_dmaintset.val = this->regRead(base);

  this->sr_dmaintset.spi_app1_int_set = this->regGet(this->sr_dmaintset.val, this->sr_dmaintset.spi_app1_int_set_pos);
  this->sr_dmaintset.spi_app2_int_set = this->regGet(this->sr_dmaintset.val, this->sr_dmaintset.spi_app2_int_set_pos);
  this->sr_dmaintset.spi_mst_tx_afifo_rempty_err_int_set = this->regGet(this->sr_dmaintset.val, this->sr_dmaintset.spi_mst_tx_afifo_rempty_err_int_set_pos);
  this->sr_dmaintset.spi_mst_rx_afifo_wfull_err_int_set = this->regGet(this->sr_dmaintset.val, this->sr_dmaintset.spi_mst_rx_afifo_wfull_err_int_set_pos);
  this->sr_dmaintset.spi_slv_cmd_err_int_set = this->regGet(this->sr_dmaintset.val, this->sr_dmaintset.spi_slv_cmd_err_int_set_pos);
  this->sr_dmaintset.r_spi_seg_magic_err_int_set = this->regGet(this->sr_dmaintset.val, this->sr_dmaintset.r_spi_seg_magic_err_int_set_pos);
  this->sr_dmaintset.spi_dma_seg_trans_done_int_set = this->regGet(this->sr_dmaintset.val, this->sr_dmaintset.spi_dma_seg_trans_done_int_set_pos);
  this->sr_dmaintset.spi_trans_done_int_set = this->regGet(this->sr_dmaintset.val, this->sr_dmaintset.spi_trans_done_int_set_pos);
  this->sr_dmaintset.spi_slv_wr_buf_done_int_set = this->regGet(this->sr_dmaintset.val, this->sr_dmaintset.spi_slv_wr_buf_done_int_set_pos);
  this->sr_dmaintset.spi_slv_rd_buf_done_int_set = this->regGet(this->sr_dmaintset.val, this->sr_dmaintset.spi_slv_rd_buf_done_int_set_pos);
  this->sr_dmaintset.spi_slv_wr_dma_done_int_set = this->regGet(this->sr_dmaintset.val, this->sr_dmaintset.spi_slv_wr_dma_done_int_set_pos);
  this->sr_dmaintset.spi_slv_rd_dma_done_int_set = this->regGet(this->sr_dmaintset.val, this->sr_dmaintset.spi_slv_rd_dma_done_int_set_pos);
  this->sr_dmaintset.spi_slv_cmda_int_set = this->regGet(this->sr_dmaintset.val, this->sr_dmaintset.spi_slv_cmda_int_set_pos);
  this->sr_dmaintset.spi_slv_cmd9_int_set = this->regGet(this->sr_dmaintset.val, this->sr_dmaintset.spi_slv_cmd9_int_set_pos);
  this->sr_dmaintset.spi_slv_cmd8_int_set = this->regGet(this->sr_dmaintset.val, this->sr_dmaintset.spi_slv_cmd8_int_set_pos);
  this->sr_dmaintset.spi_slv_cmd7_int_set = this->regGet(this->sr_dmaintset.val, this->sr_dmaintset.spi_slv_cmd7_int_set_pos);
  this->sr_dmaintset.spi_slv_en_qpi_int_set = this->regGet(this->sr_dmaintset.val, this->sr_dmaintset.spi_slv_en_qpi_int_set_pos);
  this->sr_dmaintset.spi_slv_ex_qpi_int_set = this->regGet(this->sr_dmaintset.val, this->sr_dmaintset.spi_slv_ex_qpi_int_set_pos);
  this->sr_dmaintset.spi_dma_outfifo_empty_err_int_set = this->regGet(this->sr_dmaintset.val, this->sr_dmaintset.spi_dma_outfifo_empty_err_int_set_pos);
  this->sr_dmaintset.spi_dma_infifo_full_err_int_set = this->regGet(this->sr_dmaintset.val, this->sr_dmaintset.spi_dma_infifo_full_err_int_set_pos);
}

void SpiDebug::initInteruptRegs(void) {
  this->init_sr_dmaintena();
  this->init_sr_dmaintclr();
  this->init_sr_dmaintraw();
  this->init_sr_dmaintst();
  this->init_sr_dmaintset();
}

void SpiDebug::init(void) {
  this->initUserDefRegs();
  this->initCtrlConfRegs();
  this->initClockRegs();
  this->initTimingRegs();
  this->initInteruptRegs();
}
void SpiDebug::refresh(void) {
  this->init();
}

void SpiDebug::printAll(bool onlyCoreVals) {
  Serial.printf("--- Enumerating All SPI Registers ---\n");
  this->printUserDefs(onlyCoreVals);
  this->printCtrlConfs(onlyCoreVals);
  this->printClocks(onlyCoreVals);
  this->printTimings(onlyCoreVals);
  this->printInterupts(onlyCoreVals);
}
void SpiDebug::printInterupts(bool onlyCoreVals) {
  Serial.printf("=== SPI Interupt Registers ===\n");
  Serial.printf("~SPI interrupt enable register | Access : R/W\n");
  Serial.printf("\tRegister Value : 0x%X\n", this->sr_dmaintena.val);
  if(!onlyCoreVals){
    Serial.printf("\tReset Value : 0x%X\n", this->sr_dmaintena.reset);
    Serial.printf("\tRegister Base Addresses 2 & 3 :  0x%X | 0x%X\n", this->sr_dmaintena.base_2, this->sr_dmaintena.base_3);
    Serial.printf("\tAPP1 int Enabled : %s\n", this->sr_dmaintena.spi_app1_int_ena == true ? "true" : "false");
    Serial.printf("\tApp2 int Enabled : %s\n", this->sr_dmaintena.spi_app2_int_ena == true ? "true" : "false");
    Serial.printf("\tMst RX AFIFO rempty Err Int Enabled : %s\n", this->sr_dmaintena.spi_mst_tx_afifo_rempty_err_int_ena == true ? "true" : "false");
    Serial.printf("\tMst RX AFIFO WFull Err Int Enabled : %s\n", this->sr_dmaintena.spi_mst_rx_afifo_wfull_err_int_ena == true ? "true" : "false");
    Serial.printf("\tSlave CMD Error Int Enabled : %s\n", this->sr_dmaintena.spi_slv_cmd_err_int_ena == true ? "true" : "false");
    Serial.printf("\tSegment Magic Error Int Enabled : %s\n", this->sr_dmaintena.r_spi_seg_magic_err_int_ena == true ? "true" : "false");
    Serial.printf("\tDMA Segment Transmission Done Int Enabled : %s\n", this->sr_dmaintena.spi_dma_seg_trans_done_int_ena == true ? "true" : "false");
    Serial.printf("\tTransmission Done Int Enabled : %s\n", this->sr_dmaintena.spi_trans_done_int_ena == true ? "true" : "false");
    Serial.printf("\tSlave WR Buf Done Int Enabled : %s\n", this->sr_dmaintena.spi_slv_wr_buf_done_int_ena == true ? "true" : "false");
    Serial.printf("\tSlave RD Buf Done Int Enabled : %s\n", this->sr_dmaintena.spi_slv_rd_buf_done_int_ena == true ? "true" : "false");
    Serial.printf("\tSlave WR DMA Done Int Enabled : %s\n", this->sr_dmaintena.spi_slv_wr_dma_done_int_ena == true ? "true" : "false");
    Serial.printf("\tSLAVE RD DMA Done Int Enabled : %s\n", this->sr_dmaintena.spi_slv_rd_dma_done_int_ena == true ? "true" : "false");
    Serial.printf("\tSlave CMDA Int Enabled : %s\n", this->sr_dmaintena.spi_slv_cmda_int_ena == true ? "true" : "false");
    Serial.printf("\tSlave CMD 9 Int Enabled : %s\n", this->sr_dmaintena.spi_slv_cmd9_int_ena == true ? "true" : "false");
    Serial.printf("\tSlave CMD 8 Int Enabled : %s\n", this->sr_dmaintena.spi_slv_cmd8_int_ena == true ? "true" : "false");
    Serial.printf("\tSlave CMD 7 Int Enabled : %s\n", this->sr_dmaintena.spi_slv_cmd7_int_ena == true ? "true" : "false");
    Serial.printf("\tSlave En QPI Int Enabled : %s\n", this->sr_dmaintena.spi_slv_en_qpi_int_ena == true ? "true" : "false");
    Serial.printf("\tSlave EX QPI Int Enabled : %s\n", this->sr_dmaintena.spi_slv_ex_qpi_int_ena == true ? "true" : "false");
    Serial.printf("\tDMA OUTFIFO Empty Err Int Enabled : %s\n", this->sr_dmaintena.spi_dma_outfifo_empty_err_int_ena == true ? "true" : "false");
    Serial.printf("\tDMA INFIFO Full Err Int Enabled : %s\n", this->sr_dmaintena.spi_dma_infifo_full_err_int_ena == true ? "true" : "false");
  }

  Serial.printf("~SPI interrupt clear register | Access : WT\n");
  Serial.printf("\tRegister Value : 0x%X\n", this->sr_dmaintclr.val);
  if(!onlyCoreVals){
    Serial.printf("\tReset Value : 0x%X\n", this->sr_dmaintclr.reset);
    Serial.printf("\tRegister Base Addresses 2 & 3 :  0x%X | 0x%X\n", this->sr_dmaintclr.base_2, this->sr_dmaintclr.base_3);
    Serial.printf("\tAPP1 int Clear : %s\n", this->sr_dmaintclr.spi_app1_int_clr == true ? "true" : "false");
    Serial.printf("\tApp2 int Clear : %s\n", this->sr_dmaintclr.spi_app2_int_clr == true ? "true" : "false");
    Serial.printf("\tMst RX AFIFO rempty Err Int Clear : %s\n", this->sr_dmaintclr.spi_mst_tx_afifo_rempty_err_int_clr == true ? "true" : "false");
    Serial.printf("\tMst RX AFIFO WFull Err Int Clear : %s\n", this->sr_dmaintclr.spi_mst_rx_afifo_wfull_err_int_clr == true ? "true" : "false");
    Serial.printf("\tSlave CMD Error Int Clear : %s\n", this->sr_dmaintclr.spi_slv_cmd_err_int_clr == true ? "true" : "false");
    Serial.printf("\tSegment Magic Error Int Clear : %s\n", this->sr_dmaintclr.r_spi_seg_magic_err_int_clr == true ? "true" : "false");
    Serial.printf("\tDMA Segment Transmission Done Int Clear : %s\n", this->sr_dmaintclr.spi_dma_seg_trans_done_int_clr == true ? "true" : "false");
    Serial.printf("\tTransmission Done Int Clear : %s\n", this->sr_dmaintclr.spi_trans_done_int_clr == true ? "true" : "false");
    Serial.printf("\tSlave WR Buf Done Int Clear : %s\n", this->sr_dmaintclr.spi_slv_wr_buf_done_int_clr == true ? "true" : "false");
    Serial.printf("\tSlave RD Buf Done Int Clear : %s\n", this->sr_dmaintclr.spi_slv_rd_buf_done_int_clr == true ? "true" : "false");
    Serial.printf("\tSlave WR DMA Done Int Clear : %s\n", this->sr_dmaintclr.spi_slv_wr_dma_done_int_clr == true ? "true" : "false");
    Serial.printf("\tSLAVE RD DMA Done Int Clear : %s\n", this->sr_dmaintclr.spi_slv_rd_dma_done_int_clr == true ? "true" : "false");
    Serial.printf("\tSlave CMDA Int Clear : %s\n", this->sr_dmaintclr.spi_slv_cmda_int_clr == true ? "true" : "false");
    Serial.printf("\tSlave CMD 9 Int Clear : %s\n", this->sr_dmaintclr.spi_slv_cmd9_int_clr == true ? "true" : "false");
    Serial.printf("\tSlave CMD 8 Int Clear : %s\n", this->sr_dmaintclr.spi_slv_cmd8_int_clr == true ? "true" : "false");
    Serial.printf("\tSlave CMD 7 Int Clear : %s\n", this->sr_dmaintclr.spi_slv_cmd7_int_clr == true ? "true" : "false");
    Serial.printf("\tSlave En QPI Int Clear : %s\n", this->sr_dmaintclr.spi_slv_en_qpi_int_clr == true ? "true" : "false");
    Serial.printf("\tSlave EX QPI Int Clear : %s\n", this->sr_dmaintclr.spi_slv_ex_qpi_int_clr == true ? "true" : "false");
    Serial.printf("\tDMA OUTFIFO Empty Err Int Clear : %s\n", this->sr_dmaintclr.spi_dma_outfifo_empty_err_int_clr == true ? "true" : "false");
    Serial.printf("\tDMA INFIFO Full Err Int Clear : %s\n", this->sr_dmaintclr.spi_dma_infifo_full_err_int_clr == true ? "true" : "false");
  }

  Serial.printf("~SPI interrupt raw register | Access : R/WTC/SS\n");
  Serial.printf("\tRegister Value : 0x%X\n", this->sr_dmaintraw.val);
  if(!onlyCoreVals){
    Serial.printf("\tReset Value : 0x%X\n", this->sr_dmaintraw.reset);
    Serial.printf("\tRegister Base Addresses 2 & 3 :  0x%X | 0x%X\n", this->sr_dmaintraw.base_2, this->sr_dmaintraw.base_3);
    Serial.printf("\tAPP1 int Raw : %s\n", this->sr_dmaintraw.spi_app1_int_raw == true ? "true" : "false");
    Serial.printf("\tApp2 int Raw : %s\n", this->sr_dmaintraw.spi_app2_int_raw == true ? "true" : "false");
    Serial.printf("\tMst RX AFIFO rempty Err Int Raw : %s\n", this->sr_dmaintraw.spi_mst_tx_afifo_rempty_err_int_raw == true ? "true" : "false");
    Serial.printf("\tMst RX AFIFO WFull Err Int Raw : %s\n", this->sr_dmaintraw.spi_mst_rx_afifo_wfull_err_int_raw == true ? "true" : "false");
    Serial.printf("\tSlave CMD Error Int Raw : %s\n", this->sr_dmaintraw.spi_slv_cmd_err_int_raw == true ? "true" : "false");
    Serial.printf("\tSegment Magic Error Int Raw : %s\n", this->sr_dmaintraw.r_spi_seg_magic_err_int_raw == true ? "true" : "false");
    Serial.printf("\tDMA Segment Transmission Done Int Raw : %s\n", this->sr_dmaintraw.spi_dma_seg_trans_done_int_raw == true ? "true" : "false");
    Serial.printf("\tTransmission Done Int Raw : %s\n", this->sr_dmaintraw.spi_trans_done_int_raw == true ? "true" : "false");
    Serial.printf("\tSlave WR Buf Done Int Raw : %s\n", this->sr_dmaintraw.spi_slv_wr_buf_done_int_raw == true ? "true" : "false");
    Serial.printf("\tSlave RD Buf Done Int Raw : %s\n", this->sr_dmaintraw.spi_slv_rd_buf_done_int_raw == true ? "true" : "false");
    Serial.printf("\tSlave WR DMA Done Int Raw : %s\n", this->sr_dmaintraw.spi_slv_wr_dma_done_int_raw == true ? "true" : "false");
    Serial.printf("\tSLAVE RD DMA Done Int Raw : %s\n", this->sr_dmaintraw.spi_slv_rd_dma_done_int_raw == true ? "true" : "false");
    Serial.printf("\tSlave CMDA Int Raw : %s\n", this->sr_dmaintraw.spi_slv_cmda_int_raw == true ? "true" : "false");
    Serial.printf("\tSlave CMD 9 Int Raw : %s\n", this->sr_dmaintraw.spi_slv_cmd9_int_raw == true ? "true" : "false");
    Serial.printf("\tSlave CMD 8 Int Raw : %s\n", this->sr_dmaintraw.spi_slv_cmd8_int_raw == true ? "true" : "false");
    Serial.printf("\tSlave CMD 7 Int Raw : %s\n", this->sr_dmaintraw.spi_slv_cmd7_int_raw == true ? "true" : "false");
    Serial.printf("\tSlave En QPI Int Raw : %s\n", this->sr_dmaintraw.spi_slv_en_qpi_int_raw == true ? "true" : "false");
    Serial.printf("\tSlave EX QPI Int Raw : %s\n", this->sr_dmaintraw.spi_slv_ex_qpi_int_raw == true ? "true" : "false");
    Serial.printf("\tDMA OUTFIFO Empty Err Int Raw : %s\n", this->sr_dmaintraw.spi_dma_outfifo_empty_err_int_raw == true ? "true" : "false");
    Serial.printf("\tDMA INFIFO Full Err Int Raw : %s\n", this->sr_dmaintraw.spi_dma_infifo_full_err_int_raw == true ? "true" : "false");
  }

  Serial.printf("~SPI interrupt status register | Access : RO\n");
  Serial.printf("\tRegister Value : 0x%X\n", this->sr_dmaintst.val);
  if(!onlyCoreVals){
    Serial.printf("\tReset Value : 0x%X\n", this->sr_dmaintst.reset);
    Serial.printf("\tRegister Base Addresses 2 & 3 :  0x%X | 0x%X\n", this->sr_dmaintst.base_2, this->sr_dmaintst.base_3);
    Serial.printf("\tAPP1 int ST : %s\n", this->sr_dmaintst.spi_app1_int_st == true ? "true" : "false");
    Serial.printf("\tApp2 int ST : %s\n", this->sr_dmaintst.spi_app2_int_st == true ? "true" : "false");
    Serial.printf("\tMst RX AFIFO rempty Err Int ST : %s\n", this->sr_dmaintst.spi_mst_tx_afifo_rempty_err_int_st == true ? "true" : "false");
    Serial.printf("\tMst RX AFIFO WFull Err Int ST : %s\n", this->sr_dmaintst.spi_mst_rx_afifo_wfull_err_int_st == true ? "true" : "false");
    Serial.printf("\tSlave CMD Error Int ST : %s\n", this->sr_dmaintst.spi_slv_cmd_err_int_st == true ? "true" : "false");
    Serial.printf("\tSegment Magic Error Int ST : %s\n", this->sr_dmaintst.r_spi_seg_magic_err_int_st == true ? "true" : "false");
    Serial.printf("\tDMA Segment Transmission Done Int ST : %s\n", this->sr_dmaintst.spi_dma_seg_trans_done_int_st == true ? "true" : "false");
    Serial.printf("\tTransmission Done Int ST : %s\n", this->sr_dmaintst.spi_trans_done_int_st == true ? "true" : "false");
    Serial.printf("\tSlave WR Buf Done Int ST : %s\n", this->sr_dmaintst.spi_slv_wr_buf_done_int_st == true ? "true" : "false");
    Serial.printf("\tSlave RD Buf Done Int ST : %s\n", this->sr_dmaintst.spi_slv_rd_buf_done_int_st == true ? "true" : "false");
    Serial.printf("\tSlave WR DMA Done Int ST : %s\n", this->sr_dmaintst.spi_slv_wr_dma_done_int_st == true ? "true" : "false");
    Serial.printf("\tSLAVE RD DMA Done Int ST : %s\n", this->sr_dmaintst.spi_slv_rd_dma_done_int_st == true ? "true" : "false");
    Serial.printf("\tSlave CMDA Int ST : %s\n", this->sr_dmaintst.spi_slv_cmda_int_st == true ? "true" : "false");
    Serial.printf("\tSlave CMD 9 Int ST : %s\n", this->sr_dmaintst.spi_slv_cmd9_int_st == true ? "true" : "false");
    Serial.printf("\tSlave CMD 8 Int ST : %s\n", this->sr_dmaintst.spi_slv_cmd8_int_st == true ? "true" : "false");
    Serial.printf("\tSlave CMD 7 Int ST : %s\n", this->sr_dmaintst.spi_slv_cmd7_int_st == true ? "true" : "false");
    Serial.printf("\tSlave En QPI Int ST : %s\n", this->sr_dmaintst.spi_slv_en_qpi_int_st == true ? "true" : "false");
    Serial.printf("\tSlave EX QPI Int ST : %s\n", this->sr_dmaintst.spi_slv_ex_qpi_int_st == true ? "true" : "false");
    Serial.printf("\tDMA OUTFIFO Empty Err Int ST : %s\n", this->sr_dmaintst.spi_dma_outfifo_empty_err_int_st == true ? "true" : "false");
    Serial.printf("\tDMA INFIFO Full Err Int ST : %s\n", this->sr_dmaintst.spi_dma_infifo_full_err_int_st == true ? "true" : "false");
  }

  Serial.printf("~SPI interrupt software set register | Access : WT\n");
  Serial.printf("\tRegister Value : 0x%X\n", this->sr_dmaintset.val);
  if(!onlyCoreVals){
    Serial.printf("\tReset Value : 0x%X\n", this->sr_dmaintset.reset);
    Serial.printf("\tRegister Base Addresses 2 & 3 :  0x%X | 0x%X\n", this->sr_dmaintset.base_2, this->sr_dmaintset.base_3);
    Serial.printf("\tAPP1 int Set : %s\n", this->sr_dmaintset.spi_app1_int_set == true ? "true" : "false");
    Serial.printf("\tApp2 int Set : %s\n", this->sr_dmaintset.spi_app2_int_set == true ? "true" : "false");
    Serial.printf("\tMst RX AFIFO rempty Err Int Set : %s\n", this->sr_dmaintset.spi_mst_tx_afifo_rempty_err_int_set == true ? "true" : "false");
    Serial.printf("\tMst RX AFIFO WFull Err Int Set : %s\n", this->sr_dmaintset.spi_mst_rx_afifo_wfull_err_int_set == true ? "true" : "false");
    Serial.printf("\tSlave CMD Error Int Set : %s\n", this->sr_dmaintset.spi_slv_cmd_err_int_set == true ? "true" : "false");
    Serial.printf("\tSegment Magic Error Int Set : %s\n", this->sr_dmaintset.r_spi_seg_magic_err_int_set == true ? "true" : "false");
    Serial.printf("\tDMA Segment Transmission Done Int Set : %s\n", this->sr_dmaintset.spi_dma_seg_trans_done_int_set == true ? "true" : "false");
    Serial.printf("\tTransmission Done Int Set : %s\n", this->sr_dmaintset.spi_trans_done_int_set == true ? "true" : "false");
    Serial.printf("\tSlave WR Buf Done Int Set : %s\n", this->sr_dmaintset.spi_slv_wr_buf_done_int_set == true ? "true" : "false");
    Serial.printf("\tSlave RD Buf Done Int Set : %s\n", this->sr_dmaintset.spi_slv_rd_buf_done_int_set == true ? "true" : "false");
    Serial.printf("\tSlave WR DMA Done Int Set : %s\n", this->sr_dmaintset.spi_slv_wr_dma_done_int_set == true ? "true" : "false");
    Serial.printf("\tSLAVE RD DMA Done Int Set : %s\n", this->sr_dmaintset.spi_slv_rd_dma_done_int_set == true ? "true" : "false");
    Serial.printf("\tSlave CMDA Int Set : %s\n", this->sr_dmaintset.spi_slv_cmda_int_set == true ? "true" : "false");
    Serial.printf("\tSlave CMD 9 Int Set : %s\n", this->sr_dmaintset.spi_slv_cmd9_int_set == true ? "true" : "false");
    Serial.printf("\tSlave CMD 8 Int Set : %s\n", this->sr_dmaintset.spi_slv_cmd8_int_set == true ? "true" : "false");
    Serial.printf("\tSlave CMD 7 Int Set : %s\n", this->sr_dmaintset.spi_slv_cmd7_int_set == true ? "true" : "false");
    Serial.printf("\tSlave En QPI Int Set : %s\n", this->sr_dmaintset.spi_slv_en_qpi_int_set == true ? "true" : "false");
    Serial.printf("\tSlave EX QPI Int Set : %s\n", this->sr_dmaintset.spi_slv_ex_qpi_int_set == true ? "true" : "false");
    Serial.printf("\tDMA OUTFIFO Empty Err Int Set : %s\n", this->sr_dmaintset.spi_dma_outfifo_empty_err_int_set == true ? "true" : "false");
    Serial.printf("\tDMA INFIFO Full Err Int Set : %s\n", this->sr_dmaintset.spi_dma_infifo_full_err_int_set == true ? "true" : "false");
  }
}

void SpiDebug::printTimings(bool onlyCoreVals) {
  Serial.printf("=== SPI Timing Registers ===\n");

  Serial.printf("~SPI input delay mode configuration | Access : R/W\n");
  Serial.printf("\tRegister Value : 0x%X\n", this->sr_dinmode.val);
  if(!onlyCoreVals){
    Serial.printf("\tReset Value : 0x%X\n", this->sr_dinmode.reset);
    Serial.printf("\tRegister Base Addresses 2 & 3 :  0x%X | 0x%X\n", this->sr_dinmode.base_2, this->sr_dinmode.base_3);
    Serial.printf("\tTiming HCLK Active : %s\n", this->sr_dinmode.spi_timing_hclk_active == true ? "true" : "false");
    if(this->spiMode != 3){
      Serial.printf("\tDIN 7 Mode : %d\n", this->sr_dinmode.r_spi_din7_mode);
      Serial.printf("\tDIN 6 Mode : %d\n", this->sr_dinmode.r_spi_din6_mode);
      Serial.printf("\tDIN 5 Mode : %d\n", this->sr_dinmode.r_spi_din5_mode);
      Serial.printf("\tDIN 4 Mode : %d\n", this->sr_dinmode.r_spi_din4_mode);
    }
    Serial.printf("\tDIN 3 Mode : %d\n", this->sr_dinmode.spi_din3_mode);
    Serial.printf("\tDIN 2 Mode : %d\n", this->sr_dinmode.spi_din2_mode);
    Serial.printf("\tDIN 1 Mode : %d\n", this->sr_dinmode.spi_din1_mode);
    Serial.printf("\tDIN 0 Mode : %d\n", this->sr_dinmode.spi_din0_mode);
  }

  Serial.printf("~SPI input delay number configuration | Access : R/W\n");
  Serial.printf("\tRegister Value : 0x%X\n", this->sr_dinnum.val);
  if(!onlyCoreVals){
    Serial.printf("\tReset Value : 0x%X\n", this->sr_dinnum.reset);
    Serial.printf("\tRegister Base Addresses 2 & 3 :  0x%X | 0x%X\n", this->sr_dinnum.base_2, this->sr_dinnum.base_3);
    if(this->spiMode != 3){
      Serial.printf("\tDin 7 Num : %d\n", this->sr_dinnum.r_spi_din7_num);
      Serial.printf("\tDin 6 Num : %d\n", this->sr_dinnum.r_spi_din6_num);
      Serial.printf("\tDin 5 Num : %d\n", this->sr_dinnum.r_spi_din5_num);
      Serial.printf("\tDin 4 Num : %d\n", this->sr_dinnum.r_spi_din4_num);
    }
    Serial.printf("\tDin 3 Num : %d\n", this->sr_dinnum.spi_din3_num);
    Serial.printf("\tDin 2 Num : %d\n", this->sr_dinnum.spi_din2_num);
    Serial.printf("\tDin 1 Num : %d\n", this->sr_dinnum.spi_din1_num);
    Serial.printf("\tDin 0 Num : %d\n", this->sr_dinnum.spi_din0_num);
  }

  Serial.printf("~SPI output delay mode configuration | Access : R/W\n");
  Serial.printf("\tRegister Value : 0x%X\n", this->sr_doutmode.val);
  if(!onlyCoreVals){
    Serial.printf("\tReset Value : 0x%X\n", this->sr_doutmode.reset);
    Serial.printf("\tRegister Base Addresses 2 & 3 :  0x%X | 0x%X\n", this->sr_doutmode.base_2, this->sr_doutmode.base_3);
    Serial.printf("\tD DQS Mode : %s\n", this->sr_doutmode.r_spi_d_dqs_mode == true ? "true" : "false");
    if(this->spiMode != 3){
      Serial.printf("\tDout 7 Mode : %s\n", this->sr_doutmode.r_spi_dout7_mode == true ? "true" : "false");
      Serial.printf("\tDout 6 Mode : %s\n", this->sr_doutmode.r_spi_dout6_mode == true ? "true" : "false");
      Serial.printf("\tDout 5 Mode : %s\n", this->sr_doutmode.r_spi_dout5_mode == true ? "true" : "false");
      Serial.printf("\tDout 4 Mode : %s\n", this->sr_doutmode.r_spi_dout4_mode == true ? "true" : "false");
    }
    Serial.printf("\tDout 3 Mode : %s\n", this->sr_doutmode.spi_dout3_mode == true ? "true" : "false");
    Serial.printf("\tDout 2 Mode : %s\n", this->sr_doutmode.spi_dout2_mode == true ? "true" : "false");
    Serial.printf("\tDout 1 Mode : %s\n", this->sr_doutmode.spi_dout1_mode == true ? "true" : "false");
    Serial.printf("\tDout 0 Mode : %s\n", this->sr_doutmode.spi_dout0_mode == true ? "true" : "false");
  }
}

void SpiDebug::printClocks(bool onlyCoreVals) {
  Serial.printf("=== SPI Clock Registers ===\n");
  Serial.printf("~SPI clock control register | Access : R/W\n");
  Serial.printf("\tRegister Value : 0x%X\n", this->sr_clock.val);
  if(!onlyCoreVals){
    Serial.printf("\tReset Value : 0x%X\n", this->sr_clock.reset);
    Serial.printf("\tRegister Base Addresses 2 & 3 :  0x%X | 0x%X\n", this->sr_clock.base_2, this->sr_clock.base_3);
    Serial.printf("\tCLK Equ SYSCLK : %s\n", this->sr_clock.spi_clk_equ_sysclk == true ? "true" : "false");
    Serial.printf("\tCLKDIV Pre : %d\n", this->sr_clock.spi_clkdiv_pre);
    Serial.printf("\tCLK Count N : 0x%X\n", this->sr_clock.spi_clkcnt_n);
    Serial.printf("\tCLK Count H : 0x%X\n", this->sr_clock.spi_clkcnt_h);
    Serial.printf("\tCLK Count L : 0x%X\n", this->sr_clock.spi_clkcnt_l);
  }

  Serial.printf("~SPI module clock and register clock control | Access : R/W\n");
  Serial.printf("\tRegister Value : 0x%X\n", this->sr_gate.val);
  if(!onlyCoreVals){
    Serial.printf("\tReset Value : 0x%X\n", this->sr_gate.reset);
    Serial.printf("\tRegister Base Addresses 2 & 3 :  0x%X | 0x%X\n", this->sr_gate.base_2, this->sr_gate.base_3);
    Serial.printf("\tMST CLK Sel : %s\n", this->sr_gate.spi_mst_clk_sel == true ? "true" : "false");
    Serial.printf("\tMST CLK Active : %s\n", this->sr_gate.spi_mst_clk_active == true ? "true" : "false");
    Serial.printf("\tCLK Enabled : %s\n", this->sr_gate.spi_clk_en == true ? "true" : "false");
  }
}

void SpiDebug::printCtrlConfs(bool onlyCoreVals) {
  Serial.printf("=== SPI Control Config Registers ===\n");
  Serial.printf("~SPI control register | Access : R/W\n");
  Serial.printf("\tRegister Value : 0x%X\n", this->sr_ctrl.val);
  if(!onlyCoreVals){
    Serial.printf("\tReset Value : 0x%X\n", this->sr_ctrl.reset);
    Serial.printf("\tRegister Base Addresses 2 & 3 :  0x%X | 0x%X\n", this->sr_ctrl.base_2, this->sr_ctrl.base_3);
    Serial.printf("\tWR Bit Order : 0x%X\n", this->sr_ctrl.spi_wr_bit_order);
    Serial.printf("\tRD Bit Order : 0x%X\n", this->sr_ctrl.spi_rd_bit_order);
    Serial.printf("\tWP Pol : %s\n", this->sr_ctrl.spi_wp_pol == true ? "true" : "false");
    Serial.printf("\tHold Pol : %s\n", this->sr_ctrl.spi_hold_pol == true ? "true" : "false");
    Serial.printf("\tD Pol : %s\n", this->sr_ctrl.spi_d_pol == true ? "true" : "false");
    Serial.printf("\tQ Pol : %s\n", this->sr_ctrl.spi_q_pol == true ? "true" : "false");
    if(this->spiMode != 3)
      Serial.printf("\tFread Oct : %s\n", this->sr_ctrl.r_spi_fread_oct == true ? "true" : "false");
    Serial.printf("\tFread Quad : %s\n", this->sr_ctrl.spi_fread_quad == true ? "true" : "false");
    Serial.printf("\tFread Duel : %s\n", this->sr_ctrl.spi_fread_dual == true ? "true" : "false");
    if(this->spiMode != 3)
      Serial.printf("\tFcmd Oct : %s\n", this->sr_ctrl.r_spi_fcmd_oct == true ? "true" : "false");
    Serial.printf("\tFcmd Quad : %s\n", this->sr_ctrl.spi_fcmd_quad == true ? "true" : "false");
    Serial.printf("\tFcmd Dual : %s\n", this->sr_ctrl.spi_fcmd_dual == true ? "true" : "false");
    if(this->spiMode != 3)
      Serial.printf("\tFaddr Oct : %s\n", this->sr_ctrl.r_spi_faddr_oct == true ? "true" : "false");
    Serial.printf("\tFaddr Quad : %s\n", this->sr_ctrl.spi_faddr_quad == true ? "true" : "false");
    Serial.printf("\tFaddr Dual : %s\n", this->sr_ctrl.spi_faddr_dual == true ? "true" : "false");
    Serial.printf("\tDummy Out : %s\n", this->sr_ctrl.spi_dummy_out == true ? "true" : "false");
  }

  Serial.printf("~SPI data bit length control register | Access : R/W\n");
  Serial.printf("\tRegister Value : 0x%X\n", this->sr_msdlen.val);
  if(!onlyCoreVals){
    Serial.printf("\tReset Value : 0x%X\n", this->sr_msdlen.reset);
    Serial.printf("\tRegister Base Addresses 2 & 3 :  0x%X | 0x%X\n", this->sr_msdlen.base_2, this->sr_msdlen.base_3);
    Serial.printf("\tMs Data Bit Length : %d\n", this->sr_msdlen.spi_ms_data_bitlen);
  }

  Serial.printf("~SPI misc register   R/W -- different relative to spi2 and spi3, this is for spi2\n");
  Serial.printf("\tRegister Value : 0x%X\n", this->sr_misc.val);
  if(!onlyCoreVals){
    Serial.printf("\tReset Value : 0x%X\n", this->sr_misc.reset);
    Serial.printf("\tRegister Base Addresses 2 & 3 :  0x%X | 0x%X\n", this->sr_misc.base_2, this->sr_misc.base_3);
    Serial.printf("\tQuad Din Pin Swap : %s\n", this->sr_misc.spi_quad_din_pin_swap == true ? "true" : "false");
    Serial.printf("\tCS Keep Active : %s\n", this->sr_misc.spi_cs_keep_active == true ? "true" : "false");
    Serial.printf("\tCK Idle Edge : %s\n", this->sr_misc.spi_ck_idle_edge == true ? "true" : "false");
    Serial.printf("\tDQS Idle Edge : %s\n", this->sr_misc.spi_dqs_idle_edge == true ? "true" : "false");
    Serial.printf("\tSlave CS Pol : %s\n", this->sr_misc.spi_slave_cs_pol == true ? "true" : "false");
    Serial.printf("\tCmd Dtr Enabled : %s\n", this->sr_misc.spi_cmd_dtr_en == true ? "true" : "false");
    Serial.printf("\tAddr Dtr Enabled : %s\n", this->sr_misc.spi_addr_dtr_en == true ? "true" : "false");
    Serial.printf("\tData Dtr Enabled : %s\n", this->sr_misc.spi_data_dtr_en == true ? "true" : "false");
    Serial.printf("\tClk Data Dtr Enabled : %s\n", this->sr_misc.spi_clk_data_dtr_en == true ? "true" : "false");
    Serial.printf("\tMaster CS Pol : 0x%X\n", this->sr_misc.spi_master_cs_pol);
    Serial.printf("\tCK Disabled : %s\n", this->sr_misc.spi_ck_dis == true ? "true" : "false");
    if(this->spiMode != 3){
      Serial.printf("\tCS 5 Disabled : %s\n", this->sr_misc.spi_cs5_dis == true ? "true" : "false");
      Serial.printf("\tCS 4 Disabled : %s\n", this->sr_misc.spi_cs4_dis == true ? "true" : "false");
      Serial.printf("\tCS 3 Disabled : %s\n", this->sr_misc.spi_cs3_dis == true ? "true" : "false");
    }
    Serial.printf("\tCS 2 Disabled : %s\n", this->sr_misc.spi_cs2_dis == true ? "true" : "false");
    Serial.printf("\tCS 1 Disabled : %s\n", this->sr_misc.spi_cs1_dis == true ? "true" : "false");
    Serial.printf("\tCS 0 Disabled : %s\n", this->sr_misc.spi_cs0_dis == true ? "true" : "false");
  }

  Serial.printf("~SPI DMA control register | Access : varies\n");
  Serial.printf("\tRegister Value : 0x%X\n", this->sr_dmaconf.val);
  if(!onlyCoreVals){
    Serial.printf("\tReset Value : 0x%X\n", this->sr_dmaconf.reset);
    Serial.printf("\tRegister Base Addresses 2 & 3 :  0x%X | 0x%X\n", this->sr_dmaconf.base_2, this->sr_dmaconf.base_3);
    Serial.printf("\tDMA AFIFO Reset : %s\n", this->sr_dmaconf.spi_dma_afifo_rst== true ? "true" : "false");
    Serial.printf("\tBuf AFIFO Reset : %s\n", this->sr_dmaconf.spi_buf_afifo_rst == true ? "true" : "false");
    Serial.printf("\tRX AFIFO Reset : %s\n", this->sr_dmaconf.spi_rx_afifo_rst== true ? "true" : "false");
    Serial.printf("\tDMA TX Enabled : %s\n", this->sr_dmaconf.spi_dma_tx_ena == true ? "true" : "false");
    Serial.printf("\tDMA RX Enabled : %s\n", this->sr_dmaconf.spi_dma_rx_ena == true ? "true" : "false");
    Serial.printf("\tRX EOF Enabled : %s\n", this->sr_dmaconf.spi_rx_eof_en == true ? "true" : "false");
    Serial.printf("\tSlave TX segment Transmission Clear Enabled : %s\n", this->sr_dmaconf.spi_slv_tx_seg_trans_clr_en == true ? "true" : "false");
    Serial.printf("\tSlave RX Segment transmition Clear Enabled : %s\n", this->sr_dmaconf.spi_slv_rx_seg_trans_clr_en == true ? "true" : "false");
    Serial.printf("\tDMA Slave Segmant Transmision Enabled : %s\n", this->sr_dmaconf.spi_dma_slv_seg_trans_en== true ? "true" : "false");
    Serial.printf("\tDMA INFIFO Full : %s\n", this->sr_dmaconf.spi_dma_infifo_full == true ? "true" : "false");
    Serial.printf("\tDMA OUTFIFO Empty : %s\n", this->sr_dmaconf.spi_dma_outfifo_empty == true ? "true" : "false");
  }

  Serial.printf("~SPI slave control register | Access : varies\n");
  Serial.printf("\tRegister Value : 0x%X\n", this->sr_slave.val);
  if(!onlyCoreVals){
    Serial.printf("\tReset Value : 0x%X\n", this->sr_slave.reset);
    Serial.printf("\tRegister Base Addresses 2 & 3 :  0x%X | 0x%X\n", this->sr_slave.base_2, this->sr_slave.base_3);
    Serial.printf("\tUser Config : %s\n", this->sr_slave.r_spi_usr_conf == true ? "true" : "false");
    Serial.printf("\tSoft Reset : %s\n", this->sr_slave.spi_soft_reset == true ? "true" : "false");
    Serial.printf("\tSlave Mode : %s\n", this->sr_slave.spi_slave_mode == true ? "true" : "false");
    Serial.printf("\tDMA Segment Magic Value : 0x%X\n", this->sr_slave.r_spi_dma_seg_magic_value);
    Serial.printf("\tSlave WRbuf Bit Length Enabled : %s\n", this->sr_slave.spi_slv_wrbuf_bitlen_en == true ? "true" : "false");
    Serial.printf("\tSlave RDbuf Bit Length Enabled : %s\n", this->sr_slave.spi_slv_rdbuf_bitlen_en == true ? "true" : "false");
    Serial.printf("\tSlave WRdma Bit Length Enabled : %s\n", this->sr_slave.spi_slv_wrdma_bitlen_en == true ? "true" : "false");
    Serial.printf("\tSlave RDdma Bit Length Enabled : %s\n", this->sr_slave.spi_slv_rddma_bitlen_en == true ? "true" : "false");
    Serial.printf("\tRSCK Data Out : %s\n", this->sr_slave.spi_rsck_data_out == true ? "true" : "false");
    Serial.printf("\tCLK Mode 13 : %s\n", this->sr_slave.spi_clk_mode_13 == true ? "true" : "false");
    Serial.printf("\tClock Mode : 0x%X\n", this->sr_slave.spi_clk_mode);
  }

  Serial.printf("~SPI slave control register 1 | Access : R/W/SS\n");
  Serial.printf("\tRegister Value : 0x%X\n", this->sr_slave1.val);
  if(!onlyCoreVals){
    Serial.printf("\tReset Value : 0x%X\n", this->sr_slave1.reset);
    Serial.printf("\tRegister Base Addresses 2 & 3 :  0x%X | 0x%X\n", this->sr_slave1.base_2, this->sr_slave1.base_3);
    Serial.printf("\tSlave Last Address : 0x%X\n", this->sr_slave1.spi_slv_last_addr);
    Serial.printf("\tSlave Last Command : 0x%X\n", this->sr_slave1.spi_slv_last_command);
    Serial.printf("\tSlave Data Bitlength : %d\n", this->sr_slave1.spi_slv_data_bitlen);
  }
}

void SpiDebug::printUserDefs(bool onlyCoreVals) {
  Serial.printf("=== SPI User Defined Register ===\n");
  Serial.printf("~Command Control Register | Access : varies\n");
  Serial.printf("\tRegister Value : 0x%X\n", this->sr_cmd.val);
  if(!onlyCoreVals){
    Serial.printf("\tReset Value : 0x%X\n", this->sr_cmd.reset);
    Serial.printf("\tRegister Base Addresses 2 & 3 :  0x%X | 0x%X\n", this->sr_cmd.base_2, this->sr_cmd.base_3);
    Serial.printf("\tSpi User : %s\n", this->sr_cmd.spi_user == true ? "true" : "false");
    Serial.printf("\tSpi Update : %s\n", this->sr_cmd.spi_update == true ? "true" : "false");
  }

  Serial.printf("~Address Value Register | Access : R/W\n");
  Serial.printf("\tRegister Value : 0x%X\n", this->sr_addr.val);
  if(!onlyCoreVals){
    Serial.printf("\tReset Value : 0x%X\n", this->sr_addr.reset);
    Serial.printf("\tRegister Base Addresses 2 & 3 :  0x%X | 0x%X\n", this->sr_addr.base_2, this->sr_addr.base_3);
    Serial.printf("\tUser Address : 0x%x\n", this->sr_addr.spi_usr_addr_value);
  }

  Serial.printf("~User Control Register | Access : varies\n");
  Serial.printf("\tRegister Value : 0x%X\n", this->sr_user.val);
  if(!onlyCoreVals){
    Serial.printf("\tReset Value : 0x%X\n", this->sr_user.reset);
    Serial.printf("\tRegister Base Addresses 2 & 3 :  0x%X | 0x%X\n", this->sr_user.base_2, this->sr_user.base_3);
    Serial.printf("\tUser Command : %s\n", this->sr_user.spi_usr_command == true ? "true" : "false");
    Serial.printf("\tUser Address : %s\n", this->sr_user.spi_usr_addr == true ? "true" : "false");
    Serial.printf("\tUser Dummy : %s\n", this->sr_user.spi_usr_dummy == true ? "true" : "false");
    Serial.printf("\tUser MISO : %s\n", this->sr_user.spi_usr_miso == true ? "true" : "false");
    Serial.printf("\tUser MOSI : %s\n", this->sr_user.spi_usr_mosi == true ? "true" : "false");
    Serial.printf("\tUser Dummy Idle : %s\n", this->sr_user.spi_usr_dummy_idle == true ? "true" : "false");
    Serial.printf("\tMOSI High Part : %s\n", this->sr_user.spi_usr_mosi_highpart == true ? "true" : "false");
    Serial.printf("\tMISO High Part : %s\n", this->sr_user.spi_usr_miso_highpart == true ? "true" : "false");
    Serial.printf("\tSPI SIO : %s\n", this->sr_user.spi_sio == true ? "true" : "false");
    if(this->spiMode != 3)
      Serial.printf("\tConf NXT : %s\n", this->sr_user.r_spi_usr_conf_nxt == true ? "true" : "false");
    if(this->spiMode != 3)
      Serial.printf("\tFwrite OCT : %s\n", this->sr_user.r_spi_fwrite_oct == true ? "true" : "false");
    Serial.printf("\tFwrite Quad : %s\n", this->sr_user.spi_fwrite_quad == true ? "true" : "false");
    Serial.printf("\tFwrite Dual : %s\n", this->sr_user.spi_fwrite_dual == true ? "true" : "false");
    Serial.printf("\tCK Out Edge : %s\n", this->sr_user.spi_usr_ck_out_edge == true ? "true" : "false");
    Serial.printf("\tRSCK I Edege : %s\n", this->sr_user.spi_rsck_i_edge == true ? "true" : "false");
    Serial.printf("\tCS Setup : %s\n", this->sr_user.spi_cs_setup == true ? "true" : "false");
    Serial.printf("\tCS Hold : %s\n", this->sr_user.spi_cs_hold == true ? "true" : "false");
    Serial.printf("\tTSCK I Edge : %s\n", this->sr_user.spi_tsck_i_edge == true ? "true" : "false");
    if(this->spiMode != 3)
      Serial.printf("\tOPI Mode : %s\n", this->sr_user.r_spi_opi_mode == true ? "true" : "false");
    Serial.printf("\tQPI Mode : %s\n", this->sr_user.spi_qpi_mode == true ? "true" : "false");
    Serial.printf("\tDout Din : %s\n", this->sr_user.spi_doutdin == true ? "true" : "false");
  }

  Serial.printf("~User Control Register 1 | Access: R/W\n");
  Serial.printf("\tRegister Value : 0x%X\n", this->sr_user1.val);
  if(!onlyCoreVals){
    Serial.printf("\tReset Value : 0x%X\n", this->sr_user1.reset);
    Serial.printf("\tRegister Base Addresses 2 & 3 :  0x%X | 0x%X\n", this->sr_user1.base_2, this->sr_user1.base_3);
    Serial.printf("\tAddress Bit length : %d\n", this->sr_user1.spi_usr_addr_bitlen);
    Serial.printf("\tCS Hold Time : %d\n", this->sr_user1.spi_cs_hold_time);
    Serial.printf("\tCS Setup Time : %d\n", this->sr_user1.spi_cs_setup_time);
    Serial.printf("\tMST Wfull Error end Enable : %s\n", this->sr_user1.spi_mst_wfull_err_end_en == true ? "true" : "false");
    Serial.printf("\tDummy Cycle Length : %d\n", this->sr_user1.spi_usr_dummy_cyclelen);
  }

  Serial.printf("~User Control Register 2 | Access: R/W\n");
  Serial.printf("\tRegister Value : 0x%X\n", this->sr_user2.val);
  if(!onlyCoreVals){
    Serial.printf("\tReset Value : 0x%X\n", this->sr_user2.reset);
    Serial.printf("\tRegister Base Addresses 2 & 3 :  0x%X | 0x%X\n", this->sr_user2.base_2, this->sr_user2.base_3);
    Serial.printf("\tCommand Bit Length : %d\n", this->sr_user2.spi_usr_command_bitlen);
    Serial.printf("\tMST Rempty Err End Enable : %s\n", this->sr_user2.spi_mst_rempty_err_end_en == true ? "true" : "false");
    Serial.printf("\tCommand Value : 0x%X\n", this->sr_user2.spi_usr_command_value);
  }
}

void SpiDebug::patchUserDef(bool writeMode){
	this->patch_sr_cmd(writeMode);
	this->patch_sr_addr(writeMode);
	this->patch_sr_user(writeMode);
	this->patch_sr_user1(writeMode);
	this->patch_sr_user2(writeMode);
}
void SpiDebug::patch_sr_cmd(bool writeMode){
	uint32_t r = this->sr_cmd.val;
	r = this->regSet(r, this->sr_cmd.spi_user, this->sr_cmd.spi_user_pos);
	r = this->regSet(r, this->sr_cmd.spi_update, this->sr_cmd.spi_update_pos);
	if(writeMode){
		this->regWrite(this->sr_cmd.base_2, r);
		this->init_sr_cmd();
	}else{
		this->sr_cmd.val = r;
	}
}
void SpiDebug::patch_sr_addr(bool writeMode){
	uint32_t r = this->sr_addr.val;
	r = this->regSet(r, this->sr_addr.spi_usr_addr_value, 0, 31);
	if(writeMode){
		this->regWrite(this->sr_addr.base_2, r);
		this->init_sr_addr();
	}else{
		this->sr_addr.val = r;
	}
}
void SpiDebug::patch_sr_user(bool writeMode){
	uint32_t r = this->sr_user.val;
	r = this->regSet(r, this->sr_user.spi_usr_command, this->sr_user.spi_usr_command_pos);
	r = this->regSet(r, this->sr_user.spi_usr_addr, this->sr_user.spi_usr_addr_pos);
	r = this->regSet(r, this->sr_user.spi_usr_dummy, this->sr_user.spi_usr_dummy_pos);
	r = this->regSet(r, this->sr_user.spi_usr_miso, this->sr_user.spi_usr_miso_pos);
	r = this->regSet(r, this->sr_user.spi_usr_mosi, this->sr_user.spi_usr_mosi_pos);
	r = this->regSet(r, this->sr_user.spi_usr_dummy_idle, this->sr_user.spi_usr_dummy_idle_pos);
	r = this->regSet(r, this->sr_user.spi_usr_mosi_highpart, this->sr_user.spi_usr_mosi_highpart_pos);
	r = this->regSet(r, this->sr_user.spi_usr_miso_highpart, this->sr_user.spi_usr_miso_highpart_pos);
	r = this->regSet(r, this->sr_user.spi_sio, this->sr_user.spi_sio_pos);
	r = this->regSet(r, this->sr_user.r_spi_usr_conf_nxt, this->sr_user.r_spi_usr_conf_nxt_pos);
	r = this->regSet(r, this->sr_user.r_spi_fwrite_oct, this->sr_user.r_spi_fwrite_oct_pos);
	r = this->regSet(r, this->sr_user.spi_fwrite_quad, this->sr_user.spi_fwrite_quad_pos);
	r = this->regSet(r, this->sr_user.spi_fwrite_dual, this->sr_user.spi_fwrite_dual_pos);
	r = this->regSet(r, this->sr_user.spi_usr_ck_out_edge, this->sr_user.spi_usr_ck_out_edge_pos);
	r = this->regSet(r, this->sr_user.spi_rsck_i_edge, this->sr_user.spi_rsck_i_edge_pos);
	r = this->regSet(r, this->sr_user.spi_cs_setup, this->sr_user.spi_cs_setup_pos);
	r = this->regSet(r, this->sr_user.spi_cs_hold, this->sr_user.spi_cs_hold_pos);
	r = this->regSet(r, this->sr_user.spi_tsck_i_edge, this->sr_user.spi_tsck_i_edge_pos);
	r = this->regSet(r, this->sr_user.r_spi_opi_mode, this->sr_user.r_spi_opi_mode_pos);
	r = this->regSet(r, this->sr_user.spi_qpi_mode, this->sr_user.spi_qpi_mode_pos);
	r = this->regSet(r, this->sr_user.spi_doutdin, this->sr_user.spi_doutdin_pos);

	if(writeMode){
		this->regWrite(this->sr_user.base_2, r);
		this->init_sr_user();
	}else{
		this->sr_user.val = r;
	}
}
void SpiDebug::patch_sr_user1(bool writeMode){
	uint32_t r = this->sr_user1.val;
	
	r = this->regSet(r, this->sr_user1.spi_usr_addr_bitlen, this->sr_user1.spi_usr_addr_bitlen_x, this->sr_user1.spi_usr_addr_bitlen_y);

  	r = this->regSet(r, this->sr_user1.spi_cs_hold_time, this->sr_user1.spi_cs_hold_time_x, this->sr_user1.spi_cs_hold_time_y);

  	r = this->regSet(r, this->sr_user1.spi_cs_setup_time, this->sr_user1.spi_cs_setup_time_x, this->sr_user1.spi_cs_setup_time_y);

  	r = this->regSet(r, this->sr_user1.spi_mst_wfull_err_end_en, this->sr_user1.spi_mst_wfull_err_end_en_pos);

  	r = this->regSet(r, this->sr_user1.spi_usr_dummy_cyclelen, this->sr_user1.spi_usr_dummy_cyclelen_x, this->sr_user1.spi_usr_dummy_cyclelen_y);
	if(writeMode){
		this->regWrite(this->sr_user1.base_2, r);
		this->init_sr_user1();
	}else{
		this->sr_user1.val = r;	
	}

}
void SpiDebug::patch_sr_user2(bool writeMode){
	uint32_t r = this->sr_user2.val;
	
	r = this->regSet(r, this->sr_user2.spi_usr_command_bitlen, this->sr_user2.spi_usr_command_bitlen_x,this->sr_user2.spi_usr_command_bitlen_y);

  	r = this->regSet(r, this->sr_user2.spi_mst_rempty_err_end_en, this->sr_user2.spi_mst_rempty_err_end_en_pos);

  	r = this->regSet(r, this->sr_user2.spi_usr_command_value, this->sr_user2.spi_usr_command_value_x,this->sr_user2.spi_usr_command_value_y);
	if(writeMode){
		this->regWrite(this->sr_user2.base_2, r);
		this->init_sr_user2();
	}else{
		this->sr_user2.val = r;
	}

}

void SpiDebug::patchCtrlConf(bool writeMode){
	this->patch_sr_ctrl(writeMode);
	this->patch_sr_msdlen(writeMode);
	this->patch_sr_misc(writeMode);
	this->patch_sr_dmaconf(writeMode);
	this->patch_sr_slave(writeMode);
	this->patch_sr_slave1(writeMode);
}
void SpiDebug::patch_sr_ctrl(bool writeMode){
	uint32_t r = this->sr_ctrl.val;
	  r = this->regSet(r, this->sr_ctrl.spi_wr_bit_order, this->sr_ctrl.spi_wr_bit_order_x, this->sr_ctrl.spi_wr_bit_order_y);

  r = this->regSet(r, this->sr_ctrl.spi_rd_bit_order, this->sr_ctrl.spi_rd_bit_order_x, this->sr_ctrl.spi_rd_bit_order_y);

  r = this->regSet(r, this->sr_ctrl.spi_wp_pol, this->sr_ctrl.spi_wp_pol_pos);
  r = this->regSet(r, this->sr_ctrl.spi_hold_pol, this->sr_ctrl.spi_hold_pol_pos);
  r = this->regSet(r, this->sr_ctrl.spi_d_pol, this->sr_ctrl.spi_d_pol_pos);
  r = this->regSet(r, this->sr_ctrl.spi_q_pol, this->sr_ctrl.spi_q_pol_pos);
  r = this->regSet(r, this->sr_ctrl.r_spi_fread_oct, this->sr_ctrl.r_spi_fread_oct_pos);
  r = this->regSet(r, this->sr_ctrl.spi_fread_quad, this->sr_ctrl.spi_fread_quad_pos);
  r = this->regSet(r, this->sr_ctrl.spi_fread_dual, this->sr_ctrl.spi_fread_dual_pos);
  r = this->regSet(r, this->sr_ctrl.r_spi_fcmd_oct, this->sr_ctrl.r_spi_fcmd_oct_pos);
  r = this->regSet(r, this->sr_ctrl.spi_fcmd_quad, this->sr_ctrl.spi_fcmd_quad_pos);
  r = this->regSet(r, this->sr_ctrl.spi_fcmd_dual, this->sr_ctrl.spi_fcmd_dual_pos);
  r = this->regSet(r, this->sr_ctrl.r_spi_faddr_oct, this->sr_ctrl.r_spi_faddr_oct_pos);
  r = this->regSet(r, this->sr_ctrl.spi_faddr_quad, this->sr_ctrl.spi_faddr_quad_pos);
  r = this->regSet(r, this->sr_ctrl.spi_faddr_dual, this->sr_ctrl.spi_faddr_dual_pos);
  r = this->regSet(r, this->sr_ctrl.spi_dummy_out, this->sr_ctrl.spi_dummy_out_pos);
	if(writeMode){
		this->regWrite(this->sr_ctrl.base_2, r);
		this->init_sr_ctrl();
	}else{
		this->sr_ctrl.val = r;
	}
}
void SpiDebug::patch_sr_msdlen(bool writeMode){
	uint32_t r = this->sr_msdlen.val;
	
	r = this->regSet(r, this->sr_msdlen.spi_ms_data_bitlen, this->sr_msdlen.spi_ms_data_bitlen_x, this->sr_msdlen.spi_ms_data_bitlen_y);
	if(writeMode){
		this->regWrite(this->sr_msdlen.base_2, r);
		this->init_sr_msdlen();
	}else{
		this->sr_msdlen.val = r;
	}
}
void SpiDebug::patch_sr_misc(bool writeMode){
	uint32_t r = this->sr_misc.val;
	r = this->regSet(r, this->sr_misc.spi_quad_din_pin_swap, this->sr_misc.spi_quad_din_pin_swap_pos);
	r = this->regSet(r, this->sr_misc.spi_cs_keep_active, this->sr_misc.spi_cs_keep_active_pos);
	r = this->regSet(r, this->sr_misc.spi_ck_idle_edge, this->sr_misc.spi_ck_idle_edge_pos);
	r = this->regSet(r, this->sr_misc.spi_dqs_idle_edge, this->sr_misc.spi_dqs_idle_edge_pos);
	r = this->regSet(r, this->sr_misc.spi_slave_cs_pol, this->sr_misc.spi_slave_cs_pol_pos);
	r = this->regSet(r, this->sr_misc.spi_cmd_dtr_en, this->sr_misc.spi_cmd_dtr_en_pos);
	r = this->regSet(r, this->sr_misc.spi_addr_dtr_en, this->sr_misc.spi_addr_dtr_en_pos);
	r = this->regSet(r, this->sr_misc.spi_data_dtr_en, this->sr_misc.spi_data_dtr_en_pos);
	r = this->regSet(r, this->sr_misc.spi_clk_data_dtr_en, this->sr_misc.spi_clk_data_dtr_en_pos);

	if(this->spiMode == 3){
		r = this->regSet(r, this->sr_misc.spi_master_cs_pol, this->sr_misc.spi_master_cs_pol_x, this->sr_misc.spi_master_cs_pol_y3);
	}else{
		r = this->regSet(r, this->sr_misc.spi_master_cs_pol, this->sr_misc.spi_master_cs_pol_x, this->sr_misc.spi_master_cs_pol_y);
	}

	r = this->regSet(r, this->sr_misc.spi_ck_dis, this->sr_misc.spi_ck_dis_pos);
	r = this->regSet(r, this->sr_misc.spi_cs5_dis, this->sr_misc.spi_cs5_dis_pos);
	r = this->regSet(r, this->sr_misc.spi_cs4_dis, this->sr_misc.spi_cs4_dis_pos);
	r = this->regSet(r, this->sr_misc.spi_cs3_dis, this->sr_misc.spi_cs3_dis_pos);
	r = this->regSet(r, this->sr_misc.spi_cs2_dis, this->sr_misc.spi_cs2_dis_pos);
	r = this->regSet(r, this->sr_misc.spi_cs1_dis, this->sr_misc.spi_cs1_dis_pos);
	r = this->regSet(r, this->sr_misc.spi_cs0_dis, this->sr_misc.spi_cs0_dis_pos);
	if(writeMode){
		this->regWrite(this->sr_misc.base_2, r);
		this->init_sr_misc();
	}else{
		this->sr_misc.val = r;
	}
      
}
void SpiDebug::patch_sr_dmaconf(bool writeMode){
	uint32_t r = this->sr_dmaconf.val;
  r = this->regSet(r, this->sr_dmaconf.spi_dma_afifo_rst, this->sr_dmaconf.spi_dma_afifo_rst_pos);
  r = this->regSet(r, this->sr_dmaconf.spi_buf_afifo_rst, this->sr_dmaconf.spi_buf_afifo_rst_pos);
  r = this->regSet(r, this->sr_dmaconf.spi_rx_afifo_rst, this->sr_dmaconf.spi_rx_afifo_rst_pos);
  r = this->regSet(r, this->sr_dmaconf.spi_dma_tx_ena, this->sr_dmaconf.spi_dma_tx_ena_pos);
  r = this->regSet(r, this->sr_dmaconf.spi_dma_rx_ena, this->sr_dmaconf.spi_dma_rx_ena_pos);
  r = this->regSet(r, this->sr_dmaconf.spi_rx_eof_en, this->sr_dmaconf.spi_rx_eof_en_pos);
  r = this->regSet(r, this->sr_dmaconf.spi_slv_tx_seg_trans_clr_en, this->sr_dmaconf.spi_slv_tx_seg_trans_clr_en_pos);
  r = this->regSet(r, this->sr_dmaconf.spi_slv_rx_seg_trans_clr_en, this->sr_dmaconf.spi_slv_rx_seg_trans_clr_en_pos);
  r = this->regSet(r, this->sr_dmaconf.spi_dma_slv_seg_trans_en, this->sr_dmaconf.spi_dma_slv_seg_trans_en_pos);
  r = this->regSet(r, this->sr_dmaconf.spi_dma_infifo_full, this->sr_dmaconf.spi_dma_infifo_full_pos);
  r = this->regSet(r, this->sr_dmaconf.spi_dma_outfifo_empty, this->sr_dmaconf.spi_dma_outfifo_empty_pos);
	if(writeMode){
		this->regWrite(this->sr_dmaconf.base_2, r);
		this->init_sr_dmaconf();
	}else{
		this->sr_dmaconf.val = r;
	}
}
void SpiDebug::patch_sr_slave(bool writeMode){
	uint32_t r = this->sr_slave.val;
	r = this->regSet(r, this->sr_slave.r_spi_usr_conf, this->sr_slave.r_spi_usr_conf_pos);
	r = this->regSet(r, this->sr_slave.spi_soft_reset, this->sr_slave.spi_soft_reset_pos);
	r = this->regSet(r, this->sr_slave.spi_slave_mode, this->sr_slave.spi_slave_mode_pos);

	r = this->regSet(r, this->sr_slave.r_spi_dma_seg_magic_value, this->sr_slave.r_spi_dma_seg_magic_value_x, this->sr_slave.r_spi_dma_seg_magic_value_y);

	r = this->regSet(r, this->sr_slave.spi_slv_wrbuf_bitlen_en, this->sr_slave.spi_slv_wrbuf_bitlen_en_pos);
	r = this->regSet(r, this->sr_slave.spi_slv_rdbuf_bitlen_en, this->sr_slave.spi_slv_rdbuf_bitlen_en_pos);
	r = this->regSet(r, this->sr_slave.spi_slv_wrdma_bitlen_en, this->sr_slave.spi_slv_wrdma_bitlen_en_pos);
	r = this->regSet(r, this->sr_slave.spi_slv_rddma_bitlen_en, this->sr_slave.spi_slv_rddma_bitlen_en_pos);
	r = this->regSet(r, this->sr_slave.spi_rsck_data_out, this->sr_slave.spi_rsck_data_out_pos);
	r = this->regSet(r, this->sr_slave.spi_clk_mode_13, this->sr_slave.spi_clk_mode_13_pos);

	r = this->regSet(r, this->sr_slave.spi_clk_mode, this->sr_slave.spi_clk_mode_x, this->sr_slave.spi_clk_mode_y);

	if(writeMode){
		this->regWrite(this->sr_slave.base_2, r);
		this->init_sr_slave();
	}else{
		this->sr_slave.val = r;
	}
}
void SpiDebug::patch_sr_slave1(bool writeMode){
	uint32_t r = this->sr_slave1.val;
	r = this->regSet(r, this->sr_slave1.spi_slv_last_addr, this->sr_slave1.spi_slv_last_addr_x, this->sr_slave1.spi_slv_last_addr_y);

  	r = this->regSet(r, this->sr_slave1.spi_slv_last_command, this->sr_slave1.spi_slv_last_command_x, this->sr_slave1.spi_slv_last_command_y);

	r = this->regSet(r, this->sr_slave1.spi_slv_data_bitlen, this->sr_slave1.spi_slv_data_bitlen_x, this->sr_slave1.spi_slv_data_bitlen_y);

	if(writeMode){
		this->regWrite(this->sr_slave1.base_2, r);
		this->init_sr_slave1();
	}else{
		this->sr_slave1.val = r;
	}

}

void SpiDebug::patchClock(bool writeMode){
	this->patch_sr_clock(writeMode);
	this->patch_sr_gate(writeMode);
}
void SpiDebug::patch_sr_clock(bool writeMode){
	uint32_t r = this->sr_clock.val;
	 r = this->regSet(r, this->sr_clock.spi_clk_equ_sysclk, this->sr_clock.spi_clk_equ_sysclk_pos);

	r = this->regSet(r, this->sr_clock.spi_clkdiv_pre, this->sr_clock.spi_clkdiv_pre_x, this->sr_clock.spi_clkdiv_pre_y);

	r = this->regSet(r, this->sr_clock.spi_clkcnt_n, this->sr_clock.spi_clkcnt_n_x, this->sr_clock.spi_clkcnt_n_y);

	r = this->regSet(r, this->sr_clock.spi_clkcnt_h, this->sr_clock.spi_clkcnt_h_x, this->sr_clock.spi_clkcnt_h_y);

	r = this->regSet(r, this->sr_clock.spi_clkcnt_l, this->sr_clock.spi_clkcnt_l_x, this->sr_clock.spi_clkcnt_l_y);
	if(writeMode){
		this->regWrite(this->sr_clock.base_2, r);
		this->init_sr_clock();
	}else{
		this->sr_clock.val = r;
	}
}

void SpiDebug::patch_sr_gate(bool writeMode){
	uint32_t r = this->sr_gate.val;

	r = this->regSet(r, this->sr_gate.spi_mst_clk_sel, this->sr_gate.spi_mst_clk_sel_pos);
  	r = this->regSet(r, this->sr_gate.spi_mst_clk_active, this->sr_gate.spi_mst_clk_active_pos);
 	r = this->regSet(r, this->sr_gate.spi_clk_en, this->sr_gate.spi_clk_en_pos);
	
	if(writeMode){
		this->regWrite(this->sr_gate.base_2, r);
		this->init_sr_gate();
	}else{
		this->sr_gate.val = r;
	}
}

void SpiDebug::patchTiming(bool writeMode){
	this->patch_sr_dinmode(writeMode);
	this->patch_sr_dinnum(writeMode);
	this->patch_sr_doutmode(writeMode);
}
void SpiDebug::patch_sr_dinmode(bool writeMode){
	uint32_t r = this->sr_dinmode.val;

	r = this->regSet(r, this->sr_dinmode.spi_timing_hclk_active, this->sr_dinmode.spi_timing_hclk_active_pos);

  	r = this->regSet(r, this->sr_dinmode.r_spi_din7_mode, this->sr_dinmode.r_spi_din7_mode_x, this->sr_dinmode.r_spi_din7_mode_y); 

  	r = this->regSet(r, this->sr_dinmode.r_spi_din6_mode, this->sr_dinmode.r_spi_din6_mode_x, this->sr_dinmode.r_spi_din6_mode_y); 

  	r = this->regSet(r, this->sr_dinmode.r_spi_din5_mode, this->sr_dinmode.r_spi_din5_mode_x, this->sr_dinmode.r_spi_din5_mode_y); 

  	r = this->regSet(r, this->sr_dinmode.r_spi_din4_mode, this->sr_dinmode.r_spi_din4_mode_x, this->sr_dinmode.r_spi_din4_mode_y); 

  	r = this->regSet(r, this->sr_dinmode.spi_din3_mode, this->sr_dinmode.spi_din3_mode_x, this->sr_dinmode.spi_din3_mode_y); 

  	r = this->regSet(r, this->sr_dinmode.spi_din2_mode, this->sr_dinmode.spi_din2_mode_x, this->sr_dinmode.spi_din2_mode_y); 

  	r = this->regSet(r, this->sr_dinmode.spi_din1_mode, this->sr_dinmode.spi_din1_mode_x, this->sr_dinmode.spi_din1_mode_y); 

  	r = this->regSet(r, this->sr_dinmode.spi_din0_mode, this->sr_dinmode.spi_din0_mode_x, this->sr_dinmode.spi_din0_mode_y); 

	if(writeMode){
		this->regWrite(this->sr_dinmode.base_2, r);
		this->init_sr_dinmode();
	}else{
		this->sr_dinmode.val = r;
	}

}
void SpiDebug::patch_sr_dinnum(bool writeMode){
	uint32_t r = this->sr_dinnum.val;

  	r = this->regSet(r, this->sr_dinnum.r_spi_din7_num, this->sr_dinnum.r_spi_din7_num_x, this->sr_dinnum.r_spi_din7_num_y);

  	r = this->regSet(r, this->sr_dinnum.r_spi_din6_num, this->sr_dinnum.r_spi_din6_num_x, this->sr_dinnum.r_spi_din6_num_y);

  	r = this->regSet(r, this->sr_dinnum.r_spi_din5_num, this->sr_dinnum.r_spi_din5_num_x, this->sr_dinnum.r_spi_din5_num_y);

  	r = this->regSet(r, this->sr_dinnum.r_spi_din4_num, this->sr_dinnum.r_spi_din4_num_x, this->sr_dinnum.r_spi_din4_num_y);

  	r = this->regSet(r, this->sr_dinnum.spi_din3_num, this->sr_dinnum.spi_din3_num_x, this->sr_dinnum.spi_din3_num_y);

  	r = this->regSet(r, this->sr_dinnum.spi_din2_num, this->sr_dinnum.spi_din2_num_x, this->sr_dinnum.spi_din2_num_y);

  	r = this->regSet(r, this->sr_dinnum.spi_din1_num, this->sr_dinnum.spi_din1_num_x, this->sr_dinnum.spi_din1_num_y);

	  r = this->regSet(r, this->sr_dinnum.spi_din0_num, this->sr_dinnum.spi_din0_num_x, this->sr_dinnum.spi_din0_num_y);

	if(writeMode){
		this->regWrite(this->sr_dinnum.base_2, r);
		this->init_sr_dinnum();
	}else{
		this->sr_dinnum.val = r;
	}
}
void SpiDebug::patch_sr_doutmode(bool writeMode){
	uint32_t r = this->sr_doutmode.val;
		
 	r = this->regSet(r, this->sr_doutmode.r_spi_d_dqs_mode, this->sr_doutmode.r_spi_d_dqs_mode_pos);
  	r = this->regSet(r, this->sr_doutmode.r_spi_dout7_mode, this->sr_doutmode.r_spi_dout7_mode_pos);
  	r = this->regSet(r, this->sr_doutmode.r_spi_dout6_mode, this->sr_doutmode.r_spi_dout6_mode_pos);
  	r = this->regSet(r, this->sr_doutmode.r_spi_dout5_mode, this->sr_doutmode.r_spi_dout5_mode_pos);
  	r = this->regSet(r, this->sr_doutmode.r_spi_dout4_mode, this->sr_doutmode.r_spi_dout4_mode_pos);
  	r = this->regSet(r, this->sr_doutmode.spi_dout3_mode, this->sr_doutmode.spi_dout3_mode_pos);
  	r = this->regSet(r, this->sr_doutmode.spi_dout2_mode, this->sr_doutmode.spi_dout2_mode_pos);
  	r = this->regSet(r, this->sr_doutmode.spi_dout1_mode, this->sr_doutmode.spi_dout1_mode_pos);
  	r = this->regSet(r, this->sr_doutmode.spi_dout0_mode, this->sr_doutmode.spi_dout0_mode_pos);


	if(writeMode){
		this->regWrite(this->sr_doutmode.base_2, r);
		this->init_sr_doutmode();
	}else{
		this->sr_doutmode.val = r;
	}
}

void SpiDebug::patchInterupts(bool writeMode){
	this->patch_sr_dmaintena(writeMode);
	this->patch_sr_dmaintclr(writeMode);
	this->patch_sr_dmaintraw(writeMode);
	this->patch_sr_dmaintst(writeMode);
	this->patch_sr_dmaintset(writeMode);
}
void SpiDebug::patch_sr_dmaintena(bool writeMode){
	uint32_t r = this->sr_dmaintena.val;
	
	r = this->regSet(r, this->sr_dmaintena.spi_app1_int_ena, this->sr_dmaintena.spi_app1_int_ena_pos);
  	r = this->regSet(r, this->sr_dmaintena.spi_app2_int_ena, this->sr_dmaintena.spi_app2_int_ena_pos);
  	r = this->regSet(r, this->sr_dmaintena.spi_mst_tx_afifo_rempty_err_int_ena, this->sr_dmaintena.spi_mst_tx_afifo_rempty_err_int_ena_pos);
  	r = this->regSet(r, this->sr_dmaintena.spi_mst_rx_afifo_wfull_err_int_ena, this->sr_dmaintena.spi_mst_rx_afifo_wfull_err_int_ena_pos);
  	r = this->regSet(r, this->sr_dmaintena.spi_slv_cmd_err_int_ena, this->sr_dmaintena.spi_slv_cmd_err_int_ena_pos);
  	r = this->regSet(r, this->sr_dmaintena.r_spi_seg_magic_err_int_ena, this->sr_dmaintena.r_spi_seg_magic_err_int_ena_pos);
  	r = this->regSet(r, this->sr_dmaintena.spi_dma_seg_trans_done_int_ena, this->sr_dmaintena.spi_dma_seg_trans_done_int_ena_pos);
  	r = this->regSet(r, this->sr_dmaintena.spi_trans_done_int_ena, this->sr_dmaintena.spi_trans_done_int_ena_pos);
  	r = this->regSet(r, this->sr_dmaintena.spi_slv_wr_buf_done_int_ena, this->sr_dmaintena.spi_slv_wr_buf_done_int_ena_pos);
  	r = this->regSet(r, this->sr_dmaintena.spi_slv_rd_buf_done_int_ena, this->sr_dmaintena.spi_slv_rd_buf_done_int_ena_pos);
  	r = this->regSet(r, this->sr_dmaintena.spi_slv_wr_dma_done_int_ena, this->sr_dmaintena.spi_slv_wr_dma_done_int_ena_pos);
  	r = this->regSet(r, this->sr_dmaintena.spi_slv_rd_dma_done_int_ena, this->sr_dmaintena.spi_slv_rd_dma_done_int_ena_pos);
  	r = this->regSet(r, this->sr_dmaintena.spi_slv_cmda_int_ena, this->sr_dmaintena.spi_slv_cmda_int_ena_pos);
  	r = this->regSet(r, this->sr_dmaintena.spi_slv_cmd9_int_ena, this->sr_dmaintena.spi_slv_cmd9_int_ena_pos);
  	r = this->regSet(r, this->sr_dmaintena.spi_slv_cmd8_int_ena, this->sr_dmaintena.spi_slv_cmd8_int_ena_pos);
  	r = this->regSet(r, this->sr_dmaintena.spi_slv_cmd7_int_ena, this->sr_dmaintena.spi_slv_cmd7_int_ena_pos);
  	r = this->regSet(r, this->sr_dmaintena.spi_slv_en_qpi_int_ena, this->sr_dmaintena.spi_slv_en_qpi_int_ena_pos);
  	r = this->regSet(r, this->sr_dmaintena.spi_slv_ex_qpi_int_ena, this->sr_dmaintena.spi_slv_ex_qpi_int_ena_pos);
	r = this->regSet(r, this->sr_dmaintena.spi_dma_outfifo_empty_err_int_ena, this->sr_dmaintena.spi_dma_outfifo_empty_err_int_ena_pos);
  	r = this->regSet(r, this->sr_dmaintena.spi_dma_infifo_full_err_int_ena, this->sr_dmaintena.spi_dma_infifo_full_err_int_ena_pos);

	if(writeMode){
		this->regWrite(this->sr_dmaintena.base_2, r);
		this->init_sr_dmaintena();
	}else{
		this->sr_dmaintena.val = r;	
	}
}
void SpiDebug::patch_sr_dmaintclr(bool writeMode){
	uint32_t r = this->sr_dmaintclr.val;
	
	r = this->regSet(r, this->sr_dmaintclr.spi_app1_int_clr, this->sr_dmaintclr.spi_app1_int_clr_pos);
  	r = this->regSet(r, this->sr_dmaintclr.spi_app2_int_clr, this->sr_dmaintclr.spi_app2_int_clr_pos);
  	r = this->regSet(r, this->sr_dmaintclr.spi_mst_tx_afifo_rempty_err_int_clr, this->sr_dmaintclr.spi_mst_tx_afifo_rempty_err_int_clr_pos);
  	r = this->regSet(r, this->sr_dmaintclr.spi_mst_rx_afifo_wfull_err_int_clr, this->sr_dmaintclr.spi_mst_rx_afifo_wfull_err_int_clr_pos);
  	r = this->regSet(r, this->sr_dmaintclr.spi_slv_cmd_err_int_clr, this->sr_dmaintclr.spi_slv_cmd_err_int_clr_pos);
  	r = this->regSet(r, this->sr_dmaintclr.r_spi_seg_magic_err_int_clr, this->sr_dmaintclr.r_spi_seg_magic_err_int_clr_pos);
  	r = this->regSet(r, this->sr_dmaintclr.spi_dma_seg_trans_done_int_clr, this->sr_dmaintclr.spi_dma_seg_trans_done_int_clr_pos);
  	r = this->regSet(r, this->sr_dmaintclr.spi_trans_done_int_clr, this->sr_dmaintclr.spi_trans_done_int_clr_pos);
  	r = this->regSet(r, this->sr_dmaintclr.spi_slv_wr_buf_done_int_clr, this->sr_dmaintclr.spi_slv_wr_buf_done_int_clr_pos);
  	r = this->regSet(r, this->sr_dmaintclr.spi_slv_rd_buf_done_int_clr, this->sr_dmaintclr.spi_slv_rd_buf_done_int_clr_pos);
  	r = this->regSet(r, this->sr_dmaintclr.spi_slv_wr_dma_done_int_clr, this->sr_dmaintclr.spi_slv_wr_dma_done_int_clr_pos);
  	r = this->regSet(r, this->sr_dmaintclr.spi_slv_rd_dma_done_int_clr, this->sr_dmaintclr.spi_slv_rd_dma_done_int_clr_pos);
  	r = this->regSet(r, this->sr_dmaintclr.spi_slv_cmda_int_clr, this->sr_dmaintclr.spi_slv_cmda_int_clr_pos);
  	r = this->regSet(r, this->sr_dmaintclr.spi_slv_cmd9_int_clr, this->sr_dmaintclr.spi_slv_cmd9_int_clr_pos);
  	r = this->regSet(r, this->sr_dmaintclr.spi_slv_cmd8_int_clr, this->sr_dmaintclr.spi_slv_cmd8_int_clr_pos);
  	r = this->regSet(r, this->sr_dmaintclr.spi_slv_cmd7_int_clr, this->sr_dmaintclr.spi_slv_cmd7_int_clr_pos);
  	r = this->regSet(r, this->sr_dmaintclr.spi_slv_en_qpi_int_clr, this->sr_dmaintclr.spi_slv_en_qpi_int_clr_pos);
  	r = this->regSet(r, this->sr_dmaintclr.spi_slv_ex_qpi_int_clr, this->sr_dmaintclr.spi_slv_ex_qpi_int_clr_pos);
	r = this->regSet(r, this->sr_dmaintclr.spi_dma_outfifo_empty_err_int_clr, this->sr_dmaintclr.spi_dma_outfifo_empty_err_int_clr_pos);
  	r = this->regSet(r, this->sr_dmaintclr.spi_dma_infifo_full_err_int_clr, this->sr_dmaintclr.spi_dma_infifo_full_err_int_clr_pos);

	if(writeMode){
		this->regWrite(this->sr_dmaintclr.base_2, r);
		this->init_sr_dmaintclr();
	}else{
		this->sr_dmaintclr.val = r;	
	}
}
void SpiDebug::patch_sr_dmaintraw(bool writeMode){
	uint32_t r = this->sr_dmaintraw.val;
	
	r = this->regSet(r, this->sr_dmaintraw.spi_app1_int_raw, this->sr_dmaintraw.spi_app1_int_raw_pos);
  	r = this->regSet(r, this->sr_dmaintraw.spi_app2_int_raw, this->sr_dmaintraw.spi_app2_int_raw_pos);
  	r = this->regSet(r, this->sr_dmaintraw.spi_mst_tx_afifo_rempty_err_int_raw, this->sr_dmaintraw.spi_mst_tx_afifo_rempty_err_int_raw_pos);
  	r = this->regSet(r, this->sr_dmaintraw.spi_mst_rx_afifo_wfull_err_int_raw, this->sr_dmaintraw.spi_mst_rx_afifo_wfull_err_int_raw_pos);
  	r = this->regSet(r, this->sr_dmaintraw.spi_slv_cmd_err_int_raw, this->sr_dmaintraw.spi_slv_cmd_err_int_raw_pos);
  	r = this->regSet(r, this->sr_dmaintraw.r_spi_seg_magic_err_int_raw, this->sr_dmaintraw.r_spi_seg_magic_err_int_raw_pos);
  	r = this->regSet(r, this->sr_dmaintraw.spi_dma_seg_trans_done_int_raw, this->sr_dmaintraw.spi_dma_seg_trans_done_int_raw_pos);
  	r = this->regSet(r, this->sr_dmaintraw.spi_trans_done_int_raw, this->sr_dmaintraw.spi_trans_done_int_raw_pos);
  	r = this->regSet(r, this->sr_dmaintraw.spi_slv_wr_buf_done_int_raw, this->sr_dmaintraw.spi_slv_wr_buf_done_int_raw_pos);
  	r = this->regSet(r, this->sr_dmaintraw.spi_slv_rd_buf_done_int_raw, this->sr_dmaintraw.spi_slv_rd_buf_done_int_raw_pos);
  	r = this->regSet(r, this->sr_dmaintraw.spi_slv_wr_dma_done_int_raw, this->sr_dmaintraw.spi_slv_wr_dma_done_int_raw_pos);
  	r = this->regSet(r, this->sr_dmaintraw.spi_slv_rd_dma_done_int_raw, this->sr_dmaintraw.spi_slv_rd_dma_done_int_raw_pos);
  	r = this->regSet(r, this->sr_dmaintraw.spi_slv_cmda_int_raw, this->sr_dmaintraw.spi_slv_cmda_int_raw_pos);
  	r = this->regSet(r, this->sr_dmaintraw.spi_slv_cmd9_int_raw, this->sr_dmaintraw.spi_slv_cmd9_int_raw_pos);
  	r = this->regSet(r, this->sr_dmaintraw.spi_slv_cmd8_int_raw, this->sr_dmaintraw.spi_slv_cmd8_int_raw_pos);
  	r = this->regSet(r, this->sr_dmaintraw.spi_slv_cmd7_int_raw, this->sr_dmaintraw.spi_slv_cmd7_int_raw_pos);
  	r = this->regSet(r, this->sr_dmaintraw.spi_slv_en_qpi_int_raw, this->sr_dmaintraw.spi_slv_en_qpi_int_raw_pos);
  	r = this->regSet(r, this->sr_dmaintraw.spi_slv_ex_qpi_int_raw, this->sr_dmaintraw.spi_slv_ex_qpi_int_raw_pos);
	r = this->regSet(r, this->sr_dmaintraw.spi_dma_outfifo_empty_err_int_raw, this->sr_dmaintraw.spi_dma_outfifo_empty_err_int_raw_pos);
  	r = this->regSet(r, this->sr_dmaintraw.spi_dma_infifo_full_err_int_raw, this->sr_dmaintraw.spi_dma_infifo_full_err_int_raw_pos);

	if(writeMode){
		this->regWrite(this->sr_dmaintraw.base_2, r);
		this->init_sr_dmaintraw();
	}else{
		this->sr_dmaintraw.val = r;	
	}
}
void SpiDebug::patch_sr_dmaintst(bool writeMode){
	uint32_t r = this->sr_dmaintst.val;
	
	r = this->regSet(r, this->sr_dmaintst.spi_app1_int_st, this->sr_dmaintst.spi_app1_int_st_pos);
  	r = this->regSet(r, this->sr_dmaintst.spi_app2_int_st, this->sr_dmaintst.spi_app2_int_st_pos);
  	r = this->regSet(r, this->sr_dmaintst.spi_mst_tx_afifo_rempty_err_int_st, this->sr_dmaintst.spi_mst_tx_afifo_rempty_err_int_st_pos);
  	r = this->regSet(r, this->sr_dmaintst.spi_mst_rx_afifo_wfull_err_int_st, this->sr_dmaintst.spi_mst_rx_afifo_wfull_err_int_st_pos);
  	r = this->regSet(r, this->sr_dmaintst.spi_slv_cmd_err_int_st, this->sr_dmaintst.spi_slv_cmd_err_int_st_pos);
  	r = this->regSet(r, this->sr_dmaintst.r_spi_seg_magic_err_int_st, this->sr_dmaintst.r_spi_seg_magic_err_int_st_pos);
  	r = this->regSet(r, this->sr_dmaintst.spi_dma_seg_trans_done_int_st, this->sr_dmaintst.spi_dma_seg_trans_done_int_st_pos);
  	r = this->regSet(r, this->sr_dmaintst.spi_trans_done_int_st, this->sr_dmaintst.spi_trans_done_int_st_pos);
  	r = this->regSet(r, this->sr_dmaintst.spi_slv_wr_buf_done_int_st, this->sr_dmaintst.spi_slv_wr_buf_done_int_st_pos);
  	r = this->regSet(r, this->sr_dmaintst.spi_slv_rd_buf_done_int_st, this->sr_dmaintst.spi_slv_rd_buf_done_int_st_pos);
  	r = this->regSet(r, this->sr_dmaintst.spi_slv_wr_dma_done_int_st, this->sr_dmaintst.spi_slv_wr_dma_done_int_st_pos);
  	r = this->regSet(r, this->sr_dmaintst.spi_slv_rd_dma_done_int_st, this->sr_dmaintst.spi_slv_rd_dma_done_int_st_pos);
  	r = this->regSet(r, this->sr_dmaintst.spi_slv_cmda_int_st, this->sr_dmaintst.spi_slv_cmda_int_st_pos);
  	r = this->regSet(r, this->sr_dmaintst.spi_slv_cmd9_int_st, this->sr_dmaintst.spi_slv_cmd9_int_st_pos);
  	r = this->regSet(r, this->sr_dmaintst.spi_slv_cmd8_int_st, this->sr_dmaintst.spi_slv_cmd8_int_st_pos);
  	r = this->regSet(r, this->sr_dmaintst.spi_slv_cmd7_int_st, this->sr_dmaintst.spi_slv_cmd7_int_st_pos);
  	r = this->regSet(r, this->sr_dmaintst.spi_slv_en_qpi_int_st, this->sr_dmaintst.spi_slv_en_qpi_int_st_pos);
  	r = this->regSet(r, this->sr_dmaintst.spi_slv_ex_qpi_int_st, this->sr_dmaintst.spi_slv_ex_qpi_int_st_pos);
	r = this->regSet(r, this->sr_dmaintst.spi_dma_outfifo_empty_err_int_st, this->sr_dmaintst.spi_dma_outfifo_empty_err_int_st_pos);
  	r = this->regSet(r, this->sr_dmaintst.spi_dma_infifo_full_err_int_st, this->sr_dmaintst.spi_dma_infifo_full_err_int_st_pos);

	if(writeMode){
		this->regWrite(this->sr_dmaintst.base_2, r);
		this->init_sr_dmaintst();
	}else{
		this->sr_dmaintst.val = r;	
	}

}
void SpiDebug::patch_sr_dmaintset(bool writeMode){
	uint32_t r = this->sr_dmaintset.val;
	
	r = this->regSet(r, this->sr_dmaintset.spi_app1_int_set, this->sr_dmaintset.spi_app1_int_set_pos);
  	r = this->regSet(r, this->sr_dmaintset.spi_app2_int_set, this->sr_dmaintset.spi_app2_int_set_pos);
  	r = this->regSet(r, this->sr_dmaintset.spi_mst_tx_afifo_rempty_err_int_set, this->sr_dmaintset.spi_mst_tx_afifo_rempty_err_int_set_pos);
  	r = this->regSet(r, this->sr_dmaintset.spi_mst_rx_afifo_wfull_err_int_set, this->sr_dmaintset.spi_mst_rx_afifo_wfull_err_int_set_pos);
  	r = this->regSet(r, this->sr_dmaintset.spi_slv_cmd_err_int_set, this->sr_dmaintset.spi_slv_cmd_err_int_set_pos);
  	r = this->regSet(r, this->sr_dmaintset.r_spi_seg_magic_err_int_set, this->sr_dmaintset.r_spi_seg_magic_err_int_set_pos);
  	r = this->regSet(r, this->sr_dmaintset.spi_dma_seg_trans_done_int_set, this->sr_dmaintset.spi_dma_seg_trans_done_int_set_pos);
  	r = this->regSet(r, this->sr_dmaintset.spi_trans_done_int_set, this->sr_dmaintset.spi_trans_done_int_set_pos);
  	r = this->regSet(r, this->sr_dmaintset.spi_slv_wr_buf_done_int_set, this->sr_dmaintset.spi_slv_wr_buf_done_int_set_pos);
  	r = this->regSet(r, this->sr_dmaintset.spi_slv_rd_buf_done_int_set, this->sr_dmaintset.spi_slv_rd_buf_done_int_set_pos);
  	r = this->regSet(r, this->sr_dmaintset.spi_slv_wr_dma_done_int_set, this->sr_dmaintset.spi_slv_wr_dma_done_int_set_pos);
  	r = this->regSet(r, this->sr_dmaintset.spi_slv_rd_dma_done_int_set, this->sr_dmaintset.spi_slv_rd_dma_done_int_set_pos);
  	r = this->regSet(r, this->sr_dmaintset.spi_slv_cmda_int_set, this->sr_dmaintset.spi_slv_cmda_int_set_pos);
  	r = this->regSet(r, this->sr_dmaintset.spi_slv_cmd9_int_set, this->sr_dmaintset.spi_slv_cmd9_int_set_pos);
  	r = this->regSet(r, this->sr_dmaintset.spi_slv_cmd8_int_set, this->sr_dmaintset.spi_slv_cmd8_int_set_pos);
  	r = this->regSet(r, this->sr_dmaintset.spi_slv_cmd7_int_set, this->sr_dmaintset.spi_slv_cmd7_int_set_pos);
  	r = this->regSet(r, this->sr_dmaintset.spi_slv_en_qpi_int_set, this->sr_dmaintset.spi_slv_en_qpi_int_set_pos);
  	r = this->regSet(r, this->sr_dmaintset.spi_slv_ex_qpi_int_set, this->sr_dmaintset.spi_slv_ex_qpi_int_set_pos);
	r = this->regSet(r, this->sr_dmaintset.spi_dma_outfifo_empty_err_int_set, this->sr_dmaintset.spi_dma_outfifo_empty_err_int_set_pos);
  	r = this->regSet(r, this->sr_dmaintset.spi_dma_infifo_full_err_int_set, this->sr_dmaintset.spi_dma_infifo_full_err_int_set_pos);


	if(writeMode){
		this->regWrite(this->sr_dmaintset.base_2, r);
		this->init_sr_dmaintset();
	}else{
		this->sr_dmaintset.val = r;	
	}
}


void SpiDebug::patchAll(bool writeMode){
	this->patchUserDef(writeMode);
	this->patchCtrlConf(writeMode);
	this->patchClock(writeMode);
	this->patchTiming(writeMode);
	this->patchInterupts(writeMode);
}

void SpiDebug::printComp(const char *n1, int v1, int v2, const char *n2){
	Serial.printf("%s (0x%X) vs (0x%x) %s\n", n1, v1, v2, n2);
}
bool SpiDebug::compUserDef(SpiDebug cmp, bool printout){
	if(printout) Serial.printf("~~~User Defined Register Compairisons~~~\n");

	bool ret = true;
	bool test = this->comp_sr_cmd(cmp, printout);
	printf("Test dng : %d\n", test);
	ret &= test;
	test = this->comp_sr_addr(cmp, printout);
	printf("Test dng : %d\n", test);
	ret &= test;
	test = this->comp_sr_user(cmp, printout);
	printf("Test dng : %d\n", test);
	ret &= test;
	test = this->comp_sr_user1(cmp, printout);
	printf("Test dng : %d\n", test);
	ret &= test;
	test = this->comp_sr_user2(cmp, printout);
	printf("Test dng : %d\n", test);
	ret &= test;
	return ret;
}
bool SpiDebug::comp_sr_cmd(SpiDebug cmp, bool printout){
	bool ret = true;
	bool test = this->sr_cmd.spi_user == cmp.sr_cmd.spi_user;
	if(printout && !test)
		this->printComp("spi_user", this->sr_cmd.spi_user, cmp.sr_cmd.spi_user, "<- cmp");
	ret &= test;

	test = this->sr_cmd.spi_update == cmp.sr_cmd.spi_update;
	if(printout && !test)
		this->printComp("spi_update", this->sr_cmd.spi_update, cmp.sr_cmd.spi_update, "<- cmp");
	ret &= test;

	if(!ret && printout) Serial.println();
	return ret;
}
bool SpiDebug::comp_sr_addr(SpiDebug cmp, bool printout){
	bool ret = true;
	bool test = this->sr_addr.spi_usr_addr_value == cmp.sr_addr.spi_usr_addr_value;
	if(printout && !test)
		this->printComp("spi_usr_addR_value", this->sr_addr.spi_usr_addr_value, cmp.sr_addr.spi_usr_addr_value, "<- cmp");
	ret &= test;
	return ret;
}
bool SpiDebug::comp_sr_user(SpiDebug cmp, bool printout){
	bool ret = true;
	bool test = this->sr_user.spi_usr_command == cmp.sr_user.spi_usr_command;
	if(printout && !test)
		this->printComp("spi_usr_command", this->sr_user.spi_usr_command, cmp.sr_user.spi_usr_command, "<- cmp");
	ret &= test;
	
	test = this->sr_user.spi_usr_addr == cmp.sr_user.spi_usr_addr;
	if(printout && !test)
		this->printComp("spi_usr_addr", this->sr_user.spi_usr_addr, cmp.sr_user.spi_usr_addr, "<- cmp");
	ret &= test;
	
	test = this->sr_user.spi_usr_dummy == cmp.sr_user.spi_usr_dummy;
	if(printout && !test)
		this->printComp("spi_usr_dummy", this->sr_user.spi_usr_dummy, cmp.sr_user.spi_usr_dummy, "<- cmp");
	ret &= test;
	
	test = this->sr_user.spi_usr_miso == cmp.sr_user.spi_usr_miso;
	if(printout && !test)
		this->printComp("spi_usr_miso", this->sr_user.spi_usr_miso, cmp.sr_user.spi_usr_miso, "<- cmp");
	ret &= test;
	
	test = this->sr_user.spi_usr_mosi == cmp.sr_user.spi_usr_mosi;
	if(printout && !test)
		this->printComp("spi_usr_mosi", this->sr_user.spi_usr_mosi, cmp.sr_user.spi_usr_mosi, "<- cmp");
	ret &= test;
	
	test = this->sr_user.spi_usr_dummy_idle == cmp.sr_user.spi_usr_dummy_idle;
	if(printout && !test)
		this->printComp("spi_usr_dummy_idle", this->sr_user.spi_usr_dummy_idle, cmp.sr_user.spi_usr_dummy_idle, "<- cmp");
	ret &= test;
	
	test = this->sr_user.spi_usr_mosi_highpart == cmp.sr_user.spi_usr_mosi_highpart;
	if(printout && !test)
		this->printComp("spi_usr_mosi_highpart", this->sr_user.spi_usr_mosi_highpart, cmp.sr_user.spi_usr_mosi_highpart, "<- cmp");
	ret &= test;
	
	test = this->sr_user.spi_usr_miso_highpart == cmp.sr_user.spi_usr_miso_highpart;
	if(printout && !test)
		this->printComp("spi_usr_miso_highpart", this->sr_user.spi_usr_miso_highpart, cmp.sr_user.spi_usr_miso_highpart, "<- cmp");
	ret &= test;
	
	test = this->sr_user.spi_sio == cmp.sr_user.spi_sio;
	if(printout && !test)
		this->printComp("spi_sio", this->sr_user.spi_sio, cmp.sr_user.spi_sio, "<- cmp");
	ret &= test;
	
	test = this->sr_user.r_spi_usr_conf_nxt == cmp.sr_user.r_spi_usr_conf_nxt;
	if(printout && !test)
		this->printComp("r_spi_usr_conf_nxt", this->sr_user.r_spi_usr_conf_nxt, cmp.sr_user.r_spi_usr_conf_nxt, "<- cmp");
	ret &= test;
	
	test = this->sr_user.r_spi_fwrite_oct == cmp.sr_user.r_spi_fwrite_oct;
	if(printout && !test)
		this->printComp("r_spi_fwrite_oct", this->sr_user.r_spi_fwrite_oct, cmp.sr_user.r_spi_fwrite_oct, "<- cmp");
	ret &= test;
	
	test = this->sr_user.spi_fwrite_quad == cmp.sr_user.spi_fwrite_quad;
	if(printout && !test)
		this->printComp("spi_fwrite_quad", this->sr_user.spi_fwrite_quad, cmp.sr_user.spi_fwrite_quad, "<- cmp");
	ret &= test;
	
	test = this->sr_user.spi_fwrite_dual == cmp.sr_user.spi_fwrite_dual;
	if(printout && !test)
		this->printComp("spi_fwrite_dual", this->sr_user.spi_fwrite_dual, cmp.sr_user.spi_fwrite_dual, "<- cmp");
	ret &= test;
	
	test = this->sr_user.spi_usr_ck_out_edge == cmp.sr_user.spi_usr_ck_out_edge;
	if(printout && !test)
		this->printComp("spi_usr_ck_out_edge", this->sr_user.spi_usr_ck_out_edge, cmp.sr_user.spi_usr_ck_out_edge, "<- cmp");
	ret &= test;
	
	test = this->sr_user.spi_rsck_i_edge == cmp.sr_user.spi_rsck_i_edge;
	if(printout && !test)
		this->printComp("spi_rsck_i_edge", this->sr_user.spi_rsck_i_edge, cmp.sr_user.spi_rsck_i_edge, "<- cmp");
	ret &= test;
	
	test = this->sr_user.spi_cs_setup == cmp.sr_user.spi_cs_setup;
	if(printout && !test)
		this->printComp("spi_cs_setup", this->sr_user.spi_cs_setup, cmp.sr_user.spi_cs_setup, "<- cmp");
	ret &= test;
	
	test = this->sr_user.spi_cs_hold == cmp.sr_user.spi_cs_hold;
	if(printout && !test)
		this->printComp("spi_cs_hold", this->sr_user.spi_cs_hold, cmp.sr_user.spi_cs_hold, "<- cmp");
	ret &= test;
	
	test = this->sr_user.spi_tsck_i_edge == cmp.sr_user.spi_tsck_i_edge;
	if(printout && !test)
		this->printComp("spi_tsck_i_edge", this->sr_user.spi_tsck_i_edge, cmp.sr_user.spi_tsck_i_edge, "<- cmp");
	ret &= test;
	
	test = this->sr_user.r_spi_opi_mode == cmp.sr_user.r_spi_opi_mode;
	if(printout && !test)
		this->printComp("r_spi_opi_mode", this->sr_user.r_spi_opi_mode, cmp.sr_user.r_spi_opi_mode, "<- cmp");
	ret &= test;
	
	test = this->sr_user.spi_qpi_mode == cmp.sr_user.spi_qpi_mode;
	if(printout && !test)
		this->printComp("spi_qpi_mode", this->sr_user.spi_qpi_mode, cmp.sr_user.spi_qpi_mode, "<- cmp");
	ret &= test;
	
	test = this->sr_user.spi_doutdin == cmp.sr_user.spi_doutdin;
	if(printout && !test)
		this->printComp("spi_doutdin", this->sr_user.spi_doutdin, cmp.sr_user.spi_doutdin, "<- cmp");
	ret &= test;
	

	return ret;
}
bool SpiDebug::comp_sr_user1(SpiDebug cmp, bool printout){
	bool ret = true;
	bool test = this->sr_user1.spi_usr_addr_bitlen == cmp.sr_user1.spi_usr_addr_bitlen;
	if(printout && !test)
		this->printComp("spi_usr_addr_bitlen", this->sr_user1.spi_usr_addr_bitlen, cmp.sr_user1.spi_usr_addr_bitlen, "<- cmp");
	ret &= test;
	
	test = this->sr_user1.spi_cs_hold_time == cmp.sr_user1.spi_cs_hold_time;
	if(printout && !test)
		this->printComp("spi_cs_hold_time", this->sr_user1.spi_cs_hold_time, cmp.sr_user1.spi_cs_hold_time, "<- cmp");
	ret &= test;
	
	test = this->sr_user1.spi_cs_setup_time == cmp.sr_user1.spi_cs_setup_time;
	if(printout && !test)
		this->printComp("spi_cs_setup_time", this->sr_user1.spi_cs_setup_time, cmp.sr_user1.spi_cs_setup_time, "<- cmp");
	ret &= test;
	
	test = this->sr_user1.spi_mst_wfull_err_end_en == cmp.sr_user1.spi_mst_wfull_err_end_en;
	if(printout && !test)
		this->printComp("spi_mst_wfull_err_end_en", this->sr_user1.spi_mst_wfull_err_end_en, cmp.sr_user1.spi_mst_wfull_err_end_en, "<- cmp");
	ret &= test;
	
	test = this->sr_user1.spi_usr_dummy_cyclelen == cmp.sr_user1.spi_usr_dummy_cyclelen;
	if(printout && !test)
		this->printComp("spi_usr_dummy_cyclelen", this->sr_user1.spi_usr_dummy_cyclelen, cmp.sr_user1.spi_usr_dummy_cyclelen, "<- cmp");
	ret &= test;
	
	return ret;
}
bool SpiDebug::comp_sr_user2(SpiDebug cmp, bool printout){
	bool ret = true;
	bool test = this->sr_user2.spi_usr_command_bitlen == cmp.sr_user2.spi_usr_command_bitlen;
	if(printout && !test)
		this->printComp("spi_usr_command_bitlen", this->sr_user2.spi_usr_command_bitlen, cmp.sr_user2.spi_usr_command_bitlen, "<- cmp");
	ret &= test;
	
	test = this->sr_user2.spi_mst_rempty_err_end_en == cmp.sr_user2.spi_mst_rempty_err_end_en;
	if(printout && !test)
		this->printComp("spi_mst_rempty_err_end_en", this->sr_user2.spi_mst_rempty_err_end_en, cmp.sr_user2.spi_mst_rempty_err_end_en, "<- cmp");
	ret &= test;
	
	test = this->sr_user2.spi_usr_command_value == cmp.sr_user2.spi_usr_command_value;
	if(printout && !test)
		this->printComp("spi_usr_command_value", this->sr_user2.spi_usr_command_value, cmp.sr_user2.spi_usr_command_value, "<- cmp");
	ret &= test;
	
	return ret;
}

bool SpiDebug::compCtrlConf(SpiDebug cmp, bool printout){
	bool ret = this->comp_sr_ctrl(cmp, printout);
	ret = !ret ? (this->comp_sr_msdlen(cmp, printout) & ret) : this->comp_sr_msdlen(cmp, printout);
	ret = !ret ? (this->comp_sr_misc(cmp, printout) & ret) : this->comp_sr_misc(cmp, printout);
	ret = !ret ? (this->comp_sr_dmaconf(cmp, printout) & ret) : this->comp_sr_dmaconf(cmp, printout);
	ret = !ret ? (this->comp_sr_slave(cmp, printout) & ret) : this->comp_sr_slave(cmp, printout);
	ret = !ret ? (this->comp_sr_slave1(cmp, printout) & ret) : this->comp_sr_slave1(cmp, printout);
	return ret;
}

bool SpiDebug::comp_sr_ctrl(SpiDebug cmp, bool printout){
	bool ret = true;

	bool test = this->sr_ctrl.spi_wr_bit_order == cmp.sr_ctrl.spi_wr_bit_order;
	if(printout && !test)
		this->printComp("spi_wr_bit_order", this->sr_ctrl.spi_wr_bit_order, cmp.sr_ctrl.spi_wr_bit_order, "<- cmp");
	ret &= test;
	
	test = this->sr_ctrl.spi_rd_bit_order == cmp.sr_ctrl.spi_rd_bit_order;
	if(printout && !test)
		this->printComp("spi_rd_bit_order", this->sr_ctrl.spi_rd_bit_order, cmp.sr_ctrl.spi_rd_bit_order, "<- cmp");
	ret &= test;
	
	test = this->sr_ctrl.spi_wp_pol == cmp.sr_ctrl.spi_wp_pol;
	if(printout && !test)
		this->printComp("spi_wp_pol", this->sr_ctrl.spi_wp_pol, cmp.sr_ctrl.spi_wp_pol, "<- cmp");
	ret &= test;
	
	test = this->sr_ctrl.spi_hold_pol == cmp.sr_ctrl.spi_hold_pol;
	if(printout && !test)
		this->printComp("spi_hold_pol", this->sr_ctrl.spi_hold_pol, cmp.sr_ctrl.spi_hold_pol, "<- cmp");
	ret &= test;
	
	test = this->sr_ctrl.spi_d_pol == cmp.sr_ctrl.spi_d_pol;
	if(printout && !test)
		this->printComp("spi_d_pol", this->sr_ctrl.spi_d_pol, cmp.sr_ctrl.spi_d_pol, "<- cmp");
	ret &= test;
	
	test = this->sr_ctrl.spi_q_pol == cmp.sr_ctrl.spi_q_pol;
	if(printout && !test)
		this->printComp("spi_q_pol", this->sr_ctrl.spi_q_pol, cmp.sr_ctrl.spi_q_pol, "<- cmp");
	ret &= test;
	
	test = this->sr_ctrl.r_spi_fread_oct == cmp.sr_ctrl.r_spi_fread_oct;
	if(printout && !test)
		this->printComp("r_spi_fread_oct", this->sr_ctrl.r_spi_fread_oct, cmp.sr_ctrl.r_spi_fread_oct, "<- cmp");
	ret &= test;
	
	test = this->sr_ctrl.spi_fread_quad == cmp.sr_ctrl.spi_fread_quad;
	if(printout && !test)
		this->printComp("spi_fread_quad", this->sr_ctrl.spi_fread_quad, cmp.sr_ctrl.spi_fread_quad, "<- cmp");
	ret &= test;
	
	test = this->sr_ctrl.spi_fread_dual == cmp.sr_ctrl.spi_fread_dual;
	if(printout && !test)
		this->printComp("spi_fread_dual", this->sr_ctrl.spi_fread_dual, cmp.sr_ctrl.spi_fread_dual, "<- cmp");
	ret &= test;
	
	test = this->sr_ctrl.r_spi_fcmd_oct == cmp.sr_ctrl.r_spi_fcmd_oct;
	if(printout && !test)
		this->printComp("r_spi_fcmd_oct", this->sr_ctrl.r_spi_fcmd_oct, cmp.sr_ctrl.r_spi_fcmd_oct, "<- cmp");
	ret &= test;
	
	test = this->sr_ctrl.spi_fcmd_quad == cmp.sr_ctrl.spi_fcmd_quad;
	if(printout && !test)
		this->printComp("spi_fcmd_quad", this->sr_ctrl.spi_fcmd_quad, cmp.sr_ctrl.spi_fcmd_quad, "<- cmp");
	ret &= test;
	
	test = this->sr_ctrl.spi_fcmd_dual == cmp.sr_ctrl.spi_fcmd_dual;
	if(printout && !test)
		this->printComp("spi_fcmd_dual", this->sr_ctrl.spi_fcmd_dual, cmp.sr_ctrl.spi_fcmd_dual, "<- cmp");
	ret &= test;
	
	test = this->sr_ctrl.r_spi_faddr_oct == cmp.sr_ctrl.r_spi_faddr_oct;
	if(printout && !test)
		this->printComp("r_spi_faddr_oct", this->sr_ctrl.r_spi_faddr_oct, cmp.sr_ctrl.r_spi_faddr_oct, "<- cmp");
	ret &= test;
	
	test = this->sr_ctrl.spi_faddr_quad == cmp.sr_ctrl.spi_faddr_quad;
	if(printout && !test)
		this->printComp("spi_faddr_quad", this->sr_ctrl.spi_faddr_quad, cmp.sr_ctrl.spi_faddr_quad, "<- cmp");
	ret &= test;
	
	test = this->sr_ctrl.spi_faddr_dual == cmp.sr_ctrl.spi_faddr_dual;
	if(printout && !test)
		this->printComp("spi_faddr_dual", this->sr_ctrl.spi_faddr_dual, cmp.sr_ctrl.spi_faddr_dual, "<- cmp");
	ret &= test;
	
	test = this->sr_ctrl.spi_dummy_out == cmp.sr_ctrl.spi_dummy_out;
	if(printout && !test)
		this->printComp("spi_dummy_out", this->sr_ctrl.spi_dummy_out, cmp.sr_ctrl.spi_dummy_out, "<- cmp");
	ret &= test;

	return ret;
}
bool SpiDebug::comp_sr_msdlen(SpiDebug cmp, bool printout){
	bool ret = true;
	
  	bool test = this->sr_msdlen.spi_ms_data_bitlen == cmp.sr_msdlen.spi_ms_data_bitlen;
	if(printout && !test)
		this->printComp("spi_ms_data_bitlen", this->sr_msdlen.spi_ms_data_bitlen, cmp.sr_msdlen.spi_ms_data_bitlen, "<- cmp");
	ret &= test;
	return ret;
}
bool SpiDebug::comp_sr_misc(SpiDebug cmp, bool printout){
	bool ret = true;
	bool test = this->sr_misc.spi_quad_din_pin_swap == cmp.sr_misc.spi_quad_din_pin_swap;
	if(printout && !test)
		this->printComp("spi_quad_din_pin_swap", this->sr_misc.spi_quad_din_pin_swap, cmp.sr_misc.spi_quad_din_pin_swap, "<- cmp");
	ret &= test;
	
	test = this->sr_misc.spi_cs_keep_active == cmp.sr_misc.spi_cs_keep_active;
	if(printout && !test)
		this->printComp("spi_cs_keep_active", this->sr_misc.spi_cs_keep_active, cmp.sr_misc.spi_cs_keep_active, "<- cmp");
	ret &= test;
	
	test = this->sr_misc.spi_ck_idle_edge == cmp.sr_misc.spi_ck_idle_edge;
	if(printout && !test)
		this->printComp("spi_ck_idle_edge", this->sr_misc.spi_ck_idle_edge, cmp.sr_misc.spi_ck_idle_edge, "<- cmp");
	ret &= test;
	
	test = this->sr_misc.spi_dqs_idle_edge == cmp.sr_misc.spi_dqs_idle_edge;
	if(printout && !test)
		this->printComp("spi_dqs_idle_edge", this->sr_misc.spi_dqs_idle_edge, cmp.sr_misc.spi_dqs_idle_edge, "<- cmp");
	ret &= test;
	
	test = this->sr_misc.spi_slave_cs_pol == cmp.sr_misc.spi_slave_cs_pol;
	if(printout && !test)
		this->printComp("spi_slave_cs_pol", this->sr_misc.spi_slave_cs_pol, cmp.sr_misc.spi_slave_cs_pol, "<- cmp");
	ret &= test;
	
	test = this->sr_misc.spi_cmd_dtr_en == cmp.sr_misc.spi_cmd_dtr_en;
	if(printout && !test)
		this->printComp("spi_cmd_dtr_en", this->sr_misc.spi_cmd_dtr_en, cmp.sr_misc.spi_cmd_dtr_en, "<- cmp");
	ret &= test;
	
	test = this->sr_misc.spi_addr_dtr_en == cmp.sr_misc.spi_addr_dtr_en;
	if(printout && !test)
		this->printComp("spi_addr_dtr_en", this->sr_misc.spi_addr_dtr_en, cmp.sr_misc.spi_addr_dtr_en, "<- cmp");
	ret &= test;
	
	test = this->sr_misc.spi_data_dtr_en == cmp.sr_misc.spi_data_dtr_en;
	if(printout && !test)
		this->printComp("spi_data_dtr_en", this->sr_misc.spi_data_dtr_en, cmp.sr_misc.spi_data_dtr_en, "<- cmp");
	ret &= test;
	
	test = this->sr_misc.spi_clk_data_dtr_en == cmp.sr_misc.spi_clk_data_dtr_en;
	if(printout && !test)
		this->printComp("spi_clk_data_dtr_en", this->sr_misc.spi_clk_data_dtr_en, cmp.sr_misc.spi_clk_data_dtr_en, "<- cmp");
	ret &= test;
	
	test = this->sr_misc.spi_master_cs_pol == cmp.sr_misc.spi_master_cs_pol;
	if(printout && !test)
		this->printComp("spi_master_cs_pol", this->sr_misc.spi_master_cs_pol, cmp.sr_misc.spi_master_cs_pol, "<- cmp");
	ret &= test;
	
	test = this->sr_misc.spi_ck_dis == cmp.sr_misc.spi_ck_dis;
	if(printout && !test)
		this->printComp("spi_ck_dis", this->sr_misc.spi_ck_dis, cmp.sr_misc.spi_ck_dis, "<- cmp");
	ret &= test;
	
	test = this->sr_misc.spi_cs5_dis == cmp.sr_misc.spi_cs5_dis;
	if(printout && !test)
		this->printComp("spi_cs5_dis", this->sr_misc.spi_cs5_dis, cmp.sr_misc.spi_cs5_dis, "<- cmp");
	ret &= test;
	
	test = this->sr_misc.spi_cs4_dis == cmp.sr_misc.spi_cs4_dis;
	if(printout && !test)
		this->printComp("spi_cs4_dis", this->sr_misc.spi_cs4_dis, cmp.sr_misc.spi_cs4_dis, "<- cmp");
	ret &= test;
	
	test = this->sr_misc.spi_cs3_dis == cmp.sr_misc.spi_cs3_dis;
	if(printout && !test)
		this->printComp("spi_cs3_dis", this->sr_misc.spi_cs3_dis, cmp.sr_misc.spi_cs3_dis, "<- cmp");
	ret &= test;
	
	test = this->sr_misc.spi_cs2_dis == cmp.sr_misc.spi_cs2_dis;
	if(printout && !test)
		this->printComp("spi_cs2_dis", this->sr_misc.spi_cs2_dis, cmp.sr_misc.spi_cs2_dis, "<- cmp");
	ret &= test;
	
	test = this->sr_misc.spi_cs1_dis == cmp.sr_misc.spi_cs1_dis;
	if(printout && !test)
		this->printComp("spi_cs1_dis", this->sr_misc.spi_cs1_dis, cmp.sr_misc.spi_cs1_dis, "<- cmp");
	ret &= test;
	
	test = this->sr_misc.spi_cs0_dis == cmp.sr_misc.spi_cs0_dis;
	if(printout && !test)
		this->printComp("spi_cs0_dis", this->sr_misc.spi_cs0_dis, cmp.sr_misc.spi_cs0_dis, "<- cmp");
	ret &= test;
	

	return ret;
}
bool SpiDebug::comp_sr_dmaconf(SpiDebug cmp, bool printout){
	bool ret = true;
	bool test = this->sr_dmaconf.spi_dma_afifo_rst == cmp.sr_dmaconf.spi_dma_afifo_rst;
	if(printout && !test)
		this->printComp("spi_dma_afifo_rst", this->sr_dmaconf.spi_dma_afifo_rst, cmp.sr_dmaconf.spi_dma_afifo_rst, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaconf.spi_buf_afifo_rst == cmp.sr_dmaconf.spi_buf_afifo_rst;
	if(printout && !test)
		this->printComp("spi_buf_afifo_rst", this->sr_dmaconf.spi_buf_afifo_rst, cmp.sr_dmaconf.spi_buf_afifo_rst, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaconf.spi_rx_afifo_rst == cmp.sr_dmaconf.spi_rx_afifo_rst;
	if(printout && !test)
		this->printComp("spi_rx_afifo_rst", this->sr_dmaconf.spi_rx_afifo_rst, cmp.sr_dmaconf.spi_rx_afifo_rst, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaconf.spi_dma_tx_ena == cmp.sr_dmaconf.spi_dma_tx_ena;
	if(printout && !test)
		this->printComp("spi_dma_tx_ena", this->sr_dmaconf.spi_dma_tx_ena, cmp.sr_dmaconf.spi_dma_tx_ena, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaconf.spi_dma_rx_ena == cmp.sr_dmaconf.spi_dma_rx_ena;
	if(printout && !test)
		this->printComp("spi_dma_rx_ena", this->sr_dmaconf.spi_dma_rx_ena, cmp.sr_dmaconf.spi_dma_rx_ena, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaconf.spi_rx_eof_en == cmp.sr_dmaconf.spi_rx_eof_en;
	if(printout && !test)
		this->printComp("spi_rx_eof_en", this->sr_dmaconf.spi_rx_eof_en, cmp.sr_dmaconf.spi_rx_eof_en, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaconf.spi_slv_tx_seg_trans_clr_en == cmp.sr_dmaconf.spi_slv_tx_seg_trans_clr_en;
	if(printout && !test)
		this->printComp("spi_slv_tx_seg_trans_clr_en", this->sr_dmaconf.spi_slv_tx_seg_trans_clr_en, cmp.sr_dmaconf.spi_slv_tx_seg_trans_clr_en, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaconf.spi_slv_rx_seg_trans_clr_en == cmp.sr_dmaconf.spi_slv_rx_seg_trans_clr_en;
	if(printout && !test)
		this->printComp("spi_slv_rx_seg_trans_clr_en", this->sr_dmaconf.spi_slv_rx_seg_trans_clr_en, cmp.sr_dmaconf.spi_slv_rx_seg_trans_clr_en, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaconf.spi_dma_slv_seg_trans_en == cmp.sr_dmaconf.spi_dma_slv_seg_trans_en;
	if(printout && !test)
		this->printComp("spi_dma_slv_seg_trans_en", this->sr_dmaconf.spi_dma_slv_seg_trans_en, cmp.sr_dmaconf.spi_dma_slv_seg_trans_en, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaconf.spi_dma_infifo_full == cmp.sr_dmaconf.spi_dma_infifo_full;
	if(printout && !test)
		this->printComp("spi_dma_infifo_full", this->sr_dmaconf.spi_dma_infifo_full, cmp.sr_dmaconf.spi_dma_infifo_full, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaconf.spi_dma_outfifo_empty == cmp.sr_dmaconf.spi_dma_outfifo_empty;
	if(printout && !test)
		this->printComp("spi_dma_outfifo_empty", this->sr_dmaconf.spi_dma_outfifo_empty, cmp.sr_dmaconf.spi_dma_outfifo_empty, "<- cmp");
	ret &= test;


	return ret;
}
bool SpiDebug::comp_sr_slave(SpiDebug cmp, bool printout){
	bool ret = true;
	bool test = this->sr_slave.r_spi_usr_conf == cmp.sr_slave.r_spi_usr_conf;
	if(printout && !test)
		this->printComp("r_spi_usr_conf", this->sr_slave.r_spi_usr_conf, cmp.sr_slave.r_spi_usr_conf, "<- cmp");
	ret &= test;
	
	test = this->sr_slave.spi_soft_reset == cmp.sr_slave.spi_soft_reset;
	if(printout && !test)
		this->printComp("spi_soft_reset", this->sr_slave.spi_soft_reset, cmp.sr_slave.spi_soft_reset, "<- cmp");
	ret &= test;
	
	test = this->sr_slave.spi_slave_mode == cmp.sr_slave.spi_slave_mode;
	if(printout && !test)
		this->printComp("spi_slave_mode", this->sr_slave.spi_slave_mode, cmp.sr_slave.spi_slave_mode, "<- cmp");
	ret &= test;
	
	test = this->sr_slave.r_spi_dma_seg_magic_value == cmp.sr_slave.r_spi_dma_seg_magic_value;
	if(printout && !test)
		this->printComp("r_spi_dma_seg_magic_value", this->sr_slave.r_spi_dma_seg_magic_value, cmp.sr_slave.r_spi_dma_seg_magic_value, "<- cmp");
	ret &= test;
	
	test = this->sr_slave.spi_slv_wrbuf_bitlen_en == cmp.sr_slave.spi_slv_wrbuf_bitlen_en;
	if(printout && !test)
		this->printComp("spi_slv_wrbuf_bitlen_en", this->sr_slave.spi_slv_wrbuf_bitlen_en, cmp.sr_slave.spi_slv_wrbuf_bitlen_en, "<- cmp");
	ret &= test;
	
	test = this->sr_slave.spi_slv_rdbuf_bitlen_en == cmp.sr_slave.spi_slv_rdbuf_bitlen_en;
	if(printout && !test)
		this->printComp("spi_slv_rdbuf_bitlen_en", this->sr_slave.spi_slv_rdbuf_bitlen_en, cmp.sr_slave.spi_slv_rdbuf_bitlen_en, "<- cmp");
	ret &= test;
	
	test = this->sr_slave.spi_slv_wrdma_bitlen_en == cmp.sr_slave.spi_slv_wrdma_bitlen_en;
	if(printout && !test)
		this->printComp("spi_slv_wrdma_bitlen_en", this->sr_slave.spi_slv_wrdma_bitlen_en, cmp.sr_slave.spi_slv_wrdma_bitlen_en, "<- cmp");
	ret &= test;
	
	test = this->sr_slave.spi_slv_rddma_bitlen_en == cmp.sr_slave.spi_slv_rddma_bitlen_en;
	if(printout && !test)
		this->printComp("spi_slv_rddma_bitlen_en", this->sr_slave.spi_slv_rddma_bitlen_en, cmp.sr_slave.spi_slv_rddma_bitlen_en, "<- cmp");
	ret &= test;
	
	test = this->sr_slave.spi_rsck_data_out == cmp.sr_slave.spi_rsck_data_out;
	if(printout && !test)
		this->printComp("spi_rsck_data_out", this->sr_slave.spi_rsck_data_out, cmp.sr_slave.spi_rsck_data_out, "<- cmp");
	ret &= test;
	
	test = this->sr_slave.spi_clk_mode_13 == cmp.sr_slave.spi_clk_mode_13;
	if(printout && !test)
		this->printComp("spi_clk_mode_13", this->sr_slave.spi_clk_mode_13, cmp.sr_slave.spi_clk_mode_13, "<- cmp");
	ret &= test;
	
	test = this->sr_slave.spi_clk_mode == cmp.sr_slave.spi_clk_mode;
	if(printout && !test)
		this->printComp("spi_clk_mode", this->sr_slave.spi_clk_mode, cmp.sr_slave.spi_clk_mode, "<- cmp");
	ret &= test;
	

	return ret;
}
bool SpiDebug::comp_sr_slave1(SpiDebug cmp, bool printout){
	bool ret = true;
	bool test = this->sr_slave1.spi_slv_last_addr == cmp.sr_slave1.spi_slv_last_addr;
	if(printout && !test)
		this->printComp("spi_slv_last_addr", this->sr_slave1.spi_slv_last_addr, cmp.sr_slave1.spi_slv_last_addr, "<- cmp");
	ret &= test;
	
	test = this->sr_slave1.spi_slv_last_command  == cmp.sr_slave1.spi_slv_last_command ;
	if(printout && !test)
		this->printComp("spi_slv_last_command ", this->sr_slave1.spi_slv_last_command , cmp.sr_slave1.spi_slv_last_command , "<- cmp");
	ret &= test;
	
	test = this->sr_slave1.spi_slv_data_bitlen == cmp.sr_slave1.spi_slv_data_bitlen;
	if(printout && !test)
		this->printComp("spi_slv_data_bitlen", this->sr_slave1.spi_slv_data_bitlen, cmp.sr_slave1.spi_slv_data_bitlen, "<- cmp");
	ret &= test;
	
	return ret;
}

bool SpiDebug::compClock(SpiDebug cmp, bool printout){
	bool ret = this->comp_sr_clock(cmp, printout);
	ret = !ret ? (this->comp_sr_gate(cmp, printout) & ret) : this->comp_sr_gate(cmp, printout);
	return ret;
}
bool SpiDebug::comp_sr_clock(SpiDebug cmp, bool printout){
	bool ret = true;
  
	bool test = this->sr_clock.spi_clk_equ_sysclk_pos == cmp.sr_clock.spi_clk_equ_sysclk_pos;
	if(printout && !test)
		this->printComp("spi_clk_equ_sysclk_pos", this->sr_clock.spi_clk_equ_sysclk_pos, cmp.sr_clock.spi_clk_equ_sysclk_pos, "<- cmp");
	ret &= test;
	
	test = this->sr_clock.spi_clkdiv_pre == cmp.sr_clock.spi_clkdiv_pre;
	if(printout && !test)
		this->printComp("spi_clkdiv_pre", this->sr_clock.spi_clkdiv_pre, cmp.sr_clock.spi_clkdiv_pre, "<- cmp");
	ret &= test;
	
	test = this->sr_clock.spi_clkcnt_n == cmp.sr_clock.spi_clkcnt_n;
	if(printout && !test)
		this->printComp("spi_clkcnt_n", this->sr_clock.spi_clkcnt_n, cmp.sr_clock.spi_clkcnt_n, "<- cmp");
	ret &= test;
	
	test = this->sr_clock.spi_clkcnt_h == cmp.sr_clock.spi_clkcnt_h;
	if(printout && !test)
		this->printComp("spi_clkcnt_h", this->sr_clock.spi_clkcnt_h, cmp.sr_clock.spi_clkcnt_h, "<- cmp");
	ret &= test;
	
	test = this->sr_clock.spi_clkcnt_l == cmp.sr_clock.spi_clkcnt_l;
	if(printout && !test)
		this->printComp("spi_clkcnt_l", this->sr_clock.spi_clkcnt_l, cmp.sr_clock.spi_clkcnt_l, "<- cmp");
	ret &= test;
	

	return ret;
}
bool SpiDebug::comp_sr_gate(SpiDebug cmp, bool printout){
	bool ret = true;

	bool test = this->sr_gate.spi_mst_clk_sel == cmp.sr_gate.spi_mst_clk_sel;
	if(printout && !test)
		this->printComp("spi_mst_clk_sel", this->sr_gate.spi_mst_clk_sel, cmp.sr_gate.spi_mst_clk_sel, "<- cmp");
	ret &= test;
	
	test = this->sr_gate.spi_mst_clk_active == cmp.sr_gate.spi_mst_clk_active;
	if(printout && !test)
		this->printComp("spi_mst_clk_active", this->sr_gate.spi_mst_clk_active, cmp.sr_gate.spi_mst_clk_active, "<- cmp");
	ret &= test;
	
	test = this->sr_gate.spi_clk_en == cmp.sr_gate.spi_clk_en;
	if(printout && !test)
		this->printComp("spi_clk_en", this->sr_gate.spi_clk_en, cmp.sr_gate.spi_clk_en, "<- cmp");
	ret &= test;
	

	return ret;
}

bool SpiDebug::compTiming(SpiDebug cmp, bool printout){
	bool ret = this->comp_sr_dinmode(cmp, printout);
	ret = !ret ? (this->comp_sr_dinnum(cmp, printout) & ret) : this->comp_sr_dinnum(cmp, printout);
	ret = !ret ? (this->comp_sr_doutmode(cmp, printout) & ret) : this->comp_sr_doutmode(cmp, printout);

	return ret;
}
bool SpiDebug::comp_sr_dinmode(SpiDebug cmp, bool printout){
	bool ret = true;
	bool test = this->sr_dinmode.spi_timing_hclk_active == cmp.sr_dinmode.spi_timing_hclk_active;
	if(printout && !test)
		this->printComp("spi_timing_hclk_active", this->sr_dinmode.spi_timing_hclk_active, cmp.sr_dinmode.spi_timing_hclk_active, "<- cmp");
	ret &= test;
	
	test = this->sr_dinmode.r_spi_din7_mode == cmp.sr_dinmode.r_spi_din7_mode;
	if(printout && !test)
		this->printComp("r_spi_din7_mode", this->sr_dinmode.r_spi_din7_mode, cmp.sr_dinmode.r_spi_din7_mode, "<- cmp");
	ret &= test;
	
	test = this->sr_dinmode.r_spi_din6_mode == cmp.sr_dinmode.r_spi_din6_mode;
	if(printout && !test)
		this->printComp("r_spi_din6_mode", this->sr_dinmode.r_spi_din6_mode, cmp.sr_dinmode.r_spi_din6_mode, "<- cmp");
	ret &= test;
	
	test = this->sr_dinmode.r_spi_din5_mode == cmp.sr_dinmode.r_spi_din5_mode;
	if(printout && !test)
		this->printComp("r_spi_din5_mode", this->sr_dinmode.r_spi_din5_mode, cmp.sr_dinmode.r_spi_din5_mode, "<- cmp");
	ret &= test;
	
	test = this->sr_dinmode.r_spi_din4_mode == cmp.sr_dinmode.r_spi_din4_mode;
	if(printout && !test)
		this->printComp("r_spi_din4_mode", this->sr_dinmode.r_spi_din4_mode, cmp.sr_dinmode.r_spi_din4_mode, "<- cmp");
	ret &= test;
	
	test = this->sr_dinmode.spi_din3_mode == cmp.sr_dinmode.spi_din3_mode;
	if(printout && !test)
		this->printComp("spi_din3_mode", this->sr_dinmode.spi_din3_mode, cmp.sr_dinmode.spi_din3_mode, "<- cmp");
	ret &= test;
	
	test = this->sr_dinmode.spi_din2_mode == cmp.sr_dinmode.spi_din2_mode;
	if(printout && !test)
		this->printComp("spi_din2_mode", this->sr_dinmode.spi_din2_mode, cmp.sr_dinmode.spi_din2_mode, "<- cmp");
	ret &= test;
	
	test = this->sr_dinmode.spi_din1_mode == cmp.sr_dinmode.spi_din1_mode;
	if(printout && !test)
		this->printComp("spi_din1_mode", this->sr_dinmode.spi_din1_mode, cmp.sr_dinmode.spi_din1_mode, "<- cmp");
	ret &= test;
	
	test = this->sr_dinmode.spi_din0_mode == cmp.sr_dinmode.spi_din0_mode;
	if(printout && !test)
		this->printComp("spi_din0_mode", this->sr_dinmode.spi_din0_mode, cmp.sr_dinmode.spi_din0_mode, "<- cmp");
	ret &= test;
	
	return ret;
}
bool SpiDebug::comp_sr_dinnum(SpiDebug cmp, bool printout){
	bool ret = true;
	bool test = this->sr_dinnum.r_spi_din7_num == cmp.sr_dinnum.r_spi_din7_num;
	if(printout && !test)
		this->printComp("r_spi_din7_num", this->sr_dinnum.r_spi_din7_num, cmp.sr_dinnum.r_spi_din7_num, "<- cmp");
	ret &= test;
	
	test = this->sr_dinnum.r_spi_din6_num == cmp.sr_dinnum.r_spi_din6_num;
	if(printout && !test)
		this->printComp("r_spi_din6_num", this->sr_dinnum.r_spi_din6_num, cmp.sr_dinnum.r_spi_din6_num, "<- cmp");
	ret &= test;
	
	test = this->sr_dinnum.r_spi_din5_num == cmp.sr_dinnum.r_spi_din5_num;
	if(printout && !test)
		this->printComp("r_spi_din5_num", this->sr_dinnum.r_spi_din5_num, cmp.sr_dinnum.r_spi_din5_num, "<- cmp");
	ret &= test;
	
	test = this->sr_dinnum.r_spi_din4_num == cmp.sr_dinnum.r_spi_din4_num;
	if(printout && !test)
		this->printComp("r_spi_din4_num", this->sr_dinnum.r_spi_din4_num, cmp.sr_dinnum.r_spi_din4_num, "<- cmp");
	ret &= test;
	
	test = this->sr_dinnum.spi_din3_num == cmp.sr_dinnum.spi_din3_num;
	if(printout && !test)
		this->printComp("spi_din3_num", this->sr_dinnum.spi_din3_num, cmp.sr_dinnum.spi_din3_num, "<- cmp");
	ret &= test;
	
	test = this->sr_dinnum.spi_din2_num == cmp.sr_dinnum.spi_din2_num;
	if(printout && !test)
		this->printComp("spi_din2_num", this->sr_dinnum.spi_din2_num, cmp.sr_dinnum.spi_din2_num, "<- cmp");
	ret &= test;
	
	test = this->sr_dinnum.spi_din1_num == cmp.sr_dinnum.spi_din1_num;
	if(printout && !test)
		this->printComp("spi_din1_num", this->sr_dinnum.spi_din1_num, cmp.sr_dinnum.spi_din1_num, "<- cmp");
	ret &= test;
	
	test = this->sr_dinnum.spi_din0_num == cmp.sr_dinnum.spi_din0_num;
	if(printout && !test)
		this->printComp("spi_din0_num", this->sr_dinnum.spi_din0_num, cmp.sr_dinnum.spi_din0_num, "<- cmp");
	ret &= test;
	

	return ret;
}
bool SpiDebug::comp_sr_doutmode(SpiDebug cmp, bool printout){
	bool ret = true;
	bool test = this->sr_doutmode.r_spi_d_dqs_mode == cmp.sr_doutmode.r_spi_d_dqs_mode;
	if(printout && !test)
		this->printComp("r_spi_d_dqs_mode", this->sr_doutmode.r_spi_d_dqs_mode, cmp.sr_doutmode.r_spi_d_dqs_mode, "<- cmp");
	ret &= test;
	
	test = this->sr_doutmode.r_spi_dout7_mode == cmp.sr_doutmode.r_spi_dout7_mode;
	if(printout && !test)
		this->printComp("r_spi_dout7_mode", this->sr_doutmode.r_spi_dout7_mode, cmp.sr_doutmode.r_spi_dout7_mode, "<- cmp");
	ret &= test;
	
	test = this->sr_doutmode.r_spi_dout6_mode == cmp.sr_doutmode.r_spi_dout6_mode;
	if(printout && !test)
		this->printComp("r_spi_dout6_mode", this->sr_doutmode.r_spi_dout6_mode, cmp.sr_doutmode.r_spi_dout6_mode, "<- cmp");
	ret &= test;
	
	test = this->sr_doutmode.r_spi_dout5_mode == cmp.sr_doutmode.r_spi_dout5_mode;
	if(printout && !test)
		this->printComp("r_spi_dout5_mode", this->sr_doutmode.r_spi_dout5_mode, cmp.sr_doutmode.r_spi_dout5_mode, "<- cmp");
	ret &= test;
	
	test = this->sr_doutmode.r_spi_dout4_mode == cmp.sr_doutmode.r_spi_dout4_mode;
	if(printout && !test)
		this->printComp("r_spi_dout4_mode", this->sr_doutmode.r_spi_dout4_mode, cmp.sr_doutmode.r_spi_dout4_mode, "<- cmp");
	ret &= test;
	
	test = this->sr_doutmode.spi_dout3_mode == cmp.sr_doutmode.spi_dout3_mode;
	if(printout && !test)
		this->printComp("spi_dout3_mode", this->sr_doutmode.spi_dout3_mode, cmp.sr_doutmode.spi_dout3_mode, "<- cmp");
	ret &= test;
	
	test = this->sr_doutmode.spi_dout2_mode == cmp.sr_doutmode.spi_dout2_mode;
	if(printout && !test)
		this->printComp("spi_dout2_mode", this->sr_doutmode.spi_dout2_mode, cmp.sr_doutmode.spi_dout2_mode, "<- cmp");
	ret &= test;
	
	test = this->sr_doutmode.spi_dout1_mode == cmp.sr_doutmode.spi_dout1_mode;
	if(printout && !test)
		this->printComp("spi_dout1_mode", this->sr_doutmode.spi_dout1_mode, cmp.sr_doutmode.spi_dout1_mode, "<- cmp");
	ret &= test;
	
	test = this->sr_doutmode.spi_dout0_mode == cmp.sr_doutmode.spi_dout0_mode;
	if(printout && !test)
		this->printComp("spi_dout0_mode", this->sr_doutmode.spi_dout0_mode, cmp.sr_doutmode.spi_dout0_mode, "<- cmp");
	ret &= test;

	return ret;
}

bool SpiDebug::compInterupts(SpiDebug cmp, bool printout){
	bool ret = this->comp_sr_dmaintena(cmp, printout);
	ret = !ret ? (this->comp_sr_dmaintclr(cmp, printout) & ret) : this->comp_sr_dmaintclr(cmp, printout);
	ret = !ret ? (this->comp_sr_dmaintraw(cmp, printout) & ret) : this->comp_sr_dmaintraw(cmp, printout);
	ret = !ret ? (this->comp_sr_dmaintst(cmp, printout) & ret) : this->comp_sr_dmaintst(cmp, printout);
	ret = !ret ? (this->comp_sr_dmaintset(cmp, printout) & ret) : this->comp_sr_dmaintset(cmp, printout);
	return false;
}
bool SpiDebug::comp_sr_dmaintena(SpiDebug cmp, bool printout){
	bool ret = true;
	bool test = this->sr_dmaintena.spi_app1_int_ena == cmp.sr_dmaintena.spi_app1_int_ena;
	if(printout && !test)
		this->printComp("spi_app1_int_ena", this->sr_dmaintena.spi_app1_int_ena, cmp.sr_dmaintena.spi_app1_int_ena, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintena.spi_app2_int_ena == cmp.sr_dmaintena.spi_app2_int_ena;
	if(printout && !test)
		this->printComp("spi_app2_int_ena", this->sr_dmaintena.spi_app2_int_ena, cmp.sr_dmaintena.spi_app2_int_ena, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintena.spi_mst_tx_afifo_rempty_err_int_ena == cmp.sr_dmaintena.spi_mst_tx_afifo_rempty_err_int_ena;
	if(printout && !test)
		this->printComp("spi_mst_tx_afifo_rempty_err_int_ena", this->sr_dmaintena.spi_mst_tx_afifo_rempty_err_int_ena, cmp.sr_dmaintena.spi_mst_tx_afifo_rempty_err_int_ena, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintena.spi_mst_rx_afifo_wfull_err_int_ena == cmp.sr_dmaintena.spi_mst_rx_afifo_wfull_err_int_ena;
	if(printout && !test)
		this->printComp("spi_mst_rx_afifo_wfull_err_int_ena", this->sr_dmaintena.spi_mst_rx_afifo_wfull_err_int_ena, cmp.sr_dmaintena.spi_mst_rx_afifo_wfull_err_int_ena, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintena.spi_slv_cmd_err_int_ena == cmp.sr_dmaintena.spi_slv_cmd_err_int_ena;
	if(printout && !test)
		this->printComp("spi_slv_cmd_err_int_ena", this->sr_dmaintena.spi_slv_cmd_err_int_ena, cmp.sr_dmaintena.spi_slv_cmd_err_int_ena, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintena.r_spi_seg_magic_err_int_ena == cmp.sr_dmaintena.r_spi_seg_magic_err_int_ena;
	if(printout && !test)
		this->printComp("r_spi_seg_magic_err_int_ena", this->sr_dmaintena.r_spi_seg_magic_err_int_ena, cmp.sr_dmaintena.r_spi_seg_magic_err_int_ena, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintena.spi_dma_seg_trans_done_int_ena == cmp.sr_dmaintena.spi_dma_seg_trans_done_int_ena;
	if(printout && !test)
		this->printComp("spi_dma_seg_trans_done_int_ena", this->sr_dmaintena.spi_dma_seg_trans_done_int_ena, cmp.sr_dmaintena.spi_dma_seg_trans_done_int_ena, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintena.spi_trans_done_int_ena == cmp.sr_dmaintena.spi_trans_done_int_ena;
	if(printout && !test)
		this->printComp("spi_trans_done_int_ena", this->sr_dmaintena.spi_trans_done_int_ena, cmp.sr_dmaintena.spi_trans_done_int_ena, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintena.spi_slv_wr_buf_done_int_ena == cmp.sr_dmaintena.spi_slv_wr_buf_done_int_ena;
	if(printout && !test)
		this->printComp("spi_slv_wr_buf_done_int_ena", this->sr_dmaintena.spi_slv_wr_buf_done_int_ena, cmp.sr_dmaintena.spi_slv_wr_buf_done_int_ena, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintena.spi_slv_rd_buf_done_int_ena == cmp.sr_dmaintena.spi_slv_rd_buf_done_int_ena;
	if(printout && !test)
		this->printComp("spi_slv_rd_buf_done_int_ena", this->sr_dmaintena.spi_slv_rd_buf_done_int_ena, cmp.sr_dmaintena.spi_slv_rd_buf_done_int_ena, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintena.spi_slv_wr_dma_done_int_ena == cmp.sr_dmaintena.spi_slv_wr_dma_done_int_ena;
	if(printout && !test)
		this->printComp("spi_slv_wr_dma_done_int_ena", this->sr_dmaintena.spi_slv_wr_dma_done_int_ena, cmp.sr_dmaintena.spi_slv_wr_dma_done_int_ena, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintena.spi_slv_rd_dma_done_int_ena == cmp.sr_dmaintena.spi_slv_rd_dma_done_int_ena;
	if(printout && !test)
		this->printComp("spi_slv_rd_dma_done_int_ena", this->sr_dmaintena.spi_slv_rd_dma_done_int_ena, cmp.sr_dmaintena.spi_slv_rd_dma_done_int_ena, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintena.spi_slv_cmda_int_ena == cmp.sr_dmaintena.spi_slv_cmda_int_ena;
	if(printout && !test)
		this->printComp("spi_slv_cmda_int_ena", this->sr_dmaintena.spi_slv_cmda_int_ena, cmp.sr_dmaintena.spi_slv_cmda_int_ena, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintena.spi_slv_cmd9_int_ena == cmp.sr_dmaintena.spi_slv_cmd9_int_ena;
	if(printout && !test)
		this->printComp("spi_slv_cmd9_int_ena", this->sr_dmaintena.spi_slv_cmd9_int_ena, cmp.sr_dmaintena.spi_slv_cmd9_int_ena, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintena.spi_slv_cmd8_int_ena == cmp.sr_dmaintena.spi_slv_cmd8_int_ena;
	if(printout && !test)
		this->printComp("spi_slv_cmd8_int_ena", this->sr_dmaintena.spi_slv_cmd8_int_ena, cmp.sr_dmaintena.spi_slv_cmd8_int_ena, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintena.spi_slv_cmd7_int_ena == cmp.sr_dmaintena.spi_slv_cmd7_int_ena;
	if(printout && !test)
		this->printComp("spi_slv_cmd7_int_ena", this->sr_dmaintena.spi_slv_cmd7_int_ena, cmp.sr_dmaintena.spi_slv_cmd7_int_ena, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintena.spi_slv_en_qpi_int_ena == cmp.sr_dmaintena.spi_slv_en_qpi_int_ena;
	if(printout && !test)
		this->printComp("spi_slv_en_qpi_int_ena", this->sr_dmaintena.spi_slv_en_qpi_int_ena, cmp.sr_dmaintena.spi_slv_en_qpi_int_ena, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintena.spi_slv_ex_qpi_int_ena == cmp.sr_dmaintena.spi_slv_ex_qpi_int_ena;
	if(printout && !test)
		this->printComp("spi_slv_ex_qpi_int_ena", this->sr_dmaintena.spi_slv_ex_qpi_int_ena, cmp.sr_dmaintena.spi_slv_ex_qpi_int_ena, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintena.spi_dma_outfifo_empty_err_int_ena == cmp.sr_dmaintena.spi_dma_outfifo_empty_err_int_ena;
	if(printout && !test)
		this->printComp("spi_dma_outfifo_empty_err_int_ena", this->sr_dmaintena.spi_dma_outfifo_empty_err_int_ena, cmp.sr_dmaintena.spi_dma_outfifo_empty_err_int_ena, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintena.spi_dma_infifo_full_err_int_ena == cmp.sr_dmaintena.spi_dma_infifo_full_err_int_ena;
	if(printout && !test)
		this->printComp("spi_dma_infifo_full_err_int_ena", this->sr_dmaintena.spi_dma_infifo_full_err_int_ena, cmp.sr_dmaintena.spi_dma_infifo_full_err_int_ena, "<- cmp");
	ret &= test;
	
	return ret;
}
bool SpiDebug::comp_sr_dmaintclr(SpiDebug cmp, bool printout){
	bool ret = true;
	bool test = this->sr_dmaintclr.spi_app1_int_clr == cmp.sr_dmaintclr.spi_app1_int_clr;
	if(printout && !test)
		this->printComp("spi_app1_int_clr", this->sr_dmaintclr.spi_app1_int_clr, cmp.sr_dmaintclr.spi_app1_int_clr, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintclr.spi_app2_int_clr == cmp.sr_dmaintclr.spi_app2_int_clr;
	if(printout && !test)
		this->printComp("spi_app2_int_clr", this->sr_dmaintclr.spi_app2_int_clr, cmp.sr_dmaintclr.spi_app2_int_clr, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintclr.spi_mst_tx_afifo_rempty_err_int_clr == cmp.sr_dmaintclr.spi_mst_tx_afifo_rempty_err_int_clr;
	if(printout && !test)
		this->printComp("spi_mst_tx_afifo_rempty_err_int_clr", this->sr_dmaintclr.spi_mst_tx_afifo_rempty_err_int_clr, cmp.sr_dmaintclr.spi_mst_tx_afifo_rempty_err_int_clr, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintclr.spi_mst_rx_afifo_wfull_err_int_clr == cmp.sr_dmaintclr.spi_mst_rx_afifo_wfull_err_int_clr;
	if(printout && !test)
		this->printComp("spi_mst_rx_afifo_wfull_err_int_clr", this->sr_dmaintclr.spi_mst_rx_afifo_wfull_err_int_clr, cmp.sr_dmaintclr.spi_mst_rx_afifo_wfull_err_int_clr, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintclr.spi_slv_cmd_err_int_clr == cmp.sr_dmaintclr.spi_slv_cmd_err_int_clr;
	if(printout && !test)
		this->printComp("spi_slv_cmd_err_int_clr", this->sr_dmaintclr.spi_slv_cmd_err_int_clr, cmp.sr_dmaintclr.spi_slv_cmd_err_int_clr, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintclr.r_spi_seg_magic_err_int_clr == cmp.sr_dmaintclr.r_spi_seg_magic_err_int_clr;
	if(printout && !test)
		this->printComp("r_spi_seg_magic_err_int_clr", this->sr_dmaintclr.r_spi_seg_magic_err_int_clr, cmp.sr_dmaintclr.r_spi_seg_magic_err_int_clr, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintclr.spi_dma_seg_trans_done_int_clr == cmp.sr_dmaintclr.spi_dma_seg_trans_done_int_clr;
	if(printout && !test)
		this->printComp("spi_dma_seg_trans_done_int_clr", this->sr_dmaintclr.spi_dma_seg_trans_done_int_clr, cmp.sr_dmaintclr.spi_dma_seg_trans_done_int_clr, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintclr.spi_trans_done_int_clr == cmp.sr_dmaintclr.spi_trans_done_int_clr;
	if(printout && !test)
		this->printComp("spi_trans_done_int_clr", this->sr_dmaintclr.spi_trans_done_int_clr, cmp.sr_dmaintclr.spi_trans_done_int_clr, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintclr.spi_slv_wr_buf_done_int_clr == cmp.sr_dmaintclr.spi_slv_wr_buf_done_int_clr;
	if(printout && !test)
		this->printComp("spi_slv_wr_buf_done_int_clr", this->sr_dmaintclr.spi_slv_wr_buf_done_int_clr, cmp.sr_dmaintclr.spi_slv_wr_buf_done_int_clr, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintclr.spi_slv_rd_buf_done_int_clr == cmp.sr_dmaintclr.spi_slv_rd_buf_done_int_clr;
	if(printout && !test)
		this->printComp("spi_slv_rd_buf_done_int_clr", this->sr_dmaintclr.spi_slv_rd_buf_done_int_clr, cmp.sr_dmaintclr.spi_slv_rd_buf_done_int_clr, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintclr.spi_slv_wr_dma_done_int_clr == cmp.sr_dmaintclr.spi_slv_wr_dma_done_int_clr;
	if(printout && !test)
		this->printComp("spi_slv_wr_dma_done_int_clr", this->sr_dmaintclr.spi_slv_wr_dma_done_int_clr, cmp.sr_dmaintclr.spi_slv_wr_dma_done_int_clr, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintclr.spi_slv_rd_dma_done_int_clr == cmp.sr_dmaintclr.spi_slv_rd_dma_done_int_clr;
	if(printout && !test)
		this->printComp("spi_slv_rd_dma_done_int_clr", this->sr_dmaintclr.spi_slv_rd_dma_done_int_clr, cmp.sr_dmaintclr.spi_slv_rd_dma_done_int_clr, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintclr.spi_slv_cmda_int_clr == cmp.sr_dmaintclr.spi_slv_cmda_int_clr;
	if(printout && !test)
		this->printComp("spi_slv_cmda_int_clr", this->sr_dmaintclr.spi_slv_cmda_int_clr, cmp.sr_dmaintclr.spi_slv_cmda_int_clr, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintclr.spi_slv_cmd9_int_clr == cmp.sr_dmaintclr.spi_slv_cmd9_int_clr;
	if(printout && !test)
		this->printComp("spi_slv_cmd9_int_clr", this->sr_dmaintclr.spi_slv_cmd9_int_clr, cmp.sr_dmaintclr.spi_slv_cmd9_int_clr, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintclr.spi_slv_cmd8_int_clr == cmp.sr_dmaintclr.spi_slv_cmd8_int_clr;
	if(printout && !test)
		this->printComp("spi_slv_cmd8_int_clr", this->sr_dmaintclr.spi_slv_cmd8_int_clr, cmp.sr_dmaintclr.spi_slv_cmd8_int_clr, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintclr.spi_slv_cmd7_int_clr == cmp.sr_dmaintclr.spi_slv_cmd7_int_clr;
	if(printout && !test)
		this->printComp("spi_slv_cmd7_int_clr", this->sr_dmaintclr.spi_slv_cmd7_int_clr, cmp.sr_dmaintclr.spi_slv_cmd7_int_clr, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintclr.spi_slv_en_qpi_int_clr == cmp.sr_dmaintclr.spi_slv_en_qpi_int_clr;
	if(printout && !test)
		this->printComp("spi_slv_en_qpi_int_clr", this->sr_dmaintclr.spi_slv_en_qpi_int_clr, cmp.sr_dmaintclr.spi_slv_en_qpi_int_clr, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintclr.spi_slv_ex_qpi_int_clr == cmp.sr_dmaintclr.spi_slv_ex_qpi_int_clr;
	if(printout && !test)
		this->printComp("spi_slv_ex_qpi_int_clr", this->sr_dmaintclr.spi_slv_ex_qpi_int_clr, cmp.sr_dmaintclr.spi_slv_ex_qpi_int_clr, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintclr.spi_dma_outfifo_empty_err_int_clr == cmp.sr_dmaintclr.spi_dma_outfifo_empty_err_int_clr;
	if(printout && !test)
		this->printComp("spi_dma_outfifo_empty_err_int_clr", this->sr_dmaintclr.spi_dma_outfifo_empty_err_int_clr, cmp.sr_dmaintclr.spi_dma_outfifo_empty_err_int_clr, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintclr.spi_dma_infifo_full_err_int_clr == cmp.sr_dmaintclr.spi_dma_infifo_full_err_int_clr;
	if(printout && !test)
		this->printComp("spi_dma_infifo_full_err_int_clr", this->sr_dmaintclr.spi_dma_infifo_full_err_int_clr, cmp.sr_dmaintclr.spi_dma_infifo_full_err_int_clr, "<- cmp");
	ret &= test;

	return ret;
}
bool SpiDebug::comp_sr_dmaintraw(SpiDebug cmp, bool printout){
	bool ret = true;
	bool test = this->sr_dmaintraw.spi_app1_int_raw == cmp.sr_dmaintraw.spi_app1_int_raw;
	if(printout && !test)
		this->printComp("spi_app1_int_raw", this->sr_dmaintraw.spi_app1_int_raw, cmp.sr_dmaintraw.spi_app1_int_raw, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintraw.spi_app2_int_raw == cmp.sr_dmaintraw.spi_app2_int_raw;
	if(printout && !test)
		this->printComp("spi_app2_int_raw", this->sr_dmaintraw.spi_app2_int_raw, cmp.sr_dmaintraw.spi_app2_int_raw, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintraw.spi_mst_tx_afifo_rempty_err_int_raw == cmp.sr_dmaintraw.spi_mst_tx_afifo_rempty_err_int_raw;
	if(printout && !test)
		this->printComp("spi_mst_tx_afifo_rempty_err_int_raw", this->sr_dmaintraw.spi_mst_tx_afifo_rempty_err_int_raw, cmp.sr_dmaintraw.spi_mst_tx_afifo_rempty_err_int_raw, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintraw.spi_mst_rx_afifo_wfull_err_int_raw == cmp.sr_dmaintraw.spi_mst_rx_afifo_wfull_err_int_raw;
	if(printout && !test)
		this->printComp("spi_mst_rx_afifo_wfull_err_int_raw", this->sr_dmaintraw.spi_mst_rx_afifo_wfull_err_int_raw, cmp.sr_dmaintraw.spi_mst_rx_afifo_wfull_err_int_raw, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintraw.spi_slv_cmd_err_int_raw == cmp.sr_dmaintraw.spi_slv_cmd_err_int_raw;
	if(printout && !test)
		this->printComp("spi_slv_cmd_err_int_raw", this->sr_dmaintraw.spi_slv_cmd_err_int_raw, cmp.sr_dmaintraw.spi_slv_cmd_err_int_raw, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintraw.r_spi_seg_magic_err_int_raw == cmp.sr_dmaintraw.r_spi_seg_magic_err_int_raw;
	if(printout && !test)
		this->printComp("r_spi_seg_magic_err_int_raw", this->sr_dmaintraw.r_spi_seg_magic_err_int_raw, cmp.sr_dmaintraw.r_spi_seg_magic_err_int_raw, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintraw.spi_dma_seg_trans_done_int_raw == cmp.sr_dmaintraw.spi_dma_seg_trans_done_int_raw;
	if(printout && !test)
		this->printComp("spi_dma_seg_trans_done_int_raw", this->sr_dmaintraw.spi_dma_seg_trans_done_int_raw, cmp.sr_dmaintraw.spi_dma_seg_trans_done_int_raw, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintraw.spi_trans_done_int_raw == cmp.sr_dmaintraw.spi_trans_done_int_raw;
	if(printout && !test)
		this->printComp("spi_trans_done_int_raw", this->sr_dmaintraw.spi_trans_done_int_raw, cmp.sr_dmaintraw.spi_trans_done_int_raw, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintraw.spi_slv_wr_buf_done_int_raw == cmp.sr_dmaintraw.spi_slv_wr_buf_done_int_raw;
	if(printout && !test)
		this->printComp("spi_slv_wr_buf_done_int_raw", this->sr_dmaintraw.spi_slv_wr_buf_done_int_raw, cmp.sr_dmaintraw.spi_slv_wr_buf_done_int_raw, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintraw.spi_slv_rd_buf_done_int_raw == cmp.sr_dmaintraw.spi_slv_rd_buf_done_int_raw;
	if(printout && !test)
		this->printComp("spi_slv_rd_buf_done_int_raw", this->sr_dmaintraw.spi_slv_rd_buf_done_int_raw, cmp.sr_dmaintraw.spi_slv_rd_buf_done_int_raw, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintraw.spi_slv_wr_dma_done_int_raw == cmp.sr_dmaintraw.spi_slv_wr_dma_done_int_raw;
	if(printout && !test)
		this->printComp("spi_slv_wr_dma_done_int_raw", this->sr_dmaintraw.spi_slv_wr_dma_done_int_raw, cmp.sr_dmaintraw.spi_slv_wr_dma_done_int_raw, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintraw.spi_slv_rd_dma_done_int_raw == cmp.sr_dmaintraw.spi_slv_rd_dma_done_int_raw;
	if(printout && !test)
		this->printComp("spi_slv_rd_dma_done_int_raw", this->sr_dmaintraw.spi_slv_rd_dma_done_int_raw, cmp.sr_dmaintraw.spi_slv_rd_dma_done_int_raw, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintraw.spi_slv_cmda_int_raw == cmp.sr_dmaintraw.spi_slv_cmda_int_raw;
	if(printout && !test)
		this->printComp("spi_slv_cmda_int_raw", this->sr_dmaintraw.spi_slv_cmda_int_raw, cmp.sr_dmaintraw.spi_slv_cmda_int_raw, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintraw.spi_slv_cmd9_int_raw == cmp.sr_dmaintraw.spi_slv_cmd9_int_raw;
	if(printout && !test)
		this->printComp("spi_slv_cmd9_int_raw", this->sr_dmaintraw.spi_slv_cmd9_int_raw, cmp.sr_dmaintraw.spi_slv_cmd9_int_raw, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintraw.spi_slv_cmd8_int_raw == cmp.sr_dmaintraw.spi_slv_cmd8_int_raw;
	if(printout && !test)
		this->printComp("spi_slv_cmd8_int_raw", this->sr_dmaintraw.spi_slv_cmd8_int_raw, cmp.sr_dmaintraw.spi_slv_cmd8_int_raw, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintraw.spi_slv_cmd7_int_raw == cmp.sr_dmaintraw.spi_slv_cmd7_int_raw;
	if(printout && !test)
		this->printComp("spi_slv_cmd7_int_raw", this->sr_dmaintraw.spi_slv_cmd7_int_raw, cmp.sr_dmaintraw.spi_slv_cmd7_int_raw, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintraw.spi_slv_en_qpi_int_raw == cmp.sr_dmaintraw.spi_slv_en_qpi_int_raw;
	if(printout && !test)
		this->printComp("spi_slv_en_qpi_int_raw", this->sr_dmaintraw.spi_slv_en_qpi_int_raw, cmp.sr_dmaintraw.spi_slv_en_qpi_int_raw, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintraw.spi_slv_ex_qpi_int_raw == cmp.sr_dmaintraw.spi_slv_ex_qpi_int_raw;
	if(printout && !test)
		this->printComp("spi_slv_ex_qpi_int_raw", this->sr_dmaintraw.spi_slv_ex_qpi_int_raw, cmp.sr_dmaintraw.spi_slv_ex_qpi_int_raw, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintraw.spi_dma_outfifo_empty_err_int_raw == cmp.sr_dmaintraw.spi_dma_outfifo_empty_err_int_raw;
	if(printout && !test)
		this->printComp("spi_dma_outfifo_empty_err_int_raw", this->sr_dmaintraw.spi_dma_outfifo_empty_err_int_raw, cmp.sr_dmaintraw.spi_dma_outfifo_empty_err_int_raw, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintraw.spi_dma_infifo_full_err_int_raw == cmp.sr_dmaintraw.spi_dma_infifo_full_err_int_raw;
	if(printout && !test)
		this->printComp("spi_dma_infifo_full_err_int_raw", this->sr_dmaintraw.spi_dma_infifo_full_err_int_raw, cmp.sr_dmaintraw.spi_dma_infifo_full_err_int_raw, "<- cmp");
	ret &= test;

	return ret;
}
bool SpiDebug::comp_sr_dmaintst(SpiDebug cmp, bool printout){
	bool ret = true;
	bool test = this->sr_dmaintst.spi_app1_int_st == cmp.sr_dmaintst.spi_app1_int_st;
	if(printout && !test)
		this->printComp("spi_app1_int_st", this->sr_dmaintst.spi_app1_int_st, cmp.sr_dmaintst.spi_app1_int_st, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintst.spi_app2_int_st == cmp.sr_dmaintst.spi_app2_int_st;
	if(printout && !test)
		this->printComp("spi_app2_int_st", this->sr_dmaintst.spi_app2_int_st, cmp.sr_dmaintst.spi_app2_int_st, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintst.spi_mst_tx_afifo_rempty_err_int_st == cmp.sr_dmaintst.spi_mst_tx_afifo_rempty_err_int_st;
	if(printout && !test)
		this->printComp("spi_mst_tx_afifo_rempty_err_int_st", this->sr_dmaintst.spi_mst_tx_afifo_rempty_err_int_st, cmp.sr_dmaintst.spi_mst_tx_afifo_rempty_err_int_st, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintst.spi_mst_rx_afifo_wfull_err_int_st == cmp.sr_dmaintst.spi_mst_rx_afifo_wfull_err_int_st;
	if(printout && !test)
		this->printComp("spi_mst_rx_afifo_wfull_err_int_st", this->sr_dmaintst.spi_mst_rx_afifo_wfull_err_int_st, cmp.sr_dmaintst.spi_mst_rx_afifo_wfull_err_int_st, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintst.spi_slv_cmd_err_int_st == cmp.sr_dmaintst.spi_slv_cmd_err_int_st;
	if(printout && !test)
		this->printComp("spi_slv_cmd_err_int_st", this->sr_dmaintst.spi_slv_cmd_err_int_st, cmp.sr_dmaintst.spi_slv_cmd_err_int_st, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintst.r_spi_seg_magic_err_int_st == cmp.sr_dmaintst.r_spi_seg_magic_err_int_st;
	if(printout && !test)
		this->printComp("r_spi_seg_magic_err_int_st", this->sr_dmaintst.r_spi_seg_magic_err_int_st, cmp.sr_dmaintst.r_spi_seg_magic_err_int_st, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintst.spi_dma_seg_trans_done_int_st == cmp.sr_dmaintst.spi_dma_seg_trans_done_int_st;
	if(printout && !test)
		this->printComp("spi_dma_seg_trans_done_int_st", this->sr_dmaintst.spi_dma_seg_trans_done_int_st, cmp.sr_dmaintst.spi_dma_seg_trans_done_int_st, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintst.spi_trans_done_int_st == cmp.sr_dmaintst.spi_trans_done_int_st;
	if(printout && !test)
		this->printComp("spi_trans_done_int_st", this->sr_dmaintst.spi_trans_done_int_st, cmp.sr_dmaintst.spi_trans_done_int_st, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintst.spi_slv_wr_buf_done_int_st == cmp.sr_dmaintst.spi_slv_wr_buf_done_int_st;
	if(printout && !test)
		this->printComp("spi_slv_wr_buf_done_int_st", this->sr_dmaintst.spi_slv_wr_buf_done_int_st, cmp.sr_dmaintst.spi_slv_wr_buf_done_int_st, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintst.spi_slv_rd_buf_done_int_st == cmp.sr_dmaintst.spi_slv_rd_buf_done_int_st;
	if(printout && !test)
		this->printComp("spi_slv_rd_buf_done_int_st", this->sr_dmaintst.spi_slv_rd_buf_done_int_st, cmp.sr_dmaintst.spi_slv_rd_buf_done_int_st, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintst.spi_slv_wr_dma_done_int_st == cmp.sr_dmaintst.spi_slv_wr_dma_done_int_st;
	if(printout && !test)
		this->printComp("spi_slv_wr_dma_done_int_st", this->sr_dmaintst.spi_slv_wr_dma_done_int_st, cmp.sr_dmaintst.spi_slv_wr_dma_done_int_st, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintst.spi_slv_rd_dma_done_int_st == cmp.sr_dmaintst.spi_slv_rd_dma_done_int_st;
	if(printout && !test)
		this->printComp("spi_slv_rd_dma_done_int_st", this->sr_dmaintst.spi_slv_rd_dma_done_int_st, cmp.sr_dmaintst.spi_slv_rd_dma_done_int_st, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintst.spi_slv_cmda_int_st == cmp.sr_dmaintst.spi_slv_cmda_int_st;
	if(printout && !test)
		this->printComp("spi_slv_cmda_int_st", this->sr_dmaintst.spi_slv_cmda_int_st, cmp.sr_dmaintst.spi_slv_cmda_int_st, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintst.spi_slv_cmd9_int_st == cmp.sr_dmaintst.spi_slv_cmd9_int_st;
	if(printout && !test)
		this->printComp("spi_slv_cmd9_int_st", this->sr_dmaintst.spi_slv_cmd9_int_st, cmp.sr_dmaintst.spi_slv_cmd9_int_st, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintst.spi_slv_cmd8_int_st == cmp.sr_dmaintst.spi_slv_cmd8_int_st;
	if(printout && !test)
		this->printComp("spi_slv_cmd8_int_st", this->sr_dmaintst.spi_slv_cmd8_int_st, cmp.sr_dmaintst.spi_slv_cmd8_int_st, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintst.spi_slv_cmd7_int_st == cmp.sr_dmaintst.spi_slv_cmd7_int_st;
	if(printout && !test)
		this->printComp("spi_slv_cmd7_int_st", this->sr_dmaintst.spi_slv_cmd7_int_st, cmp.sr_dmaintst.spi_slv_cmd7_int_st, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintst.spi_slv_en_qpi_int_st == cmp.sr_dmaintst.spi_slv_en_qpi_int_st;
	if(printout && !test)
		this->printComp("spi_slv_en_qpi_int_st", this->sr_dmaintst.spi_slv_en_qpi_int_st, cmp.sr_dmaintst.spi_slv_en_qpi_int_st, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintst.spi_slv_ex_qpi_int_st == cmp.sr_dmaintst.spi_slv_ex_qpi_int_st;
	if(printout && !test)
		this->printComp("spi_slv_ex_qpi_int_st", this->sr_dmaintst.spi_slv_ex_qpi_int_st, cmp.sr_dmaintst.spi_slv_ex_qpi_int_st, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintst.spi_dma_outfifo_empty_err_int_st == cmp.sr_dmaintst.spi_dma_outfifo_empty_err_int_st;
	if(printout && !test)
		this->printComp("spi_dma_outfifo_empty_err_int_st", this->sr_dmaintst.spi_dma_outfifo_empty_err_int_st, cmp.sr_dmaintst.spi_dma_outfifo_empty_err_int_st, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintst.spi_dma_infifo_full_err_int_st == cmp.sr_dmaintst.spi_dma_infifo_full_err_int_st;
	if(printout && !test)
		this->printComp("spi_dma_infifo_full_err_int_st", this->sr_dmaintst.spi_dma_infifo_full_err_int_st, cmp.sr_dmaintst.spi_dma_infifo_full_err_int_st, "<- cmp");
	ret &= test;

	return ret;
}
bool SpiDebug::comp_sr_dmaintset(SpiDebug cmp, bool printout){
	bool ret = true;
	bool test = this->sr_dmaintset.spi_app1_int_set == cmp.sr_dmaintset.spi_app1_int_set;
	if(printout && !test)
		this->printComp("spi_app1_int_set", this->sr_dmaintset.spi_app1_int_set, cmp.sr_dmaintset.spi_app1_int_set, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintset.spi_app2_int_set == cmp.sr_dmaintset.spi_app2_int_set;
	if(printout && !test)
		this->printComp("spi_app2_int_set", this->sr_dmaintset.spi_app2_int_set, cmp.sr_dmaintset.spi_app2_int_set, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintset.spi_mst_tx_afifo_rempty_err_int_set == cmp.sr_dmaintset.spi_mst_tx_afifo_rempty_err_int_set;
	if(printout && !test)
		this->printComp("spi_mst_tx_afifo_rempty_err_int_set", this->sr_dmaintset.spi_mst_tx_afifo_rempty_err_int_set, cmp.sr_dmaintset.spi_mst_tx_afifo_rempty_err_int_set, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintset.spi_mst_rx_afifo_wfull_err_int_set == cmp.sr_dmaintset.spi_mst_rx_afifo_wfull_err_int_set;
	if(printout && !test)
		this->printComp("spi_mst_rx_afifo_wfull_err_int_set", this->sr_dmaintset.spi_mst_rx_afifo_wfull_err_int_set, cmp.sr_dmaintset.spi_mst_rx_afifo_wfull_err_int_set, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintset.spi_slv_cmd_err_int_set == cmp.sr_dmaintset.spi_slv_cmd_err_int_set;
	if(printout && !test)
		this->printComp("spi_slv_cmd_err_int_set", this->sr_dmaintset.spi_slv_cmd_err_int_set, cmp.sr_dmaintset.spi_slv_cmd_err_int_set, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintset.r_spi_seg_magic_err_int_set == cmp.sr_dmaintset.r_spi_seg_magic_err_int_set;
	if(printout && !test)
		this->printComp("r_spi_seg_magic_err_int_set", this->sr_dmaintset.r_spi_seg_magic_err_int_set, cmp.sr_dmaintset.r_spi_seg_magic_err_int_set, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintset.spi_dma_seg_trans_done_int_set == cmp.sr_dmaintset.spi_dma_seg_trans_done_int_set;
	if(printout && !test)
		this->printComp("spi_dma_seg_trans_done_int_set", this->sr_dmaintset.spi_dma_seg_trans_done_int_set, cmp.sr_dmaintset.spi_dma_seg_trans_done_int_set, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintset.spi_trans_done_int_set == cmp.sr_dmaintset.spi_trans_done_int_set;
	if(printout && !test)
		this->printComp("spi_trans_done_int_set", this->sr_dmaintset.spi_trans_done_int_set, cmp.sr_dmaintset.spi_trans_done_int_set, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintset.spi_slv_wr_buf_done_int_set == cmp.sr_dmaintset.spi_slv_wr_buf_done_int_set;
	if(printout && !test)
		this->printComp("spi_slv_wr_buf_done_int_set", this->sr_dmaintset.spi_slv_wr_buf_done_int_set, cmp.sr_dmaintset.spi_slv_wr_buf_done_int_set, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintset.spi_slv_rd_buf_done_int_set == cmp.sr_dmaintset.spi_slv_rd_buf_done_int_set;
	if(printout && !test)
		this->printComp("spi_slv_rd_buf_done_int_set", this->sr_dmaintset.spi_slv_rd_buf_done_int_set, cmp.sr_dmaintset.spi_slv_rd_buf_done_int_set, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintset.spi_slv_wr_dma_done_int_set == cmp.sr_dmaintset.spi_slv_wr_dma_done_int_set;
	if(printout && !test)
		this->printComp("spi_slv_wr_dma_done_int_set", this->sr_dmaintset.spi_slv_wr_dma_done_int_set, cmp.sr_dmaintset.spi_slv_wr_dma_done_int_set, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintset.spi_slv_rd_dma_done_int_set == cmp.sr_dmaintset.spi_slv_rd_dma_done_int_set;
	if(printout && !test)
		this->printComp("spi_slv_rd_dma_done_int_set", this->sr_dmaintset.spi_slv_rd_dma_done_int_set, cmp.sr_dmaintset.spi_slv_rd_dma_done_int_set, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintset.spi_slv_cmda_int_set == cmp.sr_dmaintset.spi_slv_cmda_int_set;
	if(printout && !test)
		this->printComp("spi_slv_cmda_int_set", this->sr_dmaintset.spi_slv_cmda_int_set, cmp.sr_dmaintset.spi_slv_cmda_int_set, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintset.spi_slv_cmd9_int_set == cmp.sr_dmaintset.spi_slv_cmd9_int_set;
	if(printout && !test)
		this->printComp("spi_slv_cmd9_int_set", this->sr_dmaintset.spi_slv_cmd9_int_set, cmp.sr_dmaintset.spi_slv_cmd9_int_set, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintset.spi_slv_cmd8_int_set == cmp.sr_dmaintset.spi_slv_cmd8_int_set;
	if(printout && !test)
		this->printComp("spi_slv_cmd8_int_set", this->sr_dmaintset.spi_slv_cmd8_int_set, cmp.sr_dmaintset.spi_slv_cmd8_int_set, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintset.spi_slv_cmd7_int_set == cmp.sr_dmaintset.spi_slv_cmd7_int_set;
	if(printout && !test)
		this->printComp("spi_slv_cmd7_int_set", this->sr_dmaintset.spi_slv_cmd7_int_set, cmp.sr_dmaintset.spi_slv_cmd7_int_set, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintset.spi_slv_en_qpi_int_set == cmp.sr_dmaintset.spi_slv_en_qpi_int_set;
	if(printout && !test)
		this->printComp("spi_slv_en_qpi_int_set", this->sr_dmaintset.spi_slv_en_qpi_int_set, cmp.sr_dmaintset.spi_slv_en_qpi_int_set, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintset.spi_slv_ex_qpi_int_set == cmp.sr_dmaintset.spi_slv_ex_qpi_int_set;
	if(printout && !test)
		this->printComp("spi_slv_ex_qpi_int_set", this->sr_dmaintset.spi_slv_ex_qpi_int_set, cmp.sr_dmaintset.spi_slv_ex_qpi_int_set, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintset.spi_dma_outfifo_empty_err_int_set == cmp.sr_dmaintset.spi_dma_outfifo_empty_err_int_set;
	if(printout && !test)
		this->printComp("spi_dma_outfifo_empty_err_int_set", this->sr_dmaintset.spi_dma_outfifo_empty_err_int_set, cmp.sr_dmaintset.spi_dma_outfifo_empty_err_int_set, "<- cmp");
	ret &= test;
	
	test = this->sr_dmaintset.spi_dma_infifo_full_err_int_set == cmp.sr_dmaintset.spi_dma_infifo_full_err_int_set;
	if(printout && !test)
		this->printComp("spi_dma_infifo_full_err_int_set", this->sr_dmaintset.spi_dma_infifo_full_err_int_set, cmp.sr_dmaintset.spi_dma_infifo_full_err_int_set, "<- cmp");
	ret &= test;

	return ret;
}

bool SpiDebug::compAll(SpiDebug cmp, bool printout){
	bool ret = true;
	bool test = this->compUserDef(cmp, printout);
	ret &= test;
	test = this->compCtrlConf(cmp, printout);
	ret &= test;
	test = this->compClock(cmp, printout);
	ret &= test;
	test = this->compTiming(cmp, printout);
	ret &= test;
	test = this->compInterupts(cmp, printout);
	ret &= test;
	return ret;
}

/// Make sure to init/refresh before invoking.
bool SpiDebug::isSlave(void){
  return this->sr_slave.spi_slave_mode;
}



