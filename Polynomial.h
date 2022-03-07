//
// Created by Raunak Chhatwal on 3/6/22.
//

#ifndef CAS_POLYNOMIAL_H
#define CAS_POLYNOMIAL_H

#include <algorithm>
#include <stdexcept>
#include <ostream>

template <typename T>   // assumes T is an arithmetic type and T() is zero
            // e.g. if T is Matrix, then T() is the zero matrix
class Polynomial {
    using iterator = T*;    // the polynomial uses an array to store coefficients

private:
    T* coeff;
    size_t deg;  // one plus the degree of the polynomial
public:

    Polynomial() :coeff{new T()}, deg{0} { }
    Polynomial(std::initializer_list<T>);
    explicit Polynomial(size_t newDegree, bool valueInitialize=false);   // generates a polynomial of a certain degree,
    // with coefficients to be defined later

    Polynomial(const Polynomial<T>&);   // copy constructor
    Polynomial& operator=(const Polynomial<T>&);

    Polynomial(Polynomial<T>&&) noexcept ;        // move constructor
    Polynomial& operator=(Polynomial<T>&&) noexcept;
    ~Polynomial() { delete[] coeff; }      // destructor

    // getters and setters
    [[nodiscard]] size_t degree() const { return deg; }

    iterator begin() { return coeff; }
    iterator begin() const { return coeff; }
    iterator end() { return coeff + deg; }
    iterator end() const { return coeff + deg; }

    const T& getCoeff(size_t i) const;    // returns coefficient of x^i, returns 0 if i >= deg
    T& coefficient(size_t i);   // throws error when i >= deg

    // arithmetic operations
    Polynomial operator+(const Polynomial&) const;

    Polynomial operator-(const Polynomial&) const;

    Polynomial operator*(const Polynomial&) const;

    T operator()(const T&);

};

template <typename T>
Polynomial<T>::Polynomial(size_t newDegree, bool valueInitialize)
        :deg{newDegree}
{
    if (valueInitialize)
        coeff = new T[degree()]();
    else coeff = new T[degree()];
}


template <typename T>
Polynomial<T>::Polynomial(std::initializer_list<T> lst)
        :coeff{new T[lst.size()]}, deg{lst.size()}
{
    std::copy(lst.begin(), lst.end(), coeff);
}

template <typename T>
Polynomial<T>::Polynomial(const Polynomial<T>& pol)
        :deg{pol.degree()}
{
    delete[] coeff;
    coeff = new T[degree()];
    std::copy(pol.begin(), pol.end(), coeff);
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator=(const Polynomial<T>& pol)
{
    if (this == &pol) return *this;

    deg = pol.degree();

    delete[] coeff;
    coeff = new T[degree()];
    std::copy(pol.begin(), pol.end(), coeff);
}

template <typename T>
Polynomial<T>::Polynomial(Polynomial<T>&& pol) noexcept
        :coeff{pol.coeff}, deg{pol.degree()}
{
    pol.coeff = nullptr;
    pol.deg = 0;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator=(Polynomial<T>&& pol) noexcept {
    deg = pol.degree();

    delete[] coeff;
    coeff = pol.coeff;

    pol.coeff = nullptr;
    pol.deg = 0;
}

template <typename T>
const T& Polynomial<T>::getCoeff(size_t i) const {
    if (i >= degree()) return T();   // return value for zero

    return coeff[i];
}

template <typename T>
T& Polynomial<T>::coefficient(size_t i) {
    if (i >= degree()) throw std::out_of_range("Index is greater than degree of polynomial");

    return coeff[i];
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Polynomial<T>& pol) {
    int i;
    for (i = 0; i < pol.degree() && pol.getCoeff(i) == T(); i++);
    if (i == pol.degree()) {
        os << T();
        return os;
    }

    os << pol.getCoeff(i);
    if (i == 1) os << 'x';
    if (i > 1) os << '^' << i;
    for (++i; i < pol.degree(); i++) {
        if (pol.getCoeff(i) == T())
            continue;

        os << std::showpos << pol.getCoeff(i);
        if (i >= 1) os << 'x';
        if (i > 1) os << '^' << std::noshowpos << i;
    }

    return os;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator+(const Polynomial<T>& pol) const {
    size_t newSize  = std::max(degree(), pol.degree());
    Polynomial newPol(newSize);

    for (int i = 0; i < newSize; i++)
        newPol.coefficient(i) = getCoeff(i) + pol.getCoeff(i);

    return newPol;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator-(const Polynomial<T>& pol) const {
    size_t newSize  = std::max(degree(), pol.degree());
    Polynomial newPol(newSize);

    for (int i = 0; i < newSize; i++)
        newPol.coefficient(i) = getCoeff(i) - pol.getCoeff(i);

    return newPol;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator*(const Polynomial<T>& pol) const {
    Polynomial newPol(degree() + pol.degree() - 1, true);

    for (int i = 0; i < degree(); i++)
        for (int j = 0; j < pol.degree(); j++)
            newPol.coefficient(i + j) += getCoeff(i) * pol.getCoeff(j);

    return newPol;
}

template <typename T>
T Polynomial<T>::operator()(const T& t) {
    T val{};
    for (int i = degree() - 1; i >= 0; i--) {
        val = t * val + getCoeff(i);
    }

    return val;
}

#endif //CAS_POLYNOMIAL_H
