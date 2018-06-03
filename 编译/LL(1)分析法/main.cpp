#include"RLR.h"
#include"fileiostream.h"
#include"LLT.h"
#include<stack>
using namespace std;

vector<string> get_csselement(string str){
	vector<string> css_element;
	int len = str.length();
	string temp = "";
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

void parsing_code(string token){
	map<node, int> css_num_map;
	map<int, node> num_css_map;
	map<string, map<string, int>> predict_table;
	Predict_Table p;
	p.get_table(predict_table, css_num_map, num_css_map);
	stack<string> parse_stack;//����ջ
	parse_stack.push("$");
	parse_stack.push("statement");
	vector<string> tokens = get_csselement(token);
	tokens.push_back("$");
	int i = 0;
	int len = tokens.size();
	while (parse_stack.top()!="$"&&tokens[i]!="$"){
		if (parse_stack.top() == tokens[i]){//����ջ�͵�������ƥ��
			parse_stack.pop();
			i++;
		}
		else{
			if (predict_table.count(parse_stack.top()) == 0){//�������ջ�ײ��Ƿ��ս��֤������
				cout <<"ERROR:"<< token[i] << "  " << i << endl;
				return;//������������������������
			}
			else{//�������ջ���Ƿ��ս��
				int num = predict_table[parse_stack.top()][tokens[i]];
				if (num != 0){
					node temp = num_css_map[num];
					string css = temp.css;
					vector<string> css_elems = get_csselement(css);
					if (css_elems[0]=="#"){
						parse_stack.pop();
					}
					else{
						int len2 = css_elems.size();
						parse_stack.pop();
						for (int j = len2 - 1; j >= 0; j--){//��������ѹջ
							string temp = css_elems[j];
							parse_stack.push(temp);
						}
					}
				}
				else{
					cout << "ERROR:" << token[i] << "  " << i << endl;
					return;//������������������������
				}
			}
		}
	}
	int num = predict_table[parse_stack.top()][tokens[i]];
	node temp = num_css_map[num];
	string css = temp.css;
	vector<string> css_elems = get_csselement(css);
	if (css_elems[0] == "#"){
		parse_stack.pop();
	}
	if (parse_stack.top() == "$"&&tokens[i] == "$"){
		cout << "accept" << endl;
	}
	else{
		cout << "error" << endl;
	}
}

int main(){
	string token = "if ( 0 ) if ( 1 ) other else other";
	parsing_code(token);
	system("pause");
	return 0;
}