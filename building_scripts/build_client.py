# Temporary solution for building client.
from subprocess import call

call(['gcc','client.c','errors.c','creating_sockets.c','-o','client', '-lpthread','-g','-Wall'])
