<!-- vim-markdown-toc GitLab -->

* [The KM3NeT data formats and common definitions](#the-km3net-data-formats-and-common-definitions)
    * [Installation from source](#installation-from-source)
        * [Requirements](#requirements)
            * [Use a Docker image](#use-a-docker-image)
            * [Use Conda](#use-conda)
            * [Install from source](#install-from-source)
        * [Perform the build](#perform-the-build)
    * [Documentation](#documentation)
    * [Definitions](#definitions)
    * [Tools](#tools)
    * [Examples](#examples)

<!-- vim-markdown-toc -->

# The KM3NeT data formats and common definitions

The KM3NeT data formats are often included in other packages but separately maintained here.

The `online` data structures are used in the data acquisition (DAQ) system in
the shore stations. The `offline` data structures are used for Monte Carlo
simulations as well as high-level data.

## Installation from source

### Requirements

To build `km3net-dataformat` you need :

- [Git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git)
- [Root](https://root.cern.ch/),
- [make](https://www.gnu.org/software/make/) and/or [CMake](https://cmake.org)

Possible ways to get those requirements installed on your machine if you don't
have them already are briefly described below.

#### Use a Docker image

The idea here is to use one of the [KM3NeT-prepared Docker
image](https://git.km3net.de/common/dockerfiles/-/tree/master). This is
probably the quickier way, but will require you to work within the container
afterwards.

For instance :

```shell
$ docker run -it git.km3net.de:4567/common/dockerfiles/base/ubuntu22-rootv6.30.02:2023-12-10
root@d31efb9ece82:/# root-config --version
6.30.02
root@d31efb9ece82:/# cmake --version
cmake version 3.27.9
```

will get you an Ubuntu image with Root 6.30.02 installed (plus some other
packages, like Doxygen for instance). Note that it's a convenient image but not
a light one (it is roughly 1.4 GB).

#### Use Conda

If you happen to already have Conda installed on your system (e.g. as a way to
get Python packages), a pretty convenient way to get Root is to use the
[environment.yml](./environment.yml) provided in this repository.

```shell
$ conda env create -f environment.yml
$ conda activate km3net-dataformat
km3net-dataformat~$ root-config --version
6.30.02
km3net-dataformat~$ cmake --version
cmake version 3.27.9
```

> WARNING: if using the Makefile, you'll also need to have `gawk` installed (e.g.
on Mac the system `awk` is not compatible with the usage made in the
definitions' processing with make). This problem does not concern the CMake build.

#### Install from source

Only when everything else fails or when you really know what are you doing and
why :wink: you can try [building Root from
source](https://root.cern/install/#build-from-source).

### Perform the build

Get the code : 

```
git clone git@git.km3net.de:common/km3net-dataformat km3net-dataformat
```

And then build using either make or CMake : 

```
cd km3net-dataformat
make
```

Resulting files will be in the current directory's hierarchy (e.g. the library is in `./lib/`)

```
cd km3net-dataformat
cmake --workflow --preset release
```

Resulting files will be in the `./install-release` hierarchy (e.g. the library
is in `./install-release/lib/`)

If you want to develop with CMake, please see the [additional
documentation](cmake/README.md).

## Documentation

Class documentation can be found at the [pages](https://common.pages.km3net.de/km3net-dataformat).

## Definitions

For definitions that are common in KM3NeT, a set of CSV files exists. For
reading and writing of data, these definitions should consistently be used. The
CSV files are converted to suitable input files for the C++, python and julia
languages via the standard make procedure. It is recommended to use these files
in all codes, including codes for data analyses and data processing.

Although no strict rules exist, the definitions should be considered as "carved
in stone". Note that to extend a list of definitions, one has to consider
possible constraints (e.g. uniqueness of name and/or value). Possibly, the
values correspond to indices in an array. In that case, it is recommended to
limit the values to a reasonable maximum.

## Tools

There are some auxiliary methods and classes available in directory `tools/`.
The list of tools includes tools for relating Monte Carlo event/hit times to DAQ event/hit times
and the selection of the best reconstructed track or shower.

## Examples

The examples on reading/writing the files are provide in `examples` folder, as well as in the following external projects:

 - `km3io` - python based IO library [at this page](https://km3py.pages.km3net.de/km3io/#introduction)
 - `KM3io.jl`- Julia based IO library [at this page](https://common.pages.km3net.de/KM3io.jl/dev/examples/online_data/)
 - `aanet` reading/writing examples [aanet git](https://git.km3net.de/common/aanet/-/tree/master/examples)
 - `jpp` examples in the [documentation](https://common.pages.km3net.de/jpp/ (documentation) and [code](https://git.km3net.de/common/jpp/-/tree/master/examples)

