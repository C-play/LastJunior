#ifndef GRAPH_H
#define GRAPH_H

#include<map>
#include<set>
#include<iostream>
#include<string>

struct edge_node{
	int index;//�յ�
	std::string dis;//����
	edge_node* next;
	edge_node(int i, std::string d){//��ʼ��
		index = i;//�ߵ��յ�
		dis = d;//�ߵ���Ϣ
		next = nullptr;//��ָ��
	}
};
bool operator <(const edge_node left, const edge_node right){//����������
	if (left.index != right.index){
		return left.index < right.index;//�߼��ϵ���Ϣ���ߵ��յ��С����
	}
	else{
		return left.dis < right.dis;
	}
}
class Graph{
public:
	std::set<edge_node> find(int temp){
		return mymap[temp];
	}
	std::map<int, std::set<edge_node>> mymap;//graph�Ľṹ
	void insert(int begin, int end, std::string dis = "#"){//��ͼ�в���һ����
		edge_node temp(end, dis);
		mymap[begin].insert(temp);
	}
	void traverse(int begin){//����
		std::cout << begin << "-->";
		std::set<edge_node> myset = mymap[begin];//value��һ��set
		std::set<edge_node>::iterator it = myset.begin();//���ϵ�ָ��
		for (; it != myset.end(); it++){
			std::cout << it->index << "(" << it->dis << ")" << " ";
		}
		std::cout << std::endl;
	}
};

#endif