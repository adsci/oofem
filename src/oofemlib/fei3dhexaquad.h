/* $Header: /home/cvs/bp/oofem/oofemlib/src/fei3dhexalin.h,v 1.2 2003/05/19 13:03:57 bp Exp $ */
/*
 *
 *                 #####    #####   ######  ######  ###   ###
 *               ##   ##  ##   ##  ##      ##      ## ### ##
 *              ##   ##  ##   ##  ####    ####    ##  #  ##
 *             ##   ##  ##   ##  ##      ##      ##     ##
 *            ##   ##  ##   ##  ##      ##      ##     ##
 *            #####    #####   ##      ######  ##     ##
 *
 *
 *             OOFEM : Object Oriented Finite Element Code
 *
 *               Copyright (C) 1993 - 2008   Borek Patzak
 *
 *
 *
 *       Czech Technical University, Faculty of Civil Engineering,
 *   Department of Structural Mechanics, 166 29 Prague, Czech Republic
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

//   **************************************************
//   *** CLASS 3d quadratic hexahedra interpolation ***
//   **************************************************

// Recasted by L. Svoboda

#ifndef fei3dhexaquad_h
#define fei3dhexaquad_h


#include "flotarry.h"
#include "intarray.h"
#include "domain.h"
#include "feinterpol3d.h"

namespace oofem {

/**
 * Class representing implementation of quadratic hexahedra interpolation class.
 */
