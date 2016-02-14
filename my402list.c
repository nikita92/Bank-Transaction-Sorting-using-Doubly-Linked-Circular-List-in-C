#include <stdlib.h>
#include <stdio.h>
#include "my402list.h"
#include "cs402.h"


/************************************************
int Length() : 
    Returns the number of elements in the list
************************************************/
int My402ListLength(My402List *list){
  return list->num_members;
}


/************************************************
int Empty() : 
    Returns TRUE if the list is empty. Returns FALSE otherwise.
************************************************/
int My402ListEmpty(My402List *list){
  if(list->num_members == 0)
    return TRUE;
  else
    return FALSE;
}


/************************************************
int Append(void *obj) : 
    Add obj after Last(). This function returns TRUE if the operation is performed successfully and returns FALSE otherwise
************************************************/
int My402ListAppend(My402List *list, void *obj){
  My402ListElem *Node = NULL;
  Node = (My402ListElem*)malloc(sizeof(My402ListElem));
  if(Node == NULL)
    return FALSE;
  else{
    My402ListElem *Last = NULL;                    //Do i have to assign heap memory location to this by using malloc()
    Last = list->anchor.prev;                      //list->prev has heap memory address or stack memory address?
    Last->next = Node;                             //If there is no node and only anchor then Last == list and then list->next will point to its first node. This function implicitly assign anchor to its first node
    Node->next = &(list->anchor);
    Node->prev = Last;
    list->anchor.prev = Node;
    Node->obj = obj;
    // printf("Present Node %d is : Node Address %p, Prev  %p, Next %p, Anchor Next %p\n", list->num_members, Node, Node->prev, Node->next, list->anchor.next); 
    list->num_members++;
    return TRUE;
  }
}

/************************************************
int Prepend(void *obj)
    Add obj before First(). This function returns TRUE if the operation is performed successfully and returns FALSE otherwise.
************************************************/
int My402ListPrepend(My402List *list, void *obj){
  My402ListElem *Node = NULL;
  Node = (My402ListElem*)malloc(sizeof(My402ListElem));
  if(Node == NULL)
    return FALSE;
  else{
      My402ListElem *First = NULL;
      First = list->anchor.next;
      Node->next = First;
      list->anchor.next = Node;
      Node->prev = &(list->anchor);
      First->prev = Node;
      Node->obj = obj;
      list->num_members++;
      return TRUE;
  }
}


/************************************************
void Unlink(My402ListElem *elem)
    Unlink and delete elem from the list. Please do not delete the object pointed to by elem and do not check if elem is on the list.
************************************************/
void My402ListUnlink(My402List *list, My402ListElem *elem){
  My402ListElem *Node = NULL;
  for(Node = My402ListFirst(list); Node != NULL; Node = My402ListNext(list, Node)){
    if(Node == elem){
      elem->prev->next = elem->next;
      elem->next->prev = elem->prev;
      free(elem);
      list->num_members--;
    }
  }    
 }

/************************************************
void UnlinkAll()
    Unlink and delete all elements from the list and make the list empty. Please do not delete the objects pointed to be the list elements.
************************************************/
void My402ListUnlinkAll(My402List *list){
  My402ListElem *Node = NULL;
  for(Node = My402ListFirst(list); Node != NULL; Node = My402ListNext(list, Node)){
    free(Node);
    list->num_members--;
  }    
}

/************************************************
int InsertBefore(void *obj, My402ListElem *elem)
    Insert obj between elem and elem->prev. If elem is NULL, then this is the same as Prepend(). This function returns TRUE if the operation is performed successfully and returns FALSE otherwise. Please do not check if elem is on the list.
************************************************/
int My402ListInsertBefore(My402List *list, void *obj, My402ListElem *elem){
  My402ListElem *Node = NULL;
  My402ListElem *temp = NULL;
  Node = (My402ListElem*)malloc(sizeof(My402ListElem));
  if(Node == NULL)
    return FALSE;
  else{
    for(temp = My402ListFirst(list); temp != NULL; temp = My402ListNext(list, temp)){
      if(temp == elem){
	elem->prev->next = Node;
	Node->next = elem;
	Node->prev = elem->prev;
	elem->prev = Node;
	Node->obj = obj;
	list->num_members++;
	return TRUE;
      }
    }
    // if(temp == NULL)
      return My402ListPrepend(list, obj);
  }
}

