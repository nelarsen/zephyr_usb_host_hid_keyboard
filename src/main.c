#include <stdio.h>
#include <string.h>
#include <zephyr/zephyr.h>
 
#include "usb_host_config.h" 
#include "usb_host.h"
#include "host_keyboard.h"
#include "app.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(cdc_acm_nils, LOG_LEVEL_INF);


/*! @brief USB host keyboard instance global variable */
extern usb_host_keyboard_instance_t g_HostHidKeyboard;
usb_host_handle g_HostHandle;


void USB_OTG1_IRQHandler(void)
{
    USB_HostEhciIsrFunction(g_HostHandle);
}

void USB_HostIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbHOSTEhciIrq[] = USBHS_IRQS;
    irqNumber                = usbHOSTEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];
/* USB_HOST_CONFIG_EHCI */

/* Install isr, set priority, and enable IRQ. */
#if defined(__GIC_PRIO_BITS)
    GIC_SetPriority((IRQn_Type)irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#else
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#endif
    EnableIRQ((IRQn_Type)irqNumber);
}

void USB_HostTaskFn(void *param)
{
    USB_HostEhciTaskFunction(param);
}

/*!
 * @brief USB isr function.
 */

static usb_status_t USB_HostEvent(usb_device_handle deviceHandle,
                                  usb_host_configuration_handle configurationHandle,
                                  uint32_t eventCode)
{
    usb_status_t status = kStatus_USB_Success;

    switch (eventCode & 0x0000FFFFU)
    {
        case kUSB_HostEventAttach:
            status = USB_HostHidKeyboardEvent(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventNotSupported:
            usb_echo("device not supported.");
            break;

        case kUSB_HostEventEnumerationDone:
            status = USB_HostHidKeyboardEvent(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventDetach:
            status = USB_HostHidKeyboardEvent(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventEnumerationFail:
            usb_echo("enumeration failed 0x%x", eventCode);
            break;

        default:
            break;
    }
    return status;
}


static void USB_HostApplicationInit(void)
{
    uint32_t usbHostVersion;
    usb_status_t status = kStatus_USB_Success;

    // USB Clock is initialized in soc_rt11xx.c of Zephyr, don't do anything here
    //USB_HostClockInit();

#if ((defined FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT))
    SYSMPU_Enable(SYSMPU, 0);
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

    status = USB_HostInit(CONTROLLER_ID, &g_HostHandle, USB_HostEvent);
    if (status != kStatus_USB_Success)
    {
        LOG_ERR("host init error");
        return;
    }
    
    IRQ_CONNECT(DT_IRQ_BY_NAME(DT_NODELABEL(usb1), usb_otg, irq), DT_IRQ_BY_NAME(DT_NODELABEL(usb1), usb_otg, priority), USB_OTG1_IRQHandler, 0, 0);
    
    USB_HostIsrEnable();
    USB_HostGetVersion(&usbHostVersion);
    LOG_INF("host init done, the host stack version is %d.%d.%d.", ((uint8_t)(usbHostVersion >> 16)),
             ((uint8_t)(usbHostVersion >> 8)), ((uint8_t)(usbHostVersion)));

}


void main(void)
{  

    LOG_WRN("Disabling L1 cache completly (quick fix - bad for performance)");
    // Please see https://community.nxp.com/t5/i-MX-RT-Knowledge-Base/Using-NonCached-Memory-on-i-MXRT/ta-p/1183369 for proper solution

    SCB_DisableDCache();
       
    USB_HostApplicationInit();
    
    while (1)
    {
        USB_HostTaskFn(g_HostHandle);
        USB_HostHidKeyboardTask(&g_HostHidKeyboard);
        
        // yield for other threads, for example logging to debug console
        k_sleep(K_USEC(1));
    }
}
