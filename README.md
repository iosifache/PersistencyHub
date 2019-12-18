# PersistencyHub

**PersistencyHub** is a cross-platform library to help malware to set persistency in `Windows` and `Linux`, using platform-specific methods.

### Main Idea
The leading concept and, at the same time, the *buzzword* used in this project, is a **module**. Basically, it is a method used by a malicious program to set persistency on a machine.
Two approaches can be used:
- **targetted attacks**
  - the specifications of the machine and the runway are known before the attack begins
  - the environment of the library can be set and a specific module can be used, having big changes to succeed
- **unpredictable attacks**
  - the specifications of the machine and the runway are not known before the attack begins
  - the environment of the library can not be set and the malicious programs need to try more modules to achieve persistency

### Runway of an Attack

```
     +----------------------------------+
     |                                  |
     | Infect the targetted machine and |
     | gain root if possible            |
     |                                  |
     +---------------+------------------+
                     |
                     |
                     v
      +--------------+-----------------+
      |                                |
      |  Init the PersistencyHub by    |
      |  (auto)setting the environment |
      |                                |
      +--------------+-----------------+
                     |
                     |
                     v
          +----------+----------+
          |                     |
          | Choose a module and |
          | check compatibility |
          |                     |
          +----------+----------+
                     |
                     |
                     v
      +--------------+-------------+
      |                            |
      | If compatible, exploit and |
      | gain persistency           |
      |                            |
      +--------------+-------------+
                     |
                     |
                     v
          +----------+----------+                +-----------------------------+
          |                     +--------------->+                             |
          | Exploit the machine |                | In case of a restart,       |
          | for fun and profit  |                | the malware will be re-run  |
          |                     +<---------------+                             |
          +----------+----------+                +-----------------------------+
                     |
                     |
                     v
+--------------------+------------------+
|                                       |
| When done, delete the persistence and |
| the software itself                   |
|                                       |
+---------------------------------------+
```

### Internal Project Architecture

```
+-------------------------------------------------+
|                                                 |
|                     Helpers                     |
|                                                 |
| ----------------------------------------------- |        +---------------------------------------+
| |                                             | |        |                                       |
| |           Cross-platform Header             | |        |              Modules                  |
| |                                             | |        |                                       |
| +---------------------------------------------+ |        | +-----------------------------------+ |
| |                                             | |        | |                                   | |
| |             Dynamic Library                 | |        | |        Cross-platform Header      | |
| |   for abstraction of the operating system   | |        | |                                   | |
| |                                             | |        | +-----------------------------------+ |
| +-----------------------+---------------------+ |        | |                                   | |
| |                       |                     | |        | |         Dynamic Library           | |
| | Windows-specific Code | Linux-specific Code | |        | | with standard exported functions  | |
| |                       |                     | |        | |                                   | |
| +---------------------------------------------+ |        | +-----------------------------------+ |
+-------------------------------------------------+        +---------------------------------------+
                         |                                                      |
                         |                                                      |
                         +------------------------+-----------------------------+
                                                  |
                                                  v
                                +-----------------+-----------------+
                                |                                   |
                                |          PersistencyHub           |
                                |                                   |
                                | +-------------------------------+ |
                                | |                               | |
                                | |    Cross-platform Header      | |
                                | |                               | |
                                | +-------------------------------+ |
                                | |                               | |
                                | |        Static Library         | |
                                | | for interaction with modules  | |
                                | |                               | |
                                | +-------------------------------+ |
                                +-----------------------------------+
                                                  |
                                                  |
                                                  |
                                                  |
                                                  v
                                         +--------+----------+
                                         |                   |
                                         | Malicious Program |
                                         |                   |
                                         +-------------------+
```

### Project Structure

```
.
├── build                Directory containing the executables and the libraries(static and dynamic)
├── headers              Directory containing the headers
├── Makefile             Build rules for the project, Linux-only at this moment
├── objects              Directory containing the object files
├── README.md            This document
└── sources              Directory containing the C sources

```