# KernelUNO v1.0

A lightweight RAM-based shell for Arduino UNO with filesystem simulation, hardware control, and interactive shell.

## Features

- **Virtual Filesystem** - Create files and directories in RAM (/dev, /home)     - **Hardware Control** - GPIO management with pin mode configuration
- **System Monitoring** - Memory usage, uptime, kernel messages (dmesg)
- **23 Built-in Commands** - From basic file operations to hardware control
- **Interactive Shell** - Real-time command execution with input buffering
- **LED Disco Mode** - Fun easter egg for testing GPIO

<img width="769" height="634" alt="1" src="https://github.com/user-attachments/assets/5cab216e-e0b3-4d80-a51a-1be918adbd21" />



## Hardware Requirements

- Arduino UNO (or compatible board with ATmega328P)
- USB cable for programming
- LEDs and resistors (optional, for GPIO testing)

## Installation

1. **Clone or download** this repository
2. **Open** `KernelUNO.ino` in Arduino IDE
3. **Select Board**: Tools → Board → Arduino UNO
4. **Select Port**: Tools → Port → /dev/ttyUSB0 (or your port)
5. **Compile & Upload**: Sketch → Upload
6. **Open Serial Monitor**: Tools → Serial Monitor (115200 baud)

Alternative with arduino-cli:
```bash
arduino-cli compile --fqbn arduino:avr:uno .
arduino-cli upload --fqbn arduino:avr:uno -p /dev/ttyUSB0 .
```

## Commands

### Filesystem Commands
- `ls` - List files in current directory
- `cd [dir]` - Change directory
- `pwd` - Print working directory
- `mkdir [name]` - Create directory
- `touch [name]` - Create file
- `cat [file]` - Read file contents
- `echo [text] > [file]` - Write to file
- `rm [name]` - Remove file/directory
- `info [name]` - Display file information

### Hardware Commands
- `pinmode [pin] [in/out]` - Set pin mode
- `write [pin] [high/low]` - Write to pin
- `read [pin]` - Read pin value
- `gpio [pin] [on/off/toggle]` - GPIO control
- `gpio vixa [count]` - LED disco mode (easter egg)

### System Commands
- `uptime` - System uptime
- `uname` - System information
- `dmesg` - Kernel messages
- `df` / `free` - Free memory
- `whoami` - Current user (hardcoded root)
- `clear` - Clear screen
- `reboot` - Restart system
- `help` - Show all commands
- `sh` - sh interpreter

## Usage Examples

```bash
# Navigate filesystem
cd home
mkdir myproject
cd myproject
touch notes.txt
echo Hello World > notes.txt
cat notes.txt

# Hardware control
pinmode 13 out
gpio 13 on
gpio 13 toggle
read 2

# System info
uname
uptime
dmesg
df

# Fun mode
gpio vixa 10
```

## Memory Usage

- Program: ~38% of 32KB flash
- RAM: ~85% of 2KB SRAM (optimized)
- Filesystem: 10 files/directories max
- DMESG buffer: 6 messages

## Specifications

- **Board**: Arduino UNO (ATmega328P)
- **Clock**: 16 MHz
- **Serial Baud**: 115200
- **Filesystem**: RAM-based (no EEPROM)
- **Storage**: Volatile (resets on power cycle)

## Technical Details

- Char-array based input buffer (32 bytes max)
- Safe path concatenation to prevent buffer overflow
- Kernel message logging with timestamps
- Real-time GPIO operations
- Efficient memory management

## Limitations

- No persistent storage (EEPROM/SD)
- Limited file size (32 bytes content per file)
- Maximum 10 files/directories
- PATH limited to 16 characters
- Single user (root)

## TODO / Future Enhancements

- [ ] EEPROM persistence
- [ ] PWM/analog control

## License

BSD 3-Clause License - See LICENSE file for details

## Author

**Arc1011** ([Arc1011](https://github.com/Arc1011))  
Created in 2026.

## Contributing

Feel free to fork, modify, and improve! Send PRs for:
- Bug fixes
- Performance improvements
- New commands
- Code optimization


// The descriptive files (i.e., README and QUICKSTART) were written by Claude AI (with minor tweaks). Why? Because if I had done it myself, it would have ended up as a few lines of incoherent gibberish that wouldn't tell you anything.//
