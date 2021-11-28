/* -------------------------------------------------------------------------
 *  A repertory of multi primitive-to-primitive (MP2P) ICP algorithms in C++
 * Copyright (C) 2018-2021 Jose Luis Blanco, University of Almeria
 * See LICENSE for license information.
 * ------------------------------------------------------------------------- */
/**
 * @file   Matcher_Point2Plane.h
 * @brief  Pointcloud matcher: point to plane-fit of nearby points
 * @author Jose Luis Blanco Claraco
 * @date   July 21, 2020
 */
#pragma once

#include <mp2p_icp/Matcher_Points_Base.h>

namespace mp2p_icp
{
/** Pointcloud matcher: point to plane-fit of nearby points
 *
 * Finds point-to-plane pairings between `local` point layers and points fitting
 * a plane in layers of the `global` input metric map.
 *
 * By default, each `local` point layer is matched against the layer with the
 * same name in the `global` map, unless especified otherwise in the base class
 * member `weight_pt2pt_layers`. Refer to example configuration YAML files for
 * example configurations.
 *
 *
 * \ingroup mp2p_icp_grp
 */
class Matcher_Point2Plane : public Matcher_Points_Base
{
    DEFINE_MRPT_OBJECT(Matcher_Point2Plane, mp2p_icp)

   public:
    Matcher_Point2Plane();

    /** Parameters:
     * - `distanceThreshold`: Inliers distance threshold [meters][mandatory]
     * - `knn`: Number of neighbors to look for [mandatory]
     * - `minimumPlanePoints`: Minimum number of found points [mandatory]
     * - `planeEigenThreshold`: maximum e0/e2 ratio [mandatory]
     *
     * Where e0 and e2 are the smallest and largest eigenvalues of the Gaussian
     * covariance fitting the knn closest global points for each local point.
     *
     * Plus: the parameters of Matcher_Points_Base::initialize()
     */
    void initialize(const mrpt::containers::yaml& params) override;

   private:
    double   distanceThreshold   = 0.50;
    uint32_t knn                 = 5;
    uint32_t minimumPlanePoints  = 5;
    double   planeEigenThreshold = 0.01;

    void implMatchOneLayer(
        const mrpt::maps::CMetricMap& pcGlobal,
        const mrpt::maps::CPointsMap& pcLocal,
        const mrpt::poses::CPose3D& localPose, MatchState& ms,
        const layer_name_t& globalName, const layer_name_t& localName,
        Pairings& out) const override;
};

}  // namespace mp2p_icp
