package com.servlet;  
  
import java.io.IOException;  
import java.sql.Connection;  
import java.sql.DriverManager;  
import java.sql.PreparedStatement;  
import java.sql.ResultSet;  
import java.sql.SQLException;  
  

import java.util.ArrayList;

import javax.servlet.ServletException;  
import javax.servlet.http.HttpServlet;  
import javax.servlet.http.HttpServletRequest;  
import javax.servlet.http.HttpServletResponse;  

import com.bean.Kucuntongji;
  
public class KucuntongjiServlet extends HttpServlet {  
      
     protected void doGet(HttpServletRequest request,HttpServletResponse response) throws ServletException, IOException {    
        //��һ�����Ȱ��ַ����趨��utf-8��������һ����������������ľͻ��������  
        request.setCharacterEncoding("utf-8");     
        
        try {  
        	Class.forName("com.microsoft.sqlserver.jdbc.SQLServerDriver");
            String dbURL="jdbc:sqlserver://localhost:1433;DatabaseName=company";
            String username="sa";
            String password="19970710"; 
			Connection dbConn=null;
			
			dbConn= DriverManager.getConnection(dbURL,username,password);
			PreparedStatement ps;
			String sql="select crinfo.crdate d,crinfo.pno p,product.pname n from crinfo,product where crinfo.pno=product.pno group by crinfo.crdate,crinfo.pno,product.pname";
			ps=dbConn.prepareStatement(sql);
           
            ResultSet rs=ps.executeQuery();  
            ArrayList<Kucuntongji> kinfo_list=new ArrayList<Kucuntongji>();
            while(rs.next()){
            	Kucuntongji p=new Kucuntongji();
            	p.setPno(rs.getString("p"));
            	p.setCrdate(rs.getDate("d"));
            	p.setPname(rs.getString("n"));
            	kinfo_list.add(p);
            }
            request.setAttribute("result", kinfo_list);
            
            request.getRequestDispatcher("listkucuntongji.jsp").forward(request, response);
            ps.close();
            dbConn.close();
        } catch (SQLException e) {  
            e.printStackTrace();  
        } catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}    
    }    
    
    protected void doPost(HttpServletRequest request,    
            HttpServletResponse response) throws ServletException, IOException {    
        doGet(request, response);    
    }    
}

