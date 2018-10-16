#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/positioning.hpp>
#include <cstring>
#include <iostream>

using boost::iostreams::file_sink;
using boost::iostreams::file_source;
using boost::iostreams::position_to_offset;
using boost::iostreams::seek;
using boost::iostreams::stream_offset;

static const stream_offset MB = 1000 * 1000;

void setup()
{
    file_sink out("file1", BOOST_IOS::binary);
    const char *greetings[] = {"Hello", "Boost", "World"};
    for (int i = 0; i < 3; i++) {
        out.write(greetings[i], 5);
        seek(out, 3 * MB, BOOST_IOS::cur);
    }
}

void copy_file1_to_file2()
{
    file_source in("file1", BOOST_IOS::binary);
    file_sink out("file2", BOOST_IOS::binary);
    stream_offset off;

    off = position_to_offset(seek(in, -5, BOOST_IOS::end));
    std::cout << "in: seek " << off << std::endl;

    for (int i = 0; i < 3; i++) {
        char buf[6];
        std::memset(buf, '\0', sizeof buf);

        std::streamsize nr = in.read(buf, 5);
        std::streamsize nw = out.write(buf, 5);
        std::cout << "read: \"" << buf << "\"(" << nr << "), "
                  << "written: (" << nw << ")" << std::endl;

        off = position_to_offset(seek(in, -(3 * MB + 10), BOOST_IOS::cur));
        std::cout << "in: seek " << off << std::endl;
        off = position_to_offset(seek(out, 3 * MB, BOOST_IOS::cur));
        std::cout << "out: seek " << off << std::endl;
    }
}

int main()
{
    setup();
    copy_file1_to_file2();
}