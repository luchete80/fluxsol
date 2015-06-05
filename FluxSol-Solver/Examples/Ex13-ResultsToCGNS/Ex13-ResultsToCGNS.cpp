/************************************************************************

	Copyright 2012-2014 Luciano Buglioni - Pablo Zitelli

	Contacts:
        Luciano Buglioni: luciano.buglioni@gmail.com
        Pablo Zitelli:    zitelli.pablo@gmail.com
	This file is a part of FluxSol

	FluxSol is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    FluxSol is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    For a copy of the GNU General Public License,
    see <http://www.gnu.org/licenses/>.

*************************************************************************/
#include "FluxSol.h"
#include "Ex13-ResultsToCGNS.h"

// Solves a Laplacian and output Results in a CGNS file

int main(int argc, char *argv[])
//int main ()
{

    cout << "Reading : "<< argv[1]<<endl;

    Fv_CC_Grid mesh(argv[1]);

    write_unstructured();

    return 0;

}


//TAKEN FROM CGNS WRITE_TEST EXAMPLE

void write_coords(int nz)
{
    int k, nn, n, nij, koff, cgcoord;
    float exps[5];

    koff = nz == 1 ? 1 - NUM_SIDE : 0;
    nij = NUM_SIDE * NUM_SIDE;
    for (n = 0, k = 0; k < NUM_SIDE; k++) {
        for (nn = 0; nn < nij; nn++)
            zcoord[n++] = (float)(k + koff);
    }
    for (n = 0; n < 5; n++)
        exps[n] = (float)0.0;
    exps[1] = (float)1.0;

    if (cg_coord_write(cgfile, cgbase, nz, CGNS_ENUMV(RealSingle),
            "CoordinateX", xcoord, &cgcoord) ||
        cg_goto(cgfile, cgbase, "Zone_t", nz, "GridCoordinates_t", 1,
            "CoordinateX", 0, NULL) ||
        cg_exponents_write(CGNS_ENUMV(RealSingle), exps) ||
        cg_coord_write(cgfile, cgbase, nz, CGNS_ENUMV(RealSingle),
            "CoordinateY", ycoord, &cgcoord) ||
        cg_goto(cgfile, cgbase, "Zone_t", nz, "GridCoordinates_t", 1,
            "CoordinateY", 0, NULL) ||
        cg_exponents_write(CGNS_ENUMV(RealSingle), exps) ||
        cg_coord_write(cgfile, cgbase, nz, CGNS_ENUMV(RealSingle),
            "CoordinateZ", zcoord, &cgcoord) ||
        cg_goto(cgfile, cgbase, "Zone_t", nz, "GridCoordinates_t", 1,
            "CoordinateZ", 0, NULL) ||
        cg_exponents_write(CGNS_ENUMV(RealSingle), exps)) {
        sprintf (errmsg, "zone %d coordinates", nz);
        error_exit(errmsg);
    }
}

void write_elements(int nz)
{
    int cgsect;

    if (cg_section_write(cgfile, cgbase, nz, "Elements", CGNS_ENUMV(HEXA_8),
            1, num_element, 0, elements, &cgsect) ||
        cg_section_write(cgfile, cgbase, nz, "Faces", CGNS_ENUMV(QUAD_4),
            num_element+1, num_element+num_face, 0, faces, &cgsect) ||
        cg_parent_data_write(cgfile, cgbase, nz, cgsect, parent)) {
        sprintf (errmsg, "zone %d elements", nz);
        error_exit(errmsg);
    }
}

/*------------------------------------------------------------------------
 * unstructured grid
 *------------------------------------------------------------------------*/

