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

To Be Done...

### Apply All Patches To Check Works

To Be Done...

## What Did I Do the Core Job

To Be Done... 这里会讲解我所作工作的一些核心点以及为什么需要这样做。

- [text patching](./more-details/working/text_patching.md)
- [jump label](./more-details/working/jump_label.md)
- [static ftrace](./more-details/working/static_ftrace.md)
- [dynamic ftrace](./more-details/working/dynamic_ftrace.md)
- [syscall tracing](./more-details/working/syscall_tracing.md)

## What have I done wrong

To Be Done...

## Future

To Be Done...
