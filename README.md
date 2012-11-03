Zeus
====

The main repository for our graphics engine.

To run this:
1. Go to project properties.
2. Go to Configuration Properties-> C/C++ -> General
3. Under Additional Include Directories, link to Graphics_Engine\Include
4. Under Linker -> General, and Additional Library Directories link to Graphics_Engine\Lib
5. Under Linker -> Input, and Additional Dependencies, add: d3d10.lib;d3dx10.lib;dxgi.lib;
6. If it still doesn't compile, try adding the links under VC++ dependiencies as well.