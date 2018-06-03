package lr;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.TreeSet;

public class Lr_table {
	
	private HashMap<Integer,TreeSet<NFA_node>> nfa_graph;//��������״̬ͼ
	private HashMap<Integer, ArrayList<String>> num_css;//����ʽ
	private HashMap<Integer,TreeSet<Integer>> state_css;//ÿ��״̬�������Ĳ���ʽ
	private HashMap<Integer,TreeSet<NFA_node>> action_table;//lr������
	private HashMap<Integer,TreeSet<NFA_node>> goto_table;
	private TreeSet<Integer> final_state;
	private TreeSet<String> nonterm_set;
	private TreeSet<String> term_set;
	
	public HashMap<Integer, ArrayList<String>> getNum_css() {
		return num_css;
	}

	public HashMap<Integer, TreeSet<NFA_node>> getAction_table() {
		return action_table;
	}

	public HashMap<Integer, TreeSet<NFA_node>> getGoto_table() {
		return goto_table;
	}

	public TreeSet<Integer> getFinal_state() {
		return final_state;
	}

	public Lr_table(){
		NFA nfa=new NFA();
		state_css=nfa.getState_css();
		nfa_graph=nfa.getNfa_graph();
		num_css=nfa.getNum_new_css();
		final_state=new TreeSet<Integer>();
		nonterm_set=nfa.fis.getNonterm_set();
		term_set=nfa.fis.getTerm_set();
		action_table=new HashMap<Integer,TreeSet<NFA_node>>();
		goto_table=new HashMap<Integer,TreeSet<NFA_node>>();
		
		//����״̬�� �����״̬������nfa��key��������̬ ����ʽ��̬
		for(Integer i:state_css.keySet()){
			if(nfa_graph.containsKey(i)){
				//�����״̬������̬ ������״̬�ӽ�ȥ
				action_table.put(i,new TreeSet<NFA_node>());
				goto_table.put(i,new TreeSet<NFA_node>());
				for(NFA_node j:nfa_graph.get(i)){
					if(term_set.contains(j.edge)){
						action_table.get(i).add(j);
					}
					else{
						goto_table.get(i).add(j);
					}
				}
			}
			else{
				action_table.put(i,new TreeSet<NFA_node>());
				final_state.add(i);
				for(String j:term_set){
					for(Integer k:state_css.get(i)){//�ҵ���״̬����Ӧ�Ĳ���ʽ
						//if(!num_css.get(k).get(0).equals("start")){
							NFA_node temp=new NFA_node(j,k);
							action_table.get(i).add(temp);
						//}
					}
				}
				for(Integer k:state_css.get(i)){//�ҵ���״̬����Ӧ�Ĳ���ʽ
						NFA_node temp=new NFA_node("$",k);
						action_table.get(i).add(temp);
						action_table.get(i).add(temp);
				}
			}
		}
		//print();
	}
	
	public void print(){
		System.out.println(term_set);
		for(Integer i:action_table.keySet()){
			System.out.print(i+"\t");
			for(NFA_node j:action_table.get(i)){
				System.out.print(j.edge+"\t"+j.state+"\t");
			}
			System.out.println(" ");
		}
		
		System.out.println("----------");
		for(Integer i:goto_table.keySet()){
			System.out.print(i+"\t");
			for(NFA_node j:goto_table.get(i)){
				System.out.print(j.edge+"\t"+j.state+"\t");
			}
			System.out.println(" ");
		}
		
	}
	
	/*public static void main(String[] args){
		new Lr_table();
	}*/
	
}
