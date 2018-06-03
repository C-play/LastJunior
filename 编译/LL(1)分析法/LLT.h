#ifndef LLT_H
#define LLT_H
#include"RLR.h"
#include"fileiostream.h"
#include<queue>
#include<vector>

struct node{
	std::string nonterm;
	std::string css;
	node(std::string non, std::string c){
		nonterm = non;
		css = c;
	}
	node(){
		nonterm = "";
		css = "";
	}
};

bool operator <(const node left, const node right){
	if (left.nonterm != right.nonterm){
		return left.nonterm < right.nonterm;
	}
	else{
		return left.css < right.css;
	}
}

class Predict_Table{
private:
	std::vector<std::string> get_csselement(std::string str){
		std::vector<std::string> css_element;
		int len = str.length();
		std::string temp = "";
		for (int i = 0; i < len; i++){
			if (str[i] != ' '&&str[i] != '\t'){
				temp += str[i];
				if (i == len - 1){
					css_element.push_back(temp);
				}
			}
			else{
				if (temp != ""){
					css_element.push_back(temp);
				}
				temp = "";
			}
		}
		return css_element;
	}

	std::set<std::string> get_nonterm_first(std::set<std::string> css, std::map<std::string, std::set<std::string>> G, std::map<std::string, std::set<std::string>> nonterm_first, std::map<node, std::set<std::string>>& css_first_map, std::string nonterm){
		std::set<std::string> first_set;//ÿһ�����ս������Ӧ���׷���
		for (auto it : css){//����A����Ӧ��ÿһ������ʽ���б���
			node node_temp(nonterm, it);
			std::vector<std::string> csselems = get_csselement(it);
			std::set<std::string> css_first;//ÿһ������ʽ����Ӧ��first
			if (G[csselems[0]].empty()){//��������׷����Ƿ��ս�����߲��ǿռ�
				first_set.insert(csselems[0]);
				css_first.insert(csselems[0]);
			}
			else{
				int len = csselems.size();//����Ӧ����ʽ�ĳ���
				int i = 0;
				for (; i < len; i++){//ɨ�����ʽ�е�ÿһ��Ԫ��
					if (G[csselems[i]].empty()){
						first_set.insert(csselems[i]);
						css_first.insert(csselems[i]);
						break;
					}
					else{
						std::set<std::string> temp;
						temp = nonterm_first[csselems[i]];
						if (temp.count("#")){//��������˿ռ� �������һ�����
							for (auto it : temp){
								if (it != "#"){
									first_set.insert(it);
									css_first.insert(it);
								}
							}
						}
						else{
							for (auto it : temp){
								first_set.insert(it);
								css_first.insert(it);
							}
							break;
						}
					}
				}
				if (i == len){
					first_set.insert("#");
					css_first.insert("#");
				}
			}
			css_first_map[node_temp] = css_first;
		}
		return first_set;
	}

	std::map<std::string, std::set<std::string>> get_first_set(std::map<std::string, std::set<std::string>> G, std::map<node, std::set<std::string>>& css_first_map){
		std::map<std::string, std::set<std::string>> nonterm_first;
		for (auto it : G){//for nonterminal A do first(A):={}
			std::set<std::string> first_set;
			nonterm_first[it.first] = first_set;
		}
		bool change;
		do{
			change = false;//�������һ�����ս����first�������ı�Ļ���changeΪtrue
			for (auto i : nonterm_first){//����ÿһ���ս��
				std::set<std::string> css = G[i.first];//����ÿһ���ս������Ӧ�Ĳ���ʽ
				int flag = nonterm_first[i.first].size();
				std::set<std::string> first = get_nonterm_first(css, G, nonterm_first, css_first_map, i.first);
				if (first.size() != flag){
					nonterm_first[i.first] = first;
					change = true;
				}
			}
		} while (change);
		return nonterm_first;
	}

