#pragma once
#include <iostream>
#include <optional>
#include <iomanip>
#include <cmath>

namespace Parabola
{
    constexpr double epsilon = 0.000001;
    struct Solution;

    /// @brief Definition of quadratic equation with ability to print itself into a stream
    struct Definition
    {
    public:
        Definition() = default;
        Definition(int a, int b, int c)
            : a_(a), b_(b), c_(c) {}

        int a_ = 0;
        int b_ = 0;
        int c_ = 0;
    };

    /// @brief Contains roots and extremum of parabola
    struct Solution
    {
        Solution() = default;
        std::optional<double> r1;
        std::optional<double> r2;
        std::optional<double> e;
    };

    /// @brief Solves a quadratic equation finding its roots and extremum point
    /// @param p Definition of quadratic equation
    /// @return Solution
    Solution Solve(const Definition& p)
    {
        Solution s;

        if (p.a_ == 0)
        {
            if (p.b_ == 0)
                s.e = p.c_;
            else
                s.r1 = (double)-p.c_ / p.b_;
        }
        else
        {
            double d = (double)p.b_ * p.b_ - 4.0 * p.a_ * p.c_;
            if (d > epsilon)
            {
                s.r1 = ((double)-p.b_ - std::sqrt(d)) / 2 / p.a_;
                s.r2 = ((double)-p.b_ + std::sqrt(d)) / 2 / p.a_;
            }
            else if (std::abs(d) <= epsilon)
                s.r1 = (double)-p.b_ / 2 / p.a_;

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
