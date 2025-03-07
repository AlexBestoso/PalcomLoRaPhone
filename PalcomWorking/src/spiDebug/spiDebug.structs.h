/*
 * User-defined control registers
 * */
/// Command control register   varies
struct dbg_spi_reg_cmd{
  uint32_t val = 0;
  uint32_t reset = 0;

  uint16_t base_2 = 0x0000;
  uint16_t base_3 = 0x0000;
  
  bool spi_user = false;    uint8_t spi_user_pos = 24;
  bool spi_update = false;  uint8_t spi_update_pos = 23; 
};

/// Address value register  R/W
struct dbg_spi_reg_addr{
  uint32_t val = 0;
  uint32_t reset = 0;

  uint16_t base_2 = 0x0004; 
  uint16_t base_3 = 0x0004;
  
  uint32_t spi_usr_addr_value = 0;
}; 

/// SPI USER control register  varies
struct dbg_spi_reg_user{
  uint32_t val = 0;
  uint32_t reset = 0x800000C0;

  uint16_t base_2 = 0x0010;
  uint16_t base_3 = 0x0010;
  
  bool spi_usr_command = true;          uint8_t spi_usr_command_pos = 31;
  bool spi_usr_addr = false;            uint8_t spi_usr_addr_pos = 30;
  bool spi_usr_dummy = false;           uint8_t spi_usr_dummy_pos = 29;
  bool spi_usr_miso = false;            uint8_t spi_usr_miso_pos = 28;
  bool spi_usr_mosi = false;            uint8_t spi_usr_mosi_pos = 27;
  bool spi_usr_dummy_idle = false;      uint8_t spi_usr_dummy_idle_pos = 26;
  bool spi_usr_mosi_highpart = false;   uint8_t spi_usr_mosi_highpart_pos = 25;
  bool spi_usr_miso_highpart = false;   uint8_t spi_usr_miso_highpart_pos = 24;
  bool spi_sio = false;                 uint8_t spi_sio_pos = 17;
  bool r_spi_usr_conf_nxt = false;      uint8_t r_spi_usr_conf_nxt_pos = 15;
  bool r_spi_fwrite_oct = false;        uint8_t r_spi_fwrite_oct_pos = 14;
  bool spi_fwrite_quad = false;         uint8_t spi_fwrite_quad_pos = 13;
  bool spi_fwrite_dual = false;         uint8_t spi_fwrite_dual_pos = 12;
  bool spi_usr_ck_out_edge = false;     uint8_t spi_usr_ck_out_edge_pos = 9;
  bool spi_rsck_i_edge = false;         uint8_t spi_rsck_i_edge_pos = 8;
  bool spi_cs_setup = true;             uint8_t spi_cs_setup_pos = 7;
  bool spi_cs_hold = true;              uint8_t spi_cs_hold_pos = 6;
  bool spi_tsck_i_edge = false;         uint8_t spi_tsck_i_edge_pos = 5;
  bool r_spi_opi_mode = false;          uint8_t r_spi_opi_mode_pos = 4;
  bool spi_qpi_mode = false;            uint8_t spi_qpi_mode_pos = 3;
  bool spi_doutdin = false;             uint8_t spi_doutdin_pos = 0;
}; 

/// SPI USER control register 1  R/W
struct dbg_spi_reg_user1{
  uint32_t val = 0;
  uint32_t reset = 0xB8410007;

  uint16_t base_2 = 0x0014; 
  uint16_t base_3 = 0x0014;

  uint16_t spi_usr_addr_bitlen = 23;    
    uint8_t spi_usr_addr_bitlen_y=31; 
    uint8_t spi_usr_addr_bitlen_x=27;

  uint16_t spi_cs_hold_time = 0x1;      
    uint8_t spi_cs_hold_time_y=26;
    uint8_t spi_cs_hold_time_x=22;

  uint16_t spi_cs_setup_time = 0;       
    uint8_t spi_cs_setup_time_y=21; 
    uint8_t spi_cs_setup_time_x=17;

  bool spi_mst_wfull_err_end_en = true; 
    uint8_t spi_mst_wfull_err_end_en_pos = 16;

  uint16_t spi_usr_dummy_cyclelen = 7; 
    uint8_t spi_usr_dummy_cyclelen_y = 7; 
    uint8_t spi_usr_dummy_cyclelen_x = 0;
}; 

