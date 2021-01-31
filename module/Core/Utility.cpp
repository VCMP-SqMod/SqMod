// ------------------------------------------------------------------------------------------------
#include "Core/Buffer.hpp"
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#ifdef SQMOD_OS_WINDOWS
    #include <windows.h>
#endif // SQMOD_OS_WINDOWS

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Really poor design decision if a multi-threaded situation ever occurs. Don't do this. Ever!
*/
static SQChar g_NumBuf[1024];

// ------------------------------------------------------------------------------------------------
const SQChar * ConvNum< signed char >::ToStr(signed char v)
{
    // Write the numeric value to the buffer
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%d", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the beginning of the buffer
    return g_NumBuf;
}

signed char ConvNum< signed char >::FromStr(const SQChar * s)
{
    return ConvTo< signed char >::From(std::strtol(s, nullptr, 10));
}

signed char ConvNum< signed char >::FromStr(const SQChar * s, int32_t base)
{
    return ConvTo< signed char >::From(std::strtol(s, nullptr, base));
}

// ------------------------------------------------------------------------------------------------
const SQChar * ConvNum< unsigned char >::ToStr(unsigned char v)
{
    // Write the numeric value to the buffer
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%u", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the beginning of the buffer
    return g_NumBuf;
}

unsigned char ConvNum< unsigned char >::FromStr(const SQChar * s)
{
    return ConvTo< unsigned char >::From(std::strtoul(s, nullptr, 10));
}

unsigned char ConvNum< unsigned char >::FromStr(const SQChar * s, int32_t base)
{
    return ConvTo< unsigned char >::From(std::strtoul(s, nullptr, base));
}

// ------------------------------------------------------------------------------------------------
const SQChar * ConvNum< signed short >::ToStr(signed short v)
{
    // Write the numeric value to the buffer
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%d", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the beginning of the buffer
    return g_NumBuf;
}

signed short ConvNum< signed short >::FromStr(const SQChar * s)
{
    return ConvTo< signed short >::From(std::strtol(s, nullptr, 10));
}

signed short ConvNum< signed short >::FromStr(const SQChar * s, int32_t base)
{
    return ConvTo< signed short >::From(std::strtol(s, nullptr, base));
}

// ------------------------------------------------------------------------------------------------
const SQChar * ConvNum< unsigned short >::ToStr(unsigned short v)
{
    // Write the numeric value to the buffer
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%u", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the beginning of the buffer
    return g_NumBuf;
}

unsigned short ConvNum< unsigned short >::FromStr(const SQChar * s)
{
    return ConvTo< unsigned short >::From(std::strtoul(s, nullptr, 10));
}

unsigned short ConvNum< unsigned short >::FromStr(const SQChar * s, int32_t base)
{
    return ConvTo< unsigned short >::From(std::strtoul(s, nullptr, base));
}

// ------------------------------------------------------------------------------------------------
const SQChar * ConvNum< signed int >::ToStr(signed int v)
{
    // Write the numeric value to the buffer
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%d", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the beginning of the buffer
    return g_NumBuf;
}

signed int ConvNum< signed int >::FromStr(const SQChar * s)
{
    return ConvTo< signed int >::From(std::strtol(s, nullptr, 10));
}

signed int ConvNum< signed int >::FromStr(const SQChar * s, signed int base)
{
    return ConvTo< signed int >::From(std::strtol(s, nullptr, base));
}

// ------------------------------------------------------------------------------------------------
const SQChar * ConvNum< unsigned int >::ToStr(unsigned int v)
{
    // Write the numeric value to the buffer
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%u", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the beginning of the buffer
    return g_NumBuf;
}

unsigned int ConvNum< unsigned int >::FromStr(const SQChar * s)
{
    return ConvTo< unsigned int >::From(std::strtoul(s, nullptr, 10));
}

unsigned int ConvNum< unsigned int >::FromStr(const SQChar * s, signed int base)
{
    return ConvTo< unsigned int >::From(std::strtoul(s, nullptr, base));
}

// ------------------------------------------------------------------------------------------------
const SQChar * ConvNum< signed long long >::ToStr(signed long long v)
{
    // Write the numeric value to the buffer
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%lld", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the beginning of the buffer
    return g_NumBuf;
}

signed long long ConvNum< signed long long >::FromStr(const SQChar * s)
{
    return std::strtoll(s, nullptr, 10);
}

signed long long ConvNum< signed long long >::FromStr(const SQChar * s, int32_t base)
{
    return std::strtoll(s, nullptr, base);
}

// ------------------------------------------------------------------------------------------------
const SQChar * ConvNum< unsigned long long >::ToStr(unsigned long long v)
{
    // Write the numeric value to the buffer
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%llu", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the beginning of the buffer
    return g_NumBuf;
}

unsigned long long ConvNum< unsigned long long >::FromStr(const SQChar * s)
{
    return std::strtoull(s, nullptr, 10);
}

unsigned long long ConvNum< unsigned long long >::FromStr(const SQChar * s, int32_t base)
{
    return std::strtoull(s, nullptr, base);
}

// ------------------------------------------------------------------------------------------------
const SQChar * ConvNum< long >::ToStr(long v)
{
    // Write the numeric value to the buffer
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%ld", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the beginning of the buffer
    return g_NumBuf;
}

