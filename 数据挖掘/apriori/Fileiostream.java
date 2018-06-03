package apriori;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.TreeSet;

public class Fileiostream {
	private ArrayList<TreeSet<Integer>> data_set=new ArrayList<TreeSet<Integer>>();
    private int set_num=0;
    
    public int getSet_num() {
		return set_num;
	}
	
	public ArrayList<TreeSet<Integer>> getData_set() {
		return data_set;
	}


	public Fileiostream(String pathname){
    	try {
			BufferedReader br=new BufferedReader(new FileReader(pathname));
			String str="";
			try {
				while((str=br.readLine())!=null){//���ж�ȡԴ�ļ�
					set_num++;
					String[] str2=str.split(" ");//�Կո�ָ�ÿ������
					int len=str2.length;
					TreeSet<Integer> temp=new TreeSet<Integer>();
					for(int i=0;i<len;i++){
						temp.add(Integer.parseInt(str2[i]));//���ַ���ת��Ϊ����
					}
					data_set.add(temp);
				}
			} catch (IOException e) {
				// TODO �Զ����ɵ� catch ��
				e.printStackTrace();
			}
		} catch (FileNotFoundException e) {
			// TODO �Զ����ɵ� catch ��
			e.printStackTrace();
		}
    }
}
