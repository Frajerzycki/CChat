# Temporary solution for building server.
from subprocess import call

call(['gcc','server.c','errors.c','creating_sockets.c','-o','server', '-lpthread', '-g','-Wall'])
