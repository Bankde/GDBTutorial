# Many Faces of Breaks

นอกจาก break โง่ๆที่แสดงให้ดูก่อนหน้าแล้ว ยังมี break อีกหลายแบบที่ช่วยให้เราสามารถ debug ด้วย GDB อย่างมีความสุขมากขึ้นได้ (สำหรับคนที่ชื่นชอบการ break โง่ๆก็ข้ามได้ครับ)

## Break with condition

เราสามารถเพิ่ม condition ขณะ break ได้ โดยใช้คำสั่ง `cond` ด้วย `cond <breakpoint_no> <condition>`เช่น

```
(gdb) b 11
Breakpoint 1 at 0x890: file example.c, line 11.
(gdb) cond 1 (i==5)
(gdb) r
Starting program: /home/bankde/Desktop/tmp/example.o
Put string to split: hello world

Breakpoint 1, split (input=0x7fffffffdf40 "hello world\n") at example.c:11
11	    if (input[i] == ' ') {
(gdb) p i
$1 = 5
```

เราก็จะสามารถหยุดเฉพาะกรณี i==5 ได้ ไม่เสียเวลามานั่งเลือกครับ

## Break when Variables change

ให้หยุดเมื่อตัวแปรที่เราระบุไว้มีการเปลี่ยนแปลง ใช้คำสั่ง `watch` ครับ ข้อเสียคือ ตัวแปรนั้นต้องถูกประกาศ หรือ memory ที่จะ watch ต้องถูก allocate ก่อน  

นอกจากนี้ยังมี `rwatch` สำหรับหยุดเมื่อพบการ read และ `awatch` สำหรับทั้งการ read/write ด้วย  

```
(gdb)
10	  for (int i=0; i<strlen(input); i++) {
(gdb)
11	    if (input[i] == ' ') {
(gdb) watch i
Hardware watchpoint 2: i
(gdb) c
Continuing.

Hardware watchpoint 2: i

Old value = 0
New value = 1
0x00005555555548e3 in split (input=0x7fffffffdf40 "hello world\n") at example.c:10
10	  for (int i=0; i<strlen(input); i++) {
(gdb) c
Continuing.

Hardware watchpoint 2: i

Old value = 1
New value = 2
0x00005555555548e3 in split (input=0x7fffffffdf40 "hello world\n") at example.c:10
10	  for (int i=0; i<strlen(input); i++) {
(gdb) c
Continuing.

Hardware watchpoint 2: i

Old value = 2
New value = 3
0x00005555555548e3 in split (input=0x7fffffffdf40 "hello world\n") at example.c:10
10	  for (int i=0; i<strlen(input); i++) {
```

watch ที่ memory location ก็ได้ครับ อันนี้เครื่องผมไม่ support masking ของ watch เลยต้อง manual watch เอง เนื่องจากมันไม่เข้าใจว่าที่ watch อยู่เป็น string old/new value เลยดูประหลาดๆ ก็เลยต้อง print bytes ออกมาให้ดูครับ    

```
(gdb) b getInputAndRun
Breakpoint 1 at 0x90d: file example.c, line 20.
(gdb) r
Starting program: /home/bankde/Desktop/tmp/example.o

Breakpoint 1, getInputAndRun () at example.c:20
20	int getInputAndRun() {
(gdb) n
22	  printf("Put string to split: ");
(gdb)
23	  fgets(input, 100, stdin);
(gdb)
Put string to split: hello world
24	  if (strcmp(input, "exit") == 0) return -1;
(gdb) x/s input
0x7fffffffdf40:	"hello world\n"
(gdb) watch *0x7fffffffdf40 mask 0xffffffffff00
This target does not support masked watchpoints.
(gdb) watch *0x7fffffffdf40
Hardware watchpoint 2: *0x7fffffffdf40
(gdb) watch *0x7fffffffdf44
Hardware watchpoint 3: *0x7fffffffdf44
(gdb) watch *0x7fffffffdf48
Hardware watchpoint 4: *0x7fffffffdf48
(gdb) c
Continuing.

Hardware watchpoint 3: *0x7fffffffdf44

Old value = 1870078063
New value = 1870069871
split (input=0x7fffffffdf40 "hello") at example.c:13
13	      result[j] = input+i+1;
(gdb) x/4xb 0x7fffffffdf44
0x7fffffffdf44:	0x6f	0x00	0x77	0x6f
(gdb) c
Continuing.

Hardware watchpoint 2: *0x7fffffffdf40

Old value = 1819043176
New value = 1819043144
getInputAndRun () at example.c:28
28	    *result[1] = toupper(result[1][0]);
(gdb) c
Continuing.

Hardware watchpoint 3: *0x7fffffffdf44

Old value = 1870069871
New value = 1867972719
getInputAndRun () at example.c:29
29	    printf("First str: %s\n", result[0]);
(gdb) c
Continuing.
First str: Hello
Second str: World
```

## Break on Catch Event

ใช้ `catch` สำหรับดักพวก Exception และ loading shared library ต่างๆ ผมไม่ได้เขียน c++ เลยใช้ส่วนนี้ไม่เป็นครับ ตอนนี้ใส่ไว้ให้ผู้อ่านรู้ว่ามันก็มีนะ ส่วนใครทำเป็น มาเปิด PR เติมได้  
