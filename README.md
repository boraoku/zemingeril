# ZeminGeril

A C++ programme by Bora Okumusoglu as part of his Masters Degree thesis entitled [Stress distribution and elasto plastic settlements under shallow foundations](https://tez.yok.gov.tr/UlusalTezMerkezi/TezGoster?key=-Z0vbSUgrhM9fXoGkRe6Q8zJ7Sk7W31SHEFOFRvy43ZoScM4g3rcZCg_NG_0hva7).

### Requirements

X11 is required to compile the code in Linux / Mac environment. Mac OS X will require [XQuartz](https://www.xquartz.org).

In Windows, nothing is required except a C++ compiler like [Visual Studio(VS)](https://code.visualstudio.com/docs/cpp/config-mingw).

### Compiling & Running

For Linux / Mac environment, simply use the makefile by
```bash
cd source
make
```

And then run the compiled code:
```bash
cd .. # go back to top directory
cd build
./ZeminGeril
```

For Windows, just use the provided VS file `ZeminGeril.sln` to compile. 

There are some precompiled files under `build` folder. Windows EXE file will require [Microsoft Visual C++ Redistributable](https://docs.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170) runtime files installed on the system.

### 
License

MIT
