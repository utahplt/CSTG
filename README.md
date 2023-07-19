# Coalesced Stack Trace Graphs (CSTGs)

*This software was created in conjunction with the paper below. tcallred has captured the original code not on Git and placed it here.*

https://github.com/utahplt/CSTG/assets/2874507/1f09fc51-d4fa-457e-be26-4ca5b14a530e

*Illustration by [Ashton Wiersdorf](https://lambdaland.org)*

## Original Author
Diego Caminha B. de Oliveira

## Associated Publication 
Alan Humphrey, Qingyu Meng, Martin Berzins, Diego Caminha B. de Oliveira, Zvonimir Rakamaric, Ganesh Gopalakrishnan
[Systematic Debugging Methods for Large Scale HPC Computational Frameworks](https://soarlab.org/publications/2014_cise_hmbcrg/)
Computing in Science and Engineering (CiSE), 16(3): 48--56, doi:10.1109/MCSE.2014.11, 2014.

## Usage
DEPENDENCIES

Graphviz is needed to generate DOT CSTG files.
ps2pdf is needed to generate PDF graphs.

-------------------------------------------------
BUILD and INSTALLATION 

Run "make" and two folders will be created: "bin" and "lib".
"bin" will contain four binary files used for offline creation of CSTGs: graphDiff, tracerSum, graphMerge and graphMergeMultiple.
"lib" will contain a shared library that can be used to instrument the code and collect the stack traces.

If you desire to install the files in your local Unix system, run "sudo make install".

-------------------------------------------------
tracerSum USE

tracerSum takes the collection of stack traces and outputs the CSTG. 

./tracerSum stack_trace_collection.txt cstg

  [input]  stack_trace_collection.txt          The collection of stack traces.
  [output] cstg                                Prefix name for the ".dot", ".pdf" and ".ps" files that will be generated.


-------------------------------------------------
graphDiff USE

graphDiff takes to ".dot" CSTG files generated and creates the difference between the two.

./graphDiff cstg1.dot cstg2.dot diff_cstg

  [input]  cstg1.dot                           CSTG dot file
  [input]  cstg2.dot                           CSTG dot file
  [output] diff_cstg                           Prefix name for the ".dot", ".pdf" and ".ps" files that will be generated.

-------------------------------------------------
graphMerge USE

graphMerge takes two ".dot" CSTG files and merge them.

./graphMerge cstg1.dot cstg2.dot output

  [input]  cstg1.dot                           CSTG dot file
  [input]  cstg2.dot                           CSTG dot file
  [output] output                              Prefix name for the ".dot", ".pdf" and ".ps" files that will be generated.

-------------------------------------------------
graphMergeMultiple USE

graphMergeMultiple takes multiple ".dot" CSTG files and merge them.

Usage: graphMergeMultiple output files*

  [input]  files                               CSTG dot files previously
  [output] output                              Prefix name for the ".dot", ".pdf" and ".ps" files that will be generated.


