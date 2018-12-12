#include <iostream>
#include <vector> 
#include <random>
#include <unordered_map>
#include <set>
#include <ctime>

#include <numeric> 
#include <limits>

typedef unsigned int ui;


using namespace std;

float closest(std::vector<float>& vec, float value) {
	
    vector<float>::iterator it = std::lower_bound(vec.begin(), vec.end(), value);
    
    if (it == vec.end()) { return -1; }

	float result = *it;
	
	vec.erase(it);
	
	return result;
}
 
 

void print_vector_int(vector<int> pos){
	
	
	cout<<"\n";
	for(ui i=0; i<pos.size(); i++){
		cout<<pos[i]<<" ";
	
	}
	cout<<"\n";


}
 
 


class HashTable{

	public:
	
		int d; /// data_dimensions
		int M; /// Size "M" of the Hash Table
		int k; /// "k" random vectors
		vector< vector<float> > projections; ///Projections is a matrix of "k" vector randoms where each vector is "d" dimensional. 	
		
		unordered_map<float, vector<string> > hash_table_label;
        unordered_map<float, vector<int> > hash_table_pos;      ///Where int represents position of a audio in the general vector 

		vector<float> keysOfEachBin;

		HashTable(int cant_d, int size_HashTable, int k_projections): d(cant_d), M(size_HashTable), k(k_projections){
					
					initialize_projections( );  
					
		}  
				
		void initialize_projections( ){
				std::random_device rd;
				std::mt19937 gen(rd());
				std::normal_distribution<float> dis(0,1);
		        
				for(int i=0; i<this->k; i++){
					 
						vector<float> p_i;
						p_i.reserve(this->d); ////Usamos el RESERVE para hacer más eficiente el push_back
		        
						for(int j=0; j<this->d; j++){
								p_i.push_back( dis(gen) );
						}		
					
						projections.push_back(p_i); 
				}
		}	
 
 
		void get_keys(){
			
			for(auto key_value : hash_table_pos){
				
				keysOfEachBin.push_back(key_value.first);
			}
			
			
		}
 
		
		inline float dot_product(vector<float> &a, vector<float> &b){ 
		
				float result = std::inner_product(     a.begin(), a.end(),
														b.begin(), 0.0
													);
				return result;
		}
		
		inline float compute_hash_value( vector<float> &point_data  ){
				
				float key = 0;
				vector<float> p_i; ///VECTOR TEMPORAL ALEATORIO OBTENIDO DE LAS PROJECCIONES
				
				for( ui i=0; i< projections.size(); i++){  ///p_i is random projection vector  
						p_i = projections[i];
						
						if( dot_product(p_i, point_data) > 0 ){
							 key += 1<<i;		
						}
				} 
				return key;
		}
		
				
		inline bool insert(vector<float> &p_data, int &pos  ){ /// OJO:pos actua como un puntero de nuestro elemento
			
			float hash_value = compute_hash_value(p_data); 
			
			hash_table_pos[hash_value].push_back(pos);
						
			return true;
		}
		
		
		 
		vector<int> get_neighbours( vector<float> &point_query){
			
			float key = compute_hash_value(point_query);
			
			return hash_table_pos[key];
		} 
		
		/***
		vector<int> get_neighbours( float &hashValueOfQuery){
			
			return hash_table_pos[hashValueOfQuery];
		}
		**/
		 
};
 

class LSH{
		
		public:
		
			 vector<HashTable*> tables;
			 
			 int d; ///number of dimensions of our data
			 int M; ///Size of a HashTable
			 int k; ///Number of Projections  
			 int L; ///Represent the number of HashTables
			 
			 
			 LSH(int d_input, int M_input,int k_input, int L_input): d(d_input), M(M_input), k(k_input), L(L_input){
					tables.reserve(L_input);
						
					for(int i=0; i<L_input; i++){	
						tables.push_back( new HashTable(d, M_input, k_input) );
					}
							
			 }
			   
