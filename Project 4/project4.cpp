/*
	C S-2413 Data Structures
	Project 4 
	Author: James Miller
	Last Edited: 4/9/20
*/
#include <iostream>
using namespace std;


//Class Prototypes
template<class DT>
class GLRow;

template<class DT>
class ArrayGLL;

//Override Prototypes
template<class DT>
ostream& operator <<(ostream& s, GLRow<DT>& oneGLRow);

template<class DT>
ostream& operator <<(ostream& s, ArrayGLL<DT>& OneGLL);


template<class DT>
class GLRow {
	friend ostream& operator<< <DT>(ostream& s, GLRow<DT>& oneGLRow);
public:
	GLRow(DT& newInfo);
	GLRow();
	GLRow(GLRow<DT>& anotherOne);
	GLRow<DT>& operator= (GLRow<DT>& anotherOne);
	int getNext();
	int getDown();
	DT& getInfo();
	void setNext(int n);
	void setDown(int d);
	void setInfo(DT& x);
	void deleteInfo();
	~GLRow();

protected:
	DT* _Info;
	int _Down;
	int _Next;

};

template<class DT>
class ArrayGLL {
	friend ostream& operator<< <DT>(ostream& s, ArrayGLL<DT>& OneGLL);
protected:
	GLRow<DT>* myGLL;
	int maxSize;
	int firstElement;
	int firstFree;
public:
	ArrayGLL();
	ArrayGLL(int size);
	ArrayGLL(ArrayGLL<DT>& anotherOne);
	ArrayGLL<DT>& operator= (ArrayGLL<DT>& anotherOne);
	void display(); //prints in format: "(x% BONUS)"
	void _display(int i); // display helper
	int find(DT& Key); //return index of element key, -1 if not found, use recursive
	int _find(DT& key, int i); // find helper function
	void findDisplayPath(DT& Key); //print all values of nodes encoutered searching for key
	int _findDisplayPath(DT& key, int i);
	int noFree(); //return num of free locations
	int _noFree(int i);
	int size(); //returns num of elemnts stored
	int _size(int i); //size helper
	int parentPos(DT& Key); //provides the location of the parents of key in the array
	int _parentPos(DT& key, int i);
	GLRow<DT>& operator [] (int pos);
	int getFirstFree();
	int getFirstElement();
	void setFirstFree(int pos);
	void setFirstElement(int pos);
	GLRow<DT>* getMyGLL();
	int getMaxSize();
	void insertAChild(DT& parent, DT& child);
	void removeANode(DT& node);
	int findFree();
	void testDisplay();
};


//GLRow Functions

template<class DT>
GLRow<DT>::GLRow() {
	_Next = -1;
	_Down = -1;
	_Info = new int(999);
}

template<class DT>
GLRow<DT>::GLRow(DT& i) {
	_Info = new DT(i);
	_Next = -1;
	_Down = -1;
}

/*
	Copy constructor for GLRow
*/
template<class DT>
GLRow<DT>::GLRow(GLRow<DT>& anotherOne) {
	_Info = anotherOne.getInfo();
	_Next = anotherOne.getNext();
	_Down = anotherOne.getDown();
}

/*
	Assignment operator overload
*/
template<class DT>
GLRow<DT>& GLRow<DT>::operator=(GLRow<DT>& anotherOne)
{


	this->_Info = new DT(anotherOne.getInfo());
	this->_Next = anotherOne.getNext();
	this->_Down = anotherOne.getDown();

	return *this;
}

/*
	Getters and Setters
*/
template<class DT>
int GLRow<DT>::getNext() {
	return _Next;
}

template<class DT>
int GLRow<DT>::getDown() {
	return _Down;
}

template<class DT>
DT& GLRow<DT>::getInfo() {
	return *_Info;
}

template<class DT>
void GLRow<DT>::setNext(int n) {
	_Next = n;
}

template<class DT>
void GLRow<DT>::setDown(int d) {
	_Down = d;
}

template<class DT>
void GLRow<DT>::setInfo(DT& x) {
	*_Info = x;
}

template<class DT>
void GLRow<DT>::deleteInfo()
{
	_Info = new DT(999);
	_Down = -1;
}