void write_unstructured()
{
    int n, nelem, cgconn, cgbc;
    cgsize_t range[2];
    int cgsol, cgfld;
    char name[33];
    float exps[5];
#ifdef UNSTRUCTURED_1TO1
    int d_range[2], transform;
#else
# ifdef ABUTTING1TO1_FACES
    GridLocation_t location;
# else
    int i, j;
# endif
#endif

    printf ("writing unstructured base\n");
    fflush (stdout);

    if (cg_base_write(cgfile, "Unstructured", CellDim, PhyDim, &cgbase) ||
        cg_goto(cgfile, cgbase, "end") ||
        cg_descriptor_write("Descriptor", "Multi-block Unstructured Grid") ||
        cg_dataclass_write(CGNS_ENUMV(NormalizedByDimensional)) ||
        cg_units_write(CGNS_ENUMV(Kilogram), CGNS_ENUMV(Meter),
            CGNS_ENUMV(Second), CGNS_ENUMV(Kelvin), CGNS_ENUMV(Radian)))
        error_exit("unstructured base");

    /* write zones */

    for (n = 0; n < 9; n++)
        size[n] = 0;
    size[0] = num_coord;
    size[1] = num_element;
    for (n = 1; n <= 2; n++) {
        sprintf(name, "Zone%d", n);
        if (cg_zone_write(cgfile, cgbase, name, size,
                CGNS_ENUMV(Unstructured), &cgzone)) {
            sprintf (errmsg, "unstructured zone %d", n);
            error_exit(errmsg);
        }
        write_coords(n);
        write_elements(n);
    }
    nelem = (NUM_SIDE - 1) * (NUM_SIDE - 1);

#ifdef UNSTRUCTURED_1TO1

    /* write connectivities as 1to1 */

    range[0] = NODE_INDEX(1, 1, NUM_SIDE);
    range[1] = NODE_INDEX(NUM_SIDE, NUM_SIDE, NUM_SIDE);
    d_range[0] = NODE_INDEX(1, 1, 1);
    d_range[1] = NODE_INDEX(NUM_SIDE, NUM_SIDE, 1);
    transform = 1;
    if (cg_1to1_write(cgfile, cgbase, 1, "1to1 -> Zone2", "Zone2",
            range, d_range, &transform, &cgconn))
        error_exit("1to1->zone2");

    if (cg_1to1_write(cgfile, cgbase, 2, "1to1 -> Zone1", "Zone1",
            d_range, range, &transform, &cgconn))
        error_exit("1to1->zone1");

#else
# ifdef ABUTTING1TO1_FACES

    /* zone 1 to zone 2 connectivity as Abutting1to1 with element range */

    range[0] = num_element + num_face - nelem + 1;
    range[1] = num_element + num_face;
    for (n = 0; n < nelem; n++)
        d_pts[n] = range[0] + n - nelem;

    location = FaceCenter;

    /* this fail for version prior to 2.2 - see below */
    if (cg_conn_write(cgfile, cgbase, 1, "Abutting1to1 -> Zone2",
            location, CGNS_ENUMV(Abutting1to1),
            CGNS_ENUMV(PointRange), 2, range, "Zone2",
            CGNS_ENUMV(Unstructured), CGNS_ENUMV(PointListDonor),
            CGNS_ENUMV(Integer), nelem, d_pts, &cgconn))
        error_exit("face center abutting1to1->zone2");

    /* zone 2 to zone 1 connectivity as Abutting1to1 with element list */

    for (n = 0; n < nelem; n++) {
        pts[n]   = num_element + num_face - 2 * nelem + 1 + n;
        d_pts[n] = pts[n] + nelem;
    }
    if (cg_conn_write(cgfile, cgbase, 2, "Abutting1to1 -> Zone1",
            location, CGNS_ENUMV(Abutting1to1),
            CGNS_ENUMV(PointList), nelem, pts, "Zone1",
            CGNS_ENUMV(Unstructured), CGNS_ENUMV(PointListDonor),
            CGNS_ENUMV(Integer), nelem, d_pts, &cgconn))
        error_exit("face center abutting1to1->zone1");

# else

    /* zone 1 to zone 2 connectivity as Abutting1to1 with point range */

    range[0] = NODE_INDEX(1, 1, NUM_SIDE);
    range[1] = NODE_INDEX(NUM_SIDE, NUM_SIDE, NUM_SIDE);
    for (n = 0, j = 1; j <= NUM_SIDE; j++) {
        for (i = 1; i <= NUM_SIDE; i++)
            d_pts[n++] = NODE_INDEX(i, j, 1);
    }
    if (cg_conn_write(cgfile, cgbase, 1, "Abutting1to1 -> Zone2",
            CGNS_ENUMV(Vertex), CGNS_ENUMV(Abutting1to1),
            CGNS_ENUMV(PointRange), 2, range, "Zone2",
            CGNS_ENUMV(Unstructured), CGNS_ENUMV(PointListDonor),
            CGNS_ENUMV(Integer), npts, d_pts, &cgconn))
        error_exit("point range abutting1to1->zone2");

    /* zone 2 to zone 1 connectivity as Abutting1to1 with point list */

    for (n = 0, j = 1; j <= NUM_SIDE; j++) {
        for (i = 1; i <= NUM_SIDE; i++) {
            pts[n] = d_pts[n];
            d_pts[n++] = NODE_INDEX(i, j, NUM_SIDE);
        }
    }
    if (cg_conn_write(cgfile, cgbase, 2, "Abutting1to1 -> Zone1",
            CGNS_ENUMV(Vertex), CGNS_ENUMV(Abutting1to1),
            CGNS_ENUMV(PointList), npts, pts, "Zone1",
            CGNS_ENUMV(Unstructured), CGNS_ENUMV(PointListDonor),
            CGNS_ENUMV(Integer), npts, d_pts, &cgconn))
        error_exit("point list abutting1to1->zone1");

# endif
#endif

    /* write inlet BC (zone 1) and outlet BC (zone 2) as element lists
       and zone 1 and zone 2 walls as element range */

    range[0] = num_element + 1;
    range[1] = num_element + 4 * nelem;
    for (n = 0; n < nelem; n++) {
        pts[n]   = num_element + num_face - 2 * nelem + 1 + n;
        d_pts[n] = pts[n] + nelem;
    }

    if (cg_boco_write(cgfile, cgbase, 1, "Inlet", CGNS_ENUMV(BCInflow),
            CGNS_ENUMV(ElementList), nelem, pts, &cgbc))
        error_exit ("elementlist inlet boco");
    if (cg_boco_write(cgfile, cgbase, 2, "Outlet", CGNS_ENUMV(BCOutflow),
            CGNS_ENUMV(ElementList), nelem, d_pts, &cgbc))
        error_exit ("elementlist outlet boco");
    if (cg_boco_write(cgfile, cgbase, 1, "Walls", CGNS_ENUMV(BCWall),
            CGNS_ENUMV(ElementRange), 2, range, &cgbc))
        error_exit ("elementrange zone 1 walls boco");
    if (cg_boco_write(cgfile, cgbase, 2, "Walls", CGNS_ENUMV(BCWall),
            CGNS_ENUMV(ElementRange), 2, range, &cgbc))
        error_exit ("elementrange zone 2 walls boco");

    /* write solution for zone 1 as vertex and zone 2 as cell center */

    exps[0] = (float)1.0;
    exps[1] = (float)-3.0;
    for (n = 2; n < 5; n++)
        exps[n] = (float)0.0;
    if (cg_sol_write(cgfile, cgbase, 1, "VertexSolution",
            CGNS_ENUMV(Vertex), &cgsol) ||
        cg_field_write(cgfile, cgbase, 1, cgsol, CGNS_ENUMV(RealSingle),
            "Density", solution, &cgfld) ||
        cg_goto(cgfile, cgbase, "Zone_t", 1, "FlowSolution_t", cgsol,
            "Density", 0, NULL) ||
        cg_exponents_write(CGNS_ENUMV(RealSingle), exps))
        error_exit("zone 1 solution");
    if (cg_sol_write(cgfile, cgbase, 2, "CellCenterSolution",
            CGNS_ENUMV(CellCenter), &cgsol) ||
        cg_field_write(cgfile, cgbase, 2, cgsol, CGNS_ENUMV(RealSingle),
            "Density", solution, &cgfld) ||
        cg_goto(cgfile, cgbase, "Zone_t", 2, "FlowSolution_t", cgsol,
            "Density", 0, NULL) ||
        cg_exponents_write(CGNS_ENUMV(RealSingle), exps))
        error_exit("zone 2 solution");
}

