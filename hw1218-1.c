/*
15パズルを幅優先探索で解く。目的の配置は16進数で表された以下の形である。
入力で与えられた配置を隣接リストに設定し、目的の配置までの最短手数を出力する。
ピースをノードとして扱う。
nodeの添字がピースの数字(0はスペース、1〜fまで)で、node->posiがピースのある場所(0〜15まで)

1 2 3 4
5 6 7 8
9 a b c
d e f -
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct Node Node;
#define MAX_NODES 100000
#define MAX_STATES 100000 //大き過ぎたらできない
#define N 16
#define M 4

struct Node{
	int posi; //ノードのある位置
	int list[N]; //隣接リスト *構造体でも配列を使う場合は確保
};

static int node_used = 0;
static Node nodes[MAX_NODES];
static Node* node[N];
const int final_state[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0}; //ゴール
static int initial_state[N];

void breadth_first_search();
bool check_same_state(int queue[MAX_STATES][N], int rear);
Node* create_node(); //ノード作成
void create_list(); //隣接リスト作成
int search_position(int n); //目的の位置にいるピースの数字を返す
int* copy_state(int copy[N], int original[N]);
int compare_state(int st1[N], int st2[N]);

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
			node[x]->posi = i/2;
		}else if(c >= 'a' && c <= 'f'){
			x = c + 0x0A - 0x61; //16進数から10進数に変換
			node[x]->posi = i/2;
		}else if(c == '-'){ //空のピースを0とする
			node[0]->posi = i/2;
		}	
	}
	for(i = 0; i < N; i++){ //初期状態
		initial_state[node[i]->posi] = i;
//		printf("添字 = %dで posi = %d\n", i, node[i]->posi);
	}
	create_list();
	breadth_first_search();
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
	//	if(s1[i] != s2[i]){
	//		return false;
	//	}
	}
	return true;
}

void breadth_first_search(){
	int front = 0, rear = 1;
	int i, j, n, s, p, keep, space[MAX_STATES], queue[MAX_STATES][N]; //space position
	space[front] = node[0]->posi; //空白の場所の初期状態 
	copy_state(queue[front], initial_state); //初期状態をキューに積む
//	printf("n = %d\n", n);
//	printf("sp = %d\n", space[front]);
/*	for(j = 0; j < N; j++){
		printf("%d ", queue[front][j]);
		if(j % M == M-1){
			printf("\n");
		}
	}
*/	

	i = 0;
	while(true){ //空白からつながるリストを見る
		s = space[front]; //取り出したキューの空白がある場所
		p = node[s]->list[i]; //隣接したピースのある場所
		n = search_position(p); //隣接したピースの数字
		if(p == -1){
			break;
		}
		copy_state(queue[rear], queue[front]); //移動した状態をキューに積むために前の状態をコピー
	//	printf("空白がある場所 = %d, 隣接したピースのある場所 = %d, 隣接したピースの数字 = %d\n", s, p, n);
		queue[rear][s] = n; //0(空白)があった場所に、隣接したピースの数字を代入
		queue[rear][p] = 0; //隣接したピースがあった場所に、0を代入

		node[s]->posi = p; //空白のあった場所に隣接したピースの位置情報を入れる
		node[n]->posi = s; //隣接したピースのあった場所に空白の位置情報を入れる
		space[rear] = p; //空白情報に、隣接したピースの位置情報を代入
		printf("i=%d\n", i);
		for(j = 0; j < N; j++){
			printf("%d ", queue[rear][j]);
			if(j % M == M-1){
				printf("\n");
			}
		}
		printf("\n");
		i++;

/*		if(check_same_state(&queue[i], rear) == true){
			for(j = 0; j < N; j++){
				printf("%d ", queue[rear][j]);
			}
			printf("\n");
			break;
		}*/
//		if(memcmp(queue[i], final_state, N) )
	}
}

bool check_same_state(int queue[MAX_STATES][N], int rear){
	int i;
	for(i = 0; i <= rear; i++){
	//	if(compare_state(queue[i], final_state) == true){
	//		return true;
	//	}
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

void create_list(){
	int i, j, x;

	for(i = 0; i < N; i++){ //リストの初期化
		for(j = 0; j < M; j++){
			node[i]->list[j] = -1;
		}
	}
	for(i = 0; i < N; i++){
//		printf("添字 = %dで posi = %d\n", i, node[i]->posi);
	}
	for(i = 0; i < N; i++){
		x = node[i]->posi; //ピースの数字iがある場所
	//	printf("ピース%dのposi = %d\n", i, x);
		//4角の場合
		if(x == 0){ //左上
			node[i]->list[0] = search_position(x+1);
			node[i]->list[1] = search_position(M);
		}else if(x == N-M){ //左下
			node[i]->list[0] = search_position(x+1);
			node[i]->list[1] = search_position(x-M);
		}else if(x == M-1){ //右上
			node[i]->list[0] = search_position(x-1);
			node[i]->list[1] = search_position(x+M);
		}else if(x == N-1){ //右下
			node[i]->list[0] = search_position(x-1);
			node[i]->list[1] = search_position(x-M);

		//4角以外の端の場合
		}else if(x > 0 && x < M-1){ //上段
			node[i]->list[0] = search_position(x-1);
			node[i]->list[1] = search_position(x+1);
			node[i]->list[2] = search_position(x+M);
		}else if(x > N-M && x < N-1){ //下段
			node[i]->list[0] = search_position(x-1);
			node[i]->list[1] = search_position(x+1);
			node[i]->list[2] = search_position(x-M);
		}else if(x % M == 0){ //左側
			node[i]->list[0] = search_position(x+1);
			node[i]->list[1] = search_position(x+M);
			node[i]->list[2] = search_position(x-M);

		}else if(x % M == M-1){ //右側
			node[i]->list[0] = search_position(x-1);
			node[i]->list[1] = search_position(x+M);
			node[i]->list[2] = search_position(x-M);
			printf("添字 = %d, posi = %dで0 = %d, 1 = %d, 2 = %d\n", i, x, node[i]->list[0], node[i]->list[1], node[i]->list[2]);

		}else{ //4方向に移動可能な場合
			node[i]->list[0] = search_position(x+1);
			node[i]->list[0] = search_position(x-1);
			node[i]->list[1] = search_position(x+M);
			node[i]->list[2] = search_position(x-M);
		}
//		printf("添字 = %d, posi = %dで0 = %d, 1 = %d, 2 = %d\n", i, x, node[i]->list[0], node[i]->list[1], node[i]->list[2]);
	}
}

int search_position(int n){
	int i;
	for(i = 0; i < N; i++){
		if(node[i]->posi == n){
			return i;
		}
	}
	return -1;
}