/************************************************
int InsertAfter(void *obj, My402ListElem *elem)
    Insert obj between elem and elem->next. If elem is NULL, then this is the same as Append(). This function returns TRUE if the operation is performed successfully and returns FALSE otherwise. Please do not check if elem is on the list.
************************************************/
int My402ListInsertAfter(My402List *list, void *obj, My402ListElem *elem){
  My402ListElem *Node = NULL;
  My402ListElem *temp = NULL;
  Node = (My402ListElem*)malloc(sizeof(My402ListElem));
  if(Node == NULL)
    return FALSE;
  else{
    for(temp = My402ListFirst(list); temp != NULL; temp = My402ListNext(list, temp)){
      if(temp == elem){
	elem->next->prev = Node;
	Node->prev = elem;
	Node->next = elem->next;
	elem->next = Node;
	Node->obj = obj;
	list->num_members++;
	return TRUE;
      }
    }
    // if(temp == NULL)
      return My402ListAppend(list, obj);
  }
}

/************************************************
My402ListElem *First()
    Returns the first list element or NULL if the list is empty.
************************************************/
My402ListElem *My402ListFirst(My402List *list){
   if(list->anchor.next == &(list->anchor))
     return NULL;
   else
     return list->anchor.next;
 } 


/************************************************
My402ListElem *Last()
    Returns the last list element or NULL if the list is empty.
************************************************/
My402ListElem *My402ListLast(My402List *list){
  if(list->anchor.prev == &(list->anchor))
    return NULL;
  else
    return list->anchor.prev;
}

/************************************************
My402ListElem *Next(My402ListElem *elem)
    Returns elem->next or NULL if elem is the last item on the list. Please do not check if elem is on the list.
************************************************/
My402ListElem *My402ListNext(My402List *list, My402ListElem *elem){
  My402ListElem *temp = NULL;
  temp = &(list->anchor);
  while(temp != elem){
    temp = temp->next;
    if(temp == &(list->anchor))           //if list is empty or element is not in the list it will return here only
      return NULL;
  }
  if(elem->next == &(list->anchor))      //if the next of the element is anchor it will return NULL
    return NULL;
  else
    return elem->next;
}

/************************************************
My402ListElem *Prev(My402ListElem *elem)
    Returns elem->prev or NULL if elem is the first item on the list. Please do not check if elem is on the list.
************************************************/
My402ListElem *My402ListPrev(My402List *list, My402ListElem *elem){
  My402ListElem *temp = NULL;
  temp = &(list->anchor);
  while(temp != elem){
    temp = temp->prev;
    if(temp == &(list->anchor))           //if list is empty or element is not in the list it will return here only
      return NULL;
  }
  if(elem->prev == &(list->anchor))      //if the next of the element is anchor it will return NULL
    return NULL;
  else
    return elem->prev;
}

/************************************************
My402ListElem *Find(void *obj)
    Returns the list element elem such that elem->obj == obj. Returns NULL if no such element can be found.
************************************************/
My402ListElem *My402ListFind(My402List *list, void *obj){
  My402ListElem *Node = NULL;
  for(Node = My402ListFirst(list); Node != NULL; Node = My402ListNext(list, Node)){
    if(Node->obj == obj)
      return Node;
  }
  return NULL;
}

/************************************************
int Init() : 
    Initialize the list into an empty list. Returns TRUE if all is well and returns FALSE if there is an error initializing the list.
************************************************/
int My402ListInit(My402List *list){
  //http://stackoverflow.com/questions/4993327/is-typecast-required-in-malloc
  //http://c-faq.com/malloc/cast.html
  //is it necessary to typecast malloc return?
  
  //list = (My402List*)malloc(sizeof(My402List));  //Assign heap memory
  //list = malloc(sizeof(My402List));
  if(list == NULL)
    return FALSE;
  else{
    list->num_members = 0;
    list->anchor.obj = NULL;
    list->anchor.next = &(list->anchor);
    list->anchor.prev = &(list->anchor);
    return TRUE;
  }
}
