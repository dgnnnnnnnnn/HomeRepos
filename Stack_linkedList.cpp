#include <iostream>
using namespace std;

/* 배열과 연결리스트는 언뜻 비슷해 보이나, 차이점은 분명히 있다.
   배열은 메모리의 연속한 위치에 저장되고, 연결 리스트는 각 노드가 임의의 위치에 저장된다.*/

struct Node {
	// data와 이전노드의 주소를 가진 *prev_node
	int data;
	Node* prev_node;
};
class Stack {
private:
	// 최상단 Node를 가리킬 전역변수 top
	Node* top;
	
public:
	Stack() {
		// 생성자 -> top 초기화
		top = nullptr;
	}
	void push(int data) {
		// data를 받아 새 Node를 생성하고 prev_node와 top 설정
		Node* newNode = new Node;
		newNode->data = data;
		newNode->prev_node = top;
		top = newNode;
	}
	int pop() { 
		if (isEmpty()) {   // return top == nullptr를 받아와서 Check
			cout << "스택이 비어있습니다!" << endl;
			return false;
		}
		/* 1. pop명령으로 반환할 데이터를 받아두고 
		   2. 최상단 노드를 삭제하고 top값을 조정  */
		// 최상단 노드의 주소랑 같은 temp Node 생성
		Node* temp = top;  
		int pop_data = temp->data;
		top = temp->prev_node;

		
		delete temp;
		return pop_data;
	}
	void printStack() {
		Node* temp = top;

		if (temp == nullptr) {
			cout << "Stack이 비어있습니다!";
			return;
		}
		
		// cout << "▶ All Stacks in Data -> ";
		while (temp) {  // C++에서 포인터는 nullptr이 아닌 경우 참(true)으로 간주
			// data를 출력하고 Node를 이전 노드로 변경
			cout << temp->data << " ";
			temp = temp->prev_node;
		}
		cout << endl;
	}
	bool isEmpty() {
		// 비었는지 검사하는 문장 자체를 return
		return top == nullptr;

	}

	// 소멸자
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
		cout << "▶ Last popped : " << sNum << endl;

		cout << "▶ Unpopped data in the stack : ";
		s.printStack();
	}

	
