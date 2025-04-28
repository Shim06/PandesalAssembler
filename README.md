# PandesalAssembler

PandesalAssembler is an assembler for the [Pandesal CPU](https://github.com/Shim06/PandesalCPU), an 8-bit processor inspired by the MOS 6502.  
It reads `.asm` source files and outputs a `.bin` binary ready for use in emulators, hardware projects, or educational systems.
PandesalAssembler is available on GitHub under the <a href="https://github.com/Shim06/PandesalAssembler/blob/main/LICENSE" target="_blank">GNU General Public License v3.0</a> (GPLv3).

---

## Usage
```cmd
PandesalAssembler.exe <input file> [-o <output file>]
```

## Assembly Syntax
- Labels: end with a colon `:`
- Comments: start with `;`
- Immediate values: use `#`, with optional prefixes:
    - % for binary (`#%10101010`)
    - $ for hexadecimal (`#$FF`)
    - No prefix for decimal (`#42`)
- Addresses: use `$` prefix for hexadecimal

Example:
```assembly
start:
    LDA #$0A    ; Load 10 into A
    STA $0020   ; Store A into address $0020
    HLT         ; Halt execution
```

## Getting Started

1. Download the latest release.
2. Run `PandesalAssembler.exe` through your terminal or command prompt.
3. Pass your `.asm` file as an argument to assemble it:
```cmd
PandesalAssembler.exe your_program.asm
```
4. The assembler will generate a .bin file in the same directory.

## How to build
If you wish to build the assembler yourself, follow these steps:

### Step 1
Clone the repository:
```cmd
git clone https://github.com/Shim06/PandesalAssembler.git
```
Or download it manually using the Code → Download ZIP button on GitHub.

### Step 2
Open `PandesalAssembler.sln` in Visual Studio.
Under Configuration Properties, ensure the correct Configuration (Debug/Release) and Platform (Win32/x64) are selected.

### Step 3
Build the project:
- Click Build → Build Solution,
- or press `Ctrl + Shift + B` to start the build.

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

## License

This project is licensed under the GNU General Public License v3.0 (GPLv3) - see the [LICENSE](LICENSE) file for more details.
