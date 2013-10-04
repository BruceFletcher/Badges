Badges
======

Maybe we do need them
---------------------

This is a straight C firmware example for the
[VHS Fifth Anniversary Badge](http://vancouver.hackspace.ca/wp/2013/09/30/vhs-led-matrix-badge/).

From a code-reuse perspective, the main features are interrupt-driven UART and timer interfaces.

The timer supports an arbitrary number of repeating and one-shot callbacks at a resolution of
100 ticks per second.  This allows for easy timing of animations and game mechanics.

The UART support allows for badge-to-badge interactions, like two player games.

