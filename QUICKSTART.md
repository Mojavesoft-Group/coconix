# KernelUNO - Quick Start Guide

## 1. Install & Upload (5 minutes)

### Using Arduino IDE
1. Download `KernelUNO.ino`
2. Open in Arduino IDE
3. Select: **Tools → Board → Arduino UNO**
4. Select: **Tools → Port → /dev/ttyUSB0**
5. Click **Upload** button
6. Open **Serial Monitor** (Ctrl+Shift+M)
7. Set baud to **115200**

### Using arduino-cli
```bash
cd KernelUNO/
arduino-cli compile --fqbn arduino:avr:uno .
arduino-cli upload --fqbn arduino:avr:uno -p /dev/ttyUSB0 .
```

## 2. Connect Serial Terminal

### Linux/Mac
```bash
screen /dev/ttyUSB0 115200
# Or
minicom -D /dev/ttyUSB0 -b 115200
```

### Windows
- Use PuTTY (COM3, 115200 baud)
- Or Arduino IDE Serial Monitor

## 3. First Commands

```bash
help                    # See all commands
uname                   # System info
uptime                  # How long running
df                      # Free memory

ls                      # See commands

mkdir mydir             # Create directory
touch myfile            # Create file
echo Hello > myfile     # Write to file
cat myfile              # Read file
```

## 4. Hardware Testing

```bash
# Setup pin 13 (LED pin)
pinmode 13 out

# Turn LED on/off
gpio 13 on
gpio 13 off

# Toggle LED
gpio 13 toggle

# Disco mode! (LED blink pattern)
gpio vixa 10
```

## 5. System Monitoring

```bash
dmesg                   # See system log
df                      # Check free RAM
whoami                  # Current user
```

## Common Issues

### Upload fails?
- Check correct port: `ls /dev/tty*`
- Try different USB cable
- Restart Arduino IDE

### Serial Monitor shows garbage?
- **IMPORTANT**: Set baud to **115200** (top right)
- Close other serial programs
- Unplug/replug USB

### Only one character at a time?
- Make sure Serial Monitor baud = 115200
- Arduino must be rebooted after upload

## Keyboard Shortcuts (in Serial Monitor)

- **Backspace** - Delete character
- **Enter** - Execute command
- **Ctrl+L** - Clear screen (use `clear` command)

## File System Structure

After startup:
```
/
├── dev/               (devices/pins)
│   ├── pin2
│   ├── pin3
│   └── pin4
├── home/              (user files)
```

## LED Wiring (Optional)

```
Arduino Pin 13 → 220Ω Resistor → LED → GND
```

## Next Steps

- Create files in `/home` for notes
- Try all GPIO commands
- Check `dmesg` to see system activity

## Need Help?

- Type `help` in the shell
- Check README.md for detailed command list
- Look at dmesg output with `dmesg`

Have fun! 🚀
