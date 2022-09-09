# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(cdc_acm)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})
zephyr_include_directories(./src)

add_subdirectory_ifdef(CONFIG_USB_MCUX_W_HOST components)
add_subdirectory_ifdef(CONFIG_USB_MCUX_W_HOST usb)
