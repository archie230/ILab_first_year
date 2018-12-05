//______________________________________________________________________________________________________________________
List* List_createlist()
{
    List* list = new List;
//
    list -> head = new Node;
    list -> head -> prev =      NULL;
    list -> head -> next =      NULL;
    list -> head -> data =      POISON;
#ifndef List_Fast
    list -> head -> can1 =      can;
    list -> head -> can2 =      can;
    Node_FillingCHECKSM(list -> head);
#endif

    list -> tail = new Node;
    list -> tail -> prev =      NULL;
    list -> tail -> next =      NULL;
    list -> tail -> data =      POISON;
#ifndef List_Fast
    list -> tail -> can1 =      can;
    list -> tail -> can2 =      can;
    Node_FillingCHECKSM(list -> tail);
#endif
    list -> nodes_number = 0;
    return list;
}

Node* Node_createnode(int value)
{
    Node* node = new Node;

    node -> data = value;
    node -> next = NULL;
    node -> prev = NULL;

#ifndef List_Fast
    node -> can1 = can;
    node -> can2 = can;
#endif

    return node;
}

void List_insertafter(Node* currptr, int data, List* list)
{
    if(list == NULL) {
        printf("Empty list in fuction Insert_after!\n");
        return;
    }

    if(currptr == list -> tail)
    {
        printf("CAN'T INSERT AFTER TAIL\n");
        return;
    }

    Node* node_ins = Node_createnode(data);


    if(list -> nodes_number == 0)
    {
        printf("Zero nodes in your list before insertion\n");

        list -> head -> next = node_ins;
        list -> tail -> prev = node_ins;

        node_ins -> prev = list -> head;
        node_ins -> next = list -> tail;

        list -> nodes_number++;

#ifndef List_Fast
        Node_FillingCHECKSM(node_ins);
        Node_FillingCHECKSM(list -> tail);
        Node_FillingCHECKSM(list -> head);
#endif
        if(Node_ok(node_ins) < 0)
        {
            printf("Node not ok in insertion func\n");
            return;
        }
        return;
    }

    else {
        node_ins -> next = currptr -> next;
        node_ins -> prev = currptr;

        currptr -> next = node_ins;
        node_ins -> next -> prev = node_ins;
        list -> nodes_number++;

#ifndef List_Fast
        Node_FillingCHECKSM(node_ins);
        Node_FillingCHECKSM(list -> tail);
        Node_FillingCHECKSM(list -> head);
#endif
        if(Node_ok(node_ins) < 0)
        {
            printf("Node not ok after insertion in insertion func");
            return;
        }
        return;
    }
}

void Node_deletenode(Node* node, List* list)
{
    if(Node_ok(node) < 0){
        printf("Node not ok in delete node func\n");
        return;
    }

    node -> prev -> next = node -> next;
    node -> next -> prev = node -> prev;

    node -> next = NULL;
    node -> prev = NULL;

    list -> nodes_number--;

    delete node;
}

void List_swipe_currptr_right(Node** currptr_ptr, List* list)
{
    if(list == NULL)
    {
        printf("Empty list in fuction List_swipe_currptr_right!\n");
        return;
    }

    if(Node_ok(*currptr_ptr) < 0)
    {
        printf("Node not ok in swipe ptr func\n");
        return;
    }

    if (list -> nodes_number == 0)
    {
        printf("Can't swipe currptr in list with 0 elements\n");
        return;
    }

    if ((*currptr_ptr) -> next == NULL)
    {
        printf("Current element is tail, can't swipe pointer to right\n");
        return;
    }

    (*currptr_ptr) = (*currptr_ptr) -> next;
}

void List_swipe_currptr_left(Node** currptr_ptr, List* list)
{
    if(list == NULL)
    {
        printf("Empty list in fuction List_swipe_currptr_left!\n");
        return;
    }

    if(Node_ok(*currptr_ptr) < 0)
    {
        printf("Node not ok in swipe ptr func\n");
        return;
    }

    if (list -> nodes_number == 0)
    {
        printf("Can't swipe currptr in list with 0 elements\n");
        return;
    }

    if (list -> nodes_number == 0)
    {
        printf("Can't swipe currptr in list with 0 elements\n");
        return;
    }

    if ((*currptr_ptr) -> next == NULL)
    {
        printf("Current element is head, can't swipe pointer to right\n");
        return;
    }

    (*currptr_ptr) = (*currptr_ptr) -> prev;
}

