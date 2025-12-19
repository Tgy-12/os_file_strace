Linux Systems Programming & Kernel Exploration

/%This repository contains a collection of systems programming projects developed for the Operating Systems course at Mekelle Institute of Technology (MIT). The projects bridge the gap between user-space application development and low-level Linux kernel exploration.

#  1: OS Lab Overview & Team

## Operating Systems Lab Report
**Mekelle Institute of Technology (MIT)**  
*File I/O & System Call Tracing*  
*December 11, 2025*

### Team Members
- **Tsegay Araya** - MIT/UR1****/16
- **Mulugeta Teamrat** - MIT/UR1*****/16  
- **Tslalom Hailemariam** - MIT/UR1*****/16

### Supervisor
**IR Berhane Guesh**  
Department of Information Technology

---

#  2: Combined Lab Implementation

## 🚀 Lab 1: POSIX File Copy
###copilinng
```compile
gcc -Wall optimal.c -o copy
```
### create file
```create
or.htm
### Features
```echo "Hello, Operating Systems Lab!" > org.html
```
###Executing
```execute
./copy org.html new.html
```

Features
✅ POSIX API: open(), read(), write(), close()

✅ Error handling: ENOENT, EACCES, ENOSPC

✅ 2048-byte buffer optimization
```

# Lab 3:Sytem Call stracing
## Trace results
```Command:
$ strace ./copy org.html madset.html
```
```text
openat() → FD 3    # Open source
openat() → FD 4    # Open destination  
read()/write() × 3 # Data transfer (2KB chunks)
close() × 2        # Cleanup
```
#How to run
```Step 1:
Compile the source code:
gcc -Wall optimal.c -o copy
```
```Step 2:
Create a test file: echo "Hello, Operating Systems Lab!" >
 org.html
```
```Step 3:
 Run with tracing:
strace ./copy org.html madset.html
```
```Step 4:
 Analyze the openat -> read/write -> close sequence in the terminal output.
```

- **Tsegay Araya** - [GitHub Profile](https://github.com/Tgy-12)
- **Mulugeta Teamrat**
- **Tslalom Hailemariam**

## 👨‍🏫 Supervisor
**IR Berhane Guesh**  
Mekelle Institute of Technology (MIT)  
December 2025

---

*Academic Project - Operating Systems Course*
