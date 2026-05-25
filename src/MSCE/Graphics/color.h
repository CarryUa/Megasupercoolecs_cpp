#ifndef MSCE_COLOR_H_
#define MSCE_COLOR_H_
#include <string>

#include <MSCE/Types/vector.h>

namespace msce
{
    /**
     * @brief Class representation of color.
     */
    struct Color
    {
    private:
        /// @brief Individual channel byte.
        unsigned char r_, g_, b_, a_;

    public:
        /// @brief Constructs the color from hex code.
        /// @param hex Try and guess -_-
        Color(const std::string &hex, unsigned char a = 255);

        /// @brief Constructs the color from rgb vector and alpha variable.
        /// @param vec The rgb vector.
        /// @param a The alpha variable.
        Color(const msce::Vector3D<unsigned char> vec, unsigned char a = 255) noexcept;

        /// @brief Constructs the color from rgb vector and alpha variable.
        /// @param vec The rgb vector.
        /// @param a The alpha variable.
        Color(const Color &other) noexcept;

        /// @brief Getter for R channel
        /// @return R channel value
        unsigned char r() const noexcept;
        /// @brief Setter for R channel
        /// @param value New R channel value
        void r(unsigned char value) noexcept;

        /// @brief Getter for G channel
        /// @return G channel value
        unsigned char g() const noexcept;
        /// @brief Setter for G channel
        /// @param value New G channel value
        void g(unsigned char value) noexcept;

        /// @brief Getter for B channel
        /// @return B channel value
        unsigned char b() const noexcept;
        /// @brief Setter for B channel
        /// @param value New B channel value
        void b(unsigned char value) noexcept;

        /// @brief Getter for A channel
        /// @return A channel value
        unsigned char a() const noexcept;
        /// @brief Setter for A channel
        /// @param value New A channel value
        void a(unsigned char value) noexcept;

        /// @return Hex-code representing this color. (Without prepending '#')
        std::string hex() const noexcept;
        /// @brief Sets this color to given hex-code representation.
        /// @param value The hex representation.
        void hex(const std::string &value) noexcept;

        /// @brief Compares this color to hex provided.
        bool operator==(const std::string &hex) const noexcept;
        /// @brief Compares this color to rgb vector provided.
        bool operator==(const Vector3D<unsigned char> vec) const noexcept;
        /// @brief Compares this color to another color.
        bool operator==(const Color &other) const noexcept;
    };
}

#endif