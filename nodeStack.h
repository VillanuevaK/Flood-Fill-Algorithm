struct nodeStack
{
	public:
		int row;
		int col;
		nodeStack* link;
	
};
typedef nodeStack* nodeStackPtr;
void push1(nodeStackPtr& /*head*/, int[] /*info to put into new node*/);
void pop1(nodeStackPtr& /*head*/, int[] /*this call by refrence array from the parameter will copy the info from the node being deleted*/);
void pop1(nodeStackPtr& head);//basically just deltes node;
bool isLLempty(nodeStackPtr& /*head*/);
void emptyStack(nodeStackPtr& /*head*/);
