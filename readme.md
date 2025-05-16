# 🚀 SATISH MINI COMPILER

A simple yet powerful mini compiler that translates code written in the Satish language to C (Intermediate before final compilation). This project demonstrates the fundamentals of compiler design including lexical analysis, parsing, semantic analysis, and code generation. The compiler follows proper compiler construction principles with a pipeline architecture that transforms high-level Satish code into executable programs through well-defined phases.


## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Setup](#setup)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Language Syntax](#language-syntax)
- [Compilation Process](#compilation-process)
- [Example](#example)
- [Troubleshooting](#troubleshooting)

## 🔍 Overview

The Satish Mini Compiler is a complete compiler implementation that processes a simple programming language (Satish) and generates executable code. The compilation pipeline includes:

1. **Lexical Analysis**: Tokenizes the source code using Flex
2. **Syntax Analysis**: Parses tokens using Bison
3. **Semantic Analysis**: Validates code semantics
4. **Three-Address Code (TAC) Generation**: Creates intermediate code
5. **Target Code Generation**: Translates TAC to C code

## ✨ Features

- Integer variable declarations and operations
- Basic arithmetic operations (+, -, *, /, %)
- Input/output capabilities
- String printing support
- Single-pass compilation
- Error detection and reporting
- Comments support (single-line and multi-line)

## 🛠️ Setup

### Prerequisites

- GCC compiler
- Flex (The Fast Lexical Analyzer)
- Bison (GNU Parser Generator)
- PowerShell (for Windows execution)

### Installation

1. **Install GCC**:
   - Windows: MinGW or Cygwin
   - macOS: `brew install gcc` or Xcode Command Line Tools
   - Linux: `sudo apt-get install gcc` (Ubuntu/Debian)

2. **Install Flex & Bison**:
   - Windows: Via MinGW/Cygwin or download from [GnuWin32](http://gnuwin32.sourceforge.net/)
   - macOS: `brew install flex bison`
   - Linux: `sudo apt-get install flex bison`

3. **Clone or download this repository**:
   ```
   git clone https://github.com/SatishYadav2003/Mini-Compiler.git
   cd Mini-Compiler
   ```

## 🚀 Usage

### Windows (Using PowerShell)

1. Place all files in a folder (e.g., `C:\Users\YourName\Desktop\Mini-Compiler`)
2. Open PowerShell
3. Navigate to your folder:
   ```powershell
   cd C:\Users\YourName\Desktop\Mini-Compiler
   ```
4. Run the compiler script:
   ```powershell
   .\executer.ps1
   ```

### Linux/macOS

1. Create a shell script similar to the provided PowerShell script
2. Make it executable:
   ```bash
   chmod +x executer.sh
   ```
3. Run the script:
   ```bash
   ./executer.sh
   ```

### Compiling Your Own Code

1. Create a `.satish` file with your code
2. Update the `executer.ps1` script to use your file instead of `test.satish`
3. Run the script as outlined above

## 📁 Project Structure

- **lexer.l**: Flex specification for lexical analysis
- **parser.y**: Bison specification for syntax analysis
- **semantic.c/h**: Semantic analysis functions
- **TAC.c/h**: Three-Address Code generation functions
- **tac_to_c.c**: Converts TAC to C code
- **executer.ps1**: PowerShell automation script
- **test.satish**: Example program in Satish language

## 📝 Language Syntax

### Variable Declaration

```
int variableName;
```

### Assignment

```
variableName = expression;
```

### Input

```
take variableName;
```

### Output

```
print variableName;
print ["String message"];
print ["String message"] variableName;
```

### Expressions

```
variable = a + b;
variable = a - b;
variable = a * b;
variable = a / b;
variable = a % b;
```

### Comments

```
// Single line comment
/* Multi-line
   comment */
```

## 🔄 Compilation Process

1. **Lexical Analysis (lexer.l)**:
   - Breaks down source code into tokens
   - Recognizes keywords, identifiers, literals, etc.

2. **Syntactic Analysis (parser.y)**:
   - Ensures code follows grammar rules
   - Builds abstract syntax tree

3. **Semantic Analysis (semantic.c/h)**:
   - Checks variable declarations
   - Handles symbol table management

4. **TAC Generation (TAC.c/h)**:
   - Creates intermediate three-address code
   - Stored in TAC.txt

5. **Code Generation (tac_to_c.c)**:
   - Converts TAC to C code
   - Outputs to output.c

6. **Compilation & Execution**:
   - Compiles output.c to executable
   - Runs the final program

## 💻 Example

### Input (test.satish)
```
int a;
int b;
int c;
int d;
int e;

print ["enter the value of a: "];
take a;
print ["enter the value of b: "];
take b;
print ["enter the value of c: "];
take c;

d=a+b;
e=d*c;
print ["the value of a+b*c: "] e;
```

### Output (TAC.txt)
```
DECLARE, a, , 
DECLARE, b, , 
DECLARE, c, , 
DECLARE, d, , 
DECLARE, e, , 
PRINT, enter the value of a: , , 
TAKE, a, , 
PRINT, enter the value of b: , , 
TAKE, b, , 
PRINT, enter the value of c: , , 
TAKE, c, , 
ADD, a, b, t0
ASSIGN, t0, , d
MUL, d, c, t1
ASSIGN, t1, , e
PRINT, the value of a+b*c: , , 
PRINT, e, , 
```

### Output (output.c)
```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    int a;
    int b;
    int c;
    int d;
    int e;

    printf("%s", "enter the value of a: ");
    scanf("%d", &a);
    printf("%s", "enter the value of b: ");
    scanf("%d", &b);
    printf("%s", "enter the value of c: ");
    scanf("%d", &c);
    d = a + b;
    e = d * c;
    printf("%s", "the value of a+b*c: ");
    printf("%d", e);
    return 0;
}
```

## ❓ Troubleshooting

### Common Issues

1. **"Bison failed"**:
   - Check if Bison is properly installed and in your PATH
   - Verify there are no syntax errors in parser.y

2. **"Flex failed"**:
   - Ensure Flex is properly installed and in your PATH
   - Check for syntax errors in lexer.l

3. **"GCC Compilation failed"**:
   - Check for C syntax errors in your implementation files
   - Make sure all header files are properly included

4. **"Test file execution failed"**:
   - Review your .satish file for syntax errors
   - Check the generated TAC.txt for correctness

### Getting Help

If you encounter issues not covered here:
1. Check for error messages in the console output
2. Verify all files are in the correct location
3. Ensure all tools are properly installed

---

## 🙏 Acknowledgements

- **Satish Yadav** - Initial work and design

---

*This README was last updated on May 16, 2025*