/*  手打 循环队列queue */
#include <iostream>
#define MAX_SIZE 5
using namespace std;

template <class T>
class Queue
{
	int count;
	int front;
	int rear;
	T data[MAX_SIZE];

public:
	Queue()
	{ //初始化队列

		this->count = 0;
		this->front = 0;
		this->rear = -1;
	}

	bool isFull()
	{ //判断队列是否满
		return this->count >= MAX_SIZE;
	}

	bool isEmpty()
	{ //判断队列是否为空
		return this->count <= 0;
	}

	void push(T item)
	{ //入队

		if (this->isFull())
		{
			cerr << "队列满了!" << endl;
			return;
		}
		else
		{
			this->count++;
			this->rear = (this->rear + 1) % MAX_SIZE;
			this->data[this->rear] = item;
		}
	}

	void pop()
	{ //出队
		if (this->isEmpty())
		{
			cerr << "队列为空!" << endl;
			return;
		}
		else
		{
			this->count--;
			this->front = (this->front + 1) % MAX_SIZE;
		}
	}

	int size()
	{ //返回队列长度
		return this->count;
	}

	void show()
	{ //遍历队列
		if (this->isEmpty())
		{
			cout << "队列为空!" << endl;
			return;
		}
		else
		{
			cout << "队列遍历: ";
			int i = this->front;
			while (i != this->rear)
			{
				cout << this->data[i] << " ";
				i++;
				i = i % MAX_SIZE;
			}
			cout << this->data[this->rear] << " ";
		}
		cout << endl;
	}
};

int main()
{
	Queue<int> dummy; //申请结点空间

	dummy.push(11);
	dummy.push(5);
	dummy.push(23);
	dummy.push(18);
	dummy.pop();
	dummy.show();

	dummy.push(33);
	dummy.pop();
	dummy.show();

	dummy.push(77);
	dummy.show();

	return 0;
}

//运行结果：
//
//队列遍历: 5 23 18
//队列遍历: 23 18 33
//队列遍历: 23 18 33 77
