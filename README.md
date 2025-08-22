# GSoC-2025-Final-Report

各位，这是由学生ChenMiao和导师Shorne所完成的[GSoC2025 OpenRISC Linux Kernel Feature Develepment](https://summerofcode.withgoogle.com/programs/2025/projects/qYsJ8YEg)最终汇报。首先，我需要阐明该项目的目标以及我的完成情况(目前会实时更新，因为各种patch正在提交等待社区合并中)。

| 项目主要目标 | 项目次要目标 | 完成情况 |
| :---: | :---: | :---: |
| OpenRISC JumpLabel | \ | 已完成 [\[PATCH v3 0/2\] openrisc: Support fixmap and jump_label](https://lore.kernel.org/openrisc/20250814032717.785395-1-chenmiao.ku@gmail.com/T/#t) |
|  |  | 历史讨论记录 1.[\[PATCH 0/2\] Support fixmap and jump_label for openrisc](https://lore.kernel.org/openrisc/20250805084926.4125564-1-chenmiao.ku@gmail.com/T/#u) |
|  |  | 历史讨论记录 2.[\[PATCH v2 0/2\] Support fixmap and jump_label for openrisc](https://lore.kernel.org/openrisc/20250806020520.570988-1-chenmiao.ku@gmail.com/T/#t) |
| | fixmap/text patching 机制 | 已完成，参见[\[PATCH v3 1/2\] openrisc: Add text patching API support](https://lore.kernel.org/openrisc/20250814032717.785395-2-chenmiao.ku@gmail.com/T/#u) |
| | jump_label核心实现 | 已完成，参见[\[PATCH v3 2/2\] openrisc: Add jump label support](https://lore.kernel.org/openrisc/20250814032717.785395-3-chenmiao.ku@gmail.com/T/#u) |
| OpenRISC Ftrace | \ | 未全部完成 |
| static ftrace | \ | 已完成，参见[尚未提起pr](xxx) |
| | basic static ftrace | 已完成，参见[尚未提起pr](xxx) |
| | static ftrace with graph | 已完成，参见[尚未提起pr](xxx) |
| dynamic ftrace | \ | 未完成 |
| | basic dynamic ftrace | 未完成 |
| | dynamic ftrace with graph | 未完成 |
| | syscall ftrace | 未完成 |

关于每个任务的patch，不再通过公开的各种方式进行额外赘述，每一个上述表格所给出的链接中，均包含了该目标的实现patch，以及关于社区的讨论记录。

## How To Check MY Works

如果您想要检阅我的工作情况，请参考这一部分。我的工作环境如下所示：

``` bash
OS Verison: Ubuntu 24.10 x86_64 6.11.0-29-generic
OR1K-GCC Version: or1k-linux-gcc version 14.2.0 (GCC)
OR1K-GDB Version: GNU gdb (GDB) 17.0.50.20250605-git
Linux Kernel Source Code: master branch
```

由于OpenRISC架构并没有一款能够使用的开发板或者任何PC端，因此无可避免地我们需要使用交叉编译配合QEMU进行运行测试。那么，您需要根据下方所给出的步骤进行环境配置，直至可以运行一个OR1K架构的Linux虚拟机。这时才能够进行测试(由于目前所有patch均未合并进内核，因此您需要手动apply所有patch，并从源码编译后启动Linux)。

### Prepare OpenRISC Env For Linux Build

OpenRISC有指定的[tutorials](https://openrisc.io/tutorials/docs/Linux.html)，因此我们的环境准备则是参考tutorials进行的。并且，OpenRISC社区提供了or1k-utils，这使得我们的配置步骤有别于tutorials，因此，tutorials仅供参考(后续会将这一部署方式提交到tutorials中)。

我们的敏捷开发主要采用了QEMU+Linux+toolchain+utils，因此我们需要准备以下环境：

``` bash
OR1K_QEMU_URL="https://gitlab.com/qemu-project/qemu.git"
OR1K_LINUX_URL="https://github.com/torvalds/linux.git"
OR1K_UTILS_URL="https://github.com/stffrdhrn/or1k-utils.git"
OR1K_ROOTFS_URL="https://github.com/stffrdhrn/or1k-rootfs-build/releases/download/or1k-20250417/buildroot-qemu-rootfs-20250417.tar.xz"
OR1K_TOOLCHAIN_URL="https://github.com/stffrdhrn/or1k-toolchain-build/releases/download/or1k-14.2.0-20250418/or1k-linux-14.2.0-20250418.tar.xz"
```

对于，or1k-utils而言，所有的环境都在`$HOME/work`中，因此您需要参考如下目录结构进行配置所需环境：

``` bash
tree -L 2 work/
work/
├── gnu-toolchain
│   └── local
├── linux
└── openrisc
    ├── buildroot-rootfs
    ├── or1k-utils
    ├── qemu
    └── toolchain
```

需要注意的是，我曾经在配置工具链上花费了太多时间，但后续Shorne将我纠正过来，我们并不需要从源码进行编译各种工具链，只需要下载OpenRISC已经准备好的二进制压缩包，并解压到对应目录即可。

**如果您觉得手动下载并部署到对应位置较为麻烦，您可以参考[GSOC2025-OpenRISC-Download](https://github.com/ChenMiaoi/GSO2025-OpenRISC/tree/main)一键部署(后续移植计划到tutorials中)**。在这个脚本中，我所作的就两件事情，下载上面URL的对应资源，将下载的资源放置到合适的位置。

``` bash
download OR1K_QEMU_URL $HOME/work/openrisc/qemu
download OR1K_LINUX_URL $HOME/work/linux
download OR1K_UTOLS_URL $HOME/work/openrisc/or1k-utils
download OR1K_ROOTFS_URL $HOME/work/openrisc/buildroot-rootfs
download OR1K_TOOLCHAIN_URL $HOME/work/gnu-toolchain/local
```

为了方便，通常我会将toolchain加入到环境变量中：

``` bash
export PATH=$PATH:$HOME/work/gnu-toolchain/local/bin
source ~/.bashrc

or1k-linux-gcc -v
Using built-in specs.
COLLECT_GCC=or1k-linux-gcc
Target: or1k-linux
gcc version 14.2.0 (GCC)
```

完成上述步骤后，我们的QEMU还是源码状态，因此需要交叉编译出OpenRISC架构的qemu二进制，因此参考or1k-utils中的步骤，如下可以直接进行编译(前提是你的QEMU源码确实是按照上述步骤放置在指定位置的)：

``` bash
(now, in your qemu source code)qemu$ mkdir build
qemu$ ../../or1k-utils/qemu/config.qemu
qemu$ make -j$(nproc)

qemu$ ./build/qemu-system-or1k --version
QEMU emulator version 10.0.50
Copyright (c) 2003-2025 Fabrice Bellard and the QEMU Project developers
```

编译完成后，我们不需要执行`make install`，这样反而会干扰环境，这里生成的二进制文件，能够被or1k-utils正确索引到。

然后就可以开始验证整体环境是否可用，以下用`(or1k-utils)`表示当前在or1k-utils目录中(**请确保您的所有环境均按照上述路径配置**)：

``` bash
(or1k-utils)$ ./scripts/make-or1k-linux virt_defconfig
(or1k-utils)$ ./scripts/make-or1k-linux 
...
  NM      .tmp_vmlinux2.syms
  KSYMS   .tmp_vmlinux2.kallsyms.S
  AS      .tmp_vmlinux2.kallsyms.o
  LD      vmlinux
  NM      System.map
  OBJCOPY arch/openrisc/boot/vmlinux.bin

(or1k-utils)$ ./scripts/qemu-or1k-linux
...
INIT: Entering runlevel: 3
Seeding 256 bits and crediting
Saving 256 bits of creditable seed for next boot
Starting syslogd: OK
Starting klogd: OK
Running sysctl: OK
Starting network: OK
Starting sntp: sntp 4.2.8p18@1.4062-o Wed Apr 16 22:01:36 UTC 2025 (1)
libgcc_s.so.1 must be installed for pthread_exit to work
/etc/init.d/S48sntp: line 15:   128 Aborted                 /usr/bin/$DAEMON $SNTP_ARGS -K $SNTP_KEY_CACHE $SNTP_SERVERS
FAIL
Starting crond: OK
Starting sshd: OK

Welcome to Linux on OpenRISC
buildroot login: 
```

通过`root`用户直接回车登录，键入`uname -a`后可以看到如下信息：

``` bash
  _      __    __
 | | /| / /__ / /______  __ _  ___
 | |/ |/ / -_) / __/ _ \/  ' \/ -_)
 |__/|__/\__/_/\__/\___/_/_/_/\__/
                   / /____
                  / __/ _ \
  ____     _____  \__/\___/________  ______
 / __ \___<  / /_____| | / /  _/ _ \/_  __/
/ /_/ / __/ /  '_/___/ |/ // // , _/ / /
\____/_/ /_/_/\_\    |___/___/_/|_| /_/

 32-bit OpenRISC CPUs on a QEMU Virt Platform

(virt linux)root$ uname -a
Linux buildroot 6.16.0-12136-g51673daab9f4 #1 SMP Mon Aug 18 03:20:20 UTC 2025 openrisc GNU/Linux
```

至此，一个最为快捷的OpenRISC开发环境部署完毕。

### Apply All Patches To Check Works

当你部署完OpenRISC QEMU开发环境后，就可以进行验证工作了。我所提交的patch位于`patches`下。通过如下命令应用这些patches：

``` bash
(work/linux)$ git am *.patch
(or1k-utils)$ ./scripts/make-or1k-linux virt_defconfig
(or1k-utils)$ ./scripts/make-or1k-linux
```

可惜的是，我当未完成关于patch的测试，因此能够正常启动说明patches运作良好，后续会提供关于patches的测试。

## What Did I Do the Core Job

To Be Done... 这里会讲解我所作工作的一些核心点以及为什么需要这样做。

- [text patching](./more-details/working/text_patching.md)
- [jump label](./more-details/working/jump_label.md)
- [static ftrace](./more-details/working/static_ftrace.md)
- [dynamic ftrace](./more-details/working/dynamic_ftrace.md)
- [syscall tracing](./more-details/working/syscall_tracing.md)

## What Have I Done Wrong

To Be Done...

## Future

后续打算继续在OpenRISC社区深入，从OpenRISC中学习从架构到Linux上层。首先需要完成`dynamic ftrace`的移植。

- [ ] 完善dynamic ftrace移植
- [ ] 完善syscall ftrace移植
- [ ] ebpf移植
- [ ] OpenRISC整体架构学习
- [ ] ...
