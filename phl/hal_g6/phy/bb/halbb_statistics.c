/******************************************************************************
 *
 * Copyright(c) 2007 - 2020  Realtek Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 * wlanfae <wlanfae@realtek.com>
 * Realtek Corporation, No. 2, Innovation Road II, Hsinchu Science Park,
 * Hsinchu 300, Taiwan.
 *
 * Larry Finger <Larry.Finger@lwfinger.net>
 *
 *****************************************************************************/
#include "halbb_precomp.h"

#ifdef HALBB_STATISTICS_SUPPORT

void halbb_set_crc32_cnt2_rate(struct bb_info *bb, u16 rate_idx)
{
	struct bb_stat_info *stat_t = &bb->bb_stat_i;
	struct bb_usr_set_info *usr_set = &stat_t->bb_usr_set_i;
	struct bb_stat_cr_info *cr = &bb->bb_cmn_hooker->bb_stat_cr_i;
	bool is_ofdm_rate = halbb_is_ofdm_rate(bb, rate_idx);
	bool is_ht_rate = halbb_is_ht_rate(bb, rate_idx);
	bool is_vht_rate = halbb_is_vht_rate(bb, rate_idx);
	bool is_he_rate = halbb_is_he_rate(bb, rate_idx);
	u32 reg_addr = cr->intf_r_rate;
	u32 ofdm_rate_bitmask = cr->intf_r_rate_m;
	u32 ht_mcs_bitmask = cr->intf_r_mcs_m;
	u32 vht_mcs_bitmask = cr->intf_r_vht_mcs_m;
	u32 vht_ss_bitmask = cr->intf_r_vht_nss_m;
	u32 he_mcs_bitmask =cr->intf_r_he_mcs_m;
	u32 he_ss_bitmask = cr->intf_r_he_nss_m;
#ifdef BB_1115_DVLP_SPF
	bool is_eht_rate = halbb_is_eht_rate_wifi7(bb, rate_idx);
	u32 eht_mcs_bitmask = cr->intf_r_eht_mcs_m;
	u32 eht_ss_bitmask = cr->intf_r_eht_nss_m;
#endif
	u8 rate_digi = 0x0;
	u8 ss = 0x0;

	if (!is_ofdm_rate && !is_ht_rate && !is_vht_rate && !is_he_rate)
		BB_DBG(bb, DBG_FA_CNT,
			  "[STAT] rate_idx = (0x%x) Not support !!\n",
			  rate_idx);

	if (is_ofdm_rate) {
		rate_digi = halbb_legacy_rate_2_spec_rate(bb, rate_idx);
		halbb_set_reg(bb, reg_addr, ofdm_rate_bitmask, rate_digi);
		usr_set->ofdm2_rate_idx = rate_idx;
	}
	if (is_ht_rate) {
		rate_digi = halbb_rate_2_rate_digit(bb, rate_idx);
		halbb_set_reg(bb, reg_addr, ht_mcs_bitmask, rate_digi);
		usr_set->ht2_rate_idx = rate_idx;
	}
	if (is_vht_rate) {
		rate_digi = halbb_rate_2_rate_digit(bb, rate_idx);
		ss = halbb_rate_to_num_ss(bb, rate_idx);
		halbb_set_reg(bb, reg_addr, vht_mcs_bitmask, rate_digi);
		halbb_set_reg(bb, reg_addr, vht_ss_bitmask, ss - 1);
		usr_set->vht2_rate_idx = rate_idx;
	}
	if (is_he_rate) {
		rate_digi = halbb_rate_2_rate_digit(bb, rate_idx);
		ss = halbb_rate_to_num_ss(bb, rate_idx);
		halbb_set_reg(bb, reg_addr, he_mcs_bitmask, rate_digi);
		halbb_set_reg(bb, reg_addr, he_ss_bitmask, ss - 1);
		usr_set->he2_rate_idx = rate_idx;
	}

#ifdef BB_1115_DVLP_SPF
	if (is_eht_rate) {
		rate_digi = halbb_rate_2_rate_digit(bb, rate_idx);
		ss = halbb_rate_to_num_ss(bb, rate_idx);
		halbb_set_reg(bb, reg_addr, eht_mcs_bitmask, rate_digi);
		halbb_set_reg(bb, reg_addr, eht_ss_bitmask, ss - 1);
		usr_set->eht2_rate_idx = rate_idx;
	}
#endif
}

void halbb_set_crc32_cnt3_format(struct bb_info *bb, u8 usr_type_sel)
{
	struct bb_stat_info *stat_t = &bb->bb_stat_i;
	struct bb_fa_info *fa = &stat_t->bb_fa_i;
	struct bb_cca_info *cca = &stat_t->bb_cca_i;
	struct bb_crc_info *crc = &stat_t->bb_crc_i;
	struct bb_crc2_info *crc2 = &stat_t->bb_crc2_i;
	struct bb_usr_set_info *usr_set = &stat_t->bb_usr_set_i;
	struct bb_stat_cr_info *cr = &bb->bb_cmn_hooker->bb_stat_cr_i;
	u32 reg_addr = cr->intf_r_mac_hdr_type;
	u32 type_bitmask = cr->intf_r_mac_hdr_type_m;
	
	usr_set->stat_type_sel_i = usr_type_sel;
	usr_set->stat_mac_type_i = TYPE_DATA;

	switch(usr_set->stat_type_sel_i) {
		case STATE_PROBE_RESP:
			usr_set->stat_mac_type_i = TYPE_PROBE_RESP;
			break;
		case STATE_BEACON:
			usr_set->stat_mac_type_i = TYPE_BEACON;
			break;
		case STATE_ACTION:
			usr_set->stat_mac_type_i = TYPE_ACTION;
			break;
		case STATE_BFRP:
			usr_set->stat_mac_type_i = TYPE_BFRP;
			break;
		case STATE_NDPA:
			usr_set->stat_mac_type_i = TYPE_NDPA;
			break;
		case STATE_BA:
			usr_set->stat_mac_type_i = TYPE_BA;
			break;
		case STATE_RTS:
			usr_set->stat_mac_type_i = TYPE_RTS;
			break;
		case STATE_CTS:
			usr_set->stat_mac_type_i = TYPE_CTS;
			break;
		case STATE_ACK:
			usr_set->stat_mac_type_i = TYPE_ACK;
			break;
		case STATE_DATA:
			usr_set->stat_mac_type_i = TYPE_DATA;
			break;
		case STATE_NULL:
			usr_set->stat_mac_type_i = TYPE_NULL;
			break;
		case STATE_QOS:
			usr_set->stat_mac_type_i = TYPE_QOS;
			break;
		default:
			BB_DBG(bb, DBG_FA_CNT,
  			  "[STAT] MAC frame type cnt: Not support !!!\n");
			BB_DBG(bb, DBG_FA_CNT,
			  "[STAT] Please choose one of the following options\n");
			BB_DBG(bb, DBG_FA_CNT,
			  "[STAT] {1: Probe Request}\n");
			BB_DBG(bb, DBG_FA_CNT,
			  "[STAT] {2: Beacon}\n");
			BB_DBG(bb, DBG_FA_CNT,
			  "[STAT] {3: Action}\n");
			BB_DBG(bb, DBG_FA_CNT,
			  "[STAT] {4: Beamforming Report Poll}\n");
			BB_DBG(bb, DBG_FA_CNT,
			  "[STAT] {5: NDPA}\n");
			BB_DBG(bb, DBG_FA_CNT,
			  "[STAT] {6: BA}\n");
			BB_DBG(bb, DBG_FA_CNT,
			  "[STAT] {7: RTS}\n");
			BB_DBG(bb, DBG_FA_CNT,
			  "[STAT] {8: CTS}\n");
			BB_DBG(bb, DBG_FA_CNT,
			  "[STAT] {9: ACK}\n");
			BB_DBG(bb, DBG_FA_CNT,
			  "[STAT] {10: Data}\n");
			BB_DBG(bb, DBG_FA_CNT,
			  "[STAT] {11: Null}\n");
			BB_DBG(bb, DBG_FA_CNT,
			  "[STAT] {12: QoS Data}\n");
			break;
	}
	halbb_set_reg(bb, reg_addr, type_bitmask, usr_set->stat_mac_type_i);
}

void halbb_crc32_cnt_dbg(struct bb_info *bb, char input[][16], u32 *_used,
			 char *output, u32 *_out_len)
{
	struct bb_stat_info *stat_t = &bb->bb_stat_i;
	char help[] = "-h";
	u32 var[10] = {0};
	u8 i = 0;
	u16 rate = 0x0;
	u8 usr_type_sel = 0;
	enum phl_phy_idx bkp_phy_idx = bb->bb_phy_idx;

	if (_os_strcmp(input[1], help) == 0) {
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			    "show {phy_idx}\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			    "rst_en {en}\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "Get CRC_OK/error for specific rate_idx or mac hdr type\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "============== Specific rate cnt ==============\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "stat {1} {rate_idx in decimal}\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "---------- Specific MAC header type ----------\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "stat {2} {1: Probe Request}\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "stat {2} {2: Beacon}\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "stat {2} {3: Action}\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "stat {2} {4: Beamforming Report Poll}\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "stat {2} {5: NDPA}\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "stat {2} {6: BA}\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "stat {2} {7: RTS}\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "stat {2} {8: CTS}\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "stat {2} {9: ACK}\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "stat {2} {10: Data}\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "stat {2} {11: Null}\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "stat {2} {12: QoS Data}\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "============== Notes ==============\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "------------ Rate_idx ------------\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "CCK_idx: 0~3(Do not set!)\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "OFDM_idx: 4~11\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "HT_idx: 128~\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "VHT_1ss_idx: 256~265\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "VHT_2ss_idx: 272~281\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "HE_1ss_idx: 384~395\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "HE_2ss_idx: 400~411\n");
	} else if (_os_strcmp(input[1], "show") == 0) {
		HALBB_SCAN(input[2], DCMD_DECIMAL, &var[0]);

		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			    "[Get PMAC Counter] phy_idx = %d\n", bb->bb_phy_idx);

		if ((bb->dbg_component & DBG_FA_CNT) == 0)
			bb->dbg_component |= DBG_FA_CNT;

		halbb_pmac_statistics_io_en(bb);
	} else if (_os_strcmp(input[1], "rst_en") == 0) {
		HALBB_SCAN(input[2], DCMD_DECIMAL, &var[1]);
		stat_t->cnt_reset_en = (bool)var[1];
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			    "PMAC Rpt cnt reset en=%d\n", stat_t->cnt_reset_en);
	} else {
		HALBB_SCAN(input[1], DCMD_DECIMAL, &var[0]);

		if (var[0] == 1) {
			HALBB_SCAN(input[2], DCMD_DECIMAL, &var[1]);
			rate = (u16)var[1];
			BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
				   "{rate}={0x%x}", rate);
			halbb_set_crc32_cnt2_rate(bb, rate);
		} else if (var[0] == 2) {
			HALBB_SCAN(input[2], DCMD_DECIMAL, &var[1]);
			usr_type_sel = (u8)var[1];
			BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
				   "{MAC header type}={%d}", usr_type_sel);
			halbb_set_crc32_cnt3_format(bb, usr_type_sel);
		} 
	}	
}

void halbb_statistics_reset(struct bb_info *bb)
{
	struct bb_stat_info *stat_t = &bb->bb_stat_i;
	struct bb_fa_info *fa = &stat_t->bb_fa_i;
	struct bb_cca_info *cca = &stat_t->bb_cca_i;
	struct bb_crc_info *crc = &stat_t->bb_crc_i;
	struct bb_crc2_info *crc2 = &stat_t->bb_crc2_i;
	struct bb_tx_cnt_info *tx = &stat_t->bb_tx_cnt_i;
	
	/* @reset sw mem */
	halbb_mem_set(bb, tx, 0, sizeof(struct bb_tx_cnt_info));
	halbb_mem_set(bb, fa, 0, sizeof(struct bb_fa_info));
	halbb_mem_set(bb, cca, 0, sizeof(struct bb_cca_info));
	halbb_mem_set(bb, crc, 0, sizeof(struct bb_crc_info));
	halbb_mem_set(bb, crc2, 0, sizeof(struct bb_crc2_info));
}

void halbb_statistics_init(struct bb_info *bb)
{
	struct bb_stat_info *stat_t = &bb->bb_stat_i;
	struct bb_stat_hang_info *hang = &bb->bb_stat_i.bb_stat_hang_i;

	hang->consecutive_no_tx_cnt = 0;
	hang->consecutive_no_rx_cnt = 0;
	hang->hang_occur = false;

	stat_t->cnt_reset_en = true;
	halbb_statistics_reset(bb);
	halbb_set_crc32_cnt2_rate(bb, BB_06M);

	if (bb->ic_type & BB_IC_BE_SERIES) {
		halbb_set_crc32_cnt2_rate(bb, BE_BB_HT_MCS0);
		halbb_set_crc32_cnt2_rate(bb, BE_BB_VHT_1SS_MCS0);
		halbb_set_crc32_cnt2_rate(bb, BE_BB_HE_1SS_MCS0);
		halbb_set_crc32_cnt2_rate(bb, BE_BB_EHT_1SS_MCS0);
	} else {
		halbb_set_crc32_cnt2_rate(bb, BB_HT_MCS0);
		halbb_set_crc32_cnt2_rate(bb, BB_VHT_1SS_MCS0);
		halbb_set_crc32_cnt2_rate(bb, BB_HE_1SS_MCS0);
	}
	halbb_set_crc32_cnt3_format(bb, STATE_BEACON);
}

