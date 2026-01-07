#ifndef REALGA_MESSAGES_H
#define REALGA_MESSAGES_H

#include <stdexcept>
#include <string>
#include <sstream>

class RealGAException : public std::runtime_error
{
public:
    RealGAException(const std::string &msg)
        : std::runtime_error(msg) {}
};

#define REALGA_ERROR(condition, msg_expr)                                                                                         \
    if (condition)                                                                                                                \
    {                                                                                                                             \
        std::ostringstream oss;                                                                                                   \
        oss << msg_expr;                                                                                                          \
        throw RealGAException(std::string(__func__) + ": " + oss.str() + " (" + __FILE__ + ":" + std::to_string(__LINE__) + ")"); \
    }

#define DEBUG(msg)                        \
    {                                     \
        cout << "DEBUG: " << msg << endl; \
    }
#endif // REALGA_MESSAGES_H