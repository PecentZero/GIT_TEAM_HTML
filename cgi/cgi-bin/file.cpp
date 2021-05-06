#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	ofstream f_o("Hello.bin",ios::out|ios::binary);
	f_o.put('c');
	f_o.close();


return 0;
}
