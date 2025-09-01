<p align="center">
  <a href="https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/more-details/working/how_to_test.md">English</a> •
  <a href="https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/more-details/working/zh/how_to_test_zh.md">简体中文</a>
</p>

# How To Test

[![English](https://img.shields.io/badge/Language-English-blue)](https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/more-details/working/how_to_test.md)
[![简体中文](https://img.shields.io/badge/语言-简体中文-red)](https://github.com/ChenMiaoi/GSoC-2025-Final-Report/blob/main/more-details/working/zh/how_to_test_zh.md)

After deploying the OpenRISC QEMU development environment, you can proceed with the verification work. The patches I submitted are located under patches. Apply these patches using the following commands:

``` bash
(work/linux)$ git am *.patch
(or1k-utils)$ ./scripts/make-or1k-linux virt_defconfig
(or1k-utils)$ ./scripts/make-or1k-linux
```

On August 29, 2025, I completed the testing of static_keys and static ftrace. Now, let me reintroduce how to test my patches.

**​First, you need to confirm that you have successfully applied my patches, as this is essential for the subsequent steps**.​​ During my testing, some issues needed to be addressed: **​​Please check your toolchain. I apologize for previously using `or1k-linux-gcc`. If there are no testing requirements, this will not cause problems. However, if you need to verify my work, you should not use `or1k-linux-gcc`**.​​

Let’s first examine the specific parameters of `or1k-linux-gcc`:

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

Regarding the above, I have omitted some irrelevant output. We are primarily concerned with two parts:

- **linux-nolib**
- `lib/gcc/or1k-linux/14.2.0/include`

In tools/testing/selftests/ftrace, there is a `poll.c` file that requires `glibc`. However, as we can see above, `or1k-linux-gcc` is nolib, and we can check its search paths:

``` bash
$ ls $HOME/work/openrisc/toolchain/bin/../lib/gcc/or1k-linux/14.2.0/include
float.h  iso646.h  stdalign.h  stdatomic.h  stdckdint.h  stdfix.h      stdint.h       syslimits.h  varargs.h
gcov.h   limits.h  stdarg.h    stdbool.h    stddef.h     stdint-gcc.h  stdnoreturn.h  unwind.h
```

Therefore, if you use the or1k-linux- toolchain, you can compile the kernel normally and boot into it, but you cannot compile or run tests. We should choose the or1k-none-linux-musl- toolchain instead. Let's check its corresponding parameters:

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

Then check its search paths:

``` bash
$ ls $HOME/work/openrisc/linux-on-or1ksim/or1k-none-linux-musl/bin/../lib/gcc/or1k-none-linux-musl/15.1.0/../../../../or1k-none-linux-musl/include
aio.h        cpio.h    features.h  inttypes.h  math.h      nl_types.h  scsi         stdc-predef.h  syscall.h   unistd.h
alloca.h     crypt.h   fenv.h      iso646.h    memory.h    paths.h     search.h     stddef.h       sysexits.h  utime.h
bits         endian.h  glob.h      link.h      net         regex.h     spawn.h      string.h       time.h      wchar.h
byteswap.h   err.h     grp.h       linux       netdb.h     regulator   stdalign.h   strings.h      uchar.h     wctype.h
c++          errno.h   iconv.h     locale.h    netinet     resolv.h    stdarg.h     stropts.h      ucontext.h  wordexp.h
complex.h    fcntl.h   ifaddrs.h   malloc.h    netpacket   sched.h     stdbool.h    sys            ulimit.h    xen
```

Therefore, the initial preparations are now complete. Next, we need to manually configure the options to enable the kernel to compile the corresponding selftest modules (static_keys need to be loaded into the kernel as modules). Thus, we need to check whether you have enabled the following options:

- CONFIG_MODULES (must be enabled)
- CONFIG_MODULE_UNLOAD (must be enabled)
- CONFIG_TEST_STATIC_KEYS (must be enabled)
- CONFIG_STATIC_KEYS_SELFTEST (performs testing at boot-time, but it seems relatively unimportant)

Regarding the CONFIG_MODULES and CONFIG_MODULE_UNLOAD options, I am currently confirming with shorne whether it is feasible to write a separate patch to enable these two options by default (it has been confirmed as feasible, but it will not be submitted for now, so you need to enable them manually). For the time being, they still need to be manually enabled and verified.

Once enabled, it means you have successfully completed the setup in the following two stages:

``` bash
$ make -j12 LC_ALL=en_US.UTF-8 ARCH=openrisc CROSS_COMPILE=or1k-none-linux-musl- CONFIG_INITRAMFS_SOURCE= virt_defconfig

$ make -j12 LC_ALL=en_US.UTF-8 ARCH=openrisc CROSS_COMPILE=or1k-none-linux-musl- CONFIG_INITRAMFS_SOURCE= menuconfig
```

Then, all you need to do now is compile:

``` bash
$ make -j12 LC_ALL=en_US.UTF-8 ARCH=openrisc CROSS_COMPILE=or1k-none-linux-musl- CONFIG_INITRAMFS_SOURCE=
```

After compilation, this only generates vmlinux. The actual test and module still need to be manually compiled and installed:

``` bash
$ make -j12 LC_ALL=en_US.UTF-8 ARCH=openrisc CROSS_COMPILE=or1k-none-linux-musl- CONFIG_INITRAMFS_SOURCE= INSTALL_MOD_PATH=$HOME/work/or1k-vm-dir/modules modules_install 

$ make -j12 LC_ALL=en_US.UTF-8 ARCH=openrisc CROSS_COMPILE=or1k-none-linux-musl- CONFIG_INITRAMFS_SOURCE= V=1 \
        TARGETS="ftrace static_keys" \
        -C $HOME/work/linux/tools/testing/selftests INSTALL_PATH=$HOME/work/or1k-vm-dir/tests install
```

Please note the or1k-vm-dir here—this directory will later be used for mounting via NFS into the or1k virtual machine. Then, check whether our configuration is complete and start the virtual machine:

``` bash
$ ls $HOME/work/or1k-vm-dir/
modules  tests

(or1k-utils)$ ./scripts/qemu-or1k-linux
```

Now, let's configure how to mount this directory to the virtual machine. First, you need to confirm whether your environment has an nfs-server and then expose the corresponding configuration:

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

Then, set up the NFS export configuration:

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

It is important to note that the options insecure, rw, and no_root_squash are mandatory, especially the insecure option. Otherwise, you may encounter two errors when mounting in the virtual machine:

- Port-related errors
- Permission errors

Then, switch to the virtual machine terminal:

``` bash
(virt-machine)$ mount -t nfs -o nolock 10.9.0.100:$HOME/work/or1k-vm-dir vm-dir
```

Here is an explanation of why the 10.9.0.100 network segment is used. Please refer to the following QEMU startup parameters, where we set net=10.9.0.1/24,host=10.9.0.100. Therefore, for the virtual machine, 10.9.0.100 is the host IP.

``` bash
$HOME/work/openrisc/qemu/build/qemu-system-or1k -cpu or1200 -machine virt -no-reboot -kernel $HOME/work/linux/vmlinux -device virtio-net-pci,netdev=user -netdev user,id=user,net=10.9.0.1/24,host=10.9.0.100,hostfwd=tcp::2222-:22 -serial mon:stdio -nographic -device virtio-blk-device,drive=d0 -drive file=$HOME/work/openrisc/buildroot-rootfs/qemu-or1k-rootfs.qcow2,id=d0,if=none,format=qcow2 -gdb tcp::10001 -m 768 -append earlycon rootwait root=/dev/vda2
```

If there are no error messages, check whether the corresponding files appear in the target directory:

``` bash
(virt-machine)$ ls vm-dir/
modules  tests
```

This indicates that the mount was successful. If you attempt to run the selftest directly at this point, you may encounter errors because /sbin/modprobe cannot locate the module path. ​​This is because we still need to manually place the modules under /lib/modules:​​

``` bash
(virt-machine)$ cp -r vm-dir/modules/lib/modules /lib/
```

If you impatiently proceed to test, you may encounter this issue:

``` bash
/sbin/modprobe: invalid option -- 'n'
BusyBox v1.37.0 (2025-04-16 22:02:04 UTC) multi-call binary.

Usage: modprobe [-alrqvsD] MODULE [SYMBOL=VALUE]...
```

This occurs because the pre-downloaded busybox is a lightweight version and not fully compiled. We can use the following script to replace tests/static_keys/test_static_keys.sh:

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

After making this change, you can proceed with the test normally.

``` bash
(virt-machine)$ ./vm-dir/tests/run_kselftest.sh
```