/// SPI USER control register 2  R/W
struct dbg_spi_reg_user2{
  uint32_t val = 0;
  uint32_t reset = 0x78000000;

  uint16_t base_2 = 0x0018;
  uint16_t base_3 = 0x0018;
  
  uint8_t spi_usr_command_bitlen = 7;
    uint8_t spi_usr_command_bitlen_y=31;
    uint8_t spi_usr_command_bitlen_x=28;

  bool spi_mst_rempty_err_end_en = true;
    uint8_t spi_mst_rempty_err_end_en_pos = 27;

  uint16_t spi_usr_command_value = 0;
    uint8_t spi_usr_command_value_y=15;
    uint8_t spi_usr_command_value_x=0;
}; 


/* 
 * Control and configuration registers
 * **/
/// SPI control register   R/W
struct dbg_spi_reg_ctrl{
  uint32_t val = 0;
  uint32_t reset = 0x3C0000;

  uint16_t base_2 = 0x0008;
  uint16_t base_3 = 0x0008;

  uint8_t spi_wr_bit_order = 0;
    uint8_t spi_wr_bit_order_y=26;
    uint8_t spi_wr_bit_order_x=25;

  uint8_t spi_rd_bit_order = 0;
    uint8_t spi_rd_bit_order_y=24;
    uint8_t spi_rd_bit_order_x=23;

  bool spi_wp_pol = true;          uint8_t spi_wp_pol_pos = 21;
  bool spi_hold_pol = true;        uint8_t spi_hold_pol_pos = 20;
  bool spi_d_pol = true;           uint8_t spi_d_pol_pos = 19;
  bool spi_q_pol = true;           uint8_t spi_q_pol_pos = 18;
  bool r_spi_fread_oct = false;    uint8_t r_spi_fread_oct_pos = 16;
  bool spi_fread_quad = false;     uint8_t spi_fread_quad_pos = 15;
  bool spi_fread_dual = false;     uint8_t spi_fread_dual_pos = 14;
  bool r_spi_fcmd_oct = false;     uint8_t r_spi_fcmd_oct_pos = 10;
  bool spi_fcmd_quad = false;      uint8_t spi_fcmd_quad_pos = 9;
  bool spi_fcmd_dual = false;      uint8_t spi_fcmd_dual_pos = 8;
  bool r_spi_faddr_oct = false;    uint8_t r_spi_faddr_oct_pos = 7;
  bool spi_faddr_quad = false;     uint8_t spi_faddr_quad_pos = 6;
  bool spi_faddr_dual = false;     uint8_t spi_faddr_dual_pos = 5;
  bool spi_dummy_out = false;      uint8_t spi_dummy_out_pos = 3;

}; 

/// SPI data bit length control register   R/W
struct dbg_spi_reg_ms_dlen{
  uint32_t val = 0;
  uint32_t reset = 0x0;

  uint16_t base_2 = 0x001C;
  uint16_t base_3 = 0x001C;

  uint32_t spi_ms_data_bitlen = 0;
    uint8_t spi_ms_data_bitlen_y = 17;
    uint8_t spi_ms_data_bitlen_x = 0;
}; 


/// SPI misc register   R/W -- different relative to spi2 and spi3, this is for spi2
struct dbg_spi_reg_misc{
  uint32_t val = 0;
  uint32_t reset = 0x3e;

  uint16_t base_2 = 0x0020;
  uint16_t base_3 = 0x0020;

  bool spi_quad_din_pin_swap = false;  uint8_t spi_quad_din_pin_swap_pos = 31;
  bool spi_cs_keep_active = false;     uint8_t spi_cs_keep_active_pos = 30;
  bool spi_ck_idle_edge = false;       uint8_t spi_ck_idle_edge_pos = 29;
  bool spi_dqs_idle_edge = false;      uint8_t spi_dqs_idle_edge_pos = 24;
  bool spi_slave_cs_pol = false;       uint8_t spi_slave_cs_pol_pos = 23;
  bool spi_cmd_dtr_en = false;         uint8_t spi_cmd_dtr_en_pos = 19;
  bool spi_addr_dtr_en = false;        uint8_t spi_addr_dtr_en_pos = 18;
  bool spi_data_dtr_en = false;        uint8_t spi_data_dtr_en_pos = 17;
  bool spi_clk_data_dtr_en = false;    uint8_t spi_clk_data_dtr_en_pos = 16;