Node* List_search(List* list, int value)
{
    if(list == NULL)
    {
        printf("Empty list in fuction List_search!\n");
        return NULL;
    }

    Node* cur = list -> head;

    while(cur != NULL)
    {
        if(cur -> data == value){
            return cur;
        }

        cur = cur -> next;
    }

    printf("No elements in with value %d \n", value);
    return NULL;
}

int Node_index(List* list, Node* node)
{
    int index = 1;
    Node* cur = list -> head;
    for(index = 1; index <= list -> nodes_number; index++)
    {
        if(cur -> next == node)
            return index;

        cur = cur -> next;
    }

    printf("can't index that element");
    return ERR_index;
}

void List_printlist(List* list)
{
    if(List_ok(list) < 0)
    {
        printf("List not ok in print func\n");
        return;
    }

    Node* current = list -> head;

    printf("HEAD  ");

    for(int i = 0; i < list -> nodes_number; i++)
    {
        printf("%d  ", current -> next -> data);
        current = current -> next;
    }

    printf("TAIL\n");
}

#ifndef List_Fast
void Node_FillingCHECKSM(Node* node)
{
    {
        if (node == NULL)
        {
            printf("NULL ptr chksum ");
            return;
        }

        node -> checksum = ( ((unsigned long int) (node -> next) + (unsigned long int) (node -> prev )) % 12345);
    }
}
#endif

int Node_ok(Node* node)
{
    if(node == NULL)
    {
        printf("Null pointer!!\n");
        return ERR_NULLptr;
    }

    if(node -> data == POISON)
    {
        printf("HEAD or TAIL in the Node_ok function\n");
        return ERR_headtail;
    }

#ifndef List_Fast
    if(node -> can1 != can)
    {
        printf("ERR_can1\n");
        return ERR_can1;
    }

    if(node -> can2 != can)
    {
        printf("ERR_can2\n");
        return ERR_can2;
    }

    if(node -> checksum != ( ((unsigned long int) (node -> next) + (unsigned long int) (node -> prev )) % 12345))
    {
        printf("ERR_checksum\n");
        return ERR_chcksum;
    }

#endif

    return OK;
}

int List_ok(List* list)
{
    if(list == NULL)
    {
        printf("Empty list!!\n");
        return ERR_NULLptr;
    }

#ifndef List_Fast
    if(list -> head -> can1 != can)
    {
        printf("ERR_can1\n");
        return ERR_can1;
    }

    if(list -> head  -> can2 != can)
    {
        printf("ERR_can2\n");
        return ERR_can2;
    }

    if(list -> head  -> checksum != ( ((unsigned long int) (list -> head -> next) +
                                       (unsigned long int) (list -> head -> prev )) % 12345))
    {
        printf("ERR_checksum\n");
        return ERR_chcksum;
    }
//______________________________________________________________________________________________________________________
    if(list -> tail -> can1 != can)
    {
        printf("ERR_can1\n");
        return ERR_can1;
    }

    if(list -> tail  -> can2 != can)
    {
        printf("ERR_can2\n");
        return ERR_can2;
    }

    if(list -> tail  -> checksum != ( ((unsigned long int) (list -> tail -> next) +
                                       (unsigned long int) (list -> tail -> prev )) % 12345))
    {
        printf("ERR_checksum\n");
        return ERR_chcksum;
    }
#endif

    return OK;

}
void List_deletelist(List* list)
{
    if(List_ok(list) < 0){
        printf("List not ok in delete func\n");
        return;
    }

    Node* curr = list -> head;
    for(int i = 0; i < list -> nodes_number; i++)
    {
        curr = curr -> next;
        delete curr -> prev;
    }

    delete list -> head;
    delete list -> tail;
    delete list;
}