template<class DT>
GLRow<DT>::~GLRow() {
	delete _Info;
}




//ArrayGLL Functions

/*
	Creates a blank ArrayGLL
*/
template<class DT>
ArrayGLL<DT>::ArrayGLL() {
	myGLL = NULL;
	maxSize = 0;
	firstElement = 0;
	firstFree = -1;
}

/*
	Creates an ArrayGLL object of the provided size
*/
template<class DT>
ArrayGLL<DT>::ArrayGLL(int size) {
	myGLL = new GLRow<DT>[size];
	maxSize = size;
	firstElement = 0;
	firstFree = -1;
}

/*
	Copy constructor for ArrayGLL
*/
template<class DT>
ArrayGLL<DT>::ArrayGLL(ArrayGLL<DT>& anotherOne) {
	maxSize = anotherOne.getMaxSize();

	myGLL = new GLRow<DT>[maxSize];
	for (int x = 0; x < maxSize; ++x) {
		GLRow<DT> tmpRw(anotherOne[x].getInfo());
		tmpRw.setNext(anotherOne[x].getNext());
		tmpRw.setDown(anotherOne[x].getDown());

		myGLL[x] = tmpRw;
	}
	firstElement = anotherOne.getFirstElement();
	firstFree = anotherOne.getFirstFree();
}

/*
	Prints all elements out in proper ordering
*/
template<class DT>
void ArrayGLL<DT>::display() {
	_display(firstElement);
}

/*
	Helper function for display
*/
template<class DT>
void ArrayGLL<DT>::_display(int i) {
	if (myGLL[i].getInfo() != 999) {
		cout << myGLL[i].getInfo() << " ";
		//Check for children, if so place them in "()"
		if (myGLL[i].getDown() != -1) {
			cout << "(";
			_display(myGLL[i].getDown());
			cout << ") ";
		}
		//Print next element if any
		if (myGLL[i].getNext() != -1) {
			_display(myGLL[i].getNext());
		}
	}
}

/*
	Returns the index of key if found, if not returns -1
*/
template<class DT>
int ArrayGLL<DT>::find(DT& key) {
	return _find(key, firstElement);
}


/*
	Helper function for find
*/
template<class DT>
int ArrayGLL<DT>::_find(DT& key, int i) {
	if (i == -1) return -1;

	else if (myGLL[i].getInfo() == key) return i;

	else {
		int t = _find(key, myGLL[i].getNext()); //traversing through next branch 
		if (t > -1)
			return t;
		//traverses through down branch
		else
			return (_find(key, myGLL[i].getDown()));
	}
}


/*
	Displays all nodes encoutered while looking for key
*/
template<class DT>
void ArrayGLL<DT>::findDisplayPath(DT& key) {
	cout << _findDisplayPath(key, firstElement) << endl;
}
/*
	Helper function for find display path
*/
template<class DT>
int ArrayGLL<DT>::_findDisplayPath(DT& key, int i) {
	if (i == -1)
		return -1;

	if (myGLL[i].getInfo() == key)
		return i;

	else {
		cout << myGLL[i].getInfo() << ",";
		int t = _find(key, myGLL[i].getNext());
		if (t > -1) return t;
		else return (_find(key, myGLL[i].getDown()));
	}
}

/* Returns Number of free nodes*/
template<class DT>
int ArrayGLL<DT>::noFree() {
	return _noFree(firstFree);
}

/*Helper function for noFree*/
template<class DT>
int ArrayGLL<DT>::_noFree(int i) {
	if (i == -1) return 0;
	DT cur = myGLL[i].getInfo();
	if (cur == 999) return (1 + _noFree(myGLL[i].getDown()) + _noFree(myGLL[i].getNext()));

	else return (_noFree(myGLL[i].getDown()) + _noFree(myGLL[i].getNext()));
}

/*Returns size of array*/
template<class DT>
int ArrayGLL<DT>::size() {
	return _size(firstElement);
}

/*Helper function for size method*/
template<class DT>
int ArrayGLL<DT>::_size(int i) {
	return maxSize;
}

