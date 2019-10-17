# GDB Settings

บางครั้งเวลาเล่นๆอยู่จะเห็นว่า อาจจะไปติด "Click to continue" บ้าง assembly ไม่ถูกใจบ้าง ส่วนใหญ่สามารถ set ได้ผ่าน config ครับ ลองพิมพ์ set แล้วกด tab สองรอบดูก็ได้ครับ เยอะมาก ขอยกตัวอย่างเฉพาะที่ผมใช้บ่อยๆนะครับ  

## ปิด Enter to continue

ปิดได้ด้วย `set pagination off` ต้องระวังนะครับ ถ้าไปปิดเลยเนี่ย แล้วไป debug เจอ nonterminated string หรือ memory แสน/ล้านช่องขึ้นมา ค้างได้ง่ายๆเลย

```
(gdb) python print('A\n'*30)
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
---Type <return> to continue, or q <return> to quit---

(gdb) set pagination off
(gdb) python print('A\n'*30)
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A
A

(gdb)
```

## เลือก assembly syntax

โดย default จะเป็น ATT ครับ

```
(gdb) disassemble main
Dump of assembler code for function main:
   0x0000000000000a11 <+0>:	push   %rbp
   0x0000000000000a12 <+1>:	mov    %rsp,%rbp
   0x0000000000000a15 <+4>:	mov    $0x0,%eax
   0x0000000000000a1a <+9>:	callq  0x905 <getInputAndRun>
   0x0000000000000a1f <+14>:	cmp    $0xffffffff,%eax
   0x0000000000000a22 <+17>:	je     0xa26 <main+21>
   0x0000000000000a24 <+19>:	jmp    0xa15 <main+4>
   0x0000000000000a26 <+21>:	nop
   0x0000000000000a27 <+22>:	mov    $0x0,%eax
   0x0000000000000a2c <+27>:	pop    %rbp
   0x0000000000000a2d <+28>:	retq   
End of assembler dump.
```

เราสามารถเลือกให้เป็น intel ก็ได้ตามชอบเลย `set disassembly-flavor intel`

```
(gdb) set disassembly-flavor intel
(gdb) disassemble main
Dump of assembler code for function main:
   0x0000000000000a11 <+0>:	push   rbp
   0x0000000000000a12 <+1>:	mov    rbp,rsp
   0x0000000000000a15 <+4>:	mov    eax,0x0
   0x0000000000000a1a <+9>:	call   0x905 <getInputAndRun>
   0x0000000000000a1f <+14>:	cmp    eax,0xffffffff
   0x0000000000000a22 <+17>:	je     0xa26 <main+21>
   0x0000000000000a24 <+19>:	jmp    0xa15 <main+4>
   0x0000000000000a26 <+21>:	nop
   0x0000000000000a27 <+22>:	mov    eax,0x0
   0x0000000000000a2c <+27>:	pop    rbp
   0x0000000000000a2d <+28>:	ret    
End of assembler dump.
```

## Set GDB Layout

เพื่อการทำงานที่สะดวกขึ้น ไม่ต้อง list, disassemble ดูบ่อยๆ เราสามารถแก้ layout ได้ครับ แต่ UI มันพังๆเบี๊ยวๆบ่อยอยู่พอสมควร  

```
┌─────────────────────────────────────────────────────────────────────────────────────────┐
│0xa11 <main>            push   rbp                                                       │
│0xa12 <main+1>          mov    rbp,rsp                                                   │
│0xa15 <main+4>          mov    eax,0x0                                                   │
│0xa1a <main+9>          call   0x905 <getInputAndRun>                                    │
│0xa1f <main+14>         cmp    eax,0xffffffff                                            │
│0xa22 <main+17>         je     0xa26 <main+21>                                           │
│0xa24 <main+19>         jmp    0xa15 <main+4>                                            │
│0xa26 <main+21>         nop                                                              │
│0xa27 <main+22>         mov    eax,0x0                                                   │
│0xa2c <main+27>         pop    rbp                                                       │
│0xa2d <main+28>         ret                                                              │
│0xa2e                   xchg   ax,ax                                                     │
│0xa30 <__libc_csu_init> push   r15                                                       │
└─────────────────────────────────────────────────────────────────────────────────────────┘
exec No process In:                                                              L??   PC: ??
(gdb)


```

## Others

ก็เขียนสรุปให้ว่า อยากได้อะไร ก็ search หาเองเลยครับ ส่วนใหญ่มักจะมีไว้อยู่แล้ว
