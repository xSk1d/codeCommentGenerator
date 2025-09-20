#include <iostream>
#include <string>
using namespace std;

class CCounter
{
private:
    int m_value;

public:
    CCounter();
    void Increment();
    int GetValue();
};

CCounter::CCounter()
{
    m_value = 0;
}

void CCounter::Increment()
{
    m_value++;
}

int CCounter::GetValue()
{
    return m_value;
}

int AddNumbers(int a, int b)
{
    int sum = a + b;
    return sum;
}

int SubtractNumbers(int a, int b)
{
    int difference = a - b;
    return difference;
}

bool IsEven(int value)
{
    if (value % 2 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

string ToUpper(const string& input)
{
    string result;
    for (char c : input)
    {
        result += toupper(c);
    }
    return result;
}

int main()
{
    int firstNumber = 7;
    int secondNumber = 15;

    int totalSum = AddNumbers(firstNumber, secondNumber);
    int totalDiff = SubtractNumbers(firstNumber, secondNumber);

    cout << "Sum: " << totalSum << ", Difference: " << totalDiff << endl;

    if (IsEven(totalSum))
    {
        cout << "Sum is even." << endl;
    }
    else
    {
        cout << "Sum is odd." << endl;
    }

    for (int i = 0; i < 3; i++)
    {
        cout << "Loop iteration: " << i << endl;
    }

    string message = "hello";
    string upperMessage = ToUpper(message);
    cout << "Uppercase: " << upperMessage << endl;

    CCounter counter;
    counter.Increment();
    counter.Increment();
    cout << "Counter value: " << counter.GetValue() << endl;

    return 0;
}
