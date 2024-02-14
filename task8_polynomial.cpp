#include <utility>
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>


inline double plus(const double lhs, const double rhs) {
    return lhs + rhs;
}

inline double minus(const double lhs, const double rhs) {
    return lhs - rhs;
}


class Polynomial {
public:

    Polynomial() : _koefs({0.0}) {};

    explicit Polynomial(const std::vector<double> &vec) : _koefs(vec) {}

    friend std::ostream &operator<<(std::ostream &out, const Polynomial &poly) {
        for (size_t i = poly._koefs.size() - 1; i > 0; --i) {
            out << poly._koefs[i] << "x^" << i << " + ";
        }

        out << poly._koefs[0];


        return out;
    }

    double operator()(const double x) const {
        double _x = 1;
        double res = 0.0;

        for (double koef: _koefs) {
            res += koef * _x;
            _x *= x;
        }

        return res;
    }

    static std::vector<double>
    apply_addictive(double(*operation)(double, double), const std::vector<double> &lhs,
                    const std::vector<double> &rhs) {
        size_t lhs_size = lhs.size();
        size_t rhs_size = rhs.size();


        std::vector<double> result;
        size_t common;

        if (lhs_size >= rhs_size) {
            common = rhs_size;
            result.resize(lhs_size);
            std::copy(
                    lhs.begin() + common,  // NOLINT(*-narrowing-conversions)
                    lhs.end(),
                    result.begin() + common  // NOLINT(*-narrowing-conversions)
            );

        } else {
            common = lhs_size;
            result.resize(rhs_size);
            std::copy(
                    rhs.begin() + common,  // NOLINT(*-narrowing-conversions)
                    rhs.end(),
                    result.begin() + common  // NOLINT(*-narrowing-conversions)
            );
        }

        for (size_t i = 0; i < common; ++i) {
            result[i] = operation(lhs[i], rhs[i]);
        }

//        _reduce_vector(result);

        return result;
    }

    Polynomial operator+(const Polynomial &other) const {
        return Polynomial(apply_addictive(plus, this->_koefs, other._koefs));
    }

    Polynomial operator-(const Polynomial &other) const {
        return Polynomial(apply_addictive(minus, this->_koefs, other._koefs));
    }

    Polynomial operator+=(const Polynomial &other) {
        _koefs = apply_addictive(plus, this->_koefs, other._koefs);
        return *this;
    }

    Polynomial operator-=(const Polynomial &other) {
        _koefs = apply_addictive(minus, this->_koefs, other._koefs);
        return *this;
    }

    Polynomial operator-() const {
        std::vector<double> result(_koefs.size());
        for (size_t i = 0; i < _koefs.size(); ++i) {
            result[i] = -_koefs[i];
        }
        return Polynomial(result);
    }

    Polynomial operator*(const Polynomial &other) const {
        std::vector<double> result(_koefs.size() + other._koefs.size() - 1);

        for (size_t i = 0; i < _koefs.size(); ++i) {
            for (size_t j = 0; j < other._koefs.size(); ++j) {
                result[i + j] += _koefs[i] * other._koefs[j];
            }
        }


        Polynomial poly = Polynomial(result);
        poly.reduce(); // при умножении на 0 может быть такое
        return poly;
    }

    Polynomial operator*=(const Polynomial &other) const {
        return *this * other;
    }

    Polynomial multiply_monom(const unsigned int degree) const {
        std::vector<double> result(_koefs.size() + degree);
        for (size_t i = 0; i < _koefs.size(); ++i) {
            result[i + degree] = _koefs[i];
        }

        return Polynomial(result);
    }

    bool operator==(const Polynomial &other) const {
        if (_koefs.size() != other._koefs.size()) return false;

        for (size_t i = 0; i < _koefs.size(); ++i) {
            if (_koefs[i] != other._koefs[i]) return false;
        }

        return true;
    }

    Polynomial derivative() const {
        if (_koefs.size() == 1) return Polynomial({0});

        std::vector<double> result(_koefs.size() - 1);

        for (size_t i = 1; i < _koefs.size(); ++i) {
            result[i - 1] = static_cast<double>(i) * _koefs[i];
        }

        return Polynomial(result);
    }

    static void _reduce_vector(std::vector<double> &vec) {
        size_t new_size = vec.size();
        for (size_t i = new_size - 1; i > 0; --i) { // обязательно оставляем нулевой коэффициент
            if (vec[i] != 0.0) {
                new_size = i + 1;
                break;
            }
        }

        vec.resize(new_size);
    }

    /// Отбрасывает все старшие степени с нулями
    void reduce() {
        _reduce_vector(_koefs);
    }

    [[nodiscard]] size_t getDegree() const {
        return _koefs.size() - 1;
    }

