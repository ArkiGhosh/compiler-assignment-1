#include<bits/stdc++.h>

using namespace std;


stack<int> st;

void starTransition(int state, vector<vector<char>> &transitions){
    transitions[state][state + 1] = 'e';
    int prevLeft = st.top();
    transitions[state][prevLeft] = 'e'; 
    transitions[prevLeft][state + 1] = 'e';
}

void plusTransition(int state, vector<vector<char>> &transitions){
    transitions[state][state + 1] = 'e';
    int prevLeft = st.top();
    transitions[state][prevLeft] = 'e';
}

int pipeTransition(int state, string &s, vector<vector<char>> &transitions){
    stack<int> stAux;
    for(int j = state; j < transitions.size(); j++){
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

void normalTransition(int state, char c, vector<vector<char>> &transitions){
    transitions[state][state + 1] = c;
}

int dfs(int state, int index, string &s, vector<vector<char>> &transitions){
    if (state + 1 == (int)transitions.size()) return 0;
    int check = 0, length = 0;
    for(int i = 0; i < transitions.size(); i++){
        if (transitions[state][i] == 'e'){
            check++;
            int next = dfs(i, index, s, transitions);
            if (next < 0){
                check--;
            }
            else{
                length = max(length, next);
            }
        }
        else if (transitions[state][i] == 'a' && s[index] == 'a'){
            check++;
            int next = dfs(i, index + 1, s, transitions);
            if (next < 0){
                check--;
            }
            else{
                length = max(length, 1 + next);
            }
        }
        else if (transitions[state][i] == 'b' && s[index] == 'b'){
            check++;
            int next = dfs(i, index + 1, s, transitions);
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

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int t;
    cin >> t;
    vector<vector<vector<char>>> allTransitions;
    string re, s;

    while(t--){
        cin >> re;
        vector<vector<char>> transitions;
        int N = re.size();
        transitions.assign(N, vector<char>(N, ' '));
        // ' ' means no transition has been defined
        // The start state is always the first bracket -> 0
        
        for(int i = 0; i < N; i++){
            if (re[i] == '('){
                if (re[i + 1] == '(' | re[i + 1] == ')') normalTransition(i, 'e', transitions);
                else normalTransition(i, re[i + 1], transitions);
                st.push(i);
            }
            else{
                if (re[i] == ')'){
                    if (i + 1 < N){
                        if (re[i + 1] == '*'){
                            normalTransition(i, 'e', transitions);
                            starTransition(i + 1, transitions);
                            st.pop();
                        }
                        else if (re[i + 1] == '+'){
                            normalTransition(i, 'e', transitions);
                            plusTransition(i + 1, transitions);
                            st.pop();
                        }
                        else if (re[i + 1] == '|'){
                            st.pop();
                            int left = st.top();
                            transitions[left][i + 2] = 'e';
                            int right = pipeTransition(i + 2, re, transitions);
                            transitions[i][right] = 'e';
                        }
                        else{
                            st.pop();
                            normalTransition(i, 'e', transitions);
                        } 
                    }
                }
                else if (re[i] == 'a' | re[i] == 'b') normalTransition(i, 'e', transitions);
                else continue;
            }
        }

        allTransitions.push_back(transitions);
    }
    
    cin >> s;
    int i = 0;
    int len;
    string ans = "";
    int maxlen, match;
    while(i < (int)s.size()){
        maxlen = INT_MIN;
        for(int j = 0; j < (int)allTransitions.size(); j++){
            len = dfs(0, i, s, allTransitions[j]);
            if (len > maxlen){
                match = j + 1;
                maxlen = len;
            }
        }
        if (maxlen <= 0){
            ans += "@";
            ans.push_back(s[i]);
            i++;
        }
        else{
            char c = match + 48;
            ans += "$";
            ans.push_back(c);
            i += maxlen;
        }
    }
    ans.push_back('#');
    cout << ans << endl;
}
