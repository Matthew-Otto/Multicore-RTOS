// RP2040 register map

#ifndef __RP2040__
#define __RP2040__

// 4kb SRAM banks for CPU Stacks
#define CPU0_STACK_BASE             0x20040000
#define CPU0_STACK_TOP              0x20040FFC
#define CPU1_STACK_BASE             0x20041000
#define CPU1_STACK_TOP              0x20041FFC

// platform register categories
#define SYSINFO_BASE                0x40000000
#define SYSCFG_BASE                 0x40004000
#define CLOCKS_BASE                 0x40008000
#define RESETS_BASE                 0x4000c000
#define PSM_BASE                    0x40010000
#define IO_BANK0_BASE               0x40014000
#define IO_QSPI_BASE                0x40018000
#define PADS_BANK0_BASE             0x4001c000
#define PADS_QSPI_BASE              0x40020000
#define XOSC_BASE                   0x40024000
#define PLL_SYS_BASE                0x40028000
#define PLL_USB_BASE                0x4002c000
#define BUSCTRL_BASE                0x40030000
#define UART0_BASE                  0x40034000
#define UART1_BASE                  0x40038000
#define SPI0_BASE                   0x4003c000
#define SPI1_BASE                   0x40040000
#define I2C0_BASE                   0x40044000
#define I2C1_BASE                   0x40048000
#define ADC_BASE                    0x4004c000
#define PWM_BASE                    0x40050000
#define TIMER_BASE                  0x40054000
#define WATCHDOG_BASE               0x40058000
#define RTC_BASE                    0x4005c000
#define ROSC_BASE                   0x40060000
#define VREG_AND_CHIP_RESET_BASE    0x40064000
#define TBMAN_BASE                  0x4006c000
#define PIO0_BASE                   0x50200000
#define PIO1_BASE                   0x50300000
#define XIP_AUX_BASE                0x50400000
#define SIO_BASE                    0xd0000000
#define PPB_BASE                    0xe0000000

#define XIP_BASE                    0x10000000
#define XIP_NOALLOC_BASE            0x11000000
#define XIP_NOCACHE_BASE            0x12000000
#define XIP_NOCACHE_NOALLOC_BASE    0x13000000
#define XIP_CTRL_BASE               0x14000000
#define XIP_SRAM_BASE               0x15000000
#define XIP_SRAM_END                0x15004000
#define XIP_SSI_BASE                0x18000000


// System Config
#define PROC0_NMI_MASK                  (*((volatile uint32_t *)(SYSCFG_BASE)))
#define PROC1_NMI_MASK                  (*((volatile uint32_t *)(SYSCFG_BASE+0x04)))
#define PROC_CONFIG                     (*((volatile uint32_t *)(SYSCFG_BASE+0x08)))
#define PROC_IN_SYNC_BYPASS             (*((volatile uint32_t *)(SYSCFG_BASE+0x0c)))
#define PROC_IN_SYNC_BYPASS_HI          (*((volatile uint32_t *)(SYSCFG_BASE+0x10)))

// Clocks
#define CLK_GPOUT0_CTRL                 (*((volatile uint32_t *)(CLOCKS_BASE+0x00))) // Clock control, can be changed on-the-fly (except for auxsrc)
#define CLK_GPOUT0_DIV                  (*((volatile uint32_t *)(CLOCKS_BASE+0x04))) // Clock divisor, can be changed on-the-fly
#define CLK_GPOUT0_SELECTED             (*((volatile uint32_t *)(CLOCKS_BASE+0x08))) // Indicates which SRC is currently selected by the glitchless mux (one-hot).
#define CLK_GPOUT1_CTRL                 (*((volatile uint32_t *)(CLOCKS_BASE+0x0c))) // Clock control, can be changed on-the-fly (except for auxsrc)
#define CLK_GPOUT1_DIV                  (*((volatile uint32_t *)(CLOCKS_BASE+0x10))) // Clock divisor, can be changed on-the-fly
#define CLK_GPOUT1_SELECTED             (*((volatile uint32_t *)(CLOCKS_BASE+0x14))) // Indicates which SRC is currently selected by the glitchless mux (one-hot).
#define CLK_GPOUT2_CTRL                 (*((volatile uint32_t *)(CLOCKS_BASE+0x18))) // Clock control, can be changed on-the-fly (except for auxsrc)
#define CLK_GPOUT2_DIV                  (*((volatile uint32_t *)(CLOCKS_BASE+0x1c))) // Clock divisor, can be changed on-the-fly
#define CLK_GPOUT2_SELECTED             (*((volatile uint32_t *)(CLOCKS_BASE+0x20))) // Indicates which SRC is currently selected by the glitchless mux (one-hot).
#define CLK_GPOUT3_CTRL                 (*((volatile uint32_t *)(CLOCKS_BASE+0x24))) // Clock control, can be changed on-the-fly (except for auxsrc)
#define CLK_GPOUT3_DIV                  (*((volatile uint32_t *)(CLOCKS_BASE+0x28))) // Clock divisor, can be changed on-the-fly
#define CLK_GPOUT3_SELECTED             (*((volatile uint32_t *)(CLOCKS_BASE+0x2c))) // Indicates which SRC is currently selected by the glitchless mux (one-hot).
#define CLK_REF_CTRL                    (*((volatile uint32_t *)(CLOCKS_BASE+0x30))) // Clock control, can be changed on-the-fly (except for auxsrc)
#define CLK_REF_DIV                     (*((volatile uint32_t *)(CLOCKS_BASE+0x34))) // Clock divisor, can be changed on-the-fly
#define CLK_REF_SELECTED                (*((volatile uint32_t *)(CLOCKS_BASE+0x38))) // Indicates which SRC is currently selected by the glitchless mux (one-hot).
#define CLK_SYS_CTRL                    (*((volatile uint32_t *)(CLOCKS_BASE+0x3c))) // Clock control, can be changed on-the-fly (except for auxsrc)
#define CLK_SYS_DIV                     (*((volatile uint32_t *)(CLOCKS_BASE+0x40))) // Clock divisor, can be changed on-the-fly
#define CLK_SYS_SELECTED                (*((volatile uint32_t *)(CLOCKS_BASE+0x44))) // Indicates which SRC is currently selected by the glitchless mux (one-hot).
#define CLK_PERI_CTRL                   (*((volatile uint32_t *)(CLOCKS_BASE+0x48))) // Clock control, can be changed on-the-fly (except for auxsrc)
#define CLK_PERI_SELECTED               (*((volatile uint32_t *)(CLOCKS_BASE+0x50))) // Indicates which SRC is currently selected by the glitchless mux (one-hot).
#define CLK_USB_CTRL                    (*((volatile uint32_t *)(CLOCKS_BASE+0x54))) // Clock control, can be changed on-the-fly (except for auxsrc)
#define CLK_USB_DIV                     (*((volatile uint32_t *)(CLOCKS_BASE+0x58))) // Clock divisor, can be changed on-the-fly
#define CLK_USB_SELECTED                (*((volatile uint32_t *)(CLOCKS_BASE+0x5c))) // Indicates which SRC is currently selected by the glitchless mux (one-hot).
#define CLK_ADC_CTRL                    (*((volatile uint32_t *)(CLOCKS_BASE+0x60))) // Clock control, can be changed on-the-fly (except for auxsrc)
#define CLK_ADC_DIV                     (*((volatile uint32_t *)(CLOCKS_BASE+0x64))) // Clock divisor, can be changed on-the-fly
#define CLK_ADC_SELECTED                (*((volatile uint32_t *)(CLOCKS_BASE+0x68))) // Indicates which SRC is currently selected by the glitchless mux (one-hot).
#define CLK_RTC_CTRL                    (*((volatile uint32_t *)(CLOCKS_BASE+0x6c))) // Clock control, can be changed on-the-fly (except for auxsrc)
#define CLK_RTC_DIV                     (*((volatile uint32_t *)(CLOCKS_BASE+0x70))) // Clock divisor, can be changed on-the-fly
#define CLK_RTC_SELECTED                (*((volatile uint32_t *)(CLOCKS_BASE+0x74))) // Indicates which SRC is currently selected by the glitchless mux (one-hot).
#define CLK_SYS_RESUS_CTRL              (*((volatile uint32_t *)(CLOCKS_BASE+0x78))) //
#define CLK_SYS_RESUS_STATUS            (*((volatile uint32_t *)(CLOCKS_BASE+0x7c))) //
#define FC0_REF_KHZ                     (*((volatile uint32_t *)(CLOCKS_BASE+0x80))) // Reference clock frequency in kHz
#define FC0_MIN_KHZ                     (*((volatile uint32_t *)(CLOCKS_BASE+0x84))) // Minimum pass frequency in kHz. This is optional. Set to 0 if you are not using the pass/fail flags
#define FC0_MAX_KHZ                     (*((volatile uint32_t *)(CLOCKS_BASE+0x88))) // Maximum pass frequency in kHz. This is optional. Set to 0x1ffffff if you are not using the pass/fail flags
#define FC0_DELAY                       (*((volatile uint32_t *)(CLOCKS_BASE+0x8c))) // Delays the start of frequency counting to allow the mux to settle Delay is measured in multiples of the reference clock period
#define FC0_INTERVAL                    (*((volatile uint32_t *)(CLOCKS_BASE+0x90))) // The test interval is 0.98us * 2**interval, but let’s call it 1us * 2**interval The default gives a test interval of 250us
#define FC0_SRC                         (*((volatile uint32_t *)(CLOCKS_BASE+0x94))) // Clock sent to frequency counter, set to 0 when not required Writing to this register initiates the frequency count
#define FC0_STATUS                      (*((volatile uint32_t *)(CLOCKS_BASE+0x98))) // Frequency counter status
#define FC0_RESULT                      (*((volatile uint32_t *)(CLOCKS_BASE+0x9c))) // Result of frequency measurement, only valid when status_done=1
#define WAKE_EN0                        (*((volatile uint32_t *)(CLOCKS_BASE+0xa0))) // enable clock in wake mode
#define WAKE_EN1                        (*((volatile uint32_t *)(CLOCKS_BASE+0xa4))) // enable clock in wake mode
#define SLEEP_EN0                       (*((volatile uint32_t *)(CLOCKS_BASE+0xa8))) // enable clock in sleep mode
#define SLEEP_EN1                       (*((volatile uint32_t *)(CLOCKS_BASE+0xac))) // enable clock in sleep mode
#define ENABLED0                        (*((volatile uint32_t *)(CLOCKS_BASE+0xb0))) // indicates the state of the clock enable
#define ENABLED1                        (*((volatile uint32_t *)(CLOCKS_BASE+0xb4))) // indicates the state of the clock enable
#define INTR                            (*((volatile uint32_t *)(CLOCKS_BASE+0xb8))) // Raw Interrupts
#define INTE                            (*((volatile uint32_t *)(CLOCKS_BASE+0xbc))) // Interrupt Enable
#define INTF                            (*((volatile uint32_t *)(CLOCKS_BASE+0xc0))) // Interrupt Force
#define INTS                            (*((volatile uint32_t *)(CLOCKS_BASE+0xc4))) // Interrupt status after masking & forcing

