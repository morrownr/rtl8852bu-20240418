/** @file */
/******************************************************************************
 *
 * Copyright(c) 2019 Realtek Corporation. All rights reserved.
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
 ******************************************************************************/

#ifndef _MAC_AX_INIT_8852B_H_
#define _MAC_AX_INIT_8852B_H_

#include "../../type.h"
#if MAC_AX_8852B_SUPPORT

struct mac_ax_adapter *get_mac_8852b_adapter(enum mac_ax_intf intf,
					     u8 cv, void *drv_adapter,
					     struct mac_ax_pltfm_cb *pltfm_cb);

u32 dmac_func_en_8852b(struct mac_ax_adapter *adapter);

u32 dmac_func_pre_en_8852b(struct mac_ax_adapter *adapter);

#endif /* #if MAC_AX_8852B_SUPPORT */
#endif
