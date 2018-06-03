package lr;

import java.util.TreeSet;

public class NFA_node implements Comparable<NFA_node>{

	Integer state;
	String edge;
	 
	public NFA_node(){
		state=-1;
		edge="";
	}
	
	public NFA_node(String e,Integer n){
		state=n;
		edge=e;
	}

	@Override
	public int compareTo(NFA_node o) {
		// TODO �Զ����ɵķ������
		if(!this.edge.equals(o.edge)){
			return this.edge.compareTo(o.edge);
		}
		else{
			return this.state-o.state;
		}
	}
	
}
