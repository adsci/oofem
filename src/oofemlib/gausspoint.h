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

/*
 * The original idea for this class comes from
 * Dubois-Pelerin, Y.: "Object-Oriented  Finite Elements: Programming concepts and Implementation",
 * PhD Thesis, EPFL, Lausanne, 1992.
 */

#ifndef gausspoint_h
#define gausspoint_h

#include "oofemcfg.h"
#include "integrationrule.h"
#include "integrationpointstatus.h"
#include "element.h"
#include "materialmode.h"

namespace oofem {
class Material;
class LayeredCrossSection;
class MicroplaneMaterial;
class FiberedCrossSection;
class CrossSction;
class IntegrationRule;

/**
 * Class representing integration point in finite element program.
 * Integration point maintain its space position, integration
 * weight and corresponding material mode.
 * Link to related element which integration point belongs to
 * is also maintained.
 * Integration point generally can contain list of slave integration points
 * therefore is called as master point. Slaves are used for example to implement
 * layered or fibered cross sections by cross section class. Then in one
 * "macro" master gauss point, cross section creates few slaves (one per layer)
 * and puts them into master list. When cross sections completes requests for
 * particular master integration point, it performs integration over layers.
 * It therefore calls material class for each layer, sending corresponding
 * slave as parameter and integrates results.
 * Generally, every integration point must hold
 * its own copy of history variables (which are related to corresponding
 * material model used). These material type dependent history variables
 * are stored in material type related material status, which can be
 * managed by integration point.
 * Each material model class should introduce related material status class
 * (derived from material status class or from its children), where necessary
 * history variables are kept and can be accessed by material.
 * Material class then creates unique copy of related status in all necessary
 * integration points. Because integration point is compulsory parameter of
 * all member functions of material class, particular material then can
 * easily access its associated status from integration point and therefore its
 * history variables for particular integration point.
 *
 * To provide support to integrate element contribution by parts from element subVolumes
 * (possibly with different material parameters etc), the integration point can maintain not
 * only its element natural coordinates, but also its subVolume local coordinates, that are necessary
 * to compute its jacobian, for example. These coordinates are stored in localCoordinates attribute.
 *
 */
class OOFEM_EXPORT GaussPoint
{
private:
    /// Number.
    int number;
    /// Reference to parent integration rule.
    IntegrationRule *irule;
    /// Natural Element Coordinates of receiver.
    FloatArray *naturalCoordinates;
    /// Optional local sub-patch (sub-patches form element volume) coordinates of the receiver.
    FloatArray *subPatchCoordinates;
    /// Optional global (Cartesian) coordinates
    FloatArray *globalCoordinates;
    /// Integration weight.
    double weight;
    /// Material mode of receiver.
    MaterialMode materialMode;

protected:
    // layer and fibered material support
    /// List of slave integration points.
    std::vector< GaussPoint * >gaussPoints;
    /// Status of e.g. material in point
    IntegrationPointStatus *materialStatus;

public:
    /**
     * Creates integration point belonging to given integration rule,
     * with given number, integration weight, coordinates and material mode.
     * @param ir Integration rule to which integration point belongs to.
     * @param n Integration point number.
     * @param iNaturalCoord Natueral coordinates.
     * @param w Integration weight.
     * @param mode Material mode.
     */
    GaussPoint(IntegrationRule * ir, int n, FloatArray * iNaturalCoord, double w, MaterialMode mode);

    GaussPoint(IntegrationRule * ir, int n, double w, MaterialMode mode);

    /// Destructor
    virtual ~GaussPoint();

    /// Returns i-th natural element coordinate of receiver
    double giveNaturalCoordinate(int i) const { return naturalCoordinates->at(i); }
    /// Returns coordinate array of receiver.
    FloatArray *giveNaturalCoordinates() { return naturalCoordinates; }
    void setNaturalCoordinates(FloatArray c) {
        if(naturalCoordinates != NULL) {
            * naturalCoordinates = std :: move(c);
        }
        else {
            naturalCoordinates = new FloatArray( std :: move(c) );
        }
    }

    /// Returns local sub-patch coordinates of the receiver
    FloatArray *giveSubPatchCoordinates() {
        if ( subPatchCoordinates ) {
            return subPatchCoordinates;
        } else {
            return naturalCoordinates;
        }
    }
    void setLocalCoordinates(FloatArray c)
    {
        if ( subPatchCoordinates ) {
            * subPatchCoordinates = std :: move(c);
        } else {
            subPatchCoordinates = new FloatArray(std :: move(c));
        }
    }

    inline const FloatArray &giveGlobalCoordinates() {
        if( globalCoordinates != NULL ) {
            return *globalCoordinates;
        }
        else {
            globalCoordinates = new FloatArray();
            this->giveElement()->computeGlobalCoordinates(*globalCoordinates, *naturalCoordinates);
            return *globalCoordinates;
        }
    }

