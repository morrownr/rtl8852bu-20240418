ifeq ($(CONFIG_RTL8852B), y)
ccflags-y += -DCONFIG_RTL8852B
endif

ifeq ($(CONFIG_RTL8852BP), y)
ccflags-y += -DCONFIG_RTL8852BP
endif

ifeq ($(CONFIG_RTL8852BPT), y)
ccflags-y += -DCONFIG_RTL8852BPT
endif

ifeq ($(CONFIG_RTL8852BT), y)
ccflags-y += -DCONFIG_RTL8852BT
endif

IC_NAME := rtl8852b

ifeq ($(CONFIG_MP_INCLUDED), y)
### 8852B Default Enable VHT MP HW TX MODE ###
#ccflags-y += -DCONFIG_MP_VHT_HW_TX_MODE
#CONFIG_MP_VHT_HW_TX_MODE = y
endif

ifeq ($(CONFIG_PHL_ARCH), y)
HAL = phl/hal_g6
else
HAL = hal_g6
endif

ifeq ($(CONFIG_USB_HCI), y)
	FILE_NAME = rtl8852bu
endif
ifeq ($(CONFIG_PCI_HCI), y)
	FILE_NAME = rtl8852be
endif
ifeq ($(CONFIG_SDIO_HCI), y)
	FILE_NAME = rtl8852bs
endif


_HAL_IC_FILES +=	$(HAL)/$(IC_NAME)/$(IC_NAME)_halinit.o \
			$(HAL)/$(IC_NAME)/$(IC_NAME)_mac.o \
			$(HAL)/$(IC_NAME)/$(IC_NAME)_cmd.o \
			$(HAL)/$(IC_NAME)/$(IC_NAME)_phy.o \
			$(HAL)/$(IC_NAME)/$(IC_NAME)_ops.o \
			$(HAL)/$(IC_NAME)/$(IC_NAME)_regulation.o \
			$(HAL)/$(IC_NAME)/hal_trx_8852b.o

_HAL_IC_FILES +=	$(HAL)/$(IC_NAME)/$(HCI_NAME)/$(FILE_NAME)_halinit.o \
			$(HAL)/$(IC_NAME)/$(HCI_NAME)/$(FILE_NAME)_halmac.o \
			$(HAL)/$(IC_NAME)/$(HCI_NAME)/$(FILE_NAME)_io.o \
			$(HAL)/$(IC_NAME)/$(HCI_NAME)/$(FILE_NAME)_led.o \
			$(HAL)/$(IC_NAME)/$(HCI_NAME)/$(FILE_NAME)_ops.o

#ifeq ($(CONFIG_SDIO_HCI), y)
#_HAL_IC_FILES += $(HAL)/$(IC_NAME)/$(HCI_NAME)/hal_trx_8852bs.o
#endif

ifeq ($(CONFIG_USB_HCI), y)
_HAL_IC_FILES += $(HAL)/$(IC_NAME)/$(HCI_NAME)/hal_trx_8852bu.o
endif

ifeq ($(CONFIG_PCI_HCI), y)
_HAL_IC_FILES += $(HAL)/$(IC_NAME)/$(HCI_NAME)/hal_trx_8852be.o
endif
