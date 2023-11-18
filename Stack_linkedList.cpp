#include <iostream>
using namespace std;

/* �迭�� ���Ḯ��Ʈ�� ��� ����� ���̳�, �������� �и��� �ִ�.
   �迭�� �޸��� ������ ��ġ�� ����ǰ�, ���� ����Ʈ�� �� ��尡 ������ ��ġ�� ����ȴ�.*/

struct Node {
	// data�� ��������� �ּҸ� ���� *prev_node
	int data;
	Node* prev_node;
};
class Stack {
private:
	// �ֻ�� Node�� ����ų �������� top
	Node* top;
	
public:
	Stack() {
		// ������ -> top �ʱ�ȭ
		top = nullptr;
	}
	void push(int data) {
		// data�� �޾� �� Node�� �����ϰ� prev_node�� top ����
		Node* newNode = new Node;
		newNode->data = data;
		newNode->prev_node = top;
		top = newNode;
	}
	int pop() { 
		if (isEmpty()) {   // return top == nullptr�� �޾ƿͼ� Check
			cout << "������ ����ֽ��ϴ�!" << endl;
			return false;
		}
		/* 1. pop������� ��ȯ�� �����͸� �޾Ƶΰ� 
		   2. �ֻ�� ��带 �����ϰ� top���� ����  */
		// �ֻ�� ����� �ּҶ� ���� temp Node ����
		Node* temp = top;  
		int pop_data = temp->data;
		top = temp->prev_node;

		
		delete temp;
		return pop_data;
	}
	void printStack() {
		Node* temp = top;

		if (temp == nullptr) {
			cout << "Stack�� ����ֽ��ϴ�!";
			return;
		}
		
		// cout << "�� All Stacks in Data -> ";
		while (temp) {  // C++���� �����ʹ� nullptr�� �ƴ� ��� ��(true)���� ����
			// data�� ����ϰ� Node�� ���� ���� ����
			cout << temp->data << " ";
			temp = temp->prev_node;
		}
		cout << endl;
	}
	bool isEmpty() {
		// ������� �˻��ϴ� ���� ��ü�� return
		return top == nullptr;

	}

	// �Ҹ���
	~Stack() {
		while (!isEmpty()) {
			pop();
		}
	}
};

	int main() {
		Stack s;
		s.push(40);
		s.push(50);
		s.push(929);

		int sNum = s.pop();
		cout << "�� Last popped : " << sNum << endl;

		cout << "�� Unpopped data in the stack : ";
		s.printStack();
	}

	
