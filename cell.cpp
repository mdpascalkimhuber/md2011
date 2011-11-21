#include "cell.hpp"

// calculate the index of a cell with the coordinates
// this is a recursive function
unsigned comp_cell_index(unsigned dim, real pos[DIM], real cell_length[DIM], int cell_N[DIM]) 
{
  if (dim > 0) return (pos[dim]/cell_length[dim] + cell_N[dim]*comp_cell_index(dim-1, pos, cell_length, cell_N)); 
  return pos[dim]/cell_length[dim]; 
}; 
  
