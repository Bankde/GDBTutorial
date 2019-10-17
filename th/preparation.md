# Preparation

ก่อนอื่นให้ติดตั้ง program GCC และ GDB ครับ  

__For Linux__
```
sudo apt install gcc gdb
```

__For OS X__ (ต้องไปแก้ certificate ด้วยครับ)
```
brew install gcc gdb
```

แนะนำว่าใช้ linux เลยง่ายกว่า เดี๋ยวสะดวกจะมาเพิ่มวิธี config ใน OSX ให้อีกที  

จากนั้นก็ clone repo [GDBTutorial][GDBTutorial-repo] นี้ แล้ว compile executable สำหรับรันบนเครื่องครับ  

```
git clone
gcc example.c -o example.o -g
```

gcc ตรงไปตรงมานะครับ ส่วน -g เป็น flag สำหรับ debug เพื่อให้ตอนเสียบ GDB เข้าไปเนี่ย จะอ่านง่าย และมี code ให้เทียบได้ ถ้าไม่มีโค้ดส่วนนี้ตอนเสียบ GDB จะเห็นเป็น assembly ล้วน ๆ เลย เพื่อความง่ายและพื้นฐาน เราจะใช้ flag -g ไปก่อน  

[GDBTutorial-repo]: https://github.com/Bankde/GDBTutorial.git
