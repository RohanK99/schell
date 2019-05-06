# schell
interactive command line shell built in C
currently supports the following builtin functions from bash and all system commands:
* cd
* exit

## How to build
To create a release build follow the instructions below
* `mkdir build`
* `cd build`
* `cmake ..`
* `make -j$(nproc)`

To create a debug build follow the instructions below
* `mkdir debug-build`
* `cd debug-build`
* `cmake -DCMAKE_BUILD_TYPE=Debug ..`
* `make -j$(nproc)`

## Running the Program
The build step will produce an executable called `schell`
Run using `./schell`

Now you can use this like any other shell!




