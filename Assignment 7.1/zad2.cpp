#include <iostream>

template<typename Tip> 
void insertionSort(Tip *niz, int vel)
{
    if(niz==nullptr) return;
    for(int i=1; i<vel; i++)
    {
        Tip el=niz[i];
        int j=i-1;
        while(j>=0 && niz[j]>el)
        {
            niz[j+1]=niz[j];
            j--;
        }
        niz[j+1]=el;
    }
}

int main()
{
    int *niz=new int[6];
    for(int i=0; i<6; i++)
    {
        niz[i]=int(std::rand()%100);
    }
    insertionSort(niz,6);
    for(int i=0; i<6; i++)
    {
        std::cout<<niz[i]<<" ";
    }
    return 0;
}