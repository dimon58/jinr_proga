#include <iostream>
#include <cmath>
#include <cassert>
#include <sstream>

int gcd(int a, int b) {
    int t;
    while (b != 0) {
        t = b;
        b = a % b;
        a = t;
    }

    return a;
}


class Rational {
public:
    Rational(int m, int n) : _numenator(m), _denomenator(n) {}

    explicit Rational(int m) : _numenator(m), _denomenator(1) {}

    explicit Rational(double number, double precision) {
        _numenator = static_cast<int>(number * precision);
        _denomenator = static_cast<int>(precision);
        reduction();
    }

    Rational() : _numenator(0), _denomenator(1) {}

    friend std::ostream &operator<<(std::ostream &out, const Rational &r) {
        out << r._numenator << "/" << r._denomenator;
        return out;
    }

    Rational operator-() const {
        return {-_numenator, _denomenator};
    }

    Rational &operator++() {
        _numenator += _denomenator;
        return *this;
    }

    Rational &operator--() {
        _numenator -= _denomenator;
        return *this;
    }

    Rational operator++(int) {
        Rational result = *this;
        ++(*this);
        return result;
    }

    Rational operator--(int) {
        Rational result = *this;
        --(*this);
        return result;
    }

    Rational operator+(Rational &other) const {
        return {_numenator * other._denomenator + _denomenator * other._numenator, _denomenator * other._denomenator};
    }

    Rational operator-(Rational &other) const {
        return {_numenator * other._denomenator - _denomenator * other._numenator, _denomenator * other._denomenator};
    }

    Rational operator*(Rational &other) const {
        return {_numenator * other._numenator, _denomenator * other._denomenator};
    }

    Rational operator/(Rational &other) const {
        return {_numenator * other._denomenator, _denomenator * other._numenator};
    }

    Rational operator+=(Rational &other) {
        _numenator = _numenator * other._denomenator + _denomenator * other._numenator;
        _denomenator = _denomenator * other._denomenator;
        return *this;
    }

    Rational operator-=(Rational &other) {
        _numenator = _numenator * other._denomenator - _denomenator * other._numenator;
        _denomenator = _denomenator * other._denomenator;
        return *this;
    }

    Rational operator*=(Rational &other) {
        _numenator = _numenator * other._numenator;
        _denomenator = _denomenator * other._denomenator;
        return *this;
    }

    Rational operator/=(Rational &other) {
        _numenator = _numenator * other._denomenator;
        _denomenator = _denomenator * other._numenator;
        return *this;
    }

    friend Rational pow(Rational r, double power) {
        return {static_cast<int>(pow(r._numenator, power)), static_cast<int>( pow(r._denomenator, power))};
    }

    void reduction() {
        int t = gcd(_numenator, _denomenator);
        _numenator /= t;
        _denomenator /= t;
    }

    explicit operator double() const {
        return static_cast<double >(_numenator) / static_cast<double >(_denomenator);
    }

    [[nodiscard]] int numenator() const {
        return _numenator;
    }

    [[nodiscard]] int denomenator() const {
        return _denomenator;
    }

private:
    int _numenator, _denomenator;
};


void test_rational_number() {
    Rational r, q, p;


    // RationalNumber(int m, int n)
    r = {1, 2};
    assert(r.numenator() == 1);
    assert(r.denomenator() == 2);
    r = {-32423423, 234322};
    assert(r.numenator() == -32423423);
    assert(r.denomenator() == 234322);


    // explicit RationalNumber(int m)
    r = Rational(1);
    assert(r.numenator() == 1);
    assert(r.denomenator() == 1);
    r = Rational(23423423);
    assert(r.numenator() == 23423423);
    assert(r.denomenator() == 1);


    // explicit RationalNumber(double number, double precision)
    r = Rational(1.0, 10.0);
    assert(r.numenator() == 1);
    assert(r.denomenator() == 1);
    r = Rational(1.234824, 1e7);
    assert(r.numenator() == 1234824 / 8);
    assert(r.denomenator() == 125000);


    // RationalNumber()
    r = Rational();
    assert(r.numenator() == 0);
    assert(r.denomenator() == 1);


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
    assert(q.numenator() == -r.numenator());
    assert(q.denomenator() == r.denomenator());


    // &operator++
    r = {123, 23};
    q = ++r;
    assert(r.numenator() == 146);
    assert(r.denomenator() == 23);
    assert(q.numenator() == 146);
    assert(q.denomenator() == 23);


    // &operator--
    r = {123, 23};
    q = --r;
    assert(r.numenator() == 100);
    assert(r.denomenator() == 23);
    assert(q.numenator() == 100);
    assert(q.denomenator() == 23);

    // operator++
    r = {123, 23};
    q = r++;
    assert(r.numenator() == 146);
    assert(r.denomenator() == 23);
    assert(q.numenator() == 123);
    assert(q.denomenator() == 23);


    // operator--
    r = {123, 23};
    q = r--;
    assert(r.numenator() == 100);
    assert(r.denomenator() == 23);
    assert(q.numenator() == 123);
    assert(q.denomenator() == 23);


    // operator+
    r = {123, 23};
    q = {34, 40};
    p = r + q;
    assert(p.numenator() == 123 * 40 + 23 * 34);
    assert(p.denomenator() == 23 * 40);


    // operator-
    r = {123, 23};
    q = {34, 40};
    p = r - q;
    assert(p.numenator() == 123 * 40 - 23 * 34);
    assert(p.denomenator() == 23 * 40);


    // operator*
    r = {123, 23};
    q = {34, 40};
    p = r * q;
    assert(p.numenator() == 123 * 34);
    assert(p.denomenator() == 23 * 40);


    // operator/
    r = {123, 23};
    q = {34, 40};
    p = r / q;
    assert(p.numenator() == 123 * 40);
    assert(p.denomenator() == 23 * 34);


    // operator+=
    r = {123, 23};
    q = {34, 40};
    r += q;
    assert(r.numenator() == 123 * 40 + 23 * 34);
    assert(r.denomenator() == 23 * 40);


    // operator-=
    r = {123, 23};
    q = {34, 40};
    r -= q;
    assert(r.numenator() == 123 * 40 - 23 * 34);
    assert(r.denomenator() == 23 * 40);


    // operator*
    r = {123, 23};
    q = {34, 40};
    r *= q;
    assert(r.numenator() == 123 * 34);
    assert(r.denomenator() == 23 * 40);


    // operator/
    r = {123, 23};
    q = {34, 40};
    r /= q;
    assert(r.numenator() == 123 * 40);
    assert(r.denomenator() == 23 * 34);

    // pow
    r = {2, 3};
    q = pow(r, 3);
    assert(q.numenator() == 8);
    assert(q.denomenator() == 27);

    // reduction
    r = {14, 21};
    r.reduction();
    assert(r.numenator() == 2);
    assert(r.denomenator() == 3);
    r = {5 * 7 * 763478, 9 * 11 * 763478};
    r.reduction();
    assert(r.numenator() == 5 * 7);
    assert(r.denomenator() == 9 * 11);

    // double
    assert(fabs(static_cast<double >(Rational(1, 2)) - 0.5) < 1e-10);
    assert(fabs(static_cast<double >(Rational(76, 34)) - 76.0 / 34) < 1e-10);
    assert(fabs(static_cast<double >(Rational(-2734378, 92374)) - -2734378.0 / 92374) < 1e-10);

}

int main() {
    test_rational_number();
}