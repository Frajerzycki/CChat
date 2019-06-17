# Temporary solution for building client.
from subprocess import call

call(['gcc','client.c','additional_files/errors.c','-o','client', '-lpthread'])
