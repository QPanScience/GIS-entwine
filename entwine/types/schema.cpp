/******************************************************************************
* Copyright (c) 2016, Connor Manning (connor@hobu.co)
*
* Entwine -- Point cloud indexing
*
* Entwine is available under the terms of the LGPL2 license. See COPYING
* for specific license text and more information.
*
******************************************************************************/

#include <entwine/types/schema.hpp>

#include <pdal/PointLayout.hpp>

#include <entwine/types/simple-point-layout.hpp>

namespace
{
    std::unique_ptr<pdal::PointLayout> makePointLayout(
            std::vector<entwine::DimInfo>& dims)
    {
        std::unique_ptr<pdal::PointLayout> layout(new SimplePointLayout());
        for (auto& dim : dims)
        {
            dim.setId(layout->registerOrAssignDim(dim.name(), dim.type()));
        }

        layout->finalize();

        return layout;
    }

    /*
    std::vector<entwine::DimInfo> makeDims(const pdal::PointLayout& layout)
    {
        std::vector<entwine::DimInfo> dims;
        for (const auto& id : layout.dims())
        {
            dims.push_back(
                    entwine::DimInfo(
                        layout.dimName(id),
                        id,
                        layout.dimType(id)));

        }
        return dims;
    }
    */
}

namespace entwine
{

Schema::Schema(std::vector<DimInfo> dims)
    : m_layout(makePointLayout(dims))
    , m_dims(dims)
{ }

/*
Schema::Schema(const pdal::PointLayoutPtr layout)
    : m_dims(makeDims(*layout.get()))
    , m_layout(layout)
{ }
*/

Schema::~Schema()
{ }

std::size_t Schema::pointSize() const
{
    return m_layout->pointSize();
}

const std::vector<DimInfo>& Schema::dims() const
{
    return m_dims;
}

pdal::PointLayout& Schema::pdalLayout() const
{
    return *m_layout.get();
}

Json::Value Schema::toJson() const
{
    Json::Value json;
    for (const auto& dim : m_dims)
    {
        Json::Value cur;
        cur["name"] = dim.name();
        cur["type"] = dim.typeString();
        cur["size"] = static_cast<Json::UInt64>(dim.size());
        json.append(cur);
    }
    return json;
}

DimList Schema::fromJson(const Json::Value& json)
{
    std::vector<DimInfo> dims;
    for (Json::ArrayIndex i(0); i < json.size(); ++i)
    {
        const Json::Value& jsonDim(json[i]);
        dims.push_back(
                DimInfo(
                    jsonDim["name"].asString(),
                    jsonDim["type"].asString(),
                    jsonDim["size"].asUInt64()));
    }
    return dims;
}

} // namespace entwine
