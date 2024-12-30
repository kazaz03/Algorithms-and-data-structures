#include <iostream>

template<typename Tip>
void shellSort(Tip *niz, int vel, int *H, int hVel)
{
    for(int i=0; i<hVel; i++)
    {
        int h=H[i];
        for(int j=h; j<vel; j++)
        {
            int priv=niz[j];
            int k=j-h;
            while(k>=0 && niz[k]>priv)
            {
                niz[k+h]=niz[k];
                k=k-h;
            }
            niz[k+h]=priv;
        }
    }
}

int main()
{
    int *niz=new int[6];
    for(int i=0; i<6; i++)
    {
        niz[i]=int(std::rand()%100);
    }
    std::cout<<"Niz glasi: ";
    for(int i=0; i<6; i++)
    {
        std::cout<<niz[i]<<" ";
    }
    std::cout<<std::endl;
    int *h=new int[3];
    h[0]=3; h[1]=2; h[2]=1;
    shellSort(niz,6,h,3);
    std::cout<<"Sortiran niz glasi: ";
    for(int i=0; i<6; i++)
    {
        std::cout<<niz[i]<<" ";
    }
    return 0;
}