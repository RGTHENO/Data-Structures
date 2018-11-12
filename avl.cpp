#include <iostream>
#include <algorithm>

///PROBAR EL SIGUIENTE CASO:
///INGRESAR 183729456
///BORRAR  3 9 2

using namespace std;
 
template <class T>
class mayor{
	public:

		inline bool operator()(T a, T b){
			return a>b;
		}
};

template <class T>
class minor{
	public:
		inline bool operator()(T a, T b){
			return a<b;
		}
};
 

template <class T>
class Node{
		
	public:
		
		T x;
		Node<T>* child[2];
		Node<T>* parent;
		int altura;

		Node(T x){
			this->x=x;
			child[0] = child[1] = 0;
			parent = 0;
			altura = 1;
		}

		~Node(){
			child[0] = child[1] = 0;
			parent = 0;
			altura = 0;
		}

};

template <class T, class S>
class Btree {
	public:

		Node<T>* root;
		S cmp;

		Btree(){
			root = 0;
		}

		bool Find(T x, Node<T>** &p);
		bool Insert(T x);
		bool Remove(T x);
		void PrintTreeInOrder(Node<T>*p);
		int Height( Node<T>** p);
		void UpdateHeight( Node<T> **p);
		bool BalancearAVL(Node<T>**p);
		bool Find_ViolateNode( Node<T>** &p);
		void RightRotation( Node<T> ** &A); 
		void LeftRotation( Node<T> ** &A);
		bool LeftRightRotation(Node<T>**&A);
		bool RightLeftRotation(Node<T>**&A);
		int getAltura(Node<T>* node);
		
		
		void display(Node<T> *ptr, int level);
		
};

template <class T, class S>
bool Btree<T,S>::Find(T value, Node<T> ** &p){
    
    for(	p = &root; 
		    (*p) && (*p)->x != value && ( (*p)->child[ cmp((*p)->x, value)] );   /// Vamos a reajustar este metodo find para que me devuelva el enlace anterior
																				/// i.e un puntero al enlace padre del elemento que quiero insertar
			p = &( (*p)->child[cmp( (*p)->x, value) ] )
		);
    
    return (*p) == 0;
};

 

template<class T, class S>
void Btree<T,S>::UpdateHeight(  Node<T> ** p){ ///Estoy mandando el nodo actual (hijo) y quiero actualizar las alturas de sus ancestros
	
	
	Node<T>* current_node = *p;
	Node<T>* temp;  
	  
	 
	for(temp = current_node; temp;  temp = temp->parent ){  
		
			if(! (temp->parent) ){ ///Si el padre del actual nodo es NULO, entonces significa que nuestro temp apunta a la raiz del arbol
				root =temp; 
			}
			
			if( !temp->child[0] && !temp->child[1] ){ ///CUANDO LOS DOS HIJOS SON NULOS
				temp->altura = 1;
			}		
			else{	 
					temp->altura = 1 + max( getAltura(temp->child[0]), getAltura(temp->child[1] ) ); 	 
			}
	}    
}

  
template<class T, class S>
void Btree<T,S>::RightRotation( Node<T> **&A){
	
	///cout<<"Vamos a hacer Right Rotation\n";
	
	Node<T> *B       = (*A)->child[0];
	Node<T> *b_Right = B->child[1]; 
	Node<T> *copy_A  = *A;
	 	  
	(B)->parent = (*A)->parent; 
	
	Node<T> * padreA = (*A)->parent;
	int dir;
	if( padreA){ 
		if( padreA->child[0]){
			
			if( padreA->child[0]->x == (*A)->x){
				dir = 0;
			}
			else{
				dir = 1;
			} 
		}
		else{
			dir = 1;
		}
		(*A)->parent->child[dir] = B;	
	}
	
	B->child[1] = copy_A; 
	copy_A->parent = B;
	copy_A->child[0] = b_Right;
	 
	UpdateHeight(&copy_A); //means for the new right child of B who is the root of this subtree 
	UpdateHeight(&B);
	
}



template<class T, class S>
void Btree<T,S>::LeftRotation( Node<T> **&A){
	///cout<<"Vamos a hacer left rotation\n";
	 
	Node<T> *copy_A = (*A);
	Node<T> *B      =  (*A)->child[1];
	Node<T> *b_Left =   (B)->child[0];
	 
	B->parent =(*A)->parent;
	B->child[0] = copy_A;
	
	Node<T> * padreA = (*A)->parent;
	int dir; 
	 
	 
	if(padreA){
		if( padreA->child[0]){ 
			if( padreA->child[0]->x == (*A)->x){
				dir = 0;
			}
			else{
				dir = 1;
			} 
		}
		else{
			dir = 1;
		} 
		(*A)->parent->child[dir] = B;
	}
	
	(copy_A)->parent = B;
	(copy_A)->child[1] = b_Left;
	 
	UpdateHeight(&copy_A); //A means for b_left
	UpdateHeight(&B);
	
}

