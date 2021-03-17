




init(nfilled, 0)
init(nempty, 4)
init(sem, 1)


producer thread:

wait(nempty)
wait(sem)
make an item
signal(sem)
signal(nfilled)


consumer thread

wait(nfilled)
wait(sem)
consume an item
signal(sem)
signal(nempty)










