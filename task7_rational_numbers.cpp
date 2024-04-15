#include <iostream>
#include <cmath>
#include <cassert>
#include <sstream>
#include <vector>

using NumberType = int64_t;

template<typename T>
T gcd(T a, T b) {
    int t;
    while (b != 0) {
        t = b;
        b = a % b;
        a = t;
    }

    return a;
}


template<typename T>
T BinaryPower(T b, uint64_t e) {
    T v = 1;
    while (e != 0) {
        if ((e & 1) != 0) {
            v *= b;
        }
        b *= b;
        e >>= 1;
    }
    return v;
}

class Rational {
public:

    Rational(NumberType m, NumberType n) : _numerator(m), _denominator(n) {
//        reduce();
    }

    // Фикс бед компиляции
    Rational(int m, int n) : _numerator(m), _denominator(n) {}

    Rational(NumberType n) : _numerator(n), _denominator(1) {}

    explicit Rational(double number, double precision) {
        _numerator = static_cast<NumberType>(number * precision);
        _denominator = static_cast<NumberType>(precision);
        reduce();
    }


    Rational() : _numerator(0), _denominator(1) {}

    friend std::ostream &operator<<(std::ostream &out, const Rational &r) {
        out << r._numerator << "/" << r._denominator;
        return out;
    }

    Rational operator-() const {
        return {-_numerator, _denominator};
    }

    // prefix increment
    Rational &operator++() {
        _numerator += _denominator;
        return *this;
    }

    // postfix increment
    Rational operator++(int) {
        Rational old = *this; // copy
        operator++();
        return old;
    }

    // prefix decrement
    Rational &operator--() {
        _numerator -= _denominator;
        return *this;
    }

    // postfix decrement
    Rational operator--(int) {
        Rational result = *this; // copy
        --(*this);
        return result;
    }

    Rational operator+(const Rational &other) const {
        return {_numerator * other._denominator + _denominator * other._numerator, _denominator * other._denominator};
    }

    Rational operator-(const Rational &other) const {
        return {_numerator * other._denominator - _denominator * other._numerator, _denominator * other._denominator};
    }

    Rational operator*(const Rational &other) const {
        return {_numerator * other._numerator, _denominator * other._denominator};
    }


    template<typename T>
    friend Rational operator*(const T factor, const Rational &other) {
        return {other._numerator * factor, other._denominator};
    }

    Rational operator/(const Rational &other) const {
        return {_numerator * other._denominator, _denominator * other._numerator};
    }

    Rational operator+=(const Rational &other) {
        _numerator = _numerator * other._denominator + _denominator * other._numerator;
        _denominator = _denominator * other._denominator;
        return *this;
    }

    Rational operator-=(const Rational &other) {
        _numerator = _numerator * other._denominator - _denominator * other._numerator;
        _denominator = _denominator * other._denominator;
        return *this;
    }

    Rational operator*=(const Rational &other) {
        _numerator = _numerator * other._numerator;
        _denominator = _denominator * other._denominator;
        return *this;
    }


    Rational operator/=(const Rational &other) {

        if (this == &other) {
            _numerator = 1;
            _denominator = 1;
            return *this;
        }

        _numerator = _numerator * other._denominator;
        _denominator = _denominator * other._numerator;
        return *this;
    }


    friend Rational intPow(Rational r, unsigned long long power) {
        return {
                static_cast<NumberType>(BinaryPower(r._numerator, power)),
                static_cast<NumberType>(BinaryPower(r._denominator, power)),
        };
    }

    friend Rational pow(Rational r, double power) {
        return {static_cast<NumberType>(pow(r._numerator, power)),
                static_cast<NumberType>( pow(r._denominator, power))};
    }


    explicit operator double() const {
        return static_cast<double >(_numerator) / static_cast<double >(_denominator);
    }


    [[nodiscard]] NumberType numerator() const {
        return _numerator;
    }

    [[nodiscard]] NumberType denominator() const {
        return _denominator;
    }


    /// Приводит к несократимому виду
    void reduce() {
        NumberType t = gcd(_numerator, _denominator);
        _numerator /= t;
        _denominator /= t;


        if (_denominator < static_cast<NumberType>(0)) {
            _denominator = -_denominator;
            _numerator = -_numerator;
        }
    }

private:
    NumberType _numerator, _denominator;
};