template<class T, class S>
bool Btree<T,S>::RightLeftRotation(Node<T>** &p){
	 
	///cout<<"Caso RIghtLeftRotation\n";
	
	Node<T> *A = (*p);
	Node<T> *B = A->child[1];
	Node<T> *C = B->child[0];
	  
	A->child[1] = C->child[0];
	if(C->child[0]){
		C->child[0]->parent = A;	
	}
	 
	B->child[0] = C->child[1];
	if(C->child[1]){
		C->child[1]->parent = B;
	}
	
	C->child[0] = A;
	C->child[1] = B;
	C->parent = A->parent;
	
	
	Node<T> * padreA = A->parent;
	int dir; 
	
	if( padreA){ ///Siempre que el padre de A exista
		if( padreA->child[0]){ 
			if( padreA->child[0]->x == A->x){
				dir = 0;
			} 
		}
		else{
			dir = 1;
		}
		A->parent->child[dir] = C;
	}
	 
	A->parent = C; 
	B->parent = C;
	 
	
	 UpdateHeight(&B);
	 UpdateHeight(&A);
	 UpdateHeight(&C);
	 
	
	return 1;
}


template<class T, class S>
bool Btree<T,S>::LeftRightRotation(Node<T>**&p){
	
	///cout<<"Caso left right rotation\n";
	
	Node<T> *A = (*p);
	Node<T> *B = A->child[0];
	Node<T> *C = B->child[1];
	 
	B->child[1] = C->child[0];
	
	if(C->child[0]){ 
		C->child[0]->parent = B; 
	}
	
	A->child[0] = C->child[1]; 
	if(C->child[1]){ 
		C->child[1]->parent = A;
	}
	 
	C->child[0] = B;
	C->child[1] = A;
	C->parent = A->parent;
	B->parent = C;
	
	Node<T> * padreA = A->parent;
	int dir; 
			
	if(padreA){		
				
				if( padreA->child[0]){
					
					if( padreA->child[0]->x == A->x){
						dir = 0;
					}
					else{
						dir  = 1;
					}
				
				}
				else{
					dir = 1;
				} 
			A->parent->child[dir] = C;
	}		
		 
	 A->parent = C;	
	  
	 UpdateHeight(&B); 
	 UpdateHeight(&A);
	 UpdateHeight(&C); 
	 
	return 1;
}


template<class T, class S>
int Btree<T,S>::getAltura(Node<T>* node){
	
	if(!node) return 0;
	
	return node->altura;
}



template<class T, class S>
bool Btree<T,S>::Find_ViolateNode( Node<T>** &p){ ///Mando el nodo que acabo de insertar y empezara a visitar a sus ancestros
									  ///Hasta encontrar el ancestro que viole la regla del balanceo
			 
	///cout<<"	VAMOS A ENCONTRAR EL NODO QUE VIOLA LA REGLA\n";		
	p = & ( (*p)->parent);
	int diferenciaAlturas;

	while( *p ){
		
		diferenciaAlturas = getAltura( (*p)->child[1] )- getAltura( (*p)->child[0] );
		
		if ( diferenciaAlturas < -1 || diferenciaAlturas>1 ){
			///cout<<"	estoy dentro de Find violateNode, y el nodeo que vioa la regla es:"<< (*p)->x<<endl;
			return 1; //This sentence means que se encontro a Algun NODO QUE VIOLA la regla del balanceo
		}
		p = &( (*p)->parent );		
	}
		 
	return 0;
}




template<class T, class S>
bool Btree<T,S>::BalancearAVL(  Node<T>** p_current){
		
		Node<T>** p = p_current; ///RECUERDA QUE p_current es un puntero al ultimo que se acaba de insertar
		 
		int diferencia;
		
		if( Find_ViolateNode(p) ){  ///REMEMBTER that Find_ViolateNode returns the node that causes the violation of the balance rule
		 
				///cout<<"Vamos a Balancear porque se ha detectado un desbalance en el nodo : "<< (*p)->x <<"\n";
				diferencia = getAltura( (*p)->child[1] ) - getAltura( (*p)->child[0] );
				 
				///cout<<"La diferencia es :"<<diferencia<<endl;
				
				int alturaNietoDerecho;
				int alturaNietoIzq;
						
				if(diferencia>1 ){ ///SIgnifica que subarbol derecho pesa mas que el izquierdo
						
						
						alturaNietoDerecho = getAltura( (*p)->child[1]->child[1] );
						alturaNietoIzq = getAltura( (*p)->child[1]->child[0] );
						 
						if( alturaNietoIzq > alturaNietoDerecho ){
							///caso RightLeftRotation  
							RightLeftRotation(p); 
						}
						else{ 
							///cout<<"Caso : Left Rotation\n";
							LeftRotation(p);
						} 
				} 
				else if(diferencia <-1) {		///Significa que el subarbl izquierdo pesa mas que el derecho
						 
						alturaNietoDerecho = getAltura( (*p)->child[0]->child[1] );
						alturaNietoIzq = getAltura ( (*p)->child[0]->child[0] );
						 
						if( alturaNietoDerecho > alturaNietoIzq){
							///cout<<"Left Right Rotation\n"; 
							LeftRightRotation(p); 
						}
						else{
							///cout<<"Right Rotation\n";
							RightRotation(p);
						
						}
						 
				}	
				 
		}
		 
		return 1;
}

