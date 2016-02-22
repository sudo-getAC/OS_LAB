#include<bits/stdc++.h>
#define no_processes 100000

using namespace std;

struct process
{
    int spent_cpu_time;     // for spent_cpu_time part
    int arrival_time;
    int burst_time;
    int turnaround_time;
    int waiting_time;
    int last_time;
    bool status;
}p[no_processes];;

struct node
{
    int key;
    int index;
    struct node *left,*right;
    int height;
};

//delete_process
int max(int a,int b);

int height(struct node *N)
{
    if(N==NULL)
        return 0;

    return N->height;

}

int max(int a,int b)
{
    return (a>b?a:b);
}

struct node* newNode(int key,int i)
{
    struct node* node = (struct node*)
                        malloc(sizeof(struct node));
    node->key   = key;
    node->left   = NULL;
    node->right  = NULL;
    node->index = i;
    node->height = 1;  // new node is initially added at leaf
    return(node);
}

struct node *rightRotate(struct node *y)
{
    struct node *x=y->left;
    struct node *t2=x->right;

    x->right=y;
    y->left=t2;

    y->height=max(height(y->left),height(y->right))+1;
    x->height=max(height(x->left),height(x->right))+1;

    return x;
}

struct node *leftRotate(struct node *x)
{
    struct node *y=x->right;
    struct node *t2=y->left;

    y->left=x;
    x->right=t2;

    x->height =max(height(x->left),height(x->right))+1;
    y->height = max(height(y->left),height(y->right))+1;
    return y;

}

int getBalance(struct node *n)
{
    if(n==NULL)
        return 0;
    return height(n->left)-height(n->right);
}

struct node* insert(struct node* node, int key, int i)
{
    /* 1.  Perform the normal BST rotation */
    if (node == NULL)
        return(newNode(key,i));

    if (key < node->key)
        node->left  = insert(node->left, key, i);
    else
        node->right = insert(node->right, key, i);

    /* 2. Update height of this ancestor node */
    node->height = max(height(node->left), height(node->right)) + 1;

    /* 3. Get the balance factor of this ancestor node to check whether
       this node became unbalanced */
    int balance = getBalance(node);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && key > node->left->key)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}

/* Given a non-empty binary search tree, return the node with minimum
   key value found in that tree. Note that the entire tree does not
   need to be searched. */
struct node * minValueNode(struct node* node)
{
    struct node* current = node;

    /* loop down to find the leftmost leaf */
    if(current != NULL)
        while (current->left != NULL)
            current = current->left;

    return current;
}

struct node* deleteNode(struct node* root, int key)
{
    // STEP 1: PERFORM STANDARD BST DELETE

    if (root == NULL)
        return root;

    // If the key to be deleted is smaller than the root's key,
    // then it lies in left subtree
    if ( key < root->key )
        root->left = deleteNode(root->left, key);

    // If the key to be deleted is greater than the root's key,
    // then it lies in right subtree
    else if( key > root->key )
        root->right = deleteNode(root->right, key);

    // if key is same as root's key, then This is the node
    // to be deleted
    else
    {
        // node with only one child or no child
        if( (root->left == NULL) || (root->right == NULL) )
        {
            struct node *temp = root->left ? root->left : root->right;

            // No child case
            if(temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
             *root = *temp; // Copy the contents of the non-empty child

            free(temp);
        }
        else
        {
            // node with two children: Get the inorder successor (smallest
            // in the right subtree)
            struct node* temp = minValueNode(root->right);

            // Copy the inorder successor's data to this node
            root->key = temp->key;
            root->index = temp->index;
            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->key);
        }
    }

    // If the tree had only one node then return
    if (root == NULL)
      return root;

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = max(height(root->left), height(root->right)) + 1;

    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to check whether
    //  this node became unbalanced)
    int balance = getBalance(root);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
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

void avl_tree_cfs()
{
    int pending_processes = no_processes;
    int time = 0;
    int index = 0;
    long long waiting_time_sum = 0;
    long long turnaround_time_sum = 0;
    struct node *root = NULL;

    for(int i = 0; i < no_processes ; i++ )
    {
        p[i].spent_cpu_time = 0;
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
        p[i].last_time = p[i].arrival_time;
        p[i].status=false;
    }

    while(pending_processes > 0)
    {
        /* Insert the processes arriving at 'time' into the red black tree */
        while(p[index].arrival_time <= time && index < no_processes)
        {
            root = insert(root,p[index].spent_cpu_time,index);
            p[index].status=true;
            index++;
            //cout<<"1 ";
        }
        // allot cpu to the process with the lowest spent_cpu_time
            //cout<<"2 ";
        struct node *next = minValueNode(root);
        int time_quantum = rand()%100 + 10;
        int i = next->index;

        if(p[i].spent_cpu_time + time_quantum >= p[i].burst_time)
        {
            int x = p[i].spent_cpu_time;
            //p[i].waiting_time += time - p[i].last_time;
            time += p[i].burst_time - p[i].spent_cpu_time ;
            p[i].spent_cpu_time=p[i].burst_time;
            p[i].turnaround_time=time - p[i].arrival_time;
            p[i].status=false;
            pending_processes--;
            root = deleteNode(root,x);
        }
        else
        {
            //cout<<"4 ";
            int x = p[i].spent_cpu_time;
            //p[i].waiting_time += time - p[i].last_time;
            //p[i].last_time = time;
            p[i].spent_cpu_time+=time_quantum;
            root = deleteNode(root,x);
            time += time_quantum;
            root = insert(root,p[i].spent_cpu_time,i);
        }
        for(int j=0;j<index;++j)
            if(p[j].status)
        {
            p[j].turnaround_time+=time_quantum;
            if(i!=j)
                p[j].waiting_time+=time_quantum;
        }
    }
    //cout<<index<<endl;
    for(int i=0;i<no_processes;++i)
        waiting_time_sum+=p[i].waiting_time;
    //cout<<waiting_time_sum<<endl;

}
int main()
{
    srand(time(NULL));
    process_init();

    time_t start,finish;

    const clock_t begin1=clock();
    avl_tree_cfs();


    cout<<"Time taken for CFS with AVL trees : "<<float(clock()-begin1)/(CLOCKS_PER_SEC)<<endl;

}