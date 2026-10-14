A simple example on how to use this library to read event file.

On Lyon, if using a `make` build :
```
    source setenv.sh
    make
    ./readEvtFile mcv5.mupage_10T.sirene.jte.jchain.aanet.1.root
```

Note that the file `mcv5.mupage_10T.sirene.jte.jchain.aanet.1.root` is not provided (so it should be taken from any production).
Alternatively, you can use one of the provided sample files from the [km3net-testdata project](https://git.km3net.de/km3py/km3net-testdata/-/tree/master/km3net_testdata/data/offline?ref_type=heads)

For a CMake build, the `readEvtFile` is part of the installation.


