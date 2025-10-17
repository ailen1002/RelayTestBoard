CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR cortex-m3)
#SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET(CMAKE_C_COMPILER "D:/devtools/env-windows/tools/bin/../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-gcc.exe")
SET(CMAKE_ASM_COMPILER "D:/devtools/env-windows/tools/bin/../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-gcc.exe")
SET(CMAKE_C_COMPILER_WORKS TRUE)

SET(CMAKE_CXX_COMPILER "D:/devtools/env-windows/tools/bin/../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-g++.exe")
SET(CMAKE_CXX_COMPILER_WORKS TRUE)

SET(CMAKE_OBJCOPY "D:/devtools/env-windows/tools/bin/../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-objcopy.exe")
SET(CMAKE_SIZE "D:/devtools/env-windows/tools/bin/../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-size.exe")

# 避免 Windows 可执行测试报错
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# ------------------------------
# 标准 & 通用编译参数
# ------------------------------
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)       # 使用 GNU 扩展，相当于 -std=gnu11
set(CMAKE_CXX_STANDARD 14)

set(COMMON_FLAGS "-mcpu=cortex-m3 -mthumb -Wall -Wextra -ffunction-sections -fdata-sections")

set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${COMMON_FLAGS}")

# 链接时去掉未使用的函数和数据
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections -T ${CMAKE_SOURCE_DIR}/linkscripts/STM32F103RC/link.lds")

PROJECT(rtthread C CXX ASM)

# ========== include & source 文件保持不变 ==========
include_directories(
		${CMAKE_SOURCE_DIR}/
		${CMAKE_SOURCE_DIR}/applications
		${CMAKE_SOURCE_DIR}/drivers
		${CMAKE_SOURCE_DIR}/drivers/include
		${CMAKE_SOURCE_DIR}/drivers/include/config
		${CMAKE_SOURCE_DIR}/modbus
		${CMAKE_SOURCE_DIR}/libraries/CMSIS/Device/ST/STM32F1xx/Include
		${CMAKE_SOURCE_DIR}/libraries/CMSIS/Include
		${CMAKE_SOURCE_DIR}/libraries/CMSIS/RTOS/Template
		${CMAKE_SOURCE_DIR}/libraries/STM32F1xx_HAL_Driver/Inc
		${CMAKE_SOURCE_DIR}/libraries/STM32F1xx_HAL_Driver/Inc/Legacy
		${CMAKE_SOURCE_DIR}/rt-thread/include
		${CMAKE_SOURCE_DIR}/rt-thread/libcpu/arm/common
		${CMAKE_SOURCE_DIR}/rt-thread/libcpu/arm/cortex-m3
		${CMAKE_SOURCE_DIR}/rt-thread/components/drivers/include
		${CMAKE_SOURCE_DIR}/rt-thread/components/finsh
		${CMAKE_SOURCE_DIR}/rt-thread/components/legacy
		${CMAKE_SOURCE_DIR}/rt-thread/components/libc/compilers/common/include
		${CMAKE_SOURCE_DIR}/rt-thread/components/libc/compilers/newlib
		${CMAKE_SOURCE_DIR}/rt-thread/components/libc/posix/io/epoll
		${CMAKE_SOURCE_DIR}/rt-thread/components/libc/posix/io/eventfd
		${CMAKE_SOURCE_DIR}/rt-thread/components/libc/posix/io/poll
		${CMAKE_SOURCE_DIR}/rt-thread/components/libc/posix/ipc
)

ADD_DEFINITIONS(
		-DRT_USING_LIBC
		-DRT_USING_NEWLIBC
		-D_POSIX_C_SOURCE=1
		-D__RTTHREAD__
		-DSTM32F103xE
		-DUSE_HAL_DRIVER
		-DSOC_SERIES_STM32F1
		-DSOC_FAMILY_STM32
)

