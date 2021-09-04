# Parallel_Computing_Assignment1
Parallel Computing Assignment1
*
1. take order of both matrices
2. take sparsity of the matrices.(add non-zeros based on the sparsity.)
3. generate random locations in the matrix to fill with non-zeros. Fill zeros in the remaining locations.--> line no:160
4. convert the matrices into sparse.(to reduce the space needed to store.)
5. add the elements if they satisfy the compatibility.(we have to a00 with b00 only.) --> refer addtion at line no:195, 218
6. multiplication is a bit different from additon. a00*b00+a01*b10 has to be done. check those conditions correctly to avoid wrong answers.
   iterate first matrix row wise and second matrix column wise. then add the result place in mul matrix.--> line no:221.