void halbb_cr_cfg_stat_init(struct bb_info *bb)
{
	struct bb_stat_cr_info *cr = &bb->bb_cmn_hooker->bb_stat_cr_i;

	switch (bb->cr_type) {

	#ifdef HALBB_COMPILE_AP_SERIES
	case BB_AP:
		cr->cck_cca = CNT_CCK_CCA_P0_A;
		cr->cck_cca_m = CNT_CCK_CCA_P0_A_M;
		cr->cck_crc16fail = CNT_CCK_CRC16FAIL_P0_A;
		cr->cck_crc16fail_m = CNT_CCK_CRC16FAIL_P0_A_M;
		cr->cck_crc32ok = CNT_CCK_CRC32OK_P0_A;
		cr->cck_crc32ok_m = CNT_CCK_CRC32OK_P0_A_M;
		cr->cck_crc32fail = CNT_CCK_CRC32FAIL_P0_A;
		cr->cck_crc32fail_m = CNT_CCK_CRC32FAIL_P0_A_M;
		cr->cca_spoofing = CNT_CCA_SPOOFING_A;
		cr->cca_spoofing_m = CNT_CCA_SPOOFING_A_M;
		cr->lsig_brk_s_th = CNT_LSIG_BRK_S_TH_A;
		cr->lsig_brk_s_th_m = CNT_LSIG_BRK_S_TH_A_M;
		cr->lsig_brk_l_th = CNT_LSIG_BRK_L_TH_A;
		cr->lsig_brk_l_th_m = CNT_LSIG_BRK_L_TH_A_M;
		cr->htsig_crc8_err_s_th = CNT_HTSIG_CRC8_ERR_S_TH_A;
		cr->htsig_crc8_err_s_th_m = CNT_HTSIG_CRC8_ERR_S_TH_A_M;
		cr->htsig_crc8_err_l_th = CNT_HTSIG_CRC8_ERR_L_TH_A;
		cr->htsig_crc8_err_l_th_m = CNT_HTSIG_CRC8_ERR_L_TH_A_M;
		cr->brk = CNT_BRK_A;
		cr->brk_m = CNT_BRK_A_M;
		cr->brk_sel = CNT_BRK_SEL_A;
		cr->brk_sel_m = CNT_BRK_SEL_A_M;
		cr->rxl_err_parity = CNT_RXL_ERR_PARITY_A;
		cr->rxl_err_parity_m = CNT_RXL_ERR_PARITY_A_M;
		cr->rxl_err_rate = CNT_RXL_ERR_RATE_A;
		cr->rxl_err_rate_m = CNT_RXL_ERR_RATE_A_M;
		cr->ht_err_crc8 = CNT_HT_ERR_CRC8_A;
		cr->ht_err_crc8_m = CNT_HT_ERR_CRC8_A_M;
		cr->vht_err_siga_crc8 = CNT_VHT_ERR_SIGA_CRC8_A;
		cr->vht_err_siga_crc8_m = CNT_VHT_ERR_SIGA_CRC8_A_M;
		cr->ht_not_support_mcs = CNT_HT_NOT_SUPPORT_MCS_A;
		cr->ht_not_support_mcs_m = CNT_HT_NOT_SUPPORT_MCS_A_M;
		cr->vht_not_support_mcs = CNT_VHT_NOT_SUPPORT_MCS_A;
		cr->vht_not_support_mcs_m = CNT_VHT_NOT_SUPPORT_MCS_A_M;
		cr->err_during_bt_tx = CNT_ERR_DURING_BT_TX_A;
		cr->err_during_bt_tx_m = CNT_ERR_DURING_BT_TX_A_M;
		cr->err_during_bt_rx = CNT_ERR_DURING_BT_RX_A;
		cr->err_during_bt_rx_m = CNT_ERR_DURING_BT_RX_A_M;
		cr->edge_murx_nsts0 = CNT_EDGE_MURX_NSTS0_A;
		cr->edge_murx_nsts0_m = CNT_EDGE_MURX_NSTS0_A_M;
		cr->search_fail = CNT_SEARCH_FAIL_A;
		cr->search_fail_m = CNT_SEARCH_FAIL_A_M;
		cr->ofdm_cca = CNT_OFDM_CCA_A;
		cr->ofdm_cca_m = CNT_OFDM_CCA_A_M;
		cr->ofdm_cca_s20 = CNT_OFDM_CCA_S20_A;
		cr->ofdm_cca_s20_m = CNT_OFDM_CCA_S20_A_M;
		cr->ofdm_cca_s40 = CNT_OFDM_CCA_S40_A;
		cr->ofdm_cca_s40_m = CNT_OFDM_CCA_S40_A_M;
		cr->ofdm_cca_s80 = CNT_OFDM_CCA_S80_A;
		cr->ofdm_cca_s80_m = CNT_OFDM_CCA_S80_A_M;
		cr->ccktxon = CNT_CCKTXON_A;
		cr->ccktxon_m = CNT_CCKTXON_A_M;
		cr->ccktxen = CNT_CCKTXEN_A;
		cr->ccktxen_m = CNT_CCKTXEN_A_M;
		cr->ofdmtxon = CNT_OFDMTXON_A;
		cr->ofdmtxon_m = CNT_OFDMTXON_A_M;
		cr->ofdmtxen = CNT_OFDMTXEN_A;
		cr->ofdmtxen_m = CNT_OFDMTXEN_A_M;
		cr->drop_trig = CNT_DROP_TRIG_A;
		cr->drop_trig_m = CNT_DROP_TRIG_A_M;
		cr->pop_trig = CNT_POP_TRIG_A;
		cr->pop_trig_m = CNT_POP_TRIG_A_M;
		cr->tx_conflict = CNT_TX_CONFLICT_A;
		cr->tx_conflict_m = CNT_TX_CONFLICT_A_M;
		cr->wmac_rstb = CNT_WMAC_RSTB_A;
		cr->wmac_rstb_m = CNT_WMAC_RSTB_A_M;
		cr->en_tb_ppdu_fix_gain = CNT_EN_TB_PPDU_FIX_GAIN_A;
		cr->en_tb_ppdu_fix_gain_m = CNT_EN_TB_PPDU_FIX_GAIN_A_M;
		cr->en_tb_cca_pw_th = CNT_EN_TB_CCA_PW_TH_A;
		cr->en_tb_cca_pw_th_m = CNT_EN_TB_CCA_PW_TH_A_M;
		cr->he_crc_ok = CNT_HE_CRC_OK_A;
		cr->he_crc_ok_m = CNT_HE_CRC_OK_A_M;
		cr->he_crc_err = CNT_HE_CRC_ERR_A;
		cr->he_crc_err_m = CNT_HE_CRC_ERR_A_M;
		cr->vht_crc_ok = CNT_VHT_CRC_OK_A;
		cr->vht_crc_ok_m = CNT_VHT_CRC_OK_A_M;
		cr->vht_crc_err = CNT_VHT_CRC_ERR_A;
		cr->vht_crc_err_m = CNT_VHT_CRC_ERR_A_M;
		cr->ht_crc_ok = CNT_HT_CRC_OK_A;
		cr->ht_crc_ok_m = CNT_HT_CRC_OK_A_M;
		cr->ht_crc_err = CNT_HT_CRC_ERR_A;
		cr->ht_crc_err_m = CNT_HT_CRC_ERR_A_M;
		cr->l_crc_ok = CNT_L_CRC_OK_A;
		cr->l_crc_ok_m = CNT_L_CRC_OK_A_M;
		cr->l_crc_err = CNT_L_CRC_ERR_A;
		cr->l_crc_err_m = CNT_L_CRC_ERR_A_M;
		cr->he_crc_ok2 = CNT_HE_CRC_OK2_A;
		cr->he_crc_ok2_m = CNT_HE_CRC_OK2_A_M;
		cr->he_crc_err2 = CNT_HE_CRC_ERR2_A;
		cr->he_crc_err2_m = CNT_HE_CRC_ERR2_A_M;
		cr->vht_crc_ok2 = CNT_VHT_CRC_OK2_A;
		cr->vht_crc_ok2_m = CNT_VHT_CRC_OK2_A_M;
		cr->vht_crc_err2 = CNT_VHT_CRC_ERR2_A;
		cr->vht_crc_err2_m = CNT_VHT_CRC_ERR2_A_M;
		cr->ht_crc_ok2 = CNT_HT_CRC_OK2_A;
		cr->ht_crc_ok2_m = CNT_HT_CRC_OK2_A_M;
		cr->ht_crc_err2 = CNT_HT_CRC_ERR2_A;
		cr->ht_crc_err2_m = CNT_HT_CRC_ERR2_A_M;
		cr->l_crc_ok2 = CNT_L_CRC_OK2_A;
		cr->l_crc_ok2_m = CNT_L_CRC_OK2_A_M;
		cr->l_crc_err2 = CNT_L_CRC_ERR2_A;
		cr->l_crc_err2_m = CNT_L_CRC_ERR2_A_M;
		cr->l_crc_ok3 = CNT_L_CRC_OK3_A;
		cr->l_crc_ok3_m = CNT_L_CRC_OK3_A_M;
		cr->l_crc_err3 = CNT_L_CRC_ERR3_A;
		cr->l_crc_err3_m = CNT_L_CRC_ERR3_A_M;
		cr->ampdu_rxon = CNT_AMPDU_RXON_A;
		cr->ampdu_rxon_m = CNT_AMPDU_RXON_A_M;
		cr->ampdu_miss = CNT_AMPDU_MISS_A;
		cr->ampdu_miss_m = CNT_AMPDU_MISS_A_M;
		cr->ampdu_crc_ok = CNT_AMPDU_RX_CRC32_OK_A;
		cr->ampdu_crc_ok_m = CNT_AMPDU_RX_CRC32_OK_A_M;
		cr->ampdu_crc_err = CNT_AMPDU_RX_CRC32_ERR_A;
		cr->ampdu_crc_err_m = CNT_AMPDU_RX_CRC32_ERR_A_M;
		cr->hesu_err_sig_a_crc4 = CNT_HESU_ERR_SIG_A_CRC4_A;
		cr->hesu_err_sig_a_crc4_m = CNT_HESU_ERR_SIG_A_CRC4_A_M;
		cr->heersu_err_sig_a_crc4 = CNT_HEERSU_ERR_SIG_A_CRC4_A;
		cr->heersu_err_sig_a_crc4_m = CNT_HEERSU_ERR_SIG_A_CRC4_A_M;
		cr->hemu_err_sig_a_crc4 = CNT_HEMU_ERR_SIG_A_CRC4_A;
		cr->hemu_err_sig_a_crc4_m = CNT_HEMU_ERR_SIG_A_CRC4_A_M;
		cr->hemu_err_sigb_ch1_comm_crc4 = CNT_HEMU_ERR_SIGB_CH1_COMM_CRC4_A;
		cr->hemu_err_sigb_ch1_comm_crc4_m = CNT_HEMU_ERR_SIGB_CH1_COMM_CRC4_A_M;
		cr->hemu_err_sigb_ch2_comm_crc4 = CNT_HEMU_ERR_SIGB_CH2_COMM_CRC4_A;
		cr->hemu_err_sigb_ch2_comm_crc4_m = CNT_HEMU_ERR_SIGB_CH2_COMM_CRC4_A_M;
		cr->he_u0_err_bcc_mcs = CNT_HE_U0_ERR_BCC_MCS_A;
		cr->he_u0_err_bcc_mcs_m = CNT_HE_U0_ERR_BCC_MCS_A_M;
		cr->he_u0_err_mcs = CNT_HE_U0_ERR_MCS_A;
		cr->he_u0_err_mcs_m = CNT_HE_U0_ERR_MCS_A_M;
		cr->he_u0_err_dcm_mcs = CNT_HE_U0_ERR_DCM_MCS_A;
		cr->he_u0_err_dcm_mcs_m = CNT_HE_U0_ERR_DCM_MCS_A_M;
		cr->r1b_rx_rpt_rst = R1B_RX_RPT_RST_A;
		cr->r1b_rx_rpt_rst_m = R1B_RX_RPT_RST_A_M;
		cr->r1b_rr_sel = R1B_RR_SEL_A;
		cr->r1b_rr_sel_m = R1B_RR_SEL_A_M;
		cr->rst_all_cnt = RST_ALL_CNT_A;
		cr->rst_all_cnt_m = RST_ALL_CNT_A_M;
		cr->enable_all_cnt = ENABLE_ALL_CNT_A;
		cr->enable_all_cnt_m = ENABLE_ALL_CNT_A_M;
		cr->enable_ofdm = ENABLE_OFDM_A;
		cr->enable_ofdm_m = ENABLE_OFDM_A_M;
		cr->enable_cck = ENABLE_CCK_A;
		cr->enable_cck_m = ENABLE_CCK_A_M;
		cr->r1b_rx_dis_cca = R1B_RX_DIS_CCA_A;
		cr->r1b_rx_dis_cca_m = R1B_RX_DIS_CCA_A_M;
		cr->intf_r_rate = INTF_R_CNT_RATE_A;
		cr->intf_r_rate_m = INTF_R_CNT_RATE_A_M;
		cr->intf_r_mcs = INTF_R_CNT_MCS_A;
		cr->intf_r_mcs_m = INTF_R_CNT_MCS_A_M;
		cr->intf_r_vht_mcs = INTF_R_CNT_VHT_MCS_A;
		cr->intf_r_vht_mcs_m = INTF_R_CNT_VHT_MCS_A_M;
		cr->intf_r_he_mcs = INTF_R_CNT_HE_MCS_A;
		cr->intf_r_he_mcs_m = INTF_R_CNT_HE_MCS_A_M;
		cr->intf_r_vht_nss = INTF_R_CNT_VHT_NSS_A;
		cr->intf_r_vht_nss_m = INTF_R_CNT_VHT_NSS_A_M;
		cr->intf_r_he_nss = INTF_R_CNT_HE_NSS_A;
		cr->intf_r_he_nss_m = INTF_R_CNT_HE_NSS_A_M;
		cr->intf_r_mac_hdr_type = INTF_R_MAC_HDR_TYPE_A;
		cr->intf_r_mac_hdr_type_m = INTF_R_MAC_HDR_TYPE_A_M;
		cr->intf_r_pkt_type = INTF_R_PKT_TYPE_A;
		cr->intf_r_pkt_type_m = INTF_R_PKT_TYPE_A_M;
		cr->dbcc = DBCC_A;
		cr->dbcc_m = DBCC_A_M;
		cr->dbcc_2p4g_band_sel = DBCC_2P4G_BAND_SEL_A;
		cr->dbcc_2p4g_band_sel_m = DBCC_2P4G_BAND_SEL_A_M;
		cr->cnt_pop_trig = CNT_POP_TRIG_A;
		cr->cnt_pop_trig_m = CNT_POP_TRIG_A_M;
		cr->max_cnt_pop = MAX_CNT_POP_A;
		cr->max_cnt_pop_m = MAX_CNT_POP_A_M;
		break;

	#endif
	#ifdef HALBB_COMPILE_CLIENT_SERIES
	case BB_CLIENT:
		cr->cck_cca = CNT_CCK_CCA_P0_C;
		cr->cck_cca_m = CNT_CCK_CCA_P0_C_M;
		cr->cck_crc16fail = CNT_CCK_CRC16FAIL_P0_C;
		cr->cck_crc16fail_m = CNT_CCK_CRC16FAIL_P0_C_M;
		cr->cck_crc32ok = CNT_CCK_CRC32OK_P0_C;
		cr->cck_crc32ok_m = CNT_CCK_CRC32OK_P0_C_M;
		cr->cck_crc32fail = CNT_CCK_CRC32FAIL_P0_C;
		cr->cck_crc32fail_m = CNT_CCK_CRC32FAIL_P0_C_M;
		cr->cca_spoofing = CNT_CCA_SPOOFING_C;
		cr->cca_spoofing_m = CNT_CCA_SPOOFING_C_M;
		cr->lsig_brk_s_th = CNT_LSIG_BRK_S_TH_C;
		cr->lsig_brk_s_th_m = CNT_LSIG_BRK_S_TH_C_M;
		cr->lsig_brk_l_th = CNT_LSIG_BRK_L_TH_C;
		cr->lsig_brk_l_th_m = CNT_LSIG_BRK_L_TH_C_M;
		cr->htsig_crc8_err_s_th = CNT_HTSIG_CRC8_ERR_S_TH_C;
		cr->htsig_crc8_err_s_th_m = CNT_HTSIG_CRC8_ERR_S_TH_C_M;
		cr->htsig_crc8_err_l_th = CNT_HTSIG_CRC8_ERR_L_TH_C;
		cr->htsig_crc8_err_l_th_m = CNT_HTSIG_CRC8_ERR_L_TH_C_M;
		cr->brk = CNT_BRK_C;
		cr->brk_m = CNT_BRK_C_M;
		cr->brk_sel = CNT_BRK_SEL_C;
		cr->brk_sel_m = CNT_BRK_SEL_C_M;
		cr->rxl_err_parity = CNT_RXL_ERR_PARITY_C;
		cr->rxl_err_parity_m = CNT_RXL_ERR_PARITY_C_M;
		cr->rxl_err_rate = CNT_RXL_ERR_RATE_C;
		cr->rxl_err_rate_m = CNT_RXL_ERR_RATE_C_M;
		cr->ht_err_crc8 = CNT_HT_ERR_CRC8_C;
		cr->ht_err_crc8_m = CNT_HT_ERR_CRC8_C_M;
		cr->vht_err_siga_crc8 = CNT_VHT_ERR_SIGA_CRC8_C;
		cr->vht_err_siga_crc8_m = CNT_VHT_ERR_SIGA_CRC8_C_M;
		cr->ht_not_support_mcs = CNT_HT_NOT_SUPPORT_MCS_C;
		cr->ht_not_support_mcs_m = CNT_HT_NOT_SUPPORT_MCS_C_M;
		cr->vht_not_support_mcs = CNT_VHT_NOT_SUPPORT_MCS_C;
		cr->vht_not_support_mcs_m = CNT_VHT_NOT_SUPPORT_MCS_C_M;
		cr->err_during_bt_tx = CNT_ERR_DURING_BT_TX_C;
		cr->err_during_bt_tx_m = CNT_ERR_DURING_BT_TX_C_M;
		cr->err_during_bt_rx = CNT_ERR_DURING_BT_RX_C;
		cr->err_during_bt_rx_m = CNT_ERR_DURING_BT_RX_C_M;
		cr->edge_murx_nsts0 = CNT_EDGE_MURX_NSTS0_C;
		cr->edge_murx_nsts0_m = CNT_EDGE_MURX_NSTS0_C_M;
		cr->search_fail = CNT_SEARCH_FAIL_C;
		cr->search_fail_m = CNT_SEARCH_FAIL_C_M;
		cr->ofdm_cca = CNT_OFDM_CCA_C;
		cr->ofdm_cca_m = CNT_OFDM_CCA_C_M;
		cr->ofdm_cca_s20 = CNT_OFDM_CCA_S20_C;
		cr->ofdm_cca_s20_m = CNT_OFDM_CCA_S20_C_M;
		cr->ofdm_cca_s40 = CNT_OFDM_CCA_S40_C;
		cr->ofdm_cca_s40_m = CNT_OFDM_CCA_S40_C_M;
		cr->ofdm_cca_s80 = CNT_OFDM_CCA_S80_C;
		cr->ofdm_cca_s80_m = CNT_OFDM_CCA_S80_C_M;
		cr->ccktxon = CNT_CCKTXON_C;
		cr->ccktxon_m = CNT_CCKTXON_C_M;
		cr->ccktxen = CNT_CCKTXEN_C;
		cr->ccktxen_m = CNT_CCKTXEN_C_M;
		cr->ofdmtxon = CNT_OFDMTXON_C;
		cr->ofdmtxon_m = CNT_OFDMTXON_C_M;
		cr->ofdmtxen = CNT_OFDMTXEN_C;
		cr->ofdmtxen_m = CNT_OFDMTXEN_C_M;
		cr->drop_trig = CNT_DROP_TRIG_C;
		cr->drop_trig_m = CNT_DROP_TRIG_C_M;
		cr->pop_trig = CNT_POP_TRIG_C;
		cr->pop_trig_m = CNT_POP_TRIG_C_M;
		cr->tx_conflict = CNT_TX_CONFLICT_C;
		cr->tx_conflict_m = CNT_TX_CONFLICT_C_M;
		cr->wmac_rstb = CNT_WMAC_RSTB_C;
		cr->wmac_rstb_m = CNT_WMAC_RSTB_C_M;
		cr->en_tb_ppdu_fix_gain = CNT_EN_TB_PPDU_FIX_GAIN_C;
		cr->en_tb_ppdu_fix_gain_m = CNT_EN_TB_PPDU_FIX_GAIN_C_M;
		cr->en_tb_cca_pw_th = CNT_EN_TB_CCA_PW_TH_C;
		cr->en_tb_cca_pw_th_m = CNT_EN_TB_CCA_PW_TH_C_M;
		cr->he_crc_ok = CNT_HE_CRC_OK_C;
		cr->he_crc_ok_m = CNT_HE_CRC_OK_C_M;
		cr->he_crc_err = CNT_HE_CRC_ERR_C;
		cr->he_crc_err_m = CNT_HE_CRC_ERR_C_M;
		cr->vht_crc_ok = CNT_VHT_CRC_OK_C;
		cr->vht_crc_ok_m = CNT_VHT_CRC_OK_C_M;
		cr->vht_crc_err = CNT_VHT_CRC_ERR_C;
		cr->vht_crc_err_m = CNT_VHT_CRC_ERR_C_M;
		cr->ht_crc_ok = CNT_HT_CRC_OK_C;
		cr->ht_crc_ok_m = CNT_HT_CRC_OK_C_M;
		cr->ht_crc_err = CNT_HT_CRC_ERR_C;
		cr->ht_crc_err_m = CNT_HT_CRC_ERR_C_M;
		cr->l_crc_ok = CNT_L_CRC_OK_C;
		cr->l_crc_ok_m = CNT_L_CRC_OK_C_M;
		cr->l_crc_err = CNT_L_CRC_ERR_C;
		cr->l_crc_err_m = CNT_L_CRC_ERR_C_M;
		cr->he_crc_ok2 = CNT_HE_CRC_OK2_C;
		cr->he_crc_ok2_m = CNT_HE_CRC_OK2_C_M;
		cr->he_crc_err2 = CNT_HE_CRC_ERR2_C;
		cr->he_crc_err2_m = CNT_HE_CRC_ERR2_C_M;
		cr->vht_crc_ok2 = CNT_VHT_CRC_OK2_C;
		cr->vht_crc_ok2_m = CNT_VHT_CRC_OK2_C_M;
		cr->vht_crc_err2 = CNT_VHT_CRC_ERR2_C;
		cr->vht_crc_err2_m = CNT_VHT_CRC_ERR2_C_M;
		cr->ht_crc_ok2 = CNT_HT_CRC_OK2_C;
		cr->ht_crc_ok2_m = CNT_HT_CRC_OK2_C_M;
		cr->ht_crc_err2 = CNT_HT_CRC_ERR2_C;
		cr->ht_crc_err2_m = CNT_HT_CRC_ERR2_C_M;
		cr->l_crc_ok2 = CNT_L_CRC_OK2_C;
		cr->l_crc_ok2_m = CNT_L_CRC_OK2_C_M;
		cr->l_crc_err2 = CNT_L_CRC_ERR2_C;
		cr->l_crc_err2_m = CNT_L_CRC_ERR2_C_M;
		cr->l_crc_ok3 = CNT_L_CRC_OK3_C;
		cr->l_crc_ok3_m = CNT_L_CRC_OK3_C_M;
		cr->l_crc_err3 = CNT_L_CRC_ERR3_C;
		cr->l_crc_err3_m = CNT_L_CRC_ERR3_C_M;
		cr->ampdu_rxon = CNT_AMPDU_RXON_C;
		cr->ampdu_rxon_m = CNT_AMPDU_RXON_C_M;
		cr->ampdu_miss = CNT_AMPDU_MISS_C;
		cr->ampdu_miss_m = CNT_AMPDU_MISS_C_M;
		cr->ampdu_crc_ok = CNT_AMPDU_RX_CRC32_OK_C;
		cr->ampdu_crc_ok_m = CNT_AMPDU_RX_CRC32_OK_C_M;
		cr->ampdu_crc_err = CNT_AMPDU_RX_CRC32_ERR_C;
		cr->ampdu_crc_err_m = CNT_AMPDU_RX_CRC32_ERR_C_M;
		cr->hesu_err_sig_a_crc4 = CNT_HESU_ERR_SIG_A_CRC4_C;
		cr->hesu_err_sig_a_crc4_m = CNT_HESU_ERR_SIG_A_CRC4_C_M;
		cr->heersu_err_sig_a_crc4 = CNT_HEERSU_ERR_SIG_A_CRC4_C;
		cr->heersu_err_sig_a_crc4_m = CNT_HEERSU_ERR_SIG_A_CRC4_C_M;
		cr->hemu_err_sig_a_crc4 = CNT_HEMU_ERR_SIG_A_CRC4_C;
		cr->hemu_err_sig_a_crc4_m = CNT_HEMU_ERR_SIG_A_CRC4_C_M;
		cr->hemu_err_sigb_ch1_comm_crc4 = CNT_HEMU_ERR_SIGB_CH1_COMM_CRC4_C;
		cr->hemu_err_sigb_ch1_comm_crc4_m = CNT_HEMU_ERR_SIGB_CH1_COMM_CRC4_C_M;
		cr->hemu_err_sigb_ch2_comm_crc4 = CNT_HEMU_ERR_SIGB_CH2_COMM_CRC4_C;
		cr->hemu_err_sigb_ch2_comm_crc4_m = CNT_HEMU_ERR_SIGB_CH2_COMM_CRC4_C_M;
		cr->he_u0_err_bcc_mcs = CNT_HE_U0_ERR_BCC_MCS_C;
		cr->he_u0_err_bcc_mcs_m = CNT_HE_U0_ERR_BCC_MCS_C_M;
		cr->he_u0_err_mcs = CNT_HE_U0_ERR_MCS_C;
		cr->he_u0_err_mcs_m = CNT_HE_U0_ERR_MCS_C_M;
		cr->he_u0_err_dcm_mcs = CNT_HE_U0_ERR_DCM_MCS_C;
		cr->he_u0_err_dcm_mcs_m = CNT_HE_U0_ERR_DCM_MCS_C_M;
		cr->r1b_rx_rpt_rst = R1B_RX_RPT_RST_C;
		cr->r1b_rx_rpt_rst_m = R1B_RX_RPT_RST_C_M;
		cr->r1b_rr_sel = R1B_RR_SEL_C;
		cr->r1b_rr_sel_m = R1B_RR_SEL_C_M;
		cr->rst_all_cnt = RST_ALL_CNT_C;
		cr->rst_all_cnt_m = RST_ALL_CNT_C_M;
		cr->enable_all_cnt = ENABLE_ALL_CNT_C;
		cr->enable_all_cnt_m = ENABLE_ALL_CNT_C_M;
		cr->enable_ofdm = ENABLE_OFDM_C;
		cr->enable_ofdm_m = ENABLE_OFDM_C_M;
		cr->enable_cck = ENABLE_CCK_C;
		cr->enable_cck_m = ENABLE_CCK_C_M;
		cr->r1b_rx_dis_cca = R1B_RX_DIS_CCA_C;
		cr->r1b_rx_dis_cca_m = R1B_RX_DIS_CCA_C_M;
		cr->intf_r_rate = INTF_R_CNT_RATE_C;
		cr->intf_r_rate_m = INTF_R_CNT_RATE_C_M;
		cr->intf_r_mcs = INTF_R_CNT_MCS_C;
		cr->intf_r_mcs_m = INTF_R_CNT_MCS_C_M;
		cr->intf_r_vht_mcs = INTF_R_CNT_VHT_MCS_C;
		cr->intf_r_vht_mcs_m = INTF_R_CNT_VHT_MCS_C_M;
		cr->intf_r_he_mcs = INTF_R_CNT_HE_MCS_C;
		cr->intf_r_he_mcs_m = INTF_R_CNT_HE_MCS_C_M;
		cr->intf_r_vht_nss = INTF_R_CNT_VHT_NSS_C;
		cr->intf_r_vht_nss_m = INTF_R_CNT_VHT_NSS_C_M;
		cr->intf_r_he_nss = INTF_R_CNT_HE_NSS_C;
		cr->intf_r_he_nss_m = INTF_R_CNT_HE_NSS_C_M;
		cr->intf_r_mac_hdr_type = INTF_R_MAC_HDR_TYPE_C;
		cr->intf_r_mac_hdr_type_m = INTF_R_MAC_HDR_TYPE_C_M;
		cr->intf_r_pkt_type = INTF_R_PKT_TYPE_C;
		cr->intf_r_pkt_type_m = INTF_R_PKT_TYPE_C_M;
		cr->dbcc = DBCC_C;
		cr->dbcc_m = DBCC_C_M;
		cr->dbcc_2p4g_band_sel = DBCC_2P4G_BAND_SEL_C;
		cr->dbcc_2p4g_band_sel_m = DBCC_2P4G_BAND_SEL_C_M;
		cr->cnt_pop_trig = CNT_POP_TRIG_C;
		cr->cnt_pop_trig_m = CNT_POP_TRIG_C_M;
		cr->max_cnt_pop = MAX_CNT_POP_C;
		cr->max_cnt_pop_m = MAX_CNT_POP_C_M;
		break;
	#endif

	#ifdef HALBB_COMPILE_AP2_SERIES
	case BB_AP2:
		cr->cck_cca = CNT_CCK_CCA_P0_A2;
		cr->cck_cca_m = CNT_CCK_CCA_P0_A2_M;
		cr->cck_crc16fail = CNT_CCK_CRC16FAIL_P0_A2;
		cr->cck_crc16fail_m = CNT_CCK_CRC16FAIL_P0_A2_M;
		cr->cck_crc32ok = CNT_CCK_CRC32OK_P0_A2;
		cr->cck_crc32ok_m = CNT_CCK_CRC32OK_P0_A2_M;
		cr->cck_crc32fail = CNT_CCK_CRC32FAIL_P0_A2;
		cr->cck_crc32fail_m = CNT_CCK_CRC32FAIL_P0_A2_M;
		cr->cca_spoofing = CNT_CCA_SPOOFING_A2;
		cr->cca_spoofing_m = CNT_CCA_SPOOFING_A2_M;
		cr->cck_sfd_gg_cnt = SFD_GG_CNT_A2;
		cr->cck_sfd_gg_cnt_m = SFD_GG_CNT_A2_M;
		cr->cck_crc_16 = CRC16_GG_CNT_A2;
		cr->cck_crc_16_m = CRC16_GG_CNT_A2_M;
		cr->cck_sig_gg_cnt = SIG_GG_CNT_A2;
		cr->cck_sig_gg_cnt_m = SIG_GG_CNT_A2_M;
		cr->cck_spoofing = SPOOF_CNT_A2;
		cr->cck_spoofing_m = SPOOF_CNT_A2_M;
		cr->cck_brk_cnt = BRK_CNT_A2;
		cr->cck_brk_cnt_m = BRK_CNT_A2_M;
		cr->lsig_brk_s_th = CNT_LSIG_BRK_S_TH_A2;
		cr->lsig_brk_s_th_m = CNT_LSIG_BRK_S_TH_A2_M;
		cr->lsig_brk_l_th = CNT_LSIG_BRK_L_TH_A2;
		cr->lsig_brk_l_th_m = CNT_LSIG_BRK_L_TH_A2_M;
		cr->htsig_crc8_err_s_th = CNT_HTSIG_CRC8_ERR_S_TH_A2;
		cr->htsig_crc8_err_s_th_m = CNT_HTSIG_CRC8_ERR_S_TH_A2_M;
		cr->htsig_crc8_err_l_th = CNT_HTSIG_CRC8_ERR_L_TH_A2;
		cr->htsig_crc8_err_l_th_m = CNT_HTSIG_CRC8_ERR_L_TH_A2_M;
		cr->brk = CNT_BRK_A2;
		cr->brk_m = CNT_BRK_A2_M;
		cr->brk_sel = CNT_BRK_SEL_A2;
		cr->brk_sel_m = CNT_BRK_SEL_A2_M;
		cr->rxl_err_parity = CNT_RXL_ERR_PARITY_A2;
		cr->rxl_err_parity_m = CNT_RXL_ERR_PARITY_A2_M;
		cr->rxl_err_rate = CNT_RXL_ERR_RATE_A2;
		cr->rxl_err_rate_m = CNT_RXL_ERR_RATE_A2_M;
		cr->ht_err_crc8 = CNT_HT_ERR_CRC8_A2;
		cr->ht_err_crc8_m = CNT_HT_ERR_CRC8_A2_M;
		cr->vht_err_siga_crc8 = CNT_VHT_ERR_SIGA_CRC8_A2;
		cr->vht_err_siga_crc8_m = CNT_VHT_ERR_SIGA_CRC8_A2_M;
		cr->ht_not_support_mcs = CNT_HT_NOT_SUPPORT_MCS_A2;
		cr->ht_not_support_mcs_m = CNT_HT_NOT_SUPPORT_MCS_A2_M;
		cr->vht_not_support_mcs = CNT_VHT_NOT_SUPPORT_MCS_A2;
		cr->vht_not_support_mcs_m = CNT_VHT_NOT_SUPPORT_MCS_A2_M;
		cr->err_during_bt_tx = CNT_ERR_DURING_BT_TX_A2;
		cr->err_during_bt_tx_m = CNT_ERR_DURING_BT_TX_A2_M;
		cr->err_during_bt_rx = CNT_ERR_DURING_BT_RX_A2;
		cr->err_during_bt_rx_m = CNT_ERR_DURING_BT_RX_A2_M;
		cr->edge_murx_nsts0 = CNT_EDGE_MURX_NSTS0_A2;
		cr->edge_murx_nsts0_m = CNT_EDGE_MURX_NSTS0_A2_M;
		cr->search_fail = CNT_SEARCH_FAIL_A2;
		cr->search_fail_m = CNT_SEARCH_FAIL_A2_M;
		cr->ofdm_cca = CNT_OFDM_CCA_A2;
		cr->ofdm_cca_m = CNT_OFDM_CCA_A2_M;
		cr->ofdm_cca_s20 = CNT_OFDM_CCA_S20_A2;
		cr->ofdm_cca_s20_m = CNT_OFDM_CCA_S20_A2_M;
		cr->ofdm_cca_s40 = CNT_OFDM_CCA_S40_A2;
		cr->ofdm_cca_s40_m = CNT_OFDM_CCA_S40_A2_M;
		cr->ofdm_cca_s80 = CNT_OFDM_CCA_S80_A2;
		cr->ofdm_cca_s80_m = CNT_OFDM_CCA_S80_A2_M;
		cr->ccktxon = CNT_CCKTXON_A2;
		cr->ccktxon_m = CNT_CCKTXON_A2_M;
		cr->ccktxen = CNT_CCKTXEN_A2;
		cr->ccktxen_m = CNT_CCKTXEN_A2_M;
		cr->ofdmtxon = CNT_OFDMTXON_A2;
		cr->ofdmtxon_m = CNT_OFDMTXON_A2_M;
		cr->ofdmtxen = CNT_OFDMTXEN_A2;
		cr->ofdmtxen_m = CNT_OFDMTXEN_A2_M;
		cr->drop_trig = CNT_DROP_TRIG_A2;
		cr->drop_trig_m = CNT_DROP_TRIG_A2_M;
		cr->pop_trig = CNT_POP_TRIG_A2;
		cr->pop_trig_m = CNT_POP_TRIG_A2_M;
		cr->tx_conflict = CNT_TX_CONFLICT_A2;
		cr->tx_conflict_m = CNT_TX_CONFLICT_A2_M;
		cr->wmac_rstb = CNT_WMAC_RSTB_A2;
		cr->wmac_rstb_m = CNT_WMAC_RSTB_A2_M;
		cr->en_tb_ppdu_fix_gain = CNT_EN_TB_PPDU_FIX_GAIN_A2;
		cr->en_tb_ppdu_fix_gain_m = CNT_EN_TB_PPDU_FIX_GAIN_A2_M;
		cr->en_tb_cca_pw_th = CNT_EN_TB_CCA_PW_TH_A2;
		cr->en_tb_cca_pw_th_m = CNT_EN_TB_CCA_PW_TH_A2_M;
		cr->he_crc_ok = CNT_HE_CRC_OK_A2;
		cr->he_crc_ok_m = CNT_HE_CRC_OK_A2_M;
		cr->he_crc_err = CNT_HE_CRC_ERR_A2;
		cr->he_crc_err_m = CNT_HE_CRC_ERR_A2_M;
		cr->vht_crc_ok = CNT_VHT_CRC_OK_A2;
		cr->vht_crc_ok_m = CNT_VHT_CRC_OK_A2_M;
		cr->vht_crc_err = CNT_VHT_CRC_ERR_A2;
		cr->vht_crc_err_m = CNT_VHT_CRC_ERR_A2_M;
		cr->ht_crc_ok = CNT_HT_CRC_OK_A2;
		cr->ht_crc_ok_m = CNT_HT_CRC_OK_A2_M;
		cr->ht_crc_err = CNT_HT_CRC_ERR_A2;
		cr->ht_crc_err_m = CNT_HT_CRC_ERR_A2_M;
		cr->l_crc_ok = CNT_L_CRC_OK_A2;
		cr->l_crc_ok_m = CNT_L_CRC_OK_A2_M;
		cr->l_crc_err = CNT_L_CRC_ERR_A2;
		cr->l_crc_err_m = CNT_L_CRC_ERR_A2_M;
		cr->he_crc_ok2 = CNT_HE_CRC_OK2_A2;
		cr->he_crc_ok2_m = CNT_HE_CRC_OK2_A2_M;
		cr->he_crc_err2 = CNT_HE_CRC_ERR2_A2;
		cr->he_crc_err2_m = CNT_HE_CRC_ERR2_A2_M;
		cr->vht_crc_ok2 = CNT_VHT_CRC_OK2_A2;
		cr->vht_crc_ok2_m = CNT_VHT_CRC_OK2_A2_M;
		cr->vht_crc_err2 = CNT_VHT_CRC_ERR2_A2;
		cr->vht_crc_err2_m = CNT_VHT_CRC_ERR2_A2_M;
		cr->ht_crc_ok2 = CNT_HT_CRC_OK2_A2;
		cr->ht_crc_ok2_m = CNT_HT_CRC_OK2_A2_M;
		cr->ht_crc_err2 = CNT_HT_CRC_ERR2_A2;
		cr->ht_crc_err2_m = CNT_HT_CRC_ERR2_A2_M;
		cr->l_crc_ok2 = CNT_L_CRC_OK2_A2;
		cr->l_crc_ok2_m = CNT_L_CRC_OK2_A2_M;
		cr->l_crc_err2 = CNT_L_CRC_ERR2_A2;
		cr->l_crc_err2_m = CNT_L_CRC_ERR2_A2_M;
		cr->l_crc_ok3 = CNT_L_CRC_OK3_A2;
		cr->l_crc_ok3_m = CNT_L_CRC_OK3_A2_M;
		cr->l_crc_err3 = CNT_L_CRC_ERR3_A2;
		cr->l_crc_err3_m = CNT_L_CRC_ERR3_A2_M;
		cr->ampdu_rxon = CNT_AMPDU_RXON_A2;
		cr->ampdu_rxon_m = CNT_AMPDU_RXON_A2_M;
		cr->ampdu_miss = CNT_AMPDU_MISS_A2;
		cr->ampdu_miss_m = CNT_AMPDU_MISS_A2_M;
		cr->ampdu_crc_ok = CNT_AMPDU_RX_CRC32_OK_A2;
		cr->ampdu_crc_ok_m = CNT_AMPDU_RX_CRC32_OK_A2_M;
		cr->ampdu_crc_err = CNT_AMPDU_RX_CRC32_ERR_A2;
		cr->ampdu_crc_err_m = CNT_AMPDU_RX_CRC32_ERR_A2_M;
		cr->hesu_err_sig_a_crc4 = CNT_HESU_ERR_SIG_A_CRC4_A2;
		cr->hesu_err_sig_a_crc4_m = CNT_HESU_ERR_SIG_A_CRC4_A2_M;
		cr->heersu_err_sig_a_crc4 = CNT_HEERSU_ERR_SIG_A_CRC4_A2;
		cr->heersu_err_sig_a_crc4_m = CNT_HEERSU_ERR_SIG_A_CRC4_A2_M;
		cr->hemu_err_sig_a_crc4 = CNT_HEMU_ERR_SIG_A_CRC4_A2;
		cr->hemu_err_sig_a_crc4_m = CNT_HEMU_ERR_SIG_A_CRC4_A2_M;
		cr->hemu_err_sigb_ch1_comm_crc4 = CNT_HEMU_ERR_SIGB_CH1_COMM_CRC4_A2;
		cr->hemu_err_sigb_ch1_comm_crc4_m = CNT_HEMU_ERR_SIGB_CH1_COMM_CRC4_A2_M;
		cr->hemu_err_sigb_ch2_comm_crc4 = CNT_HEMU_ERR_SIGB_CH2_COMM_CRC4_A2;
		cr->hemu_err_sigb_ch2_comm_crc4_m = CNT_HEMU_ERR_SIGB_CH2_COMM_CRC4_A2_M;
		cr->he_u0_err_bcc_mcs = CNT_HE_U0_ERR_BCC_MCS_A2;
		cr->he_u0_err_bcc_mcs_m = CNT_HE_U0_ERR_BCC_MCS_A2_M;
		cr->he_u0_err_mcs = CNT_HE_U0_ERR_MCS_A2;
		cr->he_u0_err_mcs_m = CNT_HE_U0_ERR_MCS_A2_M;
		cr->he_u0_err_dcm_mcs = CNT_HE_U0_ERR_DCM_MCS_A2;
		cr->he_u0_err_dcm_mcs_m = CNT_HE_U0_ERR_DCM_MCS_A2_M;
		cr->r1b_rx_rpt_rst = R1B_RX_RPT_RST_A2;
		cr->r1b_rx_rpt_rst_m = R1B_RX_RPT_RST_A2_M;
		cr->rst_all_cnt = RST_ALL_CNT_A2;
		cr->rst_all_cnt_m = RST_ALL_CNT_A2_M;
		cr->enable_all_cnt = ENABLE_ALL_CNT_A2;
		cr->enable_all_cnt_m = ENABLE_ALL_CNT_A2_M;
		cr->enable_ofdm = ENABLE_OFDM_A2;
		cr->enable_ofdm_m = ENABLE_OFDM_A2_M;
		cr->enable_cck = ENABLE_CCK_A2;
		cr->enable_cck_m = ENABLE_CCK_A2_M;
		cr->r1b_rx_dis_cca = R1B_RX_DIS_CCA_A2;
		cr->r1b_rx_dis_cca_m = R1B_RX_DIS_CCA_A2_M;
		cr->intf_r_rate = INTF_R_CNT_RATE_A2;
		cr->intf_r_rate_m = INTF_R_CNT_RATE_A2_M;
		cr->intf_r_mcs = INTF_R_CNT_MCS_A2;
		cr->intf_r_mcs_m = INTF_R_CNT_MCS_A2_M;
		cr->intf_r_vht_mcs = INTF_R_CNT_VHT_MCS_A2;
		cr->intf_r_vht_mcs_m = INTF_R_CNT_VHT_MCS_A2_M;
		cr->intf_r_he_mcs = INTF_R_CNT_HE_MCS_A2;
		cr->intf_r_he_mcs_m = INTF_R_CNT_HE_MCS_A2_M;
		cr->intf_r_vht_nss = INTF_R_CNT_VHT_NSS_A2;
		cr->intf_r_vht_nss_m = INTF_R_CNT_VHT_NSS_A2_M;
		cr->intf_r_he_nss = INTF_R_CNT_HE_NSS_A2;
		cr->intf_r_he_nss_m = INTF_R_CNT_HE_NSS_A2_M;
		cr->intf_r_mac_hdr_type = INTF_R_MAC_HDR_TYPE_A2;
		cr->intf_r_mac_hdr_type_m = INTF_R_MAC_HDR_TYPE_A2_M;
		cr->intf_r_pkt_type = INTF_R_PKT_TYPE_A2;
		cr->intf_r_pkt_type_m = INTF_R_PKT_TYPE_A2_M;
		cr->dbcc = DBCC_A2;
		cr->dbcc_m = DBCC_A2_M;
		cr->dbcc_2p4g_band_sel = DBCC_2P4G_BAND_SEL_A2;
		cr->dbcc_2p4g_band_sel_m = DBCC_2P4G_BAND_SEL_A2_M;
		cr->cnt_pop_trig = CNT_POP_TRIG_A2;
		cr->cnt_pop_trig_m = CNT_POP_TRIG_A2_M;
		cr->max_cnt_pop = MAX_CNT_POP_A2;
		cr->max_cnt_pop_m = MAX_CNT_POP_A2_M;
		cr->break_option = BRK_R_BRK_OPT_63_32__A2;
		cr->break_option_m = BRK_R_BRK_OPT_63_32__A2_M;
		break;
	#endif

	#ifdef HALBB_COMPILE_BE0_SERIES
	case BB_BE0:
		cr->cck_cca = CNT_CCK_CCA_P0_BE0;
		cr->cck_cca_m = CNT_CCK_CCA_P0_BE0_M;
		cr->cck_crc16fail = CNT_CCK_CRC16FAIL_P0_BE0;
		cr->cck_crc16fail_m = CNT_CCK_CRC16FAIL_P0_BE0_M;
		cr->cck_crc32ok = CNT_CCK_CRC32OK_P0_BE0;
		cr->cck_crc32ok_m = CNT_CCK_CRC32OK_P0_BE0_M;
		cr->cck_crc32fail = CNT_CCK_CRC32FAIL_P0_BE0;
		cr->cck_crc32fail_m = CNT_CCK_CRC32FAIL_P0_BE0_M;
		cr->cca_spoofing = CNT_CCA_SPOOFING_BE0;
		cr->cca_spoofing_m = CNT_CCA_SPOOFING_BE0_M;
		cr->cck_sfd_gg_cnt = SFD_GG_CNT_BE0;
		cr->cck_sfd_gg_cnt_m = SFD_GG_CNT_BE0_M;
		cr->cck_crc_16 = CRC16_GG_CNT_BE0;
		cr->cck_crc_16_m = CRC16_GG_CNT_BE0_M;
		cr->cck_sig_gg_cnt = SIG_GG_CNT_BE0;
		cr->cck_sig_gg_cnt_m = SIG_GG_CNT_BE0_M;
		cr->cck_spoofing = SPOOF_CNT_BE0;
		cr->cck_spoofing_m = SPOOF_CNT_BE0_M;
		cr->cck_brk_cnt = BRK_CNT_BE0;
		cr->cck_brk_cnt_m = BRK_CNT_BE0_M;
		cr->lsig_brk_s_th = CNT_LSIG_BRK_S_TH_BE0;
		cr->lsig_brk_s_th_m = CNT_LSIG_BRK_S_TH_BE0_M;
		cr->lsig_brk_l_th = CNT_LSIG_BRK_L_TH_BE0;
		cr->lsig_brk_l_th_m = CNT_LSIG_BRK_L_TH_BE0_M;
		cr->htsig_crc8_err_s_th = CNT_HTSIG_CRC8_ERR_S_TH_BE0;
		cr->htsig_crc8_err_s_th_m = CNT_HTSIG_CRC8_ERR_S_TH_BE0_M;
		cr->htsig_crc8_err_l_th = CNT_HTSIG_CRC8_ERR_L_TH_BE0;
		cr->htsig_crc8_err_l_th_m = CNT_HTSIG_CRC8_ERR_L_TH_BE0_M;
		cr->brk = CNT_BRK_BE0;
		cr->brk_m = CNT_BRK_BE0_M;
		cr->brk_sel = CNT_BRK_SEL_BE0;
		cr->brk_sel_m = CNT_BRK_SEL_BE0_M;
		cr->rxl_err_parity = CNT_RXL_ERR_PARITY_BE0;
		cr->rxl_err_parity_m = CNT_RXL_ERR_PARITY_BE0_M;
		cr->rxl_err_rate = CNT_RXL_ERR_RATE_BE0;
		cr->rxl_err_rate_m = CNT_RXL_ERR_RATE_BE0_M;
		cr->ht_err_crc8 = CNT_HT_ERR_CRC8_BE0;
		cr->ht_err_crc8_m = CNT_HT_ERR_CRC8_BE0_M;
		cr->vht_err_siga_crc8 = CNT_VHT_ERR_SIGA_CRC8_BE0;
		cr->vht_err_siga_crc8_m = CNT_VHT_ERR_SIGA_CRC8_BE0_M;
		cr->ht_not_support_mcs = CNT_HT_NOT_SUPPORT_MCS_BE0;
		cr->ht_not_support_mcs_m = CNT_HT_NOT_SUPPORT_MCS_BE0_M;
		cr->vht_not_support_mcs = CNT_VHT_NOT_SUPPORT_MCS_BE0;
		cr->vht_not_support_mcs_m = CNT_VHT_NOT_SUPPORT_MCS_BE0_M;
		cr->err_during_bt_tx = CNT_ERR_DURING_BT_TX_BE0;
		cr->err_during_bt_tx_m = CNT_ERR_DURING_BT_TX_BE0_M;
		cr->err_during_bt_rx = CNT_ERR_DURING_BT_RX_BE0;
		cr->err_during_bt_rx_m = CNT_ERR_DURING_BT_RX_BE0_M;
		cr->edge_murx_nsts0 = CNT_EDGE_MURX_NSTS0_BE0;
		cr->edge_murx_nsts0_m = CNT_EDGE_MURX_NSTS0_BE0_M;
		cr->search_fail = CNT_SEARCH_FAIL_BE0;
		cr->search_fail_m = CNT_SEARCH_FAIL_BE0_M;
		cr->ofdm_cca = CNT_OFDM_CCA_BE0;
		cr->ofdm_cca_m = CNT_OFDM_CCA_BE0_M;
		cr->ofdm_cca_s20 = CNT_OFDM_CCA_S20_BE0;
		cr->ofdm_cca_s20_m = CNT_OFDM_CCA_S20_BE0_M;
		cr->ofdm_cca_s40 = CNT_OFDM_CCA_S40_BE0;
		cr->ofdm_cca_s40_m = CNT_OFDM_CCA_S40_BE0_M;
		cr->ofdm_cca_s80 = CNT_OFDM_CCA_S80_BE0;
		cr->ofdm_cca_s80_m = CNT_OFDM_CCA_S80_BE0_M;
		cr->ccktxon = CNT_CCKTXON_BE0;
		cr->ccktxon_m = CNT_CCKTXON_BE0_M;
		cr->ccktxen = CNT_CCKTXEN_BE0;
		cr->ccktxen_m = CNT_CCKTXEN_BE0_M;
		cr->ofdmtxon = CNT_OFDMTXON_BE0;
		cr->ofdmtxon_m = CNT_OFDMTXON_BE0_M;
		cr->ofdmtxen = CNT_OFDMTXEN_BE0;
		cr->ofdmtxen_m = CNT_OFDMTXEN_BE0_M;
		cr->drop_trig = CNT_DROP_TRIG_BE0;
		cr->drop_trig_m = CNT_DROP_TRIG_BE0_M;
		cr->pop_trig = CNT_POP_TRIG_BE0;
		cr->pop_trig_m = CNT_POP_TRIG_BE0_M;
		cr->tx_conflict = CNT_TX_CONFLICT_BE0;
		cr->tx_conflict_m = CNT_TX_CONFLICT_BE0_M;
		cr->wmac_rstb = CNT_WMAC_RSTB_BE0;
		cr->wmac_rstb_m = CNT_WMAC_RSTB_BE0_M;
		cr->en_tb_ppdu_fix_gain = CNT_EN_TB_PPDU_FIX_GAIN_BE0;
		cr->en_tb_ppdu_fix_gain_m = CNT_EN_TB_PPDU_FIX_GAIN_BE0_M;
		cr->en_tb_cca_pw_th = CNT_EN_TB_CCA_PW_TH_BE0;
		cr->en_tb_cca_pw_th_m = CNT_EN_TB_CCA_PW_TH_BE0_M;
		cr->eht_crc_ok = CNT_EHT_CRC_OK_BE0;
		cr->eht_crc_ok_m = CNT_EHT_CRC_OK_BE0_M;
		cr->eht_crc_err = CNT_EHT_CRC_ERR_BE0;
		cr->eht_crc_err_m = CNT_EHT_CRC_ERR_BE0_M;
		cr->he_crc_ok = CNT_HE_CRC_OK_BE0;
		cr->he_crc_ok_m = CNT_HE_CRC_OK_BE0_M;
		cr->he_crc_err = CNT_HE_CRC_ERR_BE0;
		cr->he_crc_err_m = CNT_HE_CRC_ERR_BE0_M;
		cr->vht_crc_ok = CNT_VHT_CRC_OK_BE0;
		cr->vht_crc_ok_m = CNT_VHT_CRC_OK_BE0_M;
		cr->vht_crc_err = CNT_VHT_CRC_ERR_BE0;
		cr->vht_crc_err_m = CNT_VHT_CRC_ERR_BE0_M;
		cr->ht_crc_ok = CNT_HT_CRC_OK_BE0;
		cr->ht_crc_ok_m = CNT_HT_CRC_OK_BE0_M;
		cr->ht_crc_err = CNT_HT_CRC_ERR_BE0;
		cr->ht_crc_err_m = CNT_HT_CRC_ERR_BE0_M;
		cr->l_crc_ok = CNT_L_CRC_OK_BE0;
		cr->l_crc_ok_m = CNT_L_CRC_OK_BE0_M;
		cr->l_crc_err = CNT_L_CRC_ERR_BE0;
		cr->l_crc_err_m = CNT_L_CRC_ERR_BE0_M;
		cr->eht_crc_ok2 = CNT_EHT_CRC_OK2_BE0;
		cr->eht_crc_ok2_m = CNT_EHT_CRC_OK2_BE0_M;
		cr->eht_crc_err2 = CNT_EHT_CRC_ERR2_BE0;
		cr->eht_crc_err2_m = CNT_EHT_CRC_ERR2_BE0_M;
		cr->he_crc_ok2 = CNT_HE_CRC_OK2_BE0;
		cr->he_crc_ok2_m = CNT_HE_CRC_OK2_BE0_M;
		cr->he_crc_err2 = CNT_HE_CRC_ERR2_BE0;
		cr->he_crc_err2_m = CNT_HE_CRC_ERR2_BE0_M;
		cr->vht_crc_ok2 = CNT_VHT_CRC_OK2_BE0;
		cr->vht_crc_ok2_m = CNT_VHT_CRC_OK2_BE0_M;
		cr->vht_crc_err2 = CNT_VHT_CRC_ERR2_BE0;
		cr->vht_crc_err2_m = CNT_VHT_CRC_ERR2_BE0_M;
		cr->ht_crc_ok2 = CNT_HT_CRC_OK2_BE0;
		cr->ht_crc_ok2_m = CNT_HT_CRC_OK2_BE0_M;
		cr->ht_crc_err2 = CNT_HT_CRC_ERR2_BE0;
		cr->ht_crc_err2_m = CNT_HT_CRC_ERR2_BE0_M;
		cr->l_crc_ok2 = CNT_L_CRC_OK2_BE0;
		cr->l_crc_ok2_m = CNT_L_CRC_OK2_BE0_M;
		cr->l_crc_err2 = CNT_L_CRC_ERR2_BE0;
		cr->l_crc_err2_m = CNT_L_CRC_ERR2_BE0_M;
		cr->l_crc_ok3 = CNT_L_CRC_OK3_BE0;
		cr->l_crc_ok3_m = CNT_L_CRC_OK3_BE0_M;
		cr->l_crc_err3 = CNT_L_CRC_ERR3_BE0;
		cr->l_crc_err3_m = CNT_L_CRC_ERR3_BE0_M;
		cr->ampdu_rxon = CNT_AMPDU_RXON_BE0;
		cr->ampdu_rxon_m = CNT_AMPDU_RXON_BE0_M;
		cr->ampdu_miss = CNT_AMPDU_MISS_BE0;
		cr->ampdu_miss_m = CNT_AMPDU_MISS_BE0_M;
		cr->ampdu_crc_ok = CNT_AMPDU_RX_CRC32_OK_BE0;
		cr->ampdu_crc_ok_m = CNT_AMPDU_RX_CRC32_OK_BE0_M;
		cr->ampdu_crc_err = CNT_AMPDU_RX_CRC32_ERR_BE0;
		cr->ampdu_crc_err_m = CNT_AMPDU_RX_CRC32_ERR_BE0_M;
		cr->hesu_err_sig_a_crc4 = CNT_HESU_ERR_SIG_A_CRC4_BE0;
		cr->hesu_err_sig_a_crc4_m = CNT_HESU_ERR_SIG_A_CRC4_BE0_M;
		cr->heersu_err_sig_a_crc4 = CNT_HEERSU_ERR_SIG_A_CRC4_BE0;
		cr->heersu_err_sig_a_crc4_m = CNT_HEERSU_ERR_SIG_A_CRC4_BE0_M;
		cr->hemu_err_sig_a_crc4 = CNT_HEMU_ERR_SIG_A_CRC4_BE0;
		cr->hemu_err_sig_a_crc4_m = CNT_HEMU_ERR_SIG_A_CRC4_BE0_M;
		cr->hemu_err_sigb_ch1_comm_crc4 = CNT_HEMU_ERR_SIGB_CH1_COMM_CRC4_BE0;
		cr->hemu_err_sigb_ch1_comm_crc4_m = CNT_HEMU_ERR_SIGB_CH1_COMM_CRC4_BE0_M;
		cr->hemu_err_sigb_ch2_comm_crc4 = CNT_HEMU_ERR_SIGB_CH2_COMM_CRC4_BE0;
		cr->hemu_err_sigb_ch2_comm_crc4_m = CNT_HEMU_ERR_SIGB_CH2_COMM_CRC4_BE0_M;
		cr->he_u0_err_bcc_mcs = CNT_HE_U0_ERR_BCC_MCS_BE0;
		cr->he_u0_err_bcc_mcs_m = CNT_HE_U0_ERR_BCC_MCS_BE0_M;
		cr->he_u0_err_mcs = CNT_HE_U0_ERR_MCS_BE0;
		cr->he_u0_err_mcs_m = CNT_HE_U0_ERR_MCS_BE0_M;
		cr->he_u0_err_dcm_mcs = CNT_HE_U0_ERR_DCM_MCS_BE0;
		cr->he_u0_err_dcm_mcs_m = CNT_HE_U0_ERR_DCM_MCS_BE0_M;
		cr->r1b_rx_rpt_rst = R1B_RX_RPT_RST_BE0;
		cr->r1b_rx_rpt_rst_m = R1B_RX_RPT_RST_BE0_M;
		cr->rst_all_cnt = RST_ALL_CNT_BE0;
		cr->rst_all_cnt_m = RST_ALL_CNT_BE0_M;
		cr->enable_all_cnt = ENABLE_ALL_CNT_BE0;
		cr->enable_all_cnt_m = ENABLE_ALL_CNT_BE0_M;
		cr->enable_ofdm = ENABLE_OFDM_BE0;
		cr->enable_ofdm_m = ENABLE_OFDM_BE0_M;
		cr->enable_cck = ENABLE_CCK_BE0;
		cr->enable_cck_m = ENABLE_CCK_BE0_M;
		cr->r1b_rx_dis_cca = R1B_RX_DIS_CCA_BE0;
		cr->r1b_rx_dis_cca_m = R1B_RX_DIS_CCA_BE0_M;
		cr->intf_r_rate = INTF_R_CNT_RATE_BE0;
		cr->intf_r_rate_m = INTF_R_CNT_RATE_BE0_M;
		cr->intf_r_mcs = INTF_R_CNT_MCS_BE0;
		cr->intf_r_mcs_m = INTF_R_CNT_MCS_BE0_M;
		cr->intf_r_vht_mcs = INTF_R_CNT_VHT_MCS_BE0;
		cr->intf_r_vht_mcs_m = INTF_R_CNT_VHT_MCS_BE0_M;
		cr->intf_r_he_mcs = INTF_R_CNT_HE_MCS_BE0;
		cr->intf_r_he_mcs_m = INTF_R_CNT_HE_MCS_BE0_M;
		cr->intf_r_eht_mcs = INTF_R_CNT_EHT_MCS_BE0;
		cr->intf_r_eht_mcs_m = INTF_R_CNT_EHT_MCS_BE0_M;
		cr->intf_r_vht_nss = INTF_R_CNT_VHT_NSS_BE0;
		cr->intf_r_vht_nss_m = INTF_R_CNT_VHT_NSS_BE0_M;
		cr->intf_r_he_nss = INTF_R_CNT_HE_NSS_BE0;
		cr->intf_r_he_nss_m = INTF_R_CNT_HE_NSS_BE0_M;
		cr->intf_r_eht_nss = INTF_R_CNT_EHT_NSS_BE0;
		cr->intf_r_eht_nss_m = INTF_R_CNT_EHT_NSS_BE0_M;
		cr->intf_r_mac_hdr_type = INTF_R_MAC_HDR_TYPE_BE0;
		cr->intf_r_mac_hdr_type_m = INTF_R_MAC_HDR_TYPE_BE0_M;
		cr->intf_r_pkt_type = INTF_R_PKT_TYPE_BE0;
		cr->intf_r_pkt_type_m = INTF_R_PKT_TYPE_BE0_M;
		cr->dbcc = DBCC_BE0;
		cr->dbcc_m = DBCC_BE0_M;
		cr->dbcc_2p4g_band_sel = DBCC_2P4G_BAND_SEL_BE0;
		cr->dbcc_2p4g_band_sel_m = DBCC_2P4G_BAND_SEL_BE0_M;
		cr->cnt_pop_trig = CNT_POP_TRIG_BE0;
		cr->cnt_pop_trig_m = CNT_POP_TRIG_BE0_M;
		cr->max_cnt_pop = MAX_CNT_POP_BE0;
		cr->max_cnt_pop_m = MAX_CNT_POP_BE0_M;
		break;
	#endif

	#ifdef HALBB_COMPILE_BE1_SERIES
	case BB_BE1:
		cr->cck_cca = CNT_CCK_CCA_P0_BE1;
		cr->cck_cca_m = CNT_CCK_CCA_P0_BE1_M;
		cr->cck_crc16fail = CNT_CCK_CRC16FAIL_P0_BE1;
		cr->cck_crc16fail_m = CNT_CCK_CRC16FAIL_P0_BE1_M;
		cr->cck_crc32ok = CNT_CCK_CRC32OK_P0_BE1;
		cr->cck_crc32ok_m = CNT_CCK_CRC32OK_P0_BE1_M;
		cr->cck_crc32fail = CNT_CCK_CRC32FAIL_P0_BE1;
		cr->cck_crc32fail_m = CNT_CCK_CRC32FAIL_P0_BE1_M;
		cr->cca_spoofing = CNT_CCA_SPOOFING_BE1;
		cr->cca_spoofing_m = CNT_CCA_SPOOFING_BE1_M;
		cr->cck_sfd_gg_cnt = SFD_GG_CNT_BE1;
		cr->cck_sfd_gg_cnt_m = SFD_GG_CNT_BE1_M;
		cr->cck_crc_16 = CRC16_GG_CNT_BE1;
		cr->cck_crc_16_m = CRC16_GG_CNT_BE1_M;
		cr->cck_sig_gg_cnt = SIG_GG_CNT_BE1;
		cr->cck_sig_gg_cnt_m = SIG_GG_CNT_BE1_M;
		cr->cck_spoofing = SPOOF_CNT_BE1;
		cr->cck_spoofing_m = SPOOF_CNT_BE1_M;
		cr->cck_brk_cnt = BRK_CNT_BE1;
		cr->cck_brk_cnt_m = BRK_CNT_BE1_M;
		cr->lsig_brk_s_th = CNT_LSIG_BRK_S_TH_BE1;
		cr->lsig_brk_s_th_m = CNT_LSIG_BRK_S_TH_BE1_M;
		cr->lsig_brk_l_th = CNT_LSIG_BRK_L_TH_BE1;
		cr->lsig_brk_l_th_m = CNT_LSIG_BRK_L_TH_BE1_M;
		cr->htsig_crc8_err_s_th = CNT_HTSIG_CRC8_ERR_S_TH_BE1;
		cr->htsig_crc8_err_s_th_m = CNT_HTSIG_CRC8_ERR_S_TH_BE1_M;
		cr->htsig_crc8_err_l_th = CNT_HTSIG_CRC8_ERR_L_TH_BE1;
		cr->htsig_crc8_err_l_th_m = CNT_HTSIG_CRC8_ERR_L_TH_BE1_M;
		cr->brk = CNT_BRK_BE1;
		cr->brk_m = CNT_BRK_BE1_M;
		cr->brk_sel = CNT_BRK_SEL_BE1;
		cr->brk_sel_m = CNT_BRK_SEL_BE1_M;
		cr->rxl_err_parity = CNT_RXL_ERR_PARITY_BE1;
		cr->rxl_err_parity_m = CNT_RXL_ERR_PARITY_BE1_M;
		cr->rxl_err_rate = CNT_RXL_ERR_RATE_BE1;
		cr->rxl_err_rate_m = CNT_RXL_ERR_RATE_BE1_M;
		cr->ht_err_crc8 = CNT_HT_ERR_CRC8_BE1;
		cr->ht_err_crc8_m = CNT_HT_ERR_CRC8_BE1_M;
		cr->vht_err_siga_crc8 = CNT_VHT_ERR_SIGA_CRC8_BE1;
		cr->vht_err_siga_crc8_m = CNT_VHT_ERR_SIGA_CRC8_BE1_M;
		cr->ht_not_support_mcs = CNT_HT_NOT_SUPPORT_MCS_BE1;
		cr->ht_not_support_mcs_m = CNT_HT_NOT_SUPPORT_MCS_BE1_M;
		cr->vht_not_support_mcs = CNT_VHT_NOT_SUPPORT_MCS_BE1;
		cr->vht_not_support_mcs_m = CNT_VHT_NOT_SUPPORT_MCS_BE1_M;
		cr->err_during_bt_tx = CNT_ERR_DURING_BT_TX_BE1;
		cr->err_during_bt_tx_m = CNT_ERR_DURING_BT_TX_BE1_M;
		cr->err_during_bt_rx = CNT_ERR_DURING_BT_RX_BE1;
		cr->err_during_bt_rx_m = CNT_ERR_DURING_BT_RX_BE1_M;
		cr->edge_murx_nsts0 = CNT_EDGE_MURX_NSTS0_BE1;
		cr->edge_murx_nsts0_m = CNT_EDGE_MURX_NSTS0_BE1_M;
		cr->search_fail = CNT_SEARCH_FAIL_BE1;
		cr->search_fail_m = CNT_SEARCH_FAIL_BE1_M;
		cr->ofdm_cca = CNT_OFDM_CCA_BE1;
		cr->ofdm_cca_m = CNT_OFDM_CCA_BE1_M;
		cr->ofdm_cca_s20 = CNT_OFDM_CCA_S20_BE1;
		cr->ofdm_cca_s20_m = CNT_OFDM_CCA_S20_BE1_M;
		cr->ofdm_cca_s40 = CNT_OFDM_CCA_S40_BE1;
		cr->ofdm_cca_s40_m = CNT_OFDM_CCA_S40_BE1_M;
		cr->ofdm_cca_s80 = CNT_OFDM_CCA_S80_BE1;
		cr->ofdm_cca_s80_m = CNT_OFDM_CCA_S80_BE1_M;
		cr->ccktxon = CNT_CCKTXON_BE1;
		cr->ccktxon_m = CNT_CCKTXON_BE1_M;
		cr->ccktxen = CNT_CCKTXEN_BE1;
		cr->ccktxen_m = CNT_CCKTXEN_BE1_M;
		cr->ofdmtxon = CNT_OFDMTXON_BE1;
		cr->ofdmtxon_m = CNT_OFDMTXON_BE1_M;
		cr->ofdmtxen = CNT_OFDMTXEN_BE1;
		cr->ofdmtxen_m = CNT_OFDMTXEN_BE1_M;
		cr->drop_trig = CNT_DROP_TRIG_BE1;
		cr->drop_trig_m = CNT_DROP_TRIG_BE1_M;
		cr->pop_trig = CNT_POP_TRIG_BE1;
		cr->pop_trig_m = CNT_POP_TRIG_BE1_M;
		cr->tx_conflict = CNT_TX_CONFLICT_BE1;
		cr->tx_conflict_m = CNT_TX_CONFLICT_BE1_M;
		cr->wmac_rstb = CNT_WMAC_RSTB_BE1;
		cr->wmac_rstb_m = CNT_WMAC_RSTB_BE1_M;
		cr->en_tb_ppdu_fix_gain = CNT_EN_TB_PPDU_FIX_GAIN_BE1;
		cr->en_tb_ppdu_fix_gain_m = CNT_EN_TB_PPDU_FIX_GAIN_BE1_M;
		cr->en_tb_cca_pw_th = CNT_EN_TB_CCA_PWR_TH_BE1;
		cr->en_tb_cca_pw_th_m = CNT_EN_TB_CCA_PWR_TH_BE1_M;
		cr->eht_crc_ok = CNT_EHT_CRC_OK_BE1;
		cr->eht_crc_ok_m = CNT_EHT_CRC_OK_BE1_M;
		cr->eht_crc_err = CNT_EHT_CRC_ERR_BE1;
		cr->eht_crc_err_m = CNT_EHT_CRC_ERR_BE1_M;
		cr->he_crc_ok = CNT_HE_CRC_OK_BE1;
		cr->he_crc_ok_m = CNT_HE_CRC_OK_BE1_M;
		cr->he_crc_err = CNT_HE_CRC_ERR_BE1;
		cr->he_crc_err_m = CNT_HE_CRC_ERR_BE1_M;
		cr->vht_crc_ok = CNT_VHT_CRC_OK_BE1;
		cr->vht_crc_ok_m = CNT_VHT_CRC_OK_BE1_M;
		cr->vht_crc_err = CNT_VHT_CRC_ERR_BE1;
		cr->vht_crc_err_m = CNT_VHT_CRC_ERR_BE1_M;
		cr->ht_crc_ok = CNT_HT_CRC_OK_BE1;
		cr->ht_crc_ok_m = CNT_HT_CRC_OK_BE1_M;
		cr->ht_crc_err = CNT_HT_CRC_ERR_BE1;
		cr->ht_crc_err_m = CNT_HT_CRC_ERR_BE1_M;
		cr->l_crc_ok = CNT_L_CRC_OK_BE1;
		cr->l_crc_ok_m = CNT_L_CRC_OK_BE1_M;
		cr->l_crc_err = CNT_L_CRC_ERR_BE1;
		cr->l_crc_err_m = CNT_L_CRC_ERR_BE1_M;
		cr->eht_crc_ok2 = CNT_EHT_CRC_OK2_BE1;
		cr->eht_crc_ok2_m = CNT_EHT_CRC_OK2_BE1_M;
		cr->eht_crc_err2 = CNT_EHT_CRC_ERR2_BE1;
		cr->eht_crc_err2_m = CNT_EHT_CRC_ERR2_BE1_M;
		cr->he_crc_ok2 = CNT_HE_CRC_OK2_BE1;
		cr->he_crc_ok2_m = CNT_HE_CRC_OK2_BE1_M;
		cr->he_crc_err2 = CNT_HE_CRC_ERR2_BE1;
		cr->he_crc_err2_m = CNT_HE_CRC_ERR2_BE1_M;
		cr->vht_crc_ok2 = CNT_VHT_CRC_OK2_BE1;
		cr->vht_crc_ok2_m = CNT_VHT_CRC_OK2_BE1_M;
		cr->vht_crc_err2 = CNT_VHT_CRC_ERR2_BE1;
		cr->vht_crc_err2_m = CNT_VHT_CRC_ERR2_BE1_M;
		cr->ht_crc_ok2 = CNT_HT_CRC_OK2_BE1;
		cr->ht_crc_ok2_m = CNT_HT_CRC_OK2_BE1_M;
		cr->ht_crc_err2 = CNT_HT_CRC_ERR2_BE1;
		cr->ht_crc_err2_m = CNT_HT_CRC_ERR2_BE1_M;
		cr->l_crc_ok2 = CNT_L_CRC_OK2_BE1;
		cr->l_crc_ok2_m = CNT_L_CRC_OK2_BE1_M;
		cr->l_crc_err2 = CNT_L_CRC_ERR2_BE1;
		cr->l_crc_err2_m = CNT_L_CRC_ERR2_BE1_M;
		cr->l_crc_ok3 = CNT_L_CRC_OK3_BE1;
		cr->l_crc_ok3_m = CNT_L_CRC_OK3_BE1_M;
		cr->l_crc_err3 = CNT_L_CRC_ERR3_BE1;
		cr->l_crc_err3_m = CNT_L_CRC_ERR3_BE1_M;
		cr->ampdu_rxon = CNT_AMPDU_RXON_BE1;
		cr->ampdu_rxon_m = CNT_AMPDU_RXON_BE1_M;
		cr->ampdu_miss = CNT_AMPDU_MISS_BE1;
		cr->ampdu_miss_m = CNT_AMPDU_MISS_BE1_M;
		cr->ampdu_crc_ok = CNT_AMPDU_RX_CRC32_OK_BE1;
		cr->ampdu_crc_ok_m = CNT_AMPDU_RX_CRC32_OK_BE1_M;
		cr->ampdu_crc_err = CNT_AMPDU_RX_CRC32_ERR_BE1;
		cr->ampdu_crc_err_m = CNT_AMPDU_RX_CRC32_ERR_BE1_M;
		cr->hesu_err_sig_a_crc4 = CNT_HESU_ERR_SIG_A_CRC4_BE1;
		cr->hesu_err_sig_a_crc4_m = CNT_HESU_ERR_SIG_A_CRC4_BE1_M;
		cr->heersu_err_sig_a_crc4 = CNT_HEERSU_ERR_SIG_A_CRC4_BE1;
		cr->heersu_err_sig_a_crc4_m = CNT_HEERSU_ERR_SIG_A_CRC4_BE1_M;
		cr->hemu_err_sig_a_crc4 = CNT_HEMU_ERR_SIG_A_CRC4_BE1;
		cr->hemu_err_sig_a_crc4_m = CNT_HEMU_ERR_SIG_A_CRC4_BE1_M;
		cr->hemu_err_sigb_ch1_comm_crc4 = CNT_HEMU_ERR_SIGB_CH1_COMM_CRC4_BE1;
		cr->hemu_err_sigb_ch1_comm_crc4_m = CNT_HEMU_ERR_SIGB_CH1_COMM_CRC4_BE1_M;
		cr->hemu_err_sigb_ch2_comm_crc4 = CNT_HEMU_ERR_SIGB_CH2_COMM_CRC4_BE1;
		cr->hemu_err_sigb_ch2_comm_crc4_m = CNT_HEMU_ERR_SIGB_CH2_COMM_CRC4_BE1_M;
		cr->he_u0_err_bcc_mcs = CNT_HE_U0_ERR_BCC_MCS_BE1;
		cr->he_u0_err_bcc_mcs_m = CNT_HE_U0_ERR_BCC_MCS_BE1_M;
		cr->he_u0_err_mcs = CNT_HE_U0_ERR_MCS_BE1;
		cr->he_u0_err_mcs_m = CNT_HE_U0_ERR_MCS_BE1_M;
		cr->he_u0_err_dcm_mcs = CNT_HE_U0_ERR_DCM_MCS_BE1;
		cr->he_u0_err_dcm_mcs_m = CNT_HE_U0_ERR_DCM_MCS_BE1_M;
		cr->r1b_rx_rpt_rst = R1B_RX_RPT_RST_BE1;
		cr->r1b_rx_rpt_rst_m = R1B_RX_RPT_RST_BE1_M;
		cr->rst_all_cnt = TOP_CTRL_P0_R_RST_ALL_CNT_BE1;
		cr->rst_all_cnt_m = TOP_CTRL_P0_R_RST_ALL_CNT_BE1_M;
		cr->enable_all_cnt = TOP_CTRL_P0_R_ENABLE_ALL_CNT_BE1;
		cr->enable_all_cnt_m = TOP_CTRL_P0_R_ENABLE_ALL_CNT_BE1_M;
		cr->enable_ofdm = TOP_CTRL_P0_R_ENABLE_OFDM_BE1;
		cr->enable_ofdm_m = TOP_CTRL_P0_R_ENABLE_OFDM_BE1_M;
		cr->enable_cck = TOP_CTRL_P0_R_ENABLE_CCK_BE1;
		cr->enable_cck_m = TOP_CTRL_P0_R_ENABLE_CCK_BE1_M;
		cr->r1b_rx_dis_cca = R1B_RX_DIS_CCA_BE1;
		cr->r1b_rx_dis_cca_m = R1B_RX_DIS_CCA_BE1_M;
		cr->intf_r_rate = INTF_R_CNT_RATE_BE1;
		cr->intf_r_rate_m = INTF_R_CNT_RATE_BE1_M;
		cr->intf_r_mcs = INTF_R_CNT_MCS_BE1;
		cr->intf_r_mcs_m = INTF_R_CNT_MCS_BE1_M;
		cr->intf_r_vht_mcs = INTF_R_CNT_VHT_MCS_BE1;
		cr->intf_r_vht_mcs_m = INTF_R_CNT_VHT_MCS_BE1_M;
		cr->intf_r_he_mcs = INTF_R_CNT_HE_MCS_BE1;
		cr->intf_r_he_mcs_m = INTF_R_CNT_HE_MCS_BE1_M;
		cr->intf_r_eht_mcs = INTF_R_CNT_EHT_MCS_BE1;
		cr->intf_r_eht_mcs_m = INTF_R_CNT_EHT_MCS_BE1_M;
		cr->intf_r_vht_nss = INTF_R_CNT_VHT_NSS_BE1;
		cr->intf_r_vht_nss_m = INTF_R_CNT_VHT_NSS_BE1_M;
		cr->intf_r_he_nss = INTF_R_CNT_HE_NSS_BE1;
		cr->intf_r_he_nss_m = INTF_R_CNT_HE_NSS_BE1_M;
		cr->intf_r_eht_nss = INTF_R_CNT_EHT_NSS_BE1;
		cr->intf_r_eht_nss_m = INTF_R_CNT_EHT_NSS_BE1_M;
		cr->intf_r_mac_hdr_type = INTF_R_MAC_HDR_TYPE_BE1;
		cr->intf_r_mac_hdr_type_m = INTF_R_MAC_HDR_TYPE_BE1_M;
		cr->intf_r_pkt_type = INTF_R_PKT_TYPE_BE1;
		cr->intf_r_pkt_type_m = INTF_R_PKT_TYPE_BE1_M;
		cr->dbcc = DBCC_BE1;
		cr->dbcc_m = DBCC_BE1_M;
		cr->dbcc_2p4g_band_sel = DBCC_2P4G_BAND_SEL_BE1;
		cr->dbcc_2p4g_band_sel_m = DBCC_2P4G_BAND_SEL_BE1_M;
		cr->cnt_pop_trig = CNT_POP_TRIG_BE1;
		cr->cnt_pop_trig_m = CNT_POP_TRIG_BE1_M;
		cr->max_cnt_pop = MAX_CNT_POP_BE1;
		cr->max_cnt_pop_m = MAX_CNT_POP_BE1_M;
		break;
	#endif
	default:
		BB_WARNING("[%s] BBCR Hook FAIL!\n", __func__);
		if (bb->bb_dbg_i.cr_fake_init_hook_en) {
			BB_TRACE("[%s] BBCR fake init\n", __func__);
			halbb_cr_hook_fake_init(bb, (u32 *)cr, (sizeof(struct bb_stat_cr_info) >> 2));
		}
		break;
	}

	if (bb->bb_dbg_i.cr_init_hook_recorder_en) {
		BB_TRACE("[%s] BBCR Hook dump\n", __func__);
		halbb_cr_hook_init_dump(bb, (u32 *)cr, (sizeof(struct bb_stat_cr_info) >> 2));
	}
}

