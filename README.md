# raytrace
A dirt simple raytracer written in C

## Compilation

The project is built with the
[WAF](https://github.com/waf-project/waf) tool which requires Python
3. It also depends on my C libraries, which can be found here:

    https://github.com/bjourne/c-examples

These first has to be compiled. See the compilation instructions in
that project.

After that is done, this project can be configured using:

    $ python waf configure

The `--isect` and `--shading` options can be passed to select which
intersection algorith and shader the raytracer will use. Then built
using:

    $ python waf build

If the libraries from the `c-libraries` project were installed in a
non-standard location, or not installed, their paths has to be
specified using environment variables.

On Linux:

    $ CFLAGS="-I/path/to/c-examples/libraries" \
        LDFLAGS="-L/path/to/examples/build" \
        ./waf configure build

It works similarily on Windows except that double escape characters
has to be used in paths:

    > set CFLAGS=/IC:\\path\\here\\c-examples\\libraries
    > set LDFLAGS=/LIBPATH:C:\\path\\here\\c-examples\\build
    > ./waf configure build

## Usage

If you managed to compile the program, you can run it with:

    > build/rt.exe
    usage: raytrace mesh-file image width[0-2048] height[0-2048] scale tx ty tz

To actually render something, invoke the program using:

    > build/rt.exe bunny.obj out.ppm 320 200 64.0 5 3 0
    mt bunny.obj bunny.obj 320 200 6.44396 15341

It will produce a rendering of the mesh in PPM format. The Bunny model
can be downloaded
[here](https://graphics.stanford.edu/~mdfisher/Data/Meshes/bunny.obj).

## Images

Here are some images that were rendered with the program. A head

![A head](/images/head.png)

and a dinosaur

![A dinosaur](/images/tyra.png)

and a teapot

![A dinosaur](/images/teapot.png)
