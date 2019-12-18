# PercistencyHub

**PercistencyHub** is a cross-platform library to help malware to set persistency in `Windows` and `Linux`, using platform-specific methods.

### Main Idea
The leading concept and, in the same time, the *buzzword* used in this project, is a **module**. Basically, it is a method used by a malicious program to set percistency on a machine.
There are two approches that can be used:
- **targetted attacks**
  - the specifications of the machine and the runway are known before the attack begins
  - the environment of the library can be setted and a specific module can be used, having big changes to succeed
- **unpredictable attacks**
  - the specifications of the machine and the runway are not known before the attacks begins
  - the environment of the library can not be setted and the malicious programs need to try more modules in order to achieve percisteny

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
      |  Init the PercistencyHub by    |
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
      | gain percistency           |
      |                            |
      +--------------+-------------+
                     |
                     |
                     v
          +----------+----------+                +-----------------------------+
          |                     +--------------->+                             |
          | Exploit the machine |                | In case of restart,         |
          | for fun and profit  |                | the malware will be reruned |
          |                     +<---------------+                             |
          +----------+----------+                +-----------------------------+
                     |
                     |
                     v
+--------------------+------------------+
|                                       |
| When done, delete the percistency and |
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
                                |          PercistencyHub           |
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
├── Makefile             Build rules for project, Linux-only in this moment
├── objects              Directory containing the object files
├── README.md            This document
└── sources              Directory containing the C sources

```