#include <memory>
#include <vips/vips8>

namespace jsvips {

    // Write to a char* by using std::snprintf and then convert that to a std::string
    template<typename ... Args>
    std::string format( const std::string& format, Args ... args )
    {
        size_t size = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
        if( size <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
        std::unique_ptr<char[]> buf( new char[ size ] ); 
        std::snprintf( buf.get(), size, format.c_str(), args ... );
        return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
    };

    VipsCompassDirection to_compass_direction(const std::string &position, const VipsCompassDirection default_position = VipsCompassDirection::VIPS_COMPASS_DIRECTION_CENTRE);

    template<class T, std::size_t n>
    std::size_t array_size(T (&)[n])
    { return n; }
}