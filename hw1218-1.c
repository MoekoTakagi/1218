/*
15パズルを幅優先探索で解く。目的の配置は16進数で表された以下の形である。
入力で与えられた配置を隣接リストに設定し、目的の配置までの最短手数を出力する。
ピースをノードとして扱う。
nodeの添字がピースの数字(0はスペース、1〜fまで)で、node->posiがピースのある場所(0〜15まで)
添字じゃなくデータをもたせる

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
	int list[M]; //隣接リスト *構造体でも配列を使う場合は確保
	int data;
};

static int node_used = 0;
static Node nodes[MAX_NODES];
static Node* node[N];
static int final_state[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0}; //ゴール
static int initial_state[N];

void breadth_first_search();
bool check_same_state(int* queue[], int rear);
Node* create_node(); //ノード作成
void create_list(); //隣接リスト作成
int search_position(int n); //目的の位置にいるピースの数字を返す
int* copy_state(int copy[], int original[]);
int compare_state(int st1[], int st2[]);
void print_state(int queue[]);

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
			node[x]->data = x;
		}else if(c >= 'a' && c <= 'f'){
			x = c + 0x0A - 0x61; //16進数から10進数に変換
			node[x]->posi = i/2;
			node[x]->data = x;
		}else if(c == '-'){ //空白のピースを0とする
			x = 0;
			node[x]->posi = i/2;
			node[x]->data = x;
		}
			
	}
	for(i = 0; i < N; i++){ //初期状態
		initial_state[node[i]->posi] = node[i]->data;
	//	printf("添字 = %dで posi = %d\n", i, node[i]->posi);
	}
	create_list();
	breadth_first_search();
}

void print_state(int queue[]){
	int i;
	for(i = 0; i < N; i++){
		if(queue[i] >= 1 && queue[i] <= 9){
			printf("%d ", queue[i]);
		}else if(queue[i] >= 10 && queue[i] <= 15){
			printf("%c ", queue[i]+87);
		}else if(queue[i] == 0){
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
	int i, j = 0, n, p, l, f_c = 0, r_c[MAX_STATES] = {0}, keep, queue[MAX_STATES][N], space, sp, counter = 0, sp_before, before_node[MAX_STATES], before_node_posi[MAX_STATES];
	sp = node[0]->posi; //空白の場所の初期状態
	space = 0; //空白のあるピースの数字
	copy_state(queue[front], initial_state); //初期状態をキューに積む
	before_node[0] = 0;
	before_node_posi[0] = 0;
//	original_space[0] = 0;

	while(true){
		printf("\nspaceの場所 = %d\n", sp); //取り出したキューの空白がある場所

		for(i = 0; i < M; i++){
	//		printf("次リスト%d = %d\n", i, node[space]->list[i]);
		}
		i = 0;
		while(true){ //空白からつながるリストを見る
			if((n = node[space]->list[i]) == -1 || i == 4){ //nは隣接したピースの数字
				break;
			}
			
//			printf("n = %d\n", n);
//			printf("\n");

			p = node[n]->posi; //隣接したピースのある場所
	//		printf("リスト%dが%dにある\n", i, p);
	//		printf("そのデータは = %d\n", node[n]->data);
			before_node_posi[rear] = front; //前のノードがあるキューの場所
	//		printf("前のノードがあるキューの場所は%d\n", before_node_posi[rear]);
	//		printf("そのキューの中身は\n");
	//		print_state(queue[before_node_posi[rear]]);
			printf("nのデータは%d\n", node[n]->data);
			copy_state(queue[rear], queue[before_node_posi[rear]]); //移動した状態をキューに積むために前の状態をコピー
		//	printf("i = %d, 空白がある場所 = %d, 隣接したピースの数字 = %d, 隣接したピースのある場所 = %d\n", i, s[front], n[rear-1], p[rear-1]);
			queue[rear][sp] = node[n]->data; //0(空白)があった場所に、隣接したピースの数字を代入
			queue[rear][p] = 0; //隣接したピースがあった場所に、0を代入

			before_node[rear] = sp; //今のノードにとっての前のノードの空白のある場所
			

		//	if(sp == 6){
				print_state(queue[rear]);
		//	}
			if(compare_state(queue[rear], final_state) == 1){
				for(j = 0; j < rear; j++){
		//			printf("%d番目の空白のある場所 = %d\n", j, before_node[j]);
				}
				printf("%d\n", counter+1);
				return;
			}
	//		r_c[front]++;
			rear++;
			i++;
		}

//		printf("rearは%d回\n", r_c[front]);
//		printf("frontは%d回\n", f_c);

		if(front != 0){ //元に戻す作業
			l = search_position(before_node[front]); //元の空白のあるピースの数字
			for(i = 0; i < N; i++){
			//	printf("%dの\n", i);
				for(j = 0; j < M; j++){
			//		printf("リストは%d\n", node[i]->list[j]);

					if(node[i]->list[j] == 0 && i != l){
						node[i]->list[j] = node[l]->data;
					//	printf("%dlist%dは%d\n", i, j, node[i]->list[j]);
					}else if(node[i]->list[j] == node[l]->data && i != space){
						node[i]->list[j] = 0;
					}
				//	printf("list%dは%d\n", j, node[i]->list[j]);
				}
			//	printf("\n");
			}
		//	printf("\n");
	//		printf("元の空白のある場所は%d\n", before_node[front]);
	//		printf("元の空白のあるピースの数字は%d\n", l);
			node[space]->data = node[l]->data; //今の空白のあるデータに元の空白のデータを入れる
			node[l]->data = 0; //元の空白のあるデータに0を入れる
			space = l; //元の空白のあるピースの数字を古い空白とする
		}
		front++;
		for(i = 0; i < N; i++){
			if(queue[front][i] == 0){
				sp = i; //spは次の空白のある場所
			}
		}

//		print_state(queue[front-1]);

		l = search_position(sp); //新しい空白のあるピースの数字

		for(i = 0; i < N; i++){
	//		printf("変更前%dの\n", i);
			for(j = 0; j < M; j++){
	//			printf("リストは%d ", node[i]->list[j]);
			}
	//		printf("\n");
		}
	//	printf("\n");
		//新しい空白と入れ替え

	//	printf("\n");
		node[space]->data = node[l]->data; //古い空白のあるデータに新しい空白のデータを入れる
		node[l]->data = 0; //新しい空白のあるデータに0を入れる

		for(i = 0; i < N; i++){
	//		printf("後%dの\n", i);
			for(j = 0; j < M; j++){
				if(node[i]->list[j] == 0){
					node[i]->list[j] = node[l]->data;
			//		printf("うき\n");
				}else if(node[i]->list[j] == node[l]->data){
					node[i]->list[j] = 0;
			//		printf("きたよ\n");
				}
	//			printf("リストは%d ", node[i]->list[j]);
			}
	//		printf("\n");
		}

		space = l;	


		if(front == 5){
	//		break;
		}
		if(f_c == r_c[j]){
//			f_c = 0;
//			counter++;
//			j++;
		}

	}
	for(i = 1; i < rear; i++){
	//	printf("spaceの場所=%d\n", space[i]);
	//	printf("ピースの数字= %d\n", n[i-1]);
	//	print_state(queue[i]);
	
	}
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
	//		printf("添字 = %d, posi = %dで0 = %d, 1 = %d, 2 = %d\n", i, x, node[i]->list[0], node[i]->list[1], node[i]->list[2]);
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
	//		printf("x+M = %d\n", x+M);
		//	printf("添字 = %d, posi = %dで0 = %d, 1 = %d, 2 = %d\n", i, x, node[i]->list[0], node[i]->list[1], node[i]->list[2]);

		}else if(x % M == 1 || x % M == 2){ //4方向に移動可能な場合
			node[i]->list[0] = search_position(x+1);
			node[i]->list[1] = search_position(x-1);
			node[i]->list[2] = search_position(x+M);
			node[i]->list[3] = search_position(x-M);
	//		printf("4 = %d\n", node[i]->list[4]);
	//		printf("ピース%d, posi = %dで0 = %d, 1 = %d, 2 = %d, 3 = %d\n", i, x, node[i]->list[0], node[i]->list[1], node[i]->list[2], node[i]->list[3]);
		}
//		printf("添字 = %d, posi = %dで0 = %d, 1 = %d, 2 = %d, 3 = %d\n", i, x, node[i]->list[0], node[i]->list[1], node[i]->list[2], node[i]->list[3]);
//		printf("ピースの数字 = %d\n", i);
		for(j = 0; node[i]->list[j] != -1; j++){
//				printf("j = %d でlist = %d\n", j, node[i]->list[j]);
			}
//			printf("\n");
	}
}

//場所を与えたら添字の数字を返す
int search_position(int n){
	int i;
	for(i = 0; i < N; i++){
		if(node[i]->posi == n){
			return i;
		}
	}
	return -1;
}



