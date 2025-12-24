# Welcome

EchelonF is a tool, that provides file transfering between Unix-based machines. Please report about all bugs in issues section.

# Compiling

To compile just use `make` command

> If you want to recompile application. Use `make clean`, then `make`.

# Usage and examples

First of all, you need to compile the application.  
Server and client both using port `7777`, buffer size is `4096`. Both can be changed in `echelonheaders.h` file.  
Type of connection: `TCP`.  

Both server and client instructions can be shown be using these commands:

```
$ ./EServer --help
$ ./EClient --help
```

# Alias
If you want, you can set it up in `.zshrc` or `.bashrc`

```
$ alias ecf=/path/to/client/EClient
```


