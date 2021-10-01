	//all required header files
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	#include<termios.h>
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	static struct termios old, new;
 
	// Initialize new terminal i/o settings
	void initTermios(int echo) 
	{
  		tcgetattr(0, &old); //grab old terminal i/o settings
  		new = old; //make new settings same as old settings
  		new.c_lflag &= ~ICANON; //disable buffered i/o
  		new.c_lflag &= echo ? ECHO : ~ECHO; //set echo mode
  		tcsetattr(0, TCSANOW, &new); //apply terminal io settings
	}
 
	// Restore old terminal i/o settings
	void resetTermios(void) 
	{
 		 tcsetattr(0, TCSANOW, &old);
	}
 
	// Read 1 character - echo defines echo mode
	char getch_(int echo) 
	{
 		 char ch;
 		 initTermios(echo);
 		 ch = getchar();
 		 resetTermios();
  		return ch;
	}
 
	//Read 1 character without echo getch() function definition
	char getch(void) 
	{
 		 return getch_(0);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////	
	
	int head_cnt;
	int number;
	int s,r;
	
	//structures
	
	struct list
	{
		int value;
		int state;
	}info;
	FILE *fp,*ft;
	
	struct data
	{
		struct data* node;
		int num;
		int cl,cr;
		struct data* left;
		struct data* right;
	};
	struct data* head=NULL,* p=NULL,* pc=NULL;
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	
	void find();
	void add();
	void delete();
	void show();
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	
	int main()
	{
		int choose;
		head_cnt=0;
		
		fp=fopen("btdata.dat","rb+");
		if(fp==NULL) { fp=fopen("btdata.dat","wb+"); }
		rewind(fp);
		while(fread(&info,sizeof(info),1,fp)==1)
		{
			number=info.value;
			if(info.state==1) { add(); }
			else { delete(); }
		}
		
		do
		{
		printf("\033c");
		printf("\n\n\n                                 choose the option");
		printf("\n\n           ----------------------------------------------------------");
		printf("\n\n            1) add number         2) remove number    3) find number ");
		printf("\n\n            4) show all numbers   5) clear all data   6) exit        ");
		printf("\n\n           ----------------------------------------------------------");
		printf("\n                                         ");
		do { choose=getch()-48; }while(choose!=1&&choose!=2&&choose!=3&&choose!=4&&choose!=5&&choose!=6);
		if(choose==1)
		{
			printf("\033c");
			printf("\n\n\n   please enter the number which you want to add :  ");
			scanf("%d",&number);
			add();
			if(s==1)
			{
				printf("\n\n   your entered number is 'successfully added' in the binary tree");
				fseek(fp,0,SEEK_END);
				info.value=number;
				info.state=1;
				fwrite(&info,sizeof(info),1,fp);
			}
			
			printf("\n\n\n   enter any key to continue....");
			getch();
			getch();
		}
		if(choose==2)
		{
			printf("\033c");
			if(head!=NULL)
			{
				printf("\n\n\n   please enter number which you want to remove :  ");
				scanf("%d",&number);
				delete();
				if(r==1)
				{
					printf("\n\n\n   your entered number is 'successfully removed' from the binary tree");
					fseek(fp,0,SEEK_END);
					info.value=number;
					info.state=2;
					fwrite(&info,sizeof(info),1,fp);
				}
				else
				{
					printf("\n\n\n   your entered number is 'not present' in the binary tree");
				}
			}
			else
			{
				printf("\n\n\n                ... Binary tree is empty ...");
			}
				printf("\n\n\n   enter any key to continue....");
				getch();
				getch();
		}
		if(choose==3) { find(); }
		if(choose==4) { show(); }
		if(choose==5)
		{
			printf("\033c");
			printf("\n\n\n\n\n                        All data is 'cleared' successfully");
			ft=fopen("bttemp.dat","wb+");
			fclose(ft);
			fclose(fp);
			remove("btdata.dat");
			rename("bttemp.dat","btdata.dat");
			fp=fopen("btdata.dat","rb+");
			if(fp==NULL) { fp=fopen("btdata.dat","wb+"); }
			head=NULL;
			head_cnt=0;
			printf("\n\n\n   enter any key to continue....");
			getch();
		}
		}while(choose!=6);
		
	printf("\n\n...");
	return 0;
	}
	
	
	void add()
	{
			int done=0;
			s=0;
			
			struct data* ptr = (struct data*)malloc(sizeof(struct data));
			ptr->num=number;
			
		if(head_cnt==0)
		{
			head=ptr;
			head_cnt++;
			s=1;
		}
		else
		{
			p=head;
			while(done==0)
			{
			if((p->num)>number)
			{
				if(p->left!=NULL) { p=p->left; }
				else { 
				p->left=ptr;
				p->left->node=p;
				s=1;
				done=1; }
			}
			else if((p->num)<number)
			{
				if(p->right!=NULL) { p=p->right; }
				else { 
				p->right=ptr;
				p->right->node=p;
				s=1;
				done=1; }
			}
			else 
			{
				printf("\n\n\n   your entered number is 'already present' in the binary tree");
				done=1;
			}
			}
		}
	}
	
	void delete()
	{
		p=head;
		r=0;
		
		while(p->num!=number)
		{
			if((p->num)>number)
			{
				if(p->left!=NULL)
				{
					p=p->left;
				}
				else { break; }
			}
			else
			{
				if(p->right!=NULL)
				{
					p=p->right;
				}
				else { break; }
			}
		}
		
			if(p->num==number)
			{
				if(p->right!=NULL)
				{
					pc=p;
					p=p->right;
					while(p->left!=NULL)
					{
						p=p->left;
					}
					pc->num=p->num;
					if(p==pc->right)
					{
						if(p->right==NULL) { p->node->right=NULL; }
						else{ p->node->right=p->right;
							  p->right->node=p->node; }
					}
					else
					{
						if(p->right==NULL) { p->node->left=NULL; }
						else{ p->node->left=p->right;
							  p->right->node=p->node; }
					}
				}
				else if(p->left!=NULL)
				{
					pc=p;
					p=p->left;
					while(p->right!=NULL)
					{
						p=p->right;
					}
					pc->num=p->num;
					if(p==pc->left)
					{
						if(p->left==NULL) { p->node->left=NULL; }
						else{ p->node->left=p->left;
							  p->left->node=p->node; }
					}
					else
					{
						if(p->left==NULL) { p->node->right=NULL; }
						else{ p->node->right=p->left;
							  p->left->node=p->node; }
					}
				}
				else
				{
					if(p==head)
					{
						head=NULL;
						head_cnt=0;
					}
					else
					{
						if(p->node->left!=NULL&&p->node->right!=NULL)
						{
							if(p->num==p->node->left->num){ p->node->left=NULL; }
							else{ p->node->right=NULL; }
						}
						else if(p->node->left!=NULL)
						{
							p->node->left=NULL;
						}
						else
						{
							p->node->right=NULL;
						}
					}
				}
			r=1;
			}
	}
	
	void find()
	{
		int level=0;
		
		printf("\033c");
		if(head!=NULL)
		{
		printf("\n\n\n   please enter number which you want to find :  ");
		scanf("%d",&number);
		
		p=head;
		while(p->num!=number)
		{
			if((p->num)>number)
			{
				if(p->left!=NULL)
				{
					p=p->left;
					level++;
				}
				else { break; }
			}
			else
			{
				if(p->right!=NULL)
				{
					p=p->right;
					level++;
				}
				else { break; }
			}
		}
			if(p->num==number)
			{
				printf("\n\n   your entered number is 'found' at 'level %d' in binary tree",level);
			}
			else
			{
				printf("\n\n\n   your entered number is 'not found' in the binary tree");
			}
			}
			else
			{
				printf("\n\n\n                ... Binary tree is empty ...");
			}
			
			printf("\n\n\n   enter any key to continue....");
			getch();
			getch();
	}
	
	void show()
	{
		int size;
		printf("\033c");
		fseek(fp,0,SEEK_END);
		size=ftell(fp);
		
		printf("\n\n\n   All 'added' elements in the binary tree are listed below....");
		printf("\n\n           ");
		if(size!=0)
		{
		rewind(fp);
		while(fread(&info,sizeof(info),1,fp)==1)
		{
			if(info.state==1){ printf("  %d",info.value); }
		}
		}
		else
		{
			printf("                 ... List is empty ...");
		}
		
		printf("\n\n\n   All 'removed' elements in the binary tree are listed below....");
		printf("\n\n           ");
		if(size!=0)
		{
		rewind(fp);
		while(fread(&info,sizeof(info),1,fp)==1)
		{
			if(info.state==2){ printf("  %d",info.value); }
		}
		}
		else
		{
			printf("                 ... List is empty ...");
		}
		
		printf("\n\n\n   Present elements in the binary tree listed below in 'sorted manner'....");
		printf("\n\n           ");
		
		if(head!=NULL)
		{
		p=head;
		while((head->cl!=1&&head->left!=NULL)||head->cr!=1||head!=p)
		{
		
		while(p->left!=NULL&&p->cl==0)
		{
			p->cl=1;
			p=p->left;
		}
		
		if(p->cr==0)
		{
			printf("  %d",p->num);
		}
		
		if(head->right==NULL&&(head->cl==1||head->left==NULL)&&p==head) { break; }
		
		if(p->right!=NULL&&p->cr==0)
		{
			p->cr=1;
			p=p->right;
		}
		else
		{
			if((p->cl==1||p->left==NULL)&&(p->cr==1||p->right==NULL))
			{
				p->cl=0;
				p->cr=0;
			}
			p=p->node;
		}
		
		}
		head->cl=0;
		head->cr=0;
		}
		else
		{
			printf("             ... Binary tree is empty ...");
		}
		
			printf("\n\n\n   enter any key to continue....");
			getch();
			
	
	}