SET(PROJECT_SOURCES
		# === RT-Thread 内核核心 (src) ===
		rt-thread/src/klibc/kstdio.c
		rt-thread/src/klibc/kstring.c
		rt-thread/src/clock.c
		rt-thread/src/components.c
		rt-thread/src/idle.c
		rt-thread/src/ipc.c
		rt-thread/src/irq.c
		rt-thread/src/kservice.c
		rt-thread/src/mem.c
		rt-thread/src/mempool.c
		rt-thread/src/object.c
		rt-thread/src/scheduler_comm.c
		rt-thread/src/scheduler_up.c
		rt-thread/src/thread.c
		rt-thread/src/timer.c

		# === CPU 架构相关 (libcpu) ===
		rt-thread/libcpu/arm/cortex-m3/context_gcc.S
		rt-thread/libcpu/arm/cortex-m3/cpuport.c
		rt-thread/libcpu/arm/common/atomic_arm.c
		rt-thread/libcpu/arm/common/div0.c
		rt-thread/libcpu/arm/common/showmem.c

		# === Libc 支持 ===
		rt-thread/components/libc/compilers/newlib/syscalls.c
		rt-thread/components/libc/compilers/common/cctype.c
		rt-thread/components/libc/compilers/common/cstdlib.c
		rt-thread/components/libc/compilers/common/cstring.c
		rt-thread/components/libc/compilers/common/ctime.c
		rt-thread/components/libc/compilers/common/cunistd.c
		rt-thread/components/libc/compilers/common/cwchar.c

		# === Legacy 兼容层 ===
		rt-thread/components/legacy/ipc/workqueue_legacy.c

		# === Shell (Finsh/msh) ===
		rt-thread/components/finsh/cmd.c
		rt-thread/components/finsh/msh.c
		rt-thread/components/finsh/msh_parse.c
		rt-thread/components/finsh/shell.c

		# === 驱动层 (Drivers) ===
		rt-thread/components/drivers/serial/serial.c
		rt-thread/components/drivers/pin/pin.c
		rt-thread/components/drivers/ipc/completion.c
		rt-thread/components/drivers/ipc/condvar.c
		rt-thread/components/drivers/ipc/dataqueue.c
		rt-thread/components/drivers/ipc/pipe.c
		rt-thread/components/drivers/ipc/ringblk_buf.c
		rt-thread/components/drivers/ipc/ringbuffer.c
		rt-thread/components/drivers/ipc/waitqueue.c
		rt-thread/components/drivers/ipc/workqueue.c
		rt-thread/components/drivers/i2c/i2c-bit-ops.c
		rt-thread/components/drivers/i2c/i2c_core.c
		rt-thread/components/drivers/i2c/i2c_dev.c
		rt-thread/components/drivers/core/device.c

		# === STM32 HAL 库 ===
		libraries/STM32F1xx_HAL_Driver/Src/Legacy/stm32f1xx_hal_can.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_adc.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_adc_ex.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_can.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cec.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_crc.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dac.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dac_ex.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_eth.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_hcd.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2c.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2s.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_irda.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_iwdg.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_mmc.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_nand.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_nor.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pccard.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pcd.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pcd_ex.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rtc.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rtc_ex.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_sd.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_smartcard.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_sram.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_usart.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_wwdg.c
		libraries/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_usb.c

		# === CMSIS ===
		libraries/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xe.S
		libraries/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c

		# === drivers / modbus ===
		modbus/vbc_rtu_mb_core.c
		modbus/vbc_rtu_mb_base.c
		modbus/vbc_rtu_mb_utils.c
		modbus/vbc_rtu_mb_port_rtthread.c
		modbus/vbc_rtu_mb_slave_app.c
		modbus/vbc_rtu_virtualIO.c

		# === Board / Application ===
		drivers/board.c
		drivers/drv_adc.c
		drivers/drv_clk.c
		drivers/drv_common.c
		drivers/drv_eth.c
		drivers/drv_flash_f1.c
		drivers/drv_gpio.c
		drivers/drv_hwtimer.c
		drivers/drv_pwm.c
		drivers/drv_qspi.c
		drivers/drv_rtc.c
		drivers/drv_sdio.c
		drivers/drv_soft_i2c.c
		drivers/drv_spi.c
		drivers/drv_usart.c
		drivers/drv_usbd.c
		drivers/drv_usbh.c
		drivers/drv_wdt.c

		applications/main.c
		applications/u_port.c
)

LINK_LIBRARIES(
		c
		m
)

# 输出目录
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}.elf ${PROJECT_SOURCES})
ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME}.elf POST_BUILD
		COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}.elf> ${CMAKE_BINARY_DIR}/bin/${CMAKE_PROJECT_NAME}.bin
		COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:${CMAKE_PROJECT_NAME}.elf> ${CMAKE_BINARY_DIR}/bin/${CMAKE_PROJECT_NAME}.hex
		COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}.elf>
)
