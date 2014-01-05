APPNAME = 'kge'
VERSION = '0.1.0'

top = '.'
out = 'bin'

def options(opt):
        opt.load('compiler_cxx')

def configure(conf):
        conf.load('compiler_cxx')

def build(bld):
	# Add source files
	files = []
	files.extend(bld.path.ant_glob("Source/*.cpp"))
	files.extend(bld.path.ant_glob("Source/*/*.cpp"))

	# Bulid KGE
	bld.shlib(
		source = files,
		target = "kge",
		includes     = ['Libs/sdl'],
		cxxflags     = ['-fpermissive'],
		lib          = ['SDL2-2.0.so'],
	)

	# Build tutorials
	bld.recurse('tutorials/tut01')
