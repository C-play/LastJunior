#ifndef RLR_H
#define RLR_H
#include<string>
#include<map>
#include<set>
#include<vector>
#include<queue>
#include"fileiostream.h"

class remove_left{
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

	std::string get_substr_left_recursion(std::vector<std::string> elems){
		std::string new_chanshengshi;
		elems.erase(elems.begin());
		for (auto it : elems){
			std::string temp = it;
			temp += " ";
			new_chanshengshi += temp;
		}
		return new_chanshengshi;
	}

	void remove_immediate_left_recursion(std::string nonterminal, std::map<std::string, std::set<std::string>>& G){
		std::set<std::string> chanshengshi = G[nonterminal];
		std::map<std::string, std::vector<std::string>> css_element;//ÿ������ʽ������Ӧ��Ԫ��
		std::set<std::string> left_recursion;//�������ս������ݹ��ķ�����ʽ
		std::set<std::string> no_left_recursion;//���������ս������ݹ��ķ�����ʽ
		for (std::string it : chanshengshi){
			css_element[it] = get_csselement(it);
			if (get_csselement(it)[0] == nonterminal){
				left_recursion.insert(it);
			}
			else{
				no_left_recursion.insert(it);
			}
		}
		if (left_recursion.empty()){//�����������ݹ�ʽ�򷵻ؿ�
			return;
		}
		else{//���������ݹ���Ҫ����a'�Ĳ���ʽ ���Ҹı�a��map�е����
			std::string nonterminal2 = nonterminal;
			nonterminal2 += "'";
			std::set<std::string> set1;//��Ŧ�A'
			for (std::string it : no_left_recursion){//������ʽ����������nonterminal2
				std::string temp = it;
				temp += " ";
				temp += nonterminal2;
				set1.insert(temp);
			}//�ı�A�µĲ���ʽ
			G[nonterminal] = set1;
			std::set<std::string> set2;
			set2.insert("#");//��#��ʾ�ռ�
			for (std::string it : left_recursion){//����A��ʽ�õ�A'����Ӧ�Ĳ���ʽ
				std::string temp = get_substr_left_recursion(css_element[it]);
				temp += nonterminal2;
				set2.insert(temp);
			}
			G[nonterminal2] = set2;
		}
	}

	void change_css(std::string css, std::set<std::string> change_elem_css, std::set<std::string>& new_set){
		std::vector<std::string> css_vector = get_csselement(css);
		css_vector.erase(css_vector.begin());
		std::string temp = " ";
		for (auto it : css_vector){
			temp += it;
			temp += " ";//�Կո�ָ�ÿһ������ʽ�е�Ԫ��
		}
		for (auto it : change_elem_css){
			std::string tmp = it;
			tmp += temp;
			new_set.insert(tmp);
		}
	}
	void replace_grammar_rule(std::string nonterminal, std::map<std::string, std::set<std::string>>& G, std::string change_elem){
		std::set<std::string> css = G[nonterminal];//�õ����еĲ���ʽ
		std::set<std::string> need_change_css;
		std::set<std::string> no_need_change_css;
		std::map<std::string, std::vector<std::string>> css_elems;
		for (auto it : css){//����ĳ�����ս�����в���ʽ  �ҵ��׷� ��num_nonterminal ��ͬ�Ĳ���ʽ
			css_elems[it] = get_csselement(it);
			if (css_elems[it][0] == change_elem){//����׷�����Ҫ�ı�ķ��ս����ͬ����Ҫ�滻
				need_change_css.insert(it);
			}
			else{
				no_need_change_css.insert(it);
			}
		}
		if (need_change_css.empty()){//���û����Ҫ�ı�Ĳ���ʽ�򷵻�
			return;
		}
		std::set<std::string> new_set;//���ݵõ����������ϸı���Ӧ�Ĳ���ʽ
		for (auto it : css){
			if (need_change_css.count(it)){//�����Ҫ�ı���ı���Ӧ���ʽ
				change_css(it, G[change_elem], new_set);
			}
			else{//����������ʽ����Ҫ�ı���ֱ�Ӽ�����
				new_set.insert(it);
			}
		}
		G[nonterminal] = new_set;
	}

	void general_left_recursion(std::map<std::string, std::set<std::string>>& G){
		std::map<int, std::string> num_nonterm;
		std::map<std::string, int> nonterm_num;
		int m = 0;//��������ʽ �õ����з��ս���ĸ���
		for (auto it : G){
			m++;
			num_nonterm[m] = it.first;//�õ����з��ս������Ӧ�����
			nonterm_num[it.first] = m;
		}
		for (int i = 1; i <= m; i++){
			for (int j = 1; j <= i - 1; j++){//�滻���о��׵ķ��ս��
				replace_grammar_rule(num_nonterm[i], G, num_nonterm[j]);
			}
			remove_immediate_left_recursion(num_nonterm[i], G);
		}
	}

	bool is_nonterm(std::string css, std::map<std::string, std::set<std::string>> G){
		std::vector<std::string> csselems = get_csselement(css);
		if (G[csselems[0]].empty()){//������ķ��в����ڲ���ʽ�����ս��
			return false;
		}
		else{
			return true;
		}
	}

	int get_min_length(std::set<std::string> common_css){
		int min = 99999999;
		int len;
		for (auto it : common_css){
			len = get_csselement(it).size();
			if (len < min){
				min = len;
			}
		}
		return min;
	}


