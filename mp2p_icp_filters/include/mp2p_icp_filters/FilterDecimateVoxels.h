/* -------------------------------------------------------------------------
 *  A repertory of multi primitive-to-primitive (MP2P) ICP algorithms in C++
 * Copyright (C) 2018-2021 Jose Luis Blanco, University of Almeria
 * See LICENSE for license information.
 * ------------------------------------------------------------------------- */
/**
 * @file   FilterDecimateVoxels.h
 * @brief  Builds a new layer with a decimated version of an input layer.
 * @author Jose Luis Blanco Claraco
 * @date   Sep 10, 2021
 */

#pragma once

#include <mp2p_icp/pointcloud.h>
#include <mp2p_icp_filters/FilterBase.h>
#include <mp2p_icp_filters/PointCloudToVoxelGrid.h>
#include <mrpt/maps/CPointsMap.h>

namespace mp2p_icp_filters
{
/** Builds a new layer with a decimated version of an input layer.
 *
 * This builds a voxel grid from the input point cloud, and then takes either,
 * the mean of the points in the voxel, or one of the points picked at random,
 * depending on the parameter `use_voxel_average`.
 *
 * If the given output pointcloud layer already exists, new points will be
 * appended, without clearing the former contents.
 *
 * Not compatible with calling from different threads simultaneously for
 * different input point clouds. Use independent instances for each thread if
 * needed.
 *
 * \ingroup mp2p_icp_filters_grp
 */
class FilterDecimateVoxels : public mp2p_icp_filters::FilterBase
{
    DEFINE_MRPT_OBJECT(FilterDecimateVoxels, mp2p_icp_filters)
   public:
    FilterDecimateVoxels();

    // See docs in base class.
    void initialize(const mrpt::containers::yaml& c) override;

    // See docs in FilterBase
    void filter(mp2p_icp::pointcloud_t& inOut) const override;

    struct Parameters
    {
        void load_from_yaml(const mrpt::containers::yaml& c);

        std::string input_pointcloud_layer =
            mp2p_icp::pointcloud_t::PT_LAYER_RAW;

        /** The output point cloud layer name */
        std::string output_pointcloud_layer;

        /** Size of each voxel edge [meters] */
        double voxel_filter_resolution = .20;  // [m]

        /** If enabled, the mean of each voxel is taken instead of any of
         *  the original points. */
        bool use_voxel_average = false;

        float init_extension_min_x = -10.0f;
        float init_extension_min_y = -10.0f;
        float init_extension_min_z = -5.0f;
        float init_extension_max_x = 10.0f;
        float init_extension_max_y = 10.0f;
        float init_extension_max_z = 5.0f;
    };

    /** Algorithm parameters */
    Parameters params_;

   private:
    mutable PointCloudToVoxelGrid filter_grid_;
};

/** @} */

}  // namespace mp2p_icp_filters
