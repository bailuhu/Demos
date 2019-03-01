
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
				sorted = false;//��������־
			}
		}
		n--;//ĩβԪ�ر�Ҫ�Ѿ���λ���ɼ��ٴ������г���
	}
}

int countOnes(unsigned int n) 
{ //ͳ������n�m���Äcչ������λ1�m������O(logn)
	int ones = 0; //��������λ
	while (0 < n) 
	{ //��n������0�Tǰ��������
		ones += (1 & n); //������λ����Ϊ1�����
		n >>= 1; //�Ҷ�һλ
	}
	return ones; //ކ�ؼ���
} //��Чءglibc�m���ú���int __builtin_popcount (unsigned int n)

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