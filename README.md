
Mit diesem Projekt wurde ein USB Host-Beispiel von MCUXPresso SDK für eine Zephyr-Umgebung angepasst. Zephyr enthält bisher standardmäßig keine USB Host-Unterstützung. Es handelt sich hier um das MCUXPresso SDK-Beispiel "usb_host_hid_mouse_keyboard/bm" (bm steht für Bare Metal, also ohne FreeRTOS).

Eine USB-Tastatur kann mittels einen entsprechenden OTG-Adapter an USB1 von MIMXRT1170-EVK. Die Tastaturanschläge werden über Zephyrs Debugconsole angezeigt.

Es werden Teile von Zephyrs USB-Implementierung genutzt, d.h. ist `CONFIG_USB_DEVICE_STACK`=y. Die NXP/MCUXPresso-spezifische Implementierung in Zephyr wird jedoch ausgeschaltet (`CONFIG_USB_MCUX=n`) und stattdesssen eine eigene Implementierung mit Host-Funktion genutzt (`CONFIG_USB_MCUX_W_HOST=y`). Die "eigene" Implementierung ist aus MCUXpresso SDK übernommen und ganz leicht für Zephyr angepasst und befindet sich unter components/* und usb/*. Das Symbol `CONFIG_USB_MCUX_W_HOST` löst aus, dass diese Dateien in diesen zwei Projektverzeichnissen kompiliert werden.

Die Dateien in components/lists und compoents/osa stammen aus MCUX_2.12.0 (https://github.com/NXPmicro/mcux-sdk). Die CMakeLists wurden in der banalsten Form ergänzt.

Die Dateien in usb/host, usb/host/class, usb/phy und usb/include stammen aus MCUX_2.12.0 (https://github.com/NXPmicro/mcux-sdk-middleware-usb). Es wurde lediglich das Logging an Zephyr `LOG_INF` angepasst und in usb_host.h noch `#include "fsl_os_abstraction.h` ergänzt, damit alle Definitionen gefunden werden.

Die Datei usb\device\usb_dc_mcux.c stammt aus Zephyr v3.1.0. Die entsprechende Datei aus Zephyr soll nicht compiliert werden, weil dann die falsche ISR mit dem USB-Interrupt verbunden wird (mittels `IRQ_CONNECT`). Teile von usb_dc_mcux.c werden jedoch benötigt, daher wurde sie in das Projektverzeichnis kopiert.

Wichtige Anpassungen:
- L1 Cache: Teile von MCUXPresso SDK funktioniert nicht, wenn cacheable RAM im Prozessor benutzt wird. Die MCUXpresso-Beispiele laufen standardmäßig komplett im DTCM (wird nie gecached) weswegen es nicht auffällt. Wenn OCRAM verwendet wird, muss der entsprechende Bereich vom Caching ausgenommen werden. Das ist [hier beschrieben](https://community.nxp.com/t5/i-MX-RT-Knowledge-Base/Using-NonCached-Memory-on-i-MXRT/ta-p/1183369). Zunächst wurde nur ein Quick Fix umgesetzt, nämlich L1 D-Cache komplett ausschalten. Das ist vermutlich sehr schlecht für die allgemeine Performance und müsste entsprechend des Links behoben werden!

- MINIMAL_LIBC_MALLOC_ARENA_SIZE vom Standardwert erhöht


Funktioniert noch nicht bzw. weitere Anmerkungen:
- **Die allgemeine Deaktivierung von L1 D-Cache muss gemieden werden!**
- Anschluss von Tastatur über USB Hub funktioniert noch nicht
- Es ist eine komplette Überarbeitung von USB (Device/Host) in Zephyr im Gange. Es scheint jedoch noch relativ lange zu dauern. Außerdem wird es von Nordic Semiconductor (Johann Fischer, siehe [Youtube Präsentation](https://www.youtube.com/watch?v=mWTYAAFoouQ&t=1487s)  und [Github Ticket](https://github.com/zephyrproject-rtos/zephyr/issues/42066)) vorangetrieben. Daher könnte es noch länger dauern, bevor es auch für NXP-Plattforme funktioniert. Host-Funktionalität wird ebenfalls angestrebt, aber eigentlich sekundär und nur damit man Devices damit testen kann.