    /// Возвращает результат деления столбиком на divisor в виде (результат, остаток)
    [[nodiscard]] std::pair<Polynomial, Polynomial> divide(const Polynomial &divisor) const {
        size_t self_degree = getDegree(), divider_degree = divisor.getDegree();

        if (divider_degree == 0) {
            std::vector<double> quotient(_koefs.size());
            double k = divisor._koefs[0];
            for (size_t i = 0; i < _koefs.size(); ++i) quotient[i] = _koefs[i] / k;
            return std::make_pair(Polynomial(quotient), Polynomial());
        }

        if (self_degree < divider_degree) return std::make_pair(Polynomial(), *this);

        std::vector<double> quotient(self_degree - divider_degree + 1);
        Polynomial self_copy = *this;
        double k;
        for (size_t shift = 0; shift <= self_degree - divider_degree; ++shift) {
            k = self_copy._koefs[self_copy._koefs.size() - 1 - shift] / divisor._koefs[divisor._koefs.size() - 1];
            quotient[self_degree - divider_degree - shift] = k;
//            std::cout << self_copy
//                      << " - (" << self_degree - divider_degree - shift << ") "
//                      << divisor.multiply_monom(self_degree - divider_degree - shift)
//                      << " = ";
            self_copy -= divisor.multiply_monom(self_degree - divider_degree - shift);
//            std::cout << self_copy << std::endl;
        }
//        std::cout << self_copy << std::endl;

        self_copy.reduce();

        Polynomial q = Polynomial(quotient);
        q.reduce();

        return std::make_pair(q, self_copy);

    }


private:
    // Обязательно длина хотя бы 1
    // _koefs[i] соответствует коэффициенту при x^i
    std::vector<double> _koefs;
};

void test_simple() {
    Polynomial poly;

    poly = Polynomial({1.0, 2.0});
    assert(poly(0.0) == 1.0);
    assert(poly(2.0) == 5.0);
    assert(poly.derivative() == Polynomial({2.0}));

    Polynomial p = Polynomial({1, 2}), q = Polynomial({3, 4});
    assert(p * q == Polynomial({3, 10, 8}));


}

void test_single_poly(const Polynomial &poly, double(*realFunc)(double), const std::string &funcName, double eps) {
    for (double x = -0.3; x < 0.3; x += 0.01) {
        double a = poly(x), b = realFunc(x);
        double delta = a - b, ratio = delta / b;
        if ((delta > eps) || (ratio > eps)) {
            std::cout << poly << " ~ " << funcName
                      << " | x = " << x
                      << " | poly = " << a
                      << " | func = " << b
                      << " | delta " << delta
                      << " | ratio = " << ratio
                      << std::endl;
        }
    }


}

void test_poly() {

    Polynomial p = Polynomial({0.0, 1, -1.0 / 2.0, 1.0 / 3.0, -1.0 / 4.0, 1.0 / 5.0});


    test_single_poly(
            p,
            [](double x) { return log(x + 1); },
            "ln(1+x)",
            1e-3
    );

    Polynomial q = Polynomial({1, 0.0, -1.0 / 6.0, 0.0, 1.0 / 120.0, 0.0, -1.0 / 5040.0});
    test_single_poly(
            q,
            [](double x) { return sin(x) / x; },
            "sin(x)/x",
            1e-3
    );


    test_single_poly(
            p * q,
            [](double x) { return log(1 + x) * sin(x) / x; },
            "log(1+x)*sin(x)/x",
            1e-3
    );

    double t = 0.05;
    std::cout << "P(0.05) = " << p(t) << " | log(1+x) = " << log(t + 1) << std::endl;
    std::cout << "Q(0.05) = " << q(t) << " | sin(x)/x = " << sin(t) / t << std::endl;
    std::cout << "P(0.05) * Q(0.05) = " << p(t) * q(t)
              << " | log(1+x)*sin(x)/x = " << log(t + 1) * sin(t) / t << std::endl;

}


void test_div() {
    Polynomial divisible = Polynomial({3, 2, 2, 1}),
            divisor = Polynomial({1, 1});

    std::cout << "divisible " << divisible << std::endl;
    std::cout << "divisor " << divisor << std::endl;

    auto [quotient, remainder] = divisible.divide(divisor);
    std::cout << "quotient " << quotient << std::endl;
    std::cout << "remainder " << remainder << std::endl;

    auto [q, r] = divisible.divide(Polynomial({1}));
    assert(q == divisible);
    assert(r == Polynomial());

    Polynomial big = Polynomial({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
    auto [q2, r2] = divisible.divide(big);
    assert(q2 == Polynomial());
    assert(r2 == divisible);
}

int main() {
    test_simple();
    test_poly();
    test_div();
}