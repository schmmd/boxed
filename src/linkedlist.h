#ifndef _H_LINKEDLIST
#define _H_LINKEDLIST

#ifndef NULL
#define NULL 0
#endif

template <class type>
class CLinkedList
{  
public: //TO ADD: Enqueue, Dequeue, Save, Load
  CLinkedList (); //Calls Create()
  ~CLinkedList(); //Calls Delete()
  
  void Create(); //Initiallize Head (called on constructor)
  void Delete(); //Delete      Head (called on destructor )

  bool  AddItem   (                       ); //Add node to begining
  type* GetItem   (long Number            ); //Overload for GetItem
  bool  DeleteItem(long Number            ); //Overload for DeleteItem
  bool  InsertItem(long Number, type Value); //Overload for InsertItem

  void  DeleteItems(); //Deletes all nodes other than the dummy header

  //void  DeleteElement(type element);

  bool Push(type  Value  ); //Pushes a node to the front of the Head
  type Pop (bool &Success);

  type Pop ();

  bool Enqueue(type Value   );
  type Dequeue(bool &Success);

  type Dequeue();
  //void ModifyElement(node **Head, int element, int element2);
  //void FindElement(node *Head, int element, node **location);
  //void InsertElement(node **Head, int element);

private:
  struct node
  {
    type Value;
    node *Next;
    node *Prev;
  };

  node* Head;   //Pointer to the top    of the Head

  node* GetNode(long Number);
  node* GetNode(long Number, bool Success);
};

/***************************************************
Constructor()
 Inputs nothing
 Calls Delete().
***************************************************/

template <class type>
CLinkedList<type>::CLinkedList()
{
  Create();
}

/***************************************************
Destructor()
 Calls Delete().
***************************************************/

template <class type>
CLinkedList<type>::~CLinkedList()
{
  Delete();
}

/***************************************************
Create()
 Initializes the Head by setting bottom to the dummy
 and pointing Head to the dummy as well.

 Inputs:
  nothing, but Head MUST equal NULL. If it does 
  not, we will have a memory leak. Head is
  always NULL after Delete().
 Output:
  The dummy header was created and now you may use
  functions without errors. The list is also
  made circular.
***************************************************/

template <class type>
void CLinkedList<type>::Create()
{
  Head       = new node;
  Head->Next = Head;
  Head->Prev = Head;
}

/***************************************************
Delete()
 Deletes each node and sets Head to NULL, deleting
 the "Dummy Head Node".

 Inputs: 
  Nothing
 Output:
  The list is equal to NULL and EVERY node is 
  deleted.
 Note:
  After a call to Delete(), DO NOT use ANY list
  functions other than Create. This is because the
  dummy head has been deleted, and if you call a
  function you have a great chance of having an
  error. Create() recreates the dummy head. If 
  you wish to simply clear the list, call
  DeleteItems().
***************************************************/

template <class type>
void CLinkedList<type>::Delete()
{
  node *temp = Head->Next;

  while (temp != Head)
  {
    temp = temp->Next;
    delete temp->Prev;
  }
  delete temp;

  Head   = NULL;
}

template <class type>
void CLinkedList<type>::DeleteItems()
{
  node *temp = Head->Next;

  while (temp != Head)
  {
    temp = temp->Next;
    delete temp->Prev;
  }

  Head->Next = Head;
  Head->Prev = Head;
}

/***************************************************
GetNode()
 Gets node n. The list starts at 0.
 Inputs: Node index to get
 Output: Returns a pointer to the node with the item
***************************************************/

template <class type>
typename CLinkedList<type>::node* CLinkedList<type>::GetNode(long Number)
{
  node* Temp = Head;

  for (int i=0; i<Number+1; i++)
  {
    if (Temp == NULL || Temp == Head->Prev)
    {
      return NULL;
    }

    Temp = Temp->Next;
  }

  if (Temp == NULL)
  {
    return NULL;
  }
  else
    return Temp;
}

/***************************************************
AddItem()
 Adds a node to the begining.
 Inputs the Value of the new node (in template type)
 Outputs (returns) a value indicating success
***************************************************/

