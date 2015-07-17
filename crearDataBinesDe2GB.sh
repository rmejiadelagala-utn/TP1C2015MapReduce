#!  /bin/bash

dd if=/dev/zero of=Nodo/Debug/data.bin  bs=1024  count=2M
dd if=/dev/zero of=Nodo1/Debug/data.bin  bs=1024  count=2M
dd if=/dev/zero of=Nodo2/Debug/data.bin  bs=1024  count=2M
dd if=/dev/zero of=Nodo3/Debug/data.bin  bs=1024  count=2M
dd if=/dev/zero of=Nodo4/Debug/data.bin  bs=1024  count=2M
dd if=/dev/zero of=Nodo5/Debug/data.bin  bs=1024  count=2M
dd if=/dev/zero of=Nodo6/Debug/data.bin  bs=1024  count=2M
dd if=/dev/zero of=Nodo7/Debug/data.bin  bs=1024  count=2M

exit 0;