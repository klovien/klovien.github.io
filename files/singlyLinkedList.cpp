#include<bits/stdc++.h>
using namespace std;


void test01();
class List;
class Node {

	private:
		friend class List;
		friend void test01();
		Node *next;
		int data;

	public:

		Node(int data) {
			this->data=data;
			this->next=nullptr;
		}
		int getData() {
			return this->data;
		}
		void show() {
			cout<<this->data;
		}


};




class List {
		Node *first;
		int length;
		friend void test01();

	public:
		List() {
			this->first = nullptr;
			this->length = 0;
		}
		~List() {

		}
		void print() {
			Node*p=this->first;
			while(p!=nullptr) {
				p->show();
				p = p->next;
			}
		}
		void append(int data) {
			if(first==nullptr) {
				this->first = new Node(data);
				return;
			}
			Node *p=this->first;
			while(p->next!=nullptr) {
				p=p->next;
			}
//			if(p==nullptr)
//			cout<<"p是一个空指针";
			p->next = new Node(data);
			this->length++;

		}
		Node* search(int data) {
			Node*p=this->first;
			if(data == p->data) {
				return p;
			}
			else
			while(p->next!=nullptr) {
				if(data == p->next->data) {
					return p->next;
				}
			}
			cerr<<"无此元素";
			return nullptr;
		}
		void deleteData(Node *delNode) {
			Node*p=this->first;
			if(p==delNode) {
				this->first=nullptr;
				delete delNode;
				return;
			}
			while(p->next!=nullptr) {
				if(p->next==delNode) {
					p->next=p->next->next;
					delete delNode;
					return;
				}
				p=p->next;
			}
			cerr<<"删除失败"<<endl;


		}
};


void test01() {
	List test;
	test.append(0);
	test.append(1);
	test.append(2);
	test.append(3);
	test.print();
	test.deleteData(test.search(0));
	test.print();
}



int main() {
	test01();


	return 0;
}