  uint8_t spi_master_cs_pol = 0;
    uint8_t spi_master_cs_pol_y = 12;
    uint8_t spi_master_cs_pol_y3 = 9; // y for SPI 3
    uint8_t spi_master_cs_pol_x = 7;

  bool spi_ck_dis = false;    uint8_t spi_ck_dis_pos = 6;
  bool spi_cs5_dis = true;    uint8_t spi_cs5_dis_pos = 5;
  bool spi_cs4_dis = true;    uint8_t spi_cs4_dis_pos = 4;
  bool spi_cs3_dis = true;    uint8_t spi_cs3_dis_pos = 3;
  bool spi_cs2_dis = true;    uint8_t spi_cs2_dis_pos = 2;
  bool spi_cs1_dis = true;    uint8_t spi_cs1_dis_pos = 1;
  bool spi_cs0_dis = false;   uint8_t spi_cs0_dis_pos = 0;
}; 

/// SPI DMA control register   varies
struct dbg_spi_reg_dma_conf{
  uint32_t val = 0;
  uint32_t reset = 3;

  uint16_t base_2 = 0x0030;
  uint16_t base_3 = 0x0030;

  bool spi_dma_afifo_rst = false;            uint8_t spi_dma_afifo_rst_pos = 31;
  bool spi_buf_afifo_rst = false;            uint8_t spi_buf_afifo_rst_pos = 30;
  bool spi_rx_afifo_rst = false;             uint8_t spi_rx_afifo_rst_pos = 29;
  bool spi_dma_tx_ena = false;               uint8_t spi_dma_tx_ena_pos = 28;
  bool spi_dma_rx_ena = false;               uint8_t spi_dma_rx_ena_pos = 27;
  bool spi_rx_eof_en = false;                uint8_t spi_rx_eof_en_pos = 21;
  bool spi_slv_tx_seg_trans_clr_en = false;  uint8_t spi_slv_tx_seg_trans_clr_en_pos = 20;
  bool spi_slv_rx_seg_trans_clr_en = false;  uint8_t spi_slv_rx_seg_trans_clr_en_pos = 19;
  bool spi_dma_slv_seg_trans_en = false;     uint8_t spi_dma_slv_seg_trans_en_pos = 18;
  bool spi_dma_infifo_full = true;           uint8_t spi_dma_infifo_full_pos = 1;
  bool spi_dma_outfifo_empty = true;         uint8_t spi_dma_outfifo_empty_pos = 0;
}; 

/// SPI slave control register   varies
struct dbg_spi_reg_slave{
  uint32_t val = 0;
  uint32_t reset = 0x28000000;

  uint16_t base_2 = 0x00E0;
  uint16_t base_3 = 0x00E0;

  bool r_spi_usr_conf = false;  uint8_t r_spi_usr_conf_pos = 28;
  bool spi_soft_reset = false;   uint8_t spi_soft_reset_pos = 27;
  bool spi_slave_mode = false;  uint8_t spi_slave_mode_pos = 26;

  uint8_t r_spi_dma_seg_magic_value = 10;
    uint8_t r_spi_dma_seg_magic_value_y = 25;
    uint8_t r_spi_dma_seg_magic_value_x = 22;

  bool spi_slv_wrbuf_bitlen_en = false;  uint8_t spi_slv_wrbuf_bitlen_en_pos = 11;
  bool spi_slv_rdbuf_bitlen_en = false;  uint8_t spi_slv_rdbuf_bitlen_en_pos = 10;
  bool spi_slv_wrdma_bitlen_en = false;  uint8_t spi_slv_wrdma_bitlen_en_pos = 9;
  bool spi_slv_rddma_bitlen_en = false;  uint8_t spi_slv_rddma_bitlen_en_pos = 8;
  bool spi_rsck_data_out = false;        uint8_t spi_rsck_data_out_pos = 3;
  bool spi_clk_mode_13 = false;          uint8_t spi_clk_mode_13_pos = 2;

  uint8_t spi_clk_mode = 0;
    uint8_t spi_clk_mode_y = 1; 
    uint8_t spi_clk_mode_x = 0;
}; 

