/******************************************************************************
 *
 * Copyright(c) 2022 Realtek Corporation.
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
#ifndef _HAL_FEATURE_8852B_H_
#define _HAL_FEATURE_8852B_H_

#if defined(PHL_PLATFORM_LINUX)
	#ifdef CONFIG_PHL_CHANNEL_INFO
		#ifndef MAC_FW_CATEGORY_NICCE
			#error "fw version unexpected"
		#endif
	#endif
#endif

#endif /*_HAL_FEATURE_8852B_H_*/
