### About
This is an implementation of the [real time content aware resize](https://link.springer.com/article/10.1007/s11432-009-0041-9) algorithm.
Project consist from console and gui applications (xinar and uxinar respectively) which provide resize services.

### Dependencies

To build XINAR from source you need
* *libboost-program-options*
* *libboost-chrono*
* *libboost-system*
* *libboost-date-time*
* *libboost-atomic*
* *OpenCV 2*

To build UXinar you also need QT5 *core* & *widgets* libs.
The project uses *CMake >= 3.5* as building system.


### Usage

Once you built XINAR, you can use it in 2 ways: as a console application or
via GUI.

Console usage example:

shrink image example.jpg to 600 by 600 pixels
```
:~$ ./xinar -i example.jpg -w 600 -h 600
```
All commands are available via ``` -? ``` (```--help```) flag.


GUI usage example:

Just run *uxinar* as executable and choose file to resize in the file menu.

### Structure

* **Core** -- part where resize algorithm is implemented
* **CUI, GUI** -- console and graphics applications implementation respectively

Copyright [Andery Tkachev](https://github.com/Andrey-Tkachev), [Nikita Orlov](https://github.com/acerikfy), Dmitriy Malygin, 2017.

