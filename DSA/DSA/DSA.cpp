
#include <iostream>
using namespace std;

void print(int a[], int size)
{
	for (int i = 0; i < size; i++)
		cout << a[i] << " ";

	cout << endl;
}

void bubble_sort1A(int A[], int n)
{
	bool sorted = false;
	while (!sorted)
	{
		sorted = true;
		for (int i = 1; i < n; i++)
		{
			if (A[i-1] > A[i])
			{
				swap(A[i - 1], A[i]);//void std::swap<int>(int&, int&)
				sorted = false;//清除排序标志
			}
		}
		n--;//末尾元素必要已经就位，可减少待排序列长度
	}
}

int countOnes(unsigned int n) 
{ //统计整数n癿二迕刢展开中数位1癿总数：O(logn)
	int ones = 0; //计数器复位
	while (0 < n) 
	{ //在n缩减至0乀前，反复地
		ones += (1 & n); //检查最低位，若为1则计数
		n >>= 1; //右秱一位
	}
	return ones; //迒回计数
} //等效亍glibc癿内置函数int __builtin_popcount (unsigned int n)

int main()
{
	int cnt = countOnes(10);
	cout << cnt << endl;
	int A[10] = { 6, 5, 1, 7, 8, 9, 4, 3, 0, 2 };
	bubble_sort1A(A, 10);
	print(A, 10);

	return 0;
}

class IntLinkedList
{
public:
	IntLinkedList();
	IntLinkedList(int i, IntLinkedList* in = 0);
	~IntLinkedList();

private:
	int info;
	IntLinkedList* next;
};

IntLinkedList::IntLinkedList()
{
	next = 0;
}

IntLinkedList::IntLinkedList(int i, IntLinkedList* in /*= 0*/)
{
	info = i; next = in;
}

IntLinkedList::~IntLinkedList()
{
}