void halbb_auto_debug_pmac_cnt_chk(struct bb_info *bb)
{
	struct bb_stat_info *stat = &bb->bb_stat_i;
	struct bb_tx_cnt_info *tx = &stat->bb_tx_cnt_i;
	struct bb_cca_info *cca = &stat->bb_cca_i;
	struct bb_stat_hang_info *hang = &bb->bb_stat_i.bb_stat_hang_i;

	if (tx->cck_mac_txen == 0 && tx->ofdm_mac_txen == 0)
		hang->consecutive_no_tx_cnt++;
	else
		hang->consecutive_no_tx_cnt = 0;

	if (cca->cnt_cca_all == 0)
		hang->consecutive_no_rx_cnt++;
	else
		hang->consecutive_no_rx_cnt = 0;

	if (hang->consecutive_no_tx_cnt > 30 && hang->consecutive_no_rx_cnt > 30)
		hang->hang_occur = true;
	else
		hang->hang_occur = false;

	BB_DBG(bb, DBG_FA_CNT, "[%s] consecutive no TX/Rx cnt = {%d, %d} hang_occur=%d\n",
	       __func__, hang->consecutive_no_tx_cnt, hang->consecutive_no_rx_cnt, hang->hang_occur);
}

void halbb_pmac_cck_tx_cnt(struct bb_info *bb)
{
	struct bb_stat_info *stat_t = &bb->bb_stat_i;
	struct bb_tx_cnt_info *tx = &stat_t->bb_tx_cnt_i;
	struct bb_stat_cr_info *cr = &bb->bb_cmn_hooker->bb_stat_cr_i;

	/* read Tx counter */
	tx->cck_phy_txon = halbb_get_reg(bb, cr->ccktxon, cr->ccktxon_m);
	tx->cck_mac_txen = halbb_get_reg(bb, cr->ccktxen, cr->ccktxen_m);
}