// RESETS
#define RESET                           (*((volatile uint32_t *)(RESETS_BASE+0x0))) // Reset control.
#define RESET_SET                       (*((volatile uint32_t *)(RESETS_BASE+0x2000))) // Reset control.
#define RESET_CLR                       (*((volatile uint32_t *)(RESETS_BASE+0x3000))) // Reset control.
#define WDSEL                           (*((volatile uint32_t *)(RESETS_BASE+0x4))) // Watchdog select.
#define WDSEL_SET                       (*((volatile uint32_t *)(RESETS_BASE+0x2004))) // Watchdog select.
#define WDSEL_CLR                       (*((volatile uint32_t *)(RESETS_BASE+0x3004))) // Watchdog select.
#define RESET_DONE                      (*((volatile uint32_t *)(RESETS_BASE+0x8))) // Reset done.

// PSM
#define PSM_FRCE_ON                     (*((volatile uint32_t *)(PSM_BASE+0x0))) // Force block out of reset (i.e. power it on)
#define PSM_FRCE_ON_SET                 (*((volatile uint32_t *)(PSM_BASE+0x2000))) // Force block out of reset (i.e. power it on)
#define PSM_FRCE_ON_CLR                 (*((volatile uint32_t *)(PSM_BASE+0x3000))) // Force block out of reset (i.e. power it on)
#define PSM_FRCE_OFF                    (*((volatile uint32_t *)(PSM_BASE+0x4))) // Force into reset (i.e. power it off)
#define PSM_FRCE_OFF_SET                (*((volatile uint32_t *)(PSM_BASE+0x2004))) // Force into reset (i.e. power it off)
#define PSM_FRCE_OFF_CLR                (*((volatile uint32_t *)(PSM_BASE+0x3004))) // Force into reset (i.e. power it off)
#define PSM_WDSEL                       (*((volatile uint32_t *)(PSM_BASE+0x8))) // Set to 1 if this peripheral should be reset when the watchdog fires.
#define PSM_WDSEL_SET                   (*((volatile uint32_t *)(PSM_BASE+0x2008))) // Set to 1 if this peripheral should be reset when the watchdog fires.
#define PSM_WDSEL_CLR                   (*((volatile uint32_t *)(PSM_BASE+0x3008))) // Set to 1 if this peripheral should be reset when the watchdog fires.
#define PSM_DONE                        (*((volatile uint32_t *)(PSM_BASE+0xc))) // Indicates the peripheral’s registers are ready to access.

