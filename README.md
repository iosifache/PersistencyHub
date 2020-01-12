# PersistencyHub

![Linux support YES](https://img.shields.io/badge/Linux%20suppport-YES-green)
![Linux module count](https://img.shields.io/badge/Linux%20module%20count-3-green)

![Windows support UNDER CONSTRUCTION](https://img.shields.io/badge/Windows%20support-UNDER%20CONTRUCTION-yellow)
![Windows module count](https://img.shields.io/badge/Windows%20module%20count-0-red)

![Made with C](https://img.shields.io/badge/Made%20with-C-blue)
![Build with love](https://img.shields.io/badge/Build%20with-LOVE-blue)

### Description

**PersistencyHub** is a cross-platform library to help malware to set persistency in `Windows` and `Linux`, using platform-specific methods.

It consists in:
- **several dynamic libraries**, each of them representing a persistence module, that can be used to exploit the targetted machine
- **a static library**, that will be directly included in the malicious program to ease the manipulation of the previously mentioned modules

It is worth mentioning that no dependencies are needed because it was used the `ANSI` standard of `C` and standard `POSIX`(and, soon, `WinAPI`) methods.

### Documentation

For more details, please access the Wiki page of the project at this [link](https://github.com/iosifache/PersistencyHub/wiki).

### Disclaimer :triangular_flag_on_post:
This program is for **educational purposes only**. Do not use it outside the purpose of security research.