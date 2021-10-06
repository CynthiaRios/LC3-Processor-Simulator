# LC-3-Processor-Simulator
The goal of this project is to design and implement a C/C++ cycle-accurate simulator of a 5-stage LC3-like integer pipelined processor.

### Current Implementation: 
Allows for 2 instructions to run as a proof of concept for cycle accuracy and RAW hazard detection. 

### Upcoming Work 
- Accepting text files
- Adding code for more operations
- Incorporate PC and ctrl operations
- Skip memory step if ctrl operation 
- Code for if certain steps take more time (ex, memory takes 3 clock cycles)
- Code for more hazards
- How to handle hazards with global vars instead of class vars
- Refactoring 
