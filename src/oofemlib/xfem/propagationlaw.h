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
 *               Copyright (C) 1993 - 2013   Borek Patzak
 *
 *
 *
 *       Czech Technical University, Faculty of Civil Engineering,
 *   Department of Structural Mechanics, 166 29 Prague, Czech Republic
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


#ifndef PROPAGATIONLAW_H_
#define PROPAGATIONLAW_H_

#include "oofemcfg.h"
#include "inputrecord.h"

#define _IFT_PLDoNothing_Name "propagationlawdonothing"

#define _IFT_PLCrackPrescribedDir_Name "propagationlawcrackprescribeddir"
#define _IFT_PLCrackPrescribedDir_Dir "angle" // Angle in degrees
#define _IFT_PLCrackPrescribedDir_IncLength "incrementlength" // Increment per time step


#define _IFT_PLHoopStressCirc_Name "propagationlawhoopstresscirc"
// Radius of circle used for stress sampling points
#define _IFT_PLHoopStressCirc_Radius "radius"
// Angle between sampling points on the circle
#define _IFT_PLHoopStressCirc_AngleInc "angleinc"
// Increment length per time step
#define _IFT_PLHoopStressCirc_IncLength "incrementlength"
// Threshold for crack propagation
#define _IFT_PLHoopStressCirc_HoopStressThreshold "hoopstressthreshold"
// If radial basis functions should be used for stress interpolation
#define _IFT_PLHoopStressCirc_RadialBasisFunc "useradialbasisfunc"

namespace oofem {
class Domain;
class EnrichmentDomain;
class DynamicInputRecord;

/**
 * Updates the geometry of evolving XFEM interfaces.
 * @author Erik Svenning
 */

class OOFEM_EXPORT PropagationLaw
{
public:
    PropagationLaw();
    virtual ~PropagationLaw();

    virtual const char *giveClassName() const = 0;
    virtual const char *giveInputRecordName() const = 0;

    virtual IRResultType initializeFrom(InputRecord *ir) = 0;
    virtual void giveInputRecord(DynamicInputRecord &input) = 0;

    virtual void propagateInterfaces(Domain &iDomain, EnrichmentDomain &iEnrDom) = 0;
};

/**
 * Dummy propagation law that does nothing.
 * @author Erik Svenning
 */
class OOFEM_EXPORT PLDoNothing : public PropagationLaw
{
public:
    PLDoNothing() {};
    virtual ~PLDoNothing() {};

    virtual const char *giveClassName() const { return "PLDoNothing"; }
    virtual const char *giveInputRecordName() const { return _IFT_PLDoNothing_Name; }

    virtual IRResultType initializeFrom(InputRecord *ir) { return IRRT_OK; }
    virtual void giveInputRecord(DynamicInputRecord &input);

    virtual void propagateInterfaces(Domain &iDomain, EnrichmentDomain &ioEnrDom) {};
};

/**
 * Propagation law that propagates the crack in a predefined direction.
 * @author Erik Svenning
 */
class OOFEM_EXPORT PLCrackPrescribedDir : public PropagationLaw
{
public:
    PLCrackPrescribedDir() : mAngle(0.0), mIncrementLength(0.0) {};
    virtual ~PLCrackPrescribedDir() {};

    virtual const char *giveClassName() const { return "PLCrackPrescribedDir"; }
    virtual const char *giveInputRecordName() const { return _IFT_PLCrackPrescribedDir_Name; }

    virtual IRResultType initializeFrom(InputRecord *ir);
    virtual void giveInputRecord(DynamicInputRecord &input);

    virtual void propagateInterfaces(Domain &iDomain, EnrichmentDomain &ioEnrDom);

protected:
    double mAngle, mIncrementLength;
};

/**
 * Propagation law that propagates the crack in the direction
 * that gives \sigma_{r\theta} = 0.
 * Based on
 * T.P. Fries and M. Baydoun:
 * "Crack propagation with the extended finite element method
 * and a hybrid explicit-implicit crack description",
 * Internat. J. Numer. Methods Engrg 89,
 * pp. 1527--1558 (2012)
 *
 * The stress is evaluated in several points on a circle
 * surrounding the crack tip.
 *
 * Compared to the paper above, the implementation has been extended
 * with a criterion for crack propagation instead of always
 * propagating a predefined increment length. Two options are
 * currently available for stress interpolation:
 * 1) Take stress of closest Gauss point
 * 2) Interpolate with radial basis functions
 *
 * @author Erik Svenning
 */
class OOFEM_EXPORT PLHoopStressCirc : public PropagationLaw
{
public:
    PLHoopStressCirc() : mRadius(0.0), mAngleInc(0.0), mIncrementLength(0.0), mHoopStressThreshold(0.0), mUseRadialBasisFunc(false) {};
    virtual ~PLHoopStressCirc() {};

    virtual const char *giveClassName() const { return "PLHoopStressCirc"; }
    virtual const char *giveInputRecordName() const { return _IFT_PLHoopStressCirc_Name; }

    virtual IRResultType initializeFrom(InputRecord *ir);
    virtual void giveInputRecord(DynamicInputRecord &input);

    virtual void propagateInterfaces(Domain &iDomain, EnrichmentDomain &ioEnrDom);

protected:
    double mRadius, mAngleInc, mIncrementLength, mHoopStressThreshold;
    bool mUseRadialBasisFunc;
};
} // end namespace oofem

#endif /* PROPAGATIONLAW_H_ */
