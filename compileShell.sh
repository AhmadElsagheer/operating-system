bcc -ansi -c -o shell.o shell.c
as86 lib.asm -o lib.o
ld86 -o shell -d shell.o lib.o
./loadFile shell
bochs -f conf.bxrc



