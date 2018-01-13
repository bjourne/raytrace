# raytrace
A dirt simple raytracer written in C

## Compilation

The raytracer depends on my C libraries which you can download from
this location:

    https://github.com/bjourne/c-examples

Then to compile the project, run:

    $ ./waf configure build

If you decide to install the libraries into a non-standard location,
you have to specify their paths using environment variables:

    $ CFLAGS="-I/prefix/include" LD_LIBRARY_PATH="/prefix/lib" \
        LIBRARY_PATH=$LD_LIBRARY_PATH ./waf configure build

Where `prefix` is the file system prefix in which they were
installed. If the compilation works fine, the program can then be run:

    $ LD_LIBRARY_PATH=/tmp/bl/lib ./build/rt
