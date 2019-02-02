# Goldrush

**Goldrush** is my attempt to drip into the brave world of operating system
development. The over-arching goal is a POSIX-like operating system that
can self-host itself (that is, you can build Goldrush under itself instead
of having to cross-compile binaries from another host platform).

There's no particular reason why I chose the name Goldrush. No need to wildly
speculate.

Goldrush is an educational project I am developing primarily to learn the art,
bringing together learning about low-level hardware details with learning the
fundamentals of computer science.

## Design

Whether or not Goldrush should be monolithic or microkernel-based has been raging
in my head for a while. On one hand, a monolithic kernel provides clear performance
advantages, especially in the days of Spectre which have made system calls and
context switches even more expensive than they already were. But it would be callous
to play down the clear security, isolation, and flexibility provided by microkernels.

Ultimately, I chose to go with a microkernel. The clear security benefits are not
something I'm willing to give up for a few percent more performance.

Goldrush can be broken up in a few parts:

* The _Core Kernel_ is, strictly speaking, the most fundamental portion of the system. It
  deals largely with architecture-specific initialization, memory management, scheduling,
  message ports, and other activities required to execute a native Goldrush process. (This
  is everything in `kernel/`.)
* The _Native API_ is a direct layer to the Goldrush kernel. All Goldrush servers and
  personalities are native applications that communicate directly with the Goldrush kernel.
  It includes a good portion of the native C library you'd expect, along with Goldrush-specific
  functions.
* The _POSIX Personality_ is intended to be a POSIX compatibility layer. It communicates with
  the Goldrush kernel to act on system calls from POSIX processes. If you are thinking about
  what users work with, then you're looking for this.
* The _POSIX Userland_ is the userland that the user uses to interact with the POSIX personality.

In many respects, Goldrush is a microkernel extended with some services more common of monolithic
systems.

## Current status

Currently trying to advance past writing text to the VGA console.