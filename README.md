To run a rnbo project on Bela:

- clone this repo as a Bela project on the board in Bela/projects
- generate code from rnbo
- copy the `rnbo_source.cpp` file from the generated code inside the project and build the project

Notes:
- if any parameters are present, these will "consume" available analog in channels and send in to rnbo an (unfiltered) value once per block. Remaining analog in channels are used as extra audio ins. All analog out channels are used as extra audio outs. Further customisations of parameters (e.g.: controlling them via digital in/extracting them via analog out or digital out) can be achieved customising the render.cpp file
- the project will take a while to build the first time around, but successive rebuilds should be much faster
- no presets are supported at this time
