# Goldrush

**Goldrush** is my attempt to drip into the brave world of operating system
development. The over-arching goal is a POSIX-like operating system that
can self-host itself (that is, you can build Goldrush under itself instead
of having to cross-compile binaries from another host platform).

There's no particular reason why I chose the name Goldrush. No need to wildly
speculate.

Goldrush is an educational project I am developing primarily to learn the art,
bringing together learning about low-level hardware details with learning the
fundamentals of computer science. My own interests will likely lead to a focus
on excellent networking support and related concepts, such as asynchronous I/O,
efficient network polling, ... - although other areas of the system will not be
neglected.

## Current goals

OSDev is not a marathon, it is a long, sustained effort. I've broken what I need to do
into roughly 7 parts:

* **Goal 1**: An operating system that can do something "interesting". Runs in 32-bit
  mode on Intel x86 processors only. (Later, 32-bit x86 will be replaced by 64-bit x86).
* **Goal 2**: A barebones OS that does the bare minimum to be considered multi-tasking.
  Think Linux just printing out a series of alternating As and Bs. All the major stuff
  that a OS kernel is going to need will be written at this point (memory management, ...)
* **Goal 3**: An operating system that can read from a file system and execute ELF files.
  Drivers for (emulated) PS/2 keyboards and the file system will be developed, along with
  a sketch of a system call interface. (This includes initial code for user mode/Ring 3).
* **Goal 4**: The operating system will become more Unix-like, being able to boot up to a
  manually bootstrapped system including `bash` (from the loaded file system). As part of
  this process, a C library will be created (or ported). A disk driver will also be
  developed.
* **Goal 5**: Get a multi-user system working with a proper userland, aside from basic tools.
  (Basically, start porting stuff like Perl, Python, GNU stuff, ...)
* **Goal 6**: Networking and SMP support.
* **Goal 7**: Try to move towards something POSIX-like and self-hosting. (Networking and SMP
  support will be the most useful here: we can start using beefy build agents.)

I've reached Goal 1. It's just the Bare Bones tutorial from the OSDev wiki. A lot is ahead...