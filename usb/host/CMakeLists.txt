zephyr_include_directories(.)

zephyr_library_sources(
  usb_host_devices.c
  usb_host_ehci.c
  usb_host_framework.c
  usb_host_hci.c
  )

add_subdirectory(class)