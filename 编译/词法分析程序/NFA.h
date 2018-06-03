#ifndef NFA_H
#define NFA_H

#include<iostream>
#include<string>
#include<stack>
#include"Graph.h"

struct node{
	int begin;
	int end;
	std::string edge;
	node(std::string s, int b = -1, int e = -1){
		edge = s;
		begin = b;
		end = e;
	}
	node(int b, int e, std::string s = "$"){
		begin = b;
		end = e;
	}
};

class NFA{
private:
	bool Iszimu(char s){//�ж��Ƿ�Ϊ��ĸ
		if (s >= 'a'&&s <= 'z')
			return true;//����ĸ�����棬���򷵻ؼ�
		else
			return false;
	}
	std::string get_substr(int& i, std::string str){//�õ�����������ƥ��������֮��������ַ�
		int k = 1;//k���������ż�һ�����������ż�һ
		int len = str.length();
		std::string substr;
		i++;
		for (int j = i; j < len; j++){
			if (str[j] == '('){
				k++;
				substr += str[j];
				i++;
			}
			else if (str[j] == ')'){
				k--;
				substr += str[j];
				i++;
			}
			else{
				substr += str[j];
				i++;
			}
			if (k == 0){
				break;
			}
		}
		i--;//����k=0ʱ��i����һ���Ҫ��ȥһ
		substr.erase(substr.end() - 1);//����k=0ʱ���ַ��������һ���ַ�
		return substr;
	}
	std::string get_point(int& i, std::string str){
		std::string str2 = "";
		int len = str.length();
		for (; i < len; i++){
			if ((str[i] >= 'a'&&str[i] <= 'z') || (str[i] >= 'A'&&str[i] <= 'Z')){
				str2 += str[i];
			}
			else{
				break;
			}
		}
		i--;
		return str2;
	}
	int ope_level(char s){//�������������ȼ�
		if (s == '*') return 4;
		if (s == '-') return 3;
		if (s == '|') return 2;
	}
	void Bibao_operation(int& num, std::stack<node>& point, Graph& graph){//�հ�����
		node cur = point.top();//���ڱհ��ǵ�Ŀ��������ֻȡһ��������
		point.pop();
		int begin1, end1, begin, end;
		begin1 = cur.begin;//�������Ŀ�ʼ���
		end1 = cur.end;//���������յ�
		if (begin1 == -1){//���������û����ͼ�У�����Ҫ������������ͼ��
			num++;
			begin1 = num;
			num++;
			end1 = num;
			graph.insert(begin1, end1, cur.edge);//������������ͼ��
			graph.insert(end1, begin1);
			num++;
			begin = num;//�հ�����������
			graph.insert(begin, begin1);//����������������һ���ձ�
			num++;
			end = num;//�հ���������յ�
			graph.insert(end1, end);//�յ�����������յ��һ���ձ�
			graph.insert(begin, end);//���������������������յ��һ���ձ�
			node temp(begin, end);
			point.push(temp);//���հ��������������յ�ѹ�������ջ��
		}
		else{
			graph.insert(end1, begin1);
			num++;
			begin = num;
			graph.insert(begin, begin1);
			num++;
			end = num;
			graph.insert(end1, end);
			graph.insert(begin, end);
			node temp(begin, end);
			point.push(temp);
		}
	}
	void Lianjie_operation(int& num, std::stack<node>& point, Graph& graph){//���Ӳ���
		node right = point.top();//ȡ�����Ӳ������������
		point.pop();
		node left = point.top();//ȡ�����Ӳ������������
		point.pop();
		int begin1, end1, begin2, end2, begin, end;
		begin1 = left.begin;//������������
		end1 = left.end;
		begin2 = right.begin;//�Ҳ����������
		end2 = right.end;
		if (begin1 == -1 && end1 == -1 && begin2 == -1 && end2 == -1){//������Ҳ�������û����ͼ��
			num++;
			begin1 = num;
			num++;
			end1 = num;
			graph.insert(begin1, end1, left.edge);//�������������ͼ��
			begin2 = end1;//�Ҳ����������Ϊ����������յ�
			num++;
			end2 = num;
			graph.insert(begin2, end2, right.edge);//���Ҳ���������ͼ��
			begin = begin1;
			end = end2;
			node temp(begin, end);
			point.push(temp);//�����ѹ��ջ��
		}
		else if (begin1 == -1 && end1 == -1 && begin2 != -1 && end2 != -1){
			num++;
			begin1 = num;
			end1 = begin2;
			graph.insert(begin1, end1, left.edge);
			begin = begin1;
			end = end2;
			node temp(begin, end);
			point.push(temp);
		}
		else if (begin1 != -1 && end1 != -1 && begin2 == -1 && end2 == -1){
			begin2 = end1;
			num++;
			end2 = num;
			graph.insert(begin2, end2, right.edge);
			begin = begin1;
			end = end2;
			node temp(begin, end);
			point.push(temp);
		}
		else{
			begin = begin1;
			end = end2;
			graph.insert(end1, begin2);
			node temp(begin, end);
			point.push(temp);
		}
	}
	void Alter_operation(int& num, std::stack<node>& point, Graph& graph){
		node right = point.top();//ȡ�Ҳ�����
		point.pop();
		node left = point.top();//��������
		point.pop();
		int begin, end, begin1, end1, begin2, end2;
		begin1 = left.begin;
		end1 = left.end;
		begin2 = right.begin;
		end2 = right.end;
		if (begin1 == -1 && begin2 == -1){
			num++;
			begin1 = num;
			num++;
			end1 = num;
			graph.insert(begin1, end1, left.edge);
			num++;
			begin2 = num;
			num++;
			end2 = num;
			graph.insert(begin2, end2, right.edge);
			num++;
			begin = num;//���Ӳ��������
			graph.insert(begin, begin1);
			graph.insert(begin, begin2);
			num++;
			end = num;//���Ӳ������յ�
			graph.insert(end1, end);
			graph.insert(end2, end);
			node temp(begin, end);
			point.push(temp);
		}
		else if (begin1 == -1 && begin2 != -1){
			num++;
			begin1 = num;
			num++;
			end1 = num;
			graph.insert(begin1, end1, left.edge);
			num++;
			begin = num;
			graph.insert(begin, begin1);
			graph.insert(begin, begin2);
			num++;
			end = num;
			graph.insert(end1, end);
			graph.insert(end2, end);
			node temp(begin, end);
			point.push(temp);
		}
		else if (begin1 != -1 && begin2 == -1){
			num++;
			begin2 = num;
			num++;
			end2 = num;
			graph.insert(begin2, end2, right.edge);
			num++;
			begin = num;
			graph.insert(begin, begin1);
			graph.insert(begin, begin2);
			num++;
			end = num;
			graph.insert(end1, end);
			graph.insert(end2, end);
			node temp(begin, end);
			point.push(temp);
		}
		else{
			num++;
			begin = num;
			graph.insert(begin, begin1);
			graph.insert(begin, begin2);
			num++;
			end = num;
			graph.insert(end1, end);
			graph.insert(end2, end);
			node temp(begin, end);
			point.push(temp);
		}
	}
public:
	node NFA_expression(int& num, std::string str, Graph& graph){
		int len = str.length();
		std::stack<node> point;
		std::stack<char> ope;
		for (int i = 0; i < len; i++){//���ȼ�����
			switch (str[i]){
			case '*':
				ope.push('*');
				break;
			case '|'://ѡ������������ȼ�С�ڱհ������ӣ�ѹ�����ջʱҪ����֮ǰ�Ĳ�����ʱ�հ���������
				while (ope.empty() == false && (ope_level('|') < ope_level(ope.top()))){
					if (ope_level(ope.top()) == 4){
						Bibao_operation(num, point, graph);
						ope.pop();
					}
					else{
						Lianjie_operation(num, point, graph);
						ope.pop();
					}
				}
				ope.push('|');
				break;
			case '-': 
				while (ope.empty() == false && (ope_level('-') < ope_level(ope.top()))){
					Bibao_operation(num, point, graph);
					ope.pop();
				}
				ope.push('-');
				break;
			case '('://�������ſ�ʼ�ݹ�
				point.push(NFA_expression(num, get_substr(i, str), graph));
				break;
			default://����������ѹ��ջ��
				node temp(get_point(i,str));
				point.push(temp);
				break;
			}
		}
		while (ope.empty() != true){//ȡ��������ջ�����в���
			switch (ope.top()){
			case '*':
				Bibao_operation(num, point, graph);
				ope.pop();
				break;
			case '-':
				Lianjie_operation(num, point, graph);
				ope.pop();
				break;
			default:
				Alter_operation(num, point, graph);
				ope.pop();
				break;
			}
		}
		return point.top();
	}
	std::string add_lianjie(std::string str1){//������ӷ���
		std::string str2;
		int len = str1.length();
		for (int i = 0; i < len - 1; i++){
			if (Iszimu(str1[i])){
				if (str1[i + 1] == '('){
					str2 += str1[i];
					str2 += '-';
				}
				else
					str2 += str1[i];
			}
			else if (str1[i] == '*'&&Iszimu(str1[i + 1])){
				str2 += str1[i];
				str2 += '-';
			}
			else if (str1[i] == ')'&&Iszimu(str1[i + 1])){
				str2 += str1[i];
				str2 += '-';
			}
			else str2 += str1[i];
		}
		str2 += str1[len - 1];
		return str2;
	}
};

#endif