// GPIO
#define IO_GPIO0_STATUS                 (*((volatile uint32_t *)(IO_BANK0_BASE+0x000))) // GPIO status
#define IO_GPIO0_CTRL                   (*((volatile uint32_t *)(IO_BANK0_BASE+0x004))) // GPIO control including function select and overrides.
#define IO_GPIO1_STATUS                 (*((volatile uint32_t *)(IO_BANK0_BASE+0x008))) // GPIO status
#define IO_GPIO1_CTRL                   (*((volatile uint32_t *)(IO_BANK0_BASE+0x00c))) // GPIO control including function select and overrides.
#define IO_GPIO2_STATUS                 (*((volatile uint32_t *)(IO_BANK0_BASE+0x010))) // GPIO status
#define IO_GPIO2_CTRL                   (*((volatile uint32_t *)(IO_BANK0_BASE+0x014))) // GPIO control including function select and overrides.
#define IO_GPIO3_STATUS                 (*((volatile uint32_t *)(IO_BANK0_BASE+0x018))) // GPIO status
#define IO_GPIO3_CTRL                   (*((volatile uint32_t *)(IO_BANK0_BASE+0x01c))) // GPIO control including function select and overrides.
#define IO_GPIO4_STATUS                 (*((volatile uint32_t *)(IO_BANK0_BASE+0x020))) // GPIO status
#define IO_GPIO4_CTRL                   (*((volatile uint32_t *)(IO_BANK0_BASE+0x024))) // GPIO control including function select and overrides.
#define IO_GPIO5_STATUS                 (*((volatile uint32_t *)(IO_BANK0_BASE+0x028))) // GPIO status
#define IO_GPIO5_CTRL                   (*((volatile uint32_t *)(IO_BANK0_BASE+0x02c))) // GPIO control including function select and overrides.
#define IO_GPIO6_STATUS                 (*((volatile uint32_t *)(IO_BANK0_BASE+0x030))) // GPIO status
#define IO_GPIO6_CTRL                   (*((volatile uint32_t *)(IO_BANK0_BASE+0x034))) // GPIO control including function select and overrides.
#define IO_GPIO7_STATUS                 (*((volatile uint32_t *)(IO_BANK0_BASE+0x038))) // GPIO status
#define IO_GPIO7_CTRL                   (*((volatile uint32_t *)(IO_BANK0_BASE+0x03c))) // GPIO control including function select and overrides.
#define IO_GPIO8_STATUS                 (*((volatile uint32_t *)(IO_BANK0_BASE+0x040))) // GPIO status
#define IO_GPIO8_CTRL                   (*((volatile uint32_t *)(IO_BANK0_BASE+0x044))) // GPIO control including function select and overrides.
#define IO_GPIO9_STATUS                 (*((volatile uint32_t *)(IO_BANK0_BASE+0x048))) // GPIO status
#define IO_GPIO9_CTRL                   (*((volatile uint32_t *)(IO_BANK0_BASE+0x04c))) // GPIO control including function select and overrides.
#define IO_GPIO10_STATUS                (*((volatile uint32_t *)(IO_BANK0_BASE+0x050))) // GPIO status
#define IO_GPIO10_CTRL                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x054))) // GPIO control including function select and overrides.
#define IO_GPIO11_STATUS                (*((volatile uint32_t *)(IO_BANK0_BASE+0x058))) // GPIO status
#define IO_GPIO11_CTRL                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x05c))) // GPIO control including function select and overrides.
#define IO_GPIO12_STATUS                (*((volatile uint32_t *)(IO_BANK0_BASE+0x060))) // GPIO status
#define IO_GPIO12_CTRL                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x064))) // GPIO control including function select and overrides.
#define IO_GPIO13_STATUS                (*((volatile uint32_t *)(IO_BANK0_BASE+0x068))) // GPIO status
#define IO_GPIO13_CTRL                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x06c))) // GPIO control including function select and overrides.
#define IO_GPIO14_STATUS                (*((volatile uint32_t *)(IO_BANK0_BASE+0x070))) // GPIO status
#define IO_GPIO14_CTRL                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x074))) // GPIO control including function select and overrides.
#define IO_GPIO15_STATUS                (*((volatile uint32_t *)(IO_BANK0_BASE+0x078))) // GPIO status
#define IO_GPIO15_CTRL                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x07c))) // GPIO control including function select and overrides.
#define IO_GPIO16_STATUS                (*((volatile uint32_t *)(IO_BANK0_BASE+0x080))) // GPIO status
#define IO_GPIO16_CTRL                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x084))) // GPIO control including function select and overrides.
#define IO_GPIO17_STATUS                (*((volatile uint32_t *)(IO_BANK0_BASE+0x088))) // GPIO status
#define IO_GPIO17_CTRL                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x08c))) // GPIO control including function select and overrides.
#define IO_GPIO18_STATUS                (*((volatile uint32_t *)(IO_BANK0_BASE+0x090))) // GPIO status
#define IO_GPIO18_CTRL                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x094))) // GPIO control including function select and overrides.
#define IO_GPIO19_STATUS                (*((volatile uint32_t *)(IO_BANK0_BASE+0x098))) // GPIO status
#define IO_GPIO19_CTRL                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x09c))) // GPIO control including function select and overrides.
#define IO_GPIO20_STATUS                (*((volatile uint32_t *)(IO_BANK0_BASE+0x0a0))) // GPIO status
#define IO_GPIO20_CTRL                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x0a4))) // GPIO control including function select and overrides.
#define IO_GPIO21_STATUS                (*((volatile uint32_t *)(IO_BANK0_BASE+0x0a8))) // GPIO status
#define IO_GPIO21_CTRL                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x0ac))) // GPIO control including function select and overrides.
#define IO_GPIO22_STATUS                (*((volatile uint32_t *)(IO_BANK0_BASE+0x0b0))) // GPIO status
#define IO_GPIO22_CTRL                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x0b4))) // GPIO control including function select and overrides.
#define IO_GPIO23_STATUS                (*((volatile uint32_t *)(IO_BANK0_BASE+0x0b8))) // GPIO status
#define IO_GPIO23_CTRL                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x0bc))) // GPIO control including function select and overrides.
#define IO_GPIO24_STATUS                (*((volatile uint32_t *)(IO_BANK0_BASE+0x0c0))) // GPIO status
#define IO_GPIO24_CTRL                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x0c4))) // GPIO control including function select and overrides.
#define IO_GPIO25_STATUS                (*((volatile uint32_t *)(IO_BANK0_BASE+0x0c8))) // GPIO status
#define IO_GPIO25_CTRL                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x0cc))) // GPIO control including function select and overrides.
#define IO_GPIO26_STATUS                (*((volatile uint32_t *)(IO_BANK0_BASE+0x0d0))) // GPIO status
#define IO_GPIO26_CTRL                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x0d4))) // GPIO control including function select and overrides.
#define IO_GPIO27_STATUS                (*((volatile uint32_t *)(IO_BANK0_BASE+0x0d8))) // GPIO status
#define IO_GPIO27_CTRL                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x0dc))) // GPIO control including function select and overrides.
#define IO_GPIO28_STATUS                (*((volatile uint32_t *)(IO_BANK0_BASE+0x0e0))) // GPIO status
#define IO_GPIO28_CTRL                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x0e4))) // GPIO control including function select and overrides.
#define IO_GPIO29_STATUS                (*((volatile uint32_t *)(IO_BANK0_BASE+0x0e8))) // GPIO status
#define IO_GPIO29_CTRL                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x0ec))) // GPIO control including function select and overrides.
#define IO_INTR0                        (*((volatile uint32_t *)(IO_BANK0_BASE+0x0f0))) // Raw Interrupts
#define IO_INTR1                        (*((volatile uint32_t *)(IO_BANK0_BASE+0x0f4))) // Raw Interrupts
#define IO_INTR2                        (*((volatile uint32_t *)(IO_BANK0_BASE+0x0f8))) // Raw Interrupts
#define IO_INTR3                        (*((volatile uint32_t *)(IO_BANK0_BASE+0x0fc))) // Raw Interrupts
#define IO_PROC0_INTE0                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x100))) // Interrupt Enable for proc0
#define IO_PROC0_INTE1                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x104))) // Interrupt Enable for proc0
#define IO_PROC0_INTE2                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x108))) // Interrupt Enable for proc0
#define IO_PROC0_INTE3                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x10c))) // Interrupt Enable for proc0
#define IO_PROC0_INTF0                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x110))) // Interrupt Force for proc0
#define IO_PROC0_INTF1                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x114))) // Interrupt Force for proc0
#define IO_PROC0_INTF2                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x118))) // Interrupt Force for proc0
#define IO_PROC0_INTF3                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x11c))) // Interrupt Force for proc0
#define IO_PROC0_INTS0                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x120))) // Interrupt status after masking & forcing for proc0
#define IO_PROC0_INTS1                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x124))) // Interrupt status after masking & forcing for proc0
#define IO_PROC0_INTS2                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x128))) // Interrupt status after masking & forcing for proc0
#define IO_PROC0_INTS3                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x12c))) // Interrupt status after masking & forcing for proc0
#define IO_PROC1_INTE0                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x130))) // Interrupt Enable for proc1
#define IO_PROC1_INTE1                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x134))) // Interrupt Enable for proc1
#define IO_PROC1_INTE2                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x138))) // Interrupt Enable for proc1
#define IO_PROC1_INTE3                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x13c))) // Interrupt Enable for proc1
#define IO_PROC1_INTF0                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x140))) // Interrupt Force for proc1
#define IO_PROC1_INTF1                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x144))) // Interrupt Force for proc1
#define IO_PROC1_INTF2                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x148))) // Interrupt Force for proc1
#define IO_PROC1_INTF3                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x14c))) // Interrupt Force for proc1
#define IO_PROC1_INTS0                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x150))) // Interrupt status after masking & forcing for proc1
#define IO_PROC1_INTS1                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x154))) // Interrupt status after masking & forcing for proc1
#define IO_PROC1_INTS2                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x158))) // Interrupt status after masking & forcing for proc1
#define IO_PROC1_INTS3                  (*((volatile uint32_t *)(IO_BANK0_BASE+0x15c))) // Interrupt status after masking & forcing for proc1
#define IO_DORMANT_WAKE_INTE0           (*((volatile uint32_t *)(IO_BANK0_BASE+0x160))) // Interrupt Enable for dormant_wake
#define IO_DORMANT_WAKE_INTE1           (*((volatile uint32_t *)(IO_BANK0_BASE+0x164))) // Interrupt Enable for dormant_wake
#define IO_DORMANT_WAKE_INTE2           (*((volatile uint32_t *)(IO_BANK0_BASE+0x168))) // Interrupt Enable for dormant_wake
#define IO_DORMANT_WAKE_INTE3           (*((volatile uint32_t *)(IO_BANK0_BASE+0x16c))) // Interrupt Enable for dormant_wake
#define IO_DORMANT_WAKE_INTF0           (*((volatile uint32_t *)(IO_BANK0_BASE+0x170))) // Interrupt Force for dormant_wake
#define IO_DORMANT_WAKE_INTF1           (*((volatile uint32_t *)(IO_BANK0_BASE+0x174))) // Interrupt Force for dormant_wake
#define IO_DORMANT_WAKE_INTF2           (*((volatile uint32_t *)(IO_BANK0_BASE+0x178))) // Interrupt Force for dormant_wake
#define IO_DORMANT_WAKE_INTF3           (*((volatile uint32_t *)(IO_BANK0_BASE+0x17c))) // Interrupt Force for dormant_wake
#define IO_DORMANT_WAKE_INTS0           (*((volatile uint32_t *)(IO_BANK0_BASE+0x180))) // Interrupt status after masking & forcing for dormant_wake
#define IO_DORMANT_WAKE_INTS1           (*((volatile uint32_t *)(IO_BANK0_BASE+0x184))) // Interrupt status after masking & forcing for dormant_wake
#define IO_DORMANT_WAKE_INTS2           (*((volatile uint32_t *)(IO_BANK0_BASE+0x188))) // Interrupt status after masking & forcing for dormant_wake
#define IO_DORMANT_WAKE_INTS3           (*((volatile uint32_t *)(IO_BANK0_BASE+0x18c))) // Interrupt status after masking & forcing for dormant_wake

