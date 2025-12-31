# Welcome

EchelonF is a tool, that provides file transfering between Unix-based machines. Please report about all bugs in issues section.

# Compiling

To compile application just run `./build.sh`

# Usage and examples

First of all, you need to compile the application.
> [!NOTE]
> Server and client both using port `7777`, buffer size is `4096`. Both can be changed in `echelonheaders.h` file.  
> Type of connection: `TCP`.  

Both server and client instructions can be shown be using these commands:

```bash
./EServer --help
./EClient --help
```

# Alias
If you want, you can set it up in `.zshrc` or `.bashrc`

```bash
alias efc=/path/to/client/EClient
```


