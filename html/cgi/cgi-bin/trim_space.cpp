#include <iostream>
#include <string>


using namespace std;
void trim_space(string &temp)
{
    int start = -1;
    int len,end;
    int start_end, end_start, i;
    len = temp.length();
    for (i = 0; i<len; i++)
    {
        if (temp[i] != ' ') break;    // i-1
    }
    start_end = i - 1;
    if (start_end >= 0)
        temp.replace(0, start_end + 1, "");

    len = temp.length();
    end = len;
    for (i = len - 1; i >= 0; i--)
    {
        if (temp[i] != ' ') break;  // i+1
    }
    end_start = i + 1;
    if (end_start < len)
    temp.replace(end_start, end - end_start, "");
}


int main()
{
    string test = "  dfjds  sdhf sk fshj ";

    trim_space(test);
    cout << test;
    return 0;
}
