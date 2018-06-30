baseEnv = Environment()

if baseEnv['PLATFORM'] == 'win32':
    baseEnv['CCFLAGS'].append('-DWIN32')
    baseEnv['CCFLAGS'].append('/EHsc')    

Export('baseEnv')

SConscript(['src/SConscript',
            'tutorials/SConscript',
            'plugins/SConscript'])