void halbb_pmac_cck_cnt(struct bb_info *bb)
{
	struct bb_stat_info *stat_t = &bb->bb_stat_i;
	struct bb_fa_info *fa = &stat_t->bb_fa_i;
	struct bb_cck_fa_info *cck_fa = &fa->bb_cck_fa_i;
	struct bb_cca_info *cca = &stat_t->bb_cca_i;
	struct bb_crc_info *crc = &stat_t->bb_crc_i;
	struct bb_crc2_info *crc2 = &stat_t->bb_crc2_i;
	struct bb_stat_cr_info *cr = &bb->bb_cmn_hooker->bb_stat_cr_i;
	u32 ret_value = 0;

	if ((bb->ic_type == BB_RTL8852A) || (bb->ic_type == BB_RTL8852B) ||
	    (bb->ic_type == BB_RTL8851B)) {
		/* select cck dbg port */
		halbb_set_reg(bb, cr->r1b_rr_sel, cr->r1b_rr_sel_m, 2);

		/* read CCK CCA counter */
		
#ifdef HALBB_DBCC_SUPPORT
		if (bb->bb_phy_idx == HW_PHY_1)
			ret_value = halbb_get_reg(bb, cr->cck_cca + 8, cr->cck_cca_m);
		else
#endif
			ret_value = halbb_get_reg(bb, cr->cck_cca, cr->cck_cca_m);

		cca->cnt_cck_cca = ret_value;

		/* select cck dbg port */
		halbb_set_reg(bb, cr->r1b_rr_sel, cr->r1b_rr_sel_m, 1);

		/* read CCK CRC32 counter */
#ifdef HALBB_DBCC_SUPPORT
		if (bb->bb_phy_idx == HW_PHY_1)
			ret_value = halbb_get_reg(bb, cr->cck_crc32ok + 8, MASKDWORD);
		else
#endif
			ret_value = halbb_get_reg(bb, cr->cck_crc32ok, MASKDWORD);

		crc->cnt_cck_crc32_ok = ret_value & cr->cck_crc32ok_m;
		crc->cnt_cck_crc32_error = (ret_value & cr->cck_crc32fail_m) >> 16;

		/* Read CCK FA counter */
		ret_value = halbb_get_reg(bb, 0x23e0, MASKLWORD); // Reg. doc. doesn't have CCK report reg. 0x78(0x23), need change these addr. one by one
		cck_fa->sfd_gg_cnt = ret_value;

		ret_value = halbb_get_reg(bb, 0x23e4, MASKHWORD);
		cck_fa->cnt_cck_crc_16 = ret_value;

		ret_value = halbb_get_reg(bb, 0x23e8, MASKLWORD);
		cck_fa->sig_gg_cnt = ret_value;

		/* Number of spoofing*/
		ret_value = halbb_get_reg(bb, 0x23ec, MASKBYTE0);
		cca->cnt_cck_spoofing = ret_value;

		//fa->cnt_cck_fail = cck_fa->sfd_gg_cnt + cck_fa->sig_gg_cnt;

		/* Adjust FA computation due to repeated caculatation of brk_cnt when pop starting*/
		fa->cnt_cck_fail = cca->cnt_cck_cca - crc->cnt_cck_crc32_ok -
				   crc->cnt_cck_crc32_error - cca->cnt_cck_spoofing;
	} else {
		BB_DBG(bb, DBG_FA_CNT, "[%s] Type AP2\n", __func__);

		/* read CCK CCA counter */
		cca->cnt_cck_cca = halbb_get_reg_cmn(bb, cr->cck_cca, cr->cck_cca_m, bb->bb_phy_idx);

		/* read CCK CRC32 counter */
		crc->cnt_cck_crc32_ok = halbb_get_reg_cmn(bb, cr->cck_crc32ok, cr->cck_crc32ok_m, bb->bb_phy_idx);
		crc->cnt_cck_crc32_error = halbb_get_reg_cmn(bb, cr->cck_crc32ok, cr->cck_crc32fail_m, bb->bb_phy_idx);


		/* Read CCK FA counter */
		cck_fa->sfd_gg_cnt = halbb_get_reg_cmn(bb, cr->cck_sfd_gg_cnt, cr->cck_sfd_gg_cnt_m, bb->bb_phy_idx);
		cck_fa->cnt_cck_crc_16 = halbb_get_reg_cmn(bb, cr->cck_crc_16, cr->cck_crc_16_m, bb->bb_phy_idx);
		cck_fa->sig_gg_cnt = halbb_get_reg_cmn(bb, cr->cck_sig_gg_cnt, cr->cck_sig_gg_cnt_m, bb->bb_phy_idx);

		/* Number of spoofing*/
		cca->cnt_cck_spoofing = halbb_get_reg_cmn(bb, cr->cck_spoofing, cr->cck_spoofing_m, bb->bb_phy_idx);

		/* 52C CCK_FA = CCK_BRK*/
		fa->cnt_cck_fail = halbb_get_reg_cmn(bb, cr->cck_brk_cnt, cr->cck_brk_cnt_m, bb->bb_phy_idx);
	}
}

