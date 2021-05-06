#include <iostream>
#include <algorithm>
#include <iterator>
#include <boost/algorithm/hex.hpp>
#include <boost/uuid/detail/md5.hpp>


using boost::uuids::detail::md5;

std::string toString(const md5::digest_type &digest)
{
    const auto charDigest = reinterpret_cast<const char *>(&digest);
    std::string result;
    boost::algorithm::hex(charDigest, charDigest + sizeof(md5::digest_type), std::back_inserter(result));
    return result;
}

int main ()
{
    std::string s;

    while(std::getline(std::cin, s)) {
        md5 hash;
        md5::digest_type digest;

        hash.process_bytes(s.data(), s.size());
        hash.get_digest(digest);

        std::cout << "md5(" << s << ") = " << toString(digest) << '\n';
    }

    return 0;
}