			bool insert( vector<float>& p_data, int pos){
			 	 	 
			 	 	 HashTable* table_i;
			 	 	 for(unsigned int i=0; i<tables.size(); i++){
						 table_i = tables[i];
						 table_i->insert(p_data, pos);
					 }
			 	 	   
					 return true;
			 }
	 
			set<int> query(vector<float> &p_query ){
				set<int> neighbours;
				
				for(auto table_i: tables){
						vector<int> result;
				        result = table_i->get_neighbours(p_query);
						neighbours.insert(result.begin(), result.end());
				}
				
				return neighbours;
			}
			
			
			
			set<int> query_KNN( vector<float> &p_query,unsigned int value_knn){ 
					
					set<int> knn_set;
					vector<float> hashValuesOfEachTable;
					hashValuesOfEachTable.reserve( tables.size() );
				
					for(unsigned int i=0; i<tables.size(); i++){ 
						hashValuesOfEachTable.push_back( tables[i]->compute_hash_value(p_query) );
					}
				     
				   while( knn_set.size() < value_knn){
							int i=0;
							for(auto table_i: tables){
									float tempHashValue; 
									tempHashValue = closest( table_i->keysOfEachBin, hashValuesOfEachTable[i]);
									if(tempHashValue != -1 ){ /// When I still have more upper closer elements	
										vector<int> temp = table_i->hash_table_pos[tempHashValue];
										knn_set.insert( temp.begin(), temp.end() );
									}														 
									i++;  
									if(knn_set.size() >= value_knn) break;
							} 
					} 
					
					///WE ARE GOING TO DELETE SOME ELEMENTS REMAINING 
					float lengthKNN_set = knn_set.size();
					int elementsToErase = abs( lengthKNN_set - value_knn );
							
					std::set<int>::iterator it = knn_set.begin();
					
					for(int i=0; i<elementsToErase; i++){
						  knn_set.erase(it);
						  it++;
					}
					return knn_set;
			}
				 
			
};
  
vector<vector<float> > DATA; 

int main(){
	
	int d, M, k, L;
	
	d = 91; ///Number of dimensions
	M = 5;
	k = 11; ///Number of Projection Vectors
	L = 5;  ///Number of Table Hash
	
	LSH* my_LSH =  new LSH(d,M,k,L); ///d, M, k, L

	float numRowsData = 515345;
	float x;
	
	for(register_t i=0; i<500000; i++){  ///MAXIMA CANTIDAD 515345
	
			vector<float> p;
			p.reserve(d);
			
			for(int j=0; j<d; j++){
	
				cin>>x;
				p.push_back(x);
			}
			
			DATA.push_back(p);
	}
	
	vector<float> p_data; ///UN VECTOR TEPORAL PARA ALMAENAR C/PUNTO
	
	clock_t begin = clock();  
	 
	for(int i=0; i<500000; i++){
		
		p_data = DATA[i];
		my_LSH->insert(p_data, i); 
		
	} 
	
	clock_t end = clock();
  			 
    cout<<"INSERTION for "<<DATA.size()<<" elementos : "<<float(end - begin) / CLOCKS_PER_SEC<<endl; 
  	cout<<"FIN DE LA INSERCION \n";	
  	
	  
  	///GET KEYS PREPROCESSING FOR MAKING QUERIES
  	for(auto table_i : my_LSH->tables){
		table_i->get_keys();
	}		
	
	
	cout<<"EMPEZAREMOS CON LA QUERY\n\n";	  
	set<int> neighbours;
		 
	begin = clock();
	int k_vecinos = 1000;
	neighbours = my_LSH->query_KNN(DATA[123], k_vecinos);
	
	end = clock(); 
	
    cout<<" QUERY For "<<DATA.size()<<" elementos, K ="<<" : "<<k_vecinos<<"Time :"<<double(end - begin) / CLOCKS_PER_SEC<<endl; 
  	
  	//**/
  	
	/**
	cout<<"Los vecinos son:\n";
	vector<int> result;
	result.insert(result.end(), neighbours.begin(), neighbours.end());
	cout<<"SE RECUPERARON : "<<result.size()<<endl;
	print_vector_int(result);
	**/ 


	return 0;

}

