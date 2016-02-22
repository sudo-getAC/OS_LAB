#include<bits/stdc++.h>
using namespace std;
#define no_processes 100000
#define RED		1
#define BLACK	2

struct process
{
    int spent_cpu_time;     // for spent_cpu_time part
    int arrival_time;
    int burst_time;
    int turnaround_time;
    int waiting_time;
    bool status;
};
process p[no_processes];
struct node {
	int key,index;
	struct node *left, *right, *p;
	int color;
};


typedef struct node *NODEPTR;
struct node NIL;
NODEPTR NILPTR = &NIL;

void inorder(NODEPTR x) {
	if (x != NILPTR) {
		inorder(x->left);
		printf("%d ", x->key);
		inorder(x->right);
	}
}

NODEPTR search(NODEPTR root, int k) {
    if (root == NULL)
        return root;
	if (root == NILPTR || root->key == k)
		return root;
	if (k < root->key)
		return search(root->left, k);
	else
		return search(root->right, k);
}

NODEPTR minimum(NODEPTR root) {
	while (root->left != NILPTR)
		root = root->left;
	return root;
}

NODEPTR maximum(NODEPTR root) {
	while (root->right != NILPTR)
		root = root->right;
	return root;
}

NODEPTR successor(NODEPTR root, int x) {
	NODEPTR temp = search(root, x);
	if (temp == NILPTR) {
		printf("%d not in tree\n", x);
		return temp;
	}
	if (temp->right != NILPTR)
		return minimum(temp->right);
	NODEPTR y = temp->p;
	while (y != NILPTR && temp == y->right) {
		temp = y;
		y = y->p;
	}
	return y;
}

NODEPTR predecessor(NODEPTR root, int x) {
	NODEPTR temp = search(root, x);
	if (temp == NILPTR) {
		printf("%d not in tree\n", x);
		return temp;
	}
	if (temp->left != NILPTR)
		return maximum(temp->left);
	NODEPTR y = temp->p;
	while (y != NILPTR && temp == y->left) {
		temp = y;
		y = y->p;
	}
	return y;
}
void leftrotate(NODEPTR *treeroot, NODEPTR x) {
	NODEPTR y = x->right;
	x->right = y->left;
	if (y->left != NILPTR)
		y->left->p = x;
	y->p = x->p;
	if (x->p == NILPTR)
		*treeroot = y;
	else if (x->p->left == x)
		x->p->left = y;
	else
		x->p->right = y;
	y->left = x;
	x->p = y;
}

void rightrotate(NODEPTR *treeroot, NODEPTR y) {
	NODEPTR x = y->left;
	y->left = x->right;
	if (x->right != NILPTR)
		x->right->p = y;
	x->p = y->p;
	if (y->p == NILPTR)
		*treeroot = x;
	else if (y->p->left == y)
		y->p->left = x;
	else
		y->p->right = x;
	x->right = y;
	y->p = x;
}

void rbinsertfixup(NODEPTR *treeroot, NODEPTR z) {
	while (z->p->color == RED) {
		if (z->p == z->p->p->left) {
			NODEPTR y = z->p->p->right;
			if (y->color == RED) {
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			}
			else {
				if (z == z->p->right) {
					z = z->p;
					leftrotate(treeroot,z);
				}
				z->p->color = BLACK;
				z->p->p->color = RED;
				rightrotate(treeroot,z->p->p);
			}
		}
		else {
			NODEPTR y = z->p->p->left;
			if (y->color == RED) {
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			}
			else {
				if (z == z->p->left) {
					z = z->p;
					rightrotate(treeroot,z);
				}
				z->p->color = BLACK;
				z->p->p->color = RED;
				leftrotate(treeroot,z->p->p);
			}
		}
	}
	(*treeroot)->color = BLACK;
}

void rbinsert(NODEPTR *treeroot, int z,int i) {
	NODEPTR Z = (NODEPTR) malloc(sizeof(struct node));
	Z->key = z;
	Z->index=i;
	NODEPTR y = NILPTR;
	NODEPTR x = *treeroot;
	while (x != NILPTR) {
		y = x;
		if (Z->key < x->key)
			x = x->left;
		else
			x = x->right;
	}
	Z->p = y;
	if (y == NILPTR)
		*treeroot = Z;
	else if (Z->key < y->key)
		y->left = Z;
	else
		y->right = Z;
	Z->left = NILPTR;
	Z->right = NILPTR;
	Z->color = RED;
	rbinsertfixup(treeroot,Z);
}

void rbtransplant(NODEPTR *treeroot, NODEPTR u, NODEPTR v) {
	if (u->p == NILPTR)
		*treeroot = v;
	else if (u == u->p->left)
		u->p->left = v;
	else
		u->p->right = v;
	v->p = u->p;
}

