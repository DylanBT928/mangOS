<div align="center">
  
# mangOS 🥭

**Custom 64-bit operating system built with C and Limine**

</div>

<p>mangOS is a 64-bit x86-64 operating system built using C. It is powered by the Limine bootloader and utilizes the GNU Make build system. Its long-term direction is unknown for now, and the project may continue to grow or conclude as our goals evolve. Nonetheless, we are thrilled to learn about opearting systems and see where it leads us.</p>

## Prerequisites

- x86_64-elf Cross-Compiler
- GNU Make
- QEMU
- xorriso

> **Note:** You can build the cross-compiler by following the [OSDev GCC Cross-Compiler Guide](https://wiki.osdev.org/GCC_Cross-Compiler).

## Building

The build process is automated by the GNUmakefile. 

#### Build and Run

```bash
make run         # Build and run the ISO in QEMU with serial output
```

#### Other Commands

```bash
make iso         # Build a bootable ISO
make run         # Launch the ISO in QEMU with serial output
make clean       # Remove all build artifacts
```

#### macOS

```bash
make all TOOLCHAIN=x86_64-elf
make iso TOOLCHAIN=x86_64-elf
make run-serial TOOLCHAIN=x86_64-elf
make run TOOLCHAIN=x86_64-elf
```

## References

- [Operating Systems: Three Easy Pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/)
- [OSDev Wiki](https://wiki.osdev.org/)

## License

This project is licensed under the Apache License 2.0. See the [LICENSE](./LICENSE) file for details.
