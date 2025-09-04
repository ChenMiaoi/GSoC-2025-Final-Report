<p align="center">
  <a href="https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/tests/jump-label-test/README.md">English</a> •
  <a href="https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/tests/jump-label-test/README_zh.md">简体中文</a>
</p>

# Jump Label Test Module

[![English](https://img.shields.io/badge/Language-English-blue)](https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/tests/jump-label-test/README)
[![简体中文](https://img.shields.io/badge/语言-简体中文-red)](https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/tests/jump-label-test/README_zh.md)

这里介绍针对我对于OpenRISC架构移植的jump_label的测试模块的使用方式。请您准备好如下环境：

- QEMU OR1K
- or1k-none-linux-musl- toolchain
- 已经应用了我的patches的内核

如下是我的本地环境，仅供参考：

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

如果您目前还没有准备好这些环境，请参考这一节进行环境配置[How To Check My Works](https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/README.md#how-to-check-my-works)。

按照上述方式，您能够在如下的位置找到对应的工具链：

- `QEMU`: $HOME/work/openrisc/qemu/build/qemu-or1k
- `or1k-none-linux-musl-`: [or1k-none-linux-musl DownLoad URL](https://github.com/stffrdhrn/or1k-toolchain-build/releases/download/or1k-15.1.0-20250621/or1k-none-linux-musl-15.1.0-20250621.tar.xz)

准备好环境后，就可以开始进行测试了：

``` bash
$ make -j12 LC_ALL=en_US.UTF-8 ARCH=openrisc CROSS_COMPILE=or1k-none-linux-musl- CONFIG_INITRAMFS_SOURCE= virt_defconfig

$ make -j12 LC_ALL=en_US.UTF-8 ARCH=openrisc CROSS_COMPILE=or1k-none-linux-musl- CONFIG_INITRAMFS_SOURCE= menuconfig
```

**这里需要注意，在OpenRISC架构中，defconfig没有支持MODULES，因此需要您手动开启两个选项**：

- CONFIG_MODULES=y
- CONFIG_MODULE_UNLOAD=y

然后进行编译，由于我们需要测试模块，因此还需要编译出module：

``` bash
$ make -j12 LC_ALL=en_US.UTF-8 ARCH=openrisc CROSS_COMPILE=or1k-none-linux-musl- CONFIG_INITRAMFS_SOURCE=

$ make -j12 LC_ALL=en_US.UTF-8 ARCH=openrisc CROSS_COMPILE=or1k-none-linux-musl- CONFIG_INITRAMFS_SOURCE= INSTALL_MOD_PATH=$HOME/work/or1k-vm-dir/modules modules_install
```

需要注意的是，这里进行`modules_install`后，您需要记录下一个路径：

``` bash
SYMLINK $HOME/work/or1k-vm-dir/modules/lib/modules/6.17.0-rc3-00017-gbb2483564597/build
```

然后，您可以在当前目录经看到对应的Makefile，请您修改Makefile中的`KERNEL_SRC`或者直接指定`KERNEL_SRC`为上面所记录的路径即可。

现在，您就可以直接进行编译，并且产生`jump_label_test.ko`模块，请您参考[How To Check My Works](https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/README.md#how-to-check-my-works)以及[How To Test](https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/more-details/working/how_to_test.md)章节进行配置NFS，将该模块上传至启动的虚拟机中，当然，如何启动虚拟机也在上面两个章节中。

然后就可以插入测试模块进行测试：

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

测试结果显示，使用jump_label相比常规if语句性能提升约43%。
