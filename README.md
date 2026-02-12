# EchelonF

EchelonF is a tool, that provides file transfering between Unix-based machines through TCP sockets. Please report about all bugs in issues section.

# TODO 
- [ ] Connect attempts in CRecv
- [ ] Connect attempts with custom interval
- [ ] Receive certain amount of files
- [ ] Replace 'readsome' with 'read'
- [ ] Pause and resume functions
- [ ] Logs
- [ ] SHA checksum
- [ ] Custom filepath support
- [ ] Multi-sending one file from the server
- [ ] Secure transfer by encrypting file stream
- [ ] Error handling for sending(filename, bind, etc.)

# Compiling

To compile application just run **`./build.sh`**

# Usage and examples

First of all, you need to compile the application.
> [!NOTE]
> Server and client both using port `7777`, buffer size is `4096`. Both can be changed using `--port` or `--buffer` flags.  
> Type of connection: `TCP`.  

Both server and client instructions can be shown be using these commands:

```bash
./ecf --help
```

# Tips and issues

> [!TIP]
> You can create a short alias by adding this line to your shell configuration file(e.g., `~/.bashrc` or `~/.zshrc`):
> ```bash
> alias ecf=/path/to/ecf
> ```

---

## License
This project is licensed under [MIT](LICENSE).


