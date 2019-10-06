# Assignnment 3 AI

Problem statement pdf is attached.

### The Checking will be as follows:

1. ./compile.sh
2. ./run1.sh <i>FNAME</i>
3. ./run2.sh <i>FNAME</i>

<b> minisat will be called by the TA. The output file will be <b>FNAME.satoutput</b>


### For Debugging purposes

Use the <i>debugging.sh</i> file.

<code> ./debugging.sh <i>n1 e1 n2 e2 FILE_NAME</i></code>


### Using the Python files

<b>Only for python 2.7.* </b>

To generate an instance (G,G') run

<code>python generate.py n1 e1 n2 e2</code>

where n1,e1 are the number of nodes and edges in the bigger graph (G'), n2,e2 are the number of nodes and edges in the smaller graph (G). In the current code, it is guaranteed that G is a subgraph of G'. However, you can change the variable guaranteed_subgraph to false, to generate G,G' where G may not be a subgraph of G'.

You can store in the instance in a file by redirecting the output to a file:

<code>python generate.py n1 e1 n2 e2 > filename</code>

To check whether a mapping (in the given output format) is valid, you can run

<code> python check.py test.input test.output</code>

where test.input is in the described input format, and test.output is in the described output format.
