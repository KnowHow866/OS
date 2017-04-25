struct node
{
	Queue_T process;
	struct node* next;
	struct node* prev;
};
typedef struct node Node;

struct list
{
	Node* entry;
	Node* end;
};
typedef struct list List;

List newList();
void addList(List* reserve_Queue, Node* p_new);
Node consumeList(List* reserve_Queue);
int isEmpty(List* reserve_Queue);

//-----------------------------------------

List newList(){
	puts("ready newList");
	List new;
	new.entry->next = new.end;
	new.end->prev = new.entry;
	puts("OK newList");
	return new;
}

void addList(List* reserve_Queue, Node* p_new){
	p_new->next = reserve_Queue->entry->next;
	reserve_Queue->entry->next->prev = p_new;

	reserve_Queue->entry->next = p_new;
	p_new->prev = reserve_Queue->entry;
}

Node consumeList(List* reserve_Queue){
	if(isEmpty(reserve_Queue)){
		puts("list empty err");
		Node no;
		return no;
	}
	else{
		Node* tmp= reserve_Queue->end->prev;
		reserve_Queue->end->prev = reserve_Queue->end->prev->prev;
		reserve_Queue->end->prev->prev->next = reserve_Queue->end->prev;
		return *tmp;
	}
}

int isEmpty(List* reserve_Queue){
	if(reserve_Queue->entry->next == reserve_Queue->end) return 1;
	else return 0;
}