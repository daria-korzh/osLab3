#pragma once
#include <vector>

using namespace std;

int findMin(const vector<int>& arr)
{
    int mn = arr[0];

    for (int i = 1; i < arr.size(); i++)
    {
        if (arr[i] < mn)
            mn = arr[i];
    }

    return mn;
}

int findMax(const vector<int>& arr)
{
    int mx = arr[0];

    for (int i = 1; i < arr.size(); i++)
    {
        if (arr[i] > mx)
            mx = arr[i];
    }

    return mx;
}

double findAverage(const vector<int>& arr)
{
    if (arr.empty())
        return 0;

    long long sum = 0;

    for (int i = 0; i < arr.size(); i++)
        sum += arr[i];

    return (double)sum / arr.size();
}