// PADS_BANK0
#define PADS_GPIO0_STATUS               (*((volatile uint32_t *)(PADS_BANK0_BASE+0x000))) GPIO status
#define PADS_GPIO0_CTRL                 (*((volatile uint32_t *)(PADS_BANK0_BASE+0x004))) GPIO control including function select and overrides.
#define PADS_GPIO1_STATUS               (*((volatile uint32_t *)(PADS_BANK0_BASE+0x008))) GPIO status
#define PADS_GPIO1_CTRL                 (*((volatile uint32_t *)(PADS_BANK0_BASE+0x00c))) GPIO control including function select and overrides.
#define PADS_GPIO2_STATUS               (*((volatile uint32_t *)(PADS_BANK0_BASE+0x010))) GPIO status
#define PADS_GPIO2_CTRL                 (*((volatile uint32_t *)(PADS_BANK0_BASE+0x014))) GPIO control including function select and overrides.
#define PADS_GPIO3_STATUS               (*((volatile uint32_t *)(PADS_BANK0_BASE+0x018))) GPIO status
#define PADS_GPIO3_CTRL                 (*((volatile uint32_t *)(PADS_BANK0_BASE+0x01c))) GPIO control including function select and overrides.
#define PADS_GPIO4_STATUS               (*((volatile uint32_t *)(PADS_BANK0_BASE+0x020))) GPIO status
#define PADS_GPIO4_CTRL                 (*((volatile uint32_t *)(PADS_BANK0_BASE+0x024))) GPIO control including function select and overrides.
#define PADS_GPIO5_STATUS               (*((volatile uint32_t *)(PADS_BANK0_BASE+0x028))) GPIO status
#define PADS_GPIO5_CTRL                 (*((volatile uint32_t *)(PADS_BANK0_BASE+0x02c))) GPIO control including function select and overrides.
#define PADS_GPIO6_STATUS               (*((volatile uint32_t *)(PADS_BANK0_BASE+0x030))) GPIO status
#define PADS_GPIO6_CTRL                 (*((volatile uint32_t *)(PADS_BANK0_BASE+0x034))) GPIO control including function select and overrides.
#define PADS_GPIO7_STATUS               (*((volatile uint32_t *)(PADS_BANK0_BASE+0x038))) GPIO status
#define PADS_GPIO7_CTRL                 (*((volatile uint32_t *)(PADS_BANK0_BASE+0x03c))) GPIO control including function select and overrides.
#define PADS_GPIO8_STATUS               (*((volatile uint32_t *)(PADS_BANK0_BASE+0x040))) GPIO status
#define PADS_GPIO8_CTRL                 (*((volatile uint32_t *)(PADS_BANK0_BASE+0x044))) GPIO control including function select and overrides.
#define PADS_GPIO9_STATUS               (*((volatile uint32_t *)(PADS_BANK0_BASE+0x048))) GPIO status
#define PADS_GPIO9_CTRL                 (*((volatile uint32_t *)(PADS_BANK0_BASE+0x04c))) GPIO control including function select and overrides.
#define PADS_GPIO10_STATUS              (*((volatile uint32_t *)(PADS_BANK0_BASE+0x050))) GPIO status
#define PADS_GPIO10_CTRL                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x054))) GPIO control including function select and overrides.
#define PADS_GPIO11_STATUS              (*((volatile uint32_t *)(PADS_BANK0_BASE+0x058))) GPIO status
#define PADS_GPIO11_CTRL                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x05c))) GPIO control including function select and overrides.
#define PADS_GPIO12_STATUS              (*((volatile uint32_t *)(PADS_BANK0_BASE+0x060))) GPIO status
#define PADS_GPIO12_CTRL                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x064))) GPIO control including function select and overrides.
#define PADS_GPIO13_STATUS              (*((volatile uint32_t *)(PADS_BANK0_BASE+0x068))) GPIO status
#define PADS_GPIO13_CTRL                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x06c))) GPIO control including function select and overrides.
#define PADS_GPIO14_STATUS              (*((volatile uint32_t *)(PADS_BANK0_BASE+0x070))) GPIO status
#define PADS_GPIO14_CTRL                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x074))) GPIO control including function select and overrides.
#define PADS_GPIO15_STATUS              (*((volatile uint32_t *)(PADS_BANK0_BASE+0x078))) GPIO status
#define PADS_GPIO15_CTRL                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x07c))) GPIO control including function select and overrides.
#define PADS_GPIO16_STATUS              (*((volatile uint32_t *)(PADS_BANK0_BASE+0x080))) GPIO status
#define PADS_GPIO16_CTRL                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x084))) GPIO control including function select and overrides.
#define PADS_GPIO17_STATUS              (*((volatile uint32_t *)(PADS_BANK0_BASE+0x088))) GPIO status
#define PADS_GPIO17_CTRL                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x08c))) GPIO control including function select and overrides.
#define PADS_GPIO18_STATUS              (*((volatile uint32_t *)(PADS_BANK0_BASE+0x090))) GPIO status
#define PADS_GPIO18_CTRL                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x094))) GPIO control including function select and overrides.
#define PADS_GPIO19_STATUS              (*((volatile uint32_t *)(PADS_BANK0_BASE+0x098))) GPIO status
#define PADS_GPIO19_CTRL                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x09c))) GPIO control including function select and overrides.
#define PADS_GPIO20_STATUS              (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0a0))) GPIO status
#define PADS_GPIO20_CTRL                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0a4))) GPIO control including function select and overrides.
#define PADS_GPIO21_STATUS              (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0a8))) GPIO status
#define PADS_GPIO21_CTRL                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0ac))) GPIO control including function select and overrides.
#define PADS_GPIO22_STATUS              (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0b0))) GPIO status
#define PADS_GPIO22_CTRL                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0b4))) GPIO control including function select and overrides.
#define PADS_GPIO23_STATUS              (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0b8))) GPIO status
#define PADS_GPIO23_CTRL                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0bc))) GPIO control including function select and overrides.
#define PADS_GPIO24_STATUS              (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0c0))) GPIO status
#define PADS_GPIO24_CTRL                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0c4))) GPIO control including function select and overrides.
#define PADS_GPIO25_STATUS              (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0c8))) GPIO status
#define PADS_GPIO25_CTRL                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0cc))) GPIO control including function select and overrides.
#define PADS_GPIO26_STATUS              (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0d0))) GPIO status
#define PADS_GPIO26_CTRL                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0d4))) GPIO control including function select and overrides.
#define PADS_GPIO27_STATUS              (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0d8))) GPIO status
#define PADS_GPIO27_CTRL                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0dc))) GPIO control including function select and overrides.
#define PADS_GPIO28_STATUS              (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0e0))) GPIO status
#define PADS_GPIO28_CTRL                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0e4))) GPIO control including function select and overrides.
#define PADS_GPIO29_STATUS              (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0e8))) GPIO status
#define PADS_GPIO29_CTRL                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0ec))) GPIO control including function select and overrides.
#define PADS_INTR0                      (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0f0))) Raw Interrupts
#define PADS_INTR1                      (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0f4))) Raw Interrupts
#define PADS_INTR2                      (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0f8))) Raw Interrupts
#define PADS_INTR3                      (*((volatile uint32_t *)(PADS_BANK0_BASE+0x0fc))) Raw Interrupts
#define PADS_PROC0_INTE0                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x100))) Interrupt Enable for proc0
#define PADS_PROC0_INTE1                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x104))) Interrupt Enable for proc0
#define PADS_PROC0_INTE2                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x108))) Interrupt Enable for proc0
#define PADS_PROC0_INTE3                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x10c))) Interrupt Enable for proc0
#define PADS_PROC0_INTF0                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x110))) Interrupt Force for proc0
#define PADS_PROC0_INTF1                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x114))) Interrupt Force for proc0
#define PADS_PROC0_INTF2                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x118))) Interrupt Force for proc0
#define PADS_PROC0_INTF3                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x11c))) Interrupt Force for proc0
#define PADS_PROC0_INTS0                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x120))) Interrupt status after masking & forcing for proc0
#define PADS_PROC0_INTS1                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x124))) Interrupt status after masking & forcing for proc0
#define PADS_PROC0_INTS2                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x128))) Interrupt status after masking & forcing for proc0
#define PADS_PROC0_INTS3                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x12c))) Interrupt status after masking & forcing for proc0
#define PADS_PROC1_INTE0                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x130))) Interrupt Enable for proc1
#define PADS_PROC1_INTE1                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x134))) Interrupt Enable for proc1
#define PADS_PROC1_INTE2                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x138))) Interrupt Enable for proc1
#define PADS_PROC1_INTE3                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x13c))) Interrupt Enable for proc1
#define PADS_PROC1_INTF0                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x140))) Interrupt Force for proc1
#define PADS_PROC1_INTF1                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x144))) Interrupt Force for proc1
#define PADS_PROC1_INTF2                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x148))) Interrupt Force for proc1
#define PADS_PROC1_INTF3                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x14c))) Interrupt Force for proc1
#define PADS_PROC1_INTS0                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x150))) Interrupt status after masking & forcing for proc1
#define PADS_PROC1_INTS1                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x154))) Interrupt status after masking & forcing for proc1
#define PADS_PROC1_INTS2                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x158))) Interrupt status after masking & forcing for proc1
#define PADS_PROC1_INTS3                (*((volatile uint32_t *)(PADS_BANK0_BASE+0x15c))) Interrupt status after masking & forcing for proc1
#define PADS_DORMANT_WAKE_INTE0         (*((volatile uint32_t *)(PADS_BANK0_BASE+0x160))) Interrupt Enable for dormant_wake
#define PADS_DORMANT_WAKE_INTE1         (*((volatile uint32_t *)(PADS_BANK0_BASE+0x164))) Interrupt Enable for dormant_wake
#define PADS_DORMANT_WAKE_INTE2         (*((volatile uint32_t *)(PADS_BANK0_BASE+0x168))) Interrupt Enable for dormant_wake
#define PADS_DORMANT_WAKE_INTE3         (*((volatile uint32_t *)(PADS_BANK0_BASE+0x16c))) Interrupt Enable for dormant_wake
#define PADS_DORMANT_WAKE_INTF0         (*((volatile uint32_t *)(PADS_BANK0_BASE+0x170))) Interrupt Force for dormant_wake
#define PADS_DORMANT_WAKE_INTF1         (*((volatile uint32_t *)(PADS_BANK0_BASE+0x174))) Interrupt Force for dormant_wake
#define PADS_DORMANT_WAKE_INTF2         (*((volatile uint32_t *)(PADS_BANK0_BASE+0x178))) Interrupt Force for dormant_wake
#define PADS_DORMANT_WAKE_INTF3         (*((volatile uint32_t *)(PADS_BANK0_BASE+0x17c))) Interrupt Force for dormant_wake
#define PADS_DORMANT_WAKE_INTS0         (*((volatile uint32_t *)(PADS_BANK0_BASE+0x180))) Interrupt status after masking & forcing for dormant_wake
#define PADS_DORMANT_WAKE_INTS1         (*((volatile uint32_t *)(PADS_BANK0_BASE+0x184))) Interrupt status after masking & forcing for dormant_wake
#define PADS_DORMANT_WAKE_INTS2         (*((volatile uint32_t *)(PADS_BANK0_BASE+0x188))) Interrupt status after masking & forcing for dormant_wake
#define PADS_DORMANT_WAKE_INTS3         (*((volatile uint32_t *)(PADS_BANK0_BASE+0x18c))) Interrupt status after masking & forcing for dormant_wake

