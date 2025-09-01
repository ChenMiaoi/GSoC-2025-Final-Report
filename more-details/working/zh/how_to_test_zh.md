<p align="center">
  <a href="https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/more-details/working/how_to_test.md">English</a> •
  <a href="https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/more-details/working/zh/how_to_test_zh.md">简体中文</a>
</p>

# How To Test

[![English](https://img.shields.io/badge/Language-English-blue)](https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/more-details/working/how_to_test.md)
[![简体中文](https://img.shields.io/badge/语言-简体中文-red)](https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/more-details/working/zh/how_to_test_zh.md)

当你部署完OpenRISC QEMU开发环境后，就可以进行验证工作了。我所提交的patch位于`patches`下。通过如下命令应用这些patches：

``` bash
(work/linux)$ git am *.patch
(or1k-utils)$ ./scripts/make-or1k-linux virt_defconfig
(or1k-utils)$ ./scripts/make-or1k-linux
```

2025/8/29，我完成了static_keys和static ftrace的测试。现在，就由我重新介绍如何针对于我的patch进行测试。

**首先，你需要确认你成功地应用上了我的patches，这样才能进行后续的工作**。在我的测试工作中，有一些问题需要解决：**请检查你的工具链，很抱歉在之前我使用了`or1k-linux-gcc`，如果没有测试需求，那么这不会导致问题，如果你需要验证我的工作，那么你就不应该使用`or1k-linux-gcc`**。

我们先看看`or1k-linux-gcc`的具体参数：

``` bash
$ or1k-linux-gcc -E -v -
Using built-in specs.
COLLECT_GCC=or1k-linux-gcc
Target: or1k-linux
Configured with: /opt/crossbuild/linux-nolib/gcc-14.2.0/configure --target=or1k-linux --enable-targets=all --prefix=/opt/crossbuild/output/or1k-linux --enable-languages=c --without-headers --disable-bootstrap --enable-sjlj-exceptions --with-system-libunwind --disable-nls --disable-threads --disable-shared --disable-libmudflap --disable-libssp --disable-libgomp --disable-decimal-float --disable-libquadmath --disable-libatomic --disable-libcc1 --disable-libmpx --enable-checking=release --disable-multilib --disable-lto
#include "..." search starts here:
#include <...> search starts here:
 $HOME/work/openrisc/toolchain/bin/../lib/gcc/or1k-linux/14.2.0/include
 $HOME/work/openrisc/toolchain/bin/../lib/gcc/or1k-linux/14.2.0/include-fixed
End of search list.
```

关于上面，我忽略了一些无关紧要的输出，我们重点关心两个部分：

- **linux-nolib**
- `lib/gcc/or1k-linux/14.2.0/include`

在`tools/testing/selftests/ftrace`中，存在一个`poll.c`文件，这个文件需要用到`glibc`，而上面我们可以看到，`or1k-linux-gcc`是`nolib`的，并且我们可以检查其检索路径：

``` bash
$ ls $HOME/work/openrisc/toolchain/bin/../lib/gcc/or1k-linux/14.2.0/include
float.h  iso646.h  stdalign.h  stdatomic.h  stdckdint.h  stdfix.h      stdint.h       syslimits.h  varargs.h
gcov.h   limits.h  stdarg.h    stdbool.h    stddef.h     stdint-gcc.h  stdnoreturn.h  unwind.h
```

因此，如果你使用`or1k-linux-`工具链，可以正常编译内核，进入内核，但是你无法编译测试，也无法进行测试。我们应该选择`or1k-none-linux-musl-`工具链，可以检查一下对应的参数：

``` bash
$ or1k-none-linux-musl-gcc -v -E -
Using built-in specs.
COLLECT_GCC=or1k-none-linux-musl-gcc
Target: or1k-none-linux-musl
Configured with: ../src_gcc/configure --enable-languages=c,c++ CFLAGS=-O2 CXXFLAGS=-O2 --disable-lto --disable-bootstrap --disable-assembly --disable-werror --target=or1k-none-linux-musl --prefix= --libdir=/lib --disable-multilib --with-sysroot=/or1k-none-linux-musl --enable-tls --disable-libmudflap --disable-libsanitizer --disable-gnu-indirect-function --disable-libmpx --enable-initfini-array --enable-libstdcxx-time=rt --with-build-sysroot=/opt/crossbuild/linux-musl/musl-cross-make/build/local/or1k-none-linux-musl/obj_sysroot STRIP_FOR_TARGET=/opt/crossbuild/linux-musl/musl-cross-make/build/local/or1k-none-linux-musl/obj_binutils/binutils/strip-new --build=x86_64-pc-linux-gnu --host=x86_64-pc-linux-gnu
Thread model: posix
Supported LTO compression algorithms: zlib
#include "..." search starts here:
#include <...> search starts here:
 $HOME/work/openrisc/linux-on-or1ksim/or1k-none-linux-musl/bin/../lib/gcc/or1k-none-linux-musl/15.1.0/../../../../or1k-none-linux-musl/include
 $HOME/work/openrisc/linux-on-or1ksim/or1k-none-linux-musl/bin/../lib/gcc/or1k-none-linux-musl/15.1.0/include
End of search list.
```

然后查看其检索路径：

``` bash
$ ls $HOME/work/openrisc/linux-on-or1ksim/or1k-none-linux-musl/bin/../lib/gcc/or1k-none-linux-musl/15.1.0/../../../../or1k-none-linux-musl/include
aio.h        cpio.h    features.h  inttypes.h  math.h      nl_types.h  scsi         stdc-predef.h  syscall.h   unistd.h
alloca.h     crypt.h   fenv.h      iso646.h    memory.h    paths.h     search.h     stddef.h       sysexits.h  utime.h
ar.h         ctype.h   float.h     langinfo.h  misc        poll.h      semaphore.h  stdint.h       syslog.h    utmp.h
arpa         dirent.h  fmtmsg.h    lastlog.h   mntent.h    pthread.h   setjmp.h     stdio_ext.h    tar.h       utmpx.h
asm          dlfcn.h   fnmatch.h   libgen.h    monetary.h  pty.h       shadow.h     stdio.h        termios.h   values.h
asm-generic  drm       ftw.h       libintl.h   mqueue.h    pwd.h       signal.h     stdlib.h       tgmath.h    video
assert.h     elf.h     getopt.h    limits.h    mtd         rdma        sound        stdnoreturn.h  threads.h   wait.h
bits         endian.h  glob.h      link.h      net         regex.h     spawn.h      string.h       time.h      wchar.h
byteswap.h   err.h     grp.h       linux       netdb.h     regulator   stdalign.h   strings.h      uchar.h     wctype.h
c++          errno.h   iconv.h     locale.h    netinet     resolv.h    stdarg.h     stropts.h      ucontext.h  wordexp.h
complex.h    fcntl.h   ifaddrs.h   malloc.h    netpacket   sched.h     stdbool.h    sys            ulimit.h    xen
```

因此，这样就准备好初期工作了。然后我们需要手动配置选项，使得内核编译对应的selftest模块(static_keys需要以模块的形式载入内核)。因此，我们需要检查你是否开启了如下选项：

- CONFIG_MODULES(必须开启)
- CONFIG_MODULE_UNLOAD(必须开启)
- CONFIG_TEST_STATIC_KEYS(必须开启)
- CONFIG_STATIC_KEYS_SELFTEST(在boot-time进行测试，但是感觉上没什么用)

对于`CONFIG_MODULES`和`CONFIG_MODULE_UNLOAD`选项，我目前正在和shorne确认是否可以单独写一个patch默认设置这两个选项的载入(已确认可行，但目前暂时不提交，因此需要你手动开启)。但目前还需要手动开启并检查。

开启后，也就意味着你已经通过了如下两个阶段的设置：

``` bash
$ make -j12 LC_ALL=en_US.UTF-8 ARCH=openrisc CROSS_COMPILE=or1k-none-linux-musl- CONFIG_INITRAMFS_SOURCE= virt_defconfig

$ make -j12 LC_ALL=en_US.UTF-8 ARCH=openrisc CROSS_COMPILE=or1k-none-linux-musl- CONFIG_INITRAMFS_SOURCE= menuconfig
```

那么现在你就只需要编译即可：

``` bash
$ make -j12 LC_ALL=en_US.UTF-8 ARCH=openrisc CROSS_COMPILE=or1k-none-linux-musl- CONFIG_INITRAMFS_SOURCE=
```

编译完成后，这仅仅只是产生`vmlinux`，而真正的`test`和`module`还需要我们手动编译并安装：

``` bash
$ make -j12 LC_ALL=en_US.UTF-8 ARCH=openrisc CROSS_COMPILE=or1k-none-linux-musl- CONFIG_INITRAMFS_SOURCE= INSTALL_MOD_PATH=$HOME/work/or1k-vm-dir/modules modules_install 

$ make -j12 LC_ALL=en_US.UTF-8 ARCH=openrisc CROSS_COMPILE=or1k-none-linux-musl- CONFIG_INITRAMFS_SOURCE= V=1 \
        TARGETS="ftrace static_keys" \
        -C $HOME/work/linux/tools/testing/selftests INSTALL_PATH=$HOME/work/or1k-vm-dir/tests install
```

请注意这里的`or1k-vm-dir`，这个目录在之后会用于我们nfs挂在到or1k虚拟机中。然后检查我们的配置是否完成，并且启动虚拟机：

``` bash
$ ls $HOME/work/or1k-vm-dir/
modules  tests

(or1k-utils)$ ./scripts/qemu-or1k-linux
```

现在，我们来配置如何挂在该目录到虚拟机中。首先你需要确认你的环境中是否存在`nfs-server`，然后需要暴露出对应配置：

``` bash
(real-machine)$ systemctl | grep nfs
  proc-fs-nfsd.mount      loaded active mounted   NFSD configuration filesystem
  nfs-blkmap.service      loaded active running   pNFS block layout mapping daemon
  nfs-idmapd.service      loaded active running   NFSv4 ID-name mapping service
  nfs-mountd.service      loaded active running   NFS Mount Daemon
  nfs-server.service      loaded active exited    NFS server and services
  nfsdcld.service         loaded active running   NFSv4 Client Tracking Daemon
  nfs-client.target       loaded active active    NFS client services
```

然后设置nfs暴露配置：

``` bash
(real-machine)$ cat /etc/exports 
# /etc/exports: the access control list for filesystems which may be exported
#		to NFS clients.  See exports(5).
#
# Example for NFSv2 and NFSv3:
# /srv/homes       hostname1(rw,sync,no_subtree_check) hostname2(ro,sync,no_subtree_check)
#
# Example for NFSv4:
# /srv/nfs4        gss/krb5i(rw,sync,fsid=0,crossmnt,no_subtree_check)
# /srv/nfs4/homes  gss/krb5i(rw,sync,no_subtree_check)
#
$HOME/work/or1k-vm-dir *(insecure,rw,sync,no_subtree_check,no_root_squash)
```

这里需要注意一点`insecure，rw，no_root_squash`是必须的，特别是`insecure`选项，否则你会在虚拟机挂载时遇见两个错误：

- 端口错误
- 无权限错误

然后切换到虚拟机终端：

``` bash
(virt-machine)$ mount -t nfs -o nolock 10.9.0.100:$HOME/work/or1k-vm-dir vm-dir
```

这里解释一下为什么使用的`10.9.0.100`网段，请看到下面的`QEMU`启动参数，这里我们设置了`net=10.9.0.1/24,host=10.9.0.100`，因此对于虚拟机来说，`10.9.0.100`才是我们的主机IP。

``` bash
$HOME/work/openrisc/qemu/build/qemu-system-or1k -cpu or1200 -machine virt -no-reboot -kernel $HOME/work/linux/vmlinux -device virtio-net-pci,netdev=user -netdev user,id=user,net=10.9.0.1/24,host=10.9.0.100,hostfwd=tcp::2222-:22 -serial mon:stdio -nographic -device virtio-blk-device,drive=d0 -drive file=$HOME/work/openrisc/buildroot-rootfs/qemu-or1k-rootfs.qcow2,id=d0,if=none,format=qcow2 -gdb tcp::10001 -m 768 -append earlycon rootwait root=/dev/vda2
```

如果没有任何错误提示，那么检查对应目录是否出现对应文件：

``` bash
(virt-machine)$ ls vm-dir/
modules  tests
```

这样，我们就挂载成功了。如果你现在直接进行selftest，你会发现会出现报错，因为`/sbin/modprobe`无法找到模块位置。**这是因为我们的`modules`还需要手动的存放到`/lib/modules`下：

``` bash
(virt-machine)$ cp -r vm-dir/modules/lib/modules /lib/
```

如果你又心急去测试，那么会出现这个问题：

``` bash
/sbin/modprobe: invalid option -- 'n'
BusyBox v1.37.0 (2025-04-16 22:02:04 UTC) multi-call binary.

Usage: modprobe [-alrqvsD] MODULE [SYMBOL=VALUE]...
```

这是由于当前预下载的`busybox`是轻量级的，并非全量编译。我们可以使用如下脚本代替`tests/static_keys/test_static_keys.sh`：

``` bash
#!/bin/sh
# SPDX-License-Identifier: GPL-2.0
# Runs static keys kernel module tests

# Kselftest framework requirement - SKIP code is 4.
ksft_skip=4
   
if ! modprobe -l | grep -q "test_static_key_base"; then                    
        echo "static_keys: module test_static_key_base is not found [SKIP]"
        exit $ksft_skip                                                    
fi                                                       
                                                                       
if ! modprobe -l | grep -q "test_static_keys"; then                    
        echo "static_keys: module test_static_keys is not found [SKIP]"
        exit $ksft_skip                                                
fi                                                   
                                                         
if modprobe -q test_static_key_base; then          
        if modprobe -q test_static_keys; then        
                echo "static_keys: ok"                   
                modprobe -q -r test_static_keys          
                modprobe -q -r test_static_key_base      
        else                                             
                echo "static_keys: [FAIL]"               
                modprobe -q -r test_static_key_base      
        fi                                         
else                                               
        echo "static_keys: [FAIL]"
        exit 1                    
fi
```

然后就可以正常开始测试。

``` bash
(virt-machine)$ ./vm-dir/tests/run_kselftest.sh
```
