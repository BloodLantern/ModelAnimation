#include "gltf/gltf_chunk_bin.h"

ChunkBin::ChunkBin(const unsigned int length, char type[4], std::ifstream& file)
    : Chunk(length, type)
{
}