	bool get_nonterm_follow(std::string css, std::map<std::string, std::set<std::string>> first_set, std::map<std::string, std::set<std::string>>& follow_set, std::string nonterm){
		bool change = false;
		std::vector<std::string> csselems = get_csselement(css);
		int len = csselems.size();
		for (int i = 0; i < len; i++){
			if (!first_set[csselems[i]].empty()){//������Ƿ��ս���� ��follow�� ����û��
				int flag = follow_set[csselems[i]].size();
				//set<string> follow_temp;
				if (i == len - 1){//������ս����ĩβ ������follow��
					for (auto it : follow_set[nonterm]){
						follow_set[csselems[i]].insert(it);
					}
				}
				else{//������Ƿ��ս�� ������follow ��������һ��Ԫ�ص�first����
					int j = i + 1;
					for (; j < len; j++){
						if (first_set[csselems[j]].empty()){//����������ս�� �������follow���� ��ֹͣ����
							follow_set[csselems[i]].insert(csselems[j]);
							break;
						}
						else{
							std::set<std::string> temp = first_set[csselems[j]];
							if (temp.count("#")){//�������ķ��ս��first����#,ȥ��#��������Ԫ��
								for (auto it : temp){
									if (it != "#"){
										follow_set[csselems[i]].insert(it);
									}
								}
							}
							else{
								for (auto it : temp){
									follow_set[csselems[i]].insert(it);
								}
								break;
							}
						}
					}
					if (j == len){
						for (auto it : follow_set[nonterm]){
							follow_set[csselems[i]].insert(it);
						}
					}
				}
				if (follow_set[csselems[i]].size() != flag){
					change = true;
				}
			}
		}
		return change;
	}

	std::map<std::string, std::set<std::string>> get_follow_set(std::map<std::string, std::set<std::string>> G, std::map<std::string, std::set<std::string>> first_set){
		std::map<std::string, std::set<std::string>> follow_set;
		for (auto it : G){//��ÿ�����ս��һ��follow����
			std::set<std::string> temp;
			follow_set[it.first] = temp;
		}
		follow_set["statement"].insert("$");
		//�ӵ�һ�����ս������Ӧ�Ĳ���ʽ��ʼɨ
		bool change;
		do{
			change = false;
			for (auto i : G){
				for (auto j : G[i.first]){//���ս��i����Ӧ��j����ʽ
					bool flag = get_nonterm_follow(j, first_set, follow_set, i.first);
					if (flag){
						change = true;
					}
				}
			}
		} while (change);
		return follow_set;
	}

	void get_LL1_predict_table(std::map<std::string, std::map<std::string, int>>& predict_table, std::map<node, std::set<std::string>> css_first_map, std::map<std::string, std::set<std::string>> follow_set, std::map<node, int> css_num_map){

		//ɨ��ÿһ������ʽ�ɲ���ʽ����Ӧ��first�� �õ�Ԥ���
		for (auto i : css_first_map){
			node temp(i.first.nonterm, i.first.css);
			int num = css_num_map[temp];//�õ���ʱ����ʽ�ı��
			if (i.second.count("#")){//�������������ʽ�д��ڿռ����ҵ� nonterm ����Ӧ��follow ���� ��follow��������Ӧ�Ĳ���ʽ��д������ʽ�ı��
				std::set<std::string> nonterm_follow = follow_set[i.first.nonterm];
				std::map<std::string, int> temp;
				for (auto it : nonterm_follow){
					predict_table[i.first.nonterm][it] = num;
				}
			}
			for (auto j : i.second){
				if (j != "#"){
					predict_table[i.first.nonterm][j] = num;
				}
			}
		}
	}

public:

	void get_table(std::map<std::string, std::map<std::string, int>>& predict_table,std::map<node,int>& css_num_map,std::map<int,node>& num_css_map){
		std::map<std::string, std::set<std::string>> G;
		remove_left r;
		G = r.rlr();
		std::map<node, std::set<std::string>> css_first_map;
		int num = 0;//��ʾ����ʽ�����
		for (auto i : G){
			for (auto j : i.second){
				num++;//�õ�һ������ʽ
				node temp(i.first, j);
				css_num_map[temp] = num;
				num_css_map[num] = temp;
				std::set<std::string> tmp;
				css_first_map[temp] = tmp;
			}
		}
		std::set<std::string> nonterm;
		std::set<std::string> term;
		term.insert("$");
		for (auto i : G){
			nonterm.insert(i.first);
			for (auto j : i.second){
				std::vector<std::string> css = get_csselement(j);
				int len = css.size();
				for (int k = 0; k < len; k++){
					if (G.count(css[k]) == 0 && css[k] != "#"){
						term.insert(css[k]);
					}
				}
			}
		}
		std::map<std::string, int> column;
		for (auto j : term){
			column[j] = 0;
		}
		for (auto i : nonterm){
			predict_table[i] = column;
		}
		std::map<std::string, std::set<std::string>> n = get_first_set(G, css_first_map);
		std::map<std::string, std::set<std::string>> m = get_follow_set(G, n);
		get_LL1_predict_table(predict_table, css_first_map, m, css_num_map);
	}

};

#endif