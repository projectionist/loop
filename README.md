# README

An empty C++14 scratch project providing an OpenGL ES 2.0 drawing context for projections.  
Visit [http://projectionist.array.bz/](http://projectionist.array.bz) for more about projections.  
Visit [http://projectionist.array.bz/projections](http://projectionist.array.bz/projections) for some example projections based on this repository.

## Usage

The scratch project is developed to be built on a Debian VM or a Rapsberry PI as discussed on the [tools page](http://projectionist.array.bz/tools) page of the project website.

* Clone the project: `git clone https://github.com/projectionist/scratch.git`
* `cd` in to the project folder
* run `vagrant up`
* run `vagrant reload` (**required**: for Projectionist to start Weston)
* run `vagrant ssh`
* run `make clean`. It is a good idea to run these when switching between different branches of the same repository.
* run `make`
* run `bin/main`. you should see a white trapezium on a black background, full-screen.

## Folder Structure

```
root dir, run (e.g.) `make` here  .
                                  ├── bin
vendor code                       ├── include
                                  │   └── projection
platform specific code code       ├── platform
                                  │   ├── rpi
                                  │   │   └── projection
                                  │   └── weston
                                  │       └── projection
the source for the projection     └── src
                                      └── scratch

```

* Vendor code is where we put depedencies which are not installed with the OS package manager
* The platform directory lists routines for establishing a OpenGL ES 2.0 contexts
* The source directory is the place to implement your projection


## Extending the build

* To vendor a dependency, including header-only libraries, use the `include` folder.
  * Consider using [git submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules) to manage these dependencies.
* External dependencies installed on the OS can be included in the build if they provide a `pkg-config`. The [loop](http://github.com/projectionist/loop) project uses this method for FFmpeg flags:
    * The project has it's own pkg-config: `loop.pc`
    * The project `makefile` alters to the pkg-config build variables to include itself

## Additional Platforms

The build system targets NOOBS (Raspberry PI) as the deployment platform and a Debian VM running Wayland (weston) as a development environment.

For each platform, there is a single entry point: `projection::play(drawing)`.  
Platform-specific implementations are in `platform/<platform-name>/projection/play.cpp`.

The build system selects the correct implementation.

# Also see

http://projectionist.array.bz/tools

# Acknowledgments

Based on the simple-egl example from weston source code:
https://github.com/krh/weston/blob/0eab05d2a7cf610e4d049dfd81b0100d4219e305/clients/simple-egl.c
