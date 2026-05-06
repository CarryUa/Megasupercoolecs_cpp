#include "color.h"
#include <MSCE/logger.h>
#include <charconv>
#include <array>

/// @brief Validates hex if possible. Lowers all characters.
/// @param hex
/// @return Index of first data character(first after '#'), or -1 if invalid.
static int validate_hex(std::string &hex) noexcept
{
    constexpr std::array<bool, 256> allowed_chars = []()
    {
        std::array<bool, 256> arr{};
        for (char c : {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', '#'})
        {
            arr[static_cast<unsigned char>(c)] = true;
        }
        return arr;
    }();

    if (hex.size() < 6 ||
        hex.size() > 7)
        return -1;

    unsigned char start_position = 0;

    if (hex[0] == '#' && hex.size() == 7)
        start_position = 1;

    for (unsigned char i = 0; i < hex.size(); i++)
    {
        if (!std::islower(hex[i]))
            hex[i] = std::tolower(hex[i]);

        if (!allowed_chars[hex[i]])
            return -1;
    }

    return start_position;
}

/// @brief Parses the provided hex and returns rgb Vector3D.
/// @param hex_copy Hex string.
/// @return The parsed vector. If porvided hex is invalid, returns Vector3D [-1, -1, -1]
static msce::Vector3D<int> parse_hex(std::string hex_copy) noexcept
{
    int start_pos = validate_hex(hex_copy);
    if (start_pos == -1)
        return msce::Vector3D<int>(-1, -1, -1);

    const unsigned char r = std::stoi(hex_copy.substr(start_pos, 2), nullptr, 16);
    const unsigned char g = std::stoi(hex_copy.substr(start_pos + 2, 2), nullptr, 16);
    const unsigned char b = std::stoi(hex_copy.substr(start_pos + 4, 2), nullptr, 16);

    return msce::Vector3D<int>(static_cast<int>(r),
                               static_cast<int>(g),
                               static_cast<int>(b));
}

msce::Color::Color(const std::string &hex, unsigned char a)
{
    auto rgb = parse_hex(hex);
    this->r_ = rgb.x;
    this->g_ = rgb.y;
    this->b_ = rgb.z;
    this->a_ = a;
}

msce::Color::Color(const msce::Vector3D<unsigned char> vec, unsigned char a) noexcept
{
    this->r_ = vec.x;
    this->g_ = vec.y;
    this->b_ = vec.z;
    this->a_ = a;
}

msce::Color::Color(const Color &other) noexcept
{
    this->r_ = other.r_;
    this->g_ = other.g_;
    this->b_ = other.b_;
    this->a_ = other.a_;
}

unsigned char msce::Color::r() const noexcept
{
    return this->r_;
}

void msce::Color::r(unsigned char value) noexcept
{
    this->r_ = value;
}

unsigned char msce::Color::g() const noexcept
{
    return this->g_;
}

void msce::Color::g(unsigned char value) noexcept
{
    this->g_ = value;
}

unsigned char msce::Color::b() const noexcept
{
    return this->b_;
}

void msce::Color::b(unsigned char value) noexcept
{
    this->b_ = value;
}
unsigned char msce::Color::a() const noexcept
{
    return this->a_;
}

void msce::Color::a(unsigned char value) noexcept
{
    this->a_ = value;
}

std::string msce::Color::hex() const noexcept
{
    return std::format("{:02x}{:02x}{:02x}", r_, g_, b_);
}

void msce::Color::hex(const std::string &value) noexcept
{
    auto rgb = parse_hex(value);
    if (rgb.x == -1)
        return;

    this->r_ = rgb.x;
    this->g_ = rgb.y;
    this->b_ = rgb.z;
}

bool msce::Color::operator==(const std::string &hex) const noexcept
{
    auto rgb = parse_hex(hex);
    if (rgb.x == -1)
        return false;

    return this->r_ == rgb.x &&
           this->g_ == rgb.y &&
           this->b_ == rgb.z;
}

bool msce::Color::operator==(const Vector3D<unsigned char> vec) const noexcept
{
    return this->r_ == vec.x &&
           this->g_ == vec.y &&
           this->b_ == vec.z;
}

bool msce::Color::operator==(const Color &other) const noexcept
{
    return this->r_ == other.r_ &&
           this->g_ == other.g_ &&
           this->b_ == other.b_;
}
