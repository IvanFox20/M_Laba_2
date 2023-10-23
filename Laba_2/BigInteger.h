#include <iostream>
#include <vector>
#include <string>

class BigInteger {
private:
    std::vector<int> digits;
    bool isNegative;

public:
    BigInteger() : digits(), isNegative(false) {}

    // // Конструктор, принимающий int
    BigInteger(int n) {
        isNegative = n < 0; // определяем знак числа
        if (isNegative) {
            n = -n; // если число отрицательное, меняем его на положительное
        }
        while (n > 0) { // разбиваем число на цифры и добавляем их в вектор
            digits.push_back(n % 10);
            n /= 10;
        }
    }

    // Конструктор, принимающий строку
    BigInteger(const std::string& str) {
        digits.clear();
        isNegative = false;
        if (str.length() > 0 && (str[0] == '+' || str[0] == '-')) { // если первый символ строки - знак числа
            isNegative = (str[0] == '-'); // определяем знак числа
            for (int i = str.length() - 1; i > 0; i--) { // разбиваем число на цифры и добавляем их в вектор
                digits.push_back(str[i] - '0');
            }
        }
        else {
            for (int i = str.length() - 1; i >= 0; i--) { // разбиваем число на цифры и добавляем их в вектор
                digits.push_back(str[i] - '0');
            }
        }
        removeLeadingZeros(digits); // удаляем ведущие нули из числа
    }

    // Перегрузка оператора сложения. 
    // Если числа имеют разные знаки, вызывается оператор вычитания.
    // Иначе происходит сложение цифр чисел с учетом переносов.
    BigInteger operator+(const BigInteger& other) const {
        if (isNegative != other.isNegative) { // если числа имеют разные знаки, вызываем оператор вычитания
            return *this - (-other);
        }

        BigInteger result;
        result.isNegative = isNegative;

        int carry = 0;
        int i = 0;
        while (i < digits.size() || i < other.digits.size() || carry != 0) {
            int sum = carry;
            if (i < digits.size()) {
                sum += digits[i];
            }
            if (i < other.digits.size()) {
                sum += other.digits[i];
            }
            result.digits.push_back(sum % 10);
            carry = sum / 10;
            i++;
        }

        return result;
    }

    // Перегрузка оператора вычитания.
    // Если числа имеют разные знаки, вызывается оператор сложения с отрицательным числом.
    // Иначе происходит вычитание цифр чисел с учетом заемов.
    BigInteger operator-(const BigInteger& other) const {
        if (isNegative != other.isNegative) { // если числа имеют разные знаки, вызываем оператор сложения с отрицательным числом
            return *this + (-other);
        }

        if (isNegative && other.isNegative) { // если оба числа отрицательные, меняем знаки и вызываем оператор вычитания
            return (-other) - (-*this);
        }

        if (*this < other) { // если уменьшаемое меньше вычитаемого, меняем знаки и вызываем оператор вычитания
            return -(other - *this);
        }

        BigInteger result;
        result.digits.clear();

        result.isNegative = false;

        int borrow = 0;
        int i = 0;
        while (i < digits.size() || i < other.digits.size() || borrow != 0) {
            int diff = borrow;
            if (i < digits.size()) {
                diff += digits[i];
            }
            if (i < other.digits.size()) {
                diff -= other.digits[i];
            }

            if (diff < 0) {
                diff += 10;
                borrow = -1;
            }
            else {
                borrow = 0;
            }

            result.digits.push_back(diff);
            i++;
        }

        removeLeadingZeros(result.digits);

        return result;
    }

    // Перегрузка оператора умножения. 
    // Происходит умножение цифр чисел с учетом переносов.
    // Определяется знак результата в зависимости от знаков множителей.
    BigInteger operator*(const BigInteger& other) const {
        BigInteger result;
        result.digits.resize(digits.size() + other.digits.size());

        for (int i = 0; i < digits.size(); i++) {
            int carry = 0;
            for (int j = 0; j < other.digits.size() || carry != 0; j++) {
                int tmp = result.digits[i + j] + digits[i] * (j < other.digits.size() ? other.digits[j] : 0) + carry;
                result.digits[i + j] = tmp % 10;
                carry = tmp / 10;
            }
        }

        removeLeadingZeros(result.digits);

        result.isNegative = isNegative != other.isNegative;

        return result;
    }

    // Перегрузка оператора деления. 
    // Если делитель равен 0, выбрасывается исключение.
    // Происходит деление цифр чисел с учетом заемов.
    // Определяется знак результата в зависимости от знаков делимого и делителя.
    BigInteger operator/(const BigInteger& other) const {
        if (other == 0) { // если делитель равен 0, выбрасываем исключение
            throw std::invalid_argument("Division by zero");
        }

        BigInteger quotient;
        quotient.digits.resize(digits.size());

        BigInteger remainder;
        remainder.digits = digits;

        for (int i = digits.size() - 1; i >= 0; i--) {
            remainder = remainder * 10 + digits[i];
            int q = 0;
            int l = 0;
            int r = 10;
            while (l <= r) {
                int m = (l + r) / 2;
                if (other * m <= remainder) {
                    q = m;
                    l = m + 1;
                }
                else {
                    r = m - 1;
                }
            }

            quotient.digits[i] = q;
            remainder = remainder - other * q;
        }

        removeLeadingZeros(quotient.digits);

        quotient.isNegative = isNegative != other.isNegative;

        return quotient;
    }

