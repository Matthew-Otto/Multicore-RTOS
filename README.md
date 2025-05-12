# Multicore-RTOS

A simple real-time OS for the Raspberry Pi Pico with multicore scheduling support.

This implementation is completely bare metal. It does not rely on Pico SDK or any other libraries.

I do not guarantee this implementation to be completely bug free.



## Dependencies

* arm-none-eabi-gcc (and friends)
* openocd


## References

- [RP2040 datasheet](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf)
- [ARMv6-M Architecture Reference Manual](https://documentation-service.arm.com/static/5f8ff05ef86e16515cdbf826)
- [Good baremetal examples for RP2040](https://github.com/carlosftm/RPi-Pico-Baremetal)