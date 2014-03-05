name = 'kge'

major   = 0
minor   = 1
bugfix  = 0

name_version     = 'Debug/%s-%d.%d' % (name, major, minor)
long_version     = '%d.%d.%d' % (major, minor, bugfix)

# Magic Waf variables here.
top = '.'
out = 'bin'

APPNAME     = name
VERSION     = long_version

def options(opt):
    # opt.tool_options('compiler_cc compiler_cxx')
    opt.load('compiler_cxx')

    opt.add_option('--shared', action='store_true', default=True, help='build shared library')
    opt.add_option('--debug', action='store_false', default=True, help='enable debugging')
    # opt.add_option('--build-gtk-doc', action='store_false', default=True, help='build gtk-doc api documentation')
    opt.add_option('--build-plugins', action='store_false', default=True, help='build plugins applications')
    opt.add_option('--build-tutorials', action='store_true', default=True, help='build and run tutorials')


def configure(conf):
    conf.load('compiler_cxx')
    # conf.env.CC = 'clang'
    # conf.check_tool('compiler_cc')

    conf.check_cfg(package='sdl2', uselib_store='SDL2', args='--cflags --libs', mandatory=True)
    conf.check_cfg(package='glew', uselib_store='GLEW', args='--cflags --libs', mandatory=True)
    conf.check_cfg(package='IL', uselib_store='IL', args='--cflags --libs', mandatory=True)
    # conf.check_cfg(package='glib-2.0', uselib_store='GLIB', args='--cflags --libs', mandatory=True)
    # conf.check_cfg(package='gobject-2.0', uselib_store='GOBJECT', args='--cflags --libs', mandatory=True)

    conf.define('KGE_VERSION', long_version)

    from waflib import Options
    conf.define('KGE_DEBUG', int(Options.options.debug))

    conf.write_config_header('kgeconfig.h')

    if Options.options.build_plugins:
        conf.recurse('plugins')

    if Options.options.build_tutorials:
        conf.recurse('tutorials/tut01')


def build(bld):


    # Add source files
    files = []
    files.extend(bld.path.ant_glob("Source/*.cpp"))
    files.extend(bld.path.ant_glob("Source/*/*.cpp"))
    files.extend(bld.path.ant_glob("Libs/stb_image.c"))

    lib = bld(
        features        = ['cxx'],
        target          = name_version,
        cxxflags        = ['-fpermissive', '-Wall', '-g', '-fPIC', '-DDEBUG'],
        # ccflags         = ['-Wall'], # '-std=c99',
        source          = files,
        includes        = '.',
        export_incdirs  = '.',
        uselib          = 'SDL2 IL',
        # use             = ['IL', 'ILU'],
    )

    from waflib import Options
    lib.features.append('cxxshlib' if Options.options.shared else 'cxxstaticlib')

    # if Options.options.debug:
    #     lib.ccflags.extend(['-g', '-O0'])
    # else:
    #     lib.ccflags.extend(['-O3'])

    # pc = bld(source='sgl.pc.in', target='%s.pc' % name_version, install_path='${PREFIX}/lib/pkgconfig')
    # pc.env.prefix       = bld.env.PREFIX
    # pc.env.exec_prefix  = '${prefix}'
    # pc.env.libdir       = '${exec_prefix}/lib'
    # pc.env.includedir   = '${prefix}/include/%s/%s' % (name_version, name)
    # pc.env.long_version = long_version
    # pc.env.name_version = name_version

    # bld.install_files('${PREFIX}/lib/%s/include' % name_version, 'sglconfig.h')
    # bld.install_files('${PREFIX}/include/%s/%s' % (name_version, name), bld.path.ant_glob('sgl/*.h'))

    if Options.options.build_plugins:
        bld.recurse('plugins')

    if Options.options.build_tutorials:
        bld.recurse('tutorials/tut01')
