project('Subset Sum Problem', 'cpp',
  version : '0.9')

# Read executable
readfile = ['src/main/mcp/execs/Read.cpp']
executable('read', readfile, c_args : ['-g'])

#Solve
solvefile = ['src/main/mcp/execs/Solve.cpp']
executable('solve', solvefile, cpp_args : ['-pg'])

# Generate random executable
genfile = ['src/main/mcp/execs/GenG6.cpp']
executable('gen', genfile, c_args : ['-g'])

# Gen K graph
genkfile = ['src/main/mcp/execs/GenKG6.cpp']
executable('genk', genkfile, c_args : ['-g'])

# Gen INVERSE graph
geninversefile = ['src/main/mcp/execs/GenInverse.cpp']
executable('genin', geninversefile, c_args : ['-g'])

#Insert
insertfile = ['src/main/mcp/execs/Insert.cpp']
executable('insert', insertfile, c_args : ['-g'])

# RUN
runfile = ['src/main/mcp/execs/Run.cpp']
executable('run', runfile, cpp_args : ['-pg'])

# Prints full list of massses
# populate = ['src/main/ssp/execs/Populate.cpp']
# executable('populate', populate, c_args : ['-g'])

# Coords
# coords = ['src/main/ssp/execs/Coords.cpp']
# executable('coords', coords, c_args : ['-g'])


# #FALTA SWEEP
# drawfile = ['src/main//execs/Draw.cpp']
# executable('draw', drawfile, dependencies : [sqlite_dep, draw_dep], c_args : ['-g'])

# #Draw
# drawfile = ['src/main//execs/Draw.cpp']
# executable('draw', drawfile, dependencies : [sqlite_dep, draw_dep], c_args : ['-g'])

# #Script
# runfile = ['src/main/execs/Run.cpp']
# executable('script', runfile, dependencies : [sqlite_dep], c_args : ['-g'])

# #Run
# runfile = ['src/main//execs/Create_sols.cpp']
# executable('run', runfile, dependencies : [sqlite_dep], c_args : ['-g'])

