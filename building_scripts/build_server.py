# Temporary solution for building server.
from subprocess import call

call(['gcc','server.c','additional_files/errors.c','-o','server', '-lpthread'])