/// SPI slave control register 1   R/W/SS
struct dbg_spi_reg_slave1{
  uint32_t val = 0;
  uint32_t reset = 0x0;

  uint16_t base_2 = 0x00E4;
  uint16_t base_3 = 0x00E4;

  uint32_t spi_slv_last_addr = 0;
    uint8_t spi_slv_last_addr_y = 31;
    uint8_t spi_slv_last_addr_x = 26;

  uint32_t spi_slv_last_command = 0;
    uint8_t spi_slv_last_command_y = 25;
    uint8_t spi_slv_last_command_x = 18;

  uint32_t spi_slv_data_bitlen = 0;
    uint8_t spi_slv_data_bitlen_y = 17;
    uint8_t spi_slv_data_bitlen_x = 0;
}; 

/* 
 *Clock control registers
 * */
 
/// SPI clock control register   R/W
struct dbg_spi_reg_clock{
  uint32_t val = 0;
  uint32_t reset = 0x80003043;

  uint16_t base_2 = 0x000C;
  uint16_t base_3 = 0x000C;

  bool spi_clk_equ_sysclk = true;  uint8_t spi_clk_equ_sysclk_pos = 31;

  uint8_t spi_clkdiv_pre = 0;
    uint8_t spi_clkdiv_pre_y = 21;
    uint8_t spi_clkdiv_pre_x = 18;

  uint8_t spi_clkcnt_n = 0x3;
    uint8_t spi_clkcnt_n_y = 17;
    uint8_t spi_clkcnt_n_x = 12;

  uint8_t spi_clkcnt_h = 0x1;
    uint8_t spi_clkcnt_h_y = 11;
    uint8_t spi_clkcnt_h_x = 6;

  uint8_t spi_clkcnt_l = 0x3;
    uint8_t spi_clkcnt_l_y = 5;
    uint8_t spi_clkcnt_l_x = 0;
};

/// SPI module clock and register clock control   R/W
struct dbg_spi_reg_gate{
  uint32_t val = 0;
  uint32_t reset = 0x0;

  uint16_t base_2 = 0x00E8;
  uint16_t base_3 = 0x00E8;

  bool spi_mst_clk_sel = false;     uint8_t spi_mst_clk_sel_pos = 2;
  bool spi_mst_clk_active = false;  uint8_t spi_mst_clk_active_pos = 1;
  bool spi_clk_en = false;          uint8_t spi_clk_en_pos = 0;
};

/*
 * Timing registers
 * **/
/// SPI input delay mode configuration   R/W
struct dbg_spi_reg_din_mode{
  uint32_t val = 0;
  uint32_t reset = 0x0;

  uint16_t base_2 = 0x0024;
  uint16_t base_3 = 0x0024;

  bool spi_timing_hclk_active = false;   uint8_t spi_timing_hclk_active_pos = 16;

  uint8_t r_spi_din7_mode = 0;
    uint8_t r_spi_din7_mode_y = 15;
    uint8_t r_spi_din7_mode_x = 14;

  uint8_t r_spi_din6_mode = 0;
    uint8_t r_spi_din6_mode_y = 13;
    uint8_t r_spi_din6_mode_x = 12;

  uint8_t r_spi_din5_mode = 0;
    uint8_t r_spi_din5_mode_y = 11;
    uint8_t r_spi_din5_mode_x = 10;

  uint8_t r_spi_din4_mode = 0;
    uint8_t r_spi_din4_mode_y = 9;
    uint8_t r_spi_din4_mode_x = 8;

  uint8_t spi_din3_mode = 0;
    uint8_t spi_din3_mode_y = 7;
    uint8_t spi_din3_mode_x = 6;

  uint8_t spi_din2_mode = 0;
    uint8_t spi_din2_mode_y = 5;
    uint8_t spi_din2_mode_x = 4;

  uint8_t spi_din1_mode = 0;
    uint8_t spi_din1_mode_y = 3;
    uint8_t spi_din1_mode_x = 2;

  uint8_t spi_din0_mode = 0;
    uint8_t spi_din0_mode_y = 1;
    uint8_t spi_din0_mode_x = 0;
};

/// SPI input delay number configuration   R/W
struct dbg_spi_reg_din_num{
  uint32_t val = 0;
  uint32_t reset = 0x0;

