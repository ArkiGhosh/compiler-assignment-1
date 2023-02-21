#include<bits/stdc++.h>

using namespace std;

vector<vector<char>> transitions;
stack<int> st;
int N;
int FINAL_STATE;

void starTransition(int state){
    transitions[state][state + 1] = 'e';
    int prevLeft = st.top();
    transitions[state][prevLeft] = 'e'; 
    transitions[prevLeft][state + 1] = 'e';
}

void plusTransition(int state){
    transitions[state][state + 1] = 'e';
    int prevLeft = st.top();
    transitions[state][prevLeft] = 'e';
}

int pipeTransition(int state, string &s){
    stack<int> stAux;
    for(int j = state; j < N; j++){
        if (s[j] == '(') stAux.push(j);
        else if (s[j] == ')'){
            if (stAux.empty()){
                return j;
            }
            else{
                stAux.pop();
            }
        }
    }
    return 0;
}

void normalTransition(int state, char c){
    transitions[state][state + 1] = c;
}

int dfs(int state, int index, string &s){
    if (state == FINAL_STATE) return 0;
    int check = 0, length = 0;
    for(int i = 0; i < N; i++){
        if (transitions[state][i] == 'e'){
            check++;
            int next = dfs(i, index, s);
            if (next < 0){
                check--;
            }
            else{
                length = max(length, next);
            }
        }
        else if (transitions[state][i] == 'a' && s[index] == 'a'){
            check++;
            int next = dfs(i, index + 1, s);
            if (next < 0){
                check--;
            }
            else{
                length = max(length, 1 + next);
            }
        }
        else if (transitions[state][i] == 'b' && s[index] == 'b'){
            check++;
            int next = dfs(i, index + 1, s);
            if (next < 0){
                check--;
            }
            else{
                length = max(length, 1 + next);
            }
        }
    }
    if (check <= 0) return INT_MIN;
    return length;

}
int main(){

    string re, s;
    cin >> re >> s;

    N = re.size();
    FINAL_STATE = N - 1;
    transitions.assign(N, vector<char>(N, ' '));
    // ' ' means no transition has been defined
    // The start state is always the first bracket -> 0
    
    for(int i = 0; i < N; i++){
        if (re[i] == '('){
            if (re[i + 1] == '(' | re[i + 1] == ')') normalTransition(i, 'e');
            else normalTransition(i, re[i + 1]);
            st.push(i);
        }
        else{
            if (re[i] == ')'){
                if (i + 1 < N){
                    if (re[i + 1] == '*'){
                        normalTransition(i, 'e');
                        starTransition(i + 1);
                        st.pop();
                    }
                    else if (re[i + 1] == '+'){
                        normalTransition(i, 'e');
                        plusTransition(i + 1);
                        st.pop();
                    }
                    else if (re[i + 1] == '|'){
                        st.pop();
                        int left = st.top();
                        transitions[left][i + 2] = 'e';
                        int right = pipeTransition(i + 2, re);
                        transitions[i][right] = 'e';
                    }
                    else{
                        st.pop();
                        normalTransition(i, 'e');
                    } 
                }
            }
            else if (re[i] == 'a' | re[i] == 'b') normalTransition(i, 'e');
            else continue;
        }
    }
    int i = 0;
    int len;
    string ans = "";
    while(i < (int)s.size()){
        len = dfs(0, i, s);
        if (len <= 0){
            ans += '@';
            ans += s[i];
            i++;
        }
        else{
            ans += '$';
            for(int j = i; j < len + i; j++) ans += s[j];
            i += len;
        }
    }
    ans += '#';
    cout << ans << endl;
}
