### About
This is an implementation of the 
[real time content aware resize](https://link.springer.com/article/10.1007/s11432-009-0041-9)
algorithm.
Project consist from console and gui applications (xinar and uxinar
respectively) and library for ```C++```, which provide resize services.

### Dependencies

To build XINAR from source you need
* *libboost-program-options*
* *libboost-system*
* *OpenCV 2*
* *Qt5 core* (*only for uxinar*)
* *Qt5 widgets* (*only for uxinar*)

Project uses CMake>=3.5 as building system.


### Usage


You can use *xinar* in two ways: as a ```C++``` library and as an application.


#### Application usage
Once you built *xinar*, you can use it in 2 ways: as a console application or
via GUI.

Console usage example:

Resize image ```examle.jpg``` to 600 by 600 pixels:

```
:~$ ./xinar -i example.jpg -w 600 -h 600
```
All commands are available via ``` -? ``` (```--help```) flag.


GUI usage example:

Just run *uxinar* as executable and choose file to resize in the file menu. 
To resize change size of the window.

**NOTE:** Xinar can resize picture up to 2 times in both height and width 
while expanding picture.

#### Library usage

```xinar::Resize``` is main class that provides resize services.
Let ```input``` and ```output``` be ```cv::Mat``` matrix objects,
 ```size``` be ```cv::Size``` object.
Then you can apply content aware resize in following way: 

```
...
#include "xinar.h"
...

int main() {
    ...
    xinar::Resize resize();
    resize.init(input);
    resize.process(output, size);
    ...
}

```

All documentation is available in ```docs``` folder.

### Structure

* **core** -- *seam carving algorithm implementation*
* **cui** -- *console application code*
* **filters** -- *filters functors implementation*
* **gui** -- *GUI application code*
* **include** -- *internal headers*
* **interface** -- *external headers*
* **utils** -- *various code to make life happier*

Copyright [Andery Tkachev](https://github.com/Andrey-Tkachev), 
[Nikita Orlov](https://github.com/acerikfy), Dmitriy Malygin, 2017.

