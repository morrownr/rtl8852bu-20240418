/******************************************************************************
 *
 * Copyright(c) 2019 Realtek Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 *****************************************************************************/
#ifndef _HALBB_RA_L_ENDIAN_H_
#define _HALBB_RA_L_ENDIAN_H_
/*@--------------------------[Define] ---------------------------------------*/



/*@--------------------------[Enum]------------------------------------------*/



/*@--------------------------[Structure]-------------------------------------*/

struct bb_fw_cmac_rpt_info {
	/* dword 0 */
	u32 rpt_sel: 5;
	u32 polluted: 1;
	u32 tx_state: 2;
	u32 sw_define: 4;
	u32 rsvd0: 2;
	u32 try_rate: 1;
	u32 fix_rate: 1;
	u32 macid: 7;
	u32 rsvd1: 1;
	u32 qsel: 6;
	u32 rsvd2: 1;
	u32 txop_start: 1;
	/* dword 1 */
	u32 queue_time: 16;
	u32 acc_tx_time: 8;
	u32 rsvd3: 5;
	u32 bmc: 1;
	u32 bitmap_short: 2;
	/* dword 2 */
	u32 final_rate: 9;
	u32 final_gi_ltf: 3;
	u32 data_bw: 2;
	u32 mu2su: 1;
	u32 mu_lmt: 1;
	u32 final_rts_rate: 9;
	u32 final_rts_gi_ltf: 3;
	u32 rts_tx_state: 2;
	u32 collision_head: 1;
	u32 collision_tail: 1;
	/* dword 3 */
	u32 total_pkt_num: 8;
	u32 data_tx_cnt: 6;
	u32 bpri: 1;
	u32 bbar: 1;
	u32 pkt_ok_num: 8;
	u32 rts_tx_count: 6;
	u32 rsvd4: 2;
	/* dword 4 */
	u32 init_rate: 9;
	u32 init_gi_ltf: 3;
	u32 ppdu_type: 2;
	u32 he_tb_ppdu_flag: 1;
	u32 ppdu_fst_rpt: 1;
	u32 su_txpwr: 6;
	u32 rsvd5: 2;
	u32 diff_pkt_num: 4;
	u32 user_define_ext_l: 4;
	/* dword 5 */
	u32 user_define: 8;
	u32 fw_define: 8;
	u32 txpwr_pd: 5;
	u32 bsr: 1;
	u32 rsvd6: 2;
	u32 sr_rx_count: 4;
	u32 user_define_ext_h: 4;
};

struct bb_h2c_ra_cfg_info {
	u8 is_dis_ra:1;
	u8 mode_ctrl:5;
	/*
	@ Bit0 : CCK
	@ Bit1 : OFDM
	@ Bit2 : HT
	@ Bit3 : VHT
	@ Bit4 : HE
	*/
	u8 bw_cap:2;

	u8 macid;

	u8 dcm_cap:1;
	u8 er_cap:1;
	u8 init_rate_lv:2;
	u8 upd_all:1;
	u8 en_sgi:1;
	u8 ldpc_cap:1;
	u8 stbc_cap:1;

	u8 ss_num:3;
	u8 giltf_cap:3;
	u8 upd_bw_nss_mask:1;
	u8 upd_mask:1;

	u8 ramask[8]; /* ramask[7] bit 7 is for indicate bfee csi rate ctrl */

	/* BFee CSI rate ctrl */
	u8 band_num;

	u8 ra_csi_rate_en:1;
	u8 fixed_csi_rate_en:1;
	u8 cr_tbl_sel:1;
	u8 fix_giltf_en:1;
	u8 fix_giltf:3;
	u8 partial_bw_su_er:1;

	u8 fixed_csi_rate_l;

	u8 is_noisy:1; /*nhm_ratio >= 1% then disable ra bw switch for 92XB, WLANBB-2227*/
	u8 rsvd0:4;
	u8 band:2;
	u8 is_new_bb_ra_dbgreg:1;
};

struct bb_h2c_rssi_setting {
	u8 macid;
	u8 rssi_a; /* BIT(7) : parse rssi_b*/
	u8 bcn_rssi_a; /* BIT(7) : parse bcn_rssi*/
	u8 bcn_rssi_b;

	u8 dtp_lv: 2;
	u8 rsvd1: 6;

	u8 is_fixed_rate:1;
	u8 fixed_rate:6;
	u8 fixed_is_mu:1;

	u8 fixed_rate_md:2;
	u8 fixed_giltf:3;
	u8 fixed_bw:2;
	u8 rsvd2_M:1;

	u8 rssi_b:7;
	u8 endcmd:1;
};

struct bb_h2c_ra_cfg_info_wifi7 {
	struct bb_h2c_ra_cfg_info bb_h2c_ra_cmn;

	u8 mode_ctrl_eht:7;
	u8 rsvd0:1;

	u8 bw_cap_eht:3;
	u8 rsvd1:5;

	u8 ramask[4];
};

