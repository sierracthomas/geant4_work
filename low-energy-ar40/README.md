# In-progress Geant4 executable to look at Argon-40 interactions with low-energy incoming gammas


Notes here:
- based on `exampleB1` so need to change name
- to compile:
    - enter Singularity shell and source Geant4 modules with `source <path to geant>/geant4-v11.1.1-build/geant4make.sh`
    - make a directory next to the `low-energy-ar40` directory with `mkdir build`
    - `cd build/` and `cmake ../low-energy-ar40`
    - `make`

- to run:
    - `cd build/` and run `./exampleB1 run2.mac`

- This executable will send 20 5MeV gammas into a LAr detector with a volume of 2m^3. It outputs the results to a CSV file with the following format ("process" is not yet working). Scripts to analyse this are not uploaded yet. 
`["event_id", "track_id", "type", "x", "y", "z", "delta_E", "process"]`


- Singularity issues? Try `singularity pull library://sierracthomas/particle/v11` 