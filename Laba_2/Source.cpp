#include <iostream>
#include "BigInteger.h"

int main() {
    BigInteger x("123456789012345678901234567890");
    BigInteger y("987654321098765432109876543210");
    BigInteger a;
    std::cout << "x = " << x << std::endl;
    std::cout << "y = " << y << std::endl;
    a = -x;
    std::cout << "-x = " << a << std::endl;
    a = x + y;
    std::cout << "x + y = " << a << std::endl;
    a = x - y;
    std::cout << "x - y = " << a << std::endl;
    a = x * y;
    std::cout << "x * y = " << a << std::endl;
    a = x / y;
    std::cout << "x / y = " << a << std::endl;
    a = x % y;
    std::cout << "x % y = " << a << std::endl;

    if (x == y) {
        std::cout << "x == y" << std::endl;
    }
    if (x != y) {
        std::cout << "x != y" << std::endl;
    }
    if (x < y) {
        std::cout << "x < y" << std::endl;
    }
    if (x > y) {
        std::cout << "x > y" << std::endl;
    }
    if (x <= y) {
        std::cout << "x <= y" << std::endl;
    }
    if (x >= y) {
        std::cout << "x >= y" << std::endl;
    }

    a = ++x;
    std::cout << "++x = " << a << std::endl;
    a = x++;
    std::cout << "x++ = " << a << std::endl;
    a = --x;
    std::cout << "--x = " << a << std::endl;
    a = x--;
    std::cout << "x-- = " << a << std::endl;

    std::cout << "x += y, x = " << (x += y) << std::endl;
    std::cout << "x -= y, x = " << (x -= y) << std::endl;
    std::cout << "x *= y, x = " << (x *= y) << std::endl;
    /*std::cout << "x /= y, x = " << (x /= y) << std::endl;
    std::cout << "x %= y, x = " << (x %= y) << std::endl;*/


    return 0;
}