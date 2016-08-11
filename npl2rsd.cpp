%:include <cstdio>
%:include <vector>
%:include <memory>
%:include <cstring>
%:include <string>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

struct npl {
    struct nplHeader {
        u8 m_frameCount;
        u8 m_palette[ 32 ];
        u8 m_frameWidth;
        u8 m_frameHeight;
    } m_header;
    std::vector<std::unique_ptr<u8[]>> m_frames;
};

struct rsd {
    struct rsdHeader {
        u8 m_palette[ 32 ];
        u8 m_frameCount;
        u8 m_frameWidth;
        u8 m_frameHeight;
    } m_header;
    std::vector<std::unique_ptr<u8[]>> m_frames;
};

inline constexpr u8 reorder( u8 p_num ) {
    return ( p_num << 4 ) | ( p_num >> 4 );
}

int main( int p_argc, char** p_argv) {
    for( int i = 1; i < p_argc; ++i ) {
        printf( "Processing %s.\n", p_argv[ i ] );
        FILE* f = fopen( p_argv[ i ], "r" );

        npl in;
        fread( &in.m_header, sizeof( npl::nplHeader ), 1, f );
        for( int j = 0; j < in.m_header.m_frameCount; ++j ) {
            std::unique_ptr<u8[]> frame = std::unique_ptr<u8[]>( new u8[ in.m_header.m_frameWidth * in.m_header.m_frameHeight / 2 ] );
            fread( frame.get(), sizeof( u8 ), in.m_header.m_frameWidth * in.m_header.m_frameHeight / 2, f );
            in.m_frames.push_back( std::move( frame ) );
        }
        fclose( f );

        rsd out;
        out.m_header.m_frameCount = in.m_header.m_frameCount;
        out.m_header.m_frameWidth = in.m_header.m_frameWidth;
        out.m_header.m_frameHeight = in.m_header.m_frameHeight;

        memcpy( out.m_header.m_palette, in.m_header.m_palette, 32 * sizeof( u8 ) );

        int pos = 0;
%:ifndef NO_REORDER
        for( int j = 0; j < out.m_header.m_frameCount / 9; ++j, pos += 9 ) {
            out.m_frames.push_back( std::move( in.m_frames[ pos ] ) );
            out.m_frames.push_back( std::move( in.m_frames[ pos + 3 ] ) );
            out.m_frames.push_back( std::move( in.m_frames[ pos + 4 ] ) );

            out.m_frames.push_back( std::move( in.m_frames[ pos + 1 ] ) );
            out.m_frames.push_back( std::move( in.m_frames[ pos + 5 ] ) );
            out.m_frames.push_back( std::move( in.m_frames[ pos + 6 ] ) );

            out.m_frames.push_back( std::move( in.m_frames[ pos + 2 ] ) );
            out.m_frames.push_back( std::move( in.m_frames[ pos + 7 ] ) );
            out.m_frames.push_back( std::move( in.m_frames[ pos + 8 ] ) );
        }
%:endif
        for( int j = pos; j < out.m_header.m_frameCount; ++j )
            out.m_frames.push_back( std::move( in.m_frames[ j ] ) );

        for( auto& j : out.m_frames )
            for( int k = 0; k < out.m_header.m_frameWidth * out.m_header.m_frameHeight / 2; ++k )
                j[ k ] = reorder( j[ k ] );

        int len = strlen( p_argv[ i ] );
        f = fopen( std::string( p_argv[ i ] ).replace( len - 3, 3, "rsd" ).c_str(), "w" );
        fwrite( &out.m_header, sizeof( rsd::rsdHeader ), 1, f );
        for( auto& j : out.m_frames )
            fwrite( j.get(), sizeof( u8 ), out.m_header.m_frameWidth * out.m_header.m_frameHeight / 2, f );
        fclose( f );
    }
}
