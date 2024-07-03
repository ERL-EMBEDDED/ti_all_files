#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H

#include <string>
#include <tuple>

std::tuple<int, int, int, int, std::string> loadConfig(const std::string& filename);

#endif // SERVER_CONFIG_H