void test_rational_number() {
    Rational r, q, p;


    // RationalNumber(int m, int n)
    r = {1, 2};
    assert(r.numerator() == 1);
    assert(r.denominator() == 2);
    r = {-32423423, 234322};
    assert(r.numerator() == -32423423);
    assert(r.denominator() == 234322);


    // explicit RationalNumber(int m)
    r = Rational(1);
    assert(r.numerator() == 1);
    assert(r.denominator() == 1);
    r = Rational(23423423);
    assert(r.numerator() == 23423423);
    assert(r.denominator() == 1);


    // explicit RationalNumber(double number, double precision)
    r = Rational(1.0, 10.0);
    assert(r.numerator() == 1);
    assert(r.denominator() == 1);
    r = Rational(1.234824, 1e7);
    assert(r.numerator() == 1234824 / 8);
    assert(r.denominator() == 125000);


    // RationalNumber()
    r = Rational();
    assert(r.numerator() == 0);
    assert(r.denominator() == 1);


    // &operator<<
    std::stringstream s;
    r = {3243, 23};
    s << r;
    std::string buf;
    s >> buf;
    assert(buf == "3243/23");


    // operator-
    r = {3243, 23};
    q = -r;
    assert(q.numerator() == -r.numerator());
    assert(q.denominator() == r.denominator());


    // &operator++
    r = {123, 23};
    q = ++r;
    assert(r.numerator() == 146);
    assert(r.denominator() == 23);
    assert(q.numerator() == 146);
    assert(q.denominator() == 23);


    // &operator--
    r = {123, 23};
    q = --r;
    assert(r.numerator() == 100);
    assert(r.denominator() == 23);
    assert(q.numerator() == 100);
    assert(q.denominator() == 23);

    // operator++
    r = {123, 23};
    q = r++;
    assert(r.numerator() == 146);
    assert(r.denominator() == 23);
    assert(q.numerator() == 123);
    assert(q.denominator() == 23);


    // operator--
    r = {123, 23};
    q = r--;
    assert(r.numerator() == 100);
    assert(r.denominator() == 23);
    assert(q.numerator() == 123);
    assert(q.denominator() == 23);


    // operator+
    r = {123, 23};
    q = {34, 40};
    p = r + q;
    assert(p.numerator() == 123 * 40 + 23 * 34);
    assert(p.denominator() == 23 * 40);


    // operator-
    r = {123, 23};
    q = {34, 40};
    p = r - q;
    assert(p.numerator() == 123 * 40 - 23 * 34);
    assert(p.denominator() == 23 * 40);


    // operator*
    r = {123, 23};
    q = {34, 40};
    p = r * q;
    assert(p.numerator() == 123 * 34);
    assert(p.denominator() == 23 * 40);


    // operator/
    r = {123, 23};
    q = {34, 40};
    p = r / q;
    assert(p.numerator() == 123 * 40);
    assert(p.denominator() == 23 * 34);


    // operator+=
    r = {123, 23};
    q = {34, 40};
    r += q;
    assert(r.numerator() == 123 * 40 + 23 * 34);
    assert(r.denominator() == 23 * 40);


    // operator-=
    r = {123, 23};
    q = {34, 40};
    r -= q;
    assert(r.numerator() == 123 * 40 - 23 * 34);
    assert(r.denominator() == 23 * 40);


    // operator*
    r = {123, 23};
    q = {34, 40};
    r *= q;
    assert(r.numerator() == 123 * 34);
    assert(r.denominator() == 23 * 40);


    // operator/
    r = {123, 23};
    q = {34, 40};
    r /= q;
    assert(r.numerator() == 123 * 40);
    assert(r.denominator() == 23 * 34);

    // pow
    r = {2, 3};
    q = pow(r, 3);
    assert(q.numerator() == 8);
    assert(q.denominator() == 27);

    // reduction
    r = {14, 21};
    r.reduce();
    assert(r.numerator() == 2);
    assert(r.denominator() == 3);
    r = {5 * 7 * 763478, 9 * 11 * 763478};
    r.reduce();
    assert(r.numerator() == 5 * 7);
    assert(r.denominator() == 9 * 11);

    // double
    assert(fabs(static_cast<double >(Rational(1, 2)) - 0.5) < 1e-10);
    assert(fabs(static_cast<double >(Rational(76, 34)) - 76.0 / 34) < 1e-10);
    assert(fabs(static_cast<double >(Rational(-2734378, 92374)) - -2734378.0 / 92374) < 1e-10);

    r = {1, 2};
    r /= r;


    std::cout << Rational(0, 1) << std::endl;
    Rational res;
    for (int i = 2; i < 20; ++i) {
        res += Rational(1, BinaryPower(2, i));
        res.reduce();
        std::cout << i << " " << res << std::endl;
    }
    std::cout << res;
}

template<typename T>
void printVector(const std::vector<T> &vec, const std::string &sep = ", ", const std::string &term = "\n") {

    std::cout << "{";

    size_t len = vec.size();

    if (len == 0) {
        std::cout << "}" << term;
        return;
    }

    std::cout << vec[0];

    for (size_t i = 1; i < len; ++i) {
        std::cout << sep << vec[i];
    }

    std::cout << "}" << term;
}


/// Возвращает первых n чисел Бернулли, начиная с первого
std::vector<Rational> getBernoulliNumbers(const int n) {
    std::vector<Rational> numbers(n);
    std::vector<Rational> buffer(n + 1);
    buffer[0] = 1;// Делаем нулевую итерация цикла

    for (int i = 1; i <= n; ++i) {
        buffer[i] = {1, i + 1};
        for (int j = i; j > 0; --j) {
            buffer[j - 1] = j * (buffer[j - 1] - buffer[j]);
            buffer[j - 1].reduce();
        }

        numbers[i - 1] = buffer[0];
    }

    return numbers;
}


int main() {
    test_rational_number();

    int max_k = 8;

//    for (int k = 1; k <= max_k; ++k) {
//        std::cout << "k = " << k << ": ";
//        auto BernoulliNumbers = getBernoulliNumbers(k);
//        printVector(BernoulliNumbers);
//    }


    auto BernoulliNumbers = getBernoulliNumbers(2 * max_k);
    for (int k = 1; k <= max_k; ++k) {
        std::cout << "B_" << 2 * k << " = " << BernoulliNumbers[2 * k - 1] << std::endl;
    }
}