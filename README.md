# Symbol Table

Symbol table is a crucial data structure, made and maintained by compilers to trace semantics
of identifiers (e.g information about name, type, scope, e.t.c). 

Among the stages that the compiler takes to convert source code into executable code,
semantic analysis is one of the most critical stage to validate correctness of the source code,
such as checking whether a variable has been declared prior to being used, or whether the
assignment of a value to a variable is type matched, e.t.c. The semantic analysis stage demands
the symbol table in order to trace the information required in those checks.

![image](https://user-images.githubusercontent.com/86992472/216223290-553d3079-32bc-4cfe-a6c8-fbda1dfebf72.png)

# Implementation

In the subject Data Structures and Algorithms (DSA) at HCMUT in Semester 211. Students are required to build a symbol table with language C++ with OOP architecture.

This repo is the soulution of author for this assignment. More specifications of each assignment will be found in each folder.

* **Assignment 1**: Using data structure Linked List.

![image](https://user-images.githubusercontent.com/86992472/216224424-4ae8d405-799d-435e-a949-ba543266c03d.png)

* **Assignment 2**: Using data structure Splay Tree.

![image](https://user-images.githubusercontent.com/86992472/216225076-d8172528-affe-4244-bc73-3b189883d908.png)

* **Assignment 3**: Using data structure Hash Table.

![image](https://user-images.githubusercontent.com/86992472/216225802-b89ae4f3-4481-4508-996e-f89454315a1f.png)


# Testing

You can test this data structure with some following testcases, more details will be found in the specifications of each assignment.

```bash

INSERT x number
INSERT y string
ASSIGN x 15
ASSIGN y 76

```

# Result

* Assignment 1: 98/100
* Assignment 2: 99/100
* Assignment 3: 97/100


