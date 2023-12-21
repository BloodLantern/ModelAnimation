#pragma once

#include <rapidjson/document.h>

#include "sparse_indices.h"
#include "sparse_values.h"

class AccessorSparse
{
public:
    int count;
    SparseIndices indices;
    SparseValues values;

    AccessorSparse() = default;

    explicit AccessorSparse(const rapidjson::Value& value);
};
