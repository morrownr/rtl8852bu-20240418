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
#ifndef __PCI_OPS_H_
#define __PCI_OPS_H_

#ifdef RTK_129X_PLATFORM
#define PCIE_SLOT1_MEM_START	0x9804F000
#define PCIE_SLOT1_MEM_LEN	0x1000
#define PCIE_SLOT1_CTRL_START	0x9804EC00

#define PCIE_SLOT2_MEM_START	0x9803C000
#define PCIE_SLOT2_MEM_LEN	0x1000
#define PCIE_SLOT2_CTRL_START	0x9803BC00

#define PCIE_MASK_OFFSET	0x100 /* mask offset from CTRL_START */
#define PCIE_TRANSLATE_OFFSET	0x104 /* translate offset from CTRL_START */
#endif

#define PCI_BC_CLK_REQ		BIT0
#define PCI_BC_ASPM_L0s		BIT1
#define PCI_BC_ASPM_L1		BIT2
#define PCI_BC_ASPM_L1Off	BIT3
//#define PCI_BC_ASPM_LTR	BIT4
//#define PCI_BC_ASPM_OBFF	BIT5

void	PlatformClearPciPMEStatus(_adapter *adapter);

#ifdef CONFIG_64BIT_DMA
u8 PlatformEnableDMA64(struct pci_dev *pdev);
#endif
#ifdef CONFIG_PCI_DYNAMIC_ASPM
void rtw_pci_set_aspm_lnkctl(_adapter *padapter, u8 mode);
void rtw_pci_set_l1_latency(_adapter *padapter, u8 mode);

static inline void rtw_pci_dynamic_aspm_set_mode(_adapter *padapter, u8 mode)
{
	struct dvobj_priv *pdvobjpriv = adapter_to_dvobj(padapter);
	struct pci_priv	*pcipriv = &(pdvobjpriv->pcipriv);

	if (mode == pcipriv->aspm_mode)
		return;

	pcipriv->aspm_mode = mode;

#ifdef CONFIG_PCI_DYNAMIC_ASPM_LINK_CTRL
	rtw_pci_set_aspm_lnkctl(padapter, mode);
#endif
#ifdef CONFIG_PCI_DYNAMIC_ASPM_L1_LATENCY
	rtw_pci_set_l1_latency(padapter, mode);
#endif
}
#else
#define rtw_pci_dynamic_aspm_set_mode(adapter, mode)
#endif


#endif /*__PCI_OPS_H_*/
