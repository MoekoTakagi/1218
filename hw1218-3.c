/*
ダイクストラ法を用いる。
スタートノードを節点0とし、他のノードの値を未定義とする。
まだ確定されていない、スタートノードから伸びるノードのうち、
最小の値を持つノードを見つけ、確定ノードとする。
現在の確定ノードから伸びているエッジそれぞれチェックし、
確定ノードまでのコスト+そのエッジのコストを計算し、
そのノードの現在の値よりも小さければ更新する。
その後全ての未確定ノードの中から最短経路を持つノードを一つ選び、
それを確定ノードとする。
全てのノードが確定ノードになるまで繰り返す。
*/

#include <stdio.h>
#include <stdbool.h>

typedef struct Node Node;
#define MAX_NODES 100000 //作れるノードの最大数
#define MAX_EDGES 100 //一つのノードが持つエッジの最大数
#define LARGE_NUM 1000000 //十分に大きな数字

struct Node{
	int edges_num; //ノードが持つエッジの個数
	Node* edges_to[MAX_EDGES]; //各エッジの接続先のノード
	int edges_cost[MAX_EDGES]; //各エッジのコスト
	int cost; //ノードが持つ現在の最小コスト
	bool done; //ノードが確定ノードか否か
};

Node* create_node(); //ノードの作成
void first_set_node(Node* node); //ノードの初期設定
void set_up_edges(Node* b, Node* e, int l); //エッジの初期設定
void start_node(Node* node); //スタートノードの設定
bool end_judge(Node* node[], int n); //グラフ走査の終わり判定
void update_node_cost(Node* nd); //確定ノードから伸びる未確定ノードのコストを更新する
void decide_min_node(Node* node[], int n); //未確定ノードのうち最短経路のノードを確定する

static Node nodes[MAX_NODES];
static int node_used = 0;

int main(){
	int n, b, e, l, i, j;
	scanf("%d", &n);
	Node* node[n];
	for(i = 0; i < n; i++){ //節点の個数分ノードを作る
		node[i] = create_node();
		first_set_node(node[i]);
	}
	while(scanf("%d%d%d", &b, &e, &l) != EOF){ //繋がっているノード同士のエッジの初期設定
		set_up_edges(node[b], node[e], l);
		set_up_edges(node[e], node[b], l);
	}
	start_node(node[0]); //スタートノードを0とする

	while(true){ //全てのノードを確定ノードにする
		if(end_judge(node, n) == true){ //終了判定がtrueなら全てのノードが確定ノード
			break;
		}else{
			for(i = 0; i < n; i++) update_node_cost(node[i]);
			decide_min_node(node, n);
		}
	}
	printf("%d\n", node[1]->cost);
}

Node* create_node(){
	if(node_used == MAX_NODES) return NULL;
	return &nodes[node_used++];
}
void first_set_node(Node* node){
	node->edges_num = 0;
	node->cost = LARGE_NUM;
}
void set_up_edges(Node* b, Node* e, int l){
	b->edges_to[b->edges_num] = e;
	b->edges_cost[b->edges_num] = l;
	b->edges_num++;
}
void start_node(Node* node){
	node->cost = 0; //スタートノードのコストを0とする
	node->done = true; //スタートノードの確定フラグを立てる
}
bool end_judge(Node* node[], int n){
	int i;
	for(i = 0; i < n; i++){
		if(node[i]->done != true) return false;
	}
	return true;
}
void update_node_cost(Node* nd){
	int i;
	if(nd->done == true){ //確定フラグが立っているノードから伸びるエッジを確認
		for(i = 0; i < nd->edges_num; i++){
			//確定ノードまでのコスト+そのエッジのコストがそのノードの現在の値よりも小さく、かつ、未確定ノードなら更新する。
			if(nd->edges_to[i]->done != true && nd->edges_to[i]->cost > nd->edges_cost[i] + nd->cost){
				nd->edges_to[i]->cost = nd->edges_cost[i]+nd->cost;
			}
		}
	}
}
void decide_min_node(Node* node[], int n){
	int min = LARGE_NUM, min_i, i;
	for(i = 0; i < n; i++){
		if(node[i]->done != true && node[i]->cost < min){
			min = node[i]->cost;
			min_i = i;
		}
	}
	node[min_i]->done = true; //確定フラグを立てる
}