struct bb_h2c_rssi_setting_wifi7 {
	u8 macid;
	u8 rssi_a; /* BIT(7) : parse rssi_b*/
	u8 bcn_rssi_a; /* BIT(7) : parse bcn_rssi*/
	u8 bcn_rssi_b;

	u8 fixed_rate_M:1;
	u8 fixed_bw_M:1;
	u8 fixed_rate_md_M:1;
	u8 rsvd0_M:5;

	u8 is_fixed_rate:1;
	u8 fixed_rate:7;

	u8 fixed_rate_md:2;
	u8 fixed_giltf:3;
	u8 fixed_bw:2;
	u8 fixed_is_mu:1;

	u8 rssi_b:7;
	u8 endcmd:1;
};

struct bb_h2c_ra_mask {
	u8 macid;

	u8 mask_or_reveal:1;
	u8 mask_rate:7;

	u8 mask_rate_md:2;
	u8 is_manual_adjust_ra_mask:1;
	u8 rsvd1:5;

	u8 rsvd2;
};

struct bb_h2c_ra_adjust {
	u8 macid;

	u8 drv_shift_value:7;
	u8 drv_shift_en:1;

	u8 rsvd[2];
};

struct bb_h2c_ra_d_o_timer {
	u8 macid;

	u8 d_o_timer_value:7;
	u8 d_o_timer_en:1;

	u8 rsvd[2];
};

struct bb_h2c_ra_shift_dafc_tc {
	u8 enable;

	u8 init_fb_cnt[24]; /*1ss MCS0 ~ 2ss MCS11*/

	u8 rsvd[3];
};

struct bb_h2c_mu_cfg {
	u8 cmd_type;
	u8 entry;
	u8 macid;
	u8 en_256q:1;
	u8 en_1024q:1;
	u8 rsvd3:6;
};

struct bb_h2c_ra_tx_hist_info {
	u16 macid;
	u8 per_ppdu;
	u8 rsvd;
};

#define TX_RATE_HIST_NUM (12 + 28) // Legacy rate + 2SS NUM_EHT_RATE

struct bb_ra_tx_hist_c2h_rpt {
    u32 ra_tbtt_cnt;
    u32 tx_rate_tot_cnt_hist[TX_RATE_HIST_NUM];
};

struct bb_h2c_ra_tx_info {
	u16 macid;
	u16 rsvd0;
};

struct halbb_c2h_dbg_rpt_wifi7 {
	u8 per;
	u8 rdr;
	u8 r4;
	u8 cls;
	u8 rate_up_lmt_cnt;
	u8 per_ma;
	u8 var;
	u8 d_o_n;
	u8 d_o_p;
	u8 rd_th;
	u8 ru_th;
	u8 try_per;
	u8 try_rdr;
	u8 try_r4;
	u8 txrpt_tot;
	u8 ra_timer;
	u8 tot_disra_trying_return;
	u8 r4_return;
	u8 highest_rate;
	u8 lowest_rate;
	u32 macid:16;
	u32 rsvd0:16;
	u32 cmac_tbl;
	u32 ra_mask_h;
	u32 ra_mask_l;
	u32 upd_all_h2c_0;
	u32 upd_all_h2c_1;
	u32 upd_all_h2c_2;
	u32 upd_all_h2c_3;
};

struct halbb_ra_rpt_info {
	u8 rpt_macid_l;
	u8 rpt_macid_m;

	u8 retry_ratio;

	u8 u0_muidx: 3;
	u8 u1_muidx: 3;
	u8 en_stbc: 1;
	u8 rpt_mcs_nss_M: 1;

	u8 rpt_mcs_nss: 7;
	u8 is_mu: 1;

	u8 rpt_md_sel: 2;
	u8 rpt_gi_ltf: 3;
	u8 rpt_bw: 2;
	u8 rpt_md_sel_M: 1;

	u8 rpt_bw_M: 1;
	u8 rsvd0: 7;

	u8 rsvd1;
};

struct halbb_txsts_info {
	u8 rpt_macid_l;
	u8 rpt_macid_m;

	u8 avg_agg;
	u8 rsvd0;

	u8 tx_ok_be_l;
	u8 tx_ok_be_m;
	u8 tx_ok_bk_l;
	u8 tx_ok_bk_m;
	u8 tx_ok_vi_l;
	u8 tx_ok_vi_m;
	u8 tx_ok_vo_l;
	u8 tx_ok_vo_m;

	u8 tx_retry_be_l;
	u8 tx_retry_be_m;
	u8 tx_retry_bk_l;
	u8 tx_retry_bk_m;
	u8 tx_retry_vi_l;
	u8 tx_retry_vi_m;
	u8 tx_retry_vo_l;
	u8 tx_retry_vo_m;

	u8 tx_rate_l;
	u8 tx_rate_m;
	u8 retry_ratio;
	u8 rsvd1;

	u8 tx_total_l;
	u8 tx_total_m;
	u8 rsvd2;
	u8 rsvd3;

	u8 rsvd4;
	u8 rsvd5;
	u8 rsvd6;
	u8 rsvd7;
};
/*@--------------------------[Prptotype]-------------------------------------*/

#endif
