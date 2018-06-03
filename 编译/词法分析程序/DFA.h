#ifndef DFA_H
#define DFA_H

#include<iostream>
#include"Graph.h"
#include"NFA.h"
#include<queue>
#include<string>

struct dfa_edge_node{
	std::string end_state;//dfa����̬
	std::string edge;//dfaͼ�ߵ���Ϣ
	dfa_edge_node* next;
	dfa_edge_node(std::string e, std::string ed){
		end_state = e;
		edge = ed;
		next = nullptr;
	}
};
bool operator <(const dfa_edge_node left, const dfa_edge_node right){//����������
	return left.edge < right.edge;
}
class DFA_Graph{
public:
	std::map<std::string, std::set<dfa_edge_node>> dfa_graph;
	void dfagraph_insert(std::string beginstate, std::string endstate,std::string edge){//���뺯��
		dfa_edge_node temp(endstate, edge);
		dfa_graph[beginstate].insert(temp);
	}
	void dfagraph_traverse(std::string beginstate){//����dfaͼ
		std::cout << beginstate << "---->";
		std::set<dfa_edge_node> dfa_tuple = dfa_graph[beginstate];
		std::set<dfa_edge_node>::iterator it = dfa_tuple.begin();
		for (; it != dfa_tuple.end(); it++){
			std::cout << it->end_state << "(" << it->edge << ")" << " ";
		}
		std::cout << std::endl;
	}
};

struct dfa_node{//dfa����ʾdfaͼ�������ʾ��״̬�Լ�״̬��������˳��
	std::string sta;//״̬
	int sta_num;//״̬��������˳��
	dfa_node(std::string s, int sn){
		sta = s;
		sta_num = sn;
	}
};
bool operator <(const dfa_node left, const dfa_node right){
	return left.sta_num < right.sta_num;
}


