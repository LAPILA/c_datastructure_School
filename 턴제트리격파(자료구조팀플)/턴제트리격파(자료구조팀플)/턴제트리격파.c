#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <string.h>
#include <time.h>

//------------------------------------------------------
//구조체 트리
typedef struct TreeNode {
	int key;
	struct TreeNode* left;
	struct TreeNode* right;
} TreeNode;

typedef struct {
	TreeNode* data[100];
	int front, rear;
} QueueType;

typedef struct {
	int key;
} element;

typedef struct {
	element heap[100];
	int heap_size;
} HeapType;

typedef struct {
	TreeNode* data[100];
	int top;
} StackType;

int s_is_empty(StackType* s)
{
	if (s->top == -1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int s_is_full(StackType* s)
{
	if (s->top == 99)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void push(StackType* s, TreeNode* value)
{
	s->data[++(s->top)] = value;
}

TreeNode* pop(StackType* s)
{
	if (s_is_empty(s))
	{
		printf("스택 공백 에러");
	}
	else
	{
		TreeNode* a = s->data[(s->top)--];

		return a;
	}
}

TreeNode* peek(StackType* s)
{
	if (!s_is_empty(s))
	{
		return s->data[s->top];
	}
}

void init_queue(QueueType* q)
{
	q->front = q->rear = 0;
}

int is_empty(QueueType* q)
{
	return q->front == q->rear;
}

int is_full(QueueType* q)
{
	return (q->rear + 1) % 100 == q->front;
}

void Tenqueue(QueueType* q, TreeNode* item) {
	if (is_full(q))
		printf("공백 에러");

	q->data[++(q->rear) % 100] = item;

}

TreeNode* Tdequeue(QueueType* q)
{
	if (is_empty(q))
		return NULL;
	return q->data[++(q->front) % 100];

}

void insert(HeapType* h, int key)
{
	element temp;
	temp.key = key;


	int i = ++(h->heap_size);
	int target_index = i;
	while (i > 1)
	{
		if (temp.key > h->heap[i / 2].key)
		{
			target_index = i / 2;
			h->heap[i] = h->heap[i / 2];
		}
		else
			break;
		i /= 2;
	}
	h->heap[target_index] = temp;
}

element delete(HeapType* h)
{

	element temp = h->heap[1];
	element last = h->heap[h->heap_size--];
	int target_index = 1;
	int i = target_index * 2;

	while (i <= h->heap_size)
	{
		if (h->heap[i].key < h->heap[i + 1].key)
			i++;
		if (last.key < h->heap[i].key)
		{
			h->heap[target_index] = h->heap[i];
			target_index = i;
			i *= 2;
		}
		else
			break;
	}

	h->heap[target_index] = last;
	return temp;
}

int get_node_count(TreeNode* root)
{
	int count = 0;
	if (root != NULL)
	{
		if (root->key != 0)
			count = 1 + get_node_count(root->left) + get_node_count(root->right);
	}


	return count;
}


//트리 만들기 함수
TreeNode* CreateTree(int h) //높이가 h인 트리 생성해서 반환 
{
	TreeNode* tr[100];
	int nodeCount = (int)(pow(2.0, (double)h) - 1);

	for (int i = 1; i <= nodeCount; i++)
	{
		tr[i] = (TreeNode*)malloc(sizeof(TreeNode));
	}

	int* random_num = (int*)malloc(sizeof(int) * nodeCount);
	int expect = 28 * nodeCount;
	int sum;
	do {
		sum = 0;
		srand(time(NULL));
		for (int i = 0; i < nodeCount; i++)
		{
			random_num[i] = (rand() % (50 - 5 + 1)) + 5;
			sum += random_num[i];
		}
	} while (sum >= expect - 20 && sum <= expect + 20);


	HeapType* h1 = (HeapType*)malloc(sizeof(HeapType));
	h1->heap_size = 0;
	for (int i = 0; i < nodeCount; i++)
	{
		insert(h1, random_num[i]);

	}
	for (int i = 1; i <= nodeCount; i++)
	{
		element a = delete(h1);
		tr[i]->key = a.key;

	}

	int k = (int)(pow(2.0, (double)(h - 1)) - 1);
	for (int i = 1; i <= k; i++)
	{
		tr[i]->left = tr[i * 2];
		tr[i]->right = tr[i * 2 + 1];
	}
	for (int i = k + 1; i <= nodeCount; i++)
	{
		tr[i]->left = NULL;
		tr[i]->right = NULL;
	}

	free(h1);
	return tr[1];

}

TreeNode* findTarget(TreeNode* root, char traverse[], int traverse_count) //루트노드, 순회이름, 순회횟수 
{
	//1,2,3 -> 전위,중위,후위 

	StackType* s = (StackType*)malloc(sizeof(StackType));
	s->top = -1;

	int a = get_node_count(root);
	if ((traverse_count % a) != 0) {
		traverse_count %= a;
	}
	else {
		traverse_count = a;
	}

	if (!strcmp(traverse, "중위"))
	{

		while (1)
		{
			for (; root; root = root->left)
			{
				if (root->key <= 0)
				{
					break;
				}
				push(s, root);
			}
			TreeNode* t = pop(s);
			if (!t) break;
			traverse_count--;
			if (traverse_count == 0)
			{
				free(s);
				return t;
			}


			root = t->right;

		}

	}
	else if (!strcmp(traverse, "전위"))
	{

		TreeNode* indexNode;
		push(s, root);

		while (!s_is_empty(s))
		{
			indexNode = pop(s);
			traverse_count--;
			if (traverse_count == 0)
			{
				free(s);
				return indexNode;
			}

			if (indexNode->right) {
				if (indexNode->right->key != 0)
				{
					push(s, indexNode->right);
				}
			}
			if (indexNode->left) {
				if (indexNode->left->key != 0)
				{
					push(s, indexNode->left);
				}
			}
		}
	}
	else if (!strcmp(traverse, "후위"))
	{

		while (1)
		{

			while (NULL != root)
			{
				if (root->key == 0)
				{
					break;
				}
				push(s, root);
				push(s, root);
				root = root->left;
			}
			if (!s_is_empty(s))
			{

				root = pop(s);
				if (!s_is_empty(s) && root == peek(s))
				{
					root = root->right;
				}
				else
				{

					traverse_count--;

					if (traverse_count == 0)
					{
						free(s);
						return root;
					}


					root = NULL;

				}
			}
			else
			{
				break;
			}

		}
		free(s);
	}


}



//트리그리기
void printTree(TreeNode* root, int h)
{
	QueueType* q = (QueueType*)malloc(sizeof(QueueType));
	init_queue(q);

	Tenqueue(q, root);

	for (int i = 0; i < h; i++)
	{
		for (int k = 0; k < h - i; k++)
		{
			printf("\t");

		}

		if (i == 0)
		{
			TreeNode* t = Tdequeue(q);


			printf("%d ", t->key);

			Tenqueue(q, t->left);
			Tenqueue(q, t->right);
		}
		else
		{
			switch (i)
			{
			case 1:
				printf("    ");
				break;
			case 2:
				printf("       ");
				break;
			case 3:
				printf("             ");
				break;
			}
			for (int k = 1; k <= ((int)(pow(2.0, (double)i))) / 2; k++)
			{
				TreeNode* t1 = Tdequeue(q);
				TreeNode* t2 = Tdequeue(q);

				switch (i)
				{
				case 1:
					printf("%d", t1->key);
					printf("      ");
					printf("%d  ", t2->key);
					break;
				case 2:
					printf("%d", t1->key);
					printf("    ");
					printf("%d   ", t2->key);
					break;
				case 3:
					printf("%d", t1->key);
					printf(" ");
					printf("%d ", t2->key);
					break;
				}

				Tenqueue(q, t1->left);
				Tenqueue(q, t1->right);

				Tenqueue(q, t2->left);
				Tenqueue(q, t2->right);

			}
		}


		printf("\n\n");

	}
	free(q);
}

void FreeNode(TreeNode* root) //자식노드들 전부 free 
{
	if (root->left != NULL)
		FreeNode(root->left);
	if (root->right != NULL)
		FreeNode(root->right);

	if (root) {
		root->key = 0;
	}

}

void attackNode(TreeNode* target, int damage) //target노드에 공격 
{
	target->key -= damage;
	if (target->key <= 0)
	{
		target->key = 0; 
		if (target->left != NULL)
		{
			FreeNode(target->left);
		}

		if (target->right != NULL)
		{
			FreeNode(target->right);
		}
	}

}
//-----------------------------------------------------------------
#define DEQUE_SIZE 4 // 데크의 최대 크기를 4로 정의 (3개의 값 + 1개의 빈 공간)
#define MAX_DAMAGE 30 // 데미지의 최대 값을 30으로 정의
#define MAX_RECALL 5 // 회복의 최대 값을 5로 정의

typedef struct {
	int arr[DEQUE_SIZE]; // 데크의 요소를 저장할 배열
	int front, rear; // 데크의 앞과 뒤를 가리키는 인덱스
	int selectedIndex; // 현재 선택된 요소의 인덱스
} Deque;

// Deque를 초기화하는 함수
void initDeque(Deque* d) {
	d->front = 0;
	d->rear = 0;
	d->selectedIndex = 0;
	for (int i = 0; i < DEQUE_SIZE; i++) {
		d->arr[i] = 0;
	}
}

// Deque가 가득 찼는지 확인하는 함수 가득 찼으면 1을 반환하고, 그렇지 않으면 0을 반환
int isFull(Deque* d) {
	return ((d->rear + 1) % DEQUE_SIZE == d->front);
}

// Deque가 비었는지 확인하는 함수 비었으면 1을 반환하고, 그렇지 않으면 0을 반환
int isEmpty(Deque* d) {
	return (d->front == d->rear);
}

// Deque의 앞쪽에 값을 추가하는 함수입
void enqueueFront(Deque* d, int value) {
	if (isFull(d)) {
		printf("Deque is full\n");
		return;
	}
	d->front = (d->front - 1 + DEQUE_SIZE) % DEQUE_SIZE;
	d->arr[d->front] = value;
}

// Deque의 뒤쪽에 값을 추가하는 함수
void enqueueRear(Deque* d, int value) {
	if (isFull(d)) {
		printf("Deque is full\n");
		return;
	}
	d->arr[d->rear] = value;
	d->rear = (d->rear + 1) % DEQUE_SIZE;
}

// Deque의 앞쪽에서 값을 제거하고 그 값을 반환하는 함수
int dequeueFront(Deque* d) {
	if (isEmpty(d)) {
		printf("Deque is empty\n");
		return -1;
	}
	int value = d->arr[d->front];
	d->front = (d->front + 1) % DEQUE_SIZE;
	if (d->selectedIndex == 0) {
		d->selectedIndex = (d->selectedIndex + 1) % 2;
	}
	return value;
}

// Deque의 뒤쪽에서 값을 제거하고 그 값을 반환하는 함수
int dequeueRear(Deque* d) {
	if (isEmpty(d)) {
		printf("Deque is empty\n");
		return -1;
	}
	d->rear = (d->rear - 1 + DEQUE_SIZE) % DEQUE_SIZE;
	int value = d->arr[d->rear];
	if (d->selectedIndex == 1) {
		d->selectedIndex = (d->selectedIndex - 1 + 2) % 2;
	}
	return value;
}

// 데미지를 무작위로 생성하여 Deque에 추가하는 함수
void initDamageDeque(Deque* d) {
	srand(time(0));
	for (int i = 0; i < DEQUE_SIZE - 1; i++) {
		int randomDamage = rand() % MAX_DAMAGE + 1; // 데미지 값을 무작위로 생성
		enqueueRear(d, randomDamage); // 생성한 데미지 값을 Deque의 뒤에 추가
	}
}

// 회복 값을 무작위로 생성하여 Deque에 추가하는 함수
void initRecallDeque(Deque* d) {
	for (int i = 0; i < MAX_RECALL; i++) {
		int randomRecall = rand() % MAX_DAMAGE + 1; // 회복 값을 무작위로 생성
		enqueueRear(d, randomRecall); // 생성한 회복 값을 Deque의 뒤에 추가
	}
}

// Deque의 상태를 출력하는 함수
void printDeque(Deque* d) {
	for (int i = 0; i < DEQUE_SIZE - 1; i++) {
		if (i == d->selectedIndex) {
			printf("-> "); // 선택된 요소는 화살표로 표시
		}
		else {
			printf("   ");
		}
		printf("| %d | ", d->arr[(d->front + i) % DEQUE_SIZE]); // Deque의 요소를 출력
	}
	printf("\n");
}

// 선택한 데미지 값을 제거하고 새로운 데미지 값을 추가하는 함수
int updateDamageDeque(Deque* d) {
	int removedDamage = d->selectedIndex == 0 ? dequeueFront(d) : dequeueRear(d); // 선택된 위치에서 데미지 값을 제거
	if (removedDamage != -1) {
		printf("Selected damage: %d\n", removedDamage); // 제거한 데미지 값을 출력
		int randomDamage = rand() % MAX_DAMAGE + 1; // 새로운 데미지 값을 무작위로 생성
		enqueueRear(d, randomDamage); // 생성한 데미지 값을 Deque의 뒤에 추가
	}
	return removedDamage; // 제거한 데미지 값을 반환
}

// 선택한 회복 값을 제거하고 새로운 회복 값을 추가하는 함수
int updateRecallDeque(Deque* d) {
	int removedRecall = d->selectedIndex == 0 ? dequeueFront(d) : dequeueRear(d); // 선택된 위치에서 회복 값을 제거
	if (removedRecall != -1) {
		printf("Selected recall: %d\n", removedRecall); // 제거한 회복 값을 출력
		int randomRecall = rand() % MAX_DAMAGE + 1; // 새로운 회복 값을 무작위로 생성
		enqueueRear(d, randomRecall); // 생성한 회복 값을 Deque의 뒤에 추가
	}
	return removedRecall; // 제거한 회복 값을 반환
}

// 사용자로부터 명령을 입력받는 함수
char input_Deque()
{
	char a;
	a = getchar();
	while (getchar() != '\n') {}
	return a;
}


// 데미지 선택을 위해 사용자와 상호 작용하는 함수
int interactDamage(Deque* d) {
	char command;
	int selectdamage;
	d->selectedIndex = 0; // 선택된 항목이 항상 앞쪽을 가리키도록 항상 초기화
	while (1) {
		printDeque(d); // Deque의 상태를 출력
		printf("Select a damage: 'a' - front, 'd' - rear, 'e' - enter, 'q' - quit\n"); // 사용자에게 명령을 입력하라는 메시지를 출력

		command = input_Deque(); // 사용자로부터 명령을 입력

		switch (command) {
		case 'a':
			d->selectedIndex = 0; // 앞쪽을 선택
			break;
		case 'd':
			d->selectedIndex = DEQUE_SIZE - 2; // 뒤쪽을 선택 (DEQUE_SIZE에서 1을 뺀 값이 마지막 인덱스가 되며, 또다시 1을 빼서 빈 공간을 고려해준다)
			break;
		case 'e':
			selectdamage = updateDamageDeque(d); // 선택한 데미지 값을 제거하고 새로운 값을 추가
			return selectdamage; // 제거한 데미지 값을 반환
		case 'q':
			exit(0); // 프로그램을 종료
		default:
			printf("Invalid command\n"); // 유효하지 않은 명령을 입력한 경우 메시지를 출력
		}
	}
}

// 회복 선택을 위해 사용자와 상호 작용하는 함수
int interactRecall(Deque* d) {
	char command;
	int selectrecall;
	d->selectedIndex = 0; // 선택된 항목이 항상 앞쪽을 가리키도록 항상 초기화
	while (1) {
		printDeque(d); // Deque의 상태를 출력
		printf("Select a recall: 'a' - front, 'd' - rear, 'e' - enter, 'q' - quit\n"); // 사용자에게 명령을 입력하라는 메시지를 출력

		command = input_Deque(); // 사용자로부터 명령을 입력

		switch (command) {
		case 'a':
			d->selectedIndex = 0; // 앞쪽을 선택
			break;
		case 'd':
			d->selectedIndex = DEQUE_SIZE - 2; // 뒤쪽을 선택 (DEQUE_SIZE에서 1을 뺀 값이 마지막 인덱스가 되며, 또다시 1을 빼서 빈 공간을 고려해준다)
			break;
		case 'e':
			selectrecall = updateRecallDeque(d); // 선택한 회복 값을 제거하고 새로운 값을 추가
			return selectrecall; // 제거한 회복 값을 반환
		case 'q':
			exit(0); // 프로그램을 종료.
		default:
			printf("Invalid command\n"); // 유효하지 않은 명령을 입력한 경우 메시지를 출력
		}
	}
}
//-------------------------------------------------------------------------


int main()
{
	int Start_destroy_Tree;
	int game_rule_explain;
	int next1;
	printf("트리격파게임을 시작하겠습니까?(시작 : 1) : ");
	scanf("%d", &Start_destroy_Tree);
	if (Start_destroy_Tree != 1)
	{
		printf("종료합니다");
		return;
	}
	system("cls");
	printf("게임의 간단한 설명을 보시겠습니까?(보기 : 1) : ");
	scanf("%d", &game_rule_explain);
	if (game_rule_explain == 1)
	{
		system("cls");
		printf("이 게임의 클리어 목표는 트리의 루트노드의 숫자를 0이하 만드는 것입니다.\n");
		printf("먼저 원하는 트리 높이(2 ~ 4)를 입력하면 자동으로 트리가 만들어집니다.\n");
		printf("턴마다 데미지와 순회횟수, 순회방법(전위, 중위, 후위)을 골라 공격합니다. \n");
		printf("만약 한 노드의 숫자가 0이 되면 그 노드는 죽음 판정이 되고\n");
		printf("그 노드의 자식노드들 또한 숫자가 0이 되어 죽음 판정이 됩니다.\n");
		printf("턴을 반복하여 최종적으로 루트노드의 숫자를 0으로 만들어 보세요!\n");
		printf("아무키나 눌러 다음으로 넘어가기\n");
		if (getch() == '13');
		{
			system("cls");
		}
	}
	int Tree_h;
	system("cls");
	// 높이 입력 받기
	printf("트리 높이를 입력해주세요(최대 4) : ");
	scanf("%d", &Tree_h);
	if (Tree_h > 4) {
		printf("잘못된 입력입니다.");
		return;
	}
	system("cls");
	int damage, recall;
	char traverse[10];
	//트리 선언
	TreeNode* t = CreateTree(Tree_h);
	TreeNode* target;
	// 데미지큐, 순환횟수큐 만들기
	Deque damageDeque; // 데미지를 저장할 Deque 선언
	Deque recallDeque; // 회복을 저장할 Deque 선언
	initDeque(&damageDeque); // 데미지 Deque 초기화
	initDeque(&recallDeque); // 회복 Deque 초기화

	initDamageDeque(&damageDeque); // 데미지 Deque에 초기 데미지 값 설정
	initRecallDeque(&recallDeque); // 회복 Deque에 초기 회복 값 설정

	system("cls");
	// 클리어까지 반복 시작
	do {
		while (getchar() != '\n') {}
		printTree(t, Tree_h);
		printf("Damage values:\n"); // 데미지 값을 출력하는 문장
		damage = interactDamage(&damageDeque);
		printf("Recall values:\n"); // 회복 값을 출력하는 문장
		recall = interactRecall(&recallDeque);
		printf("전위, 중위, 후위 정하기 : ");
		scanf("%s", traverse);
		target = findTarget(t, traverse, recall);
		printf("타겟 : %d / 데미지 %d\n", target->key, damage);
		attackNode(target, damage);
		Sleep(3000);
		system("cls");
		if (t->key == 0) {
			printf("루트 노드가 사라졌습니다. 클리어를 축하합니다.\n");
			if (getch() == '13');
			{
				printf("종료\n");
			}
			break;
		}

	} while (1);

	return 0;

}