  uint16_t base_2 = 0x0028;
  uint16_t base_3 = 0x0028;

  uint8_t r_spi_din7_num = 0;
    uint8_t r_spi_din7_num_y = 15;
    uint8_t r_spi_din7_num_x = 14;

  uint8_t r_spi_din6_num = 0;
    uint8_t r_spi_din6_num_y = 13;
    uint8_t r_spi_din6_num_x = 12;

  uint8_t r_spi_din5_num = 0;
    uint8_t r_spi_din5_num_y = 11;
    uint8_t r_spi_din5_num_x = 10;

  uint8_t r_spi_din4_num = 0;
    uint8_t r_spi_din4_num_y = 9;
    uint8_t r_spi_din4_num_x = 8;

  uint8_t spi_din3_num = 0;
    uint8_t spi_din3_num_y = 7;
    uint8_t spi_din3_num_x = 6;

  uint8_t spi_din2_num = 0;
    uint8_t spi_din2_num_y = 5;
    uint8_t spi_din2_num_x = 4;

  uint8_t spi_din1_num = 0;
    uint8_t spi_din1_num_y = 3;
    uint8_t spi_din1_num_x = 2;

  uint8_t spi_din0_num = 0;
    uint8_t spi_din0_num_y = 1;
    uint8_t spi_din0_num_x = 0;
};

/// SPI output delay mode configuration   R/W
struct dbg_spi_reg_dout_mode{
  uint32_t val = 0;
  uint32_t reset = 0x0;

  uint16_t base_2 = 0x002C;
  uint16_t base_3 = 0x002C;

  bool r_spi_d_dqs_mode = false;    uint8_t r_spi_d_dqs_mode_pos = 8;
  bool r_spi_dout7_mode = false;    uint8_t r_spi_dout7_mode_pos = 7;
  bool r_spi_dout6_mode = false;    uint8_t r_spi_dout6_mode_pos = 6;
  bool r_spi_dout5_mode = false;    uint8_t r_spi_dout5_mode_pos = 5;
  bool r_spi_dout4_mode = false;    uint8_t r_spi_dout4_mode_pos = 4;
  bool spi_dout3_mode = false;      uint8_t spi_dout3_mode_pos = 3;
  bool spi_dout2_mode = false;      uint8_t spi_dout2_mode_pos = 2;
  bool spi_dout1_mode = false;      uint8_t spi_dout1_mode_pos = 1;
  bool spi_dout0_mode = false;      uint8_t spi_dout0_mode_pos = 0;
};

/*
 * Interrupt registers
 * */

/// SPI interrupt enable register   R/W
struct dbg_spi_reg_dma_int_ena{
  uint32_t val = 0;
  uint32_t reset = 0x0;

  uint16_t base_2 = 0x0034;
  uint16_t base_3 = 0x0034;

  bool spi_app1_int_ena = false;                     uint8_t spi_app1_int_ena_pos = 20;
  bool spi_app2_int_ena = false;                     uint8_t spi_app2_int_ena_pos = 19;
  bool spi_mst_tx_afifo_rempty_err_int_ena = false;  uint8_t spi_mst_tx_afifo_rempty_err_int_ena_pos = 18;
  bool spi_mst_rx_afifo_wfull_err_int_ena = false;   uint8_t spi_mst_rx_afifo_wfull_err_int_ena_pos = 17;
  bool spi_slv_cmd_err_int_ena = false;              uint8_t spi_slv_cmd_err_int_ena_pos = 16;
  bool r_spi_seg_magic_err_int_ena = false;          uint8_t r_spi_seg_magic_err_int_ena_pos = 14;
  bool spi_dma_seg_trans_done_int_ena = false;       uint8_t spi_dma_seg_trans_done_int_ena_pos = 13;
  bool spi_trans_done_int_ena = false;               uint8_t spi_trans_done_int_ena_pos = 12;
  bool spi_slv_wr_buf_done_int_ena = false;          uint8_t spi_slv_wr_buf_done_int_ena_pos = 11;
  bool spi_slv_rd_buf_done_int_ena = false;          uint8_t spi_slv_rd_buf_done_int_ena_pos = 10;
  bool spi_slv_wr_dma_done_int_ena = false;          uint8_t spi_slv_wr_dma_done_int_ena_pos = 9;
  bool spi_slv_rd_dma_done_int_ena = false;          uint8_t spi_slv_rd_dma_done_int_ena_pos = 8;
  bool spi_slv_cmda_int_ena = false;                 uint8_t spi_slv_cmda_int_ena_pos = 7;
  bool spi_slv_cmd9_int_ena = false;                 uint8_t spi_slv_cmd9_int_ena_pos = 6;
  bool spi_slv_cmd8_int_ena = false;                 uint8_t spi_slv_cmd8_int_ena_pos = 5;
  bool spi_slv_cmd7_int_ena = false;                 uint8_t spi_slv_cmd7_int_ena_pos = 4;
  bool spi_slv_en_qpi_int_ena = false;               uint8_t spi_slv_en_qpi_int_ena_pos = 3;
  bool spi_slv_ex_qpi_int_ena = false;               uint8_t spi_slv_ex_qpi_int_ena_pos = 2;
  bool spi_dma_outfifo_empty_err_int_ena = false;    uint8_t spi_dma_outfifo_empty_err_int_ena_pos = 1;
  bool spi_dma_infifo_full_err_int_ena = false;      uint8_t spi_dma_infifo_full_err_int_ena_pos = 0;

};