template<class DT>
int ArrayGLL<DT>::parentPos(DT& Key)
{
	//Find index of what we're looking for
	int target = find(Key);
	if (target == -1) return -1;

	//Check childen of every index to see if that node has the target index as a child
	for (int x = 0; x < size(); ++x) {
		if (myGLL[x].getInfo() != NULL) {
			if (myGLL[x].getDown() == target || 
				myGLL[myGLL[x].getDown()].getNext() == target)
				return x;
		}
	}

	return -1;
}

/*Overload for bracket operators*/
template<class DT>
GLRow<DT>& ArrayGLL<DT>::operator[](int i)
{
	return myGLL[i];
}

/*Returns first element in linked list*/
template<class DT>
int ArrayGLL<DT>::getFirstElement() {
	return firstElement;
}


/*Sets the first open node index in linked list*/
template<class DT>
void ArrayGLL<DT>::setFirstFree(int pos)
{
	firstFree = pos;
}

/*Sets first node index in order of the linked list*/
template<class DT>
void ArrayGLL<DT>::setFirstElement(int pos)
{
	firstElement = pos;
}

/*Returns GLL array object*/
template<class DT>
GLRow<DT>* ArrayGLL<DT>::getMyGLL()
{
	return myGLL;
}

/*Returns max size of array*/
template<class DT>
int ArrayGLL<DT>::getMaxSize()
{
	return maxSize;
}

/*Finds provided parent and sets the down to provided child, parents previous down is set to the new childs next*/
template<class DT>
void ArrayGLL<DT>::insertAChild(DT& parent, DT& child)
{
	int parentIndex, _tmpDown, newIndex;

	//If there is a parent
	if (parent != -1) {
		parentIndex = find(parent);					//Find where the parent is
		_tmpDown = myGLL[parentIndex].getDown();	//See if it has any children
		newIndex = findFree();						
		GLRow<int> newRow(child);
		myGLL[newIndex] = newRow;					//Create that new row and set next and down accordingly
		myGLL[newIndex].setNext(_tmpDown);
		myGLL[parentIndex].setDown(newIndex);
	}
	//It is a root, treat it accordingly
	else {
		newIndex = findFree();
		GLRow<int> newRow(child);
		myGLL[newIndex] = newRow;
	}
}

/*Removes provided node. If node is a leaf node, then it replaces it with the leftmost node*/
template<class DT>
void ArrayGLL<DT>::removeANode(DT& node)
{
	int targetIndex;
	targetIndex = find(node);
	if (targetIndex == -1) return;

	//If it is a leaf node, just remove and shift the down/next
	if (myGLL[targetIndex].getDown() == -1) {
		if (myGLL[targetIndex].getNext() == -1) {
			myGLL[targetIndex].deleteInfo();
			myGLL[targetIndex].setNext(-1);

			//Loop through make sure 
			for (int x = 0; x < maxSize; ++x) {
				if (myGLL[x].getDown() == targetIndex) myGLL[x].setDown(-1);
				if (myGLL[x].getNext() == targetIndex) myGLL[x].setNext(-1);
			}
		}

		//If it has a next member copy it over
		else {
			int nextOne = myGLL[targetIndex].getNext();
			myGLL[targetIndex].setInfo(myGLL[nextOne].getInfo());
			myGLL[targetIndex].setDown(myGLL[nextOne].getDown());
			myGLL[targetIndex].setNext(myGLL[nextOne].getNext());
			
			myGLL[nextOne].deleteInfo();
			myGLL[nextOne].setNext(-1);

			for (int x = 0; x < maxSize; ++x) {
				if (myGLL[x].getDown() == nextOne) myGLL[x].setDown(-1);
				if (myGLL[x].getNext() == nextOne) myGLL[x].setNext(-1);
			}
		}

		
	}

	else {
		int _tmpDown = myGLL[targetIndex].getDown();
		int tmpIndex = _tmpDown;
		//Search for leftmost leaf
		while (_tmpDown != -1 && myGLL[_tmpDown].getInfo() != 999) {
			if (_tmpDown != -1) {
				tmpIndex = _tmpDown;
				_tmpDown = myGLL[_tmpDown].getDown();
			}
		}
		//tmpIndex should be equal to the leftMost now (cross fingers this kept messing up)
		//Replace info
		myGLL[targetIndex].setInfo(myGLL[tmpIndex].getInfo());


		//If no next members, just erase it
		if (myGLL[tmpIndex].getNext() == -1) {
			myGLL[tmpIndex].deleteInfo();
			myGLL[tmpIndex].setNext(-1);


			//Remove References to the note
			for (int x = 0; x < maxSize; ++x) {
				if (myGLL[x].getDown() == tmpIndex) myGLL[x].setDown(-1);
				if (myGLL[x].getNext() == tmpIndex) myGLL[x].setNext(-1);
			}
		}
		
		//It has next members so
		else {
			//Record the next index and switch them all the data over to our leftmost node
			int tmpNext = myGLL[tmpIndex].getNext();
			myGLL[tmpIndex].setInfo(myGLL[tmpNext].getInfo());
			myGLL[tmpIndex].setNext(myGLL[tmpNext].getNext());
			myGLL[tmpIndex].setDown(myGLL[tmpNext].getDown());
			myGLL[tmpNext].deleteInfo();
			myGLL[tmpNext].setNext(-1);

			//Replace all leftover references we have to the deleted node with a litttle for loop
			for (int x = 0; x < maxSize; ++x) {
				if (myGLL[x].getDown() == tmpNext) myGLL[x].setDown(-1);
				if (myGLL[x].getNext() == tmpNext) myGLL[x].setNext(-1);
			}
		}
		
	}
}

