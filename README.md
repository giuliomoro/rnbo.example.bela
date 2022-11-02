To run a rnbo project on Bela:

- clone this repo as a Bela project on the board in Bela/projects
- generate code from rnbo
- copy the `rnbo_source.cpp` file from the generated code inside the project and build the project. You can set the name of this file in "Export name" in the rnbo export dialog to something else than `rnbo_source.cpp`. That's fine, but remember then to remove the `rnbo_source.cpp` file that this repo comes with.

Notes:
- if any parameters are present and `parametersFromAnalog` is set to `true`, these will "consume" available analog in channels and send in to rnbo an (unfiltered) value once per block. Remaining analog in channels are used as extra audio ins. All analog out channels are used as extra audio outs. Further customisations of parameters (e.g.: controlling them via digital in/extracting them via analog out or digital out) can be achieved customising the render.cpp file
- if you also copy `presets.json` to the project, presets will be read and the first one (by default) will be loaded. For this to be useful, you may have to set `parametersFromAnalog = false`, so that the values set by the preset are not overridden at every block
- the project will take a while to build the first time around, but successive rebuilds should be much faster

TODO:
- calling setParameter() for every audio callback has a big amount on CPU usage. Should use a filter and a quantiser with hysteresis and only update on changes.