/// SPI interrupt clear register   WT
struct dbg_spi_reg_dma_int_clr{
  uint32_t val = 0;
  uint32_t reset = 0x0;

  uint16_t base_2 = 0x0038;
  uint16_t base_3 = 0x0038;

  bool spi_app1_int_clr = false;                     uint8_t spi_app1_int_clr_pos = 20;
  bool spi_app2_int_clr = false;                     uint8_t spi_app2_int_clr_pos = 19;
  bool spi_mst_tx_afifo_rempty_err_int_clr = false;  uint8_t spi_mst_tx_afifo_rempty_err_int_clr_pos = 18;
  bool spi_mst_rx_afifo_wfull_err_int_clr = false;   uint8_t spi_mst_rx_afifo_wfull_err_int_clr_pos = 17;
  bool spi_slv_cmd_err_int_clr = false;              uint8_t spi_slv_cmd_err_int_clr_pos = 16;
  bool r_spi_seg_magic_err_int_clr = false;          uint8_t r_spi_seg_magic_err_int_clr_pos = 14;
  bool spi_dma_seg_trans_done_int_clr = false;       uint8_t spi_dma_seg_trans_done_int_clr_pos = 13;
  bool spi_trans_done_int_clr = false;               uint8_t spi_trans_done_int_clr_pos = 12;
  bool spi_slv_wr_buf_done_int_clr = false;          uint8_t spi_slv_wr_buf_done_int_clr_pos = 11;
  bool spi_slv_rd_buf_done_int_clr = false;          uint8_t spi_slv_rd_buf_done_int_clr_pos = 10;
  bool spi_slv_wr_dma_done_int_clr = false;          uint8_t spi_slv_wr_dma_done_int_clr_pos = 9;
  bool spi_slv_rd_dma_done_int_clr = false;          uint8_t spi_slv_rd_dma_done_int_clr_pos = 8;
  bool spi_slv_cmda_int_clr = false;                 uint8_t spi_slv_cmda_int_clr_pos = 7;
  bool spi_slv_cmd9_int_clr = false;                 uint8_t spi_slv_cmd9_int_clr_pos = 6;
  bool spi_slv_cmd8_int_clr = false;                 uint8_t spi_slv_cmd8_int_clr_pos = 5;
  bool spi_slv_cmd7_int_clr = false;                 uint8_t spi_slv_cmd7_int_clr_pos = 4;
  bool spi_slv_en_qpi_int_clr = false;               uint8_t spi_slv_en_qpi_int_clr_pos = 3;
  bool spi_slv_ex_qpi_int_clr = false;               uint8_t spi_slv_ex_qpi_int_clr_pos = 2;
  bool spi_dma_outfifo_empty_err_int_clr = false;    uint8_t spi_dma_outfifo_empty_err_int_clr_pos = 1;
  bool spi_dma_infifo_full_err_int_clr = false;      uint8_t spi_dma_infifo_full_err_int_clr_pos = 0;
};

/// SPI interrupt raw register   R/WTC/SS
struct dbg_spi_reg_dma_int_raw{
  uint32_t val = 0;
  uint32_t reset = 0x0;

