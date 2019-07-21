def options(ctx):
    ctx.load('compiler_c compiler_cxx')
    ctx.add_option(
        '--isect',
        type = 'string',
        default = 'ISECT_MT',
        help = 'Preferred intersection algorithm.'
        )
    ctx.add_option(
        '--shading',
        type = 'string',
        default = 'FANCY_SHADING',
        help = 'Preferred shading style.'
        )

def configure(ctx):
    ctx.load('compiler_c')
    ctx.define('ISECT_METHOD', ctx.options.isect, quote = False)
    ctx.define('SHADING_STYLE', ctx.options.shading, quote = False)
    #ctx.check(lib = 'm')
    ctx.check(lib = 'datatypes')
    ctx.check(lib = 'linalg')
    ctx.check(lib = 'isect')
    ctx.check(lib = 'file3d')
    ctx.check(lib = 'paths')
    if ctx.env.CC_NAME == 'msvc':
        base_flags = [
            '/WX', '/W3', '/O2', '/EHsc',
            # Without these flags, msvc generates a billion bullshit
            # warnings.
            '/D_CRT_SECURE_NO_WARNINGS',
            '/D_CRT_NONSTDC_NO_DEPRECATE'
        ]
        speed_flags = []
    else:
        base_flags = ['-Wall', '-Werror', '-fPIC', '-std=gnu11']
        speed_flags = ['-O3',
                       '-fomit-frame-pointer',
                       '-march=native',
                       '-mtune=native']
    ctx.env.append_unique('CFLAGS', base_flags + speed_flags)

def build(ctx):
    source = ctx.path.ant_glob('src/*.c')
    ctx.program(source = source, target = 'rt', use = [
        'DATATYPES',
        'FILE3D',
        'ISECT',
        'LINALG',
        'PATHS',
        'M',
    ])
