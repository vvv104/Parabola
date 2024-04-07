#pragma once
#include <iostream>
#include <optional>
#include <iomanip>
#include <cmath>

namespace Parabola
{
    // some reasonably small value to compare doubles
    constexpr double epsilon = 0.000001;
    struct Solution;

    /// @brief Definition of quadratic equation with ability to print itself into a stream
    struct Definition
    {
    public:
        Definition() = default;
        Definition(int a, int b, int c)
            : a_(a), b_(b), c_(c) {}

        // parameters of equation ax^2 + bx + c = 0
        int a_ = 0;
        int b_ = 0;
        int c_ = 0;
    };

    /// @brief Contains roots and extremum of parabola
    struct Solution
    {
        Solution() = default;
        // real roots of quadratic equation
        // if 2 different then r1.has_value() and r2.has_value()
        // if 2 coincident then r1 contains the value and not r2.has_value()
        // if there is no real roots then both has no value
        std::optional<double> r1;
        std::optional<double> r2;
        // x for extremum value
        // may not exist if a == 0
        std::optional<double> e;
    };

    /// @brief Solves a quadratic equation finding its roots and extremum point
    /// @param p Definition of quadratic equation
    /// @return Solution
    Solution Solve(const Definition& p)
    {
        Solution s;

        // degenerate case of quadratic equatiob is linear equation (a == 0)
        if (p.a_ == 0)
        {
            // find a point of intersection of a line with x ordinate
            if (p.b_ != 0)
                s.r1 = (double)-p.c_ / p.b_;
        }
        else
        {
            // discriminant value
            double d = (double)p.b_ * p.b_ - 4.0 * p.a_ * p.c_;
            // positive discrimitant means we have 2 real roots
            if (d > epsilon)
            {
                s.r1 = ((double)-p.b_ - std::sqrt(d)) / 2 / p.a_;
                s.r2 = ((double)-p.b_ + std::sqrt(d)) / 2 / p.a_;
            }
            // zero discriminant - we have 2 coincident roots
            else if (std::abs(d) <= epsilon)
                s.r1 = (double)-p.b_ / 2 / p.a_;

            // finding the solution of 2ax + b = 0 equation which is the x coordinate of extrema
            s.e = (double)-p.b_ / 2 / p.a_;
        }

        return s;
    }

    std::ostream& operator<<(std::ostream& stream, const Definition& p)
    {
        stream << '(' << p.a_ << ' ' << p.b_ << ' ' << p.c_ << ')';
        return stream;
    }

    std::ostream& operator<<(std::ostream& stream, const Solution& p)
    {
        stream << std::fixed << std::setprecision(2);

        if (!p.r1.has_value())
            stream << "no roots";
        else
        {
            stream << '(' << p.r1.value();
            if (p.r2.has_value())
               stream << ' ' << p.r2.value();
            stream << ')';
        }

        stream << ' ';

        if (!p.e.has_value())
            stream << "no Xmin";
        else
        {
            stream << "Xmin=" << p.e.value();
        }

        return stream;
    }

    using Result = std::pair<Definition, Solution>;
}