long ConvNum< long >::FromStr(const SQChar * s)
{
    return std::strtol(s, nullptr, 10);
}

long ConvNum< long >::FromStr(const SQChar * s, int32_t base)
{
    return std::strtol(s, nullptr, base);
}

// ------------------------------------------------------------------------------------------------
const SQChar * ConvNum< unsigned long >::ToStr(unsigned long v)
{
    // Write the numeric value to the buffer
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%lu", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the beginning of the buffer
    return g_NumBuf;
}

unsigned long ConvNum< unsigned long >::FromStr(const SQChar * s)
{
    return std::strtoul(s, nullptr, 10);
}

unsigned long ConvNum< unsigned long >::FromStr(const SQChar * s, int32_t base)
{
    return std::strtoul(s, nullptr, base);
}

// ------------------------------------------------------------------------------------------------
const SQChar * ConvNum< float >::ToStr(float v)
{
    // Attempt to convert the value to a string
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%f", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the data from the buffer
    return g_NumBuf;
}

float ConvNum< float >::FromStr(const SQChar * s)
{
    return std::strtof(s, nullptr);
}

float ConvNum< float >::FromStr(const SQChar * s, int32_t /*base*/)
{
    return std::strtof(s, nullptr);
}

// ------------------------------------------------------------------------------------------------
const SQChar * ConvNum< double >::ToStr(double v)
{
    // Attempt to convert the value to a string
    if (std::snprintf(g_NumBuf, sizeof(g_NumBuf), "%f", v) < 0)
    {
        g_NumBuf[0] = '\0';
    }
    // Return the data from the buffer
    return g_NumBuf;
}

double ConvNum< double >::FromStr(const SQChar * s)
{
    return std::strtod(s, nullptr);
}

double ConvNum< double >::FromStr(const SQChar * s, int32_t /*base*/)
{
    return std::strtod(s, nullptr);
}

// ------------------------------------------------------------------------------------------------
const SQChar * ConvNum< bool >::ToStr(bool v)
{
    if (v)
    {
        g_NumBuf[0] = 't';
        g_NumBuf[1] = 'r';
        g_NumBuf[2] = 'u';
        g_NumBuf[3] = 'e';
        g_NumBuf[4] = '\0';
    }
    else
    {
        g_NumBuf[0] = 'f';
        g_NumBuf[1] = 'a';
        g_NumBuf[2] = 'l';
        g_NumBuf[3] = 's';
        g_NumBuf[4] = 'e';
        g_NumBuf[5] = '\0';
    }
    return g_NumBuf;
}

bool ConvNum< bool >::FromStr(const SQChar * s)
{
    return std::strcmp(s, "true") == 0;
}

bool ConvNum< bool >::FromStr(const SQChar * s, int32_t /*base*/)
{
    return std::strcmp(s, "true") == 0;
}

// ------------------------------------------------------------------------------------------------
bool NameFilterCheck(const SQChar * filter, const SQChar * name)
{
    // If only one of them is null then they don't match
    if ((!filter && name) || (filter && !name))
    {
        return false;
    }
    // If they're both null or the filter is empty then there's nothing to check for
    else if ((!filter && !name) || (*filter == '\0'))
    {
        return true;
    }

    SQChar ch;
    // Start comparing the strings
    while (true)
    {
        // Grab the current character from filter
        ch = *(filter++);
        // See if the filter or name was completed
        if (ch == '\0' || *name == '\0')
        {
            break; // They matched so far
        }
        // Are we supposed to perform a wild-card search?
        else if (ch == '*')
        {
            // Grab the next character from filter
            ch = *(filter++);
            // Start comparing characters until the first match
            while (*name != '\0')
            {
                if (*(name++) == ch)
                {
                    break;
                }
            }
        }
        // See if the character matches doesn't have to match
        else if (ch != '?' && *name != ch)
        {
            return false; // The character had to match and failed
        }
        else
        {
            ++name;
        }
    }

    // At this point the name satisfied the filter
    return true;
}

// ------------------------------------------------------------------------------------------------
bool NameFilterCheckInsensitive(const SQChar * filter, const SQChar * name)
{
    // If only one of them is null then they don't match
    if ((!filter && name) || (filter && !name))
    {
        return false;
    }
    // If they're both null or the filter is empty then there's nothing to check for
    else if ((!filter && !name) || (*filter == '\0'))
    {
        return true;
    }

    SQChar ch;
    // Start comparing the strings
    while (true)
    {
        // Grab the current character from filter
        ch = static_cast< SQChar >(std::tolower(*(filter++)));
        // See if the filter or name was completed
        if (ch == '\0' || *name == '\0')
        {
            break; // They matched so far
        }
        // Are we supposed to perform a wild-card search?
        else if (ch == '*')
        {
            // Grab the next character from filter
            ch = static_cast< SQChar >(std::tolower(*(filter++)));
            // Start comparing characters until the first match
            while (*name != '\0')
            {
                if (static_cast< SQChar >(std::tolower(*(name++))) == ch)
                {
                    break;
                }
            }
        }
        // See if the character matches doesn't have to match
        else if (ch != '?' && static_cast< SQChar >(std::tolower(*name)) != ch)
        {
            return false; // The character had to match and failed
        }
        else
        {
            ++name;
        }
    }

    // At this point the name satisfied the filter
    return true;
}

} // Namespace:: SqMod
