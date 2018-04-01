# ansys

ANSYS is an educational exploration into a multi-tasking system.

This code does not run on a Mac! I am using ubuntu:xenial from Dockerhub.
```
docker run -it -v /path/to/ansys:/ansys --cap-add=SYS_PTRACE --security-opt seccomp=unconfined ubuntu:xenial /bin/bash
```

To get setup with dependencies, run `./deps.sh`.

To run the tests, run `make test`.