void rbdeletefixup(NODEPTR *treeroot, NODEPTR x) {
	while (x != *treeroot && x->color == BLACK) {
		if (x == x->p->left) {
			NODEPTR w = x->p->right;
			if (w->color == RED) {
				w->color = BLACK;
				x->p->color = RED;
				leftrotate(treeroot,x->p);
				w = x->p->right;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;
				x = x->p;
			}
			else {
			 	if (w->right->color == BLACK) {
					w->left->color = BLACK;
					w->color = RED;
					rightrotate(treeroot,w);
					w = x->p->right;
				}
				w->color = x->p->color;
				x->p->color = BLACK;
				w->right->color = BLACK;
				leftrotate(treeroot,x->p);
				x = *treeroot;
			}
		}
		else {
			NODEPTR w = x->p->left;
			if (w->color == RED) {
				w->color = BLACK;
				x->p->color = RED;
				rightrotate(treeroot,x->p);
				w = x->p->left;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;
				x = x->p;
			}
			else {
				if (w->left->color == BLACK) {
					w->right->color = BLACK;
					w->color = RED;
					leftrotate(treeroot,w);
					w = x->p->left;
				}
				w->color = x->p->color;
				x->p->color = BLACK;
				w->left->color = BLACK;
				rightrotate(treeroot,x->p);
				x = *treeroot;
			}
		}
	}
	x->color = BLACK;
}

void rbdelete(NODEPTR *treeroot, int z) {
	//cout<<"qq";
	NODEPTR Z = search(*treeroot, z);
	//cout<<"qq";
	if(Z==NULL)
        return;
	if (Z == NILPTR) {
		//printf("Node to be deleted not found\n");
		return;
	}
	NODEPTR y = Z;
	int yoc = y->color;
	NODEPTR x;
	//cout<<"qq";
	if (Z->left == NILPTR) {
		x = Z->right;
		rbtransplant(treeroot,Z,Z->right);
	}
	else if (Z->right == NILPTR) {
		x = Z->left;
		rbtransplant(treeroot,Z,Z->left);
	}
	else {
		y = minimum(Z->right);
		yoc = y->color;
		x = y->right;
		if (y->p == Z)
			x->p = y;
		else {
			rbtransplant(treeroot,y,y->right);
			y->right = Z->right;
			y->right->p = y;
		}
		rbtransplant(treeroot,Z,y);
		y->left = Z->left;
		y->left->p = y;
		y->color = Z->color;
	}
	if (yoc == BLACK)
		rbdeletefixup(treeroot,x);
}
inline bool cmp(const struct process a,const struct process b)
{
    if(a.arrival_time<=b.arrival_time)
        return true;
    else
        return false;
}

void process_init()
{
    for(int i=0;i<no_processes;++i)
    {
        p[i].arrival_time=rand()%50000;
        p[i].burst_time=rand()%100+10;

    }
    sort(p,p+no_processes,cmp);
    p[0].arrival_time=0;
}


void red_black_tree_cfs()
{
    int pending_processes = no_processes;
    int time = 0;
    int index_arr = 0;
    int waiting_time_sum = 0;
    int turnaround_time_sum = 0;
    struct node *root = NULL;

    NIL.left=NIL.right=NIL.p=NILPTR;
    NIL.color=BLACK;
    NODEPTR tree=NILPTR;
    for(int i = 0; i < no_processes ; i++ )
    {
        p[i].spent_cpu_time = 0;
        p[i].waiting_time=0;
        p[i].turnaround_time=0;
        p[i].status=false;
        //cout<<p[i].arrival_time<<endl;
    }

    while(pending_processes > 0)
    {
        /* Insert the processes arriving at 'time' into the red black tree */

        while(index_arr < no_processes and p[index_arr].arrival_time <= time)
        {
          //  cout<<index_arr<<"bego";
            rbinsert(&tree,p[index_arr].spent_cpu_time,index_arr);
            //cout<<"beg ";
            p[index_arr].status=true;
            index_arr++;
        }
        //cout<<"hey";
        //cout<<"1 ";
        // allot cpu to the process with the lowest spent_cpu_time

        int mini_key=minimum(tree)->key;
        int time_quantum = rand()%100 + 10;
        int i = minimum(tree)->index;
        //cout<<"2 ";

        if(p[i].spent_cpu_time + time_quantum >= p[i].burst_time)
        {
            //cout<<"3 ";
            int x = p[i].spent_cpu_time;
            time += p[i].burst_time - p[i].spent_cpu_time;
            p[i].spent_cpu_time=p[i].burst_time;
            p[i].turnaround_time+=(p[i].burst_time-time_quantum);
            p[i].status=false;
            pending_processes--;
           // cout<<"delete";
            //cout<<mini_key<<endl;
            rbdelete(&tree,mini_key);
            //cout<<"delete";
        }
        else
        {

            int x = p[i].spent_cpu_time;
            p[i].spent_cpu_time+=time_quantum;
            time += time_quantum;
           // cout<<"4 ";
            //cout<<mini_key<<endl;
            rbdelete(&root,mini_key);
            //cout<<"5 ";
            rbinsert(&tree,p[i].spent_cpu_time,i );
            //cout<<"6 ";
            //p[next.index].turnaround_time+=time_quantum;
        }
        for(int j=0;j<index_arr;++j)
            if(p[j].status)
        {
            p[j].turnaround_time+=time_quantum;
            if(i!=j)
                p[j].waiting_time+=time_quantum;
        }
    }
    for(int j=0;j<no_processes;++j)
        waiting_time_sum+=p[j].waiting_time;
    cout<<" Average waiting time      : "<<float(waiting_time_sum/no_processes)<<endl;
    return;
}
int main()
{
    srand(time(NULL));
    process_init();
    clock_t begin1=clock();
    red_black_tree_cfs();
    cout<<" CFS with RB Trees ran for : "<<float(clock()-begin1)/(CLOCKS_PER_SEC)<<endl;

    //cout<<"here";
    return 0;

}

