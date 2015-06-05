#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#if defined(_WIN32) && !defined(__NUTC__)
# include <io.h>
# define unlink _unlink
#else
# include <unistd.h>
#endif
#include "cgnslib.h"

#ifndef CGNSTYPES_H
# define cgsize_t int
#endif
#ifndef CGNS_ENUMT
# define CGNS_ENUMT(e) e
# define CGNS_ENUMV(e) e
#endif

#define TWOPI 6.2831853

/* set STRUCTURED_FACES to write the structured zone bocos for the
   mixed case as face range and face list instead of as point range/list
#define STRUCTURED_FACES
*/

/* set UNSTRUCTURED_1TO1 to write the unstructured case
   zone connectivities as 1to1 instead of abutting1to1
#define UNSTRUCTURED_1TO1
*/

/* set ABUTTING1TO1_FACES to write the unstructured case
   zone connectivites as abutting1to1 with faces(elements)
   instead of points. This also writes the mixed case zone 1
   to zone 2 connectivity with a face range
#define ABUTTING1TO1_FACES
*/

int CellDim = 3, PhyDim = 3;

int cgfile, cgbase, cgzone;
//int CellDim, PhyDim;
cgsize_t size[9];

#define NUM_SIDE 5
#define NODE_INDEX(I,J,K) ((I)+NUM_SIDE*(((J)-1)+NUM_SIDE*((K)-1)))
#define CELL_INDEX(I,J,K) ((I)+(NUM_SIDE-1)*(((J)-1)+(NUM_SIDE-1)*((K)-1)))

int num_coord;
float *xcoord, *ycoord, *zcoord;
int num_element, num_face;
cgsize_t *elements, *faces, *parent;

int max_sol;
float *solution;

int npts;
cgsize_t *pts, *d_pts;
float *interp;

char errmsg[128];

void init_data();
void write_structured(), write_unstructured();
void write_mixed(), write_mismatched();

void error_exit (char *where)
{
    fprintf (stderr, "ERROR:%s:%s\n", where, cg_get_error());
    exit (1);
}
