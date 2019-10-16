# GDB Information

นอกจากการอ่าน memory และตัวแปรแล้ว GDB ยังสามารถดูข้อมูลอื่นๆเกี่ยวกับโปรแกรมและ OS ได้อีกมหาศาลเลย

ทั้งนี้เวลาพิมพ์แล้ว ต้องการหาข้อมูลต่างๆ สามารถหาจาก search engine หรือแม้แต่กด tab ใน GDB เลือกเอาก็ได้ครับ เช่น พิมพ์ info ลงใน GDB console แล้วกด tab ดู

```
(gdb) info
address                    proc
all-registers              program
args                       record
auto-load                  registers
auxv                       scope
bookmarks                  selectors
breakpoints                set
checkpoints                sharedlibrary
classes                    signals
common                     skip
copying                    source
dcache                     sources
display                    stack
exceptions                 static-tracepoint-markers
extensions                 symbol
files                      target
float                      tasks
frame                      terminal
frame-filter               threads
functions                  tracepoints
guile                      tvariables
handle                     type-printers
inferiors                  types
```

## Info breakpoints

```
(gdb) info breakpoints
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x0000555555554867 in split at example.c:7
	breakpoint already hit 1 time
```

## Info functions

```
(gdb) info functions
All defined functions:

File ../argp/argp-fmtstream.h:
size_t __argp_fmtstream_point(argp_fmtstream_t);
int __argp_fmtstream_putc(argp_fmtstream_t, int);
int __argp_fmtstream_puts(argp_fmtstream_t, const char *);
size_t __argp_fmtstream_set_lmargin(argp_fmtstream_t, size_t);

---Type <return> to continue, or q <return> to quit---
```

## Info registers

```
(gdb) info registers
rax            0x0	0
rbx            0x0	0
rcx            0x0	0
rdx            0x48	72
rsi            0x0	0
rdi            0x68	104
rbp            0x7fffffffdfb0	0x7fffffffdfb0
rsp            0x7fffffffdf30	0x7fffffffdf30
r8             0x55555575667b	93824994338427
r9             0x0	0
r10            0x555555756010	93824994336784
r11            0x0	0
r12            0x555555554750	93824992233296
r13            0x7fffffffe0a0	140737488347296
r14            0x0	0
r15            0x0	0
rip            0x5555555549a0	0x5555555549a0 <getInputAndRun+155>

---Type <return> to continue, or q <return> to quit---
```

## Info threads

```
(gdb) info threads
  Id   Target Id         Frame
* 1    process 19559 "example.o" 0x00005555555549a0 in getInputAndRun ()
    at example.c:28
```

## Info files

```
(gdb) info files
Symbols from "/home/bankde/Desktop/tmp/example.o".
Native process:
	Using the running image of child process 19559.
	While running this, GDB does not access memory from...
Local exec file:
	`/home/bankde/Desktop/tmp/example.o', file type elf64-x86-64.
	Entry point: 0x555555554750
	0x0000555555554238 - 0x0000555555554254 is .interp
	0x0000555555554254 - 0x0000555555554274 is .note.ABI-tag
	0x0000555555554274 - 0x0000555555554298 is .note.gnu.build-id
	0x0000555555554298 - 0x00005555555542bc is .gnu.hash

---Type <return> to continue, or q <return> to quit---
```

## Info variables

มีหลายคำสั่ง ผลลัพธ์ขึ้นอยู่กับ stack ที่เลือก (เลือกดีๆ)

```
(gdb) b split
Breakpoint 1 at 0x555555554867: file example.c, line 7.
(gdb) r
Starting program: /home/bankde/Desktop/tmp/example.o
Put string to split: hello world

Breakpoint 1, split (input=0x7fffffffdf40 "hello world\n") at example.c:7
7	  char** result = malloc(10*sizeof(char*));

(gdb) info local
result = 0x7ffff7a62bcd <_IO_fgets+173>
j = 0

(gdb) info args
input = 0x7fffffffdf40 "hello world\n"

(gdb) info variables
All defined variables:

File ../csu/init-first.c:
int __libc_argc;
char **__libc_argv;
int __libc_multiple_libcs;

File ../elf/dl-sysdep.c:
int __GI___libc_enable_secure;
void *__GI___libc_stack_end;
---Type <return> to continue, or q <return> to quit---
```

## Info Operating System

info os ต่างๆ เช่น cpus

```
(gdb) info os cpus
processor  vendor_id  cpu family model      model name stepping   microcode  cpu MHz    cache size physical id siblings   core id    cpu cores  apicid     initial apicid fpu        fpu_exception cpuid level wp         flags      bugs       bogomips   clflush size cache_alignment address sizes power management
0          GenuineIntel 6          69         Intel(R) Core(TM) i5-4288U CPU @ 2.60GHz 1          0x24       2600.385   3072 KB    0          1          0          1          0          0          yes        yes        13         yes        fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts mmx fxsr sse sse2 ss syscall nx pdpe1gb rdtscp lm constant_tsc arch_perfmon pebs bts nopl xtopology tsc_reliable nonstop_tsc cpuid aperfmperf pni pclmulqdq ssse3 fma cx16 pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand hypervisor lahf_lm abm cpuid_fault epb invpcid_single pti ibrs ibpb stibp fsgsbase tsc_adjust bmi1 avx2 smep bmi2 invpcid xsaveopt dtherm ida arat pln pts cpu_meltdown spectre_v1 spectre_v2 spec_store_bypass l1tf mds swapgs 5200.77    64         64         42 bits physical, 48 bits virtual
```

## Memory mappings

```
(gdb) info proc mappings
process 20291
Mapped address spaces:

          Start Addr           End Addr       Size     Offset objfile
      0x555555554000     0x555555555000     0x1000        0x0 /home/bankde/Desktop/tmp/example.o
      0x555555754000     0x555555755000     0x1000        0x0 /home/bankde/Desktop/tmp/example.o
      0x555555755000     0x555555756000     0x1000     0x1000 /home/bankde/Desktop/tmp/example.o
      0x555555756000     0x555555777000    0x21000        0x0 [heap]
      0x7ffff79e4000     0x7ffff7bcb000   0x1e7000        0x0 /lib/x86_64-linux-gnu/libc-2.27.so
      0x7ffff7bcb000     0x7ffff7dcb000   0x200000   0x1e7000 /lib/x86_64-linux-gnu/libc-2.27.so
---Type <return> to continue, or q <return> to quit---
```

เรียกได้ว่าแทบมีทุกอย่างทีคุณต้องการแล้ว อยากได้อะไรหาไม่เจอ ลอง search engine ดูครับ มักจะเจอที่ต้องการเสมอเลย
