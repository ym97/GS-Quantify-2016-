#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include <string>
#include<string.h>
#include <sstream>
#include <vector>

using namespace std;

struct _job{
    unsigned long proc_id;
    unsigned long timestamp;
    string orig;
    string instr;
    int imp ;
    unsigned int duration;
    unsigned long assigntime;
};
typedef struct _job job;

struct _cpu{
	unsigned long start_time;
    unsigned int duration;
};
typedef struct _cpu cpu;

vector<cpu> myCpu;


struct _node{
	
    job* jobassigned;
    struct _node* next;
};
typedef struct _node node;

void splitt(const string &s, char delim, vector<string> &elems) {
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
//	cout<<"splitty";
    }
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    splitt(s, delim, elems);
    return elems;
}
  
int freecpus(unsigned long curtime){
	int count =0;
	for(int i=0;i<myCpu.size();i++){
		cpu ttemp = myCpu.at(i);
		
		if(ttemp.duration==0) count++;
		if(curtime-ttemp.start_time==ttemp.duration){
			ttemp.start_time=0;
			ttemp.duration=0;
			count++;
		
		}
	}
	return count;
}


// function to compare priority of 2 jobs given their pointers
// return 1 is job A is prior to Job B else -1
int isgreater(job* a,job* b ){
	if(a->imp > b->imp)
		{return 1;}
	else{
		if(a->imp==b->imp) {
				if(a->timestamp < b->timestamp) {
				return 1;}
				else if (a->timestamp == b->timestamp ) {
					if(a->duration < b->duration) return 1;
				}
		}
	}
	return -1;
}
	
// new node being created

node* newNode(string a,string b,string c,string d,string e, string f){
	
	 	 job* tjob = new job();
	// 	 tjob = (job*)malloc(sizeof(job));
	 	 tjob->timestamp = atol(f.c_str());
		 tjob->proc_id = atol(e.c_str());
	 	 tjob->orig = d;
	 	 tjob->instr =c;
	 	 tjob->imp =atoi(b.c_str()) ;
	 	 tjob->duration=atol(a.c_str() );
		 tjob->assigntime = 0;
		  
		 node* tempor = new node();		 
		 tempor->jobassigned = tjob ;
		 tempor->next=NULL;
		 
		 return tempor;
}	

node* sortedInsert(node* stackptr,node* new_node)
{
    node* current;
    /* Special case for the head end */
    if (stackpter == NULL || isgreater(new_node->jobassigned,stackptr->jobassigned))
    {
        new_node->next = stackptr;
        stackptr = new_node;
    }
    else
    {
        /* Locate the node before the point of insertion */
        current = stackptr;
        while (current->next!=NULL &&
               (isgreater(current->next->jobassigned,new_node->jobassigned)==1))
        {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
    return stackptr;
}




//function to test whether a string has only numeric charaters --- true if yes, false if no
bool is_digits(const string &str)
{
    return str.find_first_not_of("0123456789") == string::npos;
}

//query of type 1
void query1(node *headptr, unsigned long timestamp, unsigned long k) {
   int count = 0;
   node *traverse = headptr;
   if (traverse == NULL) return;
   while (count < k) {
        if ((traverse -> jobassigned)->timestamp > timestamp) {
		traverse = traverse -> next;
		continue;
	}
	else if (((traverse->jobassigned)->assigntime > timestamp) || ((traverse->jobassigned)->assigntime == 0)) {
	  cout << "job " << (traverse->jobassigned)->timestamp << " " << (traverse->jobassigned)->proc_id << " " << (traverse->jobassigned)->orig << " " << (traverse->jobassigned)->instr << " " << (traverse->jobassigned)->imp << " " << (traverse->jobassigned)->duration << "\n";
	  count++;
	} 
        if (traverse -> next == NULL) return;
        traverse = traverse -> next;
   }
  
}
	
void query2(node *headptr, unsigned long timestamp, string orig) {
   node *traverse = headptr;
  
   while (traverse != NULL) {
       if ((traverse -> jobassigned)->timestamp > timestamp) {
		traverse = traverse -> next;
		continue;
	}
       else if (((traverse->jobassigned)->assigntime > timestamp) || ((traverse->jobassigned)->assigntime == 0)) {
	  if (!strcmp((traverse->jobassigned)->orig.c_str(),orig.c_str())) {
		cout << "job " << (traverse->jobassigned)->timestamp << " " << (traverse->jobassigned)->proc_id << " " << (traverse->jobassigned)->orig << " " << (traverse->jobassigned)->instr << " " << (traverse->jobassigned)->imp << " " << (traverse->jobassigned)->duration << "\n";
	  }
	}
        if (traverse -> next == NULL) return;
	traverse = traverse->next;
   }
}

	
int main()
{
   int no_cpu=0 , k;
   string line;
   node* stackptr ;
   node* headptr;
   	unsigned long currentime;
	 stackptr=NULL;
    headptr=NULL;
    node* head ;
    head=NULL;
   while (getline(cin, line)) {
  
	 //process the line read from the user
   	 vector<string> parts;
   	 splitt(line,' ',parts);
	 
	 if (parts[0]=="job"){
	 	
	 	// insertion in to the priority
	     node* newn = newNode(parts[1],parts[2],parts[3],parts[4], parts[5],parts[6]);
		 if(headptr==NULL){
		 	stackptr= sortedInsert(stackptr , newn);
		    headptr = stackptr;
		 }
		 else{
		 	stackptr= sortedInsert(stackptr, newn);
		 }	   	 	
		
	 }
	 else if (parts[0]=="assign"){
	 	
	 	currentime = atol(parts[1].c_str()) ; 	 	
        k = atoi(parts[2].c_str()) ;
        int free = freecpus(currentime);
        if(free<k)k = free ;
        
        // k has the min of k free
         int nextfree=0;
         
         while(stackptr != NULL && k>0){
         
		    while(nextfree < no_cpu){
         	 cpu tempp = myCpu.at(nextfree);
			 if(tempp.duration == 0){
			 	 break;
			 }
		    }	
         	 cpu te = myCpu.at(nextfree);
			 te.start_time = stackptr->jobassigned->timestamp;
			 te.duration = stackptr->jobassigned->duration;
		      cout << "job " << (stackptr->jobassigned)->timestamp << " " << (stackptr->jobassigned)->proc_id << " " << (stackptr->jobassigned)->orig << " " << (stackptr->jobassigned)->instr << " " << (stackptr->jobassigned)->imp << " " << (stackptr->jobassigned)->duration <<"\n";
			 stackptr = stackptr->next;
         	 k--;
         	 
		 }
       
 	 }
	 else if(parts[0]=="query"){
	 	 unsigned long timestamp = atol(parts[1].c_str());
	    if (is_digits(parts[2])) {//query of type 1
	    	unsigned long k = atol(parts[2].c_str());
		query1(headptr, timestamp, k);

	    } else { //query of type 2
		query2(headptr, timestamp, parts[2]);
            }
       
	 }
	 else {
   	  no_cpu =atoi(parts[1].c_str() ) ; 	 	
          
	   // initialising the cpu array
	    for(int i=0;i<no_cpu;i++){
	    	cpu temp;
			temp.duration=0;
	    	temp.start_time=0;
	    	myCpu.push_back(temp);
		}
     }
	  
	 parts.clear();
	 line = ' ';
  }  
   return 0;
}

