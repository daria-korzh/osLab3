#include <iostream>
#include <vector>
#include <windows.h>
#include <cstdlib>

using namespace std;

struct ThreadData
{
    int number;
    vector<int>* arr;
    vector<int> markedIndices;
    HANDLE startEvent;
    HANDLE blockedEvent;
    HANDLE continueEvent;
    HANDLE thread;
    int command;
    bool active;
};

CRITICAL_SECTION cs;

DWORD WINAPI marker(LPVOID lpParam)
{
    ThreadData* data = (ThreadData*)lpParam;

    WaitForSingleObject(data->startEvent, INFINITE);

    srand(data->number);

    int size = (int)data->arr->size();

    while (true)
    {
        int index = rand() % size;
        bool occupied = false;

        EnterCriticalSection(&cs);

        if ((*data->arr)[index] == 0)
        {
            Sleep(5);

            (*data->arr)[index] = data->number;
            data->markedIndices.push_back(index);

            LeaveCriticalSection(&cs);

            Sleep(5);
        }
        else
        {
            occupied = true;

            cout << "\nПоток marker " << data->number << endl;
            cout << "Количество помеченных элементов: "
                << data->markedIndices.size() << endl;
            cout << "Невозможно пометить индекс: "
                << index << endl;

            LeaveCriticalSection(&cs);
        }

        if (occupied)
        {
            SetEvent(data->blockedEvent);

            WaitForSingleObject(data->continueEvent, INFINITE);

            if (data->command == 1)
            {
                EnterCriticalSection(&cs);

                for (int i = 0; i < data->markedIndices.size(); i++)
                {
                    int markedIndex = data->markedIndices[i];

                    if ((*data->arr)[markedIndex] == data->number)
                    {
                        (*data->arr)[markedIndex] = 0;
                    }
                }

                LeaveCriticalSection(&cs);

                return 0;
            }

            ResetEvent(data->blockedEvent);
        }
    }
}

void printArray(const vector<int>& arr)
{
    cout << "\nМассив: ";

    for (int i = 0; i < arr.size(); i++)
    {
        cout << arr[i] << " ";
    }

    cout << endl;
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
        cout << "Размер массива должен быть больше 0!" << endl;
        return 1;
    }

    vector<int> arr(size, 0);

    int count;

    cout << "Введите количество потоков marker: ";
    cin >> count;

    if (count <= 0 || count > 60)
    {
        cout << "Количество потоков должно быть от 1 до 60!" << endl;
        return 1;
    }

    InitializeCriticalSection(&cs);

    vector<ThreadData> threads(count);

    for (int i = 0; i < count; i++)
    {
        threads[i].number = i + 1;
        threads[i].arr = &arr;
        threads[i].command = 0;
        threads[i].active = true;

        threads[i].startEvent =
            CreateEvent(NULL, TRUE, FALSE, NULL);

        threads[i].blockedEvent =
            CreateEvent(NULL, TRUE, FALSE, NULL);

        threads[i].continueEvent =
            CreateEvent(NULL, FALSE, FALSE, NULL);

        threads[i].thread = NULL;

        if (threads[i].startEvent == NULL ||
            threads[i].blockedEvent == NULL ||
            threads[i].continueEvent == NULL)
        {
            cout << "Ошибка создания событий!" << endl;
            return 1;
        }
    }

    for (int i = 0; i < count; i++)
    {
        threads[i].thread = CreateThread(
            NULL,
            0,
            marker,
            &threads[i],
            0,
            NULL
        );

        if (threads[i].thread == NULL)
        {
            cout << "Ошибка создания потока!" << endl;
            return 1;
        }
    }

    for (int i = 0; i < count; i++)
    {
        SetEvent(threads[i].startEvent);
    }

    int activeCount = count;

    while (activeCount > 0)
    {
        for (int i = 0; i < count; i++)
        {
            if (threads[i].active)
            {
                WaitForSingleObject(
                    threads[i].blockedEvent,
                    INFINITE
                );
            }
        }

        printArray(arr);

        int number;

        while (true)
        {
            cout << "\nВведите номер потока marker "
                << "для завершения (1-" << count << "): ";

            cin >> number;

            if (number >= 1 &&
                number <= count &&
                threads[number - 1].active)
            {
                break;
            }

            cout << "Такого активного потока нет!" << endl;
        }

        ThreadData* selected = &threads[number - 1];

        selected->command = 1;

        ResetEvent(selected->blockedEvent);
        SetEvent(selected->continueEvent);

        WaitForSingleObject(selected->thread, INFINITE);

        selected->active = false;
        activeCount--;

        CloseHandle(selected->thread);
        CloseHandle(selected->startEvent);
        CloseHandle(selected->blockedEvent);
        CloseHandle(selected->continueEvent);

        cout << "\nПоток marker " << number
            << " завершил работу." << endl;

        printArray(arr);

        for (int i = 0; i < count; i++)
        {
            if (threads[i].active)
            {
                threads[i].command = 0;

                ResetEvent(threads[i].blockedEvent);
                SetEvent(threads[i].continueEvent);
            }
        }
    }

    DeleteCriticalSection(&cs);

    cout << "\nВсе потоки marker завершили работу." << endl;

    return 0;
}
