#pragma once

template <typename BidirIter, typename Less>
void insertion_sort (BidirIter first, BidirIter last, Less less)
{
    using std::swap;

    BidirIter cur = first;

    while (cur != last) {
        BidirIter prev = cur;
        BidirIter x = cur;
        ++x;

        if (x == last)
            break;

        do {
            if (less(*x, *prev))
                swap(*x, *prev);

            --x;
        } while (prev-- != first);

        ++cur;
    }
}
