# Making coredump

สำหรับภาษา low level แบบ c หลายครั้งที่มันตาย เราสามารถระบุให้มันสร้าง coredump ขึ้นมาได้ ซึ่ง coredump มันคือ dying message สุดท้ายที่โปรแกรมทิ้งไว้ให้ก่อนตาย ประกอบด้วย memory/register ทั้งหมดของโปรแกรมครับ หมายความว่าเราจะรู้ได้หมดเลยว่ามันเกิดอะไรขึ้นโดยการอ่าน coredump มันคือไฟล์สำหรับไว้ debug ดีๆนี่เอง  

เพื่อให้เวลามันพังแล้วมี coredump เราจะเพิ่ม option ulimit ตามข้างล่างไว้ครับ  

```
ulimit -c 100000000
```

ถ้าโหลดโค้ดผมมา compile จากบทที่แล้วเรียบร้อยก็ลองสั่งทำงานได้เลยครับส่วนคนที่ยังไม่ได้โหลด ดูโค้ดตามลิ้งค์ข้างล่างได้ครับ (ไม่ค่อย make sense เพราะผมจำเป็นต้องเขียนให้มันมีบัค) และผมจะใช้โค้ดตัวอย่างนี้จนจบบทสุดท้ายเลย  
https://github.com/Bankde/GDBTutorial/blob/master/example.c

โปรแกรมตัวอย่างของผมคือ รับ input ของ user เข้าไป ทำการ split ออกจากกันด้วย space แล้วทำให้ตัวอักษรตัวแรกเป็นตัวพิมพ์ใหญ่ครับ โค้ดมันคงดูไม่ make sense และดูแย่ ๆ แต่ก็ต้องเขียนออกมาให้มันแย่เพื่อจะ demo ให้ดูแหละครับ  

```
$ ./example.o
Put string to split: hello world
First str: Hello
Second str: World

Put string to split: test 1  
First str: Test
Second str: 1

Put string to split: helloworld
Segmentation fault (core dumped)
```

ข้างบนก็จะเป็นข้อความเวลาที่โปรแกรมพัง ข้อความอื่นๆ เช่น  

```
Program received signal SIGSEGV, Segmentation fault.
*** stack smashing detected ***: <unknown> terminated
Aborted (core dumped)
Program received signal SIGBUS, Bus error.
```

และอื่น ๆ อีกมากมาย error แนวนี้ส่วนใหญ่เกิดจากการที่โปรแกรมทำงานผิดพลาดทั้งนั้น แต่การผิดพลาดพวกนี้ สามารถไล่ไปหาต้นตอได้ง่ายด้วย GDB ครับ  

ถ้า follow ตามผมมา หลังจากที่เรารันโปรแกรมจนมันตายไปรอบหนึ่งแล้ว มักจะมี coredump อยู่ครับ  

```
$ ls -al
total 400
drwxr-xr-x 2 bankde bankde   4096 Oct 12 08:22 .
drwxr-xr-x 5 bankde bankde   4096 Oct  2 03:51 ..
-rw------- 1 bankde bankde 385024 Oct 12 08:22 core
-rw-r--r-- 1 bankde bankde    810 Oct 12 01:31 example.c
-rwxr-xr-x 1 bankde bankde  11560 Oct 12 08:21 example.o
```