template <class T, class S>
bool Btree<T,S>::Insert(T value) {
     
    //cout<<"Voy a insertar "<<value<<" :)"<<endl;
    
    
    if ( root == NULL ){
		
			root = new Node<T> (value);
			return 1;
	}
    
    Node<T>** p;
    if (Find(value,p)) return 0;  ///Este metodo me devuelve un puntero al enlace padre del elemento que quiero insertar
								  ///Remember that the last instruction inside our find  method is :  return (*p)==0;
	      
	Node<T>* nuevo_nodo = new Node<T> (value);
	nuevo_nodo->parent = (*p);
	
	int pos = cmp( (*p)->x,value );
	
	(*p)->child[ pos ] = nuevo_nodo; ///Recuerda que "p" es un puntero PADRE
	 
	 
	UpdateHeight( &( (*p)->child[pos] )); 
	BalancearAVL( &( (*p)->child[pos] ));
	
	 
		
    return 1;
}

 

template <class T, class S>
Node<T> ** LeftMoreRight( Node<T>** sub_root){
    
    Node<T> **p= sub_root;
    p = &((*p)->child[0]);
    
    while ((*p)->child[1]){
     
        p = &( (*p)->child[1] );
    }
    
    return p;
};


template <class T, class S>
bool Btree<T, S>::Remove(T x) {
     
    Node<T> ** p;
    Node<T> ** q;
    
    if(Find(x,p)) return  0;  ///RECUERD QUE LA ULTIMA INSTRUCCIN DEL FIND es:  (*p) == 0
     
    ///caso con 2 hijos
    if( (*p)->child[0] && (*p)->child[1]){
	    q = LeftMoreRight<T,S>(p); 
        (*p)->x = (*q)->x;     
        p = q;  
    }
   ///caso 0 hijos y 1 hijo juntos
	Node<T>* parent_Rescue; 
	Node<T>* temp; 
	temp = *p;   
	
    /// CASO 0 HIJOS
	if( !(*p)->child[0] && !(*p)->child[1] ){ ///Cuando el nodo que vamos a eliminar no tiene hijos	
		///Vamos a rescatar el padre del nodo que vamos a eliminar, si este nodo no tiene hijos		
	 
		parent_Rescue = (*p)->parent; 
		//(*p) = (*p)->child[ (*p)->child[1] != 0 ];
		(*p) = 0;
		delete temp;
	
		UpdateHeight(&parent_Rescue); 
		BalancearAVL(&parent_Rescue);
		
    }
    else{ ///CASO 1 HIJO 
		 
		(*p) = (*p)->child[ (*p)->child[1] != 0 ];
		(*p)->parent = temp->parent;
		delete temp;
	
		UpdateHeight(p); 
		BalancearAVL(p);	
	} 
    
    return 1;
}
 
template <class T, class S>
void Btree<T,S>::PrintTreeInOrder(Node<T>*p) {
    
    if(!p){
		 return;
    }
   
    PrintTreeInOrder(p->child[0]);
    cout<<" p:"<<p->x<<" y su altura es: "<<p->altura<<endl;
    PrintTreeInOrder(p->child[1]);
    
}

template<class T, class S>
void Btree<T,S>::display(Node<T> *ptr, int level)

{

    int i;

    if (ptr != NULL)

    {

        display(ptr->child[1], level + 1);

        printf("\n");

        if (ptr == root)

            cout << "Root -> ";

        for (i = 0; i < level && ptr != root; i++)

            cout << "        ";

        cout << ptr->x;

        display(ptr->child[0], level + 1);

    }

}




int main() {
 
    Btree<int, minor<int> > *Arbol= new Btree<int, minor<int> >();
      
    Arbol->Insert(1);
	Arbol->Insert(8);
    Arbol->Insert(3);
    Arbol->Insert(7);    
    Arbol->Insert(2);
    Arbol->Insert(9);
     Arbol->Insert(4);
    Arbol->Insert(5);
     Arbol->Insert(6);
    
    cout<<"Raiz : "<<Arbol->root->x<<" y su altura es :"<<Arbol->root->altura<<endl; 
      
    ///cout<<"Raiz del arbol :"<<Arbol->root->x<<endl;
     cout<<"\nDespues de hacer las inserciones\n";  
    Arbol->PrintTreeInOrder(Arbol->root);
    
    Arbol->Remove(3);
    Arbol->Remove(9);
    Arbol->Remove(2);
    cout<<"Despues de hacer las eliminaciones el arbol queda :\n"; 
    Arbol->PrintTreeInOrder(Arbol->root);
    cout<<"La nueva raiz es :"<<Arbol->root->x<<" y su altura es:"<<Arbol->root->altura<<endl;
     
    
    Arbol->display(Arbol->root,1);
    return 0;

}
