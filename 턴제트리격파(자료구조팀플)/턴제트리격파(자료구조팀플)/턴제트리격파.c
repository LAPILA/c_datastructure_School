#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <string.h>
#include <time.h>

//------------------------------------------------------
//����ü Ʈ��
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
		printf("���� ���� ����");
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
		printf("���� ����");

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


//Ʈ�� ����� �Լ�
TreeNode* CreateTree(int h) //���̰� h�� Ʈ�� �����ؼ� ��ȯ 
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

TreeNode* findTarget(TreeNode* root, char traverse[], int traverse_count) //��Ʈ���, ��ȸ�̸�, ��ȸȽ�� 
{
	//1,2,3 -> ����,����,���� 

	StackType* s = (StackType*)malloc(sizeof(StackType));
	s->top = -1;

	int a = get_node_count(root);
	if ((traverse_count % a) != 0) {
		traverse_count %= a;
	}
	else {
		traverse_count = a;
	}

	if (!strcmp(traverse, "����"))
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
	else if (!strcmp(traverse, "����"))
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
	else if (!strcmp(traverse, "����"))
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



//Ʈ���׸���
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

void FreeNode(TreeNode* root) //�ڽĳ��� ���� free 
{
	if (root->left != NULL)
		FreeNode(root->left);
	if (root->right != NULL)
		FreeNode(root->right);

	if (root) {
		root->key = 0;
	}

}

void attackNode(TreeNode* target, int damage) //target��忡 ���� 
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
#define DEQUE_SIZE 4 // ��ũ�� �ִ� ũ�⸦ 4�� ���� (3���� �� + 1���� �� ����)
#define MAX_DAMAGE 30 // �������� �ִ� ���� 30���� ����
#define MAX_RECALL 5 // ȸ���� �ִ� ���� 5�� ����

typedef struct {
	int arr[DEQUE_SIZE]; // ��ũ�� ��Ҹ� ������ �迭
	int front, rear; // ��ũ�� �հ� �ڸ� ����Ű�� �ε���
	int selectedIndex; // ���� ���õ� ����� �ε���
} Deque;

// Deque�� �ʱ�ȭ�ϴ� �Լ�
void initDeque(Deque* d) {
	d->front = 0;
	d->rear = 0;
	d->selectedIndex = 0;
	for (int i = 0; i < DEQUE_SIZE; i++) {
		d->arr[i] = 0;
	}
}

// Deque�� ���� á���� Ȯ���ϴ� �Լ� ���� á���� 1�� ��ȯ�ϰ�, �׷��� ������ 0�� ��ȯ
int isFull(Deque* d) {
	return ((d->rear + 1) % DEQUE_SIZE == d->front);
}

// Deque�� ������� Ȯ���ϴ� �Լ� ������� 1�� ��ȯ�ϰ�, �׷��� ������ 0�� ��ȯ
int isEmpty(Deque* d) {
	return (d->front == d->rear);
}

// Deque�� ���ʿ� ���� �߰��ϴ� �Լ���
void enqueueFront(Deque* d, int value) {
	if (isFull(d)) {
		printf("Deque is full\n");
		return;
	}
	d->front = (d->front - 1 + DEQUE_SIZE) % DEQUE_SIZE;
	d->arr[d->front] = value;
}

// Deque�� ���ʿ� ���� �߰��ϴ� �Լ�
void enqueueRear(Deque* d, int value) {
	if (isFull(d)) {
		printf("Deque is full\n");
		return;
	}
	d->arr[d->rear] = value;
	d->rear = (d->rear + 1) % DEQUE_SIZE;
}

// Deque�� ���ʿ��� ���� �����ϰ� �� ���� ��ȯ�ϴ� �Լ�
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

// Deque�� ���ʿ��� ���� �����ϰ� �� ���� ��ȯ�ϴ� �Լ�
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

// �������� �������� �����Ͽ� Deque�� �߰��ϴ� �Լ�
void initDamageDeque(Deque* d) {
	srand(time(0));
	for (int i = 0; i < DEQUE_SIZE - 1; i++) {
		int randomDamage = rand() % MAX_DAMAGE + 1; // ������ ���� �������� ����
		enqueueRear(d, randomDamage); // ������ ������ ���� Deque�� �ڿ� �߰�
	}
}

// ȸ�� ���� �������� �����Ͽ� Deque�� �߰��ϴ� �Լ�
void initRecallDeque(Deque* d) {
	for (int i = 0; i < MAX_RECALL; i++) {
		int randomRecall = rand() % MAX_DAMAGE + 1; // ȸ�� ���� �������� ����
		enqueueRear(d, randomRecall); // ������ ȸ�� ���� Deque�� �ڿ� �߰�
	}
}

// Deque�� ���¸� ����ϴ� �Լ�
void printDeque(Deque* d) {
	for (int i = 0; i < DEQUE_SIZE - 1; i++) {
		if (i == d->selectedIndex) {
			printf("-> "); // ���õ� ��Ҵ� ȭ��ǥ�� ǥ��
		}
		else {
			printf("   ");
		}
		printf("| %d | ", d->arr[(d->front + i) % DEQUE_SIZE]); // Deque�� ��Ҹ� ���
	}
	printf("\n");
}

// ������ ������ ���� �����ϰ� ���ο� ������ ���� �߰��ϴ� �Լ�
int updateDamageDeque(Deque* d) {
	int removedDamage = d->selectedIndex == 0 ? dequeueFront(d) : dequeueRear(d); // ���õ� ��ġ���� ������ ���� ����
	if (removedDamage != -1) {
		printf("Selected damage: %d\n", removedDamage); // ������ ������ ���� ���
		int randomDamage = rand() % MAX_DAMAGE + 1; // ���ο� ������ ���� �������� ����
		enqueueRear(d, randomDamage); // ������ ������ ���� Deque�� �ڿ� �߰�
	}
	return removedDamage; // ������ ������ ���� ��ȯ
}

// ������ ȸ�� ���� �����ϰ� ���ο� ȸ�� ���� �߰��ϴ� �Լ�
int updateRecallDeque(Deque* d) {
	int removedRecall = d->selectedIndex == 0 ? dequeueFront(d) : dequeueRear(d); // ���õ� ��ġ���� ȸ�� ���� ����
	if (removedRecall != -1) {
		printf("Selected recall: %d\n", removedRecall); // ������ ȸ�� ���� ���
		int randomRecall = rand() % MAX_DAMAGE + 1; // ���ο� ȸ�� ���� �������� ����
		enqueueRear(d, randomRecall); // ������ ȸ�� ���� Deque�� �ڿ� �߰�
	}
	return removedRecall; // ������ ȸ�� ���� ��ȯ
}

// ����ڷκ��� ����� �Է¹޴� �Լ�
char input_Deque()
{
	char a;
	a = getchar();
	while (getchar() != '\n') {}
	return a;
}


// ������ ������ ���� ����ڿ� ��ȣ �ۿ��ϴ� �Լ�
int interactDamage(Deque* d) {
	char command;
	int selectdamage;
	d->selectedIndex = 0; // ���õ� �׸��� �׻� ������ ����Ű���� �׻� �ʱ�ȭ
	while (1) {
		printDeque(d); // Deque�� ���¸� ���
		printf("Select a damage: 'a' - front, 'd' - rear, 'e' - enter, 'q' - quit\n"); // ����ڿ��� ����� �Է��϶�� �޽����� ���

		command = input_Deque(); // ����ڷκ��� ����� �Է�

		switch (command) {
		case 'a':
			d->selectedIndex = 0; // ������ ����
			break;
		case 'd':
			d->selectedIndex = DEQUE_SIZE - 2; // ������ ���� (DEQUE_SIZE���� 1�� �� ���� ������ �ε����� �Ǹ�, �Ǵٽ� 1�� ���� �� ������ ������ش�)
			break;
		case 'e':
			selectdamage = updateDamageDeque(d); // ������ ������ ���� �����ϰ� ���ο� ���� �߰�
			return selectdamage; // ������ ������ ���� ��ȯ
		case 'q':
			exit(0); // ���α׷��� ����
		default:
			printf("Invalid command\n"); // ��ȿ���� ���� ����� �Է��� ��� �޽����� ���
		}
	}
}

// ȸ�� ������ ���� ����ڿ� ��ȣ �ۿ��ϴ� �Լ�
int interactRecall(Deque* d) {
	char command;
	int selectrecall;
	d->selectedIndex = 0; // ���õ� �׸��� �׻� ������ ����Ű���� �׻� �ʱ�ȭ
	while (1) {
		printDeque(d); // Deque�� ���¸� ���
		printf("Select a recall: 'a' - front, 'd' - rear, 'e' - enter, 'q' - quit\n"); // ����ڿ��� ����� �Է��϶�� �޽����� ���

		command = input_Deque(); // ����ڷκ��� ����� �Է�

		switch (command) {
		case 'a':
			d->selectedIndex = 0; // ������ ����
			break;
		case 'd':
			d->selectedIndex = DEQUE_SIZE - 2; // ������ ���� (DEQUE_SIZE���� 1�� �� ���� ������ �ε����� �Ǹ�, �Ǵٽ� 1�� ���� �� ������ ������ش�)
			break;
		case 'e':
			selectrecall = updateRecallDeque(d); // ������ ȸ�� ���� �����ϰ� ���ο� ���� �߰�
			return selectrecall; // ������ ȸ�� ���� ��ȯ
		case 'q':
			exit(0); // ���α׷��� ����.
		default:
			printf("Invalid command\n"); // ��ȿ���� ���� ����� �Է��� ��� �޽����� ���
		}
	}
}
//-------------------------------------------------------------------------


int main()
{
	int Start_destroy_Tree;
	int game_rule_explain;
	int next1;
	printf("Ʈ�����İ����� �����ϰڽ��ϱ�?(���� : 1) : ");
	scanf("%d", &Start_destroy_Tree);
	if (Start_destroy_Tree != 1)
	{
		printf("�����մϴ�");
		return;
	}
	system("cls");
	printf("������ ������ ������ ���ðڽ��ϱ�?(���� : 1) : ");
	scanf("%d", &game_rule_explain);
	if (game_rule_explain == 1)
	{
		system("cls");
		printf("�� ������ Ŭ���� ��ǥ�� Ʈ���� ��Ʈ����� ���ڸ� 0���� ����� ���Դϴ�.\n");
		printf("���� ���ϴ� Ʈ�� ����(2 ~ 4)�� �Է��ϸ� �ڵ����� Ʈ���� ��������ϴ�.\n");
		printf("�ϸ��� �������� ��ȸȽ��, ��ȸ���(����, ����, ����)�� ��� �����մϴ�. \n");
		printf("���� �� ����� ���ڰ� 0�� �Ǹ� �� ���� ���� ������ �ǰ�\n");
		printf("�� ����� �ڽĳ��� ���� ���ڰ� 0�� �Ǿ� ���� ������ �˴ϴ�.\n");
		printf("���� �ݺ��Ͽ� ���������� ��Ʈ����� ���ڸ� 0���� ����� ������!\n");
		printf("�ƹ�Ű�� ���� �������� �Ѿ��\n");
		if (getch() == '13');
		{
			system("cls");
		}
	}
	int Tree_h;
	system("cls");
	// ���� �Է� �ޱ�
	printf("Ʈ�� ���̸� �Է����ּ���(�ִ� 4) : ");
	scanf("%d", &Tree_h);
	if (Tree_h > 4) {
		printf("�߸��� �Է��Դϴ�.");
		return;
	}
	system("cls");
	int damage, recall;
	char traverse[10];
	//Ʈ�� ����
	TreeNode* t = CreateTree(Tree_h);
	TreeNode* target;
	// ������ť, ��ȯȽ��ť �����
	Deque damageDeque; // �������� ������ Deque ����
	Deque recallDeque; // ȸ���� ������ Deque ����
	initDeque(&damageDeque); // ������ Deque �ʱ�ȭ
	initDeque(&recallDeque); // ȸ�� Deque �ʱ�ȭ

	initDamageDeque(&damageDeque); // ������ Deque�� �ʱ� ������ �� ����
	initRecallDeque(&recallDeque); // ȸ�� Deque�� �ʱ� ȸ�� �� ����

	system("cls");
	// Ŭ������� �ݺ� ����
	do {
		while (getchar() != '\n') {}
		printTree(t, Tree_h);
		printf("Damage values:\n"); // ������ ���� ����ϴ� ����
		damage = interactDamage(&damageDeque);
		printf("Recall values:\n"); // ȸ�� ���� ����ϴ� ����
		recall = interactRecall(&recallDeque);
		printf("����, ����, ���� ���ϱ� : ");
		scanf("%s", traverse);
		target = findTarget(t, traverse, recall);
		printf("Ÿ�� : %d / ������ %d\n", target->key, damage);
		attackNode(target, damage);
		Sleep(3000);
		system("cls");
		if (t->key == 0) {
			printf("��Ʈ ��尡 ��������ϴ�. Ŭ��� �����մϴ�.\n");
			if (getch() == '13');
			{
				printf("����\n");
			}
			break;
		}

	} while (1);

	return 0;

}