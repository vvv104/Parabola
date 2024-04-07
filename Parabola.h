#pragma once
#include <iostream>
#include <optional>
#include <iomanip>

namespace Parabola
{
    constexpr double epsilon = 0.000001;
    class Solution;

    /// @brief Definition of quadratic equation with ability to print itself into a stream
    class Definition
    {
    public:
        Definition() = default;
        Definition(int a, int b, int c)
            : a_(a), b_(b), c_(c) {}

    private:
        int a_ = 0;
        int b_ = 0;
        int c_ = 0;

        friend std::ostream& operator<<(std::ostream& stream, const Definition& p);
        friend Solution Solve(const Definition& p);
    };

    /// @brief Contains roots and extremum of parabola
    class Solution
    {
    public:
        Solution() = default;

    private:
        std::optional<double> r1_;
        std::optional<double> r2_;
        std::optional<double> e_;

        friend std::ostream& operator<<(std::ostream& stream, const Solution& p);
        friend Solution Solve(const Definition& p);
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
                s.e_ = p.c_;
            else
                s.r1_ = (double)-p.c_ / p.b_;
        }
        else
        {
            double d = (double)p.b_ * p.b_ - 4.0 * p.a_ * p.c_;
            if (d > epsilon)
            {
                s.r1_ = ((double)-p.b_ - std::sqrt(d)) / 2 / p.a_;
                s.r2_ = ((double)-p.b_ + std::sqrt(d)) / 2 / p.a_;
            }
            else if (std::abs(d) <= epsilon)
                s.r1_ = (double)-p.b_ / 2 / p.a_;

            s.e_ = (double)-p.b_ / 2 / p.a_;
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

        if (!p.r1_.has_value())
            stream << "no roots";
        else
        {
            stream << '(' << p.r1_.value();
            if (p.r2_.has_value())
               stream << ' ' << p.r2_.value();
            stream << ')';
        }

        stream << ' ';

        if (!p.e_.has_value())
            stream << "no Xmin";
        else
        {
            stream << "Xmin=" << p.e_.value();
        }

        return stream;
    }

    using Result = std::pair<Definition, Solution>;
}
