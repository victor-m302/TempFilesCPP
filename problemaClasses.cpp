
#include <vector>
#include <iostream>

using namespace std;

class ClasseA{//classe que eu quero modificar
    public:
    int num;
    ClasseA(int n){
        num = n;
    }
    ClasseA(){
        num = 0;
    }    
    
};

class ClasseB:public ClasseA{
    public:
    ClasseB(ClasseA objA){
        objA.num = 7;
    }
};

int main(){
    ClasseA sabao(5);
    ClasseB room(sabao);
    cout<<sabao.num<<endl; //esperando sair 7
    return 0;
}
