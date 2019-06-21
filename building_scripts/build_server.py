# Temporary solution for building server.
from subprocess import call

call(['gcc','server.c','errors.c','-o','server', '-lpthread', '-g','-Wall'])
