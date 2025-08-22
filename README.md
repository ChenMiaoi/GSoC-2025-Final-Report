<p align="center">
  <a href="https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/README.md">English</a> •
  <a href="https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/README_zh.md">简体中文</a>
</p>

# GSoC-2025-Final-Report

[![English](https://img.shields.io/badge/Language-English-blue)](https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/README)
[![简体中文](https://img.shields.io/badge/语言-简体中文-red)](https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/README_zh.md)

Everyone, this is the final report of the [GSoC2025 OpenRISC Linux Kernel Feature Develepment](https://summerofcode.withgoogle.com/programs/2025/projects/qYsJ8YEg) completed by student ChenMiao and mentor Shorne. First, I need to clarify the goals of this project and my completion status (it will be updated in real time as various patches are being submitted and awaiting community integration).

| Primary Project Goals | Secondary Project Goals | Completion Status |
| :---: | :---: | :---: |
| OpenRISC JumpLabel | \ | Completed [\[PATCH v3 0/2\] openrisc: Support fixmap and jump_label](https://lore.kernel.org/openrisc/20250814032717.785395-1-chenmiao.ku@gmail.com/T/#t) |
|  |  | Historical Discussion Record 1.[\[PATCH 0/2\] Support fixmap and jump_label for openrisc](https://lore.kernel.org/openrisc/20250805084926.4125564-1-chenmiao.ku@gmail.com/T/#u) |
|  |  | Historical Discussion Record 2.[\[PATCH v2 0/2\] Support fixmap and jump_label for openrisc](https://lore.kernel.org/openrisc/20250806020520.570988-1-chenmiao.ku@gmail.com/T/#t) |
| | fixmap/text patching mechanism | Completed, see [\[PATCH v3 1/2\] openrisc: Add text patching API support](https://lore.kernel.org/openrisc/20250814032717.785395-2-chenmiao.ku@gmail.com/T/#u) |
| | jump_label core implementation | Completed, see [\[PATCH v3 2/2\] openrisc: Add jump label support](https://lore.kernel.org/openrisc/20250814032717.785395-3-chenmiao.ku@gmail.com/T/#u) |
| OpenRISC Ftrace | \ | Not fully completed |
| static ftrace | \ | Completed, see [PR not yet submitted](xxx) |
| | basic static ftrace | Completed, see [PR not yet submitted](xxx) |
| | static ftrace with graph | Completed, see [PR not yet submitted](xxx) |
| dynamic ftrace | \ | Not completed |
| | basic dynamic ftrace | Not completed |
| | dynamic ftrace with graph | Not completed |
| | syscall ftrace | Not completed |

Regarding the patches for each task, additional details will not be provided through various public channels. Each of the links provided in the table above includes the implementation patches for the corresponding goal, as well as the discussion records from the community.

## How To Check MY Works

If you would like to review my work, please refer to this section. My working environment is as follows:

``` bash
OS Verison: Ubuntu 24.10 x86_64 6.11.0-29-generic
OR1K-GCC Version: or1k-linux-gcc version 14.2.0 (GCC)
OR1K-GDB Version: GNU gdb (GDB) 17.0.50.20250605-git
Linux Kernel Source Code: master branch
```

Since the OpenRISC architecture does not have a commercially available development board or any PC-compatible hardware, it is inevitable that we must use cross-compilation in combination with QEMU for runtime testing. Therefore, you will need to configure the environment according to the steps provided below until you can successfully run a Linux virtual machine with the OR1K architecture. Only then will testing be possible (as none of the patches have been merged into the kernel yet, you will need to manually apply all patches and boot Linux after compiling from source code).

### Prepare OpenRISC Env For Linux Build

OpenRISC has designated [tutorials](https://openrisc.io/tutorials/docs/Linux.html), so our environment setup is based on these tutorials. Additionally, the OpenRISC community provides or1k-utils, which makes our configuration steps different from the tutorials. Therefore, the tutorials are for reference only (this deployment method will be submitted to the tutorials later).

Our agile development primarily utilizes QEMU + Linux + toolchain + utils, so we need to prepare the following environment:

``` bash
OR1K_QEMU_URL="https://gitlab.com/qemu-project/qemu.git"
OR1K_LINUX_URL="https://github.com/torvalds/linux.git"
OR1K_UTILS_URL="https://github.com/stffrdhrn/or1k-utils.git"
OR1K_ROOTFS_URL="https://github.com/stffrdhrn/or1k-rootfs-build/releases/download/or1k-20250417/buildroot-qemu-rootfs-20250417.tar.xz"
OR1K_TOOLCHAIN_URL="https://github.com/stffrdhrn/or1k-toolchain-build/releases/download/or1k-14.2.0-20250418/or1k-linux-14.2.0-20250418.tar.xz"
```

For or1k-utils, all environments are located in $HOME/work, so you need to configure the required environment according to the following directory structure:

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

It is worth noting that I once spent too much time configuring the toolchain, but Shorne later corrected me. We do not need to compile various toolchains from source code; we only need to download the pre-packaged binary archives provided by OpenRISC and extract them to the corresponding directories.

**​​If you find it cumbersome to manually download and deploy the resources to the appropriate locations, you can refer to [GSOC2025-OpenRISC-Download](https://github.com/ChenMiaoi/GSO2025-OpenRISC/tree/main) a one-click deployment (this method is planned to be integrated into the tutorials later**)​​. In this script, I have done two things: downloading the corresponding resources from the URLs above and placing the downloaded resources in the appropriate locations.

``` bash
download OR1K_QEMU_URL $HOME/work/openrisc/qemu
download OR1K_LINUX_URL $HOME/work/linux
download OR1K_UTOLS_URL $HOME/work/openrisc/or1k-utils
download OR1K_ROOTFS_URL $HOME/work/openrisc/buildroot-rootfs
download OR1K_TOOLCHAIN_URL $HOME/work/gnu-toolchain/local
```

For convenience, I usually add the toolchain to the environment variables:

``` bash
export PATH=$PATH:$HOME/work/gnu-toolchain/local/bin
source ~/.bashrc

or1k-linux-gcc -v
Using built-in specs.
COLLECT_GCC=or1k-linux-gcc
Target: or1k-linux
gcc version 14.2.0 (GCC)
```

After completing the above steps, our QEMU is still in the source code state, so we need to cross-compile the QEMU binary for the OpenRISC architecture. Therefore, referring to the steps in or1k-utils, you can directly compile it as follows (provided that your QEMU source code is indeed placed in the specified location as described above):

``` bash
(now, in your qemu source code)qemu$ mkdir build
qemu$ ../../or1k-utils/qemu/config.qemu
qemu$ make -j$(nproc)

qemu$ ./build/qemu-system-or1k --version
QEMU emulator version 10.0.50
Copyright (c) 2003-2025 Fabrice Bellard and the QEMU Project developers
```

After the compilation is complete, we do not need to run `make install`, as this may interfere with the environment. The binary files generated here can be correctly indexed by or1k-utils.

You can then begin verifying whether the overall environment is functional. The following uses `(or1k-utils)` to indicate that you are currently in the or1k-utils directory (**​​please ensure all your environments are configured according to the paths mentioned above​​**):

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

Log in directly as the `root` user by pressing Enter, and after typing uname -a, you will see the following information:

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

So far, the fastest OpenRISC development environment deployment is complete.

### Apply All Patches To Check Works

After deploying the OpenRISC QEMU development environment, you can proceed with the verification work. The patches I submitted are located under the [patches](./patches/) directory. Apply these patches using the following commands:

``` bash
(work/linux)$ git am *.patch
(or1k-utils)$ ./scripts/make-or1k-linux virt_defconfig
(or1k-utils)$ ./scripts/make-or1k-linux
```

Unfortunately, I have not yet completed the testing for the patches. However, if the system boots normally, it indicates that the patches are functioning well. Further testing for the patches will be provided later.

## What Have I Done Wrong

To Be Done...

## Future

I plan to continue delving deeper into the OpenRISC community and learn everything from the architecture to the upper layers of Linux through OpenRISC. The first priority is to complete the porting of `dynamic ftrace`.

- [ ] Finalize the porting of dynamic ftrace
- [ ] Finalize the porting of syscall ftrace
- [ ] Port eBPF
- [ ] Study the overall architecture of OpenRISC
- [ ] ...
