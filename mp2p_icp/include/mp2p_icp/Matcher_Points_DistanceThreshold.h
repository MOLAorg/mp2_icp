/* -------------------------------------------------------------------------
 *  A repertory of multi primitive-to-primitive (MP2P) ICP algorithms in C++
 * Copyright (C) 2018-2021 Jose Luis Blanco, University of Almeria
 * See LICENSE for license information.
 * ------------------------------------------------------------------------- */
/**
 * @file   Matcher_Points_DistanceThreshold.h
 * @brief  Pointcloud matcher: fixed distance thresholds
 * @author Jose Luis Blanco Claraco
 * @date   June 22, 2020
 */
#pragma once

#include <mp2p_icp/Matcher_Points_Base.h>

namespace mp2p_icp
{
/** Pointcloud matcher: fixed distance thresholds
 *
 * \ingroup mp2p_icp_grp
 */
class Matcher_Points_DistanceThreshold : public Matcher_Points_Base
{
    DEFINE_MRPT_OBJECT(Matcher_Points_DistanceThreshold, mp2p_icp)

   public:
    Matcher_Points_DistanceThreshold();

    Matcher_Points_DistanceThreshold(double distThreshold)
        : Matcher_Points_DistanceThreshold()
    {
        threshold = distThreshold;
    }

    /*** Parameters:
     * - `threshold`: Inliers distance threshold [meters][mandatory]
     * - `pairingsPerPoint`: Number of pairings in "global" for each "local"
     * points. Default=1. If more than one, they will be picked in ascending
     * order of distance, up to `threshold`. [optional].
     *
     * Plus: the parameters of Matcher_Points_Base::initialize()
     */
    void initialize(const mrpt::containers::yaml& params) override;

   private:
    double       threshold        = 0.50;
    unsigned int pairingsPerPoint = 1;

    void implMatchOneLayer(
        const mrpt::maps::CPointsMap& pcGlobal,
        const mrpt::maps::CPointsMap& pcLocal,
        const mrpt::poses::CPose3D& localPose, MatchState& ms,
        const layer_name_t& globalName, const layer_name_t& localName,
        Pairings& out) const override;
};

}  // namespace mp2p_icp
