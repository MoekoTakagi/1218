/*
15パズルを幅優先探索で解く。目的の配置は16進数で表された以下の形である。
入力で与えられた配置を隣接リストに設定し、目的の配置までの最短手数を出力する。
ピースをノードとして扱う。
nodeの添字がピースの位置、node->posiがピースのある場所(0〜15まで)
添字じゃなくデータをもたせる
キューを使って簡潔に表す

1 2 3 4
5 6 7 8
9 a b c
d e f -
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct Node Node;
typedef struct Queue Queue;
#define MAX_NODES 100000
#define MAX_STATES 100000 //大き過ぎたらできない
#define N 16
#define M 4

struct Node{
	int list[M]; //隣接リスト *構造体でも配列を使う場合は確保
	int data; //ピースが持つ数字
};

struct Queue{
	int data[N];
	int space; //キューが持つ空白のポジション
	int point; //キューがいる場所
	Queue* parent;
	Queue* kids[M+1];
	bool done;
};

static int node_used = 0;
static Node nodes[MAX_NODES];
static Node* node[N];
static Queue ques[MAX_STATES];
static Queue* queue[MAX_STATES];
static int que_used = 0;
static int final_state[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0}; //ゴール
static int initial_state[N];

void breadth_first_search();
bool check_same_state(int* queue[], int rear);
Node* create_node(); //ノード作成
void create_list(); //隣接リスト作成
int search_position(int n); //目的の位置にいるピースの数字を返す
int* copy_state(int copy[], int original[]);
int compare_state(int st1[], int st2[]);
void print_state(Queue* queue);
Queue* create_queue();
void end_search(Queue* queue);

int main(){
	int i, x;
	char c;
	for(i = 0; i < N; i++){
		node[i] = create_node();
	}
	for(i = 0; i < 2*N; i++){
		c = getchar();
		if(c <= '9' && c >= '1'){
			x = c - '0';
			node[i/2]->data = x;
		}else if(c >= 'a' && c <= 'f'){
			x = c + 0x0A - 0x61; //16進数から10進数に変換
			node[i/2]->data = x;
		}else if(c == '-'){ //空白のピースを0とする
			x = 0;
			node[i/2]->data = x;
		}
	}
	for(i = 0; i < N; i++){ //初期状態
		initial_state[i] = node[i]->data;
	}
	create_list();
	breadth_first_search();
}

void print_state(Queue* queue){
	int i;
	for(i = 0; i < N; i++){
		if(queue->data[i] >= 1 && queue->data[i] <= 9){
			printf("%d ", queue->data[i]);
		}else if(queue->data[i] >= 10 && queue->data[i] <= 15){
			printf("%c ", queue->data[i]+87);
		}else if(queue->data[i] == 0){
			printf("- ");
		}
		if(i % M == M-1){
			printf("\n");
		}
	}
	printf("\n");
}


int* copy_state(int copy[], int original[]){
	int i;
	for(i = 0; i < N; i++){
		copy[i] = original[i];
	}
	return &copy[0];
}

int compare_state(int s1[], int s2[]){
	int i;
	for(i = 0; i < N; i++){
		if(s1[i] != s2[i]){
			return 0;
		}
	}
	return 1;
}

void breadth_first_search(){
	int front = 0, rear = 1;
	int i, j = 0, p, f_c = 0, r_c[MAX_STATES] = {0}, sp, counter = 0;
	queue[0] = create_queue();
	for(i = 0; i <= M; i++){
		queue[0]->kids[i] = NULL;
	}
	queue[0]->parent = NULL;
	queue[0]->point = 0;

	copy_state(queue[front]->data, initial_state); //初期状態をキューに積む

	while(true){
		queue[front]->done = true;
		for(i = 0; i < N; i++){
			node[i]->data = queue[front]->data[i];
			if(node[i]->data == 0){
				sp = i;
				queue[front]->space = i;
			}
		}

	//	printf("\nspaceの場所 = %d\n", sp); //取り出したキューの空白がある場所
		i = 0;
		while(true){
			//空白からつながるリストを見る
			if((p = node[sp]->list[i]) == -1 || i == 4){ //pは隣接したピースのポジション
				break;
			}
			queue[rear] = create_queue();
			queue[rear]->parent = queue[front];
			queue[front]->kids[i] = queue[rear];
			copy_state(queue[rear]->data, queue[rear]->parent->data);

			queue[rear]->data[sp] = node[p]->data; //0(空白)があった場所に、隣接したピースの数字を代入
			queue[rear]->data[p] = 0; //隣接したピースがあった場所に、0を代入
			queue[rear]->space = p;
			queue[rear]->point = rear;

			if(compare_state(queue[rear]->data, final_state) == 1){
				end_search(queue[rear]);
				return;
			}

			for(j = 0; j <= M; j++){
				queue[rear]->kids[j] = NULL;
			}

		//	printf("今のキュー\n");
		//	print_state(queue[rear]);

			rear++;
			i++;
		}
		front++;

		if(front == 8){
	//		break;
		}
	}
}

void end_search(Queue* que){
	int counter = 0;
	while(que != NULL){
		que = que->parent;
		counter++;
	}
	printf("%d\n", counter-1);
}

bool check_same_state(int* queue[], int rear){
	int i;
	for(i = 0; i <= rear; i++){
		if(compare_state(queue[i], queue[rear])){
			return true;
		}
	}
	return false;
}


Node* create_node(){
	if(node_used == MAX_NODES){
		return NULL;
	}else{
		return &nodes[node_used++];
	}
}

Queue* create_queue(){
	if(que_used == MAX_STATES){
		return NULL;
	}else{
		return &ques[que_used++];
	}
}

void create_list(){
	int i, j, x;
	for(i = 0; i < N; i++){ //リストの初期化
		for(j = 0; j < M; j++){
			node[i]->list[j] = -1;
		}
	}
	for(i = 0; i < N; i++){ //リストにポジションを参照させる
		//4角の場合
		if(i == 0){ //左上
			node[i]->list[0] = i+1;
			node[i]->list[1] = M;
		}else if(i == N-M){ //左下
			node[i]->list[0] = i+1;
			node[i]->list[1] = i-M;
	//		printf("添字 = %d, posi = %dで0 = %d, 1 = %d, 2 = %d\n", i, x, node[i]->list[0], node[i]->list[1], node[i]->list[2]);
		}else if(i == M-1){ //右上
			node[i]->list[0] = i-1;
			node[i]->list[1] = i+M;
		}else if(i == N-1){ //右下
			node[i]->list[0] = i-1;
			node[i]->list[1] = i-M;

		//4角以外の端の場合
		}else if(i > 0 && i < M-1){ //上段
			node[i]->list[0] = i-1;
			node[i]->list[1] = i+1;
			node[i]->list[2] = i+M;
		}else if(i > N-M && i < N-1){ //下段
			node[i]->list[0] = i-1;
			node[i]->list[1] = i+1;
			node[i]->list[2] = i-M;
		}else if(i % M == 0){ //左側
			node[i]->list[0] = i+1;
			node[i]->list[1] = i+M;
			node[i]->list[2] = i-M;

		}else if(i % M == M-1){ //右側
			node[i]->list[0] = i-1;
			node[i]->list[1] = i+M;
			node[i]->list[2] = i-M;
	//		printf("x+M = %d\n", x+M);
		//	printf("添字 = %d, posi = %dで0 = %d, 1 = %d, 2 = %d\n", i, x, node[i]->list[0], node[i]->list[1], node[i]->list[2]);

		}else if(i % M == 1 || i % M == 2){ //4方向に移動可能な場合
			node[i]->list[0] = i+1;
			node[i]->list[1] = i-1;
			node[i]->list[2] = i+M;
			node[i]->list[3] = i-M;
	//		printf("4 = %d\n", node[i]->list[4]);
	//		printf("ピース%d, posi = %dで0 = %d, 1 = %d, 2 = %d, 3 = %d\n", i, x, node[i]->list[0], node[i]->list[1], node[i]->list[2], node[i]->list[3]);
		}
//		printf("ポジション=%d, データ=%dで0 = %d, 1 = %d, 2 = %d, 3 = %d\n", i, node[i]->data, node[i]->list[0], node[i]->list[1], node[i]->list[2], node[i]->list[3]);
//		printf("ピースの数字 = %d\n", i);
//		for(j = 0; node[i]->list[j] != -1; j++){
//				printf("j = %d でlist = %d\n", j, node[i]->list[j]);
//			}
//			printf("\n");
	}
}

//場所を与えたら添字の数字を返す
int search_position(int n){
	int i;
	for(i = 0; i < N; i++){
//		if(node[i]->posi == n){
			return i;
//		}
	}
	return -1;
}



