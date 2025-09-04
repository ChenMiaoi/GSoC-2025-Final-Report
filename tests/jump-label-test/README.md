<p align="center">
  <a href="https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/tests/jump-label-test/README.md">English</a> •
  <a href="https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/tests/jump-label-test/README_zh.md">简体中文</a>
</p>

# Jump Label Test Module

[![English](https://img.shields.io/badge/Language-English-blue)](https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/tests/jump-label-test/README)
[![简体中文](https://img.shields.io/badge/语言-简体中文-red)](https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/tests/jump-label-test/README_zh.md)

Here is the introduction to the usage of the test module for the jump_label I ported to the OpenRISC architecture. Please prepare the following environment:

- QEMU OR1K
- or1k-none-linux-musl- toolchain
- A kernel source that has already applied my patches

The following is my local environment, for reference only:

``` bash
$ qemu-or1k --version
qemu-or1k version 10.0.50
Copyright (c) 2003-2025 Fabrice Bellard and the QEMU Project developers

$ or1k-none-linux-musl-gcc -v
Using built-in specs.
COLLECT_GCC=or1k-none-linux-musl-gcc
COLLECT_LTO_WRAPPER=/home/nya/work/openrisc/linux-on-or1ksim/or1k-none-linux-musl/bin/../libexec/gcc/or1k-none-linux-musl/15.1.0/lto-wrapper
Target: or1k-none-linux-musl
Thread model: posix
Supported LTO compression algorithms: zlib
gcc version 15.1.0 (GCC) 
```

If you haven’t prepared these environments yet, please refer to this section for environment configuration: [How To Check My Works](https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/README.md#how-to-check-my-works).

Following the instructions above, you can find the corresponding toolchain at the following locations:

- `QEMU`: $HOME/work/openrisc/qemu/build/qemu-or1k
- `or1k-none-linux-musl-`: [or1k-none-linux-musl DownLoad URL](https://github.com/stffrdhrn/or1k-toolchain-build/releases/download/or1k-15.1.0-20250621/or1k-none-linux-musl-15.1.0-20250621.tar.xz)

Once the environment is prepared, you can proceed with testing:

``` bash
$ make -j12 LC_ALL=en_US.UTF-8 ARCH=openrisc CROSS_COMPILE=or1k-none-linux-musl- CONFIG_INITRAMFS_SOURCE= virt_defconfig

$ make -j12 LC_ALL=en_US.UTF-8 ARCH=openrisc CROSS_COMPILE=or1k-none-linux-musl- CONFIG_INITRAMFS_SOURCE= menuconfig
```

​​**Please note that in the OpenRISC architecture, the defconfig does not support MODULES by default, so you need to manually enable two options​​**:

- CONFIG_MODULES=y
- CONFIG_MODULE_UNLOAD=y

Then proceed with the compilation. Since we need to test the module, you must also compile the modules:

``` bash
$ make -j12 LC_ALL=en_US.UTF-8 ARCH=openrisc CROSS_COMPILE=or1k-none-linux-musl- CONFIG_INITRAMFS_SOURCE=

$ make -j12 LC_ALL=en_US.UTF-8 ARCH=openrisc CROSS_COMPILE=or1k-none-linux-musl- CONFIG_INITRAMFS_SOURCE= INSTALL_MOD_PATH=$HOME/work/or1k-vm-dir/modules modules_install
```

Please note that after performing modules_install, you need to record the following path, such as:

``` bash
SYMLINK $HOME/work/or1k-vm-dir/modules/lib/modules/6.17.0-rc3-00017-gbb2483564597/build
```

Then, you can see the corresponding Makefile in the current directory. Please modify the KERNEL_SRC in the Makefile or directly specify KERNEL_SRC as the path recorded above.

Now, you can proceed with compilation directly and generate the jump_label_test.ko module. Please refer to [How To Check My Works](https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/README.md#how-to-check-my-works) and [How To Test](https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/more-details/working/how_to_test.md) for instructions on configuring NFS and uploading this module to the started virtual machine. Of course, how to start the virtual machine is also covered in the above two sections.

You can then insert the test module for testing:

``` bash
$ insmod jump_label_test.ko
[   18.530000] jump_label_test: loading out-of-tree module taints kernel.
[   18.530000] Jump label performance test module loaded
[   21.130000] Normal branch time: 1217991300 ns (121 ns per iteration)
[   21.130000] Jump label (false) time: 690088100 ns (69 ns per iteration)
[   21.130000] Jump label (true) time: 691661000 ns (69 ns per iteration)
$ rmmod jump_label_test.ko 
[   37.700000] Jump label test module unloaded
```

The test results show that using jump_label provides approximately a 43% performance improvement compared to conventional if statements.
