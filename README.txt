To run this program, enter the following commands in the terminal:
g++ -o flightPlan flightPlan.cpp
./flightPlan input.txt PathsToCalculateFile.txt output.txt

Make sure to move the input files to the same directory as the source code files. The example input.txt (and input2.txt) + PathsToCalculateFile.txt generate the given output file to show an example of the program's output. The program can display the most efficient route of flight(s) based on time or cost efficiency.

Project Report:

To start, the project involved using several data structures learned throughout the course. As the
instructions stated, we were using linked lists (linked lists of linked lists, specifically) to create an adjacency matrix to see what flight paths were possible. We were also using stacks to keep track of the flight paths and time/cost information. Together, this would allow us to use the iterative backtracking algorithm required for this project. I had to research what the iterative backtracking algorithm exactly was, and how it worked. I found a presentation that explained it with an example of the “N-Queens” problem, and it showed how this algorithm (along with using stacks) allowed us to create a non-recursive algorithm. One of the main challenges aside from implementing the algorithm itself was how to take the input, dissect it into separate variables, and then print it on another file (instead of the usual print to command line). As this algorithm is meant to be exhaustive, meaning it will try to find all possible flight paths and then only decide the best option (based on if the user wants least cost/least time), this may mean it won’t be the best in performance, as compared to a “efficient” or non-exhaustive algorithm.
