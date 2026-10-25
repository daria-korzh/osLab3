#include <iostream>
#include <vector>
#include <windows.h>
#include "Functions.h"

using namespace std;

typedef void (*ReplaceMinMaxFunc)(int*, int, int, int, int);

struct ThreadData
{
    const vector<int>* arr;
    int minVal;
    int maxVal;
    double averageVal;
};

DWORD WINAPI minMaxThread(LPVOID lpParam)
{
    ThreadData* data = (ThreadData*)lpParam;

    data->minVal = findMin(*data->arr);
    data->maxVal = findMax(*data->arr);

    cout << "[min_max] Минимальный элемент: "
        << data->minVal << endl;

    cout << "[min_max] Максимальный элемент: "
        << data->maxVal << endl;

    return 0;
}

DWORD WINAPI averageThread(LPVOID lpParam)
{
    ThreadData* data = (ThreadData*)lpParam;

    data->averageVal = findAverage(*data->arr);

    cout << "[average] Среднее арифметическое значение: "
        << data->averageVal << endl;

    return 0;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int size;

    cout << "Введите размер массива: ";
    cin >> size;

    if (size <= 0)
    {
        cout << "Ошибка: размер массива должен быть больше 0!" << endl;
        return 0;
    }

    vector<int> arr(size);

    cout << "Введите элементы массива (" << size << " шт.):" << endl;

    for (int i = 0; i < size; i++)
    {
        cin >> arr[i];
    }

    ThreadData data;
    data.arr = &arr;
    data.minVal = 0;
    data.maxVal = 0;
    data.averageVal = 0.0;

    HANDLE hMinMax = CreateThread(
        NULL, 0, minMaxThread, &data, 0, NULL
    );

    HANDLE hAverage = CreateThread(
        NULL, 0, averageThread, &data, 0, NULL
    );

    if (hMinMax == NULL || hAverage == NULL)
    {
        cout << "Ошибка при создании потоков!" << endl;

        if (hMinMax != NULL)
            CloseHandle(hMinMax);

        if (hAverage != NULL)
            CloseHandle(hAverage);

        return 1;
    }

    WaitForSingleObject(hMinMax, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    CloseHandle(hMinMax);
    CloseHandle(hAverage);

    int avgInt = (int)data.averageVal;

    HMODULE hLib = LoadLibraryA("test.dll");

    if (hLib != NULL)
    {
        ReplaceMinMaxFunc replaceMinMax =
            (ReplaceMinMaxFunc)GetProcAddress(hLib, "replaceMinMax");

        if (replaceMinMax != NULL)
        {
            replaceMinMax(
                arr.data(),
                size,
                data.minVal,
                data.maxVal,
                avgInt
            );
        }
        else
        {
            cout << "Функция replaceMinMax не найдена!" << endl;
        }

        FreeLibrary(hLib);
    }
    else
    {
        cout << "Не удалось загрузить test.dll" << endl;
    }

    cout << endl;
    cout << "Массив (min и max заменены на "
        << avgInt << "):" << endl;

    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << " ";
    }

    cout << endl;

    return 0;
}
