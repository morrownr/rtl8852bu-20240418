ifeq ($(CONFIG_PLATFORM_RTL8198D), y)
ccflags-y += -DCONFIG_BIG_ENDIAN -DCONFIG_PLATFORM_RTL8198D
ccflags-y += -DCONFIG_IOCTL_CFG80211 -DRTW_USE_CFG80211_STA_EVENT
export DIR_LINUX=$(shell pwd)/../../../../../../linux-4.4.x
ARCH ?= mips
CROSS_COMPILE := /toolchain/msdk-4.8.5-mips-EB-4.4-u0.9.33-m32ut-180206/bin/msdk-linux- 
KSRC := $(DIR_LINUX)
MODULE_NAME := $(MODULE_NAME)_98d
ifeq ($(CONFIG_PCI_HCI), y)
ccflags-y += -DCONFIG_PLATFORM_OPS
_PLATFORM_FILES := platform/platform_mips_98d_pci.o
OBJS += $(_PLATFORM_FILES)
endif
endif
