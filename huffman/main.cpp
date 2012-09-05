#include <cstdlib>
#include <iostream>
#include <list>

using namespace std;
struct Node
{
    char index;         //ֵ 
    int weight;         //Ȩ 
    Node* parent;       //����� 
    Node* left;
    Node* right;
};
class HuffmanTree
{
    private:
        Node* root;
        void destory(Node*);
    public:
    HuffmanTree(){}
    HuffmanTree(char* index,int* weight,int n);     //�ó���Ϊn�����齨��
    ~HuffmanTree();
    void Encode(char);      //���� 
    char Decode(char*);      //���� 
};
HuffmanTree::HuffmanTree(char* index,int* weight,int n)
{
	root=new Node;
    Node* temp=new Node[n];
    int i;
    for(i=0;i<n;i++)
    {
        temp[i].index=index[i];
        temp[i].weight=weight[i];
        temp[i].parent=temp[i].left=temp[i].right=NULL;
    }
    list<Node> nodelist(temp,temp+n);       //�����Ա����������,�����������ֵ 

    while(nodelist.size()!=1)                                     //�������� 
    {
        list<Node>::iterator p;
        list<Node>::iterator n1,n2;
        n1=nodelist.begin();                    //n1,n2�������Ȩֵ��С,�ڶ�С�Ľ��,�������� 
        for(p=nodelist.begin();p!=nodelist.end();p++)
        {
            if(p->weight<n1->weight)  n1=p;        
        }
        Node* tempnode1=new Node;                     //�ҵ���С�� 
        *tempnode1=*n1;
        nodelist.erase(n1);         
        n2=nodelist.begin();
        for(p=nodelist.begin();p!=nodelist.end();p++)
        {
            if(p->weight<n2->weight)  n2=p; 
        }
        Node* tempnode2=new Node;                     //�ҵ��ڶ�С��
        *tempnode2=*n2;
        nodelist.erase(n2); 
        Node* tempparent=new Node;
        tempparent->left=tempnode1;
        tempparent->right=tempnode2;
        tempparent->weight=tempnode1->weight+tempnode2->weight;
        tempparent->parent=NULL;
        tempnode1->parent=tempparent;
        tempnode2->parent=tempparent;
        nodelist.push_back(*tempparent);
    }
    *root=nodelist.back();
}
HuffmanTree::~HuffmanTree()
{
    destory(root);
}
void HuffmanTree::destory(Node* node)
{
    if(node!=NULL)
    {
        if(node->left!=NULL)
            destory(node->left);
        if(node->right!=NULL)
            destory(node->right);
        delete node;
    }
}
char HuffmanTree::Decode(char* str)
{
	int i=0;
	Node* p=root;
	while(str[i]!='\0')
	{
		if(str[i]=='0')
			p=p->left;
		if(str[i]=='1')
			p=p->right;
		i++;
	}
	return p->index;
}

int main(int argc, char *argv[])
{
    char index[8]={'a','b','c','d','e','f','g','h'};
    int weight[8]={5,29,7,8,14,23,3,11};
    HuffmanTree tree(index,weight,8);
	cout<<tree.Decode("110");
    system("PAUSE");
    return EXIT_SUCCESS;
}
