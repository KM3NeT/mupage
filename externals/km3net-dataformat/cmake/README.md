## Building km3net-dataformat with CMake

### Using a simplified workflow

If you are not already a CMake user, the recommendation is to start by using a
workflow preset, e.g. the one named `dev`

``` 
$ cd someplace
$ git clone git@git.km3net.de:common/km3net-dataformat
$ pushd km3net-dataformat
$ git checkout cmake # only needed until we merge this branch with master
$ cmake --workflow --preset dev
```

This will configure and build the project.

Once the first build is done, you can then work from the `build-dev` directory directly, e.g. 

```
$ cd build-dev
$ cmake --build . 
$ ctest
```

### Configuring the CMake build the old/more advanced way

This step is normally only performed once. Pick a directory where you'll
perform the build. It could be a **sub**directory of the source, but also
completely elsewhere. If the example below, we use a directory at the same
level as the source directory. Note that we are _not_ using the source
directory itself : the build directory is a disposable one, the source
directory clearly is not !

```shell
$ cd someplace
$ git clone git@git.km3net.de:common/km3net-dataformat
$ pushd km3net-dataformat
$ git checkout cmake # only needed until we merge this branch with master
$ popd
$ mkdir build
$ cd build
$ cmake ../km3net-dataformat -DCMAKE_GENERATOR=Ninja -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=someplace_where_to_install -DCMAKE_BUILD_TYPE=Release
```

### Running the build

Still within the `build` directory, you can now launch the build itself :

```shell
$ cmake --build . 
...
[70/70] Linking CXX shared library stage/lib/libKM3NeTROOT.dylib
```

The output library is at this stage to be found under `stage/lib` directory.

### Testing

Run the test(s) :

```shell
$ ctest
Test project someplace/build
    Start 1: head
1/1 Test #1: head .............................   Passed    3.57 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   3.57 sec
```

### Installing

If tests pass, then you can install :

```shell
$ cmake --install .
```

At this stage the library will be found under `someplace_where_to_install/lib`
and the include files under
`someplace_where_to_install/include/km3net-dataformat`.

