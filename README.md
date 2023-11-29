## ARRAY OF CdTe PIXELS <br/> Geant4-based simulation
Luca Marchetti <br/>  Università degli Studi di Torino <br/> A.A. 2023/2024

<br/>

## REQUIREMENTS
It is a CMake project, so **CMake** must be installed in order to compile and run the project. 
Furthermore, a working **Geant4** version installed on the computer is of course necessary as well. 
The results are stored in ROOT files, so it is advisable that a working version of **ROOT** is 
installed on the computer.

<br/>

## HOW TO COMPILE AND RUN 
On MacOS follow the steps below:
1. Download source code.
2. Unzip it in a directory of choice.
3. Open a terminal and navigate to the source code directory.
4. Make a build folder by writing
   ~~~bash
   mkdir build
   ~~~
5. Navigate to the build folder and generate the cmake files
   ~~~bash
   cmake ..
   ~~~
6. Make the project 
   ~~~bash
   make
   ~~~
7. To run the simulation in interactive mode:
   ~~~bash
   ./rivelatore 
   ~~~
8. To run the simulation in batch mode:
   ~~~bash
   ./rivelatore run.mac
   ~~~
   
