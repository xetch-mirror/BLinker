# Configuring BLinker

This guide covers building and installing BLinker so it's runnable from anywhere (`blinker`, not `./blinker`) on Linux, macOS, and Windows.

## Requirements

- A C compiler (`gcc` or `clang`)
- `make`
- NASM (only needed if you're assembling `.s` files before linking)

---

## Linux

```bash
# Build
make

# Install system-wide (adds it to $PATH via /usr/local/bin)
sudo make install

# Verify
blinker
```

If you'd rather not install system-wide, add the project folder to your `$PATH` for the current session:

```bash
export PATH=$PATH:$(pwd)
```

Add that line to `~/.bashrc` (or `~/.zshrc`) to make it permanent.

---

## macOS

Same as Linux — macOS uses the same shell/`$PATH` model.

```bash
make
sudo make install
blinker
```

If `gcc` isn't installed, install Xcode Command Line Tools first:

```bash
xcode-select --install
```

(Note: `gcc` on macOS is often actually an alias for `clang` — this is fine, BLinker's Makefile works with either.)

---

## Windows

Two supported paths:

### Option A — WSL (recommended)
Install [WSL](https://learn.microsoft.com/en-us/windows/wsl/install), then follow the **Linux** steps above inside your WSL terminal.

### Option B — MinGW-w64 (native Windows)
1. Install [MinGW-w64](https://www.mingw-w64.org/).
2. Open a MinGW shell in the project folder and run:
   ```
   mingw32-make
   ```
3. Add the project folder to your Windows `PATH`:
   - Search "Environment Variables" in the Start menu
   - Edit the `Path` variable under your user account
   - Add the full path to the folder containing `blinker.exe`
4. Open a **new** terminal window (PATH changes don't apply to already-open terminals) and verify:
   ```
   blinker
   ```

---

## Verifying the install

Run `blinker` with no arguments — you should see:

```
usage: blinker <out> <in1> [in2 ...]
```

If you get `command not found` / `'blinker' is not recognized`, the install step didn't complete or your terminal needs to be reopened to pick up the `$PATH` change.

---

## Linking your first file

```bash
blinker hello hello.o
```

If linking `.s` (assembly) files directly, make sure NASM is installed and on your `$PATH` — BLinker shells out to it automatically to assemble before linking.