// XOSC
#define XOSC_CTRL                       (*((volatile uint32_t *)(XOSC_BASE+0x00))) // Crystal Oscillator Control
#define XOSC_STATUS                     (*((volatile uint32_t *)(XOSC_BASE+0x04))) // Crystal Oscillator Status
#define XOSC_DORMANT                    (*((volatile uint32_t *)(XOSC_BASE+0x08))) // Crystal Oscillator pause control
#define XOSC_STARTUP                    (*((volatile uint32_t *)(XOSC_BASE+0x0c))) // Controls the startup delay
#define XOSC_COUNT                      (*((volatile uint32_t *)(XOSC_BASE+0x1c))) // A down counter running at the XOSC frequency which counts to zero and stops.

// PLL_SYS
#define PLL_SYS_CS                      (*((volatile uint32_t *)(PLL_SYS_BASE+0x0))) // Control and Status
#define PLL_SYS_CS_SET                  (*((volatile uint32_t *)(PLL_SYS_BASE+0x2000))) // Control and Status
#define PLL_SYS_CS_CLR                  (*((volatile uint32_t *)(PLL_SYS_BASE+0x3000))) // Control and Status
#define PLL_SYS_PWR                     (*((volatile uint32_t *)(PLL_SYS_BASE+0x4))) // Controls the PLL power modes.
#define PLL_SYS_PWR_SET                 (*((volatile uint32_t *)(PLL_SYS_BASE+0x2004))) // Controls the PLL power modes.
#define PLL_SYS_PWR_CLR                 (*((volatile uint32_t *)(PLL_SYS_BASE+0x3004))) // Controls the PLL power modes.
#define PLL_SYS_FBDIV_INT               (*((volatile uint32_t *)(PLL_SYS_BASE+0x8))) // Feedback divisor
#define PLL_SYS_FBDIV_INT_SET           (*((volatile uint32_t *)(PLL_SYS_BASE+0x2008))) // Feedback divisor
#define PLL_SYS_FBDIV_INT_CLR           (*((volatile uint32_t *)(PLL_SYS_BASE+0x3008))) // Feedback divisor
#define PLL_SYS_PRIM                    (*((volatile uint32_t *)(PLL_SYS_BASE+0xc))) // Controls the PLL post dividers for the primary output
#define PLL_SYS_PRIM_SET                (*((volatile uint32_t *)(PLL_SYS_BASE+0x200c))) // Controls the PLL post dividers for the primary output
#define PLL_SYS_PRIM_CLR                (*((volatile uint32_t *)(PLL_SYS_BASE+0x300c))) // Controls the PLL post dividers for the primary output

// PLL_USB
#define PLL_USB_CS                      (*((volatile uint32_t *)(PLL_USB_BASE+0x0))) // Control and Status
#define PLL_USB_PWR                     (*((volatile uint32_t *)(PLL_USB_BASE+0x4))) // Controls the PLL power modes.
#define PLL_USB_FBDIV_INT               (*((volatile uint32_t *)(PLL_USB_BASE+0x8))) // Feedback divisor
#define PLL_USB_PRIM                    (*((volatile uint32_t *)(PLL_USB_BASE+0xc))) // Controls the PLL post dividers for the primary output

