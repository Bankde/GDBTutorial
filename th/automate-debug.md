# Automate commands

ความขี้เกียจก็เป็นเรื่องปกติ หลายครั้งเวลา debug เราก็คงกดๆๆๆ เหมือนเดิมเช่น print next print next เราคงอยากเขียน automation อะไรสักอย่างมาครอบ ซึ่ง GDB ก็มีคำสั่ง support ให้ครับ ชื่อ `commands` สำหรับสั่งว่าแต่ละ breakpoint แล้วให้ทำอะไรบ้าง เมื่อเสร็จแล้ว ให้ลงท้ายด้วย `end` ครับ โดยเรามักจะใส่ `continue` แทรกก่อนด้วย เพื่อให้มันทำงานต่อเลย ไม่ต้องหยุดรอ user input จากเราอีก (ปล. ลองเอาออกดูก็ได้ครับ จะเข้าใจ)

```
(gdb) break <anything>
(gdb) commands
<any automate commands>
...
continue
end
(gdb)
```

```
(gdb) break example.c:11
Breakpoint 1 at 0x890: file example.c, line 11.
(gdb) commands
Type commands for breakpoint(s) 1, one per line.
End with a line saying just "end".
>p input[i]
>p i
>continue
>end
(gdb) r
Starting program: /home/bankde/Desktop/tmp/example.o
Put string to split: hello world

Breakpoint 1, split (input=0x7fffffffdf40 "hello world\n") at example.c:11
11	    if (input[i] == ' ') {
$1 = 104 'h'
$2 = 0

Breakpoint 1, split (input=0x7fffffffdf40 "hello world\n") at example.c:11
11	    if (input[i] == ' ') {
$3 = 101 'e'
$4 = 1

Breakpoint 1, split (input=0x7fffffffdf40 "hello world\n") at example.c:11
11	    if (input[i] == ' ') {
$5 = 108 'l'
$6 = 2

Breakpoint 1, split (input=0x7fffffffdf40 "hello world\n") at example.c:11
11	    if (input[i] == ' ') {
$7 = 108 'l'
$8 = 3

Breakpoint 1, split (input=0x7fffffffdf40 "hello world\n") at example.c:11
---Type <return> to continue, or q <return> to quit---
11	    if (input[i] == ' ') {
$9 = 111 'o'
$10 = 4

Breakpoint 1, split (input=0x7fffffffdf40 "hello world\n") at example.c:11
11	    if (input[i] == ' ') {
$11 = 32 ' '
$12 = 5
First str: Hello
Second str: World
```

จะเห็นว่ามัน print ออกมาให้อัตโนมัติเลย เอาไว้ไล่หา bug ได้ดีมากโดยที่เราไม่ต้องไปนั่งแทรก print เข้าไปแต่ละจุดแล้ว compile ใหม่ครับ  

ผมก็เพิ่งสังเกตตอนเขียนเนี่ยแหละว่า function split ผมมันบัคด้วยนี่หว่า สังเกตไหมครับ i มันรันถึงแค่ 5 เอง แปลว่า split ไม่ครบทุกตัว เพราะว่าผมใช้ strlen ใน loop และหลังจากเข้า if ครั้งแรกผมหั่น string ด้วย null character ทำให้ strlen ของ input มันลดลง ตัวที่ 6-11 จึงไม่ได้ถูกเช็ค แต่ก็ทิ้งไว้แบบนี้แหละ ให้ผู้อ่านเห็นว่า GDB มันไล่หาบัคได้ดีจริงๆนะ (ปล. ไม่ได้วางแผนไว้นะ เพิ่งเห็นจริงๆ 555)  
