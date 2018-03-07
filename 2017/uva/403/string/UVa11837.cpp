/* UVa problem: 11837 Musical Plagiarism
 *
 * Topic: string
 *
 * Level: mandatory, nontrivial
 * 
 * Brief problem description: 
 *
 *   Find the substring 
 *
 * Solution Summary:
 *
 *   convert semitones and normalize them
 *
 * Used Resources:
 *
 *   cp3
 *
 * I hereby certify that I have produced the following solution myself 
 * using the resources listed above in accordance with the CMPUT 403 
 * collaboration policy.
 *
 * --- Dennis Truong
 */
 #include <cstdio>
 #include <iostream>
 #include <vector>
 #include <map>
 #include <cstring>
 using namespace std;
map<string,char> notes;
int NUM = 12;
void read(vector<int> & v, int n) {
    int p,c;
    string s;
    cin >> s;
    p = notes[s];
    for (int i = 1; i < n; i++) {
        cin >> s; c = notes[s];
        v.push_back((NUM + c - p) % NUM);
        p = c;
    }
}
string tostring(vector<int> v) {
    string s;
    for (auto i: v) {
        s += 'a'+i;
    }
    return s;
}
 int main() {
    notes["G#"] = notes["Ab"] = 0;
    notes["A"] = 1;
    notes["A#"] = notes["Bb"] = 2;
    notes["B"] = notes["Cb"] = 3;
    notes["B#"] = notes["C"] = 4;
    notes["C#"] = notes["Db"] = 5;
    notes["D"] = 6;
    notes["D#"] = notes["Eb"] = 7;
    notes["E"] = notes["Fb"] = 8;
    notes["E#"] = notes["F"] = 9;
    notes["F#"] = notes["Gb"] = 10;
    notes["G"] = 11;
     int m,t;
     while(cin >> m >> t,m) {
        vector<int> song, plag;
        read(song,m);
        read(plag,t);
        string s = tostring(song),
            p = tostring(plag);
        size_t nPos = s.find(p,0);
        if (nPos == string::npos) {
            cout << "N" << endl;
        } else {
            cout << "S" << endl;
        }
     }

     return 0;
 }