	void get_new_css(std::set<std::string>& css, std::queue<std::string>& Q, std::string now_css, std::map<std::string, std::set<std::string>> G){//�õ��µĲ���ʽ�����µĲ���ʽ���������
		std::vector<std::string> csselems = get_csselement(now_css);
		std::set<std::string> other_css = G[csselems[0]];
		csselems.erase(csselems.begin());
		std::string temp = " ";
		for (auto it : csselems){//�õ�ȥ���׷���Ĳ���ʽ
			temp += it;
			temp += " ";
		}
		for (auto it : other_css){//�õ��µĲ���ʽ
			std::string tmp = it;
			tmp += temp;
			css.insert(tmp);
			Q.push(tmp);//���²����Ĳ���ʽ���������
		}
		css.erase(now_css);//ȥ��ԭ���Ĳ���ʽ
	}

	std::string partition_css(std::string last, std::string css){
		std::string substring = "";
		std::vector<std::string> last_elems = get_csselement(last);
		std::vector<std::string> css_elems = get_csselement(css);
		int len1 = last_elems.size();
		int i = 0;
		for (; i < len1; i++){
			if (last_elems[i] != css_elems[i]){
				break;
			}
		}
		int len2 = css_elems.size();
		for (int j = i; j < len2; j++){
			substring += css_elems[j];
			substring += " ";
		}
		return substring;
	}

	bool get_max_length(std::map<std::string, std::set<std::string>>& G, std::string nonterm, std::queue<std::string>& Q1){//�õ�����ƥ�䳤��
		std::set<std::string> css = G[nonterm];
		//�Բ���ʽ����һ��ɨ�轫�������к����ս���׷���ʽ�ӻ�Ϊ�ս��
		std::queue<std::string> Q2;
		for (auto it : css){
			Q2.push(it);
		}
		while (!Q2.empty()){
			std::string now_css = Q2.front();//��ǰ������Ĳ���ʽ
			if (!is_nonterm(now_css, G)){//��������׷����ս�������滻
				Q2.pop();
			}
			else{
				get_new_css(css, Q2, now_css, G);
				Q2.pop();//�µĲ���ʽ�õ��ˣ�Ҳ�����Ÿ�������ʽ������
			}
		}
		G[nonterm] = css;//G����Ӧ�Ĳ���ʽ�����˸ı�
		//�ڸı����ʽ������£��ҵ�������Ӧ����Ӵ�
		std::map<std::string, std::vector<std::string>> css_elems;//����ʽ�е�Ԫ��
		for (auto it : css){//�������׷���ͬ�Ĵ�
			css_elems[it] = get_csselement(it);
		}
		std::map<std::string, std::set<std::string>> common_map;
		std::string key;
		int max = 0;
		for (auto i : css){
			for (auto j : css){
				if (css_elems[i][0] == css_elems[j][0]){
					common_map[i].insert(j);//����׷���ͬ�����һ��������
					if (common_map[i].size() > max){
						key = i;
						max = common_map[i].size();
					}
				}
			}
		}
		if (max == 1){//����׷�������ͬ ���������κθı�
			return false;
		}
		//���������ͬ���׷��� �ı� ����ʽ �ҵ���ͬ���������ƥ���Ӵ�
		std::set<std::string> common_css = common_map[key];
		std::string last = " ";
		int index = 0;
		bool flag = true;
		int len = get_min_length(common_css);
		while (flag){
			std::string temp = css_elems[key][index];
			for (auto it : common_css){// �����һ���Ӵ�����ͬ ��������ǰѭ��
				if (css_elems[it][index] != temp){
					flag = false;
				}
			}
			if (flag){
				last += " ";
				last += css_elems[key][index];
				index++;
			}
			if (index == len){
				flag = false;
			}
		}
		//�ı����ʽ ����ͬ�������Ӻϲ�
		std::string temp = nonterm;
		temp += "'";
		while (!G[temp].empty()){
			temp += "'";
		}
		std::set<std::string> new_css2;
		std::set<std::string> new_css3;
		for (auto it : css){
			if (!common_css.count(it)){//�����������ͬ�ļ����г��� ��ֱ�ӽ������nonterm���µĲ���ʽ��
				new_css2.insert(it);
			}
			else{
				std::string temp2 = last;
				temp2 += " ";
				temp2 += temp;
				new_css2.insert(temp2);
				std::string temp3 = partition_css(last, it);
				if (temp3 == ""){
					temp3 += "#";
				}
				temp3 += " ";
				new_css3.insert(temp3);
			}
		}
		G[nonterm] = new_css2;
		G[temp] = new_css3;
		Q1.push(temp);
		return true;
	}

	void get_left_factoring(std::map<std::string, std::set<std::string>>& G){
		std::queue<std::string> Q1;//����һ��map �����еķ��ս�����������
		for (auto it : G){
			Q1.push(it.first);
		}
		while (!Q1.empty()){//�����зǿ�ʱ �ҳ����ս������Ӧ�Ĳ���ʽ
			std::string now_nt = Q1.front();//��ǰ���ս��
			bool change = true;
			while (change){
				bool tmp = get_max_length(G, now_nt, Q1);
				if (!tmp){
					change = false;
				}
			}
			Q1.pop();
		}
	}
public:
	std::map<std::string, std::set<std::string>> rlr(){
		std::map<std::string, std::set<std::string>> G;
		Fileiostream filestream;
		filestream.fileiostream(G);
		general_left_recursion(G);
		get_left_factoring(G);
		return G;
	}
};


#endif