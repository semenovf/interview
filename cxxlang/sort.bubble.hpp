#pragma once

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
