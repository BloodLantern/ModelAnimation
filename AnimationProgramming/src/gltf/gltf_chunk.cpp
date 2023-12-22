#include "gltf/gltf_chunk.h"

Chunk::Chunk(const unsigned int length, char type[4])
    : m_Length(length)
    , m_Type{ type[0], type[1], type[2], type[3] }
{
}

unsigned int Chunk::GetLength() const
{
    return m_Length;
}