void halbb_pmac_ofdm_tx_cnt(struct bb_info *bb)
{
	struct bb_stat_info *stat_t = &bb->bb_stat_i;
	struct bb_tx_cnt_info *tx = &stat_t->bb_tx_cnt_i;
	struct bb_stat_cr_info *cr = &bb->bb_cmn_hooker->bb_stat_cr_i;
	enum phl_phy_idx phy_idx = bb->bb_phy_idx;
	u32 ret_value = 0;

	/* read Tx counter */
	ret_value = halbb_get_reg_cmn(bb, cr->ofdmtxon, MASKDWORD, phy_idx);
	tx->ofdm_phy_txon = ret_value & cr->ofdmtxon_m;
	tx->ofdm_mac_txen = (ret_value & cr->ofdmtxen_m) >> 16;
}

void halbb_pmac_ofdm_cnt(struct bb_info *bb)
{
	struct bb_stat_info *stat_t = &bb->bb_stat_i;
	struct bb_fa_info *fa = &stat_t->bb_fa_i;
	struct bb_legacy_fa_info *legacy_fa = &fa->bb_legacy_fa_i;
	struct bb_ht_fa_info *ht_fa = &fa->bb_ht_fa_i;
	struct bb_vht_fa_info *vht_fa = &fa->bb_vht_fa_i;
	struct bb_he_fa_info *he_fa = &fa->bb_he_fa_i;
	struct bb_cca_info *cca = &stat_t->bb_cca_i;
	struct bb_crc_info *crc = &stat_t->bb_crc_i;
	struct bb_crc2_info *crc2 = &stat_t->bb_crc2_i;
	struct bb_stat_cr_info *cr = &bb->bb_cmn_hooker->bb_stat_cr_i;
	struct bb_tx_cnt_info *tx = &stat_t->bb_tx_cnt_i;
	enum phl_phy_idx phy_idx = bb->bb_phy_idx;
	u32 ret_value = 0;

	/* read OFDM CRC32 counter */
	ret_value = halbb_get_reg_cmn(bb, cr->l_crc_ok, MASKDWORD, phy_idx);
	crc->cnt_ofdm_crc32_ok = ret_value & cr->l_crc_ok_m;
	crc->cnt_ofdm_crc32_error = (ret_value & cr->l_crc_err_m) >> 16;

	/* read OFDM2 CRC32 counter */
	ret_value = halbb_get_reg_cmn(bb, cr->l_crc_ok2, MASKDWORD, phy_idx);
	crc2->cnt_ofdm2_crc32_ok = ret_value & cr->l_crc_ok2_m;
	crc2->cnt_ofdm2_crc32_error = (ret_value & cr->l_crc_err2_m) >> 16;

	/* read OFDM3 CRC32 counter */
	ret_value = halbb_get_reg_cmn(bb, cr->l_crc_ok3, MASKDWORD, phy_idx);
	crc2->cnt_ofdm3_crc32_ok = ret_value & cr->l_crc_ok3_m;
	crc2->cnt_ofdm3_crc32_error = (ret_value & cr->l_crc_err3_m) >> 16;

	/* read HT CRC32 counter */
	ret_value = halbb_get_reg_cmn(bb, cr->ht_crc_ok, MASKDWORD, phy_idx);
	crc->cnt_ht_crc32_ok = ret_value & cr->ht_crc_ok_m;
	crc->cnt_ht_crc32_error = (ret_value & cr->ht_crc_err_m) >> 16;

	/* read HT2 CRC32 counter */
	ret_value = halbb_get_reg_cmn(bb, cr->ht_crc_ok2, MASKDWORD, phy_idx);
	crc2->cnt_ht2_crc32_ok = ret_value & cr->ht_crc_ok2_m;
	crc2->cnt_ht2_crc32_error = (ret_value & cr->ht_crc_err2_m) >> 16;

	/*read VHT CRC32 counter */
	ret_value = halbb_get_reg_cmn(bb, cr->vht_crc_ok, MASKDWORD, phy_idx);
	crc->cnt_vht_crc32_ok = ret_value & cr->vht_crc_ok_m;
	crc->cnt_vht_crc32_error = (ret_value & cr->vht_crc_err_m) >> 16;

	/*read VHT2 CRC32 counter */
	ret_value = halbb_get_reg_cmn(bb, cr->vht_crc_ok2, MASKDWORD, phy_idx);
	crc2->cnt_vht2_crc32_ok = ret_value & cr->vht_crc_ok2_m;
	crc2->cnt_vht2_crc32_error = (ret_value & cr->vht_crc_err2_m) >> 16;

	/*read HE CRC32 counter */
	ret_value = halbb_get_reg_cmn(bb, cr->he_crc_ok, MASKDWORD, phy_idx);
	crc->cnt_he_crc32_ok = ret_value & cr->he_crc_ok_m;
	crc->cnt_he_crc32_error = (ret_value & cr->he_crc_err_m) >> 16;

	/*read HE2 CRC32 counter */
	ret_value = halbb_get_reg_cmn(bb, cr->he_crc_ok2, MASKDWORD, phy_idx);
	crc2->cnt_he2_crc32_ok = ret_value & cr->he_crc_ok2_m;
	crc2->cnt_he2_crc32_error = (ret_value & cr->he_crc_err2_m) >> 16;

	/*read AMPDU CRC32 counter */
	ret_value = halbb_get_reg_cmn(bb, cr->ampdu_crc_ok, MASKDWORD, phy_idx);
	crc->cnt_ampdu_crc_ok = ret_value & cr->ampdu_crc_ok_m;
	crc->cnt_ampdu_crc_error = (ret_value & cr->ampdu_crc_err_m) >> 16;

#ifdef HALBB_COMPILE_BE0_SERIES
	if (bb->ic_type & BB_IC_BE_SERIES) {
		/*read EHT CRC32 counter */
		ret_value = halbb_get_reg_cmn(bb, cr->eht_crc_ok, MASKDWORD, phy_idx);
		crc->cnt_eht_crc32_ok = ret_value & cr->eht_crc_ok_m;
		crc->cnt_eht_crc32_error = (ret_value & cr->eht_crc_err_m) >> 16;

		/*read EHT2 CRC32 counter */
		ret_value = halbb_get_reg_cmn(bb, cr->eht_crc_ok2, MASKDWORD, phy_idx);
		crc2->cnt_eht2_crc32_ok = ret_value & cr->eht_crc_ok2_m;
		crc2->cnt_eht2_crc32_error = (ret_value & cr->eht_crc_err2_m) >> 16;
	}
#endif

	ret_value = halbb_get_reg_cmn(bb, cr->brk, cr->brk_m, phy_idx);
	/* After BB parameter v42, BB will trigger break to reset BB TX outer */
	/* BB FA counter plus 1 after every PHY_TXON JIRA: WLANBB-2954*/
	if ((bb->ic_type == BB_RTL8852C)
		&& (halbb_get_reg_cmn(bb, cr->break_option, BIT(31), phy_idx) == 1)) {
		if (ret_value > (tx->ofdm_phy_txon + tx->cck_phy_txon))
			ret_value -= (tx->ofdm_phy_txon + tx->cck_phy_txon);
		else
			ret_value = 0;
	}
	fa->cnt_total_brk = ret_value;

	/* Acut workaround because of no HE cnt */
	fa->cnt_ofdm_fail= ret_value;

	/* @calculate OFDM FA counter instead of reading brk_cnt*/
	ret_value = halbb_get_reg_cmn(bb, cr->search_fail, cr->search_fail_m, phy_idx);
	legacy_fa->cnt_sb_search_fail = ret_value;

	/* Legacy portion */
	ret_value = halbb_get_reg_cmn(bb, cr->lsig_brk_s_th, cr->lsig_brk_s_th_m, phy_idx);
	legacy_fa->cnt_lsig_brk_s_th = ret_value;

	ret_value = halbb_get_reg_cmn(bb, cr->lsig_brk_l_th, cr->lsig_brk_l_th_m, phy_idx);
	legacy_fa->cnt_lsig_brk_l_th = ret_value;

	ret_value = halbb_get_reg_cmn(bb, cr->rxl_err_parity, cr->rxl_err_parity_m, phy_idx);
	legacy_fa->cnt_parity_fail = ret_value;
	
	ret_value = halbb_get_reg_cmn(bb, cr->rxl_err_rate, cr->rxl_err_rate_m, phy_idx);
	legacy_fa->cnt_rate_illegal = ret_value;

	/* HT portion */
	ret_value = halbb_get_reg_cmn(bb, cr->ht_not_support_mcs, cr->ht_not_support_mcs_m, phy_idx);
	ht_fa->cnt_mcs_fail = ret_value;

	ret_value = halbb_get_reg_cmn(bb, cr->htsig_crc8_err_s_th, cr->htsig_crc8_err_s_th_m, phy_idx);
	ht_fa->cnt_crc8_fail_s_th = ret_value;

	ret_value = halbb_get_reg_cmn(bb, cr->htsig_crc8_err_l_th, cr->htsig_crc8_err_l_th_m, phy_idx);
	ht_fa->cnt_crc8_fail_l_th = ret_value;

	ht_fa->cnt_crc8_fail = ht_fa->cnt_crc8_fail_s_th + ht_fa->cnt_crc8_fail_l_th;

	/* VHT portion */
	ret_value = halbb_get_reg_cmn(bb, cr->vht_not_support_mcs, cr->vht_not_support_mcs_m, phy_idx);
	vht_fa->cnt_mcs_fail_vht = ret_value;

	ret_value = halbb_get_reg_cmn(bb, cr->vht_err_siga_crc8, cr->vht_err_siga_crc8_m, phy_idx);
	vht_fa->cnt_crc8_fail_vhta = ret_value;

	/* read OFDM CCA counter */
	ret_value = halbb_get_reg_cmn(bb, cr->ofdm_cca, cr->ofdm_cca_m, phy_idx);
	cca->cnt_ofdm_cca = ret_value;
	ret_value = halbb_get_reg_cmn(bb, cr->cca_spoofing, cr->cca_spoofing_m, phy_idx);
	cca->cnt_ofdm_spoofing = ret_value;
	ret_value = halbb_get_reg_cmn(bb, cr->ampdu_miss, cr->ampdu_miss_m, phy_idx);
	crc->cnt_ampdu_miss = ret_value;
	/* POP counter */
	ret_value = halbb_get_reg_cmn(bb, cr->cnt_pop_trig, cr->cnt_pop_trig_m, phy_idx);
	cca->pop_cnt = ret_value;
}

