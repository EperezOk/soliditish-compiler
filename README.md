# EVM Compiler

A compiler for a language to create smart contracts, that is transpiled to Solidity.

## Requirements

The following dependencies are required to build the project:

- [Bison v3.8.2](https://www.gnu.org/software/bison/)
- [CMake v3.24.1](https://cmake.org/)
- [Flex v2.6.4](https://github.com/westes/flex)
- [GCC v11.1.0](https://gcc.gnu.org/)
- [Make v4.3](https://www.gnu.org/software/make/)

## Build

To build the project, run the following command from the root directory:

```bash
script/build.sh
```

## Compile a file

1. Create a file with the program to compile.
2. Run the compiler from the root directory of the project, passing the path to the program to compile as a parameter:
    ```bash
    script/start.sh program
    ```
3. The compiler will generate a file with the same name as the program, but with the extension `.sol`, in the same directory as the program.

## Run tests

```bash
script/test.sh
```

To add new test cases, create new files containing the program to test, inside the `test/accept` or `test/reject` folders as appropriate (i.e., whether it should be accepted or rejected by the compiler).
