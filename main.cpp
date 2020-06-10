#include <stdint.h>
#include <tetra_source/Level2NativeLibSo/TetraLogicalProc/tetra_dl_stream.h>
#include <tetra_source/Level3NativeLibSo/CommonHostLib/types.h>
#include <memory>
#include <fstream>
#include <iostream>
#include <vector>
#include <assert.h>

int main(int argc, char *argv[])
{
    std::cout << "Start Tetra processing\n";
    assert( argc == 2 );

    std::ifstream in( argv[ 1 ], std::ifstream::binary );
    assert( in.is_open( ) );


    in.seekg( 0, in.end );
    int32_t actualLength = in.tellg( ) / sizeof( WordComplex );
    in.seekg( 0, in.beg );

    std::vector<WordComplex> iq(actualLength);

    in.read( reinterpret_cast<char*>(iq.data()), actualLength * sizeof( WordComplex ) );
    in.seekg( 0, in.beg );


    static const uint32_t numChannels = 4;
    static const uint32_t filterCount = 20;
    static const uint32_t tetraSlotSize = 510;

//    void koko();
//    koko();

    std::shared_ptr<Tetra::TetraDlStream> stream;
    stream = std::make_shared<Tetra::TetraDlStream>(filterCount * numChannels);

    uint32_t tetraSlotCount = actualLength/tetraSlotSize;
    uint32_t currFilter = 16;

    auto iqPtr = iq.data();
    for(uint32_t sl = 0; sl < tetraSlotCount; ++sl) {
        stream->Process( &iqPtr[ sl* tetraSlotSize + numChannels * currFilter], tetraSlotSize );
    }
}

