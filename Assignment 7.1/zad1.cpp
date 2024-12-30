#include <iostream>
#include <fstream>

void modifikovani_bubble_sort(int *niz, int vel)
{
    if(niz==nullptr) return;
    int flag=1;
    for(int i=0; i<vel && flag==1; i++)
    {
        flag=0;
        for(int j=0; j<vel-1-i; j++)
        {
            if(niz[j]>niz[j+1])
            {
                std::swap(niz[j],niz[j+1]);
                flag=1;
            }
        }
    }
}

void generisiZaBubble(std::string filename, int vel)
{
    std::ofstream ulaz(filename);
    for(int i=0; i<vel; i++)
    {
        ulaz<<int(std::rand())<<' ';
    }
    ulaz.close();
}

int main()
{
    generisiZaBubble("brojevi.txt",10);
    int *niz=nullptr;
    int vel=10;
    niz=new int[vel];
    std::ifstream izlaz("brojevi.txt");
    int broj; int brojac=0;
    while(izlaz>>broj)
    {
        niz[brojac]=broj;
        brojac++;
    }
    izlaz.close();
    std::cout<<"Niz glasi: ";
    for(int i=0; i<vel; i++)
    {
        std::cout<<niz[i]<<" ";
    }
    std::cout<<std::endl;
    modifikovani_bubble_sort(niz,vel);
    std::cout<<"Sortiran niz glasi: ";
    for(int i=0; i<vel; i++)
    {
        std::cout<<niz[i]<<" ";
    }
    return 0;
}