/* -------------------------------------------------------------------------
 * A repertory of multi primitive-to-primitive (MP2P) ICP algorithms in C++
 * Copyright (C) 2018-2023 Jose Luis Blanco, University of Almeria
 * See LICENSE for license information.
 * ------------------------------------------------------------------------- */
/**
 * @file   GeneratorEdgesFromCurvature.cpp
 * @brief  Generator of edge points from organized point clouds
 * @author Jose Luis Blanco Claraco
 * @date   Dec 6, 2023
 */

#include <mp2p_icp_filters/GeneratorEdgesFromCurvature.h>
#include <mrpt/containers/yaml.h>
#include <mrpt/maps/CSimplePointsMap.h>
#include <mrpt/math/utils.h>  // absDiff()
#include <mrpt/obs/CObservationRotatingScan.h>

#include <utility>  // std::pair

IMPLEMENTS_MRPT_OBJECT(GeneratorEdgesFromCurvature, Generator, mp2p_icp_filters)

using namespace mp2p_icp_filters;

void GeneratorEdgesFromCurvature::ParametersEdges::load_from_yaml(
    const mrpt::containers::yaml& c)
{
    MCP_LOAD_REQ(c, max_cosine);
    MCP_LOAD_OPT(c, min_point_clearance);
}

void GeneratorEdgesFromCurvature::initialize(const mrpt::containers::yaml& c)
{
    // parent base method:
    Generator::initialize(c);

    paramsEdges_.load_from_yaml(c);
}

bool GeneratorEdgesFromCurvature::filterRotatingScan(  //
    const mrpt::obs::CObservationRotatingScan& pc,
    mp2p_icp::metric_map_t&                    out) const
{
    auto outPc = mrpt::maps::CSimplePointsMap::Create();

    ASSERT_(!pc.organizedPoints.empty());

    const auto nRows = pc.rowCount;
    const auto nCols = pc.columnCount;

    ASSERT_EQUAL_(nRows, pc.organizedPoints.rows());
    ASSERT_EQUAL_(nCols, pc.organizedPoints.cols());

    ASSERT_EQUAL_(nRows, pc.rangeImage.rows());
    ASSERT_EQUAL_(nCols, pc.rangeImage.cols());

    // for each row:
    for (size_t r = 0; r < nRows; r++)
    {
        // compute range diff:
        for (size_t i = 1; i + 1 < nCols; i++)
        {
            // we need at least 3 consecutive valid points:
            if (!pc.rangeImage(r, i - 1) || !pc.rangeImage(r, i) ||
                !pc.rangeImage(r, i + 1))
                continue;

            const auto& pt_im1 = pc.organizedPoints(r, i - 1);
            const auto& pt_i   = pc.organizedPoints(r, i);
            const auto& pt_ip1 = pc.organizedPoints(r, i + 1);

            const auto v1  = (pt_i - pt_im1);
            const auto v2  = (pt_ip1 - pt_i);
            const auto v1n = v1.norm();
            const auto v2n = v2.norm();

            if (v1n < paramsEdges_.min_point_clearance ||
                v2n < paramsEdges_.min_point_clearance)
                continue;

            const float score = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

            if (std::abs(score) < paramsEdges_.max_cosine * v1n * v2n)
            {
                // this point passes:
                outPc->insertPoint(pc.organizedPoints(r, i));
            }
        }

    }  // end for each row

    out.layers[params_.target_layer] = outPc;
    return true;  // Yes, it's implemented
}
