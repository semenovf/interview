#pragma once

template <typename ForwardIter, typename Less>
void selection_sort (ForwardIter first, ForwardIter last, Less less)
{
    using std::swap;

    while (first != last) {
        ForwardIter min = first;
        ForwardIter a = first;
        ++a;

        for (; a != last; ++a) {
            if (less(*a, *min))
                min = a;
        }

        if (min != first)
            swap(*first, *min);

        ++first;
    }
}
