/*
 * Copyright (c) 2018-2019, NXP
 * Copyright (c) 2019 PHYTEC Messtechnik GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT nxp_mcux_usbd


#include "usb_dc_mcux.h"

#if defined(CONFIG_USB_DC_NXP_EHCI)

#if DT_NODE_HAS_STATUS(DT_NODELABEL(usb1), okay) && CONFIG_USB_DC_NXP_EHCI
#define CONTROLLER_ID kUSB_ControllerEhci0
#elif DT_NODE_HAS_STATUS(DT_NODELABEL(usb2), okay) && CONFIG_USB_DC_NXP_EHCI
#define CONTROLLER_ID kUSB_ControllerEhci1
#endif

#endif

int usb_dc_set_address(const uint8_t addr)
{
	return 0;
}

int usb_dc_ep_check_cap(const struct usb_dc_ep_cfg_data *const cfg)
{
	return 0;
}


int usb_dc_ep_configure(const struct usb_dc_ep_cfg_data *const cfg)
{
	return 0;
}

int usb_dc_ep_set_stall(const uint8_t ep)
{
	return 0;
}

int usb_dc_ep_clear_stall(const uint8_t ep)
{
	return 0;
}

int usb_dc_ep_is_stalled(const uint8_t ep, uint8_t *const stalled)
{
	return 0;
}

int usb_dc_ep_halt(const uint8_t ep)
{
	return 0;
}

int usb_dc_ep_enable(const uint8_t ep)
{
	return 0;
}

int usb_dc_ep_disable(const uint8_t ep)
{
	return 0;
}

int usb_dc_ep_flush(const uint8_t ep)
{
	return 0;
}

int usb_dc_ep_write(const uint8_t ep, const uint8_t *const data,
		    const uint32_t data_len, uint32_t *const ret_bytes)
{
	return 0;
}


int usb_dc_ep_read_wait(uint8_t ep, uint8_t *data, uint32_t max_data_len,
			uint32_t *read_bytes)
{
	return 0;
}

int usb_dc_ep_read_continue(uint8_t ep)
{
	return 0;
}

int usb_dc_ep_read(const uint8_t ep, uint8_t *const data,
		   const uint32_t max_data_len, uint32_t *const read_bytes)
{
	return 0;
}



int usb_dc_ep_mps(const uint8_t ep)
{
	return 0;
}