// UART0
#define UART0_UARTDR                    (*((volatile uint32_t *)(UART0_BASE+0x000))) // Data Register, UARTDR
#define UART0_UARTRSR                   (*((volatile uint32_t *)(UART0_BASE+0x004))) // Receive Status Register/Error Clear Register, UARTRSR/UARTECR
#define UART0_UARTFR                    (*((volatile uint32_t *)(UART0_BASE+0x018))) // Flag Register, UARTFR
#define UART0_UARTILPR                  (*((volatile uint32_t *)(UART0_BASE+0x020))) // IrDA Low-Power Counter Register, UARTILPR
#define UART0_UARTIBRD                  (*((volatile uint32_t *)(UART0_BASE+0x024))) // Integer Baud Rate Register, UARTIBRD
#define UART0_UARTFBRD                  (*((volatile uint32_t *)(UART0_BASE+0x028))) // Fractional Baud Rate Register, UARTFBRD
#define UART0_UARTLCR_H                 (*((volatile uint32_t *)(UART0_BASE+0x02c))) // Line Control Register, UARTLCR_H
#define UART0_UARTCR                    (*((volatile uint32_t *)(UART0_BASE+0x030))) // Control Register, UARTCR
#define UART0_UARTIFLS                  (*((volatile uint32_t *)(UART0_BASE+0x034))) // Interrupt FIFO Level Select Register, UARTIFLS
#define UART0_UARTIMSC                  (*((volatile uint32_t *)(UART0_BASE+0x038))) // Interrupt Mask Set/Clear Register, UARTIMSC
#define UART0_UARTRIS                   (*((volatile uint32_t *)(UART0_BASE+0x03c))) // Raw Interrupt Status Register, UARTRIS
#define UART0_UARTMIS                   (*((volatile uint32_t *)(UART0_BASE+0x040))) // Masked Interrupt Status Register, UARTMIS
#define UART0_UARTICR                   (*((volatile uint32_t *)(UART0_BASE+0x044))) // Interrupt Clear Register, UARTICR
#define UART0_UARTDMACR                 (*((volatile uint32_t *)(UART0_BASE+0x048))) // DMA Control Register, UARTDMACR
#define UART0_UARTPERIPHID0             (*((volatile uint32_t *)(UART0_BASE+0xfe0))) // UARTPeriphID0 Register
#define UART0_UARTPERIPHID1             (*((volatile uint32_t *)(UART0_BASE+0xfe4))) // UARTPeriphID1 Register
#define UART0_UARTPERIPHID2             (*((volatile uint32_t *)(UART0_BASE+0xfe8))) // UARTPeriphID2 Register
#define UART0_UARTPERIPHID3             (*((volatile uint32_t *)(UART0_BASE+0xfec))) // UARTPeriphID3 Register
#define UART0_UARTPCELLID0              (*((volatile uint32_t *)(UART0_BASE+0xff0))) // UARTPCellID0 Register
#define UART0_UARTPCELLID1              (*((volatile uint32_t *)(UART0_BASE+0xff4))) // UARTPCellID1 Register
#define UART0_UARTPCELLID2              (*((volatile uint32_t *)(UART0_BASE+0xff8))) // UARTPCellID2 Register
#define UART0_UARTPCELLID3              (*((volatile uint32_t *)(UART0_BASE+0xffc))) // UARTPCellID3 Register

// ROSC
#define ROSC_CTRL                       (*((volatile uint32_t *)(ROSC_BASE+0x00))) // Ring Oscillator control
#define ROSC_FREQA                      (*((volatile uint32_t *)(ROSC_BASE+0x04))) // Ring Oscillator frequency control A
#define ROSC_FREQB                      (*((volatile uint32_t *)(ROSC_BASE+0x08))) // Ring Oscillator frequency control B
#define ROSC_DORMANT                    (*((volatile uint32_t *)(ROSC_BASE+0x0c))) // Ring Oscillator pause control
#define ROSC_DIV                        (*((volatile uint32_t *)(ROSC_BASE+0x10))) // Controls the output divider
#define ROSC_PHASE                      (*((volatile uint32_t *)(ROSC_BASE+0x14))) // Controls the phase shifted output
#define ROSC_STATUS                     (*((volatile uint32_t *)(ROSC_BASE+0x18))) // Ring Oscillator Status
#define ROSC_RANDOMBIT                  (*((volatile uint32_t *)(ROSC_BASE+0x1c))) // Returns a 1 bit random value
#define ROSC_COUNT                      (*((volatile uint32_t *)(ROSC_BASE+0x20))) // A down counter running at the ROSC frequency which counts to zero and stops.

