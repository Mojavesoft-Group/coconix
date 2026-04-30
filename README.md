# Coconix
Coconix is a Unix-like shell for Arduino Uno boards.

## Compatibility
In this table, _yes_ denotes a board on which Coconix can make use of its full feature-set, _partially_ denotes a board on which Coconix can provide limited functionality, and _no_ denotes a board that is unlikely to work with Coconix.
| Board    | Supported by Coconix? | Notes |
| -------- | ------- | ------- |
| Arduino UNO R3  | Yes    | N/A |
| Arduino UNO R4 | Yes     | EEPROM is emulated in flash on R4 boards.* |
| Arduino Due    | Partially    | Coconix on the Arduino Due cannot operate piezo buzzers or save data to non-volatile storage.* |
| Adafruit Metro M0 Express    | Partially    | Coconix on the Adafruit Metro M0 Express cannot save data to non-volatile storage.* |
| ESP32 family    | No    | Coconix on ESP32-family microcontrollers is untested and most likely broken. |

_*For more information on non-volatile storage support, read the FAQ at `EEPROM.md`._

## Commands
- `uname`
- `cd`
- `ls`
- `pwd`
- `mkdir`
- `touch`
- `cat`
- `echo`
- `rm`
- `info`
- `pinmode`
- `write`
- `read`
- `gpio`
- `sh`
- `uptime`
- `dmesg`
- `df`
- `free`
- `whoami`
- `clear`
- `reboot`
- `pwm`
- `sleep`
- `tone`
- `notone`
- `sync`

# TODO

- [ ] eeprom
- [ ] i2c
- [ ] pwm (done)
- [ ] date cmd
- [ ] alias cmd
- [ ] 'slots' cmd

## License
Coconix uses the BSD 3-clause license and contains large portions of code originating from [KernelUNO](https://github.com/Arc1011/KernelUNO).

## A note about ArduinOS and KernelESP
Some may notice that the [ArduinOS](https://github.com/Chirrenthen/ArduinOS) and [KernelESP](https://github.com/Chirrenthen/KernelESP) projects have many of the same commands as Coconix.

ArduinOS and KernelESP are rewrites of the KernelUNO codebase with features adopted from then-unmerged pull requests, and they are not affiliated with the Coconix project in any way. In addition, all versions of the ArduinOS project predating [72062e7](https://github.com/Chirrenthen/ArduinOS/commit/72062e7c260762eae3c828dba2ad7a4a0d1d9bf4) contain portions of code derived from the copyrighted work of the authors of Coconix and KernelUNO, and which have an unclear copyright status.
