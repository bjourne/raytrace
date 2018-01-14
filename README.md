# raytrace
A dirt simple raytracer written in C

## Compilation

The project is built using the
[WAF](https://github.com/waf-project/waf) tool. It requires you to
have Python installed. It also depends on my C libraries, which you
can download from the following location:

    https://github.com/bjourne/c-examples

Then to configure the project, run:

    $ ./waf configure

The `--isect` and `--shading` options can be passed to control the
compilation of the toy raytracer. Then build: using:

    $ ./waf build

If you decide to install the libraries into a non-standard location,
you will have to specify their paths using environment variables:

    $ CFLAGS="-I/prefix/include" LD_LIBRARY_PATH="/prefix/lib" \
        LIBRARY_PATH=$LD_LIBRARY_PATH ./waf configure build

Where `prefix` is the file system prefix in which they were
installed.

## Usage

If you managed to compile the program, you can run it with:

    $ LD_LIBRARY_PATH=/prefix/lib ./build/rt
    usage: raytrace mesh-file image width[0-2048] height[0-2048] scale tx ty tz

Where `prefix` is the aforementioned library prefix. To actually
render something, invoke the program like this:

    $ LD_LIBRARY_PATH=/prefix/lib ./build/rt bunny.obj out.ppm 320 200 50.0 0 0 0
    mt bunny.obj 320 200 3.86878 487

## Images

Here are some images that were rendered with the program:

![A head](/images/head.png)
