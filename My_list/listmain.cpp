#include <list.h>

int main()
{
    //=====================================
    List* list = List_createlist();
    Node* currptr = list -> head;
    //=====================================

    List_insertafter(currptr, 365, list);

    List_insertafter(currptr, 364, list);

    List_insertafter(currptr, 363, list);

    List_insertafter(currptr, 362, list);

    List_insertafter(currptr, 361, list);

    List_insertafter(currptr, 360, list);

    List_printlist(list);

    printf("%d\n", Node_index(list, List_search(list, 365)));



    List_deletelist(list);
    return 0;
}