template <class type>
bool CLinkedList<type>::AddItem()
{
  node * NewNode = new node; //Allocate dynamic memory

  if (NewNode == NULL) 
    return 0;

  NewNode->Next = Head->Next; //Next = begining of old Head (after dummy header)
  NewNode->Prev = Head; //The previous node is the dummy header
  
  (NewNode->Next)->Prev = NewNode; //The old begining of the Head's Prev is the new node

  Head->Next = NewNode; //The dummy header links to the new node

  if (Head->Prev == Head)
    Head->Prev = NewNode;

  return true;
}

/***************************************************
GetItem()
 Gets the node at index X where the Head starts at 0
 Inputs: the index of the node to get
 Output: a pointer of type CNode<Headtype> or NULL
 if failure
***************************************************/

template <class type>
type* CLinkedList<type>::GetItem(long Number)
{ 
  node *temp = GetNode(Number);

  if (temp != NULL)
    return &temp->Value;
  else
  {
    return NULL;
  }
}

/***************************************************
DeleteItem()
 Deletes item n where the Head starts at 0.
 Inputs: Index to delete and a boolean varible
 Output: Returns a boolean value indicating success
***************************************************/

template <class type>
bool CLinkedList<type>::DeleteItem(long Number)
{
  node *temp = GetNode(Number);

  if (temp != NULL)
  {
    (temp->Prev)->Next = temp->Next;
    (temp->Next)->Prev = temp->Prev;
    delete temp;
    
    return true;
  }
  else
  {
    return false;
  }
}

/***************************************************
Insert Node()
 Inserts a node with specified Value before Number
 Inputs: the place Number (the Head starts at 0) and
         the Value for the new node
 Output: a pointer to the new node or NULL on 
       failure
***************************************************/

template<class type>
bool CLinkedList<type>::InsertItem(long Number, type Value) //Inserts before [Number]th item (from 0)
{
  node* temp = GetNode(Number);

  if (temp != NULL)
  {
    node *NewNode = new node;
    NewNode->Value = Value;
    NewNode->Next = temp;
    NewNode->Prev = temp->Prev;

    (temp->Prev)->Next = NewNode;
    temp->Prev         = NewNode;

    return true;
  }
  else
  {
    return false;
  }
}

/***************************************************
Push()
 See Add() above
***************************************************/

template<class type>
bool CLinkedList<type>::Push(type Value)
{
  return AddItem(Value);
}

/***************************************************
Pop()
 Deletes and returns the top node Value.
 No input.
 Outputs the top node Value.
***************************************************/

template<class type>
type CLinkedList<type>::Pop(bool& Success)
{
  if (Head == NULL || Head->Next == NULL) //If no Head or no node to pop
  {
    return Head->Value;
  }

  type* Point = GetItem(0);

  if (Point == NULL)
  {
    Success = false;
    return Head->Value;
  }

  type Value = *Point;

  DeleteItem(0);

  return Value;
}

/* Overload bypassing Success */

template<class type>
type CLinkedList<type>::Pop()
{
  bool Success;
  return Pop(Success);
}

/***************************************************
Enqueue()
 Adds a node the the end of the list.
 Inputs: The value of the node to insert
 Output: Returns a boolean value indicating success
***************************************************/

template<class type>
bool CLinkedList<type>::Enqueue(type Value)
{
  node* NewNode = new node;

  if (Head == NULL || NewNode == NULL)
    return false;

  NewNode->Value   = Value;
  NewNode->Next    = Head;
  NewNode->Prev    = Head->Prev;

  (Head->Prev)->Next = NewNode; //(Head->Prev) is the last node
  Head->Prev         = NewNode; //Head's prev is the last node

  return true;
}

/***************************************************
Dequeue()
 Retrieves and removes the value at the top of the 
 list.
 Inputs: The value of the node to retrieve
 Output: Returns a boolean value indicating success
***************************************************/

template<class type>
type CLinkedList<type>::Dequeue(bool& Success)
{
  return Pop(Success);
}

/* Overload bypassing Success */

template<class type>
type CLinkedList<type>::Dequeue()
{
  bool Success;
  return Pop(Success);
}

#endif