void halbb_pmac_print_cnt(struct bb_info *bb, bool cck_en)
{
	struct bb_stat_info *stat_t = &bb->bb_stat_i;
	struct bb_fa_info *fa = &stat_t->bb_fa_i;
	struct bb_cck_fa_info *cck_fa = &fa->bb_cck_fa_i;
	struct bb_legacy_fa_info *legacy_fa = &fa->bb_legacy_fa_i;
	struct bb_ht_fa_info *ht_fa = &fa->bb_ht_fa_i;
	struct bb_vht_fa_info *vht_fa = &fa->bb_vht_fa_i;
	struct bb_he_fa_info *he_fa = &fa->bb_he_fa_i;
	struct bb_cca_info *cca = &stat_t->bb_cca_i;
	struct bb_crc_info *crc = &stat_t->bb_crc_i;
	struct bb_crc2_info *crc2 = &stat_t->bb_crc2_i;
	struct bb_tx_cnt_info *tx = &stat_t->bb_tx_cnt_i;
	struct rtw_hal_stat_info *stat_info = &bb->hal_com->band[bb->bb_phy_idx].stat_info;

	if (!cck_en) {
		fa->cnt_fail_all = fa->cnt_ofdm_fail;
		cca->cnt_cca_all = cca->cnt_ofdm_cca;
	} else {
		fa->cnt_fail_all = fa->cnt_ofdm_fail + fa->cnt_cck_fail;
		cca->cnt_cca_all = cca->cnt_cck_cca + cca->cnt_ofdm_cca;
	}

	crc->cnt_crc32_error_all = crc->cnt_eht_crc32_error +
				   crc->cnt_he_crc32_error +
				   crc->cnt_vht_crc32_error +
				   crc->cnt_ht_crc32_error +
				   crc->cnt_ofdm_crc32_error +
				   crc->cnt_cck_crc32_error;

	crc->cnt_crc32_ok_all = crc->cnt_eht_crc32_ok +
				   crc->cnt_he_crc32_ok +
				   crc->cnt_vht_crc32_ok +
				   crc->cnt_ht_crc32_ok +
				   crc->cnt_ofdm_crc32_ok +
				   crc->cnt_cck_crc32_ok;

	stat_info->cnt_fail_all = fa->cnt_fail_all;
	stat_info->cnt_cck_fail = fa->cnt_cck_fail;
	stat_info->cnt_ofdm_fail = fa->cnt_ofdm_fail;
	stat_info->cnt_cca_all = cca->cnt_cca_all;
	stat_info->cnt_ofdm_cca = cca->cnt_ofdm_cca;
	stat_info->cnt_cck_cca = cca->cnt_cck_cca;
	stat_info->cnt_crc32_error_all = crc->cnt_crc32_error_all;
	stat_info->cnt_he_crc32_error = crc->cnt_he_crc32_error;
	stat_info->cnt_vht_crc32_error = crc->cnt_vht_crc32_error;
	stat_info->cnt_ht_crc32_error = crc->cnt_ht_crc32_error;
	stat_info->cnt_ofdm_crc32_error = crc->cnt_ofdm_crc32_error;
	stat_info->cnt_cck_crc32_error = crc->cnt_cck_crc32_error;
	stat_info->cnt_crc32_ok_all = crc->cnt_crc32_ok_all;
	stat_info->cnt_he_crc32_ok = crc->cnt_he_crc32_ok;
	stat_info->cnt_vht_crc32_ok = crc->cnt_vht_crc32_ok;
	stat_info->cnt_ht_crc32_ok = crc->cnt_ht_crc32_ok;
	stat_info->cnt_ofdm_crc32_ok = crc->cnt_ofdm_crc32_ok;
	stat_info->cnt_cck_crc32_ok = crc->cnt_cck_crc32_ok;

	if (bb->bb_dig_i.p_cur_dig_unit)
		stat_info->igi_fa_rssi = bb->bb_dig_i.p_cur_dig_unit->igi_fa_rssi;

	BB_DBG(bb, DBG_FA_CNT, "[TX]\n");
	BB_DBG(bb, DBG_FA_CNT,
	       " *[Tx Cnt] {CCK_TXEN/TXON, OFDM_TXEN/TXON}: {%d/%d, %d/%d}\n",
	       tx->cck_mac_txen, tx->cck_phy_txon, tx->ofdm_mac_txen,
	       tx->ofdm_phy_txon);

	BB_DBG(bb, DBG_FA_CNT, "[Rx-CCA]\n");
	BB_DBG(bb, DBG_FA_CNT,
		  " *[CCA Cnt] {CCK + OFDM = Total} = {%d + %d = %d}\n",
		  cca->cnt_cck_cca, cca->cnt_ofdm_cca, cca->cnt_cca_all);
	BB_DBG(bb, DBG_FA_CNT,
		  " *[CCA Spoofing Cnt] {CCK, OFDM} = {%d, %d}\n",
		  cca->cnt_cck_spoofing, cca->cnt_ofdm_spoofing);
	BB_DBG(bb, DBG_FA_CNT,
		  " *[AMPDU Miss] = {%d}\n", crc->cnt_ampdu_miss);

	halbb_print_devider(bb, BB_DEVIDER_LEN_16, false, DBG_FA_CNT);

	BB_DBG(bb, DBG_FA_CNT, "[Rx-FA]\n");
	BB_DBG(bb, DBG_FA_CNT,
		  " *[Total HW Brk Cnt] = {%d}\n", fa->cnt_total_brk);
	BB_DBG(bb, DBG_FA_CNT,
		  " *[ALL] {CCK, OFDM, Total} = {%d, %d, %d}\n",
		  fa->cnt_cck_fail, fa->cnt_ofdm_fail, fa->cnt_fail_all);
	BB_DBG(bb, DBG_FA_CNT,
		  " *[CCK] SFD_err=%d, SIG_err=%d CRC16=%d\n",
		  cck_fa->sfd_gg_cnt, cck_fa->sig_gg_cnt, cck_fa->cnt_cck_crc_16);
	BB_DBG(bb, DBG_FA_CNT,
		  " *[OFDM] Parity_err=%d, Rate=%d, LSIG_brk_s=%d, LSIG_brk_l=%d, SBD=%d\n",
		  legacy_fa->cnt_parity_fail, legacy_fa->cnt_rate_illegal,
		  legacy_fa->cnt_lsig_brk_s_th, legacy_fa->cnt_lsig_brk_l_th,
		  legacy_fa->cnt_sb_search_fail);
	BB_DBG(bb, DBG_FA_CNT, " *[HT]   CRC8=%d, MCS=%d\n",
		  ht_fa->cnt_crc8_fail, ht_fa->cnt_mcs_fail);
	BB_DBG(bb, DBG_FA_CNT,
		  " *[VHT]  SIGA_CRC8=%d, MCS=%d\n",
		  vht_fa->cnt_crc8_fail_vhta, vht_fa->cnt_mcs_fail_vht);

	halbb_print_devider(bb, BB_DEVIDER_LEN_16, false, DBG_FA_CNT);

	BB_DBG(bb, DBG_FA_CNT, "[CRC Cnt]\n");
	BB_DBG(bb, DBG_FA_CNT,
		  " *[CRC32 OK]  {CCK, OFDM, HT, VHT, HE, EHT, Total} = {%d, %d, %d, %d, %d, %d, %d}\n",
		  crc->cnt_cck_crc32_ok, crc->cnt_ofdm_crc32_ok,
		  crc->cnt_ht_crc32_ok, crc->cnt_vht_crc32_ok,
		  crc->cnt_he_crc32_ok, crc->cnt_eht_crc32_ok,
		  crc->cnt_crc32_ok_all);
	BB_DBG(bb, DBG_FA_CNT,
		  " *[CRC32 Err] {CCK, OFDM, HT, VHT, HE, EHT, Total} = {%d, %d, %d, %d, %d, %d, %d}\n",
		  crc->cnt_cck_crc32_error, crc->cnt_ofdm_crc32_error,
		  crc->cnt_ht_crc32_error, crc->cnt_vht_crc32_error,
		  crc->cnt_he_crc32_error, crc->cnt_eht_crc32_error,
		  crc->cnt_crc32_error_all);

	halbb_print_devider(bb, BB_DEVIDER_LEN_16, false, DBG_FA_CNT);

	BB_DBG(bb, DBG_FA_CNT, "[HALBB DM status]\n");
	BB_DBG(bb, DBG_FA_CNT, " *[DIG] IGI=%d\n", stat_info->igi_fa_rssi);
}

