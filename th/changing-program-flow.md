# Changing program flow

ขณะที่เรา dynamic debugging บน GDB เนี่ย นอกจากแค่ทำตาม flow program ปกติแล้ว เรายังสามารถเปลี่ยน flow ของโปรแกรมได้ด้วย เช่น การแก้ข้อมูลต่างๆ, เปลี่ยน 1 เป็น 0, กระโดดไปมา, สั่ง function เพิ่ม เป็นต้น  

อันนี้มีประโยชน์เมื่อผู้ใช้งานต้องการทดสอบอะไรบางอย่างบน GDB โดยที่ไม่ต้องเสียเวลาไปแก้โค้ด compile ใหม่หลายๆครั้งครับ ดู advance นิดหน่อย ในมหาลัยคงไม่จำเป็นต้องใช้ อ่านผ่านๆให้รู้ว่ามันมีนะ พอจะใช้จริงค่อยหาตาม search engine ดูก็ได้ครับ  

สำหรับนัก reverse engineering ทั้งหลาย รวมทั้งนักเล่น CTF ต่างๆ หัวข้อนี้มีประโยชน์มหาศาลครับ บางข้อนี่ง่ายงงโจทย์เลย  

## Edit variables

แก้ variables ง่ายมากครับ เช่น

```
(gdb) r
Starting program: /home/bankde/Desktop/tmp/example.o
Put string to split: helloworld

Breakpoint 1, split (input=0x7fffffffdf40 "helloworld\n") at example.c:7
7	  char** result = malloc(10*sizeof(char*));
(gdb) p input
$4 = 0x7fffffffdf40 "helloworld\n"
(gdb) set var input = "hello world"
(gdb) p input
$5 = 0x7ffff7fe1f20 "hello world"
(gdb) c
Continuing.
First str: Hello
Second str: World
```

จะเห็นว่า input ที่เราใส่เข้าไป `helloworld` แล้วปกติมันจะทำให้โปรแกรม crash ถูกเปลี่ยนเป็น `hello world` จนสามารถรันโปรแกรมได้ตามปกติได้ (ถ้า string อาจจะมีข้อจำกัดเรื่อง memory space นะครับ)

นอกจากนี้ยังสามารถแก้ registers ต่างๆได้ด้วย (ถ้าไม่ได้ลง asm มากๆ ข้ามส่วนนี้ไปได้ครับ) เช่น

```
(gdb) b *0x00005555555548a2
Breakpoint 2 at 0x5555555548a2: file example.c, line 11.
(gdb) cond 2 i==5
(gdb) r
Starting program: /home/bankde/Desktop/tmp/example.o
Put string to split: helloaworld

Breakpoint 2, 0x00005555555548a2 in split (input=0x7fffffffdf40 "helloaworld\n")
    at example.c:11
11	    if (input[i] == ' ') {
(gdb) info reg
rax            0x61	97
rbx            0x5	5
rcx            0x0	0
rdx            0x5	5
rsi            0x0	0
rdi            0x7fffffffdf40	140737488346944
rbp            0x7fffffffdf20	0x7fffffffdf20
rsp            0x7fffffffdef0	0x7fffffffdef0
r8             0x55555575667c	93824994338428
r9             0x0	0
r10            0x555555756010	93824994336784
r11            0x0	0
r12            0x555555554750	93824992233296
r13            0x7fffffffe0a0	140737488347296
r14            0x0	0
r15            0x0	0
rip            0x5555555548a2	0x5555555548a2 <split+72>
eflags         0x206	[ PF IF ]
cs             0x33	51
ss             0x2b	43
ds             0x0	0
es             0x0	0
fs             0x0	0
---Type <return> to continue, or q <return> to quit---
gs             0x0	0
(gdb) p i
$2 = 5
(gdb) p input[i]
$3 = 97 'a'
(gdb) i r eflags
eflags         0x206	[ PF IF ]
(gdb) set $ZF = 6
(gdb) set $eflags |= (1 << $ZF)
(gdb) i r eflags
eflags         0x246	[ PF ZF IF ]
(gdb) c
Continuing.
First str: Hello
Second str: World
```

ตัวอย่างแอบยากนิดนึงเพราะเป็น ZeroFlag ซึ่งไม่ได้มี register ของตัวเอง จึงต้องแก้ eflags แต่ถ้าในกรณี eax, rax ก็แค่ `set $eax = -1` แบบนี้ได้เลย เท่านี้ส่วนของ main ก็จะได้รับ return เป็น -1 แล้วจบโปรแกรมได้ เช่น

```
(gdb) b *0x00005555555549fb
Breakpoint 3 at 0x5555555549fb: file example.c, line 33.
(gdb) r
Starting program: /home/bankde/Desktop/tmp/example.o
Put string to split: hello world
First str: Hello
Second str: World

Breakpoint 3, getInputAndRun () at example.c:33
33	}
(gdb) set $eax = -1
(gdb) c
Continuing.
[Inferior 1 (process 20460) exited normally]
```

## Jump to any instruction

กระโดดได้เลยครับ ข้ามไปเลยยยยย จะเห็นว่าผมข้ามโค้ดส่วนที่เป็น toupper ไปทั้งก้อนเลย ต้องระวังให้ดีนะครับ เพราะการ jump เนี่ยไม่ได้มีการดัก scope อะไรให้เรา พังได้ง่ายๆ ปกติผมใช้ jump เพื่อทดสอบอะไรสั้นๆ เช่น ข้าม function ที่มีปัญหาไปก่อน เป็นต้น

```
(gdb) break example.c:27
Breakpoint 1 at 0x976: file example.c, line 27.
(gdb) r
Starting program: /home/bankde/Desktop/tmp/example.o
Put string to split: hello world

Breakpoint 1, getInputAndRun () at example.c:27
27	    *result[0] = toupper(result[0][0]);
(gdb) jump example.c:29
Continuing at 0x5555555549bc.
First str: hello
Second str: world
```

## Run function

เราสามารถสั่ง function อะไรก็ได้ที่เราต้องการได้ระหว่าง GDB อยู่ครับ

```
(gdb) call system("/bin/bash")
bankde@ubuntu:~/Desktop/tmp$ ls
core  example.c  example.o
bankde@ubuntu:~/Desktop/tmp$ exit
exit
$37 = 0
(gdb)
```
