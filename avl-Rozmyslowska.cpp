#include<iostream>
#include<string>
#include<fstream>
using namespace std;

class AVL
{
    ofstream plik;

    struct node
    {
        string data;
        int height = 0;
        node* left = NULL;
        node* right = NULL;
        node* translation = NULL;
    };

    node* root;

    node* insert(string s, node* t)
    {
        if(t == NULL)
        {
            t = new node;
            t->data = s;
            WhereIAm = t;
            return t;
        }
        else if(s < t->data)
        {
            t->left = insert(s, t->left);

            if(height(t->left) - height(t->right) == 2)
            {
                if(s < t->left->data)
                    t = RightRotate(t);
                else
                    t = LeftRightRotate(t);
            }
        }
        else if(s > t->data)
        {
            t->right = insert(s, t->right);

            if(height(t->right) - height(t->left) == 2)
            {
                if(s > t->right->data)
                    t = LeftRotate(t);
                else
                    t = RightLeftRotate(t);
            }
        }

        t->height = max(height(t->left), height(t->right))+1;
        return t;
    }

    node* RightRotate(node* &t)  //left left case
    {
        node* u = t->left;
        t->left = u->right;
        u->right = t;
        t->height = max(height(t->left), height(t->right))+1;
        u->height = max(height(u->left), t->height)+1;
        return u;
    }

    node* LeftRotate(node* &t)  //right right case
    {
        node* u = t->right;
        t->right = u->left;
        u->left = t;
        t->height = max(height(t->left), height(t->right))+1;
        u->height = max(height(t->right), t->height)+1 ;
        return u;
    }

    node* RightLeftRotate(node* &t)
    {
        t->right = RightRotate(t->right);
        return LeftRotate(t);
    }

    node* LeftRightRotate(node* &t)
    {
        t->left = LeftRotate(t->left);
        return RightRotate(t);
    }

    node* remove(string s, node* t)
    {
        node* temp;

        if(t == NULL)
            return NULL;

        else if(s < t->data)
            t->left = remove(s, t->left);
        else if(s > t->data)
            t->right = remove(s, t->right);

        else if(t->left && t->right)
        {
            temp = szukajNastepnik(t->right);
            t->data = temp->data;
            t->right = remove(t->data, t->right);
        }
        else
        {
            temp = t;
            if(t->left == NULL)
                t = t->right;
            else if(t->right == NULL)
                t = t->left;
            delete temp;
        }
        if(t == NULL)
            return t;

        t->height = max(height(t->left), height(t->right))+1;

        if(height(t->left) - height(t->right) == 2)
        {
            if(height(t->left->left) - height(t->left->right) == 1)
                return LeftRotate(t);
            else
                return RightLeftRotate(t);
        }
        else if(height(t->right) - height(t->left) == 2)
        {
            if(height(t->right->right) - height(t->right->left) == 1)
                return RightRotate(t);
            else
                return LeftRightRotate(t);
        }
        return t;
    }

    int height(node* t)
    {
        if(t == NULL)
            return -1;
        else
            return t->height;
    }

    void KLP(node* t)
    {
        if(t == NULL)
            return;

        if(k==1) plik << endl;
        k=1;
        plik << t->data << " " << t->translation->data;

        KLP(t->left);
        KLP(t->right);
    }

    node* szukajNastepnik(node* t)
    {
        if(t == NULL)
            return NULL;

        else if(t->left == NULL)
            return t;

        else
            return szukajNastepnik(t->left);
    }

public:
    node* pom,* WhereIAm;
    int k=0;

    AVL()
    {
        root = NULL;
    }

    void insert(string s)
    {
        root = insert(s, root);
    }

    void remove(string s)
    {
        root = remove(s, root);
    }

    void WypiszDoPliku()
    {
        plik.open("plik.txt");
        KLP(root);
        k=0;
        plik.close();
    }

    node* find(string s)
    {
        node* t = root;

        while(t->data != s)
        {
            if(s < t->data) t = t->left;
            else t = t->right;

            if(t == NULL)
                break;
        }
        return t;
    }
};

class dictionary
{
    AVL p,a;

public:
    void wstawP(string s1, string s2)
    {
        p.insert(s1);
        a.insert(s2);
        p.WhereIAm->translation = a.WhereIAm;
        a.WhereIAm->translation = p.WhereIAm;
    }

    void wstawA(string s2, string s1)
    {
        p.insert(s1);
        a.insert(s2);
        p.WhereIAm->translation = a.WhereIAm;
        a.WhereIAm->translation = p.WhereIAm;
    }

    void wyszukajP(string s)
    {
        p.pom = p.find(s);
        if(p.pom == NULL) cout<<"Nie ma takiego slowa."<<endl;
        else cout<<p.pom->translation->data<<endl;
    }

    void wyszukajA(string s)
    {
        a.pom = a.find(s);
        if(a.pom == NULL) cout<<"Nie ma takiego slowa."<<endl;
        else cout<<a.pom->translation->data<<endl;
    }

    void usunP(string s)
    {
        p.pom = p.find(s);
        if(p.pom)
        {
            p.remove(s);
            a.remove(p.pom->translation->data);
        }
    }
    void usunA(string s)
    {
        a.pom = a.find(s);
        if(a.pom)
        {
            a.remove(s);
            p.remove(a.pom->translation->data);
        }
    }
    void zapisz()
    {
        a.WypiszDoPliku();
    }

    void wczytaj()
    {
        fstream plik;
        plik.open("plik.txt");

        while(!plik.eof())
        {
            string p,a;
            plik>>a>>p;
            wstawP(p,a);
        }
        plik.close();
    }
};

int main()
{
    dictionary d;

    int x;
    string s1,s2;
    string s;

    while(1)
    {
        cin>>x;

        switch (x)
        {

        case 1: cin>>s1>>s2;
            d.wstawA(s1,s2);break;

        case 2: cin>>s1>>s2;
            d.wstawP(s1,s2);break;

        case 3: cin>>s;
            d.usunA(s);break;

        case 4: cin>>s;
            d.usunP(s);break;

        case 5: cin>>s;
            d.wyszukajA(s);break;

        case 6: cin>>s;
            d.wyszukajP(s);break;

        case 7:
            d.wczytaj();break;

        case 8:
            d.zapisz();break;
        }
    }
}
