# Dynamic Debugging

ต่อมา GDB มันไม่ใช่แค่อ่าน coredump ที่ตายแล้วอย่างเดียว เราสามารถรันโปรแกรมของเราบน GDB ได้เลย ซึ่งลักษณะมันจะออกมาคล้ายๆโปรแกรม IDE ที่กดหยุดได้ สั่งเลื่อนที่ละบรรทัด ดูตัวแปร อะไรแบบนี้ได้หมดเลย ต่างกันแค่ว่ามันเปลี่ยนจาก GUI กดๆ มาเป็น commandline เท่านั้นเอง จริงๆแล้วมันทำอะไรได้เยอะกว่า GUI มาก ถ้าเราใช้ commandline เป็นครับ  

การใช้ GDB เพื่อรันโปรแกรม ทำได้ 2 แบบครับ คือ  

1. เริ่มต้น run ด้วย GDB เลย `gdb ./<executable>`
```
$ gdb ./example.o
Reading symbols from ./example.o...done.
(gdb)
```
2. เอา GDB ไปเสียบ program ที่รันอยู่แล้วก็ได้ครับ (อาจจะต้องใช้ sudo เพราะเรื่อง security ของ Linux version ใหม่ๆ) `sudo gdb ./<executable> -p <pid_of_running_prog>`
```
$ sudo gdb ./example.o -p 18805
0x00007f123f97e081 in __GI___libc_read (fd=0, buf=0x55e5136ed670, nbytes=1024)
    at ../sysdeps/unix/sysv/linux/read.c:27
27	../sysdeps/unix/sysv/linux/read.c: No such file or directory.
(gdb)
```

ข้อสองเนี่ยมันดีตรงที่ เกิดโปรแกรมมีปัญหาต่างๆแต่ยังไม่ตาย (เช่น deadlock, data corrupt) GDB ก็สามารถเข้าไปดูได้เหมือนกัน  

เพื่อความง่าย ผมจะทำวิธี 1 ให้เป็นตัวอย่างเพื่อทำตามนะครับ  

```
$ gdb ./example.o
Reading symbols from ./example.o...done.
(gdb) r
Starting program: /home/bankde/Desktop/tmp/example.o
Put string to split: hello world
First str: Hello
Second str: World

Put string to split:
```

สั่ง `r` หรือ `run` คือการรันโปรแกรมปกติครับ แต่มันไม่ค่อยมีประโยชน์เท่าไร หลายครั้งเราอยากหยุดโปรแกรมตรงสักจุดเพื่อดูว่าเกิดอะไรขึ้น เราจะใช้คำสั่ง `break` หรือ `b` ครับ  

```
(gdb) b getInputAndRun
Breakpoint 1 at 0x90d: file example.c, line 20.
(gdb) b example.c:26
Breakpoint 2 at 0x966: file example.c, line 26.
(gdb) b *<instruction_addr>
(gdb) b *0x000055555555496d
Breakpoint 3 at 0x55555555496d: file example.c, line 26.
```

เรา break ได้หลายท่ามากเลยครับ ซึ่งหลังจากที่โปรแกรมทำงานไปถึงจุดที่เราสั่ง break ไว้ มันก็จะหยุดเพื่อรอให้เราสั่ง GDB อะไรก็ได้ต่อ ส่วนกรณีที่เราไม่เอา breakpoint ไหนแล้ว ก็สามารถเอาออกได้ด้วยคำสั่ง `delete`

```
delete <breakpoint_no>
(gdb) delete 1
(gdb) delete
Delete all breakpoints? (y or n) y
```

ระหว่างติด breakpoint เราสามารถสั่งให้มันรันทีละคำสั่งได้ ด้วย `step` `next` `stepi` `nexti` ครับ

1. step - ทำงานทีละบรรทัดของ source code แต่ถ้าเจอ function แล้ว function นั้นมี source code มันจะเข้าไปใน function นั้นด้วย ย่อด้วย `s`
2. next - เหมือน step แต่ไม่เข้า function ย่อด้วย `n`
3. stepi - ทำงานทีละบรรทัดของ assembly code (machine code) เข้าไปใน function ที่เจอด้วย ย่อด้วย `si`
4. nexti - เหมือน stepi แต่ไม่เข้า function ย่อด้วย `ni`

นอกจากนี้มีคำสั่งอื่นๆอีกครับ
1. continue - ย่อด้วย `c` คือให้มันทำงานต่อ จนจบ หรือจนเจอ breakpoint ใหม่
2. finish - ทำงานจนจบ function แล้วหยุด
3. until <location> - ทำจนถึงบรรทัดที่ระบุ ให้อารมณ์คล้ายๆ temporary breakpoint  

ส่วนเวลากด enter ใน GDB โดยไม่ได้ระบุคำสั่งอะไร จะแปลว่าให้รันคำสั่งก่อนหน้าอีกครั้งหนึง จึงไม่ต้องพิมพ์ next next next บ่อยๆนะครับ enter รัวๆได้เลย

```
(gdb) break split
Breakpoint 1 at 0x867: file example.c, line 7.
(gdb) r
Starting program: /home/bankde/Desktop/tmp/example.o
Put string to split: helloworld

Breakpoint 1, split (input=0x7fffffffdf40 "helloworld\n") at example.c:7
7	  char** result = malloc(10*sizeof(char*));
(gdb) n
8	  result[0] = input;
(gdb)
9	  int j = 1;
(gdb)
10	  for (int i=0; i<strlen(input); i++) {
(gdb)
11	    if (input[i] == ' ') {
(gdb)
10	  for (int i=0; i<strlen(input); i++) {
(gdb) until 17
split (input=0x7fffffffdf40 "helloworld\n") at example.c:17
17	  return result;
(gdb) p result[0]
$1 = 0x7fffffffdf40 "helloworld\n"
(gdb) p result[1]
$2 = 0x0
(gdb) finish
Run till exit from #0  split (input=0x7fffffffdf40 "helloworld\n")
    at example.c:17
0x0000555555554972 in getInputAndRun () at example.c:26
26	    char** result = split(input);
Value returned is $3 = (char **) 0x555555756a80
(gdb) n
27	    *result[0] = toupper(result[0][0]);
(gdb) p result[0]
$5 = 0x7fffffffdf40 "helloworld\n"
(gdb) n
28	    *result[1] = toupper(result[1][0]);
(gdb) p result[0]
$6 = 0x7fffffffdf40 "Helloworld\n"
(gdb) n

Program received signal SIGSEGV, Segmentation fault.
0x00005555555549a0 in getInputAndRun () at example.c:28
28	    *result[1] = toupper(result[1][0]);
```

รันจนมัน segmentation fault ได้เลยครับ ก็จะเห็นละว่าเราสามารถสั่งให้โปรแกรมเราทำงานทีละบรรทัดได้ดั่งใจเลย ไม่ต้องเอา printf ไปแทรกทุกบรรทัดละ ทีเหลือก็ความเชี่ยวชาญกับความชินมือละครับ ต้องค่อยๆฝึกละ
