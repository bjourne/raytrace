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

def build(ctx):
    source = ctx.path.ant_glob('src/*.c')
    ctx.program(source = source, target = 'rt')