  uint16_t base_2 = 0x003C;
  uint16_t base_3 = 0x003C;

  bool spi_app1_int_raw = false;                     uint8_t spi_app1_int_raw_pos = 20;
  bool spi_app2_int_raw = false;                     uint8_t spi_app2_int_raw_pos = 19;
  bool spi_mst_tx_afifo_rempty_err_int_raw = false;  uint8_t spi_mst_tx_afifo_rempty_err_int_raw_pos = 18;
  bool spi_mst_rx_afifo_wfull_err_int_raw = false;   uint8_t spi_mst_rx_afifo_wfull_err_int_raw_pos = 17;
  bool spi_slv_cmd_err_int_raw = false;              uint8_t spi_slv_cmd_err_int_raw_pos = 16;
  bool r_spi_seg_magic_err_int_raw = false;          uint8_t r_spi_seg_magic_err_int_raw_pos = 14;
  bool spi_dma_seg_trans_done_int_raw = false;       uint8_t spi_dma_seg_trans_done_int_raw_pos = 13;
  bool spi_trans_done_int_raw = false;               uint8_t spi_trans_done_int_raw_pos = 12;
  bool spi_slv_wr_buf_done_int_raw = false;          uint8_t spi_slv_wr_buf_done_int_raw_pos = 11;
  bool spi_slv_rd_buf_done_int_raw = false;          uint8_t spi_slv_rd_buf_done_int_raw_pos = 10;
  bool spi_slv_wr_dma_done_int_raw = false;          uint8_t spi_slv_wr_dma_done_int_raw_pos = 9;
  bool spi_slv_rd_dma_done_int_raw = false;          uint8_t spi_slv_rd_dma_done_int_raw_pos = 8;
  bool spi_slv_cmda_int_raw = false;                 uint8_t spi_slv_cmda_int_raw_pos = 7;
  bool spi_slv_cmd9_int_raw = false;                 uint8_t spi_slv_cmd9_int_raw_pos = 6;
  bool spi_slv_cmd8_int_raw = false;                 uint8_t spi_slv_cmd8_int_raw_pos = 5;
  bool spi_slv_cmd7_int_raw = false;                 uint8_t spi_slv_cmd7_int_raw_pos = 4;
  bool spi_slv_en_qpi_int_raw = false;               uint8_t spi_slv_en_qpi_int_raw_pos = 3;
  bool spi_slv_ex_qpi_int_raw = false;               uint8_t spi_slv_ex_qpi_int_raw_pos = 2;
  bool spi_dma_outfifo_empty_err_int_raw = false;    uint8_t spi_dma_outfifo_empty_err_int_raw_pos = 1;
  bool spi_dma_infifo_full_err_int_raw = false;      uint8_t spi_dma_infifo_full_err_int_raw_pos = 0;
};

/// SPI interrupt status register   RO
struct dbg_spi_reg_dma_int_st{
  uint32_t val = 0;
  uint32_t reset = 0x0;

  uint16_t base_2 = 0x0040;
  uint16_t base_3 = 0x0040;

