#include "gltf/gltf_chunk.h"

Chunk::Chunk(const unsigned int length, char type[4])
    : length(length)
    , type{ type[0], type[1], type[2], type[3] }
{
}