// SIO
#define SIO_CPUID                       (*((volatile uint32_t *)(SIO_BASE+0x000)))  // Processor core identifier
#define GPIO_IN                         (*((volatile uint32_t *)(SIO_BASE+0x004)))  // Input value for GPIO pins
#define GPIO_HI_IN                      (*((volatile uint32_t *)(SIO_BASE+0x008)))  // Input value for QSPI pins
#define GPIO_OUT                        (*((volatile uint32_t *)(SIO_BASE+0x010)))  // GPIO output value
#define GPIO_OUT_SET                    (*((volatile uint32_t *)(SIO_BASE+0x014)))  // GPIO output value set
#define GPIO_OUT_CLR                    (*((volatile uint32_t *)(SIO_BASE+0x018)))  // GPIO output value clear
#define GPIO_OUT_XOR                    (*((volatile uint32_t *)(SIO_BASE+0x01c)))  // GPIO output value XOR
#define GPIO_OE                         (*((volatile uint32_t *)(SIO_BASE+0x020)))  // GPIO output enable
#define GPIO_OE_SET                     (*((volatile uint32_t *)(SIO_BASE+0x024)))  // GPIO output enable set
#define GPIO_OE_CLR                     (*((volatile uint32_t *)(SIO_BASE+0x028)))  // GPIO output enable clear
#define GPIO_OE_XOR                     (*((volatile uint32_t *)(SIO_BASE+0x02c)))  // GPIO output enable XOR
#define GPIO_HI_OUT                     (*((volatile uint32_t *)(SIO_BASE+0x030)))  // QSPI output value
#define GPIO_HI_OUT_SET                 (*((volatile uint32_t *)(SIO_BASE+0x034)))  // QSPI output value set
#define GPIO_HI_OUT_CLR                 (*((volatile uint32_t *)(SIO_BASE+0x038)))  // QSPI output value clear
#define GPIO_HI_OUT_XOR                 (*((volatile uint32_t *)(SIO_BASE+0x03c)))  // QSPI output value XOR
#define GPIO_HI_OE                      (*((volatile uint32_t *)(SIO_BASE+0x040)))  // QSPI output enable
#define GPIO_HI_OE_SET                  (*((volatile uint32_t *)(SIO_BASE+0x044)))  // QSPI output enable set
#define GPIO_HI_OE_CLR                  (*((volatile uint32_t *)(SIO_BASE+0x048)))  // QSPI output enable clear
#define GPIO_HI_OE_XOR                  (*((volatile uint32_t *)(SIO_BASE+0x04c)))  // QSPI output enable XOR
#define FIFO_ST                         (*((volatile uint32_t *)(SIO_BASE+0x050)))  // Status register for inter-core FIFOs (mailboxes).
#define FIFO_WR                         (*((volatile uint32_t *)(SIO_BASE+0x054)))  // Write access to this cores TX FIFO
#define FIFO_RD                         (*((volatile uint32_t *)(SIO_BASE+0x058)))  // Read access to this cores RX FIFO
#define SPINLOCK_ST                     (*((volatile uint32_t *)(SIO_BASE+0x05c)))  // Spinlock state
#define DIV_UDIVIDEND                   (*((volatile uint32_t *)(SIO_BASE+0x060)))  // Divider unsigned dividend
#define DIV_UDIVISOR                    (*((volatile uint32_t *)(SIO_BASE+0x064)))  // Divider unsigned divisor
#define DIV_SDIVIDEND                   (*((volatile uint32_t *)(SIO_BASE+0x068)))  // Divider signed dividend
#define DIV_SDIVISOR                    (*((volatile uint32_t *)(SIO_BASE+0x06c)))  // Divider signed divisor
#define DIV_QUOTIENT                    (*((volatile uint32_t *)(SIO_BASE+0x070)))  // Divider result quotient
#define DIV_REMAINDER                   (*((volatile uint32_t *)(SIO_BASE+0x074)))  // Divider result remainder
#define DIV_CSR                         (*((volatile uint32_t *)(SIO_BASE+0x078)))  // Control and status register for divider.
#define INTERP0_ACCUM0                  (*((volatile uint32_t *)(SIO_BASE+0x080)))  // Read/write access to accumulator 0
#define INTERP0_ACCUM1                  (*((volatile uint32_t *)(SIO_BASE+0x084)))  // Read/write access to accumulator 1
#define INTERP0_BASE0                   (*((volatile uint32_t *)(SIO_BASE+0x088)))  // Read/write access to BASE0 register.
#define INTERP0_BASE1                   (*((volatile uint32_t *)(SIO_BASE+0x08c)))  // Read/write access to BASE1 register.
#define INTERP0_BASE2                   (*((volatile uint32_t *)(SIO_BASE+0x090)))  // Read/write access to BASE2 register.
#define INTERP0_POP_LANE0               (*((volatile uint32_t *)(SIO_BASE+0x094)))  // Read LANE0 result, and simultaneously write lane results to both accumulators (POP).
#define INTERP0_POP_LANE1               (*((volatile uint32_t *)(SIO_BASE+0x098)))  // Read LANE1 result, and simultaneously write lane results to both accumulators (POP).
#define INTERP0_POP_FULL                (*((volatile uint32_t *)(SIO_BASE+0x09c)))  // Read FULL result, and simultaneously write lane results to both accumulators (POP).
#define INTERP0_PEEK_LANE0              (*((volatile uint32_t *)(SIO_BASE+0x0a0)))  // Read LANE0 result, without altering any internal state (PEEK).
#define INTERP0_PEEK_LANE1              (*((volatile uint32_t *)(SIO_BASE+0x0a4)))  // Read LANE1 result, without altering any internal state (PEEK).
#define INTERP0_PEEK_FULL               (*((volatile uint32_t *)(SIO_BASE+0x0a8)))  // Read FULL result, without altering any internal state (PEEK).
#define INTERP0_CTRL_LANE0              (*((volatile uint32_t *)(SIO_BASE+0x0ac)))  // Control register for lane 0
#define INTERP0_CTRL_LANE1              (*((volatile uint32_t *)(SIO_BASE+0x0b0)))  // Control register for lane 1
#define INTERP0_ACCUM0_ADD              (*((volatile uint32_t *)(SIO_BASE+0x0b4)))  // Values written here are atomically added to ACCUM0
#define INTERP0_ACCUM1_ADD              (*((volatile uint32_t *)(SIO_BASE+0x0b8)))  // Values written here are atomically added to ACCUM1
#define INTERP0_BASE_1AND0              (*((volatile uint32_t *)(SIO_BASE+0x0bc)))  // On write, the lower 16 bits go to BASE0, upper bits to BASE1 simultaneously.
#define INTERP1_ACCUM0                  (*((volatile uint32_t *)(SIO_BASE+0x0c0)))  // Read/write access to accumulator 0
#define INTERP1_ACCUM1                  (*((volatile uint32_t *)(SIO_BASE+0x0c4)))  // Read/write access to accumulator 1
#define INTERP1_BASE0                   (*((volatile uint32_t *)(SIO_BASE+0x0c8)))  // Read/write access to BASE0 register.
#define INTERP1_BASE1                   (*((volatile uint32_t *)(SIO_BASE+0x0cc)))  // Read/write access to BASE1 register.
#define INTERP1_BASE2                   (*((volatile uint32_t *)(SIO_BASE+0x0d0)))  // Read/write access to BASE2 register.
#define INTERP1_POP_LANE0               (*((volatile uint32_t *)(SIO_BASE+0x0d4)))  // Read LANE0 result, and simultaneously write lane results to both accumulators (POP).
#define INTERP1_POP_LANE1               (*((volatile uint32_t *)(SIO_BASE+0x0d8)))  // Read LANE1 result, and simultaneously write lane results to both accumulators (POP).
#define INTERP1_POP_FULL                (*((volatile uint32_t *)(SIO_BASE+0x0dc)))  // Read FULL result, and simultaneously write lane results to both accumulators (POP).
#define INTERP1_PEEK_LANE0              (*((volatile uint32_t *)(SIO_BASE+0x0e0)))  // Read LANE0 result, without altering any internal state (PEEK).
#define INTERP1_PEEK_LANE1              (*((volatile uint32_t *)(SIO_BASE+0x0e4)))  // Read LANE1 result, without altering any internal state (PEEK).
#define INTERP1_PEEK_FULL               (*((volatile uint32_t *)(SIO_BASE+0x0e8)))  // Read FULL result, without altering any internal state (PEEK).
#define INTERP1_CTRL_LANE0              (*((volatile uint32_t *)(SIO_BASE+0x0ec)))  // Control register for lane 0
#define INTERP1_CTRL_LANE1              (*((volatile uint32_t *)(SIO_BASE+0x0f0)))  // Control register for lane 1
#define INTERP1_ACCUM0_ADD              (*((volatile uint32_t *)(SIO_BASE+0x0f4)))  // Values written here are atomically added to ACCUM0
#define INTERP1_ACCUM1_ADD              (*((volatile uint32_t *)(SIO_BASE+0x0f8)))  // Values written here are atomically added to ACCUM1
#define INTERP1_BASE_1AND0              (*((volatile uint32_t *)(SIO_BASE+0x0fc)))  // On write, the lower 16 bits go to BASE0, upper bits to BASE1 simultaneously.
#define SPINLOCK0                       (*((volatile uint32_t *)(SIO_BASE+0x100)))  // Spinlock register 0
#define SPINLOCK1                       (*((volatile uint32_t *)(SIO_BASE+0x104)))  // Spinlock register 1
#define SPINLOCK2                       (*((volatile uint32_t *)(SIO_BASE+0x108)))  // Spinlock register 2
#define SPINLOCK3                       (*((volatile uint32_t *)(SIO_BASE+0x10c)))  // Spinlock register 3
#define SPINLOCK4                       (*((volatile uint32_t *)(SIO_BASE+0x110)))  // Spinlock register 4
#define SPINLOCK5                       (*((volatile uint32_t *)(SIO_BASE+0x114)))  // Spinlock register 5
#define SPINLOCK6                       (*((volatile uint32_t *)(SIO_BASE+0x118)))  // Spinlock register 6
#define SPINLOCK7                       (*((volatile uint32_t *)(SIO_BASE+0x11c)))  // Spinlock register 7
#define SPINLOCK8                       (*((volatile uint32_t *)(SIO_BASE+0x120)))  // Spinlock register 8
#define SPINLOCK9                       (*((volatile uint32_t *)(SIO_BASE+0x124)))  // Spinlock register 9
#define SPINLOCK10                      (*((volatile uint32_t *)(SIO_BASE+0x128)))  // Spinlock register 10
#define SPINLOCK11                      (*((volatile uint32_t *)(SIO_BASE+0x12c)))  // Spinlock register 11
#define SPINLOCK12                      (*((volatile uint32_t *)(SIO_BASE+0x130)))  // Spinlock register 12
#define SPINLOCK13                      (*((volatile uint32_t *)(SIO_BASE+0x134)))  // Spinlock register 13
#define SPINLOCK14                      (*((volatile uint32_t *)(SIO_BASE+0x138)))  // Spinlock register 14
#define SPINLOCK15                      (*((volatile uint32_t *)(SIO_BASE+0x13c)))  // Spinlock register 15
#define SPINLOCK16                      (*((volatile uint32_t *)(SIO_BASE+0x140)))  // Spinlock register 16
#define SPINLOCK17                      (*((volatile uint32_t *)(SIO_BASE+0x144)))  // Spinlock register 17
#define SPINLOCK18                      (*((volatile uint32_t *)(SIO_BASE+0x148)))  // Spinlock register 18
#define SPINLOCK19                      (*((volatile uint32_t *)(SIO_BASE+0x14c)))  // Spinlock register 19
#define SPINLOCK20                      (*((volatile uint32_t *)(SIO_BASE+0x150)))  // Spinlock register 20
#define SPINLOCK21                      (*((volatile uint32_t *)(SIO_BASE+0x154)))  // Spinlock register 21
#define SPINLOCK22                      (*((volatile uint32_t *)(SIO_BASE+0x158)))  // Spinlock register 22
#define SPINLOCK23                      (*((volatile uint32_t *)(SIO_BASE+0x15c)))  // Spinlock register 23
#define SPINLOCK24                      (*((volatile uint32_t *)(SIO_BASE+0x160)))  // Spinlock register 24
#define SPINLOCK25                      (*((volatile uint32_t *)(SIO_BASE+0x164)))  // Spinlock register 25
#define SPINLOCK26                      (*((volatile uint32_t *)(SIO_BASE+0x168)))  // Spinlock register 26
#define SPINLOCK27                      (*((volatile uint32_t *)(SIO_BASE+0x16c)))  // Spinlock register 27
#define SPINLOCK28                      (*((volatile uint32_t *)(SIO_BASE+0x170)))  // Spinlock register 28
#define SPINLOCK29                      (*((volatile uint32_t *)(SIO_BASE+0x174)))  // Spinlock register 29
#define SPINLOCK30                      (*((volatile uint32_t *)(SIO_BASE+0x178)))  // Spinlock register 30
#define SPINLOCK31                      (*((volatile uint32_t *)(SIO_BASE+0x17c)))  // Spinlock register 31