  bool spi_app1_int_st = false;                     uint8_t spi_app1_int_st_pos = 20;
  bool spi_app2_int_st = false;                     uint8_t spi_app2_int_st_pos = 19;
  bool spi_mst_tx_afifo_rempty_err_int_st = false;  uint8_t spi_mst_tx_afifo_rempty_err_int_st_pos = 18;
  bool spi_mst_rx_afifo_wfull_err_int_st = false;   uint8_t spi_mst_rx_afifo_wfull_err_int_st_pos = 17;
  bool spi_slv_cmd_err_int_st = false;              uint8_t spi_slv_cmd_err_int_st_pos = 16;
  bool r_spi_seg_magic_err_int_st = false;          uint8_t r_spi_seg_magic_err_int_st_pos = 14;
  bool spi_dma_seg_trans_done_int_st = false;       uint8_t spi_dma_seg_trans_done_int_st_pos = 13;
  bool spi_trans_done_int_st = false;               uint8_t spi_trans_done_int_st_pos = 12;
  bool spi_slv_wr_buf_done_int_st = false;          uint8_t spi_slv_wr_buf_done_int_st_pos = 11;
  bool spi_slv_rd_buf_done_int_st = false;          uint8_t spi_slv_rd_buf_done_int_st_pos = 10;
  bool spi_slv_wr_dma_done_int_st = false;          uint8_t spi_slv_wr_dma_done_int_st_pos = 9;
  bool spi_slv_rd_dma_done_int_st = false;          uint8_t spi_slv_rd_dma_done_int_st_pos = 8;
  bool spi_slv_cmda_int_st = false;                 uint8_t spi_slv_cmda_int_st_pos = 7;
  bool spi_slv_cmd9_int_st = false;                 uint8_t spi_slv_cmd9_int_st_pos = 6;
  bool spi_slv_cmd8_int_st = false;                 uint8_t spi_slv_cmd8_int_st_pos = 5;
  bool spi_slv_cmd7_int_st = false;                 uint8_t spi_slv_cmd7_int_st_pos = 4;
  bool spi_slv_en_qpi_int_st = false;               uint8_t spi_slv_en_qpi_int_st_pos = 3;
  bool spi_slv_ex_qpi_int_st = false;               uint8_t spi_slv_ex_qpi_int_st_pos = 2;
  bool spi_dma_outfifo_empty_err_int_st = false;    uint8_t spi_dma_outfifo_empty_err_int_st_pos = 1;
  bool spi_dma_infifo_full_err_int_st = false;      uint8_t spi_dma_infifo_full_err_int_st_pos = 0;
};

/// SPI interrupt software set register   WT
struct dbg_spi_reg_dma_int_set{
  uint32_t val = 0;
  uint32_t reset = 0x0;

  uint16_t base_2 = 0x0044;
  uint16_t base_3 = 0x0044;

  bool spi_app1_int_set = false;                     uint8_t spi_app1_int_set_pos = 20;
  bool spi_app2_int_set = false;                     uint8_t spi_app2_int_set_pos = 19;
  bool spi_mst_tx_afifo_rempty_err_int_set = false;  uint8_t spi_mst_tx_afifo_rempty_err_int_set_pos = 18;
  bool spi_mst_rx_afifo_wfull_err_int_set = false;   uint8_t spi_mst_rx_afifo_wfull_err_int_set_pos = 17;
  bool spi_slv_cmd_err_int_set = false;              uint8_t spi_slv_cmd_err_int_set_pos = 16;
  bool r_spi_seg_magic_err_int_set = false;          uint8_t r_spi_seg_magic_err_int_set_pos = 14;
  bool spi_dma_seg_trans_done_int_set = false;       uint8_t spi_dma_seg_trans_done_int_set_pos = 13;
  bool spi_trans_done_int_set = false;               uint8_t spi_trans_done_int_set_pos = 12;
  bool spi_slv_wr_buf_done_int_set = false;          uint8_t spi_slv_wr_buf_done_int_set_pos = 11;
  bool spi_slv_rd_buf_done_int_set = false;          uint8_t spi_slv_rd_buf_done_int_set_pos = 10;
  bool spi_slv_wr_dma_done_int_set = false;          uint8_t spi_slv_wr_dma_done_int_set_pos = 9;
  bool spi_slv_rd_dma_done_int_set = false;          uint8_t spi_slv_rd_dma_done_int_set_pos = 8;
  bool spi_slv_cmda_int_set = false;                 uint8_t spi_slv_cmda_int_set_pos = 7;
  bool spi_slv_cmd9_int_set = false;                 uint8_t spi_slv_cmd9_int_set_pos = 6;
  bool spi_slv_cmd8_int_set = false;                 uint8_t spi_slv_cmd8_int_set_pos = 5;
  bool spi_slv_cmd7_int_set = false;                 uint8_t spi_slv_cmd7_int_set_pos = 4;
  bool spi_slv_en_qpi_int_set = false;               uint8_t spi_slv_en_qpi_int_set_pos = 3;
  bool spi_slv_ex_qpi_int_set = false;               uint8_t spi_slv_ex_qpi_int_set_pos = 2;
  bool spi_dma_outfifo_empty_err_int_set = false;    uint8_t spi_dma_outfifo_empty_err_int_set_pos = 1;
  bool spi_dma_infifo_full_err_int_set = false;      uint8_t spi_dma_infifo_full_err_int_set_pos = 0;
};
