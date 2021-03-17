// COMP1521 18s1 Q2 ... shred() function

// put any needed #include's here

void shred(int infd, int outfd)
{
    int size = 64;
    char *buf[BUFSIZ];
    char nl = '\n';
    while (read(infd, &buf, size) > 0)  {
        for (int i = 0; i < size; i++) {
            char random = (rand() % 26) + 'A';
            buf[i] = random;
        }
        write(outfd, &buf, size);
        write(outfd, &nl, 1);
    }
}
