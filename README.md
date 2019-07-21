# raytrace
A dirt simple raytracer written in C

## Compilation

The project is built using the
[WAF](https://github.com/waf-project/waf) tool. It requires you to
have Python installed. It also depends on my C libraries, which you
can download from the following location:

    https://github.com/bjourne/c-examples

These first has to be compiled. See the compilation instructions in
that project.

To configure this project, first run:

    $ python waf configure

The `--isect` and `--shading` options can be passed to control the
compilation of the toy raytracer. Then build using:

    $ python waf build

If the libraries from the `c-libraries` project were installed in a
non-standard location, their paths has to be specified using
environment variables. On Unix:

    $ CFLAGS="-I/prefix/include" LD_LIBRARY_PATH="/prefix/lib" \
        LIBRARY_PATH=$LD_LIBRARY_PATH ./waf configure build

On Windows, when compiling with Microsoft Visual C, the environment variables would be set like this:

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
