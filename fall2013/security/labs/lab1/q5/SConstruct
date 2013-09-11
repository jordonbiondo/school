#shared
source_files = ['src/simple-shell.c']
env = Environment(CC = 'gcc', CCFLAGS=['-Wall'])

#optimized env
opt = env.Clone()
opt.Append(CCFLAGS = ['-O2'])

#debug env
dbg = env.Clone()
dbg.Append(CCFLAGS = ['-g', '-DDEBUG=1'])

#main program
shell = env.Program('build/simple-shell', source_files)

#optimized program
o = opt.Object('build/ss-opt', source_files)
optimize = opt.Program(o)

#debug program
d = dbg.Object('build/ss-dbg', source_files)
debug = dbg.Program(d)