void halbb_pmac_print_cnt2(struct bb_info *bb)
{
	struct bb_stat_info *stat_t = &bb->bb_stat_i;
	struct bb_crc2_info *crc2 = &stat_t->bb_crc2_i;
	struct bb_usr_set_info *usr_set = &stat_t->bb_usr_set_i;
	u32 tmp = 0;

	halbb_print_devider(bb, BB_DEVIDER_LEN_16, false, DBG_FA_CNT);
	BB_DBG(bb, DBG_FA_CNT, "[Low Rate CRC32 Cnt]\n");
	if (usr_set->ofdm2_rate_idx) {
		tmp = crc2->cnt_ofdm2_crc32_error + crc2->cnt_ofdm2_crc32_ok;
		crc2->ofdm2_pcr = (u8)HALBB_DIV(crc2->cnt_ofdm2_crc32_ok * 100,
						tmp);
		halbb_print_rate_2_buff(bb, usr_set->ofdm2_rate_idx, RTW_GILTF_LGI_4XHE32, bb->dbg_buf,
					HALBB_SNPRINT_SIZE);
		BB_DBG(bb, DBG_FA_CNT,
			  " *[OFDM:%s CRC32 Cnt] {err, ok}= {%d, %d} (PCR=%d percent)\n",
			  bb->dbg_buf, crc2->cnt_ofdm2_crc32_error,
			  crc2->cnt_ofdm2_crc32_ok, crc2->ofdm2_pcr);
	}
	if (usr_set->ht2_rate_idx) {
		tmp = crc2->cnt_ht2_crc32_error + crc2->cnt_ht2_crc32_ok;
		crc2->ht2_pcr = (u8)HALBB_DIV(crc2->cnt_ht2_crc32_ok * 100,
					      tmp);
		halbb_print_rate_2_buff(bb, usr_set->ht2_rate_idx, RTW_GILTF_LGI_4XHE32, bb->dbg_buf,
					HALBB_SNPRINT_SIZE);
		BB_DBG(bb, DBG_FA_CNT,
			  " *[HT:%s CRC32 Cnt] {err, ok}= {%d, %d} (PCR=%d percent)\n",
			  bb->dbg_buf, crc2->cnt_ht2_crc32_error,
			  crc2->cnt_ht2_crc32_ok, crc2->ht2_pcr);
	}
	if(usr_set->vht2_rate_idx) {
		tmp = crc2->cnt_vht2_crc32_error +
		      crc2->cnt_vht2_crc32_ok;
		crc2->vht2_pcr = (u8)HALBB_DIV(crc2->cnt_vht2_crc32_ok *
					       100, tmp);
		halbb_print_rate_2_buff(bb, usr_set->vht2_rate_idx,
					RTW_GILTF_LGI_4XHE32, bb->dbg_buf, HALBB_SNPRINT_SIZE);
		BB_DBG(bb, DBG_FA_CNT,
			  " *[VHT:%s CRC32 Cnt] {err, ok}= {%d, %d} (PCR=%d percent)\n",
			  bb->dbg_buf, crc2->cnt_vht2_crc32_error,
			  crc2->cnt_vht2_crc32_ok, crc2->vht2_pcr);
	}
	if (usr_set->he2_rate_idx) {
		tmp = crc2->cnt_he2_crc32_error +
		      crc2->cnt_he2_crc32_ok;
		crc2->he2_pcr = (u8)HALBB_DIV(crc2->cnt_he2_crc32_ok *
					       100, tmp);
		halbb_print_rate_2_buff(bb, usr_set->he2_rate_idx,
					RTW_GILTF_LGI_4XHE32, bb->dbg_buf, HALBB_SNPRINT_SIZE);
		BB_DBG(bb, DBG_FA_CNT,
			  " *[HE:%s CRC32 Cnt] {err, ok}= {%d, %d} (PCR=%d percent)\n",
			  bb->dbg_buf, crc2->cnt_he2_crc32_error,
			  crc2->cnt_he2_crc32_ok, crc2->he2_pcr);
	}

#ifdef BB_1115_DVLP_SPF
	if (bb->ic_type & BB_IC_BE_SERIES) {
		if (usr_set->eht2_rate_idx) {
			tmp = crc2->cnt_eht2_crc32_error +
			      crc2->cnt_eht2_crc32_ok;
			crc2->eht2_pcr = (u8)HALBB_DIV(crc2->cnt_eht2_crc32_ok *
						       100, tmp);
			halbb_print_rate_2_buff(bb, usr_set->eht2_rate_idx,
						RTW_GILTF_LGI_4XHE32, bb->dbg_buf, HALBB_SNPRINT_SIZE);
			BB_DBG(bb, DBG_FA_CNT,
				  " *[EHT:%s CRC32 Cnt] {err, ok}= {%d, %d} (PCR=%d percent)\n",
				  bb->dbg_buf, crc2->cnt_eht2_crc32_error,
				  crc2->cnt_eht2_crc32_ok, crc2->eht2_pcr);
		}
	}
#endif
}