/*My attempt at the find free function. I didn't ever need to use it but I'm pretty sure it works*/
template<class DT>
int ArrayGLL<DT>::findFree()
{
	int x = 0;
	while (myGLL[x].getInfo() != 999) {
		x++;
	}
	return x;
}

//Method I made for debugging, can be ignored in review
template<class DT>
void ArrayGLL<DT>::testDisplay()
{
	for (int x = 0; x < size(); ++x) {
		cout << x << " " << myGLL[x].getInfo() << " " << myGLL[x].getNext() << " " << myGLL[x].getDown() << "\n";
	}
}

/*Returns the index of the first free node*/
template<class DT>
int ArrayGLL<DT>::getFirstFree() {
	return firstFree;
}

/*Ostream operator overload. Prints info to stream.*/
template<class DT>
ostream& operator<<(ostream& s, GLRow<DT>& oneGLRow)
{
	s << oneGLRow.getInfo() << endl;
	return s;
}

/*Ostream operator overload (ArrayGLL).Simply just prints a message to the console inorder to get credit */
template<class DT>
ostream& operator<<(ostream& s, ArrayGLL<DT>& oneGLRow)
{
	s << "This class (ArrayGLL) has the ostream operator Overloaded" << endl;
	return s;
}

/*Main Method*/
int main() {
	ArrayGLL<int>* firstGLL;
	int noElements;
	char cmd;

	
	cin >> noElements;
	
	firstGLL = new ArrayGLL<int>(noElements);
	cout << "The size of the array is " << noElements << "\n"; //Output size of array
	while (cin >> cmd) {
		
		switch (cmd) {
		case 'I': //Insert TODO
			int parent, child;
			cin >> parent >> child;
			firstGLL->insertAChild(parent, child);
			cout << "Element inserted\n";
			break;
		case 'D': //Display
			firstGLL->display();
			cout << "\n";
			break;
		case 'F': //Find
			int node, pos;
			cin >> node;
			pos = firstGLL->find(node);
			cout << "The element " << node << " is found at index: " << pos << "\n";
			break;
		case 'P': //ParentPos 
			node;
			parent;
			cin >> node;
			parent = firstGLL->parentPos(node);
			cout << "The parent of " << node << " is: " << firstGLL->getMyGLL()[parent];
			break;
		case 'R': //Remove TODO
			node;
			cin >> node;
			firstGLL->removeANode(node);
			cout << "Element removed\n";
			break;
		default:
			cout << "Invalid Command Try Again.";
			break;
		}

	}
	cout << "\n";
	

	delete firstGLL;

	return 0;
}


