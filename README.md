To run a rnbo project on Bela:

- clone this repo as a Bela project on the board in Bela/projects
- generate code from rnbo
- you need to copy into the project the main file from the rnbo-generated code. This would be called `rnbo_source.cpp` by default, but its name can be set in "Export name" in the rnbo export dialog to something else. At any time, in this project you should only have exactly one such file (regardless of its name) or you will get linker errors at build time.
- build and run the project

Notes:
- if any parameters are present and `parametersFromAnalog` is set to `true`, these will "consume" available analog in channels and send in to rnbo an (unfiltered) value once per block. Remaining analog in channels are used as extra audio ins. All analog out channels are used as extra audio outs. Further customisations of parameters (e.g.: controlling them via digital in/extracting them via analog out or digital out) can be achieved customising the render.cpp file
- if you also copy `presets.json` to the project, presets will be read and the first one (by default) will be loaded. For this to be useful, you may have to set `parametersFromAnalog = false`, so that the values set by the preset are not overridden at every block
- the project will take a while to build the first time around, but successive rebuilds should be much faster

TODO:
- calling setParameter() for every audio callback has a big amount on CPU usage. Should use a filter and a quantiser with hysteresis and only update on changes.
