#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

if(CONFIG_NRF_MODEM_LINK_WITH_BINARY)

nrfxlib_calculate_lib_path(lib_path)

set(NRF_MODEM_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${lib_path})

if(NOT EXISTS ${NRF_MODEM_PATH})
  message(WARNING "This combination of SoC and floating point ABI is not supported by libmodem."
                  "(${NRF_MODEM_PATH} doesn't exist.)")
endif()

# Link in libmodem and libc.
# libc is linked in because it is used by libmodem
# and must therefore be placed after libmodem on the linker's command line.
set(NRF_MODEM_TARGET modem)
zephyr_library_import(${NRF_MODEM_TARGET} ${NRF_MODEM_PATH}/libmodem.a)

zephyr_include_directories(include)

endif()