    void setGlobalCoordinates(const FloatArray &iCoord) {
        if( globalCoordinates != NULL ) {
            *globalCoordinates = iCoord;
        }
        else {
            globalCoordinates = new FloatArray(iCoord);
        }
    }

    /// Returns  integration weight of receiver.
    virtual double giveWeight() { return weight; }
    void setWeight(double w) { weight = w; }
    /// Returns number of receiver.
    int giveNumber() { return number; }
    /// Returns corresponding integration rule to receiver.
    IntegrationRule *giveIntegrationRule() { return irule; }
    /// Returns corresponding element to receiver.
    Element *giveElement() { return irule->giveElement(); }

    /// Returns corresponding material mode of receiver.
    MaterialMode giveMaterialMode() { return this->materialMode; }
    /// Sets material mode of receiver.
    void setMaterialMode(MaterialMode newMode) { this->materialMode = newMode; }
    ///@todo giveMaterial routine most be removed from gauss-points, it doesn't fit with different types of cross-sections.

    /// Returns reference to material associated to related element of receiver.
    Material *giveMaterial() { return giveElement()->giveMaterial(); }

    /// Returns reference to cross section associated to related element of receiver.
    CrossSection *giveCrossSection() { return giveElement()->giveCrossSection(); }

    /**
     * Returns reference to associated material status (NULL if not defined).
     */
    IntegrationPointStatus *giveMaterialStatus() { return this->materialStatus; };

    /**
     * Sets Material status managed by receiver.
     * @param ptr Pointer to new status of receiver.
     * @return Pointer to new status.
     * @deprecated should be removed since only one mat stat is saved in the integration point
     */
    IntegrationPointStatus *setMaterialStatus(IntegrationPointStatus *ptr, int n)
    {
        return this->setMaterialStatus(ptr);
    }

    /**
     * Sets Material status managed by receiver.
     * @param ptr Pointer to new status of receiver.
     * @return Pointer to new status.
     */
    IntegrationPointStatus *setMaterialStatus(IntegrationPointStatus *ptr)
    {
        if ( this->materialStatus != NULL ) {
            OOFEM_ERROR("status already exist");
        }
        this->materialStatus = ptr;
        return ptr;
    }
    /**
     * Returns index-th slave gauss point of receiver.
     * @param index Index of returned slave.
     * @return Slave gp.
     */
    GaussPoint *giveSlaveGaussPoint(int index);
    /**
     * Prints output of receiver to file. Corresponding printOutputAt  function for
     * associated status is called. The same function is also invoked for all available
     * slaves of receiver.
     */
    virtual void printOutputAt(FILE *file, TimeStep *tStep);
    /**
     * Updates internal state of receiver after finishing time step.
     * Material::updateYourself (receiver, tStep) function is called to
     * update material status. Same function is also invoked for
     * all receiver's slaves.
     */
    virtual void updateYourself(TimeStep *tStep);

    // store & restore context functions
    /*
     * Stores receiver state to output stream, including associated material status.
     * Warning: Slaves are not saved, they must be saved by corresponding
     * cross section or material model, which creates these slaves.
     * This is because they may have special weights and coordinates,
     * and these are not saved into context file, because they remain the same
     * during whole solution (only variables which vary are stored into context).
     * Note: does not invoke FEMComponents saveContext, since this writes only
     * class id header, but typically due to large number of IPs,
     * this is avoided.
     * @exception throws an ContextIOERR exception if error encountered.
     */
    //contextIOResultType saveContext (FILE* stream, void *obj = NULL);
    /*
     * Restores receiver state to output stream, including associated material status.
     * Warning: Slaves are not restored, they must be restored by corresponding
     * cross section or material model, which creates these slaves.
     * This is because they may have special weights and coordinates,
     * and these are not saved into context file, because they remain the same
     * during whole solution (only variables which vary are stored into context).
     * Note: does not invoke FEMComponents restoreContext, since this writes only
     * class id header, but typically due to large number of IPs,
     * this is avoided.
     * @exception throws an ContextIOERR exception if error encountered.
     */
    //contextIOResultType restoreContext(FILE* stream, void *obj = NULL);

    /// Returns class name of the receiver.
    virtual const char *giveClassName() const { return "GaussPoint"; }
    /// Initializes receiver according to object description stored in input record.
    virtual IRResultType initializeFrom(InputRecord *ir) { return IRRT_OK; }

    friend class LayeredCrossSection;
    friend class MicroplaneMaterial;
    friend class FiberedCrossSection;
    //friend class Material;
    //friend class LayeredMaterial;
};

typedef GaussPoint IntegrationPoint;
} // end namespace oofem
#endif // gausspoint_h
