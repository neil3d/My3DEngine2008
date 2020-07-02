#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

//-- add by neil
size_t base64_decode(std::string const& encoded_string, std::vector<char>& out_buffer);