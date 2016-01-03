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

typedef struct Node Node;
#define MAX_NODES 100000
#define N 16
#define M 4

struct Node{
	int posi; //ノードのある位置
	int list[]; //隣接リスト
};

static int node_used = 0;
static Node nodes[MAX_NODES];
static Node* node[N];
const int final_state[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0}; //ゴール

Node* create_node(); //ノード作成
void create_list(); //隣接リスト作成
int search_position(int n); //目的の位置にいるピースの数字を返す

int main(){
	int i, x, initial_state[N];
	char c;
	for(i = 0; i < N; i++){
		node[i] = create_node();
	}
	for(i = 0; i < 2*N; i++){
		c = getchar();
		if(c <= '9' && c >= '1'){
			x = c - '0';
			node[x]->posi = i/2;
		//	printf("x = %d, posi = %d\n", x, node[x]->posi);
		//	printf("x = %d\n", x);
		}else if(c >= 'a' && c <= 'f'){
			x = c + 0x0A - 0x61; //16進数から10進数に変換
			node[x]->posi = i/2;
		//	printf("%d\n", x);

		}else if(c == '-'){ //空のピースを0とする
			node[0]->posi = i/2;

		}
	}
	for(i = 0; i < N; i++){ //初期状態
//		printf("i = %d, posi = %d\n", i, node[i]->posi);
		initial_state[node[i]->posi] = i;
	}
	for(i = 0; i < N; i++){
		printf("%d ", initial_state[i]);
	}
	create_list();
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
		x = node[i]->posi;
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
		}else if(x % M == 0){
			node[i]->list[0] = search_position(x+1);
			node[i]->list[1] = search_position(x+M);
			node[i]->list[2] = search_position(x-M);
		}else if(x % M == 3){
			node[i]->list[0] = search_position(x-1);
			node[i]->list[1] = search_position(x+M);
			node[i]->list[2] = search_position(x-M);

		}else{ //4方向に移動可能な場合
			node[i]->list[0] = search_position(x+1);
			node[i]->list[0] = search_position(x-1);
			node[i]->list[1] = search_position(x+M);
			node[i]->list[2] = search_position(x-M);
		}
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