class Dfa{
private:
	std::string set_to_string(std::set<int> point_set){//������ת�����ַ�����
		std::string state = "";
		std::set<int>::iterator it = point_set.begin();
		for (; it != point_set.end(); it++){
			state += std::to_string(*it);//������ת�����ַ�������
			state += ",";
		}
		return state;
	}
	std::set<int> string_to_set(std::string str){//���ַ�����ת�������͵ļ��� ����nfa��״̬���ϵ��Ӽ�
		int len = str.length();
		std::set<int> state_nums;
		int num = 0;
		for (int i = 0; i < len; i++){
			if (str[i] == ','){
				state_nums.insert(num);
				num = 0;
			}
			else{
				num = num * 10 + (str[i] - '0');
			}
		}
		return state_nums;
	}
	std::set<int> set_union(std::set<int> set1, std::set<int> set2){//���������������󲢼����ز���
		std::set<int>::iterator it = set2.begin();
		for (; it != set2.end(); it++){
			if (set1.count(*it) == false){
				set1.insert(*it);
			}
		}
		return set1;
	}
	std::set<dfa_node> set_union(std::set<dfa_node> set1, std::set<dfa_node> set2){
		//ȥ��set1��������set2������ͬ��Ԫ�� ����set1
		std::set<dfa_node>::iterator it1;
		std::set<dfa_node>::iterator it2;
		for (it1 = set1.begin(); it1 != set1.end(); it1++){
			for (it2 = set2.begin(); it2 != set2.end(); it2++){
				if (it1->sta != it2->sta){
					set1.insert(*it2);
				}
			}
		}
		return set1;
	}
	std::set<dfa_node> set_divide(std::set<dfa_node> set1, std::set<dfa_node> set2){//���ϵĲ�
		std::set<dfa_node> divide_set = set1;
		std::set<dfa_node>::iterator it1, it2;
		for (it1 = set1.begin(); it1 != set1.end(); it1++){
			for (it2 = set2.begin(); it2 != set2.end(); it2++){
				if (it1->sta == it2->sta){
					divide_set.erase(*it1);
				}
			}
		}
		return divide_set;
	}
	std::set<int> e_closure(Graph graph, int tmp){
		//�Ե����e�ϲ�
		//����bfs�������ҵ���ͨ������ձ����ܵ���ص㼯
		std::queue<int> q;
		q.push(tmp);//����������
		std::set<int> visited;//ͨ�������жϵ��Ƿ񱻷��ʹ���ͬʱ�õ���Щ�㼯
		while (q.empty() == false){
			if (visited.count(q.front())){//�������㱻���ʹ�
				q.pop();
				continue;
			}
			else{
				int temp = q.front();
				visited.insert(temp);
				std::set<edge_node> find_point = graph.find(temp);
				std::set<edge_node>::iterator it = find_point.begin();
				for (; it != find_point.end(); it++){
					if (it->dis == "#"){//���ͨ���ǿձ߷��������
						q.push(it->index);
					}
				}
				q.pop();
			}
		}
		return visited;
	}
	std::set<int> e_closure(Graph graph, std::set<int> point_set){//״̬����e�ϲ� ����״̬��
		std::set<int> get_point_set;
		std::set<int> visited;
		std::queue<int> Q;
		std::set<int>::iterator it_point = point_set.begin();
		for (; it_point != point_set.end(); it_point++){
			Q.push(*it_point);
		}
		while (!Q.empty()){
			if (visited.count(Q.front())){
				Q.pop();
				continue;
			}
			else{
				int temp = Q.front();
				get_point_set = set_union(e_closure(graph, temp), get_point_set);
				std::set<int>::iterator it = get_point_set.begin();
				for (; it != get_point_set.end(); it++){
					Q.push(*it);
				}
				visited.insert(temp);
				Q.pop();
			}
		}
		return get_point_set;
	}
	std::set<std::string> get_nfa_edge(Graph graph, int begin){
		//�õ�nfa�г��˿ձ����бߵ���Ϣ
		std::set<std::string> all_edge;//�������бߵ���Ϣ���� 
		//����bfs�����õ����бߵ���Ϣ
		std::queue<int> Q;
		Q.push(begin);
		std::set<int> visited;
		while (Q.empty() == false){
			if (visited.count(Q.front())){
				Q.pop();
				continue;
			}
			else{
				visited.insert(Q.front());
				std::set<edge_node> point_edge = graph.mymap[Q.front()];
				std::set<edge_node>::iterator it = point_edge.begin();
				for (; it != point_edge.end(); it++){
					Q.push(it->index);
					if (it->dis != "#"){
						all_edge.insert(it->dis);
					}
				}
				Q.pop();
			}
		}
		return all_edge;
	}
	std::set<int> state_conversion(std::set<int> cur_beginstate, std::string cur_edge, Graph graph){
		//NFA״̬����������ź������״̬
		std::set<int>::iterator it_state = cur_beginstate.begin();
		std::set<int> cur_endstate;
		for (; it_state != cur_beginstate.end(); it_state++){
			std::set<edge_node> nfa_edge = graph.mymap[*it_state];//ȡ����ǰ������Ӧ�ߵļ���
			std::set<edge_node>::iterator it = nfa_edge.begin();
			for (; it != nfa_edge.end(); it++){
				if (it->dis == cur_edge){
					cur_endstate.insert(it->index);
				}
			}
		}
		cur_endstate = e_closure(graph, cur_endstate);
		return cur_endstate;
	}
	std::set<dfa_node> DFA(Graph graph, node temp, DFA_Graph& dfa_graph){
		int dfa_num = 0;
		std::set<dfa_node> state, dfa_state;//�²�����״̬�� dfa��ǰ����״̬��
		std::set<int> init_set = e_closure(graph, temp.begin);//��ʼ״̬����
		std::string s = set_to_string(init_set);
		dfa_num++;
		dfa_node dfa_temp(s, dfa_num);//�����̬
		state.insert(dfa_temp);
		dfa_state.insert(dfa_temp);
		std::queue<std::string> Q;
		Q.push(s);
		std::set<std::string> visited;
		std::set<std::string> all_edge = get_nfa_edge(graph, temp.begin);
		while (Q.empty() == false){
			if (visited.count(Q.front())){
				Q.pop();
				continue;
			}
			else{
				std::set<std::string>::iterator it_edge = all_edge.begin();
				for (; it_edge != all_edge.end(); it_edge++){//ĳ״̬ͨ���ߵõ�����״̬
					std::set<int> cur_state = string_to_set(Q.front());//��ǰ״̬��������nfa״̬
					std::set<int> new_set = state_conversion(cur_state, *it_edge, graph);
					std::string new_state = set_to_string(e_closure(graph, new_set));
					if (!new_state.empty()){
						dfa_graph.dfagraph_insert(Q.front(), new_state, *it_edge);
						dfa_num++;
						dfa_node dfa_temp(new_state, dfa_num);
						state.insert(dfa_temp);
					}
				}
				state = set_divide(state, dfa_state);
				dfa_state = set_union(dfa_state, state);
				visited.insert(Q.front());
				std::set<dfa_node>::iterator it_state = state.begin();
				for (; it_state != state.end(); it_state++){
					Q.push(it_state->sta);
				}
				Q.pop();
			}
		}
		int num = 0;//��������״̬����ͨ�������ȥ�� ���dfa_num���ܲ��������� ͨ��num����������
		std::set<dfa_node>::iterator it = dfa_state.begin();
		std::set<dfa_node> new_dfa_state;
		for (; it != dfa_state.end(); it++){
			num++;
			dfa_node tmp(it->sta, num);
			new_dfa_state.insert(tmp);
		}
		return new_dfa_state;
	}
public:
	//��ԭ������״̬��Ϊstring��Ϊint��
	void graph_conversion(Graph A, node temp, Graph& final_dfa_graph,std::set<int>& dfa_states,std::set<int>& final_states){
		DFA_Graph Dfa_graph;
		std::set<dfa_node> dfa_state = DFA(A, temp, Dfa_graph);
		std::set<std::string> all_edge = get_nfa_edge(A, temp.begin);
		std::queue<dfa_node> Q;
		Q.push(*dfa_state.begin());
		std::set<dfa_node> visited;
		while (!Q.empty()){
			if (visited.count(Q.front())){
				Q.pop();
				continue;
			}
			else{
				int begin = Q.front().sta_num;
				std::set<dfa_edge_node> dfa_point_edge = Dfa_graph.dfa_graph[Q.front().sta];
				std::set<dfa_edge_node>::iterator it1 = dfa_point_edge.begin();
				std::set<dfa_node>::iterator it2;
				for (; it1 != dfa_point_edge.end(); it1++){
					for (it2 = dfa_state.begin(); it2 != dfa_state.end(); it2++){
						if (it1->end_state == it2->sta){
							final_dfa_graph.insert(begin, it2->sta_num, it1->edge);
							Q.push(*it2);
						}
					}
				}
				visited.insert(Q.front());
				Q.pop();
			}
		}
		std::set<dfa_node>::iterator it3 = dfa_state.begin();
		for (; it3 != dfa_state.end(); it3++){
			final_dfa_graph.traverse(it3->sta_num);
		}
		for (auto it : dfa_state){
			std::set<int> visited =string_to_set(it.sta);
			if (visited.count(temp.end)){
				final_states.insert(it.sta_num);
			}
			dfa_states.insert(it.sta_num);
		}
	}

};

#endif