    // Перегрузка оператора остатка от деления.
    // Если делитель равен 0, выбрасывается исключение.
    // Если уменьшаемое меньше вычитаемого, возвращается уменьшаемое.
    BigInteger operator%(const BigInteger& other) const {
        if (other == 0) { // если делитель равен 0, выбрасываем исключение
            throw std::invalid_argument("Division by zero");
        }

        if (*this < other) { // если уменьшаемое меньше вычитаемого, возвращаем уменьшаемое
            return *this;
        }

        BigInteger quotient = *this / other;
        BigInteger remainder = *this - other * quotient;
        return remainder;
    }

    bool operator==(const BigInteger& other) const {
        return digits == other.digits && isNegative == other.isNegative;
    }

    bool operator!=(const BigInteger& other) const {
        return !(*this == other);
    }

    // Сравнение происходит с учетом знака и количества цифр в числах.
    bool operator<(const BigInteger& other) const {
        // Если знаки чисел разные, то отрицательное число меньше положительного.
        if (isNegative != other.isNegative) {
            return isNegative;
        }

        // Если количество цифр в числах разное, то меньшее число имеет меньшее количество цифр.
        if (digits.size() != other.digits.size()) {
            return (digits.size() < other.digits.size()) ^ isNegative;
        }

        // Сравниваем цифры чисел от старших разрядов к младшим.
        for (int i = digits.size() - 1; i >= 0; i--) {
            if (digits[i] != other.digits[i]) {
                return (digits[i] < other.digits[i]) ^ isNegative;
            }
        }

        // Если все цифры равны, то числа равны и возвращаем false.
        return false;
    }

    bool operator>(const BigInteger& other) const {
        return other < *this;
    }

    bool operator<=(const BigInteger& other) const {
        return !(other < *this);
    }

    bool operator>=(const BigInteger& other) const {
        return !(*this < other);
    }

    BigInteger& operator++() {
        *this = *this + 1;
        return *this;
    }

    BigInteger operator++(int) {
        BigInteger tmp(*this);
        operator++();
        return tmp;
    }

    BigInteger& operator--() {
        *this = *this - 1;
        return *this;
    }

    BigInteger operator--(int) {
        BigInteger tmp(*this);
        operator--();
        return tmp;
    }

    BigInteger& operator+=(const BigInteger& other) {
        *this = *this + other;
        return *this;
    }

    BigInteger& operator-=(const BigInteger& other) {
        *this = *this - other;
        return *this;
    }

    BigInteger& operator*=(const BigInteger& other) {
        *this = *this * other;
        return *this;
    }

    /* BigInteger& operator/=(const BigInteger& other) {
         *this = *this / other;
         return *this;
     }*/

    BigInteger& operator%=(const BigInteger& other) {
        *this = *this % other;
        return *this;
    }


    //BigInteger& operator%=(const BigInteger& other) {
    //    if (other == 0) {
    //        throw std::invalid_argument("Division by zero");
    //    }

    //    // ≈сли текущее число меньше делител€, то остаток равен текущему числу
    //    if (*this < other) {
    //        return *this;
    //    }

    //    BigInteger quotient = *this / other;
    //    *this -= other * quotient;

    //    // ”дал€ем ведущие нули из числа
    //    while (digits.size() > 1 && digits.back() == 0) {
    //        digits.pop_back();
    //    }

    //    return *this;
    //}

    // ќператоры унарного минуса и приведени€ к bool
    BigInteger operator-() const {
        BigInteger result(*this);
        result.isNegative = !isNegative;
        return result;
    }

    explicit operator bool() const {
        return digits.size() > 0;
    }

    // ћетоды преобразовани€ к строке и вывода в поток
    std::string toString() const {
        std::string result;
        // Если число равно нулю, то возвращается строка "0".
        if (digits.size() == 0) {
            return "0";
        }
        // Если число отрицательное, то перед числом добавляется знак "-".
        if (isNegative) {
            result.push_back('-');
        }
        // Цифры числа добавляются в строку от старших разрядов к младшим.
        for (int i = digits.size() - 1; i >= 0; i--) {
            result.push_back(digits[i] + '0');
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const BigInteger& obj) {
        os << obj.toString();
        return os;
    }

    friend std::istream& operator>>(std::istream& is, BigInteger& obj) {
        std::string str;
        is >> str;
        obj = BigInteger(str);
        return is;
    }

private:
    // ¬спомогательный метод дл€ удалени€ ведущих нулей
    void removeLeadingZeros(std::vector<int>& digits) const {
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
    }
};
