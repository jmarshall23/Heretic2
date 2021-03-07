# Heretic2

Heretic 2 - Engine Reconstruction Branch:

I have got Heretic 2 running without any closed source bits; I modified the Quake 2 GPL release, to become the "quake2.dll" in Heretic 2. The code changes I made to create the new "quake2.dll" foundation aren't made hap hazzardly; I have mapped enough functions in quake2.dll and was able to use hex rays to help me make the needed code modifications to get this far.

I have got the foundation on its feet, just a few big things are left:

1) 3rd person camera has to be re-created.
2) Player pmove needs to be re-written based on the hex rays output of﻿ the original Quake2.dll.
3) Client effects need to be hooked up the network replication.
4) Minor things like obituary notifications and such.
5) Some server side support functions.

To run:
Simply copy over the base data to the root folder here, you don't need any of the original binaries. 
