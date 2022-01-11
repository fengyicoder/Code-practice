
#include <unordered_map>
#include <stack>

using namespace std;

struct FreqStack {
    int maxFreq;
    unordered_map<int, int> valToFreq;
    unordered_map<int, stack<int> > freqToVals; 

    void push(int val) {
        int freq = valToFreq.count(val) ? 0 : valToFreq[val];
        freq++;
        valToFreq[val] = freq;
        freqToVals.insert(make_pair(freq, stack<int>()));
        freqToVals[freq].push(val);
        maxFreq = max(maxFreq, freq);
    }

    int pop() {
        stack<int> vals = freqToVals[maxFreq];
        int val = vals.top();
        vals.pop();
        int freq = valToFreq[val] - 1;
        valToFreq[val] = freq;
        if (vals.empty()) maxFreq--;
        return val;
    }
};