void halbb_pmac_print_cnt3(struct bb_info *bb)
{
	struct bb_stat_info *stat_t = &bb->bb_stat_i;
	struct bb_fa_info *fa = &stat_t->bb_fa_i;
	struct bb_cca_info *cca = &stat_t->bb_cca_i;
	struct bb_crc_info *crc = &stat_t->bb_crc_i;
	struct bb_usr_set_info *usr_set = &stat_t->bb_usr_set_i;
	struct bb_crc2_info *crc2 = &stat_t->bb_crc2_i;
	u32 tmp = 0;
	u8 pcr = 0;

	tmp = crc2->cnt_ofdm3_crc32_ok + crc2->cnt_ofdm3_crc32_error;
	pcr = (u8)HALBB_DIV(crc2->cnt_ofdm3_crc32_ok * 100, tmp);
	
	switch(usr_set->stat_type_sel_i) {
	case STATE_PROBE_RESP:
		BB_DBG(bb, DBG_FA_CNT,
		  "[Probe Response Data CRC32 Cnt] {error, ok}= {%d, %d} (PCR=%d percent)\n",
		  crc2->cnt_ofdm3_crc32_error,
		  crc2->cnt_ofdm3_crc32_ok, pcr);
		break;
	case STATE_BEACON:
		BB_DBG(bb, DBG_FA_CNT,
		  "[Beacon CRC32 Cnt] {error, ok}= {%d, %d} (PCR=%d percent)\n",
		  crc2->cnt_ofdm3_crc32_error,
		  crc2->cnt_ofdm3_crc32_ok, pcr);
		break;
	case STATE_ACTION:
		BB_DBG(bb, DBG_FA_CNT,
		  "[Action CRC32 Cnt] {error, ok}= {%d, %d} (PCR=%d percent)\n",
		  crc2->cnt_ofdm3_crc32_error,
		  crc2->cnt_ofdm3_crc32_ok, pcr);
		break;
	case STATE_BFRP:
		BB_DBG(bb, DBG_FA_CNT,
		  "[BFRP CRC32 Cnt] {error, ok}= {%d, %d} (PCR=%d percent)\n",
		  crc2->cnt_ofdm3_crc32_error,
		  crc2->cnt_ofdm3_crc32_ok, pcr);
		break;
	case STATE_NDPA:
		BB_DBG(bb, DBG_FA_CNT,
		  "[NDPA CRC32 Cnt] {error, ok}= {%d, %d} (PCR=%d percent)\n",
		  crc2->cnt_ofdm3_crc32_error,
		  crc2->cnt_ofdm3_crc32_ok, pcr);
		break;
	case STATE_BA:
		BB_DBG(bb, DBG_FA_CNT,
		  "[BA CRC32 Cnt] {error, ok}= {%d, %d} (PCR=%d percent)\n",
		  crc2->cnt_ofdm3_crc32_error,
		  crc2->cnt_ofdm3_crc32_ok, pcr);
		break;
	case STATE_RTS:
		BB_DBG(bb, DBG_FA_CNT,
		  "[RTS CRC32 Cnt] {error, ok}= {%d, %d} (PCR=%d percent)\n",
		  crc2->cnt_ofdm3_crc32_error,
		  crc2->cnt_ofdm3_crc32_ok, pcr);
		break;
	case STATE_CTS:
		BB_DBG(bb, DBG_FA_CNT,
		  "[CTS CRC32 Cnt] {error, ok}= {%d, %d} (PCR=%d percent)\n",
		  crc2->cnt_ofdm3_crc32_error,
		  crc2->cnt_ofdm3_crc32_ok, pcr);
		break;
	case STATE_ACK:
		BB_DBG(bb, DBG_FA_CNT,
		  "[ACK CRC32 Cnt] {error, ok}= {%d, %d} (PCR=%d percent)\n",
		  crc2->cnt_ofdm3_crc32_error,
		  crc2->cnt_ofdm3_crc32_ok, pcr);
		break;
	case STATE_DATA:
		BB_DBG(bb, DBG_FA_CNT,
		  "[DATA CRC32 Cnt] {error, ok}= {%d, %d} (PCR=%d percent)\n",
		  crc2->cnt_ofdm3_crc32_error,
		  crc2->cnt_ofdm3_crc32_ok, pcr);
		break;
	case STATE_NULL:
		BB_DBG(bb, DBG_FA_CNT,
		  "[Null CRC32 Cnt] {error, ok}= {%d, %d} (PCR=%d percent)\n",
		  crc2->cnt_ofdm3_crc32_error,
		  crc2->cnt_ofdm3_crc32_ok, pcr);
		break;
	case STATE_QOS:
		BB_DBG(bb, DBG_FA_CNT,
		  "[QoS CRC32 Cnt] {error, ok}= {%d, %d} (PCR=%d percent)\n",
		  crc2->cnt_ofdm3_crc32_error,
		  crc2->cnt_ofdm3_crc32_ok, pcr);
		break;
	default:
		break;
	}

}

void halbb_pmac_cnt_reg_reset(struct bb_info *bb, bool cck_en)
{
	struct bb_stat_cr_info *cr = &bb->bb_cmn_hooker->bb_stat_cr_i;

	if (cck_en) {
		/* @reset CCK FA counter */
		halbb_set_reg(bb, cr->r1b_rx_rpt_rst, cr->r1b_rx_rpt_rst_m, 0);
		halbb_set_reg(bb, cr->r1b_rx_rpt_rst, cr->r1b_rx_rpt_rst_m, 1);
	}

	/* @make sure cnt is enable */
	halbb_set_reg_cmn(bb, cr->enable_all_cnt, cr->enable_all_cnt_m, 1, bb->bb_phy_idx);

	/* @reset all bb hw cnt */
	halbb_mp_cnt_reset(bb);
}

void halbb_pmac_statistics_pause_val(struct bb_info *bb, u32 *val_buf, u8 val_len)
{
	struct bb_env_mntr_info *env = &bb->bb_env_mntr_i;
	u32 tmp_val = 0;

	if (val_len != 1) {
		BB_DBG(bb, DBG_FA_CNT, "[Error][Statistics]Need val_len=1\n");
		return;
	}

	tmp_val = val_buf[0]; /*Just prevent compile warning*/

	BB_DBG(bb, DBG_FA_CNT, "[%s]\n", __func__);
}

void halbb_pmac_statistics_io_en(struct bb_info *bb)
{
	enum phl_phy_idx phy_idx = bb->bb_phy_idx;
	u8 fc = bb->hal_com->band[phy_idx].cur_chandef.center_ch;
	bool cck_en = false;

	if (fc == 0) {
		BB_WARNING("[%s] phy=%d, fc=%d\n", __func__, phy_idx, fc);
		return;
	}

	if (fc <= 14)
		cck_en = true;

	BB_DBG(bb, DBG_FA_CNT, "[%s] phy=%d, cck_en=%d, fc=%d\n",
	       __func__, phy_idx, cck_en, fc);

	if (cck_en) {
		halbb_pmac_cck_tx_cnt(bb);
		halbb_pmac_cck_cnt(bb);
	}

	halbb_pmac_ofdm_tx_cnt(bb);
	halbb_pmac_ofdm_cnt(bb);

	halbb_pmac_print_cnt(bb, cck_en);
	halbb_pmac_print_cnt2(bb);
	halbb_pmac_print_cnt3(bb);

	#ifdef HALBB_AUTO_DBG_SUPPORT
	halbb_auto_debug_pmac_cnt_chk(bb);
	#endif

	if (bb->bb_stat_i.cnt_reset_en)
		halbb_pmac_cnt_reg_reset(bb, cck_en);
}

bool halbb_pmac_statistics_abort(struct bb_info *bb)
{
	if (!(bb->support_ability & BB_FA_CNT)) {
		BB_DBG(bb, DBG_ENV_MNTR, "[%s] Pmac_statistics Not support\n", __func__);
		return true;
	}

	if (bb->pause_ability & BB_FA_CNT) {
		BB_DBG(bb, DBG_FA_CNT, "[%s]: Pause FA_cnt in LV=%d\n",
		       __func__, bb->pause_lv_table.lv_fa_cnt);
		return true;
	}

	return false;
}

void halbb_pmac_statistics(struct bb_info *bb)
{
	enum phl_phy_idx phy_idx = bb->bb_phy_idx;
	u8 fc = bb->hal_com->band[phy_idx].cur_chandef.center_ch;
	bool cck_en = false;

	halbb_show_cr_cnt(bb, BB_WD_PMAC_STATISTICS);

	if (halbb_pmac_statistics_abort(bb))
		return;

	halbb_pmac_statistics_io_en(bb);
}

void halbb_pmac_statistics_ex(struct bb_info *bb_0, bool en, enum phl_phy_idx phy_idx)
{
	/*export PMAC-relative for outer driver debug*/
	struct bb_info *bb = bb_0;
	enum halbb_pause_type pause_type;
	u32 val = 0;
	u8 rpt;

#ifdef HALBB_DBCC_SUPPORT
	HALBB_GET_PHY_PTR(bb_0, bb, phy_idx);
#endif

	BB_DBG(bb, DBG_COMMON_FLOW, "[%s] phy_idx=%d, en=%d\n",
	       __func__, bb->bb_phy_idx, en);

	if (en) {
		pause_type = HALBB_PAUSE_NO_SET;
		bb->dbg_component |= DBG_FA_CNT;
	} else {
		pause_type = HALBB_RESUME_NO_RECOVERY;
		bb->dbg_component &= ~DBG_FA_CNT;
	}
		
	rpt = halbb_pause_func(bb, F_FA_CNT, pause_type, HALBB_PAUSE_LV_2, 2, &val, phy_idx);

	halbb_pmac_statistics_io_en(bb);
}

#endif
