# NSD.Lib – NES Sound Driver & Library

**NSD.Lib** is a sound driver and music library for the Nintendo Entertainment System (NES / Famicom).  
It supports both **NES game development** and **chip­tune / NSF music production**.

👉 **Full documentation (Japanese):**  
https://shaw02.github.io/nsdlib/

---

## ✨ Features

- NES APU sound driver (2A03)
- Supports **NSF (NES Sound Format)**
- Short MML–style music description optimized for NES
- MML compiler (**nsc.exe**) supporting multiple output formats
- Designed for both **game embedding** and **standalone music**
- Suitable for real hardware and emulators
- Compact and cycle-conscious implementation
- Compatible with **cc65 / ca65** toolchains

---

## 🚀 Quick Start

This README intentionally stays short.

For **setup, build instructions, driver integration, and detailed explanations**,  
please refer to the official manual:

📘 **NSD.Lib Manual**  
https://shaw02.github.io/nsdlib/

Typical workflows:

### 🎮 Game Development

1. Write music using **Short MML**
2. Convert MML to ca65 assembly using `nsc.exe`
3. Assemble and link with your NES game
4. Call the sound driver each frame from the game loop

### 🎵 Chiptune / NSF Music

1. Write music using **MML**
2. Convert MML directly into an **NSF file** using `nsc.exe`
3. Play the NSF on emulators, players, or real hardware

---

## 🎼 MML Compiler (nsc.exe)

The MML compiler supports the following conversions:

- **MML → ca65 assembly source**  
  For embedding music into NES game programs

- **MML → NSF (NES Sound Format)**  
  For standalone music and chiptune distribution

➡️ **Short MML format and compiler reference:**  
https://shaw02.github.io/nsdlib/nsc_asm.html

---

## 📁 Repository Structure

    nsdlib/
    ├── bin/        ; binary for NSF (NES Sound Format)
    ├── lib/        ; library for cc65 & ca65
    ├── include/    ; include files for cc65 & ca65
    ├── src/        ; sound driver & MML compiler source
    ├── sample/     ; sample NES programs and music data
    ├── docs/       ; documents

Each directory is explained in detail in the manual.

---

## 🧰 Requirements

- NES toolchain (**cc65 / ca65**)
- NES emulator, NSF player, or real hardware
- Basic knowledge of NES programming or chiptune composition

---

## 📚 Documentation

- 📘 **Main Manual**  
  https://shaw02.github.io/nsdlib/

- 🎼 **MML Compiler (nsc.exe) Usage**  
  https://shaw02.github.io/nsdlib/usage_nsc.html  
  *Command-line usage and conversion options*

- 🎮 **Sound Driver / Library Usage & API Reference**  
  https://shaw02.github.io/nsdlib/usage_nsd.html  
  *Driver initialization, per-frame calls, and function reference*

- 🧠 **GitHub Wiki (Notes & Design)**  
  https://github.com/Shaw02/nsdlib/wiki

---

## ❤️ Contributing

Issues and pull requests are welcome.

When reporting bugs, please include:
- Emulator, NSF player, or hardware used
- Minimal reproduction steps
- MML or source code, if applicable

---

## 📄 License

This project is released under the **BSD 2-Clause License**.  
See [LICENSE.md](LICENSE.md) for details.

---

## ✍ Author

S.W.  
NES sound driver & MML compiler / homebrew & chiptune development
