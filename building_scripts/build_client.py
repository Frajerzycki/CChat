# Temporary solution for building client.
from subprocess import call

call(['gcc','client.c','errors.c','create_client.c','-o','client', '-lpthread','-g','-Wall'])
