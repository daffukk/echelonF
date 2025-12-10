# Welcome

Tool that provides file transfering between Unix-based machines. Please report about all bugs in issues section.

# Compiling

To compile just use `make` command

> If you want to recompile application. Use `make clean`, then `make`.

# Usage

First of all, you need to compile the application.
Server and client both using port `7777` and `TCP` connection 
```
$ ./EServer <mode> <file>`
$ ./EClient <mode> <file> <ip/domain>
```

## Examples

> You must move `EClient` to sending file directory if you are not using alias

### 1. Client sends


```
$ ./EServer recv
$ ./EClient send video.mp4 example.com
```

### 2. Server sends

```
$ ./EClient recv example.com
$ ./EServer send video.mp4
```

# Alias
If you want, you can set it up in `.zshrc` or `.bashrc`

```
$ alias ecf=/path/to/client/EClient
```


