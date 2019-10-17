# Example 1: Testing the Patch Logic on GDB

สมมติมันมีโปรแกรมรันอยู่แล้ว และคุณไม่อยาก shutdown มันเพราะอยากได้รางวัลชนะเลิศ 0-downtime ปีที่ 10 การ patch โดยใช้ GDB เสียบแล้วเปลี่ยน flow program ~~สามารถทำได้ครับ~~ __ทำได้ แต่ไม่ควรทำครับ__ ผมล้อเล่นนะเรื่องเอาไป patch เนี่ย แต่จะทำให้ดูว่ามัน powerful มาก และมากพอที่บางครั้งเราเขียนลองเขียนโปรแกรมอะไรบางอย่างเพื่อ confirm ความคิดของเราได้ โดยไม่ต้องไปนั่งแก้ source + compile ใหม่ก่อนเลย

มีโปรแกรมรันอยู่ก่อนแล้ว ผมก็เปิดอีก terminal นึงแล้วเอา GDB เสียบไปที่ process id ที่รันอยู่

```
$ ./example.o
Put string to split: hello world
First str: Hello
Second str: World

Put string to split:
```

```
$ sudo gdb ./example.o -p `pgrep example.o`
0x00007f474d3a8081 in __GI___libc_read (fd=0, buf=0x55beb275a670, nbytes=1024)
    at ../sysdeps/unix/sysv/linux/read.c:27
27	../sysdeps/unix/sysv/linux/read.c: No such file or directory.
(gdb)
```

เรารู้ว่ามันจะตายถ้า user ใส่ string ก้อนเดียว จากนั้นก็ดูโค้ดเราว่ามันควรแก้ตรงไหนบ้างเพื่อให้ไม่ตาย

```
6 char** split(char* input) {
7   char** result = malloc(10*sizeof(char*));
8   result[0] = input;
9   int j = 1;
```

```
20 int getInputAndRun() {
 21   char input[100];
 22   printf("Put string to split: ");
 23   fgets(input, 100, stdin);
 24   if (strcmp(input, "exit") == 0) return -1;
 25   else {
 26     char** result = split(input);
 27     *result[0] = toupper(result[0][0]);
 28     *result[1] = toupper(result[1][0]);
 29     printf("First str: %s\n", result[0]);
 30     printf("Second str: %s\n", result[1]);
 31     return 0;
```

ถ้าถูกต้องคงต้องไปแก้ return ให้เป็น struct แทนอะไรแบบนี้ แต่อันนี้ถ้าแก้ง่ายๆ ก็ให้ result = calloc แทน (ค่าเริ่มต้นจะได้เป็น 0 แน่นอน) จากนั้นบรรทัดที่ 28 ให้เช็คว่าถ้า result[1]==0 ก็ไม่ต้องทำ toupper ให้ข้ามไปบรรทัด 29 เลย ซึ่ง logic ทั้งหมดเขียนได้ด้วย GDB ตามข้างล่างเลย พอเสร็จแล้วสั่ง continue

```
(gdb) b example.c:8
Breakpoint 1 at 0x55beb209d875: file example.c, line 8.
(gdb) commands
Type commands for breakpoint(s) 1, one per line.
End with a line saying just "end".
>set var result = calloc(10*sizeof(char*),1)
>continue
>end
(gdb) b example.c:28 if result[1] == 0
Breakpoint 2 at 0x55beb209d995: file example.c, line 28.
(gdb) commands
Type commands for breakpoint(s) 2, one per line.
End with a line saying just "end".
>jump example.c:29
>continue
>end
(gdb) c
Continuing.
```

โปรแกรมผมก็จะไม่ตายแล้ว เราก็จะรู้ละว่าควรไปแก้โปรแกรมจริงๆของเรายังไง ให้มันไม่ตายครับ

```
Put string to split: helloworld
First str: Helloworld

Second str: (null)
Put string to split: helloworld
First str: Helloworld

Second str: (null)
Put string to split: test
First str: Test

Second str: (null)
Put string to split: a
First str: A

Put string to split: hello world
First str: Hello
Second str: World
```
