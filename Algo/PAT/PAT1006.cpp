#include<bits/stdc++.h>
using namespace std;

//me1字符串比较 me2 转int比较

/*me 1
int main(){
    char id[20],in[20],out[20];
    char fst[20] = { "24:00:00" };
    char last[20] = { "00:00:00" };
    char lkman[20], unlkman[20];
    
    int n,i,j;
    cin>>n;
    
    for(i=0;i<n;i++){
        cin>>id>>in>>out;
        if(strcmp(out,last)>0){//out -last
            strcpy(last,out);//out->last
            strcpy(lkman,id);
        }
        if(strcmp(in,fst)<0){
            strcpy(fst,in);
            strcpy(unlkman,id);
        }
    }

    cout<<unlkman<<" "<<lkman;
    system("pause");
    return 0;
}
*/

//me 2 
int main() {
    int n, minn = INT_MAX, maxn = INT_MIN;
    scanf("%d", &n);
    string unlocked, locked;
    for(int i = 0; i < n; i++) {
        string t;
        cin >> t;
        int h1, m1, s1, h2, m2, s2;
        scanf("%d:%d:%d %d:%d:%d", &h1, &m1, &s1, &h2, &m2, &s2);
        int tempIn = h1 * 3600 + m1 * 60 + s1;
        int tempOut = h2 * 3600 + m2 * 60 + s2;
        if (tempIn < minn) {
            minn = tempIn;
            unlocked = t;
        }
        if (tempOut > maxn) {
            maxn = tempOut;
            locked = t;
        }
    }
    cout << unlocked << " " << locked;
    return 0;
}