class FEI3dHexaQuad : public FEInterpolation3d
{
protected:

public:
    FEI3dHexaQuad() : FEInterpolation3d(1) { }
    /**
     *  Evaluates the array of interpolation functions (shape functions) at given point.
     *  @param answer contains resulting array of evaluated interpolation functions
     *  @param lcoords array containing (local) coordinates
     *  @param cellgeo underlying cell geometry
     *  @param time time
     */
    virtual void evalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry& cellgeo, double time);
    /**
     *  Evaluates the matrix of derivatives of interpolation functions (shape functions) at given point.
     *  These derivatives are in global coordinate system (where the nodal coordinates are defined)
     *  @param matrix contains resulting matrix of derivatives, the member at i,j position contains value of dNi/dxj
     *  @param lcoords array containing (local) coordinates
     *  @param cellgeo underlying cell geometry
     *  @param time time
     */
    virtual void evaldNdx(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry& cellgeo, double time);
    /**
     *  Evaluates global coordinates from given local ones
     *  These derivatives are in global coordinate system (where the nodal coordinates are defined)
     *  @param answer contains resulting global coordinates
     *  @param lcoords array containing (local) coordinates
     *  @param cellgeo underlying cell geometry
     *  @param time time
     */
    virtual void local2global(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry& cellgeo, double time);
    /**
     *  Evaluates local coordinates from given global ones. Returns nonzero if local coordinates are interpolating,
     *  zero if extrapolating (nonzero is returned if point is within the element geometry, zero otherwise).
     *  These derivatives are in global coordinate system (where the nodal coordinates are defined)
     *  @param answer contains evaluated local coordinates
     *  @param gcoords array containing global coordinates
     *  @param cellgeo underlying cell geometry
     *  @param time time
     *  @return nonzero is returned if point is within the element geometry, zero otherwise
     */
    virtual int  global2local(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry& cellgeo, double time);
    /**
     * Evaluates the jacobian of transformation between local and global coordinates.
     */
    virtual double giveTransformationJacobian(const FloatArray &lcoords, const FEICellGeometry& cellgeo, double time);

    /**@name Edge interpolation servises */
    //@{
    /**
     *  Evaluates the array of edge interpolation functions (shape functions) at given point.
     *  @param answer contains resulting array of evaluated interpolation functions
     *  @param lcoords array containing (local) coordinates
     *  @param cellgeo underlying cell geometry
     *  @param time time
     */
    virtual void edgeEvalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry& cellgeo, double time);
    /**
     *  Evaluates the matrix of derivatives of edge interpolation functions (shape functions) at given point.
     *  These derivatives are in global coordinate system (where the nodal coordinates are defined)
     *  @param answer contains resulting matrix of derivatives, the member at i,j position contains value of dNj/dxi
     *  @param iedge determines the edge number
     *  @param lcoords array containing (local) coordinates
     *  @param cellgeo underlying cell geometry
     *  @param time time
     */
    virtual void edgeEvaldNdx(FloatMatrix &answer, int iedge,
                              const FloatArray &lcoords, const FEICellGeometry& cellgeo, double time);
    /**
     *  Evaluates edge global coordinates from given local ones
     *  These derivatives are in global coordinate system (where the nodal coordinates are defined)
     *  @param answer contains resulting global coordinates
     *  @param iedge determines edge number
     *  @param lcoords array containing (local) coordinates
     *  @param cellgeo underlying cell geometry
     *  @param time time
     */
    virtual void edgeLocal2global(FloatArray &answer, int iedge,
                                  const FloatArray &lcoords, const FEICellGeometry& cellgeo, double time);
    /**
     * Evaluates the edge jacobian of transformation between local and global coordinates.
     */
    virtual double edgeGiveTransformationJacobian(int iedge, const FloatArray &lcoords,
                                                  const FEICellGeometry& cellgeo, double time);
    /**
     */
    virtual void computeLocalEdgeMapping(IntArray &edgeNodes, int iedge);
    //void computeGlobalEdgeMapping (IntArray& edgeNodes, IntArray& elemNodes, int iedge);
    //@}

    /**@name Surface interpolation services */
    //@{
    /**
     *  Evaluates the array of edge interpolation functions (shape functions) at given point.
     *  @param answer contains resulting array of evaluated interpolation functions
     *  @param lcoords array containing (local) coordinates
     *  @param cellgeo underlying cell geometry
     *  @param time time
     */
    virtual void surfaceEvalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry& cellgeo, double time);
    /**
     *  Evaluates the matrix of derivatives of edge interpolation functions (shape functions) at given point.
     *  These derivatives are in global coordinate system (where the nodal coordinates are defined)
     *  @param answer contains resulting matrix of derivatives, the member at i,j position contains value of dNj/dxi
     *  @param iedge determines the edge number
     *  @param lcoords array containing (local) coordinates
     *  @param cellgeo underlying cell geometry
     *  @param time time
     */
    //virtual void surfaceEvaldNdx (FloatMatrix&answer, int iedge,
    //               const FloatArray& lcoords, const FEICellGeometry& cellgeo, double time);
    /**
     *  Evaluates edge global coordinates from given local ones
     *  These derivatives are in global coordinate system (where the nodal coordinates are defined)
     *  @param answer contains resulting global coordinates
     *  @param iedge determines edge number
     *  @param lcoords array containing (local) coordinates
     *  @param cellgeo underlying cell geometry
     *  @param time time
     */
    virtual void surfaceLocal2global(FloatArray &answer, int iedge,
                                     const FloatArray &lcoords, const FEICellGeometry& cellgeo, double time);
    /**
     * Evaluates the edge jacobian of transformation between local and global coordinates.
     */
    virtual double surfaceGiveTransformationJacobian(int isurf, const FloatArray &lcoords,
                                                     const FEICellGeometry& cellgeo, double time);
    virtual void computeLocalSurfaceMapping(IntArray &nodes, int iSurf);
    void computeGlobalSurfaceMapping(IntArray &edgeNodes, IntArray &elemNodes, int iedge);
    //@}

protected:
    double edgeComputeLength(IntArray &edgeNodes, const FEICellGeometry& cellgeo);
    void   giveDerivativeKsi(FloatArray &, double, double, double);
    void   giveDerivativeEta(FloatArray &, double, double, double);
    void   giveDerivativeDzeta(FloatArray &, double, double, double);
public:
    void   giveJacobianMatrixAt(FloatMatrix &jacobianMatrix, const FloatArray &lcoords, const FEICellGeometry& cellgeo);
};

} // end namespace oofem
#endif
