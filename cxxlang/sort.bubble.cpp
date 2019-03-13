#include <array>
#include <utility>
#include <algorithm>
#include <functional>
#include "catch.hpp"

template <typename BidirIter, typename Less>
void bubble_sort (BidirIter first, BidirIter last, Less less)
{
    using std::swap;

    while (first != last) {

        BidirIter a = first;
        BidirIter b = first;
        ++b;

        while (b != last) {
            if (less(*b, *a))
                swap(*a, *b);

            ++a;
            ++b;
        };

        // last element is sorted, so shift end position to left
        --last;
    }
}

TEST_CASE ("Bubble sort", "algo.sort") {
    std::array<int, 10> a{5, 7, 4, 2, 8, 6, 1, 9, 0, 3};

    bubble_sort(std::begin(a), std::end(a), std::less<int>{});

    CHECK(a[0] == 0);
    CHECK(a[1] == 1);
    CHECK(a[2] == 2);
    CHECK(a[3] == 3);
    CHECK(a[4] == 4);
    CHECK(a[5] == 5);
    CHECK(a[6] == 6);
    CHECK(a[7] == 7);
    CHECK(a[8] == 8);
    CHECK(a[9] == 9);
}