// ARM M0+ registers
#define SYST_CSR                        (*((volatile uint32_t *)(PPB_BASE+0xe010))) // SysTick Control and Status Register
#define SYST_RVR                        (*((volatile uint32_t *)(PPB_BASE+0xe014))) // SysTick Reload Value Register
#define SYST_CVR                        (*((volatile uint32_t *)(PPB_BASE+0xe018))) // SysTick Current Value Register
#define SYST_CALIB                      (*((volatile uint32_t *)(PPB_BASE+0xe01c))) // SysTick Calibration Value Register
#define NVIC_ISER                       (*((volatile uint32_t *)(PPB_BASE+0xe100))) // Interrupt Set-Enable Register
#define NVIC_ICER                       (*((volatile uint32_t *)(PPB_BASE+0xe180))) // Interrupt Clear-Enable Register
#define NVIC_ISPR                       (*((volatile uint32_t *)(PPB_BASE+0xe200))) // Interrupt Set-Pending Register
#define NVIC_ICPR                       (*((volatile uint32_t *)(PPB_BASE+0xe280))) // Interrupt Clear-Pending Register
#define NVIC_IPR0                       (*((volatile uint32_t *)(PPB_BASE+0xe400))) // Interrupt Priority Register 0
#define NVIC_IPR1                       (*((volatile uint32_t *)(PPB_BASE+0xe404))) // Interrupt Priority Register 1
#define NVIC_IPR2                       (*((volatile uint32_t *)(PPB_BASE+0xe408))) // Interrupt Priority Register 2
#define NVIC_IPR3                       (*((volatile uint32_t *)(PPB_BASE+0xe40c))) // Interrupt Priority Register 3
#define NVIC_IPR4                       (*((volatile uint32_t *)(PPB_BASE+0xe410))) // Interrupt Priority Register 4
#define NVIC_IPR5                       (*((volatile uint32_t *)(PPB_BASE+0xe414))) // Interrupt Priority Register 5
#define NVIC_IPR6                       (*((volatile uint32_t *)(PPB_BASE+0xe418))) // Interrupt Priority Register 6
#define NVIC_IPR7                       (*((volatile uint32_t *)(PPB_BASE+0xe41c))) // Interrupt Priority Register 7
#define CORE_CPUID                      (*((volatile uint32_t *)(PPB_BASE+0xed00))) // CPUID Base Register
#define ICSR                            (*((volatile uint32_t *)(PPB_BASE+0xed04))) // Interrupt Control and State Register
#define VTOR                            (*((volatile uint32_t *)(PPB_BASE+0xed08))) // Vector Table Offset Register
#define AIRCR                           (*((volatile uint32_t *)(PPB_BASE+0xed0c))) // Application Interrupt and Reset Control Register
#define SCR                             (*((volatile uint32_t *)(PPB_BASE+0xed10))) // System Control Register
#define CCR                             (*((volatile uint32_t *)(PPB_BASE+0xed14))) // Configuration and Control Register
#define SHPR2                           (*((volatile uint32_t *)(PPB_BASE+0xed1c))) // System Handler Priority Register 2
#define SHPR3                           (*((volatile uint32_t *)(PPB_BASE+0xed20))) // System Handler Priority Register 3
#define SHCSR                           (*((volatile uint32_t *)(PPB_BASE+0xed24))) // System Handler Control and State Register
#define MPU_TYPE                        (*((volatile uint32_t *)(PPB_BASE+0xed90))) // MPU Type Register
#define MPU_CTRL                        (*((volatile uint32_t *)(PPB_BASE+0xed94))) // MPU Control Register
#define MPU_RNR                         (*((volatile uint32_t *)(PPB_BASE+0xed98))) // MPU Region Number Register
#define MPU_RBAR                        (*((volatile uint32_t *)(PPB_BASE+0xed9c))) // MPU Region Base Address Register
#define MPU_RASR                        (*((volatile uint32_t *)(PPB_BASE+0xeda0))) // MPU Region Attribute and Size Register

// XIP_SSI
#define CTRLR0                          (*((volatile uint32_t *)(XIP_SSI_BASE+0x00))) // Control register 0
#define CTRLR1                          (*((volatile uint32_t *)(XIP_SSI_BASE+0x04))) // Master Control register 1
#define SSIENR                          (*((volatile uint32_t *)(XIP_SSI_BASE+0x08))) // SSI Enable
#define MWCR                            (*((volatile uint32_t *)(XIP_SSI_BASE+0x0c))) // Microwire Control
#define SER                             (*((volatile uint32_t *)(XIP_SSI_BASE+0x10))) // Slave enable
#define BAUDR                           (*((volatile uint32_t *)(XIP_SSI_BASE+0x14))) // Baud rate
#define TXFTLR                          (*((volatile uint32_t *)(XIP_SSI_BASE+0x18))) // TX FIFO threshold level
#define RXFTLR                          (*((volatile uint32_t *)(XIP_SSI_BASE+0x1c))) // RX FIFO threshold level
#define TXFLR                           (*((volatile uint32_t *)(XIP_SSI_BASE+0x20))) // TX FIFO level
#define RXFLR                           (*((volatile uint32_t *)(XIP_SSI_BASE+0x24))) // RX FIFO level
#define SR                              (*((volatile uint32_t *)(XIP_SSI_BASE+0x28))) // Status register
#define IMR                             (*((volatile uint32_t *)(XIP_SSI_BASE+0x2c))) // Interrupt mask
#define ISR                             (*((volatile uint32_t *)(XIP_SSI_BASE+0x30))) // Interrupt status
#define RISR                            (*((volatile uint32_t *)(XIP_SSI_BASE+0x34))) // Raw interrupt status
#define TXOICR                          (*((volatile uint32_t *)(XIP_SSI_BASE+0x38))) // TX FIFO overflow interrupt clear
#define RXOICR                          (*((volatile uint32_t *)(XIP_SSI_BASE+0x3c))) // RX FIFO overflow interrupt clear
#define RXUICR                          (*((volatile uint32_t *)(XIP_SSI_BASE+0x40))) // RX FIFO underflow interrupt clear
#define MSTICR                          (*((volatile uint32_t *)(XIP_SSI_BASE+0x44))) // Multi-master interrupt clear
#define ICR                             (*((volatile uint32_t *)(XIP_SSI_BASE+0x48))) // Interrupt clear
#define DMACR                           (*((volatile uint32_t *)(XIP_SSI_BASE+0x4c))) // DMA control
#define DMATDLR                         (*((volatile uint32_t *)(XIP_SSI_BASE+0x50))) // DMA TX data level
#define DMARDLR                         (*((volatile uint32_t *)(XIP_SSI_BASE+0x54))) // DMA RX data level
#define IDR                             (*((volatile uint32_t *)(XIP_SSI_BASE+0x58))) // Identification register
#define SSI_VERSION_ID                  (*((volatile uint32_t *)(XIP_SSI_BASE+0x5c))) // Version ID
#define DR0                             (*((volatile uint32_t *)(XIP_SSI_BASE+0x60))) // Data Register 0 (of 36)
#define RX_SAMPLE_DLY                   (*((volatile uint32_t *)(XIP_SSI_BASE+0xf0))) // RX sample delay
#define SPI_CTRLR0                      (*((volatile uint32_t *)(XIP_SSI_BASE+0xf4))) // SPI control
#define TXD_DRIVE_EDGE                  (*((volatile uint32_t *)(XIP_SSI_BASE+0xf8))) // TX drive edge

// ICSR offsets
#define NMIPENDSET_OFFSET     31
#define PENDSVSET_OFFSET      28
#define PENDSVCLR_OFFSET      27
#define PENDSTSET_OFFSET      26
#define PENDSTCLR_OFFSET      25
#define ISRPREEMPT_OFFSET     23
#define ISRPENDING_OFFSET     22
#define VECTPENDING_OFFSET    12
#define VECTACTIVE_OFFSET     0

#endif  // __RP2040__